//---------------------------------------------------------------------------

#pragma hdrstop

#include "CbPercAgentNegotiation.h"

#include "CbPercussionistAgent.h"
#include "CbFacade.h"

#include "FAMA\FmMessageTemplate.h"

#include "Util\MathUtil.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;
//---------------------------------------------------------------------------


//...............................//
// CLASSE CbPercAgentNegotiation
//...............................//

CbPercAgentNegotiation::CbPercAgentNegotiation(CbPercussionistAgent* ag, const CbRhythmicRole* __role, int barIndex)
	: FmBehaviour(ag)
{
	this->role = __role;
	this->bar = barIndex;
	sprintf(this->barStr, "%d", barIndex);

	char strConversationId[30];
	sprintf(strConversationId, "main_flow_%d", barIndex);

	char strInReplyTo[30];
	sprintf(strInReplyTo, "sec_flow_%d", barIndex);

	this->onlyNegotiationMessages = FmMessageTemplate::matchConversationId(strConversationId);
	this->onlyRepliesToRequests = FmMessageTemplate::matchInReplyTo(strInReplyTo);

	this->state = REQ_ROLES;

	// acoxambre !
    if (strcmp(this->role->getName(), "solo") == 0) {
		ag->setIntensityScale(1.4);
	}
}

CbPercAgentNegotiation::~CbPercAgentNegotiation() {
	delete onlyRepliesToRequests;
	delete onlyNegotiationMessages;

	for (int i = agentInfos.size() - 1; i >= 0; i --) {
		delete agentInfos[i];
	}

	MapAgentPattern::iterator it;
	for (it = referencePatterns.begin(); it != referencePatterns.end(); it ++) {
		if (it->second != NULL) {
			delete it->second;
			it->second = NULL;
		}
	}
}

CbPercussionistAgent* CbPercAgentNegotiation::getPercAgent() {
	return static_cast<CbPercussionistAgent*>(myAgent);
}

std::ofstream& CbPercAgentNegotiation::log(bool prompt) {
	ofstream* ofs = getPercAgent()->getLogStream(prompt);
	if (prompt) {
		(*ofs) << "{HNEG, " << this->state << "} - ";
	}
	return *ofs;
}

void CbPercAgentNegotiation::addAgInfo(AID id) {
	agentInfos.push_back(new AgentInfo(id));
}

CbPercAgentNegotiation::AgentInfo* CbPercAgentNegotiation::getAgInfo(int i) {
	return agentInfos[i];
}

CbPercAgentNegotiation::AgentInfo* CbPercAgentNegotiation::getAgInfo(AID id) {
	AgentInfo* agi = NULL;
	for (int i = agentInfos.size() - 1; i >= 0; i --) {
		if ( (*id) == (*(agentInfos[i]->id)) ) {
			agi = agentInfos[i];
			break;
		}
	}
	return agi;
}

int CbPercAgentNegotiation::getNumAgInfos() {
	return agentInfos.size();
}

CbPercAgentNegotiation::AgentInfo* CbPercAgentNegotiation::getAgInfoNotFixed(int index) {
	AgentInfo* result = NULL;
	int relIndex = -1;

	for (int i = 0; i < agentInfos.size(); i ++) {
		if (agentInfos[i]->fixed == false)  {
			relIndex ++;
		}
		if (relIndex == index) {
			result = agentInfos[i];
			break;
		}
	}
	return result;
}

int CbPercAgentNegotiation::getNumAgInfosNotFixed() {
	int num = 0;
	for (int i = 0; i < agentInfos.size(); i ++) {
		if (agentInfos[i]->fixed == false) num ++;
	}
	return num;
}

void CbPercAgentNegotiation::setCurrentPattern(int roleIndex) {
	CbPercussionistAgent* ag = getPercAgent();

	int patternMainIndex = ag->repertoire->getMainIndex(this->role->getName(), roleIndex);
	int patternNumBars = ag->repertoire->getRhythmPattern(patternMainIndex)->getNumBars();
	int patternBar = this->bar % patternNumBars;

	log() << "Padrao preliminar: " << roleIndex;
	ag->history.set(this->bar, this->role, NULL, patternMainIndex, patternBar);
}

int CbPercAgentNegotiation::getCurrentPattern() {
	CbPercussionistAgent* ag = getPercAgent();
	int currentPattern = -1;

	const CbPercPerfInfo* info = ag->history.get(this->bar);

	if (info != NULL) {
		// get the secondary index (index by role)
		currentPattern = ag->repertoire->getRoleIndex(this->role->getName(), info->patternIndex);
	}

	return currentPattern;
}

void CbPercAgentNegotiation::clearCurrentPattern() {
	getPercAgent()->history.erase(this->bar);
}

int CbPercAgentNegotiation::getCurrentPatternMainIndex() {
	const CbPercPerfInfo* info = getPercAgent()->history.get(this->bar);
	return info->patternIndex;
}

int CbPercAgentNegotiation::getCurrentPatternBar() {
	const CbPercPerfInfo* info = getPercAgent()->history.get(this->bar);
	return info->patternBar;
}

void CbPercAgentNegotiation::commitCurrentPattern() {
	CbPercussionistAgent* ag = getPercAgent();
	const CbPercPerfInfo* info = getPercAgent()->history.get(this->bar);

	const CbRpInfo* rpinfo = ag->repertoire->getRhythmPattern(info->patternIndex)->getInfo();
	log() << "Padrao Escolhido: \"" << rpinfo->getName() << "\" (main-index:" << info->patternIndex
		  << ", role-index:" << ag->repertoire->getRoleIndex(this->role->getName(), info->patternIndex) << ")";

	// FALTA: setar AID da referencia (so um? talvez deva remover esse atributo... e adicionar mais contexto...)
	ag->history.set(this->bar, this->role, NULL, info->patternIndex, info->patternBar);
}

void CbPercAgentNegotiation::action() {

	switch (this->state) {
	case REQ_ROLES:
		actionRequestRoles();
		break;
	case WAIT_ROLES:
		actionWaitForRoles();
		break;
	case CHOOSE_1ST_PTRN:
		actionChooseFirstPattern();
		break;
	case REF_SYNC:
		actionSynchronizeWithReference();
		break;
	case CHOOSE_PTRN:
		actionChoosePattern();
		break;
	case REQ_PATRN_REF:
		actionRequestPatternsRef();
		break;
	case WAIT_PTRN_REF:
		actionWaitForPatternsRef();
		break;
	case REQ_PTRN:
		actionRequestPatterns();
		break;
	case WAIT_PTRN:
		actionWaitForPatterns();
		break;
	case SEND_RESULT:
		actionSendEvaluationResult();
		break;
	case CMP_RESULTS:
		actionCompareResults();
		break;
	case SEND_PTRN_MXR:
		actionSendPatternToMixer();
		break;
	case FINISHED:
		break;
	}

}

void CbPercAgentNegotiation::actionRequestRoles() {
	char reqRoleStr[20];
	sprintf(reqRoleStr, "role(%d)", this->bar);

	FmAclMessage reqRoles (FP_REQUEST);
	reqRoles.setContent(reqRoleStr);
	reqRoles.setReplyWith(this->onlyRepliesToRequests->getInReplyTo());

	Vector_AID band;
	getPercAgent()->getMusiciansIds(band);

	for (int i = 0; i < band.size(); i ++) {
		reqRoles.addReceiver(band[i]);
	}

	this->pendingAnswers = band.size();

	log() << "Requesting roles to " << this->pendingAnswers << " agents. ";
	myAgent->send(reqRoles);

	this->state = WAIT_ROLES;
}

void CbPercAgentNegotiation::actionWaitForRoles() {
	FmAclMessage *rec = myAgent->receive(*onlyRepliesToRequests);

	if (rec != NULL) {

		if (rec->getPerformative() == FP_INFORM) {
			string receivedRoleName = rec->getContent();
			const CbRhythmicRole* receivedRole = CbFacade::getInstance()->getRole(receivedRoleName.c_str());

			log() << "A role was received: \"" << receivedRoleName << "\"";
			this->pendingAnswers --;

			if (receivedRoleName == this->role->getName()) {
				// adiciona agente do mesmo papel
				this->addAgInfo(rec->getSender());

			} else if (receivedRole->isReferenceTo(*this->role)) {
				// adiciona agente do papel de referencia
				this->referencePatterns[rec->getSender()] = NULL;

			} else if (this->role->isReferenceTo(*receivedRole)) {
				// adiciona agente para o qual 'this->role' é referencia
				this->agentsToNotify.push_back(rec->getSender());

			}

		}

		delete rec;
	}

	if (this->pendingAnswers == 0) {
		log() << "All roles were sent/received !";
		this->state = CHOOSE_1ST_PTRN;
	}

}

void CbPercAgentNegotiation::actionChooseFirstPattern() {
	CbPercussionistAgent *ag = getPercAgent();

	const CbPercPerfInfo* perfInfo = ag->history.searchBackward(this->role->getName());

	if (strcmp(this->role->getName(),"livre") == 0) {
		if (perfInfo == NULL) {
			log() << "Just choosing randomly...";
			int patternIndex = randx() % ag->repertoire->getNumRhythmPatterns("livre");
			this->setCurrentPattern(patternIndex);
		} else {
			log() << "Repeating last random choice...";
			this->setCurrentPattern(ag->repertoire->getRoleIndex(this->role->getName(), perfInfo->patternIndex));
		}

		this->state = SEND_PTRN_MXR;
		return;
	}


	if (perfInfo == NULL) {
		log() << "There's no previous choice for this role...";

		if (strcmp(this->role->getName(),"base") == 0
				|| strcmp(this->role->getName(),"base-complementar") == 0
				|| strcmp(this->role->getName(),"uniforme") == 0) {
			log() << "Choosing randomly and reordering using the pattern chosen..." << endl;

			// escolhe o primeiro padrao randomicamente
			int firstPatternIndex = randx() % ag->repertoire->getNumRhythmPatterns(this->role->getName());
			const CbRhythmPattern* firstPattern = ag->repertoire->getRhythmPattern(firstPatternIndex);

			// ordena a base por ele
			ag->repertoire->sort(this->role->getName(), AVC_CHRONOT_DIST, firstPattern);

		} else {
			log() << "Ordering randomly...";
			ag->repertoire->sort(this->role->getName(), CbPercussionistAgent::random_eval);

		}

		this->setCurrentPattern(0); // o primeiro padrao do papel

	} else {
		log() << "Previous choice found!";
		this->setCurrentPattern(ag->repertoire->getRoleIndex(this->role->getName(), perfInfo->patternIndex));

	}


	// para esperar as respostas dos agentes do papel de referencia
	this->pendingAnswers = this->referencePatterns.size();

	this->state = REF_SYNC;
}

void CbPercAgentNegotiation::actionSynchronizeWithReference() {
	FmAclMessage* acl = myAgent->receive(*onlyNegotiationMessages);

	if (acl != NULL) {
		if (acl->getPerformative() == FP_INFORM) {
			log() << "Received notification from one reference.";
			this->pendingAnswers --;
		} else {
			log() << acl->getContent();
		}
	}

	// ja recebeu todas as respostas
	if (this->pendingAnswers == 0) {
		log() << "All notifications from references were received !";
		this->state = REQ_PTRN;
	}

}

void CbPercAgentNegotiation::actionChoosePattern() {
	double eval, bestEval = 35000.00;
	int bestPattern = -1;

	CbPercussionistAgent *ag = getPercAgent();

	int currentPattern = this->getCurrentPattern();

	// para evitar que, antes de terminar esse processo de escolha,
	// um agente que pedir o padrao receba o antigo
	this->clearCurrentPattern();

	// testa uma janela de 20 padrões
	int startIndex = MAX(currentPattern - 10, 0);
	int endIndex = MIN(startIndex + 20, ag->repertoire->getNumRhythmPatterns(this->role->getName()));

	log() << "Choosing pattern...";
	for (int i = startIndex; i < endIndex; i ++) {
		eval = this->evaluatePattern(ag->repertoire->getMainIndex(this->role->getName(), i));

		if (eval < bestEval) {
			bestPattern = i;
			bestEval = eval;
		}
	}

	log() << "Pattern chosen: " << ag->repertoire->getRhythmPattern(this->role->getName(), bestPattern)->getInfo()->getName();
	this->setCurrentPattern(bestPattern);

	this->state = REQ_PTRN;
}

double CbPercAgentNegotiation::evaluatePattern(int patrnMainIndex) {
	CbPercussionistAgent* agent = getPercAgent();
	const CbRhythmPattern* currPtrn = agent->repertoire->getRhythmPattern(patrnMainIndex);
	double eval;

	double historicalEvaluation = this->role->evaluateMatchHistory(*currPtrn, &(agent->history), agent->repertoire, this->bar);
	log () << "Historical-Evaluation: " << historicalEvaluation;

	double horizontalEvaluation = 0.0;

	for (int i = getNumAgInfos() - 1; i >= 0; i --) {
		eval = this->role->evaluateMatch(*currPtrn, *(getAgInfo(i)->pattern));
		//log () << "  >> Partial Horizontal-Evaluation: " << eval;
		horizontalEvaluation += eval;
	}

	if (getNumAgInfos() != 0) {
		horizontalEvaluation = horizontalEvaluation / getNumAgInfos();
	}
	log () << "Horizontal-Evaluation: " << horizontalEvaluation;

	double verticalEvaluation = 0;
	MapAgentPattern::iterator it;

	for (it = referencePatterns.begin(); it != referencePatterns.end(); it ++) {
		eval = this->role->evaluateMatchRef(*currPtrn, *(it->second));
		//log () << "  >> Partial Vertical-Evaluation: " << eval;
		verticalEvaluation += eval;
	}

	if (referencePatterns.size() != 0) {
		verticalEvaluation = verticalEvaluation / referencePatterns.size();
	}
	log () << "Vertical-Evaluation: " << verticalEvaluation;

	return historicalEvaluation
		   + (agent->getHorizontalEvaluationWeigth() * horizontalEvaluation)
		   + (agent->getVerticalEvaluationWeigth() * verticalEvaluation);
}

void CbPercAgentNegotiation::actionRequestPatterns() {
	char reqPatternStr[20];
	sprintf(reqPatternStr, "pattern(%d)", this->bar);

	FmAclMessage reqRoles (FP_REQUEST);
	reqRoles.setContent(reqPatternStr);
	reqRoles.setReplyWith(this->onlyRepliesToRequests->getInReplyTo().c_str());

	for (int i = this->getNumAgInfosNotFixed() - 1; i >= 0; i --) {
		reqRoles.addReceiver(this->getAgInfoNotFixed(i)->id);
	}

	// para receber os padroes de todos os agentes
	this->pendingAnswers = this->getNumAgInfosNotFixed();

	log() << "Requesting patterns to " << this->pendingAnswers << " agents !";
	myAgent->send(reqRoles);

	this->state = WAIT_PTRN;
}

void CbPercAgentNegotiation::actionWaitForPatterns() {
	FmAclMessage* acl = myAgent->receive(*onlyRepliesToRequests);

	if (acl != NULL) {
		CbPercussionistAgent* ag = getPercAgent();

		if (acl->getPerformative() == FP_INFORM) {
			log() << "A pattern was received !";
			AgentInfo* info = getAgInfo(acl->getSender());
			if (info->pattern == NULL) {
				delete info->pattern;
			}
			info->pattern = ag->readPatternFromMessage(acl);
			this->pendingAnswers --;

		}
	}

	// ja recebeu todas as frases
	if (this->pendingAnswers == 0) {
		log() << "All patterns were received/sent !";
		this->state = REQ_PATRN_REF;
	}
}

void CbPercAgentNegotiation::actionRequestPatternsRef() {
	char reqPatternStr[20];
	sprintf(reqPatternStr, "pattern(%d)", this->bar);

	FmAclMessage reqRoles (FP_REQUEST);
	reqRoles.setContent(reqPatternStr);
	reqRoles.setReplyWith(this->onlyRepliesToRequests->getInReplyTo().c_str());

	MapAgentPattern::iterator it;
	for (it = referencePatterns.begin(); it != referencePatterns.end(); it ++) {
		reqRoles.addReceiver(it->first);
	}

	// para receber os padroes de todos os agentes
	this->pendingAnswers = this->referencePatterns.size();

	log() << "Requesting patterns to " << this->pendingAnswers << " REFERENCE agents !";
	myAgent->send(reqRoles);

	this->state = WAIT_PTRN_REF;
}

void CbPercAgentNegotiation::actionWaitForPatternsRef() {
	FmAclMessage* acl = myAgent->receive(*onlyRepliesToRequests);

	if (acl != NULL) {
		CbPercussionistAgent* ag = getPercAgent();

		if (acl->getPerformative() == FP_INFORM) {
			if (this->referencePatterns[acl->getSender()] != NULL) {
				delete referencePatterns[acl->getSender()];
			}

			this->referencePatterns[acl->getSender()] = ag->readPatternFromMessage(acl);
			this->pendingAnswers --;
		}
	}

	// ja recebeu todas as frases
	if (this->pendingAnswers == 0) {
		log() << "All reference patterns were received !";
		this->state = SEND_RESULT;
	}
}

void CbPercAgentNegotiation::actionSendEvaluationResult() {
	FmAclMessage informResult (FP_INFORM);
	char messageContent[10];

	this->evaluation = this->evaluatePattern(this->getCurrentPatternMainIndex());
	this->bestEvaluation = this->evaluation;

	sprintf(messageContent, "result=%lf", this->evaluation);

	informResult.setContent(messageContent);
	informResult.setConversationId(onlyNegotiationMessages->getConversationId());

	for (int i = this->getNumAgInfosNotFixed() - 1; i >= 0; i --) {
		informResult.addReceiver(this->getAgInfoNotFixed(i)->id);
	}

	log() << "Sendind evaluation: " << this->evaluation;
	myAgent->send(informResult);

	// para aguardar o recebimento dos valores de avaliação dos outros
	this->pendingAnswers = this->getNumAgInfosNotFixed();

	this->state = CMP_RESULTS;
}

void CbPercAgentNegotiation::actionCompareResults() {
	FmAclMessage* acl = myAgent->receive(*onlyNegotiationMessages);

	AgentInfo *info;
	double agentEval;

	if (acl != NULL) {
		sscanf(acl->getContent().c_str(), "result=%lf", &agentEval);
		log() << "Received evaluation: " << agentEval;

		info = this->getAgInfo(acl->getSender());

		info->evaluation = agentEval;
		if (LESS(agentEval, this->bestEvaluation) == true) {
			this->bestEvaluation = agentEval;
		}

		this->pendingAnswers --;

		delete acl;
	}

	if (this->pendingAnswers == 0) {
		// encontra todos os agentes (empatados) com o melhor valor de avaliação
		for (int i = this->getNumAgInfosNotFixed() - 1; i >= 0; i --) {
			info = getAgInfoNotFixed(i);
			if (EQUALS(info->evaluation, this->bestEvaluation) == true) {
				info->fixed = true;
			}
		}

		// se o agente eh um dos q possui o melhor valor
		if (EQUALS(this->evaluation, this->bestEvaluation) == true) {
			log() << "Eu sou o melhor =D";
			this->state = SEND_PTRN_MXR;
		} else {
			log() << "Nao sou o melhor =(";
			this->state = CHOOSE_PTRN;
		}
	}

}

void CbPercAgentNegotiation::actionSendPatternToMixer() {
	CbPercussionistAgent* ag = getPercAgent();

	this->commitCurrentPattern();

	if (this->bar == 0) {
		if (strcmp(this->role->getName(),"base") == 0
				|| strcmp(this->role->getName(),"base-complementar") == 0
				|| strcmp(this->role->getName(),"uniforme") == 0) {
			log() << "Reordenando o repertorio...";
			const CbRhythmPattern* pattern = ag->repertoire->getRhythmPattern(this->getCurrentPatternMainIndex());
			ag->repertoire->sort(this->role->getName(), AVC_CHRONOT_DIST, pattern);
		}
	}

	log() << "Enviando para o mixer...";
	int currPtrnMainIndex = this->getCurrentPatternMainIndex();
	int currPtrnBar = this->getCurrentPatternBar();
	FmAclMessage* mixerMsg = ag->createMessageFromPattern(currPtrnMainIndex, currPtrnBar);

	mixerMsg->setInReplyTo(this->barStr);
	mixerMsg->addReceiver(getPercAgent()->mixerAgent);

	ag->send(*mixerMsg);
	delete mixerMsg;

	log() << "Notificando " << agentsToNotify.size() << " agentes...";
	FmAclMessage notifyMessage (FP_INFORM);

	notifyMessage.setContent("pattern-chosen");
	notifyMessage.setConversationId(onlyNegotiationMessages->getConversationId());

	Vector_AID::iterator it;
	for (it = agentsToNotify.begin(); it != agentsToNotify.end(); it ++) {
		notifyMessage.addReceiver(*it);
	}

	ag->send(notifyMessage);

	this->state = FINISHED;

    // acoxambre !
    if (strcmp(this->role->getName(), "solo") == 0) {
    	ag->setIntensityScale(1.0);
    }

}

bool CbPercAgentNegotiation::done() {
	return this->state == FINISHED;
}

