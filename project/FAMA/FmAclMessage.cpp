

#include "FmAclMessage.h"

using namespace std;


const char* FmAclMessage::PERF_NAMES[] = {
   "UNKNOWN",           //0
   "ACCEPT_PROPOSAL",
   "AGREE",
   "CANCEL",
   "CFP",
   "CONFIRM",           //5
   "DISCONFIRM",
   "FAILURE",
   "INFORM",
   "INFORM_IF",
   "INFORM_REF",        //10
   "NOT_UNDERSTOOD",
   "PROPAGATE",
   "PROPOSE",
   "PROXY",
   "QUERY_IF",          //15
   "QUERY_REF",
   "REFUSE",
   "REJECT_PROPOSAL",
   "REQUEST",
   "REQUEST_WHEN",      //20
   "REQUEST_WHENEVER",
   "SUBSCRIBE"};


FmAclMessage::FmAclMessage(EFmFipaPerformative performative) {
	this->performative = performative;
}

FmAclMessage::~FmAclMessage() {

}

void FmAclMessage::addReceiver(const FmAID *r) {
	this->receivers.push_back(r);
}

void FmAclMessage::setContent(const string& content) {
	this->content = content;
}

void FmAclMessage::setConversationId(const string& id) {
	this->conversationId = id;
}

void FmAclMessage::setInReplyTo(const string &inReply) {
    this->inReplyTo = inReply;
}

void FmAclMessage::setLanguage(const string &language) {
    this->language = language;
}

void FmAclMessage::setPerformative(EFmFipaPerformative perf) {
    this->performative = perf;
}

void FmAclMessage::setReplyWith(const string &rwith) {
    this->replyWith = rwith;
}

void FmAclMessage::setSender(const FmAID *sender) {
	this->sender = sender;
}

void FmAclMessage::setContent(const char* content) {
    this->content = content;
}

void FmAclMessage::setConversationId(const char* id) {
    this->conversationId = id;
}

void FmAclMessage::setInReplyTo(const char* inreply) {
    this->inReplyTo = inreply;
}

void FmAclMessage::setLanguage(const char* language) {
    this->language = language;
}

void FmAclMessage::setReplyWith(const char* rwith) {
    this->replyWith = rwith;
}

string& FmAclMessage::getContent() {
    return this->content;
}

string& FmAclMessage::getConversationId() {
    return this->conversationId;
}

string& FmAclMessage::getInReplyTo() {
    return this->inReplyTo;
}

string& FmAclMessage::getLanguage() {
    return this->language;
}

string& FmAclMessage::getReplyWith() {
    return this->replyWith;
}

int FmAclMessage::getNumReceivers() const {
    return receivers.size();
}

const FmAID* FmAclMessage::getReceiver(int i) const {
    if (i >= 0 && i < receivers.size()) {
        return receivers[i];
    } else {
        return NULL;
    }
}

int FmAclMessage::getNumReplyTo() const {
    return replyTo.size();
}

AID FmAclMessage::getReplyTo(int i) const {
    if (i >= 0 && i < replyTo.size()) {
        return replyTo[i];
    } else {
        return NULL;
    }
}

const string& FmAclMessage::getContent() const {
	return content;
}

const string& FmAclMessage::getConversationId() const {
	return this->conversationId;
}

const string& FmAclMessage::getInReplyTo() const {
	return this->inReplyTo;
}

const string& FmAclMessage::getLanguage() const {
	return this->language;
}

EFmFipaPerformative FmAclMessage::getPerformative() const {
	return this->performative;
}

const char* FmAclMessage::getPerformativeName() const {
    return getPerformativeName(this->performative);
}

const string& FmAclMessage::getReplyWith() const {
	return this->replyWith;
}

AID FmAclMessage::getSender() const {
	return sender;
}

const char * FmAclMessage::getPerformativeName(EFmFipaPerformative p) {
	int i = (p < 0 || p > 21) ? 0 : (p+1);
	return PERF_NAMES[i];
}

FmAclMessage* FmAclMessage::createReply(EFmFipaPerformative p) const {
	FmAclMessage* m = new FmAclMessage(p);
	Vector_AID::const_iterator it = replyTo.begin();

	while (it != replyTo.end()) {
		m->addReceiver(*it);
		it ++;
	}

	if (this->replyTo.size() == 0) {
		m->addReceiver(getSender());
	}

	//obs: o sender do reply deve ser setado externamente
	//(ele eh setado automaticamente no metodo 'send' do agente)

	m->setLanguage(this->language);
	m->setInReplyTo(this->replyWith);

	m->setConversationId(this->conversationId);

	return m;
}

ostream& operator<<(ostream& o, const FmAclMessage& m) {
    int i;

	o << "- Message -------" << endl;
	o << "|\tFrom: " << m.sender->getName();

	o << endl << "|\tTo:";
	for (i = 0; i < m.getNumReceivers(); i ++) {
		o << " " << m.getReceiver(i)->getName();
	}

	o << endl << "|\tPerformative: " << m.getPerformativeName() << endl;
	o << "|\tContent: " << m.content << endl;

	return o;
}


