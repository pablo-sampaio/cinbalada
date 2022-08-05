
#ifndef __FMACLMESSAGE_H__
#define __FMACLMESSAGE_H__

#include "util.h"
#include "FmAID.h"

/**
 * FIPA performatives, to identify the 'speech acts'
 * in messages sent by the FAMA agents.
 */
typedef enum {
  FP_UNKNOWN = -1,
  FP_ACCEPT_PROPOSAL = 0,
  FP_AGREE = 1,
  FP_CANCEL = 2,
  FP_CFP = 3,
  FP_CONFIRM = 4,
  FP_DISCONFIRM = 5,
  FP_FAILURE = 6,
  FP_INFORM = 7,
  FP_INFORM_IF = 8,
  FP_INFORM_REF = 9,
  FP_NOT_UNDERSTOOD = 10,
  FP_PROPAGATE = 11,
  FP_PROPOSE = 12,
  FP_PROXY = 13,
  FP_QUERY_IF = 14,
  FP_QUERY_REF = 15,
  FP_REFUSE = 16,
  FP_REJECT_PROPOSAL = 17,
  FP_REQUEST = 18,
  FP_REQUEST_WHEN = 19,
  FP_REQUEST_WHENEVER = 20,
  FP_SUBSCRIBE = 21 } EFmFipaPerformative;


/**
 * This class represents messages that can be sent and received by agents.
 * Every message has a FIPA performative field to identify its intention, as
 * well as the following fields:
 *   - sender: agent who sends the message (auto assigned by 'FmAgent::send' methd)
 *   - receivers: agents that will receive the message
 *   - content: the message itself
 *   - language: the language used in the content field
 *   - conversationId: to identify the flow of messages
 *   - replyWith: a label to be used in replies to the message
 *   - inReplyTo: identifies what message the current message replies (if any)
 */

class FmAclMessage {

private:
	static const char* PERF_NAMES[];

	const FmAID* sender;
	EFmFipaPerformative performative;
	Vector_AID receivers;
	Vector_AID replyTo;

	std::string content;
	std::string conversationId;
	std::string language;
	std::string inReplyTo;
	std::string replyWith;

public:
	FmAclMessage(EFmFipaPerformative performative);

	static const char *getPerformativeName(EFmFipaPerformative p);
	virtual ~FmAclMessage();

	void addReceiver(const FmAID *r);
	void addReplyTo(FmAID *dest);
	void clearAllReceivers();
	void clearAllReplyTo();

	void setSender(const FmAID *sender);
	void setPerformative(EFmFipaPerformative perf);

	void setContent(const std::string &content);
	void setConversationId(const std::string &id);
	void setInReplyTo(const std::string &reply);
	void setLanguage(const std::string &language);
	void setReplyWith(const std::string &rwith);

	void setContent(const char* content);
	void setConversationId(const char* id);
	void setInReplyTo(const char* inreply);
	void setLanguage(const char* language);
	void setReplyWith(const char* rwith);

	const FmAID* getSender() const;
	EFmFipaPerformative getPerformative() const;
    const char *getPerformativeName() const;

    // methods to read the attributes
	std::string& getContent();
	std::string& getConversationId();
	std::string& getInReplyTo();
	std::string& getLanguage();
	std::string& getReplyWith();

    // methods to read the attributes (const versions)
	const std::string& getContent() const;
	const std::string& getConversationId() const;
	const std::string& getInReplyTo() const;
	const std::string& getLanguage() const;
	const std::string& getReplyWith() const;

    // methods to iterate the receivers
    int getNumReceivers() const;
    const FmAID* getReceiver(int i) const;

    // methods to iterate the "replyTo" adresses
    int getNumReplyTo() const;
    const FmAID* getReplyTo(int i) const;

    /* {deallocation of result}: user
     */
	FmAclMessage* createReply(EFmFipaPerformative p = FP_UNKNOWN) const;

	friend std::ostream& operator<<(std::ostream& o, const FmAclMessage& a);

};

#endif