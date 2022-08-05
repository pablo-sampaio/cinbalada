
#include "CbSimpleRoleAgent.h"

#include "Parameters.h"
#include "CbOutputterAgent.h"

#include <values.h>

#include "Util/MathUtil.h"

static ofstream logFile("logs\\CbSimpleRoleAgent.txt", ios::out);


const char* CbSimpleRoleAgent::rolesStr[] = {
	"none",
    "basic-1",
    "basic-2",
    "solo-1"
};

const char* CbSimpleRoleAgent::agentRoleToStr(ECbAgentRole role) {
	int ri = role;

    if (ri >= 0 && ri < 4) {
    	return rolesStr[ri + 1];
    } else {
    	return rolesStr[0];
    }
}

ECbAgentRole CbSimpleRoleAgent::strToAgentRole(const char* str) {
	int pos = AR_NONE;

    for (int i = 1; i < 4; i ++) {
    	if (strcmp(rolesStr[i],str) == 0) {
			pos = i - 1;
            break;
        }
    }

    return ECbAgentRole(pos);
}


//...........................//
// CLASSE CbSimpleRoleAgent
//...........................//

CbSimpleRoleAgent::CbSimpleRoleAgent(std::string name, CbAgentRepertoire *rep, ECbAgentRole __role)
	: CbAbstractAgent(name, rep), role(__role)
{
    this->maxRepertIndex = rep->getNumRhythmPatterns();
	this->setLogStream(&logFile, false);
}

CbSimpleRoleAgent::~CbSimpleRoleAgent() {
	// nada a deletar
}

void CbSimpleRoleAgent::setupB1Patterns() {
	int size = repertoire->getNumRhythmPatterns();

   	log() << "Definindo a faixa de padroes aceitaveis para o papel B1...";
	repertoire->sort(AVC_NBASICNESS_1, true);

    // podia tb usar uma busca binaria (para repertorios grandes)
    int i = 0;
	this->maxRepertIndex = -1;

    while (i < size	&& repertoire->getRhythmPattern(i)->nbasicness_1() < CB_PAR_B1PATTERNS_MAX_NBASICNESS1) {
		this->maxRepertIndex = i;
		i ++;
    }
	log() << "Padroes B1 \'verdadeiros\': " << (maxRepertIndex + 1);

	int minMaxIndex = int(ceil(0.25 * size));
    if (this->maxRepertIndex < minMaxIndex) {
    	log() << "Alcance menor que 25% do repertorio,  reajustando para " << minMaxIndex;
		this->maxRepertIndex = minMaxIndex;
    }

	for (i = maxRepertIndex; i < size; i ++) {
    	repertoire->setEnabled(i, false);
    }
}

void CbSimpleRoleAgent::setupB2Patterns() {
	int size = repertoire->getNumRhythmPatterns();

   	log() << "Definindo a faixa de padroes aceitaveis para o papel B2...";
	repertoire->sort(AVC_NBASICNESS_2, true);

    // podia tb usar uma busca binaria (para repertorios grandes)
    int i = 0;
	this->maxRepertIndex = -1;

    while (i < size	&& repertoire->getRhythmPattern(i)->nbasicness_2()	< CB_PAR_B2PATTERNS_MAX_NBASICNESS2) {
		this->maxRepertIndex = i;
		i ++;
    }
	log() << "Padroes B2 \'verdadeiros\': " << (maxRepertIndex + 1);

	int minMaxIndex = int(ceil(0.50 * size));
    if (this->maxRepertIndex < minMaxIndex) {
    	log() << "Alcance menor que 50% do repertorio,  reajustando para " << minMaxIndex;
		this->maxRepertIndex = minMaxIndex;
    }

	for (i = maxRepertIndex; i < size; i ++) {
    	repertoire->setEnabled(i, false);
    }
}

void CbSimpleRoleAgent::setupS1Patterns() {
	int size = repertoire->getNumRhythmPatterns();

   	log() << "Definindo a faixa de padroes aceitaveis para o papel S1...";
	repertoire->sort(AVC_NBASICNESS_1, false);

    // podia tb usar uma busca binaria (para repertorios grandes)
    int i = 0;
	this->maxRepertIndex = -1;

    while (i < size	&& repertoire->getRhythmPattern(i)->nbasicness_1() >= CB_PAR_B1PATTERNS_MAX_NBASICNESS1) {
		this->maxRepertIndex = i;
		i ++;
    }
	log() << "Padroes S1 \'verdadeiros\': " << (maxRepertIndex + 1);

	int minMaxIndex = int(ceil(0.50 * size));
    if (this->maxRepertIndex < minMaxIndex) {
    	log() << "Alcance menor que 50% do repertorio,  reajustando para " << minMaxIndex;
		this->maxRepertIndex = minMaxIndex;
    }

	for (i = maxRepertIndex; i < size; i ++) {
    	repertoire->setEnabled(i, false);
    }
}

int CbSimpleRoleAgent::choosePattern(const Vector_pRhythmPattern &ref) {
	int resultado = -1;

	switch (this->role) {
    case AR_BASIC_1:
		resultado = chooseB1Pattern(ref);
    	break;
    case AR_BASIC_2:
		resultado = chooseB2Pattern(ref);
    	break;
    case AR_SOLO_1:
		resultado = chooseS1Pattern(ref);
    	break;
    }

    return resultado;
}

int CbSimpleRoleAgent::chooseB1Pattern(const Vector_pRhythmPattern &refPatterns) {
	int resultado = -1;
    int avgTimesChosen = this->repertoire->getAvgTimesChosen();

    if (refPatterns.size() == 0) {
		log() << "Nao ha agentes de referencia! Escolhendo padrao...";
		for (int i = 0; i <= this->maxRepertIndex; i ++) {
			if (this->repertoire->isEnabled(i)
					&& this->repertoire->getTimesChosen(i) <= avgTimesChosen) {
				resultado = i;
                break;
			}
		}

    } else {
	    int bestAval = MAXINT;  // um valor alto qualquer
	    int currAval;

	   	log() << "Escolhendo o padrao mais adequado aos demais...";
	    for (int p = 0; p <= this->maxRepertIndex; p ++) {
        	if (this->repertoire->isEnabled(p)
		            && this->repertoire->getTimesChosen(p) <= avgTimesChosen) {
				for (int j = 0; j < refPatterns.size(); j ++) {
		        	currAval = this->repertoire->getRhythmPattern(p)->geberFunction(refPatterns[j]);
		        	if (currAval < bestAval) {
		                bestAval = currAval;
	                    resultado = p;
		            }
		    	}
            }
	    }

	} //end else

	if (resultado == -1) {
		log() << "ATENCAO: Nao foi possivel escolher nenhum padrao!!! Escolhendo padrao 0...";
		resultado = 0;
	}

	log() << "Padrao escolhido: " << resultado << ".";
    return resultado;
}

int CbSimpleRoleAgent::chooseB2Pattern(const Vector_pRhythmPattern &refPatterns) {
	return chooseB1Pattern(refPatterns);
}

int CbSimpleRoleAgent::chooseS1Pattern(const Vector_pRhythmPattern &refPatterns) {
	return chooseB1Pattern(refPatterns);
}

void CbSimpleRoleAgent::setup() {
	log() << "Iniciando agente com papel \"" << agentRoleToStr(role) << "\"...";

	switch (role) {
    case AR_BASIC_1:
		setupB1Patterns();
    	addBehaviour(new JoinerBehaviour(this, AR_BASIC_1));
    	break;
    case AR_BASIC_2:
        setupB2Patterns();
    	addBehaviour(new JoinerBehaviour(this, AR_BASIC_1));
    	break;
    case AR_SOLO_1:
        setupS1Patterns();
    	addBehaviour(new JoinerBehaviour(this, AR_BASIC_1));
    	break;
    }
}

void CbSimpleRoleAgent::rejoinBand() {
	switch (role) {
    case AR_BASIC_1:
    	addBehaviour(new JoinerBehaviour(this, AR_BASIC_1));
    	break;
    case AR_BASIC_2:
    	addBehaviour(new JoinerBehaviour(this, AR_BASIC_1));
    	break;
    case AR_SOLO_1:
    	addBehaviour(new JoinerBehaviour(this, AR_BASIC_1));
    	break;
    }
}


//............................................//
// CLASSE CbSimpleRoleAgent::JoinerBehaviour
//............................................//

CbSimpleRoleAgent::JoinerBehaviour::JoinerBehaviour(CbSimpleRoleAgent *ag, ECbAgentRole ref)
	: FmBehaviour(ag)
{
	this->agent = ag;
    this->referenceRole = ref;
    this->pendingAnswers = 0;
    this->state = REQ_ROLES;
}

CbSimpleRoleAgent::JoinerBehaviour::~JoinerBehaviour() {
	for (int i = 0; i < refPatterns.size(); i ++) {
    	delete refPatterns[i];
        refPatterns[i] = NULL;
    }
}

void CbSimpleRoleAgent::JoinerBehaviour::action() {
	switch (this->state) {
    case REQ_ROLES:
    	reqRolesState();
    	break;
    case RECEIVE_ROLES:
    	recvRolesState();
    	break;
    case REQ_PATTERNS:
    	reqPatternsState();
    	break;
    case RECEIVE_PATTERNS:
    	recvPatternsState();
    	break;
    case CHOOSE_PATTERN:
    	choosePatternState();
    	break;
    }
}

void CbSimpleRoleAgent::JoinerBehaviour::reqRolesState() {
	Vector_AID bandIds;

    agent->getBandIds(bandIds);

	FmAclMessage msg (FP_REQUEST);
    msg.setContent(MSG_REQ_ROLES);

    this->pendingAnswers = bandIds.size();

    for (int i = 0; i < bandIds.size(); i ++) {
    	msg.addReceiver(bandIds[i]);
    }

    log() << "Requisitando os papeis dos agentes (" << bandIds.size() << " agentes)...";

    agent->send(msg);

    this->state = RECEIVE_ROLES;
}

void CbSimpleRoleAgent::JoinerBehaviour::recvRolesState() {
	FmAclMessage* acl = agent->receive();

    if (acl != NULL) {

	    if (acl->getPerformative() == FP_INFORM) {
		    log() << "Resposta recebida: " << acl->getContent();

	    	ECbAgentRole agRole = strToAgentRole(acl->getContent().c_str());
	    	if (agRole == this->referenceRole) {
	        	refAgents.push_back(acl->getSender());
	        }
            this->pendingAnswers --;

        } else {
		    log() << "Erro!!! Mensagem inesperada: " << acl->getPerformativeName() << " - " << acl->getContent();
            throw EXCP_UNEXPECTED_MSG;
        }

        delete acl;
    }

    if (pendingAnswers == 0) {
    	log() << "Nao ha mais respostas pendentes!";

    	// nao ha agentes do papel de referencia
    	if (refAgents.size() == 0) {
            this->state = CHOOSE_PATTERN;

        // existem agentes do papel de referencia
        } else {
        	this->state = REQ_PATTERNS;

        }

    }

}

void CbSimpleRoleAgent::JoinerBehaviour::reqPatternsState() {
	FmAclMessage snd (FP_REQUEST);
	snd.setContent(MSG_REQ_RPATTERN);

	for (int i = 0; i < refAgents.size(); i ++) {
		snd.addReceiver(refAgents[i]);
	}
	this->pendingAnswers = this->refAgents.size();

	log() << "Requisitando padroes dos agentes referencia (" << refAgents.size() << " agentes)...";
	agent->send(snd);
	this->state = RECEIVE_PATTERNS;
}

void CbSimpleRoleAgent::JoinerBehaviour::recvPatternsState() {
	FmAclMessage* acl = agent->receive();

    if (acl != NULL) {
	    if (acl->getPerformative() == FP_INFORM) {
		    TiXmlDocument doc;

	        doc.Parse(acl->getContent().c_str());

	        if (doc.Error() == false) {
	           	log() << "Padrao recebido!";
	        	FmMidiFileTrack *phrase = readXmlPhrase(*(doc.FirstChildElement(XML_NODE_PHRASE)));
		        CbRhythmPattern *rp = new CbRhythmPattern(*phrase, CbMeter::SIMPLE_QUADR);

	            delete phrase;
				refPatterns.push_back(rp);
                this->pendingAnswers --;

	        } else {
	        	throw EXCP_PARSING_ERROR;

	        }
	    }

	    delete acl;
    }

    if (pendingAnswers == 0) {
    	log() << "Nao ha mais respostas (padroes) pendentes!";
        this->state = CHOOSE_PATTERN;
	}
}

void CbSimpleRoleAgent::JoinerBehaviour::choosePatternState() {
	int rp = agent->choosePattern(this->refPatterns);
	agent->setCurrRhythmPattern(rp);

    switch (agent->role) {
    case AR_SOLO_1:
		agent->addBehaviour(new SoloPlayerBehaviour(agent));
    	break;
    default:
		agent->addBehaviour(new PlayerBehaviour(agent));
		break;
    }

	this->state = QUIT;
}

bool CbSimpleRoleAgent::JoinerBehaviour::done() {
	return this->state == QUIT;
}

std::ofstream& CbSimpleRoleAgent::JoinerBehaviour::log(bool prompt) {
	ofstream* ofs = agent->getLogStream(prompt);
    const char* roleStr = agentRoleToStr(agent->role);
    if (prompt) {
        (*ofs) << "{JOINER<" << roleStr << ">, " << state << "} - ";
    }
    return *ofs;
}


//............................................//
// CLASSE CbSimpleRoleAgent::PlayerBehaviour
//............................................//

CbSimpleRoleAgent::PlayerBehaviour::PlayerBehaviour(CbSimpleRoleAgent* agent)
	: FmBehaviour(agent)
{
	this->agent = agent;
}

void CbSimpleRoleAgent::PlayerBehaviour::onStart() {
    agent->registerInOutputter();
    agent->registerInBand();
    this->state = SETUP;
}

void CbSimpleRoleAgent::PlayerBehaviour::onEnd() {
    agent->unregisterFromOutputter();  // problema: no rejoin o outputter vai criar nova track
    agent->unregisterFromBand();
}

void CbSimpleRoleAgent::PlayerBehaviour::action() {
	switch (state) {
    case SETUP:
	    setupState();
    	break;
	case PLAYING:
        playingState();
        break;
	}
}

void CbSimpleRoleAgent::PlayerBehaviour::setupState() {
    FmAclMessage *rec = agent->receive();

    if (rec == NULL) return;

	if (rec->getPerformative() == FP_AGREE) {
		log() << "Agente cadastrado no outputter com sucesso !";
		this->state = PLAYING;
	} else {
	    log() << "Erro!!! Mensagem inesperada: " << rec->getPerformativeName() << " - " << rec->getContent();
		throw EXCP_UNEXPECTED_MSG;
	}

	delete rec;
}

// responde req do papel
// responde req do padrao (outputter ou demais agentes)
void CbSimpleRoleAgent::PlayerBehaviour::playingState() {
    FmAclMessage *rec;
    FmAclMessage *snd;

	if (agent->CbAbstractAgent::changePatternAsked == true) {
    	log() << "Requisicao de mudanca recebida !!!";
    	this->state = QUIT;
        agent->rejoinBand();
	    agent->CbAbstractAgent::changePatternAsked = false;
    }

	rec = agent->receive();

    if (rec == NULL) return;

    if (rec->getPerformative() == FP_REQUEST) {
    	if (rec->getContent() == MSG_REQ_ROLES) {
        	log() << "Informando o papel...";
	        snd = new FmAclMessage(FP_INFORM);
            snd->setContent(agentRoleToStr(agent->role));
	        snd->addReceiver(rec->getSender());
	        agent->send(*snd);
	        delete snd;
        } else if (rec->getContent() == MSG_REQ_RPATTERN
        			||rec->getContent() == MSG_REQ_SEND_NEXTPHRASE) {
        	log() << "Informando o padrao atual...";
	        int cb = agent->getCurrRhythmPattern();
	        snd = agent->createMsgXmlPhrase(agent->repertoire->getRhythmPattern(cb));
	        snd->addReceiver(rec->getSender());
	        agent->send(*snd);
	        delete snd;
        } else {
		    log() << "Erro!!! Mensagem inesperada: " << rec->getPerformativeName() << " - " << rec->getContent();
            throw EXCP_UNEXPECTED_MSG;
        }
    }

    delete rec;
}

bool CbSimpleRoleAgent::PlayerBehaviour::done() {
    return state == QUIT;
}

std::ofstream& CbSimpleRoleAgent::PlayerBehaviour::log(bool prompt) {
	ofstream* ofs = agent->getLogStream(prompt);
    if (prompt) {
        (*ofs) << "{PLAYER, " << state << "} - ";
    }
    return *ofs;
}


//...............................................//
// CLASSE CbSimpleRoleAgent::SoloPlayerBehaviour
//...............................................//

CbSimpleRoleAgent::SoloPlayerBehaviour::SoloPlayerBehaviour(CbSimpleRoleAgent* agent) {
	this->agent = agent;
    this->basePattern = agent->getCurrRhythmPattern();
}

void CbSimpleRoleAgent::SoloPlayerBehaviour::onStart() {
    agent->registerInOutputter();
    agent->registerInBand();
    this->state = SETUP;
}

void CbSimpleRoleAgent::SoloPlayerBehaviour::onEnd() {
    agent->unregisterFromOutputter();  // problema: no rejoin o outputter vai criar nova track
    agent->unregisterFromBand();
}

void CbSimpleRoleAgent::SoloPlayerBehaviour::action() {
	switch (state) {
    case SETUP:
	    setupState();
    	break;
	case PLAYING:
        playingState();
        break;
    case CHOOSE_PATTERN:
    	choosePattern();
        break;
	}
}

void CbSimpleRoleAgent::SoloPlayerBehaviour::setupState() {
    FmAclMessage *rec = agent->receive();

    if (rec == NULL) return;

	if (rec->getPerformative() == FP_AGREE) {
		log() << "Agente cadastrado no outputter com sucesso !";
		this->state = PLAYING;
	} else {
	    log() << "Erro!!! Mensagem inesperada: " << rec->getPerformativeName() << " - " << rec->getContent();
		throw EXCP_UNEXPECTED_MSG;
	}

	delete rec;
}

void CbSimpleRoleAgent::SoloPlayerBehaviour::playingState() {
    FmAclMessage *rec;
    FmAclMessage *snd;

	if (agent->CbAbstractAgent::changePatternAsked == true) {
    	log() << "Requisicao de mudanca recebida !!!";
        agent->rejoinBand();
	    agent->CbAbstractAgent::changePatternAsked = false;
    	this->state = QUIT;
    }

	rec = agent->receive();

    if (rec == NULL) return;

    if (rec->getPerformative() == FP_REQUEST) {
    	if (rec->getContent() == MSG_REQ_ROLES) {
        	log() << "Informando o papel...";
	        snd = new FmAclMessage(FP_INFORM);
            snd->setContent(agentRoleToStr(agent->role));
	        snd->addReceiver(rec->getSender());
	        agent->send(*snd);
	        delete snd;
        } else if (rec->getContent() == MSG_REQ_RPATTERN
        			|| rec->getContent() == MSG_REQ_SEND_NEXTPHRASE) {
        	log() << "Informando o padrao atual...";
	        int cb = agent->getCurrRhythmPattern();
	        snd = agent->createMsgXmlPhrase(agent->repertoire->getRhythmPattern(cb));
	        snd->addReceiver(rec->getSender());
	        agent->send(*snd);
	        delete snd;

        	if (rec->getContent() == MSG_REQ_SEND_NEXTPHRASE) {
            	this->state = CHOOSE_PATTERN;
            }

        } else {
		    log() << "Erro!!! Mensagem inesperada: " << rec->getPerformativeName() << " - " << rec->getContent();
            throw EXCP_UNEXPECTED_MSG;
        }
    }

    delete rec;
}

void CbSimpleRoleAgent::SoloPlayerBehaviour::choosePattern() {
	Vector_pRhythmPattern ref;

    ref.push_back(agent->repertoire->getRhythmPattern(this->basePattern));

	int rp = agent->choosePattern(ref);
	agent->setCurrRhythmPattern(rp);

    this->state = PLAYING;
}

bool CbSimpleRoleAgent::SoloPlayerBehaviour::done() {
	return this->state == QUIT;
}

std::ofstream& CbSimpleRoleAgent::SoloPlayerBehaviour::log(bool prompt) {
	ofstream* ofs = agent->getLogStream(prompt);
    if (prompt) {
        (*ofs) << "{PLAYER-SOLO, " << state << "} - ";
    }
    return *ofs;
}

