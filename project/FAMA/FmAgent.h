#ifndef __FMAGENT_H__
#define __FMAGENT_H__

#include "util.h"

#include "FmAID.h"


/**
 * This abstract class models agents of the framework FAMA. This class should
 * be inherited to implement application-specific agents. The tasks an agent
 * executes are implemented as behaviours (implementations of FmBehaviour).
 * At least one behaviour should be defined in the method 'setup', called when
 * the agent is initiated.
 * Each agent receives an unique identifier, given by the container when the
 * agent is created. Such identifier is used to indentiy sender an receiver(s)
 * of messages sent by the agents.
 * This class also provides methods to send/receive messages (asynchronously)
 * and methods to control the agent's life-cycle.
 */

class FmAgent {

private:
	AID agentID;
	Vector_pBehaviour behaviours;
	FmMessageQueue *msgQueue;
    bool suspendOnCreation;
    bool autoDeletion;

    // used to iterate the agent's behaviours
    int remainingBehaviours;
    int currentBehaviour;

protected:

    friend class FmTContainer;

    /* Used only by the system.
     */
    void setID(AID id);

    /* Method to include a message in the message queue of the agent.
     * Used only by the system.
     * {deallocation of parameter}: done by the caller
     */
	void includeMessage(FmAclMessage *msg);

    /* Method to remover a behaviour of the agent. Used only by the system.
     * (An agent implementer willing to remove a behaviour should make the
     * method 'done' of the class FmBehaviour return true in the desired
     * situation).
     */
	void removeBehaviour(FmBehaviour *b);

    /**
     * Methods to iterate the behaviour of an agent without repetitions,
     * starting in the point where previous iteration stopped.
     * To start iteration (from the point it stoppped last time), one should
     * call 'resetBehaviourIterator'. Then,  it's possible to use methods
     * 'hasNextBehaviour' and 'getNextBehaviour' to iterate all the agent's
     * behaviours.
     */
    void resetBehaviourIterator();
    FmBehaviour* getNextBehaviour();
    bool hasNextBehaviour();

    /* Indicates whether an agent should be created with status SUSPENDED set.
     * If false, agent will be created with status ACTIVE.
     */
    bool createSuspended();

    /* Change agent's status. Use methods 'do<State>' instead.
     */
    virtual void setStatus(EFmAgentStatus st);

public:

    /* Creates an agent with the given name. If createSuspended is true, agent
     * will have his 'setup' method executed and then will be suspended, until
     * the method "doActivate' is called (externally). Otherwise, it will be
     * activated automatically by the system.
     * If autodelete is true, the agent will be automatically deleted (deallocated)
     * by the system whenever it stops his execution. If false, the object should
     * be deleted externally (but only when agent's status is DELETED).
     */
    FmAgent(const std::string &ln, bool createSuspended=false, bool autoDelete=true);

    /* Creates an agent with a name given automatically by the system.
     * The agent will be created with ACTIVE status and will be automatically
     * deallocated.
     */
	FmAgent();

    /* Deallocates all behaviours.
     */
	virtual ~FmAgent();

	AID getID() const;
    std::string getName() const;

    FmTContainer* getContainer();

	void setName(std::string name);

    /* Method to send messages to other agents. The sender address is
     * automatically set by this method.
     */
	void send(FmAclMessage &msg);

	/* Methods to receive messages asynchronously.
	 * {deallocation of result}: caller
     */
	FmAclMessage* receive();
	FmAclMessage* receive(const FmMessageTemplate &mt);

    /* Method called when the agent is initiated. Must be implemented by
     * the agent developer. The implementation should add, at least, one
     * behaviour to the agent.
     */
	virtual void setup() = 0;

    /* Method called just before the agent is removed from the system.
     * It may be overriden to implement some cleanup necessary.
     */
	virtual void takeDown();

    /* Add new behaviours to be executed by the agent.
     */
	void addBehaviour(FmBehaviour *b);

    /* Sends a reply to an unexpected message.
     */
    void sendNotUnderstoodReply(FmAclMessage *msg);

    /* Methods to change the status of agents. Only active agents have
     * their behaviours executed by the system.
     */
	void doActivate();
	void doDelete();
	void doSuspend();
	void doWait();
	void doWake();

    /* Indicates if the agent should be automatically deleted (deallocated)
     * by the system when it stops his execution. If false, the object should
     * be deleted externally (when agent's status is DELETED).
     */
    bool autoDelete();
	void setAutoDelete(bool del);

};

#endif