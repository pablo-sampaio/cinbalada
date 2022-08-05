
#include "CbMixerAgent.h"

#include <fstream>
#include "Util/MathUtil.h"

// d - tick duration; r - resolution (ticks per beat)
#define TICKDURATION_TO_BPM(d, r)      ( 60000.0 / (double(d) * double(r)) )

// t - tempo in BPM; r - resolution (ticks per beat)
#define BPM_TO_TICKDURATION(t, r)     ( 60000.0 / (double(t) * double(r)) )


using namespace std;

// arquivo de log da classe 'CbMixerAgent'
static ofstream logFile("logs\\CbMixerAgent.txt", ios::out);

// arquivo de log das funcoes auxiliares
static ofstream logFunc("logs\\CbMixerAgent-functions.txt", ios::out);


/////// Classe CbMixerAgent ///////

CbMixerAgent::CbMixerAgent()
	: FmAgent("mixer-agent"),
	  resolution(DEFAULT_MIDIFILE_RESOLUTION)
{
	this->fileOutput = NULL;
	this->recording = false;
	this->midiOutput = NULL;
	this->executionStatus = ES_STOPPED;
	this->muted = false;
	this->semaphore = new TCriticalSection;

	this->currentTick = 0;
	this->currentBar = 0;
	this->maxBars = -1; // will loop indefinitely

	this->timerPeriod = BPM_TO_TICKDURATION(100, resolution);  // andamento de 100 BPM

	this->meter = new CbMeter(4,4);
	this->OnSetup = NULL;
	this->OnStatusChange = NULL;

	logFile << "INFO: Mixer criado!" << endl;
}

CbMixerAgent::~CbMixerAgent() {
	if (midiOutput != NULL) {
		this->semaphore->Acquire();
		delete midiOutput;
		this->semaphore->Release();
	}

	if (this->fileOutput != NULL) {
		delete fileOutput;
	}

	delete meter;
	delete semaphore;

	// deleta os vetores de FmAID*
	map<AID,ProducerInfo*>::iterator it;
	for (it = players.begin(); it != players.end(); it++) {
		delete it->second;
		it->second = NULL;
	}

	logFile << "INFO: Mixer deletado !" << endl;
}

void CbMixerAgent::setup() {
	FmServiceDescription servcDesc;
	servcDesc.setName(MIXER_AG_SERVC_NAME);
	servcDesc.setType(MIXER_AG_SERVC_TYPE);

	Vector_AID agents;

	getContainer()->searchService(servcDesc, agents);

	//testa se já existe um agente mixer...
	if (agents.size() > 0) {
		logFile << "ERRO: Ja existe um agente mixer (" << agents[0] << ") !" << endl;
		doDelete();

	} else {
		getContainer()->addService(this, servcDesc);
		addBehaviour(new MixerBehaviour(this));

		// prepara a saida midi
		this->midiOutput = new FmMidiOutput();

		// setup do timer
		TIMECAPS tc;
		int DESIRED_RES = 1;

		if (timeGetDevCaps(&tc,sizeof(TIMECAPS)) == TIMERR_NOERROR) {
			this->timerResolution = min(max(tc.wPeriodMin,DESIRED_RES),tc.wPeriodMax);
			timeBeginPeriod(this->timerResolution);
		}

		logFile << "INFO: Mixer pronto ! " << endl;

		if (this->OnSetup != NULL) {
			this->OnSetup(this);
		}
	}
}

void CbMixerAgent::takeDown() {
	logFile << "INFO: Encerrando... " << endl;
	this->stop();
	timeEndPeriod(this->timerResolution);
}

int CbMixerAgent::getCurrentBar() {
	return this->currentBar;
}

int CbMixerAgent::getCurrentTick() {
	return this->currentTick;
}

CbMixerAgent::MixerBehaviour::MixerBehaviour(CbMixerAgent* agent) {
	this->mixer = agent;
}

void CbMixerAgent::MixerBehaviour::action() {
	FmAclMessage* msg = mixer->receive();

	// mecanismo de exclusao mutua
	mixer->semaphore->Acquire();

	if (msg != NULL) {

		switch (msg->getPerformative()) {
		case FP_REQUEST:
			//logFile << "INFO: Recebido REQUEST: " << msg->getContent() << endl;
			if (msg->getContent() == MSG_REQ_REG_MUSPRODUCER) {
				mixer->registerPlayer(msg);
			} else if (msg->getContent().find(MSG_REQ_REG_MUSLISTENER_OF) == 0) {
				mixer->registerListener(msg);   //NAO TESTADO
			} else if (msg->getContent() == MSG_REQ_UNREG_MUSPRODUCER) {
				mixer->unregisterPlayer(msg);
			} else if (msg->getContent().find(MSG_REQ_UNREG_MUSLISTENER_OF) == 0) {
				mixer->unregisterListener(msg); //NAO TESTADO
			} else {
				logFile << "ERRO: Mensagem inesperada: \n" << msg << endl << endl;
			}
			break;

		case FP_INFORM:
			//logFile << "INFO: Recebido INFORM (+phrase)" << endl;
			if (mixer->players.find(msg->getSender()) != mixer->players.end()) {
				mixer->collateBars(msg);
			} else {
				logFile << "ERRO: Player nao cadastrado: \n" << msg << endl << endl;
			}
			break;

		default:
			logFile << "ERRO: Mensagem inesperada: \n" << msg << endl << endl;
		}

		delete msg;
	}

	mixer->semaphore->Release();
}

void __fastcall CbMixerAgent::registerPlayer(FmAclMessage* msg) {
	AID id = msg->getSender();

	logFile << "INFO: Cadastrando player \"" << id->getName() << "\" " << (*id) << endl;

	ProducerInfo *pinfo = new ProducerInfo();
	this->players[msg->getSender()] = pinfo;

	if (this->recording) {
		// cria uma track no arquivo exclusiva para o agente
		pinfo->track = this->fileOutput->createTrack();
       	logFile << "INFO: Track \"" << pinfo->track << "\" criada para o agente" << endl;
	} else {
		pinfo->track = -1;
	}

}

void __fastcall CbMixerAgent::unregisterPlayer(FmAclMessage* msg) {
	AID id = msg->getSender();
	Vector_AID* listIds;

	logFile << "INFO: Descadastrando agente \"" << id->getName() << "\" " << (*id) << endl;

	map<AID,ProducerInfo*>::iterator it = this->players.find(id);

	if (it == this->players.end()) {
		logFile << "ERRO: Agente nao cadastrado !" << endl;

	} else {
		ProducerInfo *pinfo = it->second;
		int numListeners = pinfo->listeners.size();

		if (numListeners > 0) {
			char notifyListenerContent[100];
			sprintf(notifyListenerContent, "%s%d", MSG_REQ_UNREG_MUSLISTENER_OF, id->getID());

			FmAclMessage* notifyListeners = new FmAclMessage(FP_INFORM);
			notifyListeners->setContent(notifyListenerContent);

			for (int i = 0; i < numListeners; i ++) {
				notifyListeners->addReceiver(pinfo->listeners[i]);
			}

			send(*notifyListeners);

			delete notifyListeners;
			delete listIds;
		}

		delete pinfo;
		players.erase(it);

		logFile << "INFO: Agente descadastrado! "
				<< numListeners << " listeners do agente foram notificados." << endl;

	}

}

void __fastcall CbMixerAgent::registerListener(FmAclMessage* msg) {
	int pl = atoi(msg->getContent().c_str() + 30);                //TESTAR !!!
	logFile << "INFO: Cadastrar listener para o player " << pl << endl; //TESTE

	AID playerId = getContainer()->searchAgent(pl);
	ProducerInfo *pinfo = this->players[playerId];

	pinfo->listeners.push_back(msg->getSender());
}

void __fastcall CbMixerAgent::unregisterListener(FmAclMessage* msg) {
	int pl = atoi(msg->getContent().c_str() + 32);                //TESTAR !!!
	logFile << "INFO: Descadastrar listener do player " << pl << endl; //TESTE

	AID playerId = getContainer()->searchAgent(pl);
	ProducerInfo *pinfo = this->players[playerId];

	Vector_AID::iterator it;
	for (it = pinfo->listeners.begin(); it != pinfo->listeners.end(); it ++) {
		if ( **it == *(msg->getSender()) ) {
			pinfo->listeners.erase(it);
			break;
		}
	}
}

void __fastcall CbMixerAgent::collateBars(FmAclMessage* msg) {
	TiXmlDocument doc("received-music.xml");
	FmMidiFileTrack* phrase;

	logFile << "INFO: Collating..." << endl;
	doc.Parse(msg->getContent().c_str());

	int msgCurrBar = atoi(msg->getInReplyTo().c_str());

	if (doc.Error()) {
		logFile << "ERRO: Invalid XML message !" << endl;
		logFile << "---" << endl;
		logFile << msg->getContent().c_str() << endl;
		logFile << "---" << endl << endl;

	} else if (this->executionStatus != ES_STOPPED) {
		// interpreta os nos XML
		phrase = readXmlPhrase(*doc.FirstChildElement(XML_NODE_PHRASE));

		// apenas um compasso !
		phrase->truncate(meter->getNumQuarterNotes() * phrase->getResolution() - 1);

		// grava no arquivo
		if (this->recording == true) {
			int agentTrack = players[msg->getSender()]->track;
			int offsetBarsTicks = int( meter->getNumQuarterNotes() * this->fileOutput->getResolution() * msgCurrBar);

			this->fileOutput->addEvents(*phrase, agentTrack, offsetBarsTicks);
		}

		// resolucao do sequenciador
		phrase->setResolution(this->resolution);

		// guarda para fazer o sequenciamento
		if (msgCurrBar == this->currentBar) {
			this->sequencerBuffer.addPhrase(*phrase, true);
		} else if (msgCurrBar == (this->currentBar + 1)) {
			this->sequencerBuffer.addPhrase(*phrase, false);
		} else {
			logFile << "ERRO: Invalid 'in-reply-to': " << msgCurrBar << " !" << endl;
		}

		delete phrase;

		// repassa aos listeners (FALTA: indicar o compasso)
		ProducerInfo *pinfo = this->players[msg->getSender()];
		Vector_AID::iterator it;
		FmAclMessage* listenMsg;

		if (pinfo->listeners.size() > 0) {
			listenMsg = new FmAclMessage(FP_INFORM);

			for (it = pinfo->listeners.begin(); it != pinfo->listeners.end(); it ++) {
				listenMsg->addReceiver(*it);
			}

			listenMsg->setContent(msg->getContent());
		}

	}
}

void __fastcall CbMixerAgent::informStart() {
	FmAclMessage msg (FP_INFORM);

	msg.setContent(MSG_INF_MIXER_START);
	msg.setLanguage(MIXER_LANGUAGE);

	map<AID,ProducerInfo*>::iterator it;
	for (it = players.begin(); it != players.end(); it ++) {
		msg.addReceiver(it->first);
	}

	send(msg);
}

void __fastcall CbMixerAgent::informNextBar() {
	FmAclMessage msg (FP_INFORM);

	char aux[20];
	sprintf(aux, "bar=%d", this->currentBar + 1); // informa o compasso seguinte
	msg.setContent(aux);

	map<AID,ProducerInfo*>::iterator it;
	for (it = players.begin(); it != players.end(); it ++) {
		msg.addReceiver(it->first);
	}

	sprintf(aux, "%d", this->currentBar + 1);
	msg.setReplyWith(aux);
	msg.setLanguage(MIXER_LANGUAGE);

	send(msg);
}

void CALLBACK CbMixerAgent::timerCallBack(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) {
	CbMixerAgent* mixer = (CbMixerAgent*)dwUser;
	mixer->executeMidiTick();
}

void __fastcall CbMixerAgent::executeMidiTick() {

	if (this->executionStatus == ES_PAUSED) {
		return;
	}

	// mecanismo de exclusao mutua
	this->semaphore->Acquire();

	if (this->muted == false) {
		Vector_pMidiEvent* evts =  this->sequencerBuffer.getCurrBuffer()->get(this->currentTick);

		if (evts != NULL) {
			//logFile << "Enviando eventos..." << endl;
			this->executeEvents(this->midiOutput, evts);
		}
	}

	this->currentTick = (this->currentTick + 1) % int(meter->getNumQuarterNotes() * this->resolution);

	// fim da exclusão mutua
	this->semaphore->Release();

	// se acabou o compasso...
	if (this->currentTick == 0) {
		logFile << "INFO: Fim do Compasso " << this->currentBar << endl;
		this->currentBar ++;

		if (this->currentBar == this->maxBars) {
			stop();

		} else if (this->currentBar == (this->maxBars - 1)) {
			this->sequencerBuffer.changeBuffer();

		} else {
			this->sequencerBuffer.changeBuffer();
			informNextBar();
		}
	}
}


void __fastcall CbMixerAgent::executeEvents(FmMidiOutput* out, const Vector_pMidiEvent* evts) {
	FmMidiChannelEvent* ev;

	for (int i = 0; i < evts->size(); i ++) {

		if ((*evts)[i]->isChannelEvent()) {
			ev = static_cast<FmMidiChannelEvent*>((*evts)[i]);

			switch (ev->getType()) {
			case MD_NOTE_ON:
				out->playNote(ev->getChannel(), ev->getVelocity(), ev->getPitch());
				break;
			case MD_NOTE_OFF:
				out->stopNote(ev->getChannel(), ev->getPitch());
				break;
			case MD_PROG_CHANGE:
				out->setInstrument(ev->getChannel(), ev->getPatchNumber());
				break;
			}
		}

	}
}

void CbMixerAgent::startRecording() {
	this->semaphore->Acquire();

   	logFile << "INFO: Preparando mixer para gravaçao..." << endl;

	if (this->recording == true) {
		delete this->fileOutput;
	}

	this->recording = true;
	this->fileOutput = new FmMidiFile(1, 480);  // uma track para meta-eventos (set-tempo e time-signature)

	// seta a metrica
	if (meter->isRegular()) {
		int numBeats;
		if (meter->getTatumsInBeat(0) == 4) {
			this->fileOutput->setDenominator(2); // quarter-note (1/2^2)
			numBeats = meter->getNumQuarterNotes();
		} else {
			this->fileOutput->setDenominator(3); // eigth-note (1/2^3)
			numBeats = 2 * meter->getNumQuarterNotes();
		}
		this->fileOutput->setNumerator(numBeats);
	}

	// define tracks de cada agente aqui
	map<AID,ProducerInfo*>::iterator it;
	for (it = players.begin(); it != players.end(); it ++) {
		it->second->track = this->fileOutput->createTrack();
       	logFile << "INFO: Track \"" << it->second->track << "\" criada "
                << "para o agente \"" << it->first->getName() << "\"" << endl;
	}

	this->semaphore->Release();
}

void CbMixerAgent::stopRecording() {
	this->recording = false;
}

bool CbMixerAgent::isRecording() {
	return this->recording;
}

FmMidiFile* CbMixerAgent::getRecord() {
	FmMidiFile *res = NULL;
	if (this->executionStatus == ES_STOPPED) {
		res = this->fileOutput;
	}
	return res;
}

void CbMixerAgent::toggleMute() {
	this->muted = !this->muted;
}

bool CbMixerAgent::isMuted() {
	return this->muted;
}

bool __fastcall CbMixerAgent::startMMTimer() {
	MMRESULT result;

	result = timeSetEvent(this->timerPeriod, this->timerResolution, timerCallBack, (DWORD)this, TIME_PERIODIC);
	if (result != NULL)	{
		this->timerId = (UINT)result;
	}

	return result != NULL;
}

bool __fastcall CbMixerAgent::stopMMTimer() {
	MMRESULT	result;

	result = timeKillEvent(this->timerId);
	if (TIMERR_NOERROR == result) {
		timerId = 0;
	}

	return TIMERR_NOERROR == result;
}

int CbMixerAgent::getBarsCycle() {
	return this->maxBars;
}

void CbMixerAgent::setBarsCycle(int n) {
	this->maxBars = n;
}

void CbMixerAgent::start(bool recording) {
	if (this->executionStatus == ES_STOPPED) {
		if (recording == true) {
			startRecording();
		}

		// artificio para dar um tempo ate os agentes mandarem frases
		this->currentBar = -1;
		this->currentTick = - resolution; // tempo de preparacao para receber frases

		this->informStart();        // informa do inicio (eminente) da performance
		this->informNextBar();      // pede para o compasso zero

		this->executionStatus = ES_PLAYING;
		if (this->OnStatusChange != NULL) {
			this->OnStatusChange(this);
		}

		this->startMMTimer();
		logFile << "INFO: Mixer em funcionamento ! " << endl;

	} else if (this->executionStatus == ES_PAUSED) {
		this->executionStatus = ES_PLAYING;
		if (this->OnStatusChange != NULL) {
			this->OnStatusChange(this);
		}

		this->startMMTimer();
		logFile << "INFO: Mixer retomando funcionamento ! " << endl;
	}
}

void CbMixerAgent::stop() {
	if (this->executionStatus != ES_STOPPED) {

		if (this->executionStatus != ES_PAUSED) {
			this->stopMMTimer();
		}

		if (this->recording == true) {
			this->stopRecording();

			// se nao terminou automaticamente...
			if (this->currentBar != this->maxBars) {
				int barsTicks = this->currentBar * int(meter->getNumQuarterNotes() * this->resolution);
				this->fileOutput->truncate(barsTicks + this->currentTick);
			}
		}

		this->currentBar = 0;
		this->currentTick = 0;

		sequencerBuffer.clear(); // limpa os buffers

		this->executionStatus = ES_STOPPED;
		if (this->OnStatusChange != NULL) {
			this->OnStatusChange(this);
		}

		logFile << "INFO: Mixer parado (stop) ! " << endl;
	}
}

void CbMixerAgent::pause() {
	if (this->executionStatus == ES_PLAYING) {
		this->stopMMTimer();
		this->executionStatus = ES_PAUSED;
		if (this->OnStatusChange != NULL) {
			this->OnStatusChange(this);
		}
		logFile << "INFO: Mixer pausado ! " << endl;
	}
}

ECbExecutionStatus CbMixerAgent::getStatus() {
	return this->executionStatus;
}

void CbMixerAgent::getDriversList(TStrings &resultList) {
	this->midiOutput->getDriversList(resultList);
}

int CbMixerAgent::getNumDrivers() {
	return this->midiOutput->getNumDrivers();
}

AnsiString CbMixerAgent::getDriverName(int i) {
	return this->midiOutput->getDriverName(i);
}

int CbMixerAgent::getDriver() {
	return this->midiOutput->getDriver();
}

void CbMixerAgent::setDriver(int driverIndex) {
	this->midiOutput->setDriver(driverIndex);
}

int CbMixerAgent::getTempo() {
	return TICKDURATION_TO_BPM(this->timerPeriod,this->resolution);
}

void CbMixerAgent::setTempo(int t) {
	if (t > 0) {
		if (this->executionStatus == ES_PLAYING) {
			stopMMTimer();
		}
		this->timerPeriod = BPM_TO_TICKDURATION(t,resolution);
		if (this->executionStatus == ES_PLAYING) {
			startMMTimer();
		}

		if (this->recording == true) {
			int fileRes = this->fileOutput->getResolution();
			FmMidiMetaEvent* settempo = FmMidiEvent::createSetTempo((double)t, this->currentTick);
			int offsetBars = int( meter->getNumQuarterNotes() * fileRes * this->currentBar );

			this->fileOutput->addEvent(settempo,
					CONVERTTICK(this->currentTick,this->resolution,fileRes), offsetBars);

			delete settempo;
		}
	}
}

int CbMixerAgent::getResolution() {
	return this->resolution;
}

void CbMixerAgent::setResolution(int res) {
	if (this->executionStatus == ES_STOPPED) {
		this->resolution = res;
	}
}

const CbMeter* CbMixerAgent::getMeter() {
	return meter;
}

void CbMixerAgent::setMeter(const CbMeter& mtr) {
	if (this->executionStatus == ES_STOPPED) {
		delete this->meter;
		this->meter = new CbMeter(mtr);
	}
}


/////// Classe CbMixerAgent::SequencerBuffer ///////

CbMixerAgent::SequencerBuffer::SequencerBuffer() {
	this->currBuffer = 0;
	this->buffer[0] = new ArrayNotes(true);
	this->buffer[1] = new ArrayNotes(true);
}

CbMixerAgent::SequencerBuffer::~SequencerBuffer() {
	delete this->buffer[0];
	delete this->buffer[1];
}

CbMixerAgent::ArrayNotes* CbMixerAgent::SequencerBuffer::getCurrBuffer() {
	return this->buffer[currBuffer];
}

CbMixerAgent::ArrayNotes* CbMixerAgent::SequencerBuffer::getNextBuffer() {
	return this->buffer[(currBuffer + 1) % 2];
}

void CbMixerAgent::SequencerBuffer::clear() {
	this->buffer[currBuffer]->clear();
	this->buffer[(currBuffer + 1) % 2]->clear();
}

void CbMixerAgent::SequencerBuffer::changeBuffer() {
	this->buffer[currBuffer]->clear();
	this->currBuffer = (this->currBuffer + 1) % 2;
}

void CbMixerAgent::SequencerBuffer::addPhrase(const FmMidiFileTrack& track, bool curr) {
	Vector_pMidiEvent events;
	ArrayNotes* destBuffer;

	track.getAllEvents(events);

	if (curr == true) {
		destBuffer = this->buffer[currBuffer];
	} else {
		destBuffer = this->buffer[(currBuffer + 1) % 2];
	}

	Vector_pMidiEvent* aux;
	int tick;

	for (int i = 0; i < events.size(); i ++) {
		tick = events[i]->getTick();
		aux = destBuffer->get(tick);
		if (aux == NULL) {
			aux = new Vector_pMidiEvent;
			destBuffer->add(aux,tick);
		}
		aux->push_back(events[i]->clone());
	}

}


/////// Funcoes para manipular XML ///////

FmMidiFileTrack* readXmlPhrase(const TiXmlElement& phraseEl) {
	const TiXmlElement *noteEl;
	int instr = -1, resolution;
	int pitch, dynamic, starttick, endtick;
	int ch = 1;
	FmMidiFileTrack* miditr = NULL;

	if (phraseEl.QueryIntAttribute(XML_ATTR_PHRASE_RESOLUTION, &resolution) != TIXML_SUCCESS) {
		logFunc << "ERRO: Resolucao não especificada !!!" << endl << endl;
		return NULL;
	}

	miditr = new FmMidiFileTrack(resolution);

	if (phraseEl.QueryIntAttribute(XML_ATTR_PHRASE_PATCH, &instr) != TIXML_SUCCESS) {
		logFunc << "ERRO: Instrumento não especificado !!!" << endl << endl;
		delete miditr;
		return NULL;
	} else {
		if (instr == -1) {
			ch = 9; //canal 9 para percussão, 1 para outro
		} else {
			ch = 1;
			miditr->addEvent(FmMidiEvent::createProgChange(instr, 0, ch));
		}
	}

	noteEl = phraseEl.FirstChildElement(XML_NODE_NOTE);

	while (noteEl != NULL) {

		if (noteEl->QueryIntAttribute(XML_ATTR_NOTE_PITCH, &pitch) != TIXML_SUCCESS) {
			logFunc << "ERRO: Pitch não especificado !!!" << endl << endl;
		}

		if (noteEl->QueryIntAttribute(XML_ATTR_NOTE_DYNAMIC, &dynamic) != TIXML_SUCCESS) {
			dynamic = 100; //valor default
		}

		if (noteEl->QueryIntAttribute(XML_ATTR_NOTE_STRTICK, &starttick) != TIXML_SUCCESS) {
			logFunc << "WRNG: Start tick não especificado !!!" << endl;
		} else {
			miditr->addEvent(FmMidiEvent::createNoteOn(pitch, dynamic, starttick, ch));
		}

		if (noteEl->QueryIntAttribute(XML_ATTR_NOTE_ENDTICK, &endtick) != TIXML_SUCCESS) {
			logFunc << "WRNG: End tick não especificado !!!" << endl;
		} else {
			miditr->addEvent(FmMidiEvent::createNoteOff(pitch, dynamic, endtick, ch));
		}

		noteEl = noteEl->NextSiblingElement(XML_NODE_NOTE);
	}

	return miditr;
}

TiXmlElement* writeXmlPhrase(const FmMidiFileTrack& phrase, int startTick, int endTick) {
	TiXmlElement* phraseEl = new TiXmlElement(XML_NODE_PHRASE);
	TiXmlElement* noteEl;
	Vector_pMidiEvent events;
	FmMidiChannelEvent* ev;

	if (endTick == -1) {
		endTick = phrase.getMaxTick();
	}

	phrase.getAllEvents(events);

	for (int j = 0; j < events.size(); j ++) {
		if ( (events[j]->getTick() <= endTick)  &&  (events[j]->isChannelEvent()) ) {
			ev = static_cast<FmMidiChannelEvent*>(events[j]);

			switch (ev->getType()) {
			case MD_NOTE_ON:
				noteEl = new TiXmlElement(XML_NODE_NOTE);
				noteEl->SetAttribute(XML_ATTR_NOTE_PITCH, ev->getPitch());
				noteEl->SetAttribute(XML_ATTR_NOTE_DYNAMIC, ev->getVelocity());
				noteEl->SetAttribute(XML_ATTR_NOTE_STRTICK, (ev->getTick() - startTick));
                if ((ev->getTick() - startTick) < 40000) {
       				phraseEl->InsertEndChild(*noteEl);
                } else {
	                logFunc << "BUG: Note on com tick muito alto! Tick: " << (ev->getTick() - startTick) << endl;
                }
				delete noteEl;
				break;
			case MD_NOTE_OFF:
				noteEl = new TiXmlElement(XML_NODE_NOTE);
				noteEl->SetAttribute(XML_ATTR_NOTE_PITCH, ev->getPitch());
				noteEl->SetAttribute(XML_ATTR_NOTE_ENDTICK, (ev->getTick() - startTick));
                if ((ev->getTick() - startTick) < 40000) {
       				phraseEl->InsertEndChild(*noteEl);
                } else {
	                logFunc << "BUG: Note off com tick muito alto! Tick: " << (ev->getTick() - startTick) << endl;
                }
				delete noteEl;
				break;
			case MD_PROG_CHANGE:
				phraseEl->SetAttribute(XML_ATTR_PHRASE_PATCH, ev->getPatchNumber());
				break;
			}
		}

	} //for

	if (phraseEl->Attribute(XML_ATTR_PHRASE_PATCH) == NULL) {
		phraseEl->SetAttribute(XML_ATTR_PHRASE_PATCH, -1);
	}

	phraseEl->SetAttribute(XML_ATTR_PHRASE_RESOLUTION, phrase.getResolution());

	return phraseEl;
}


