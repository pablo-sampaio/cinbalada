
#ifndef __FMBEHAVIOUR_H__
#define __FMBEHAVIOUR_H__

#include "util.h"
#include "FmAgent.h"


typedef enum {BHV_NEW, BHV_ACTIVE, BHV_BLOCKED, BHV_DELETED} EFmBehaviourStatus;

/**
 * When an agent is created it has to create adequate behaviours in order
 * to do the desired tasks. This class represents the behaviours of an agent
 * should be inherited to implement the taks of an agent.
 */
class FmBehaviour {

protected:

	FmAgent *myAgent;
    EFmBehaviourStatus status;

public:
	FmBehaviour(FmAgent *a = NULL);
	virtual ~FmBehaviour();

	void setAgent(FmAgent *a);

    /* Action that is executed by the behaviour. This method is repeatedly
     * called until the method 'done' returns 'true'.
     * An agent designer must implement this method. The implementation
     * shouldn't have an infinite loop.
     */
	virtual void action() = 0;

	/* Returns 'true' to indicate that the behaviour has finished.
     * Should be implemented by the user.
	 */
	virtual bool done() = 0;

    /* This method is called before the first call to action method, as
     * soon as the beahviour is created.
     * Can be implemented by an user (agent designer).
     */
	virtual void onStart();

    /* This method is the last action executed by the behaviour.
     * After the method 'done' returns true, it is executed and then
     * the behaviour is deleted.
     * Can be implemented by an agent designer user.
     */
    virtual void onEnd();

	/* Blocks a behaviour. Blocked behaviours are unblocked when new
     * messages arrive.
	 */
	void block();

    /* Methods to query or change the status of the behaviour,
     * Should only be used by the system.
     */
	EFmBehaviourStatus getStatus();
	void setStatus(EFmBehaviourStatus st);

};



/**
 * This class is a behaviour whose 'action' method is
 * executed just once.
 */
class FmOneShotBehaviour : public FmBehaviour {
public:
	FmOneShotBehaviour(FmAgent *a = NULL) : FmBehaviour(a) {}
	bool done();
};


/**
 * This class is a behaviour whose 'action' method is executed
 * indefinitely (until the agent is deleted).
 */
class FmCyclicBehaviour : public FmBehaviour {
public:
	FmCyclicBehaviour(FmAgent *a = NULL) : FmBehaviour(a) {}
	bool done();
};


#endif
