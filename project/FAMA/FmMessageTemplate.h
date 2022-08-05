
#ifndef __FMMESSAGETEMPLATE_H__
#define __FMMESSAGETEMPLATE_H__

#include "util.h"
#include "FmAID.h"
#include "FmAclMessage.h"


typedef enum EFmTemplateType {TT_CONTENT, TT_CONVERSATION_ID,
				TT_IN_REPLY_TO, TT_PERFORMATIVE, TT_REPLY_WITH,
				TT_SENDER, TT_NOT, TT_AND, TT_OR} TemplateType;


/**
 * Template used to filter incoming messages. Messages templates can
 * be used in 'receive' operations to ignore undesired messages.
 * This class uses the "Factory" design pattern.
 */
class FmMessageTemplate {

private:
	std::string content;
	std::string conversationId;
	std::string inReplyTo;
	EFmFipaPerformative performative;
	std::string replyWith;
	const FmAID* sender;
	EFmTemplateType type;
	FmMessageTemplate *msgTemplate1;
	FmMessageTemplate *msgTemplate2;

	FmMessageTemplate();

public:
	bool matches(FmAclMessage *message) const;

	/* Methods to create message templates ("factory" design pattern).
	 * {deallocation of result}: caller
	 */

	static FmMessageTemplate* matchContent(std::string &content);
	static FmMessageTemplate* matchContent(const char* content);

	static FmMessageTemplate* matchConversationId(std::string &id);
	static FmMessageTemplate* matchConversationId(const char* id);

	static FmMessageTemplate* matchInReplyTo(std::string &inReplyTo);
	static FmMessageTemplate* matchInReplyTo(const char* inReplyTo);

	static FmMessageTemplate* matchPerformative(EFmFipaPerformative performative);

	static FmMessageTemplate* matchReplyWith(std::string &replyWith);
	static FmMessageTemplate* matchReplyWith(const char* replyWith);

	static FmMessageTemplate* matchSender(AID sender);

	/* Acess methods.
	 */
	const std::string& getContent() const;
	const std::string& getConversationId() const;
	const std::string& getInReplyTo() const;
	const std::string& getReplyWith() const;
	EFmFipaPerformative getPerformative() const;
	AID getSender() const;

	/* Methods to create composite message templates.
	 * Atention: when the 'father' template is destroyed, all 'sons'
	 * are also destroyed.
	 * {deallocation of parameters}: this class
	 * {deallocation of result}: caller
     */
	static FmMessageTemplate* and(FmMessageTemplate* m1, FmMessageTemplate* m2);
	static FmMessageTemplate* or(FmMessageTemplate* m1, FmMessageTemplate* m2);
	static FmMessageTemplate* not(FmMessageTemplate* m1);

    /* Destructor. Operands are also deleted when the template is OR, AND or NOT.
     */
    virtual ~FmMessageTemplate();
};

#endif