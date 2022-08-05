
#include "CbMutedMixerAgent.h"

#include <fstream>
#include "Util/MathUtil.h"

using namespace std;

// arquivo de log
static ofstream logFile("logs\\CbMutedMixerAgent.txt", ios::out);


/////// Classe CbMutedMixerAgent ///////

CbMutedMixerAgent::CbMutedMixerAgent() {
	this->pendingAnswers = 0;
	logFile << "INFO: Mixer criado!" << endl;
}

CbMutedMixerAgent::~CbMutedMixerAgent() {
	logFile << "INFO: Mixer deletado !" << endl;
}

void CbMutedMixerAgent::setup() {
	FmServiceDescription servcDesc;
	servcDesc.setName(MIXER_AG_SERVC_NAME);
	servcDesc.setType(MIXER_AG_SERVC_TYPE);

	Vector_AID agents;

	getContainer()->searchService(servcDesc, agents);

	//testa se já existe um agente mixer...
	if (agents.size() > 0) {
		logFile << "ERRO: Ja existe um agente mixer ! " << agents[0] << endl;
		doDelete();

	} else {
		getContainer()->addService(this, servcDesc);
		addBehaviour(new MixerBehaviour(this));

		logFile << "INFO: Mixer pronto ! " << endl;

		if (this->OnSetup != NULL) {
			this->OnSetup(this);
		}
	}
}

void CbMutedMixerAgent::takeDown() {
	logFile << "INFO: Encerrando... " << endl;
}

void __fastcall CbMutedMixerAgent::collateBars(FmAclMessage* msg) {
	CbMixerAgent::collateBars(msg);
	this->pendingAnswers --;
}

void __fastcall CbMutedMixerAgent::informNextBar() {
	CbMixerAgent::informNextBar();
	this->pendingAnswers = this->players.size();
}

void __fastcall CbMutedMixerAgent::executeMidiTick() {

	if (this->executionStatus == ES_PAUSED) {
		return;
	}

	// se acabou o compasso...
	if (this->pendingAnswers == 0) {
		logFile << "INFO: Todas as respostas recebidas para compasso " << (this->currentBar + 1) << endl;
		this->currentBar ++;

		//if (this->currentBar == (this->maxBars - 1)) {   // PROBLEMA: DEVIA ESTAR CERTO, POREM DA ERRO NO ARQUIVO MIDI
		if (this->currentBar == this->maxBars) {
			stop();
		} else if (this->currentBar != (this->maxBars - 1)) {
			informNextBar();
		}

	} else {
		logFile << "WRNG: Sem resposta !" << endl;

    }
}
