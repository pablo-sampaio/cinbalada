//---------------------------------------------------------------------------

#ifndef __CB_RHYTHMIC_PROJECT_H______
#define __CB_RHYTHMIC_PROJECT_H______

#include <map>
#include <vector>

//---------------------------------------------------------------------------


/* XML node "RhythmicProject" */
#define XML_NODE_RHYTHM_PROJECT          "RhythmicProject"
#define XML_ATTR_RHYTHM_PROJECT_NAME     "name"
#define XML_ATTR_RHYTHM_PROJECT_TEMPO    "tempo"
#define XML_ATTR_RHYTHM_PROJECT_METER    "meter-index"
#define XML_ATTR_RHYTHM_PROJECT_NBARS    "num-bars"
#define XML_ATTR_RHYTHM_PROJECT_REPEAT   "auto-repeat"

/* XML node "PercussionistAgent" */
#define XML_NODE_PERC_AGENT              "PercussionistAgent"
#define XML_ATTR_PERC_AGENT_NAME         "name"
#define XML_ATTR_PERC_AGENT_INSTR        "instrument"
#define XML_ATTR_PERC_AGENT_DEF_ROLE     "default-role"

/* XML node "Bar" */
#define XML_NODE_BAR               "Bar"
#define XML_ATTR_BAR_INDEX         "index"
#define XML_ATTR_BAR_ROLE          "role"


class TiXmlElement;

/**
 * A rhythmic project, that keeps track of the instruments, roles,
 * tempo, meter and other configurations set by the user in the
 * CInBalada system.
 */
class CbRhythmicProject
{
public:
	typedef std::map<int,std::string> MapRoles;

private:
	std::string name;       // project name
	std::string file;       // xml file where the project is stored

	double tempo;           // tempo in bpm
	int meterIndex;         // the index of the meter in the facade
	int numBars;            // the number of bars
	bool autoRepeat;        // turns project loop on/off

	struct PercAgentInfo {
		std::string name;
		std::string instrument;
		std::string defaultRole;
		MapRoles roles;
	};
	typedef std::vector<PercAgentInfo*> AgentInfoList;

	AgentInfoList agents;

	// methods to read/write xml file
	void loadAgentInfo(TiXmlElement* element);
	void loadBarRole(TiXmlElement* element, PercAgentInfo* agentInfo);

	TiXmlElement* createAgentNode(PercAgentInfo* agentInfo);
	TiXmlElement* createBarNode(int index, const char* role);

	// removes entrys after the given number of bars
	void truncateRoleChanges(int numBars);

public:

	CbRhythmicProject();
	virtual ~CbRhythmicProject();

	void loadFile();
	void saveFile();
	void clear();

	const char* getName() const;
	void setName(const char* n);

	const char* getFile() const;
	void setFile(const char* f);

	double getTempo() const;
	void setTempo(double tempo);

	int getMeterIndex() const;
	void setMeterIndex(int i);

	int getNumBars() const;
	void setNumBars(int n);

	bool shouldAutoRepeat() const;
	void toggleAutoRepeat();

	int getNumAgents() const;

	const char* getAgentName(int agentIndex) const;
	const char* getAgentInstrument(int agentIndex) const;
	const char* getAgentDefaultRole(int agentIndex) const;

	const MapRoles* getAgentRoleChanges(int agentIndex) const;
	void setAgentRoleChange(int agentIndex, int bar, const char* role);

	int addAgent(const char* name, const char* instrument, const char* role);
	void removeAgent(int agentIndex);
};


//---------------------------------------------------------------------------
#endif
