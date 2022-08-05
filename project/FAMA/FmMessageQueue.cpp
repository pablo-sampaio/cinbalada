
#include "FmMessageQueue.h"


FmMessageQueue::FmMessageQueue() {
	myAgent = NULL;
}

FmMessageQueue::~FmMessageQueue() {

}

void FmMessageQueue::includeMessage(FmAclMessage *msg) {
	msgBox.push_back(msg);
}

int FmMessageQueue::getNumMessages() {
    return msgBox.size();
}

FmAclMessage* FmMessageQueue::getNextMessage() {
	FmAclMessage *msg = NULL;

	if (msgBox.size() > 0) {
		Vector_pAclMessage::iterator first = msgBox.begin();
		msg = *first;
		msgBox.erase(first);
	}

	return msg;
}

FmAclMessage* FmMessageQueue::getNextMessage(const FmMessageTemplate &mt) {
	FmAclMessage *current = NULL;
	FmAclMessage *msg = NULL;
	Vector_pAclMessage::iterator it;

	for (it = msgBox.begin(); it != msgBox.end(); it++) {
		current = *it;

		if (mt.matches(current)) {
			msg = current;
			msgBox.erase(it);
			break;
		}

	}

	return msg;
}