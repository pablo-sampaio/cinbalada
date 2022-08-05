
#include "FmAgent.h"

#include "FmBehaviour.h"
#include "FmTContainer.h"
#include "FmMessageQueue.h"

using namespace std;


FmAgent::FmAgent(const string &ln, bool createSuspended, bool autoDelete) {
	this->agentID = NULL;
    this->msgQueue = new FmMessageQueue;

    this->suspendOnCreation = createSuspended;
    this->autoDeletion = autoDelete;
	this->currentBehaviour = 0;

    FmTContainer::getInstance()->addAgent(this, ln);
}

FmAgent::FmAgent() {
	this->agentID = NULL;
    this->msgQueue = new FmMessageQueue;

    this->suspendOnCreation = false;
    this->autoDeletion = true;
	this->currentBehaviour = -1;

    FmTContainer::getInstance()->addAgent(this, "");
}

FmAgent::~FmAgent() {
	Vector_pBehaviour::iterator it;
	for (it = behaviours.begin(); it != behaviours.end(); it ++) {
        delete *it;
	}
    delete msgQueue;
}

void FmAgent::setID(AID id) {
    if (this->agentID == NULL) {
        this->agentID = id;
    }
}

FmTContainer* FmAgent::getContainer() {
    return FmTContainer::getInstance();
}

bool FmAgent::createSuspended() {
    return suspendOnCreation;
}

bool FmAgent::autoDelete() {
    return autoDeletion;
}

void FmAgent::setAutoDelete(bool del) {
	this->autoDeletion = del;
}

void FmAgent::takeDown() {
	//user defined
}

void FmAgent::addBehaviour(FmBehaviour *b) {
	b->setAgent(this);
	behaviours.push_back(b);
    this->remainingBehaviours = 0; //invalida o iterador, pq alterou o tamanho
	this->doActivate();
}

void FmAgent::removeBehaviour(FmBehaviour *b) {
	int i;
    int size = behaviours.size();
    bool found = false;

	for (i = 0; i < size; i ++) {

		if (behaviours[i] == b ) {
            found = true;
            this->remainingBehaviours = 0; //invalida o iterador, pq alterou o tamanho
            if (i < currentBehaviour) {
                //evita que o currentBehaviour pule um elemento
                currentBehaviour --;
            }
			break;
		}
	}

    if (found) {
        Vector_pBehaviour::iterator it;
        for (it = behaviours.begin(); it != behaviours.end(); it++) {
            if (*it == b) {
                behaviours.erase(it);
                delete(b);
                break;
            }
        }
    }

}

AID FmAgent::getID() const {
	return agentID;
}

void FmAgent::doActivate() {
    setStatus(AG_ACTIVE);
}

void FmAgent::doDelete() {
    setStatus(AG_TO_DELETE);
}

void FmAgent::doSuspend() {
    setStatus(AG_SUSPENDED);
}

void FmAgent::doWait() {
    setStatus(AG_WAITING);
}

void FmAgent::doWake() {
	setStatus(AG_ACTIVE);
}

void FmAgent::setStatus(EFmAgentStatus st) {
	FmTContainer::getInstance()->setStatus(this, st);
}

void FmAgent::resetBehaviourIterator() {
    this->remainingBehaviours = this->behaviours.size();
}

FmBehaviour* FmAgent::getNextBehaviour() {
	FmBehaviour *b = NULL;
	if (remainingBehaviours > 0) {
		b = behaviours[currentBehaviour % behaviours.size()];
		currentBehaviour ++;
		remainingBehaviours --;
	}
	return b;
}

bool FmAgent::hasNextBehaviour() {
    return this->remainingBehaviours > 0;
}

string FmAgent::getName() const {
	return this->agentID->getName();
}

void FmAgent::setName(string name) {
	FmTContainer::getInstance()->setName(this, name);
}

void FmAgent::includeMessage(FmAclMessage *msg) {
	msgQueue->includeMessage(msg);

	Vector_pBehaviour::iterator it;
	for (it = behaviours.begin(); it != behaviours.end(); it++) {
		if ( (*it)->getStatus() == BHV_BLOCKED ) {
			(*it)->setStatus(BHV_ACTIVE);
		}
	}

}

FmAclMessage* FmAgent::receive() {
	return msgQueue->getNextMessage();
}

FmAclMessage* FmAgent::receive(const FmMessageTemplate &mt) {
	return msgQueue->getNextMessage(mt);
}

void FmAgent::send(FmAclMessage &msg) {
    msg.setSender(getID());
	FmTContainer::getInstance()->sendMessage(&msg);
}

void FmAgent::sendNotUnderstoodReply(FmAclMessage* msg) {
    FmAclMessage* res = new FmAclMessage(FP_NOT_UNDERSTOOD);

    res->addReceiver(msg->getSender());
    res->setInReplyTo(msg->getReplyWith());
    res->setContent(msg->getContent());

    send(*res);
    delete res;
}

