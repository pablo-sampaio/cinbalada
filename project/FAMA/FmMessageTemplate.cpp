
#include "FmMessageTemplate.h"

#include "FmAclMessage.h"

using namespace std;


FmMessageTemplate::FmMessageTemplate() {
    msgTemplate1 = NULL;
    msgTemplate2 = NULL;
}

bool FmMessageTemplate::matches(FmAclMessage *message) const {
    bool result = false;

    switch (this->type) {
        case TT_CONTENT:
		result = (this->content == message->getContent());
		break;

		case TT_CONVERSATION_ID:
		result = (this->conversationId == message->getConversationId());
		break;

        case TT_IN_REPLY_TO:
		result = (this->inReplyTo == message->getInReplyTo());
		break;

		case TT_PERFORMATIVE:
		result = (this->performative == message->getPerformative());
		break;

		case TT_REPLY_WITH:
		result = (this->replyWith == message->getReplyWith());
		break;

		case TT_SENDER:
		result = (*this->sender == *message->getSender());
		break;

        case TT_NOT:
        result = ! msgTemplate1->matches(message);
        break;

        case TT_AND:
		result = msgTemplate1->matches(message) &&  msgTemplate2->matches(message);
        break;

        case TT_OR:
        result = msgTemplate1->matches(message) ||  msgTemplate2->matches(message);
        break;

		default:
		break;
	}

	return result;
}


FmMessageTemplate* FmMessageTemplate::matchContent(string& content) {
	FmMessageTemplate *ret = new FmMessageTemplate();

	ret->type = TT_CONTENT;
	ret->content = content;

	return ret;
}

FmMessageTemplate* FmMessageTemplate::matchContent(const char* content) {
	FmMessageTemplate *ret = new FmMessageTemplate();

	ret->type = TT_CONTENT;
	ret->content = content;

	return ret;
}

FmMessageTemplate* FmMessageTemplate::matchConversationId(string& id) {
	FmMessageTemplate *ret = new FmMessageTemplate();

	ret->type = TT_CONVERSATION_ID;
	ret->conversationId = id;

	return ret;
}

FmMessageTemplate* FmMessageTemplate::matchConversationId(const char* id) {
	FmMessageTemplate *ret = new FmMessageTemplate();

	ret->type = TT_CONVERSATION_ID;
	ret->conversationId = id;

	return ret;
}

FmMessageTemplate* FmMessageTemplate::matchInReplyTo(string& inReplyTo) {
	FmMessageTemplate *ret = new FmMessageTemplate();

	ret->type = TT_IN_REPLY_TO;
	ret->inReplyTo = inReplyTo;

	return ret;
}

FmMessageTemplate* FmMessageTemplate::matchInReplyTo(const char* inReplyTo) {
	FmMessageTemplate *ret = new FmMessageTemplate();

	ret->type = TT_IN_REPLY_TO;
	ret->inReplyTo = inReplyTo;

	return ret;
}

FmMessageTemplate* FmMessageTemplate::matchPerformative(EFmFipaPerformative performative) {
	FmMessageTemplate *ret = new FmMessageTemplate();

	ret->type = TT_PERFORMATIVE;
	ret->performative = performative;

	return ret;
}

FmMessageTemplate* FmMessageTemplate::matchReplyWith(string& replyWith) {
	FmMessageTemplate *ret = new FmMessageTemplate();

	ret->type = TT_REPLY_WITH;
	ret->replyWith = replyWith;

	return ret;
}

FmMessageTemplate* FmMessageTemplate::matchReplyWith(const char* replyWith) {
	FmMessageTemplate *ret = new FmMessageTemplate();

	ret->type = TT_REPLY_WITH;
	ret->replyWith = replyWith;

	return ret;
}

FmMessageTemplate* FmMessageTemplate::matchSender(const FmAID *sender) {
	FmMessageTemplate *ret = new FmMessageTemplate();

	ret->type = TT_SENDER;
	ret->sender = sender;

	return ret;
}

FmMessageTemplate* FmMessageTemplate::and(FmMessageTemplate* m1, FmMessageTemplate* m2) {
	FmMessageTemplate *ret = new FmMessageTemplate();

	ret->type = TT_AND;
	ret->msgTemplate1 = m1;
	ret->msgTemplate2 = m2;

	return ret;
}

FmMessageTemplate* FmMessageTemplate::or(FmMessageTemplate* m1, FmMessageTemplate* m2) {
	FmMessageTemplate *ret = new FmMessageTemplate();

	ret->type = TT_OR;
	ret->msgTemplate1 = m1;
	ret->msgTemplate2 = m2;

	return ret;
}

FmMessageTemplate* FmMessageTemplate::not(FmMessageTemplate* m1) {
    FmMessageTemplate *ret = new FmMessageTemplate();

    ret->type = TT_NOT;
    ret->msgTemplate1 = m1;

    return ret;
}

const std::string& FmMessageTemplate::getContent() const {
	return content;
}

const std::string& FmMessageTemplate::getConversationId() const {
	return conversationId;
}

const std::string& FmMessageTemplate::getInReplyTo() const {
	return inReplyTo;
}

const std::string& FmMessageTemplate::getReplyWith() const {
	return replyWith;
}

EFmFipaPerformative FmMessageTemplate::getPerformative() const {
	return performative;
}

AID FmMessageTemplate::getSender() const {
	return sender;
}

FmMessageTemplate::~FmMessageTemplate() {
	if (msgTemplate1 != NULL) {
		delete msgTemplate1;
	}
	if (msgTemplate2 != NULL) {
        delete msgTemplate2;
    }
}
