//---------------------------------------------------------------------------

#ifndef __CB_PERCAGENT_NEGOTIATION_H______
#define __CB_PERCAGENT_NEGOTIATION_H______

#include "FAMA\FmBehaviour.h"
#include "CInBalada\RhythmPattern\CbRhythmicRole.h"

//---------------------------------------------------------------------------

class CbPercussionistAgent;

/**
 * Behaviour that implements the negotiation between
 * agents of the same role.
 */
class CbPercAgentNegotiation : public FmBehaviour {
private:

	struct AgentInfo {
		AID id;                    // the identifier/address of an agent
		CbRhythmPattern *pattern;  // the pattern chosen by the agent
		bool fixed;                // indicates whether the agent can change its pattern
		double evaluation;         // the general evaluation of its pattern
		AgentInfo(AID aid) : id(aid), pattern(NULL), fixed(false), evaluation(0) {}
		virtual ~AgentInfo() { delete pattern; }
	};

	typedef enum {REQ_ROLES, WAIT_ROLES, CHOOSE_1ST_PTRN, REF_SYNC, CHOOSE_PTRN,
					REQ_PATRN_REF, WAIT_PTRN_REF, REQ_PTRN, WAIT_PTRN,
					SEND_RESULT, CMP_RESULTS, SEND_PTRN_MXR, FINISHED} EState;

	EState state;

	const CbRhythmicRole* role;
	FmMessageTemplate* onlyNegotiationMessages;
	FmMessageTemplate* onlyRepliesToRequests;

	int bar;
	char barStr[10];

	int pendingAnswers;

	double evaluation;      // evaluation of the current-pattern compared to related agents
	double bestEvaluation;  // best evaluation found between the related agents

	typedef std::vector<AgentInfo*> VectorAgentInfo;
	typedef std::map<AID,CbRhythmPattern*> MapAgentPattern;

	VectorAgentInfo agentInfos;         // agents of same role
	MapAgentPattern referencePatterns;  // agents of reference role
	Vector_AID agentsToNotify;          // agents to which 'role' is reference


	/* Functions to control the array with information
	 * about the agents in the band.
	 */
	void addAgInfo(AID id);
	AgentInfo* getAgInfo(int i);
	AgentInfo* getAgInfo(AID id);
	int getNumAgInfos();
	AgentInfo* getAgInfoNotFixed(int pos);
	int getNumAgInfosNotFixed();

	/* Functions to control temporary (uncommited) choices of pattern.
	 */
	void setCurrentPattern(int roleIndex);
	int getCurrentPattern();
	int getCurrentPatternMainIndex();
	int getCurrentPatternBar();

	/* This functions erases the previously chosen pattern from the historic
	 * (used before starting the proccess to choose a new pattern, after the
	 * first round of the negotiation).
	 */
	void clearCurrentPattern();

	/* This function makes the temporary choice the definitive one.
	 */
	void commitCurrentPattern();

	/* Avaliates the current pattern in face of related agents' patterns.
	 */
	double evaluatePattern(int patrnMainIndex);

public:

	CbPercAgentNegotiation(CbPercussionistAgent* ag, const CbRhythmicRole* __role, int barIndex);

	virtual ~CbPercAgentNegotiation();

	inline CbPercussionistAgent* getPercAgent();
	std::ofstream& log(bool prompt=true);

	virtual void action();
	void actionRequestRoles();
	void actionWaitForRoles();
	void actionChooseFirstPattern();
	void actionSynchronizeWithReference();
	void actionChoosePattern();
	void actionRequestPatternsRef();
	void actionWaitForPatternsRef();
	void actionRequestPatterns();
	void actionWaitForPatterns();
	void actionSendEvaluationResult();
	void actionCompareResults();
	void actionSendPatternToMixer();

	virtual bool done();

};


//---------------------------------------------------------------------------
#endif

 