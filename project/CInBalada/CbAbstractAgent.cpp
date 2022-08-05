
#include "CbAbstractAgent.h"

#include "CbMixerAgent.h"

using namespace std;


CbAbstractAgent::CbAbstractAgent(string name, bool susp, bool autoDel)
	: FmAgent(name, susp, autoDel)
{
	this->mixerAgent = NULL;

	this->logStream = NULL;
	this->deleteStream = false;

	this->OnStatusChange = NULL;

	// encontra o "mixer-agent"
	FmServiceDescription servcDesc;
	servcDesc.setName(MIXER_AG_SERVC_NAME);
	Vector_AID result;

	getContainer()->searchService(servcDesc, result);

	this->mixerAgent = result.at(0);
}

CbAbstractAgent::~CbAbstractAgent()
{
	if (deleteStream & logStream != NULL) {
		delete logStream;
	}
}

void CbAbstractAgent::setStatus(EFmAgentStatus st) {
	FmAgent::setStatus(st);
	if (OnStatusChange != NULL) {
		OnStatusChange(this);
	}
}

void CbAbstractAgent::setLogStream(ofstream* logof, bool autoDelete) {
	if (this->logStream != NULL && this->deleteStream) {
		delete this->logStream;
	}
	this->logStream = logof;
	this->deleteStream = autoDelete;
}

ofstream* CbAbstractAgent::getLogStream(bool prompt) {
    if (prompt) {
        (*logStream) << endl << "[" << this->getID()->getName() << "] - ";
    }
    return logStream;
}

ofstream& CbAbstractAgent::log(bool prompt) {
    if (prompt) {
        (*logStream) << endl << "[" << this->getID()->getName() << "] - ";
    }
    return (*logStream);
}

void CbAbstractAgent::getMusiciansIds(Vector_AID &result) {
	// consulta pelo servico...
	FmServiceDescription servcDesc;
	servcDesc.setType(CINBALADA_AGENTS_SERVC_TYPE);
	getContainer()->searchService(servcDesc, result);

	// remove o proprio agente (this)...
	Vector_AID::iterator it;
	for (it = result.begin(); it != result.end(); it ++) {
		if (*(*it) == *(this->getID())) {
			result.erase(it);
			break;
		}
	}
}

void CbAbstractAgent::registerAsMusician(const char* instr) {
	FmServiceDescription servcDesc;
	servcDesc.setType(CINBALADA_AGENTS_SERVC_TYPE);
	servcDesc.setName(instr);
	getContainer()->addService(this, servcDesc);
}

void CbAbstractAgent::unregisterAsMusician() {
	FmServiceDescription servcDesc;
	servcDesc.setType(CINBALADA_AGENTS_SERVC_TYPE);
	getContainer()->deleteService(this, servcDesc);
}

void CbAbstractAgent::sendMusicians(FmAclMessage* acl, AID not) {
	Vector_AID banda;

	getMusiciansIds(banda);

	int size = banda.size();
	bool added = false;

	for (int i =0; i < size; i++) {
		if (not == NULL || *(banda[i]) != (*not)) {
			acl->addReceiver(banda[i]);
			added = true;
		}
	}

	if (added) send(*acl);
}

void CbAbstractAgent::registerInMixer() {
	FmAclMessage* snd = new FmAclMessage(FP_REQUEST);
	snd->setContent("register as music producer");
	snd->addReceiver(mixerAgent);
	send(*snd);

	delete snd;
}

void CbAbstractAgent::unregisterFromMixer() {
	FmAclMessage *msg = new FmAclMessage(FP_REQUEST);
	msg->setContent("unregister as music producer");
	msg->addReceiver(mixerAgent);
	send(*msg);
	delete msg;
}
