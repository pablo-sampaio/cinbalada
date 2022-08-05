
#ifndef _____CB_RANDOM_AGENT_H____
#define _____CB_RANDOM_AGENT_H____

#include "CbAbstractAgent.h"
#include "CbPercRepertoire.h"

/**
 * Percussionists agents that choose randomly one pattern
 * of his repertoire.
 */
class CbRandomAgent : public CbAbstractAgent
{
private:
	CbPercRepertoire* repertoire;   // the rhythm patterns the agent can play
	int currentPattern;

	// copied form CbPercussionistAgent
	void createMessageContentFromPattern(int patternMainIndex, int bar, FmAclMessage& result);

public:

	CbRandomAgent(std::string name, CbPercRepertoire *rep);
	virtual ~CbRandomAgent();

	/* Randomly chooses a pattern.
	 */
	int choosePattern();

	/* Returns last chosen pattern.
	 */
	int getCurrentPattern();

	/* Implementacao do metodo da classe FmAgent.
     * Inicia um 'RandomAgentBehavour'.
     */
	void setup();


    /**
     * Implements the behaviour of the "random-agent". It simply
     * register the agent in the "outputter-agent" and chooses
     * randomly a rhythm pattern to send to output.
     */
	class RandomAgentBehaviour : public FmCyclicBehaviour {
	private:
		CbRandomAgent* agent;
	public:
		RandomAgentBehaviour(CbRandomAgent*);
		std::ofstream& log(bool prompt=true);
		void onStart();
		void action();
		void onEnd();
    };

    friend class RandomAgentBehaviour;

};

#endif /* _____CB_RANDOM_AGENT_H____ */
