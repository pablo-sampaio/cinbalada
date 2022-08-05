
#ifndef ______CBABSTRACTAGENT_H______
#define ______CBABSTRACTAGENT_H______

#include <fstream>
#include "FAMA\FAMA.h"
#include "RhythmPattern\CbRhythmPattern.h"
#include "CbMixerAgent.h"

#define EXCP_UNEXPECTED_MSG  "Unexpected message !!!"
#define EXCP_PARSING_ERROR   "Parsing error in message !!!"

#define CINBALADA_AGENTS_SERVC_TYPE  "Musician Service"


class CbAbstractAgent;
typedef void (__closure *CbAgentEvent)(CbAbstractAgent* agent);

/**
 * Abstract musical agent. Musical agents in general (percussionists
 * or not) are implementations of this abstract agent.
 */
class CbAbstractAgent : public FmAgent
{
private:
	std::ofstream* logStream;
	bool deleteStream;

protected:

	AID mixerAgent;

	/* Constructor. Receives agent's name and agent's repertoire.
	 * {deallocation of parameter}: done by this class
	 */
	CbAbstractAgent(std::string name, bool susp = false, bool autoDel = true);

	/* Sets the log stream. Parameter 'autoDelete' informs wheter
	 * the stream should be deallocated by the agent's destructor.
	 */
	void setLogStream(std::ofstream* logof, bool autoDelete);

	/* Returns the pointer to the stream to be used as agent's log.
	 */
	std::ofstream* getLogStream(bool prompt=true);

	/* Returns the same reference as above in a more friendly and
	 * ready-to-use way.
	 */
	std::ofstream& log(bool prompt=true);

	/* Returns the address of the others musician agents.
	 */
	void getMusiciansIds(Vector_AID&);

	/* This method registers the service of an musician agent.
	 */
	void registerAsMusician(const char* instrument);
	void unregisterAsMusician();

	/* Sends a message to the whole band (broadcast) but the agent
	 * represented by parameter 'not'.
	 */
	void sendMusicians(FmAclMessage* acl, AID not = NULL);

	/* Methods to register/unregister in the mixer-agent.
	 * Attribute 'mixerAgent' is automatically set when registering.
	 */
	void registerInMixer();
	void unregisterFromMixer();

public:

	/* Event triggered when the agent status changes (should be in FmAgent!).
	 */
	CbAgentEvent OnStatusChange;

	/* Method from class FmAgent overridden to allow the
	 * event 'OnStatusChange'.
	 */
	virtual void setStatus(EFmAgentStatus st);

	virtual ~CbAbstractAgent();

};

#endif /* ______CBABSTRACTAGENT_H______ */
