//---------------------------------------------------------------------------

#ifndef __CB_PERCUSSIONIST_AGENT_H______
#define __CB_PERCUSSIONIST_AGENT_H______

#include "CbAbstractAgent.h"

#include "CbPercRepertoire.h"
#include "CbPercPerfHistory.h"
#include "CbPercPlan.h"

#include "CbPercAgentNegotiation.h"
//---------------------------------------------------------------------------


/* Contents of some messages exchanged between agents.
 */
#define MSG_REQ_ROLES         "reply roles"
#define MSG_REQ_RPATTERN      "current rhythm-pattern"

/* Default evaluation weigths.
 */
#define DEFAULT_VERT_EVAL_WEIGTH    0.4
#define DEFAULT_HORIZ_EVAL_WEIGTH   0.6


/**
 * Agent's behaviours forward definitions.
 */
class CbPercAgentBehaviour;


/**
 * Agent who plays a percussion instrument. Agents may assume
 * different rhtyhmic roles in each bar.
 */
class CbPercussionistAgent : public CbAbstractAgent
{
protected:

	friend CbPercAgentBehaviour;
	friend CbPercAgentNegotiation;

	CbPercPlan planRoles;           // the sequence of roles
	CbPercRepertoire* repertoire;   // the rhythm patterns the agent can play
	CbPercPerfHistory history;      // history of the performance

	double intensityScale;             // used for scaling the velocity of the MIDI notes
	double verticalEvaluationWeigth;   // weigth used in the vertical evaluation during negotiation
    double horizontalEvaluationWeigth; // weigth used in the horizontal evaluation during negotiation


	/* Changes the intensities of midi notes in the given track and returns
	 * a new track with the scaled (multiplied by a factor) intensities.
	 */
	FmMidiFileTrack* scaleIntensities(const FmMidiFileTrack* originalTrack);

	/* Converts the given rhtyhm pattern to XML and creates an INFORM message
	 * with it.  If parameter 'bar' is set a value higher than -1, only the
	 * given bar will be converted; if it is set to -1, the whole pattern will
	 * be converted.
	 * {deallocation of result}: caller
	 */
	FmAclMessage* createMessageFromPattern(int patternMainIndex, int bar = -1);

	/* Do the same as previous method, but the message her must have been
	 * previously created. Only 'content' and 'language' fields of the message
	 * are set.
	 */
	void createMessageContentFromPattern(int patternMainIndex, int bar, FmAclMessage& result);

	/* Reads the message created by one of the two previous methods.
	 */
	CbRhythmPattern* readPatternFromMessage(const FmAclMessage* msg);


public:

	/* Events that can be trigerred to inform of changes in
	 * the current rhythmic pattern of the agent.
	 */
	CbAgentEvent OnRPatternChange;

	/* Constructor.
	 * {Deallocation of 'rep'}: this class
	 * {Deallocation of 'defRole'}: external (automatic}
	 */
	CbPercussionistAgent(std::string name, CbPercRepertoire *rep,
						 const CbRhythmicRole* defRole, bool susp=false,
						 bool autoDel=true);

	/* Destructor.
	 */
	virtual ~CbPercussionistAgent();

	/* Returns the repertoire that the agent can play.
	 */
	CbPercRepertoire* getRepertoire();

	/* Returns the plan of the performance.
	 */
	CbPercPlan* getPlan();

	/* Historical information of an agent's performance, per bar.
	 */
	CbPercPerfHistory* getHistory();

	/* Methods to read/write the scaling factor for the dinamic parameter
	 * of notes (MIDI velocity).
	 */
	double getIntensityScale() const;
	void setIntensityScale(double w);

	/* Methods to read/write the evaluation weights used in the negotiation.
     */
	void setEvaluationWeigths(double vert, double horiz);
    double getVerticalEvaluationWeigth();
    double getHorizontalEvaluationWeigth();

	/* The percussion instrument that the agent plays.
	 */
	std::string getInstrumentName() const;

	/* Overriden from class 'FmAgent'. Adds the behaviours of the agent.
	 */
	virtual void setup();

	/* Overriden from class 'FmAgent'.
	 */
	virtual void takeDown();

	/* Random evaluation function to order the repertoire.
	 */
	static double random_eval(const CbRhythmPattern* rp, const CbRhythmPattern* ref);

};


/**
 * Main behaviour of the percussionist agent.
 */
class CbPercAgentBehaviour : public FmCyclicBehaviour {
private:
	FmMessageTemplate* msgTemplate;
	Vector_pAclMessage patternRequests;

public:

	CbPercAgentBehaviour(CbPercussionistAgent* ag);
	virtual ~CbPercAgentBehaviour();

	CbPercussionistAgent* getPercAgent();

	virtual void action();
	void clearPerformanceHistory();
	void createBehaviourForBar(FmAclMessage* acl);
	void processRoleRequest(FmAclMessage* acl);
	void processPatternRequest(FmAclMessage* acl);
	void processPendingPatternRequests();
	bool replyPatternRequest(FmAclMessage* acl, int bar);
};


//---------------------------------------------------------------------------
#endif
