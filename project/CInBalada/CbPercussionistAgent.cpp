//---------------------------------------------------------------------------

#include "CbFacade.h"
#include "FAMA\FmMessageTemplate.h"
#include "Util\MathUtil.h"

#pragma hdrstop

#include "CbPercussionistAgent.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;
//---------------------------------------------------------------------------


//...............................//
// CLASSE CbPercussionistAgent
//...............................//

CbPercussionistAgent::CbPercussionistAgent(string name, CbPercRepertoire *rep,
									const CbRhythmicRole* defRole, bool susp, bool autoDel) :
	CbAbstractAgent(name, susp, autoDel) ,
	planRoles(defRole)
{
	this->repertoire = rep;
	this->repertoire->setHistory(&(this->history));

    AnsiString appDirectory = CbFacade::getInstance()->getApplicationDir();
	AnsiString logFileName = appDirectory + "\\logs\\PercussionistAgent - " + name.c_str() + ".txt";

	setLogStream(new ofstream(logFileName.c_str()), true);

    // acoxambre, por causa da base atual
    if (rep->getInstrumentName() == "surdo" || rep->getInstrumentName() == "bumbo"
	        || rep->getInstrumentName() == "caixa") {
		this->intensityScale = 0.8;
    } else {
		this->intensityScale = 1.0;
    }

	this->verticalEvaluationWeigth = DEFAULT_VERT_EVAL_WEIGTH;
    this->horizontalEvaluationWeigth = DEFAULT_HORIZ_EVAL_WEIGTH;

	this->OnRPatternChange = NULL;
}

CbPercussionistAgent::~CbPercussionistAgent() {
	this->repertoire->setHistory(NULL);
	delete this->repertoire;
}

CbPercRepertoire* CbPercussionistAgent::getRepertoire() {
	return this->repertoire;
}

CbPercPlan* CbPercussionistAgent::getPlan() {
	return &planRoles;
}

CbPercPerfHistory* CbPercussionistAgent::getHistory() {
	return &this->history;
}

double CbPercussionistAgent::getIntensityScale() const {
	return this->intensityScale;
}

void CbPercussionistAgent::setIntensityScale(double w) {
	if (w > 0.0) {
		this->intensityScale = w;
	}
}

void CbPercussionistAgent::setEvaluationWeigths(double vert, double horiz) {
	if (vert >= 0 && horiz >= 0) {
		this->verticalEvaluationWeigth = vert;
	    this->horizontalEvaluationWeigth = horiz;
    }
}

double CbPercussionistAgent::getVerticalEvaluationWeigth() {
	return this->verticalEvaluationWeigth;
}

double CbPercussionistAgent::getHorizontalEvaluationWeigth() {
	return this->horizontalEvaluationWeigth;
}

std::string CbPercussionistAgent::getInstrumentName() const {
	return this->repertoire->getInstrumentName();
}

FmMidiFileTrack* CbPercussionistAgent::scaleIntensities(const FmMidiFileTrack* originalTrack) {
	FmMidiFileTrack* newTrack = new FmMidiFileTrack(originalTrack->getResolution());
	Vector_pMidiEvent events;
	FmMidiChannelEvent* channelEvent;
	FmMidiEvent* aux;
	double newIntensity;

	originalTrack->getAllEvents(events);
	int numEvents = events.size();

	// muda intensidades...
	for (int i = 0; i < numEvents; i ++) {

		if (events[i]->isChannelEvent()) {
			channelEvent = static_cast<FmMidiChannelEvent*>(events[i]);

			switch (channelEvent->getType()) {
			case MD_NOTE_ON:
				newIntensity = int(this->intensityScale * channelEvent->getVelocity());
				if (newIntensity > 127) {
					newIntensity = 127;
				}
				aux = FmMidiEvent::createNoteOn(channelEvent->getPitch(), newIntensity, channelEvent->getTick(), channelEvent->getChannel());
				newTrack->addEvent(aux);
				break;
			case MD_NOTE_OFF:
				newIntensity = int(this->intensityScale * channelEvent->getVelocity());
				if (newIntensity > 127) {
					newIntensity = 127;
				}
				aux = FmMidiEvent::createNoteOff(channelEvent->getPitch(), newIntensity, channelEvent->getTick(), channelEvent->getChannel());
				newTrack->addEvent(aux);
				break;
			default:
				newTrack->addEvent(channelEvent);
				break;
			}

		} else {
			newTrack->addEvent(events[i]);

		}

	} // for

	return newTrack;
}

FmAclMessage* CbPercussionistAgent::createMessageFromPattern(int patternMainIndex, int bar) {
	FmAclMessage *msg = new FmAclMessage(FP_INFORM);
	createMessageContentFromPattern(patternMainIndex, bar, *msg);
	return msg;
}

void CbPercussionistAgent::createMessageContentFromPattern(int patternMainIndex, int bar, FmAclMessage& result) {
	const CbRhythmPattern* rp = this->repertoire->getRhythmPattern(patternMainIndex);
	FmMidiFileTrack *phrase = scaleIntensities(rp->getPattern());
	TiXmlElement *doc;

	if (bar <= -1) {
		doc = writeXmlPhrase(*phrase, bar);

	} else {
		double ticksPerBar = this->repertoire->getMeter()->getNumQuarterNotes() * phrase->getResolution();
		int startTick = int( bar * ticksPerBar );
		int endTick = int( (bar + 1) * ticksPerBar ) - 1;

		log() << "Compasso \"" << bar << "\" - de " << startTick << " a " << endTick;

		doc = writeXmlPhrase(*phrase, startTick, endTick);
	}

	string xmlPattern;
	xmlPattern << (*doc);
	result.setContent(xmlPattern);
	result.setLanguage("xml");

	delete doc;
	delete phrase;
}

CbRhythmPattern* CbPercussionistAgent::readPatternFromMessage(const FmAclMessage* msg) {
	const CbMeter* meter = this->repertoire->getMeter();  //assume que todos os agentes tem a mesma metrica
	TiXmlDocument *doc = new TiXmlDocument("");

	doc->Parse(msg->getContent().c_str());

	FmMidiFileTrack* track = readXmlPhrase(*(doc->RootElement()));
	CbRhythmPattern *rp = new CbRhythmPattern(*track, *meter);
	delete track;
	delete doc;

	return rp;
}

double CbPercussionistAgent::random_eval(const CbRhythmPattern* rp, const CbRhythmPattern* ref) {
	return randx() % 100;
}

void CbPercussionistAgent::setup() {
	registerAsMusician(this->repertoire->getInstrumentName().c_str());
	registerInMixer();
	addBehaviour(new CbPercAgentBehaviour(this));
}

void CbPercussionistAgent::takeDown() {
	unregisterFromMixer();
	unregisterAsMusician();
}


//...............................//
// CLASSE CbPercAgentBehaviour
//...............................//

CbPercAgentBehaviour::CbPercAgentBehaviour(CbPercussionistAgent* ag)
	: FmCyclicBehaviour(ag)
{
	string blank = "";

	FmMessageTemplate* matchConvId = FmMessageTemplate::matchConversationId(blank);
	FmMessageTemplate* matchInReplyTo = FmMessageTemplate::matchInReplyTo(blank);

	this->msgTemplate = FmMessageTemplate::and(matchConvId, matchInReplyTo);
}

CbPercAgentBehaviour::~CbPercAgentBehaviour() {
	delete msgTemplate;
}

CbPercussionistAgent* CbPercAgentBehaviour::getPercAgent() {
	return static_cast<CbPercussionistAgent*>(myAgent);
}

void CbPercAgentBehaviour::action() {
	CbPercussionistAgent* ag = getPercAgent();
	FmAclMessage* acl = ag->receive(*this->msgTemplate);

	if (acl == NULL) {
		if (this->patternRequests.empty()) {
			block();
		} else {
			processPendingPatternRequests();
		}

	} else {
		string msgContent = acl->getContent();

		if (acl->getPerformative() == FP_INFORM) {
			if (msgContent == MSG_INF_MIXER_START) {
				clearPerformanceHistory();
			} else {
				createBehaviourForBar(acl);
			}

		} else if (acl->getPerformative() == FP_REQUEST) {
			if (msgContent.find("role") == 0) {
				processRoleRequest(acl);
			} else if (msgContent.find("pattern") == 0) {
				processPatternRequest(acl);
			}

		}

		delete acl;
	}

}

void CbPercAgentBehaviour::clearPerformanceHistory() {
	CbPercussionistAgent* ag = getPercAgent();
	ag->log() << "Recebido 'sync'. Limpando historico...";
	ag->history.clear();
}

void CbPercAgentBehaviour::createBehaviourForBar(FmAclMessage* acl) {
	CbPercussionistAgent* ag = getPercAgent();

	int bar = -1;
	sscanf(acl->getContent().c_str(), "bar=%d", &bar);

	if (bar != -1) {
		int planBar = bar % CbFacade::getInstance()->getBarsCycle();

		const CbRhythmicRole* role = ag->planRoles.getRole(planBar);
		string roleStr = role->getName();

		ag->log() << endl;
		ag->log() << "------------------------------- ---";
		ag->log() << "Criando behaviour para compasso (" << bar << ")";
		ag->log() << "------------------------------- ---";
		ag->addBehaviour(new CbPercAgentNegotiation(ag, role, bar));

	} else {
		ag->log() << "Mensagem \"bar\" invalida: \"" << acl->getContent() << "\"";

	}

}

void CbPercAgentBehaviour::processRoleRequest(FmAclMessage* acl) {
	CbPercussionistAgent* ag = getPercAgent();
	int bar = 0;

	sscanf(acl->getContent().c_str(), "role(%d)", &bar);

	const CbRhythmicRole* role = ag->planRoles.getRole(bar);
	string roleStr = role->getName();

	FmAclMessage *reply = acl->createReply(FP_INFORM);
	reply->setContent(roleStr);

	ag->log() << "Respondendo: papel \"" << roleStr << "\"";
	ag->send(*reply);

	delete reply;
}

void CbPercAgentBehaviour::processPatternRequest(FmAclMessage* msg) {
	CbPercussionistAgent* ag = getPercAgent();
	int bar = -1;

	sscanf(msg->getContent().c_str(), "pattern(%d)", &bar);

	if (bar != -1) {
		if (replyPatternRequest(msg, bar) == false) {
			patternRequests.push_back(new FmAclMessage(*msg));
			ag->log() << "Padrao ainda nao foi escolhido. Requisição registrada.";
		}

	} else {
		ag->log() << "Mensagem \"pattern\" invalida: \"" << msg->getContent() << "\"";

	}
}

void CbPercAgentBehaviour::processPendingPatternRequests() {
	CbPercussionistAgent* ag = getPercAgent();
	int barRequested = -1;

	FmAclMessage* currMessage;
	Vector_pAclMessage::iterator beginIt = patternRequests.begin();
	FmAclMessage* answer;

	for (int i = this->patternRequests.size() - 1; i >= 0; i --) {
		currMessage = patternRequests[i];
		sscanf(currMessage->getContent().c_str(), "pattern(%d)", &barRequested);

		if (replyPatternRequest(currMessage, barRequested) == true) {
			ag->log() << "(pendencia resolvida)";
			delete currMessage;
			patternRequests.erase(beginIt + i);
		}
	}

}

bool CbPercAgentBehaviour::replyPatternRequest(FmAclMessage* msg, int barRequested) {
	CbPercussionistAgent* ag = getPercAgent();
	bool replyDone = false;

	if (ag->history.has(barRequested)) {
		const CbPercPerfInfo* perfInfo = ag->history.get(barRequested);
		int patrnMainIndex = perfInfo->patternIndex;
		int patrnBar = perfInfo->patternBar;

		FmAclMessage* answer = msg->createReply(FP_INFORM);
		ag->createMessageContentFromPattern(patrnMainIndex, patrnBar, *answer);

		ag->log() << "Respondendo: padrao \"" << patrnMainIndex << "\"";
		ag->send(*answer);

		delete answer;
		replyDone = true;
	}

	return replyDone;
}

