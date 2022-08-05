
#ifndef ____FACADE_H___
#define ____FACADE_H___

#include <vector>

#include "CbPercussionistAgent.h"
#include "CbRandomAgent.h"

#include "CbXmlRhythmPatternInfoRepository.h"
#include "CbRhythmicProject.h"
#include "CbMutedMixerAgent.h"


/* Directory where the MIDI files are stored */
#define MIDI_REPERT_DIR   "midis"
#define XML_REPERT_FILE   "midx.xml"


/**
 * CInBalada system facade. It is the single point of access for
 * the funcionalities of the CInBalada system.
 */
class CbFacade {
protected:
	AnsiString applicationDir;
	AnsiString midiDirectory;

	ICbRpInfoRepository* rhythmRepository;

	bool **roleSupport;  // matrix 'instruments' x 'roles', that indicates the
						 // roles supported for each instrument

	Vector_pRRoles roles;

	FmTContainer *container;
	CbMixerAgent *mixer;

	typedef std::vector<CbAbstractAgent*> Vector_pAgents;
	Vector_pAgents band;

	// pesos de avaliacao usados na criacao dos agentes (passar para o projeto!)
	//double agentsInitialHorizontalWeigth;
	//double agentsInitialVerticalWeigth;

	typedef std::vector<CbMeter*> Vector_pMeters;
	Vector_pMeters meters;

	CbRhythmicProject project;
	bool projectChange;

	// singleton
	static CbFacade instance;


	/* Private constructor.
	 */
	CbFacade();

	/* Makes project settings active.
	 */
	void applyProjectSettings();

	/* Remove all agents.
	 */
	void clearAgents();

	/* Creates a percussionist agent.
	 */
	CbPercussionistAgent* createAgent(AnsiString agentName, const char* instrName, const char* roleStr);


public:

	virtual ~CbFacade();

	/* Method to access the only instance of the facade.
	 */
	static CbFacade* getInstance();


	/////// BASIC ACCESS METHODS ///////

    /* Application's fully qualified directory path.
     */
	AnsiString getApplicationDir();

	/* Access the mixer agent, responsible for sequencing the MIDI output.
	 */
	CbMixerAgent* getMixer();

	/* Acess to the repository of rhythm patterns used by all agents.
	 */
	ICbRpInfoRepository* getRpInfoRepository();

	/* Acess all rhythmic roles available in the system.
	 */
	int getNumRoles();
	const CbRhythmicRole* getRole(int i);
	const CbRhythmicRole* getRole(const char* name);
	int getRoleIndex(const char* name);


	/////// GENERAL CONFIGURATION FUNCTIONS ///////

	int getTempo();
	void setTempo(int tempoBpm);

	int getBarsCycle();
	void setBarsCycle(int bars);

	void toggleRepeatCycle();
	bool isRepeatingCycle();

	const CbMeter* getMeter(int i);
	int getNumMeters();
	const char* getMeterName(int i);

	const CbMeter* getCurrentMeter();
	int getCurrentMeterIndex();
	void setCurrentMeter(int index);

	AnsiString getProjectFileName();

	AnsiString getProjectName();
	void setProjectName(const AnsiString& projectName);

	void newProject(const AnsiString& name, int numBars, int tempo, int meterIndex);

	void saveProjectFile();
	void saveProjectFile(const AnsiString& file);
	void loadProjectFile(const AnsiString& file);

	bool projectHasChanged();


	/////// AGENT-RELATED FUNCTIONS ///////

	int addAgent(AnsiString name, const char* instrument, const char* role);
	void removeAgent(int index);  // atencao: muda os indices posteriores

	CbPercussionistAgent* getAgent(int i);
	int getNumAgents();

	void setAgentRole(int ag, int bar, int roleIndex);
	int getAgentRole(int ag, int bar);

	void setAgentDefaultRole(int ag, int roleIndex);
	int getAgentDefaultRole(int ag);

	bool agentHasPatternOfRole(int agentIndex, const char* role);

    //void setAgentsInitialEvaluationWeigths(double vertical, double horizontal);


	/////// ADVANCED FUNCTIONS ///////

	/* Methods to control MIDI exporting.
	 */
	bool hasRecorded();
	void exportRecordingToMidi(const char *fileName);

	/* Indicates wheter the given instrument has some pattern of
	 * the given rhythmic role.
	 */
	bool instrumentHasPatternOfRole(int instrIndex, const char* role);

	/* Indicates whether an agent can be created for the instrument.
     */
   	//bool instrumentIsAvailable(int instrIndex);

	/* Initializes the matrix used to indicate which rhythmic roles
	 * are available for each instrument. (This is not done in the constructor
	 * because it takes too much time; this initialization could better be done
	 * in a splash screen, for example).
	 */
	void fillRoleSupportMatrix();

	/* Change mixer agent so that no output is sent to the soundcard and
	 * the processing of bars is no more sequenced in time (generation of
	 * rhythmic performances becomes much more fast).
	 * Also adds a CbFreeRole to the facade list of roles to be used by
	 * the abstract agent.
	 */
	void setDebug();

	/* Creates agent with abstract evaluation function.
	 */
	int addRandomAgent(AnsiString name, const char* instrument);

	/* Returns the agent at the given index casted to CbRandomAgent*.
	 */
	CbRandomAgent* getRandomAgent(int agentIndex);

};


#endif
