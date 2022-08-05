
#include "CbFacade.h"
#include <vector>


typedef CbRhythmicProject::MapRoles MapRoles;  // apenas para facilitar


CbFacade CbFacade::instance;

CbFacade* CbFacade::getInstance() {
	return &instance;
}

CbFacade::CbFacade() {
	this->applicationDir = ExpandFileName(".");  // fazer usando GetModuleFileName...

	this->mixer = new CbMixerAgent();
	this->container = FmTContainer::getInstance();

	AnsiString xmlPath = applicationDir + "\\" + XML_REPERT_FILE;
	this->rhythmRepository = new CbXmlRpInfoRepository(xmlPath.c_str());

	this->midiDirectory = applicationDir + "\\" + MIDI_REPERT_DIR + "\\";

	// ler do cadastro de RpInfo...
	this->meters.push_back(new CbMeter(4,4));
	this->meters.push_back(new CbMeter(4,3));

    // pesos de avaliacao dos agentes
	//this->agentsInitialHorizontalWeigth = DEFAULT_HORIZ_EVAL_WEIGTH;
	//this->agentsInitialVerticalWeigth = DEFAULT_VERT_EVAL_WEIGTH;

	// cadastra papeis
	CbRhythmicRole* aux;

	aux = new CbBasicRole;
	this->roles.push_back(aux);
	aux = new CbComplBasicRole;
	this->roles.push_back(aux);
	aux = new CbSoloRole;
	this->roles.push_back(aux);
	aux = new CbFillRole;
	this->roles.push_back(aux);
	aux = new CbMuteRole;
	this->roles.push_back(aux);

	aux = new CbUniformRole;       // REMOVER !!! (serve apenas para os experimentos)
	this->roles.push_back(aux);
	aux = new CbFreeRole;         // REMOVER !!! (serve apenas para os experimentos)
	this->roles.push_back(aux);

	this->roleSupport = NULL;  // inicializado pelo metodo 'fillRoleSupportMatrix'

	applyProjectSettings();
	this->projectChange = false;
}

CbFacade::~CbFacade() {
	Vector_pAgents::iterator it;
	for (it = band.begin(); it != band.end(); it ++) {
		(*it)->doDelete();
	}
	band.clear();

	// obs: mixer is auto-deleted

	if (roleSupport != NULL) {
		int i = 0;
        while (roleSupport[i] != NULL) {
			delete[] roleSupport[i];
            i ++;
		}
		delete[] roleSupport;
	}

	for (int i = 0; i < meters.size(); i ++) {
		delete meters[i];
	}
	meters.clear();

	for (int i = 0; i < roles.size(); i ++) {
		delete roles[i];
	}
	roles.clear();

	delete rhythmRepository;
}

AnsiString CbFacade::getApplicationDir() {
	return this->applicationDir;
}

void CbFacade::setDebug() {
	this->mixer->doDelete();  // serah auto-deletado pela plataforma
	this->mixer = new CbMutedMixerAgent();

	this->roles.push_back(new CbFreeRole);
}

void CbFacade::applyProjectSettings() {
	this->mixer->stop();

	if (this->mixer->isMuted()) {
		this->mixer->toggleMute();
	}

	this->mixer->setMeter(* (this->meters[this->project.getMeterIndex()]));
	this->mixer->setTempo(int(this->project.getTempo()));

	if (this->project.shouldAutoRepeat()) {
		this->mixer->setBarsCycle(-1);
	} else {
		this->mixer->setBarsCycle(this->project.getNumBars());
	}

	const MapRoles* agentRoles;
	MapRoles::const_iterator it;
	const CbRhythmicRole* role;
	CbPercussionistAgent* agent;

	this->clearAgents();

	int numAgents = this->project.getNumAgents();

	for (int i = 0; i < numAgents; i ++) {
		// cria e adiciona o agente
		agent = this->createAgent(this->project.getAgentName(i),
								  this->project.getAgentInstrument(i), this->project.getAgentDefaultRole(i));
		this->band.push_back(agent);

		// configura seus papeis nao-padrao
		agentRoles = this->project.getAgentRoleChanges(i);
		for (it = agentRoles->begin(); it != agentRoles->end(); it ++) {
			role = this->getRole(it->second.c_str());
			agent->getPlan()->setRole(it->first, role);
		}
	}

}

void CbFacade::clearAgents() {
	this->mixer->stop();

	Vector_pAgents::iterator it;

	for (it = band.begin(); it != band.end(); it ++) {
		(*it)->doDelete();   // area de memoria eh limpa pelo container
	}

	band.clear();
}

void CbFacade::fillRoleSupportMatrix() {
	int numInstruments = this->rhythmRepository->getNumInstruments();
	int numRoles = this->roles.size();

	const char* instrName;
	CbPercRepertoire* repertoire;

	// a lista de instrumentos
	Vector_String instrumentList;
	rhythmRepository->getAllInstrumentNames(instrumentList);

	// deleta o array, se ja existir
	if (this->roleSupport != NULL) {
		int i = 0;

        while (roleSupport[i] != NULL) {
			delete[] roleSupport[i];
            i ++;
		}

		delete[] roleSupport;
	}

	// aloca o array de ponteiros (q sera usado como array bidimensional)
	this->roleSupport = new bool*[numInstruments + 1];

    // para indicar o fim do array
   	this->roleSupport[numInstruments] = NULL;

	// preenche a matriz...
	for (int i = 0; i < numInstruments; i ++) {
		this->roleSupport[i] = new bool[numRoles];  // aloca arrays por instrumento
		instrName = instrumentList[i].c_str();

		repertoire = new CbPercRepertoire(instrName, *this->rhythmRepository, midiDirectory.c_str());
		repertoire->rearrange(this->roles, this->mixer->getMeter());

		for (int j = 0; j < numRoles; j ++) {
			this->roleSupport[i][j] = (repertoire->getNumRhythmPatterns(this->roles[j]->getName()) > 0);
		}

		delete repertoire;
	}


}

CbMixerAgent* CbFacade::getMixer() {
	return this->mixer;
}

ICbRpInfoRepository* CbFacade::getRpInfoRepository() {
	return this->rhythmRepository;
}

int CbFacade::getNumRoles() {
	return roles.size();
}

const CbRhythmicRole* CbFacade::getRole(int i) {
	return roles[i];
}

const CbRhythmicRole* CbFacade::getRole(const char* nm) {
	string name = nm;
	CbRhythmicRole* result = NULL;

	for (int i = roles.size() - 1; i >= 0; i --) {
		if (name == roles[i]->getName()) {
			result = roles[i];
		}
	}

	return result;
}

int CbFacade::getRoleIndex(const char* nm) {
	string name = nm;
	int result = 0;

	for (int i = roles.size() - 1; i >= 0; i --) {
		if (name == roles[i]->getName()) {
			result = i;
		}
	}

	return result;
}

bool CbFacade::instrumentHasPatternOfRole(int instrIndex, const char* role) {
	int roleIndex = getRoleIndex(role);
	return roleSupport[instrIndex][roleIndex];
}

bool CbFacade::agentHasPatternOfRole(int agentIndex, const char* roleName) {
	const CbRhythmicRole* role = this->getRole(roleName);
	return this->getAgent(agentIndex)->getRepertoire()->hasPatternInRole(role);
}

/*void CbFacade::setAgentsEvaluationInitialWeigths(double vert, double horiz) {
    // FALTA: salvar no projeto ! (esse eh o peso para novos agentes, por performance!)
}*/

int CbFacade::getTempo() {
	return this->mixer->getTempo();
}

void CbFacade::setTempo(int tempoBpm) {
	const CbMeter* meter = getCurrentMeter();
	if (tempoBpm >= meter->getMinTempo() && tempoBpm <= meter->getMaxTempo()) {
		this->mixer->setTempo(tempoBpm);
		this->project.setTempo(tempoBpm);
		this->projectChange = true;
	} else {
		throw "Tempo out of range !";
	}
}

int CbFacade::getBarsCycle() {
	return this->project.getNumBars();
}

void CbFacade::setBarsCycle(int bars) {
    CbPercussionistAgent* agent;
	int numAgents = this->getNumAgents();

	for (int i = 0; i < numAgents; i ++) {
		agent = this->getAgent(i);
        if (agent != NULL) {
	        agent->getPlan()->truncate(bars);
        }
	}

	if (this->project.shouldAutoRepeat()) {
		this->mixer->setBarsCycle(-1);
	} else {
		this->mixer->setBarsCycle(bars);
	}

	this->project.setNumBars(bars);
	this->projectChange = true;
}

const CbMeter* CbFacade::getCurrentMeter() {
	return this->meters[getCurrentMeterIndex()];
}

int CbFacade::getCurrentMeterIndex() {
	return this->project.getMeterIndex();
}

void CbFacade::setCurrentMeter(int i) {
	const CbMeter *oldMeter, *meter;

	if (i != getCurrentMeterIndex() && i >= 0 && i < meters.size()) {
		oldMeter = this->meters[getCurrentMeterIndex()];

		this->project.setMeterIndex(i);
		meter = this->meters[i];

		this->mixer->stop();
		this->mixer->setMeter(*getCurrentMeter());

		// recalcula a matriz
		fillRoleSupportMatrix();

		// rearruma os repertorios dos agentes...
		for (int i = this->getNumAgents() - 1; i >= 0; i --) {
			this->getAgent(i)->getRepertoire()->rearrange(this->roles, meter);
		}

		// ajusta o tempo proporcionalmente aos limites
		int oldTempo = this->mixer->getTempo();
		double aux1 = (meter->getMaxPreferedTempo() - meter->getMinPreferedTempo())
							* (oldTempo - oldMeter->getMinPreferedTempo());
		double aux2 = oldMeter->getMaxPreferedTempo() - oldMeter->getMinPreferedTempo();
		int adjustedTempo =  int( (aux1 / aux2) + double(meter->getMinPreferedTempo()) );

		// pode acontecer com metricas complexas...
		if (adjustedTempo > meter->getMaxTempo()) {
			adjustedTempo = meter->getMaxTempo();
		} else if (adjustedTempo < meter->getMinTempo()) {
			adjustedTempo = meter->getMinTempo();
		}

		this->mixer->setTempo(adjustedTempo);
	}
}

const CbMeter* CbFacade::getMeter(int i) {
	if (i >= 0 && i < meters.size()) {
		return meters[i];
	} else {
		return NULL;
	}
}

int CbFacade::getNumMeters() {
	return meters.size();
}

const char* CbFacade::getMeterName(int i) {
	// guardar no cadastro de padroes (?)
	if (i == 0) {
		return "Quaternário (~4/4)";
	} else {
		return "Ternário (~12/8)";
	}
}

AnsiString CbFacade::getProjectFileName() {
	return this->project.getFile();
}

AnsiString CbFacade::getProjectName() {
	return this->project.getName();
}

void CbFacade::setProjectName(const AnsiString& projectName) {
	this->project.setName(projectName.c_str());
	this->projectChange = true;
}

void CbFacade::newProject(const AnsiString& name, int numBars, int tempo, int meterIndex) {
	this->project.clear();

	this->project.setName(name.c_str());
	this->project.setFile("");
	this->project.setNumBars(numBars);
	this->project.setTempo(tempo);
	this->project.setMeterIndex(meterIndex);

	if (this->project.shouldAutoRepeat()) {
		this->project.toggleAutoRepeat();
	}

	this->applyProjectSettings();
	this->projectChange = true;
}

void CbFacade::saveProjectFile() {
	this->project.saveFile();
	this->projectChange = false;  // nao ha mais mudancas 'uncommited'
}

void CbFacade::saveProjectFile(const AnsiString& file) {
	this->project.setFile(file.c_str());
	this->saveProjectFile();
}

void CbFacade::loadProjectFile(const AnsiString& file) {
	this->project.setFile(file.c_str());
	this->project.loadFile();
	this->applyProjectSettings();
	this->projectChange = false;
}

bool CbFacade::projectHasChanged() {
	return this->projectChange;
}

int CbFacade::addAgent(AnsiString agentName, const char* instrName, const char* roleStr) {
	CbPercussionistAgent *agent = createAgent(agentName, instrName, roleStr);

	this->band.push_back(agent);
	this->project.addAgent(agent->getName().c_str(), instrName, roleStr);

	this->projectChange = true;

	return band.size() - 1;
}

CbPercussionistAgent* CbFacade::createAgent(AnsiString agentName, const char* instrName, const char* roleStr) {
	CbPercRepertoire* repertoire = new CbPercRepertoire(instrName, *this->rhythmRepository,
															midiDirectory.c_str());
	repertoire->rearrange(this->roles, this->mixer->getMeter());

	AnsiString agentBaseName = agentName;
	int i = 1;

	while (container->hasAgentWithName(agentName.c_str()) && i < 100) {
		agentName = agentBaseName + "-" + i;
		i ++;
	}

	string agentNameStl = agentName.c_str();
	const CbRhythmicRole *role = this->getRole(roleStr);
	CbPercussionistAgent *agent = new CbPercussionistAgent(agentNameStl, repertoire,
														   role, false, true); // will be auto-deleted
	return agent;
}

void CbFacade::removeAgent(int index) {
	if (index >= 0 && index < band.size()) {
		this->band[index]->doDelete();
		this->band.erase(band.begin() + index);
		this->project.removeAgent(index);
		this->projectChange = true;
	}
}

CbPercussionistAgent* CbFacade::getAgent(int index) {
	if (index >= 0 && index < band.size()) {
		return dynamic_cast<CbPercussionistAgent*>(band[index]);
	} else {
		return NULL;
	}
}

int CbFacade::getNumAgents() {
	return band.size();
}

void CbFacade::setAgentRole(int ag, int bar, int roleIndex) {
	if (bar < this->getBarsCycle()) {
		const CbRhythmicRole* role = this->getRole(roleIndex);
		this->getAgent(ag)->getPlan()->setRole(bar, role);
		this->project.setAgentRoleChange(ag, bar, role->getName());
		this->projectChange = true;
	}
}

int CbFacade::getAgentRole(int ag, int bar) {
	CbPercussionistAgent* agent = this->getAgent(ag);
	const CbRhythmicRole* role = agent->getPlan()->getRole(bar);
	return this->getRoleIndex(role->getName());
}

void CbFacade::setAgentDefaultRole(int ag, int roleIndex) {
	const CbRhythmicRole* role = this->getRole(roleIndex);
	this->getAgent(ag)->getPlan()->setDefaultRole(role);
}

int CbFacade::getAgentDefaultRole(int ag) {
	const CbRhythmicRole* role = this->getAgent(ag)->getPlan()->getDefaultRole();
	return this->getRoleIndex(role->getName());
}


bool CbFacade::hasRecorded() {
	FmMidiFile* mdfile = this->mixer->getRecord();
	return (mdfile != NULL) && (mdfile->getMaxTick() > 0);
}

void CbFacade::exportRecordingToMidi(const char *fileName) {
	mixer->getRecord()->saveToFile(fileName);
}

void CbFacade::toggleRepeatCycle() {
	if (this->mixer->getStatus() != ES_PLAYING) {
		this->project.toggleAutoRepeat();
		this->projectChange = true;

		if (this->project.shouldAutoRepeat()) {
			this->mixer->setBarsCycle(-1);  // loop infinito
		} else {
			this->mixer->setBarsCycle(this->project.getNumBars());
		}
	}

}

bool CbFacade::isRepeatingCycle() {
	return this->project.shouldAutoRepeat();
}

int CbFacade::addRandomAgent(AnsiString agentName, const char* instrName) {
	CbPercRepertoire* repertoire = new CbPercRepertoire(instrName, *this->rhythmRepository,
															midiDirectory.c_str());

	repertoire->rearrange(this->roles, this->mixer->getMeter());

	AnsiString agentBaseName = agentName;
	int i = 1;

	while (container->hasAgentWithName(agentName.c_str()) && i < 100) {
		agentName = agentBaseName + "-" + i;
		i ++;
	}

	string agentNameStl = agentName.c_str();

	/*
	const CbRhythmicRole *role = this->getRole("livre");
	CbPercussionistAgent *agent = new CbPercussionistAgent(agentNameStl,
														   repertoire, role, false, true); // will be auto-deleted
	/*/
	CbRandomAgent *agent = new CbRandomAgent(agentNameStl, repertoire);
	//*/

	this->band.push_back(agent);

	return this->band.size() - 1;
}

CbRandomAgent* CbFacade::getRandomAgent(int agentIndex) {
	if (agentIndex >= 0 && agentIndex < band.size()) {
		return dynamic_cast<CbRandomAgent*>(band[agentIndex]);
	} else {
		return NULL;
	}
}


