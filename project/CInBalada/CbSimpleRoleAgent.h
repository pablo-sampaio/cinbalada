
#ifndef _____CB_SIMPLEROLE_AGENT_H____
#define _____CB_SIMPLEROLE_AGENT_H____

#include "CbAbstractAgent.h"
#include "CbAgentRepertoire.h"

typedef enum {AR_NONE = -1, AR_BASIC_1, AR_BASIC_2, AR_SOLO_1} ECbAgentRole;

// service name of 'simple-role' agents
//#define SROLE_AGENT_SERVICE_NAME "Simple-Role Percussionist"

#define MSG_REQ_ROLES         "reply roles"
#define MSG_REQ_RPATTERN      "current rhythm-pattern"


/**
 * Percussionists agents with (simple) roles in a group
 * performance.
 */
class CbSimpleRoleAgent : public CbAbstractAgent
{
private:
	static const char* rolesStr[];

    ECbAgentRole role;    // agent's role
    int maxRepertIndex;   // only patterns in the range [0..maxIndexPattern] are analysed

    /* Methods to choose the range of patterns suitable to the pattern.
     */
	void setupB1Patterns();
	void setupB2Patterns();
	void setupS1Patterns();

	/* Chooses one pattern of the repertoire, taking in account the set of patterns
     * given as parameter and the role of the agent.
     */
    int choosePattern(const Vector_pRhythmPattern &ref);
    int chooseB1Pattern(const Vector_pRhythmPattern &ref);
    int chooseB2Pattern(const Vector_pRhythmPattern &ref);
    int chooseS1Pattern(const Vector_pRhythmPattern &ref);

	/* Adds a new JoinerBehaviour to join the band again.
     */
	void rejoinBand();

public:

    /* Methods to make convertions from 'ECbAgentRole' to strings
     * and from strings back to 'ECbAgentRole'.
     */
	static const char* agentRoleToStr(ECbAgentRole role);
	static ECbAgentRole strToAgentRole(const char* roleStr);

	/* Constructor / Destructor
     */
    CbSimpleRoleAgent(std::string name, CbAgentRepertoire *rep, ECbAgentRole role);
	virtual ~CbSimpleRoleAgent();

    /* Implementação do metodo da superclasse.
     */
	void setup();


	/**
     * Behaviour of a "simple-role" agent joining the band.
     */
    class JoinerBehaviour : public FmBehaviour {
	private:
   		CbSimpleRoleAgent* agent;    // the agent joining the band
        ECbAgentRole referenceRole;  // role used as reference when choosing agent's pattern

    	typedef enum {REQ_ROLES, RECEIVE_ROLES, REQ_PATTERNS, RECEIVE_PATTERNS, CHOOSE_PATTERN, QUIT} State;
        State state;

        int pendingAnswers;
        Vector_AID refAgents;
        Vector_pRhythmPattern refPatterns;

    public:
		JoinerBehaviour(CbSimpleRoleAgent *ag, ECbAgentRole ref);
		virtual ~JoinerBehaviour();
    	void action();
        void reqRolesState();
        void recvRolesState();
        void reqPatternsState();
        void recvPatternsState();
        void choosePatternState();
        bool done();
        std::ofstream& log(bool prompt=true);
    };

	/**
     * Behaviour of an agent that is already playing
     * (doesn't matter in wich role).
     */
    class PlayerBehaviour : public FmBehaviour {
    private:
   		CbSimpleRoleAgent* agent;
    	typedef enum {SETUP, PLAYING, QUIT} State;
        State state;

    public:
	    PlayerBehaviour(CbSimpleRoleAgent* agent);
        void onStart();
        void onEnd();
    	void action();
        void setupState();
        void playingState();
        bool done();
        std::ofstream& log(bool prompt=true);
	};

    /**
     * Behaviour of a solo agent playing.
     */
    class SoloPlayerBehaviour : public FmBehaviour {
    private:
   		CbSimpleRoleAgent* agent;
    	typedef enum {SETUP, PLAYING, CHOOSE_PATTERN, QUIT} State;
        State state;
		int basePattern;

    public:
	    SoloPlayerBehaviour(CbSimpleRoleAgent* agent);
        void onStart();
        void onEnd();
    	void action();
        void setupState();
        void playingState();
        void choosePattern();
        bool done();
        std::ofstream& log(bool prompt=true);
	};

    friend class JoinerBehaviour;
	friend class PlayerBehaviour;
	friend class SoloPlayerBehaviour;
};

#endif /* _____CB_SIMPLEROLE_AGENT_H____ */
