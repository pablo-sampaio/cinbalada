//---------------------------------------------------------------------------

#include "TinyXml\tinyxml.h"

#pragma hdrstop

#include "CbRhythmicProject.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;
//---------------------------------------------------------------------------


CbRhythmicProject::CbRhythmicProject() {
	this->tempo = 100.0;
	this->meterIndex = 0;
	this->numBars = 8;
	this->autoRepeat = false;
}

CbRhythmicProject::~CbRhythmicProject() {
	AgentInfoList::iterator it;
	// desaloca informacoes dos agentes
	for (it = agents.begin(); it != agents.end(); it ++) {
		delete *it;
	}
}

void CbRhythmicProject::loadFile() {
	TiXmlDocument document (this->file);
	TiXmlElement* element;

	string fileName = this->file;
	this->clear();
    this->file = fileName;

	document.LoadFile();

	// carrega o no do projeto (que deve ser unico)
	element = document.FirstChildElement(XML_NODE_RHYTHM_PROJECT);

	// carrega os atributos
	this->name = element->Attribute(XML_ATTR_RHYTHM_PROJECT_NAME);

	this->tempo = 100.0;
	element->Attribute(XML_ATTR_RHYTHM_PROJECT_TEMPO, &this->tempo);

	this->meterIndex = 0;
	element->Attribute(XML_ATTR_RHYTHM_PROJECT_METER, &this->meterIndex);

	this->numBars = 8;
	element->Attribute(XML_ATTR_RHYTHM_PROJECT_NBARS, &this->numBars);

	this->autoRepeat = false;
	const char* repeatStr = element->Attribute(XML_ATTR_RHYTHM_PROJECT_REPEAT);
	this->autoRepeat = (strcmpi(repeatStr,"true") == 0) ? true : false;

	// carrega os nos dos agentes
	element = element->FirstChildElement(XML_NODE_PERC_AGENT);
	while (element != NULL) {
		loadAgentInfo(element);
		element = element->NextSiblingElement(XML_NODE_PERC_AGENT);
	}
}

void CbRhythmicProject::loadAgentInfo(TiXmlElement* element) {
	PercAgentInfo* agentInfo = new PercAgentInfo;

	agentInfo->name = element->Attribute(XML_ATTR_PERC_AGENT_NAME);
	agentInfo->instrument = element->Attribute(XML_ATTR_PERC_AGENT_INSTR);
	agentInfo->defaultRole = element->Attribute(XML_ATTR_PERC_AGENT_DEF_ROLE);

	// carrega os nos com papeis
	element = element->FirstChildElement(XML_NODE_BAR);
	while (element != NULL) {
		loadBarRole(element, agentInfo);
		element = element->NextSiblingElement(XML_NODE_BAR);
	}

	this->agents.push_back(agentInfo);
}

void CbRhythmicProject::loadBarRole(TiXmlElement* element, PercAgentInfo* agentInfo) {
	int index = -1;
	const char* indexStr = element->Attribute(XML_ATTR_BAR_INDEX, &index);
	const char* roleName = element->Attribute(XML_ATTR_BAR_ROLE);

	agentInfo->roles[index] = roleName;
}

void CbRhythmicProject::saveFile() {
	TiXmlDocument document (this->file.c_str());

	TiXmlElement* projectElement = new TiXmlElement(XML_NODE_RHYTHM_PROJECT);
	TiXmlElement* agentElement;

	projectElement->SetAttribute(XML_ATTR_RHYTHM_PROJECT_NAME, this->name.c_str());
	projectElement->SetAttribute(XML_ATTR_RHYTHM_PROJECT_TEMPO, this->tempo);
	projectElement->SetAttribute(XML_ATTR_RHYTHM_PROJECT_METER, this->meterIndex);
	projectElement->SetAttribute(XML_ATTR_RHYTHM_PROJECT_NBARS, this->numBars);
	projectElement->SetAttribute(XML_ATTR_RHYTHM_PROJECT_REPEAT, (this->autoRepeat ? "true" : "false"));

	// salva informacoes dos agentes
	AgentInfoList::iterator it;
	for (it = agents.begin(); it != agents.end(); it ++) {
		agentElement = createAgentNode(*it);
		projectElement->InsertEndChild(*agentElement);
		delete agentElement;
	}

	document.InsertEndChild(*projectElement);
	delete projectElement;

	document.SaveFile();
}

TiXmlElement* CbRhythmicProject::createAgentNode(PercAgentInfo* agentInfo) {
	TiXmlElement* agentElement = new TiXmlElement(XML_NODE_PERC_AGENT);
	TiXmlElement* barElement;

	agentElement->SetAttribute(XML_ATTR_PERC_AGENT_NAME, agentInfo->name.c_str());
	agentElement->SetAttribute(XML_ATTR_PERC_AGENT_INSTR, agentInfo->instrument.c_str());
	agentElement->SetAttribute(XML_ATTR_PERC_AGENT_DEF_ROLE, agentInfo->defaultRole.c_str());

	// salva os papeis atribuidos por compasso
	MapRoles::iterator it;
	for (it = agentInfo->roles.begin(); it != agentInfo->roles.end(); it ++) {
		barElement = createBarNode(it->first, it->second.c_str());
		if (it->first < this->numBars) {
			agentElement->InsertEndChild(*barElement);
		}
		delete barElement;
	}

	return agentElement;
}

TiXmlElement* CbRhythmicProject::createBarNode(int index, const char* role) {
	TiXmlElement* element = new TiXmlElement(XML_NODE_BAR);

	element->SetAttribute(XML_ATTR_BAR_INDEX, index);
	element->SetAttribute(XML_ATTR_BAR_ROLE, role);

	return element;
}

void CbRhythmicProject::clear() {
	AgentInfoList::iterator it;

	this->name = "";
	this->file = "";

	this->tempo = 100.0;
	this->meterIndex = 0;
	this->numBars = 8;
	this->autoRepeat = false;

	// desaloca informacoes dos agentes
	for (it = agents.begin(); it != agents.end(); it ++) {
		delete *it;
	}

	this->agents.clear();
}

const char* CbRhythmicProject::getName() const {
	return this->name.c_str();
}

void CbRhythmicProject::setName(const char* n) {
	this->name = n;
}

const char* CbRhythmicProject::getFile() const {
	return this->file.c_str();
}

void CbRhythmicProject::setFile(const char* f) {
	this->file = f;
}

double CbRhythmicProject::getTempo() const {
	return this->tempo;
}

void CbRhythmicProject::setTempo(double t){
	this->tempo = t;
}

int CbRhythmicProject::getMeterIndex() const {
	return this->meterIndex;
}

void CbRhythmicProject::setMeterIndex(int i) {
	this->meterIndex = i;
}

int CbRhythmicProject::getNumBars() const {
	return this->numBars;
}

void CbRhythmicProject::setNumBars(int bars) {
	if (bars < this->numBars) {
		this->truncateRoleChanges(bars);
	}

	this->numBars = bars;
}

bool CbRhythmicProject::shouldAutoRepeat() const {
	return this->autoRepeat;
}

void CbRhythmicProject::toggleAutoRepeat() {
	this->autoRepeat = !this->autoRepeat;
}

int CbRhythmicProject::getNumAgents() const {
	return this->agents.size();
}

const char* CbRhythmicProject::getAgentName(int agentIndex) const {
	return this->agents[agentIndex]->name.c_str();
}

const char* CbRhythmicProject::getAgentInstrument(int agentIndex) const {
	return this->agents[agentIndex]->instrument.c_str();
}

const char* CbRhythmicProject::getAgentDefaultRole(int agentIndex) const {
	return this->agents[agentIndex]->defaultRole.c_str();
}

const CbRhythmicProject::MapRoles* CbRhythmicProject::getAgentRoleChanges(int agentIndex) const {
	return &(this->agents[agentIndex]->roles);
}

void CbRhythmicProject::truncateRoleChanges(int numBars) {
	AgentInfoList::iterator itAgents;
	MapRoles::iterator itRoles;
	PercAgentInfo* agent;

	for (itAgents = agents.begin(); itAgents != agents.end(); itAgents ++) {
		agent = *itAgents;

		itRoles = agent->roles.lower_bound(numBars);
		while (itRoles != agent->roles.end()) {
			agent->roles.erase(itRoles);
			itRoles = agent->roles.lower_bound(numBars);
		}
	}

}

void CbRhythmicProject::setAgentRoleChange(int agentIndex, int bar, const char* role) {
	PercAgentInfo* agentInfo = this->agents[agentIndex];

	if (agentInfo->defaultRole == role) {
		MapRoles::iterator it = agentInfo->roles.find(bar);
		if (it != agentInfo->roles.end()) {
			agentInfo->roles.erase(it);  // remove a posicao
		}
	} else {
		agentInfo->roles[bar] = role;
	}
}

int CbRhythmicProject::addAgent(const char* name, const char* instrument, const char* role) {
	PercAgentInfo* agentInfo = new PercAgentInfo;

	agentInfo->name = name;
	agentInfo->instrument = instrument;
	agentInfo->defaultRole = role;

	this->agents.push_back(agentInfo);

	return this->agents.size() - 1;
}

void CbRhythmicProject::removeAgent(int agentIndex) {
	PercAgentInfo* agentInfo = this->agents[agentIndex];
	this->agents.erase(agents.begin() + agentIndex);
	delete agentInfo;
}
