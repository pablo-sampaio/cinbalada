
#include "CbRandomAgent.h"
#include "CbFacade.h"

#include "Util\MathUtil.h"

using namespace std;


//.........................//
// CLASSE CbRandomAgent
//.........................//

CbRandomAgent::CbRandomAgent(string name, CbPercRepertoire *rep)
	: CbAbstractAgent(name)
{
	this->repertoire = rep;

    AnsiString appDirectory = CbFacade::getInstance()->getApplicationDir();
	AnsiString logFileName = appDirectory + "\\logs\\CbRandomAgent - " + name.c_str() + ".txt";

	this->setLogStream(new ofstream(logFileName.c_str()), true);
}

CbRandomAgent::~CbRandomAgent() {
	// nada a deletar
}

int CbRandomAgent::choosePattern() {
	int repertSize = this->repertoire->getNumRhythmPatterns("livre");
	int patternRoleIndex = randx() % repertSize;

    this->currentPattern = this->repertoire->getMainIndex("livre", patternRoleIndex);

    log() << "Padrão escolhido: " << this->currentPattern;
	return this->currentPattern;
}

int CbRandomAgent::getCurrentPattern() {
	return this->currentPattern;
}

void CbRandomAgent::setup() {
	FmAgent::addBehaviour(new RandomAgentBehaviour(this));
}


//............................................//
// CLASSE CbRandomAgent::RandomAgentBehaviour
//............................................//

CbRandomAgent::RandomAgentBehaviour::RandomAgentBehaviour(CbRandomAgent *ag)
	: FmCyclicBehaviour(ag)
{
	this->agent = ag;
}

ofstream& CbRandomAgent::RandomAgentBehaviour::log(bool prompt) {
	return agent->log(prompt);
}

void CbRandomAgent::createMessageContentFromPattern(int patternMainIndex, int bar, FmAclMessage& result) {
	const CbRhythmPattern* rp = this->repertoire->getRhythmPattern(patternMainIndex);
	const FmMidiFileTrack *phrase = rp->getPattern();
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
}

void CbRandomAgent::RandomAgentBehaviour::onStart() {
	agent->registerInMixer();
}

void CbRandomAgent::RandomAgentBehaviour::action() {
	FmAclMessage *rec, *snd;

	rec = agent->receive();  // so pode vir do mixer !!!

	if (rec == NULL) {
		block();
		return;
	}

	log() << "Recebido: " << rec->getPerformativeName() << " - " << rec->getContent();

	if (rec->getContent() == MSG_INF_MIXER_START) {
    	agent->choosePattern();

    } else if (rec->getContent().find("bar=") == 0) {
		int currPattern = agent->getCurrentPattern();

		snd = rec->createReply(FP_INFORM);
		agent->createMessageContentFromPattern(currPattern, 0, *snd);

		log() << "Enviando frase atual...";
		agent->send(*snd);

		delete snd;

	} else {
		throw EXCP_UNEXPECTED_MSG;
	}

	delete rec;
}

void CbRandomAgent::RandomAgentBehaviour::onEnd() {
	agent->unregisterFromMixer();
}
