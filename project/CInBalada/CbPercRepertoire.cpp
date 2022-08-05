
#include "CbPercRepertoire.h"

#include <map>
#include <fstream>
#include <algorithm>

using namespace std;

static ofstream logFile("logs\\CbPercRepertoire.txt", ios::out);


CbPercRepertoire::RepertItem::RepertItem(CbRhythmPattern* __rp, bool __enabled)
	: index(-1), pattern(__rp), enabled(__enabled), timesChosen(0), eval(0)
{
}

CbPercRepertoire::RepertItem::~RepertItem() {
	delete pattern;
}


CbPercRepertoire::CbPercRepertoire(string instrName, const ICbRpInfoRepository& repository,
						const char* midiDirectory)
{
	this->instrument = new CbPercInstrument( *(repository.getInstrument(instrName)) );
	this->history = NULL;

	Vector_pRpInfo patterns;
	repository.getRpInfosByInstrument(instrName, patterns);

    logFile << "All patterns of " << instrName << ":" << endl;
	for (int i = 0; i < patterns.size(); i ++) {
    	logFile << "   [" << i << "] - " << patterns[i]->getName() << endl;
		this->addRhythmPattern(*(patterns[i]), midiDirectory);
	}
    logFile << endl;

	// adiciona padroes mudos (MUDAR: ler metricas do repositorio?)
	this->addRhythmPattern(new CbRhythmPattern(CbMeter::SIMPLE_QUADR));
	this->addRhythmPattern(new CbRhythmPattern(CbMeter::SIMPLE_TRIPLE));

	this->meter = NULL;
}

CbPercRepertoire::~CbPercRepertoire() {
	//ofstream logFile("\\CbPercRepertoire-Finalization.txt", ios::out | ios::app);
	//logFile << "Apagando repertorio de \"" << this->getInstrumentName() << "\"...";

	MapRolesItems::iterator it;
	for (it = rolesItems.begin(); it != rolesItems.end(); it ++) {
		delete it->second;  // deleta os vetores
	}

	RepertItem* pattrn;

	for (int i = 0; i < this->items.size(); i ++) {
		pattrn = this->items[i];
		this->items[i] = NULL;
		delete pattrn;
	}

	delete instrument;
	//logFile << " OK !" << endl;
}

void CbPercRepertoire::rearrange(const Vector_pRRoles& roles, const CbMeter* mtr) {
	MapRolesItems::iterator it;
	for (it = rolesItems.begin(); it != rolesItems.end(); it ++) {
		delete it->second;  // deleta os vetores anteriores
	}
	this->rolesItems.clear();

	Vector_RepertItem *vItems;
	const CbRhythmPattern *pattern;

	logFile << "Rearranging " << instrument->getName() << "'s repertoire "
			<< "with meter: " << mtr->getNumTatums() << endl;

	if (this->meter != NULL) {
		delete this->meter;
	}
	this->meter = new CbMeter(*mtr);

	for (int i = 0; i < roles.size(); i ++) {
		vItems = new Vector_RepertItem;
		logFile << "[" << roles[i]->getName() << "]" << endl;

		for (int j = 0; j < this->items.size(); j ++) {
			pattern = items[j]->pattern;

			if (pattern->getMeter()->isCompatibleWith(mtr) && roles[i]->patternIsSuitable(*pattern)) {
				// guarda a referencia
				vItems->push_back(items[j]);

				logFile << " <> " << pattern->getInfo()->getName()
						<< "\t (" << pattern->getInfo()->getId() << ")" << endl;
			}
		}

		logFile << " >> Number of items: " << vItems->size() << endl;
		this->rolesItems[roles[i]->getName()] = vItems;
	}

	logFile << endl;

}

const CbMeter* CbPercRepertoire::getMeter() const {
	return this->meter;
}

void CbPercRepertoire::setHistory(const CbPercPerfHistory* h) {
	this->history = h;
}

string CbPercRepertoire::getInstrumentName() const {
	return instrument->getName();
}

const CbPercInstrument* CbPercRepertoire::getInstrument() const {
	return instrument;
}

void CbPercRepertoire::addRhythmPattern(const CbRpInfo& rpi, const char* midiDirectory) {
	RepertItem* repertItem = new RepertItem(new CbRhythmPattern(rpi,midiDirectory));
	repertItem->index = this->items.size();
	this->items.push_back(repertItem);
}

void CbPercRepertoire::addRhythmPattern(CbRhythmPattern* pattern) {
	RepertItem* repertItem = new RepertItem(pattern);
	repertItem->index = this->items.size();
	this->items.push_back(repertItem);
}

double CbPercRepertoire::getAvgTimesChosen(const char* role) const {
	std::string key = role;
	Vector_RepertItem *v = rolesItems.find(key)->second;

	int totalTimes = 0;
	int numEnabled = 0;

	for (int i = 0; i < v->size(); i ++) {
		if (v->at(i)->enabled == true) {
			numEnabled ++;
			totalTimes += v->at(i)->timesChosen;
		}
	}

	return double(totalTimes) / double(numEnabled);
}

int CbPercRepertoire::getNumRhythmPatterns() const {
	return items.size();
}

const CbRhythmPattern* CbPercRepertoire::getRhythmPattern(int index) const {
	return items[index]->pattern;
}

int CbPercRepertoire::getNumRhythmPatterns(const char* roleName) const {
	std::string key = roleName;
	return rolesItems.find(key)->second->size();
}

bool CbPercRepertoire::hasPatternInRole(const CbRhythmicRole* role) {
	return getNumRhythmPatterns(role->getName()) > 0;
}

const CbRhythmPattern* CbPercRepertoire::getRhythmPattern(const char* roleName, int index) {
	std::string key = roleName;
	return rolesItems[key]->at(index)->pattern;
}

int CbPercRepertoire::getMainIndex(const char* roleName, int roleIndex) const {
	std::string key = roleName;
	MapRolesItems::const_iterator it = this->rolesItems.find(key);
	return it->second->at(roleIndex)->index;
}

int CbPercRepertoire::getRoleIndex(const char* roleName, int mainIndex) const {
	std::string key = roleName;
	Vector_RepertItem* rolePatterns = this->rolesItems.find(roleName)->second;

	int secIndex = -1;

	for (int i = rolePatterns->size() - 1; i >= 0; i --) {
		if ((*rolePatterns)[i]->index == mainIndex) {
			secIndex = i;
			break;
		}
	}

	return secIndex;
}

void CbPercRepertoire::getStyles(Vector_string& results) const {
	map<string,int> aux; //acoxambre

	Vector_RepertItem::const_iterator it;
	const CbRpInfo* info;

	results.clear();

	for (it = items.begin(); it != items.end(); it++) {
		info = (*it)->pattern->getInfo();

		for (int i = 0; i < info->getNumStyles(); i ++) {
			if (aux.find(info->getStyle(i)) == aux.end()) {
				aux[info->getStyle(i)] = -1; //nao importa o valor
				results.push_back(info->getStyle(i));
			}
		}
	}

}

void CbPercRepertoire::setEnabled(int indexPattern, bool enabled) {
	items[indexPattern]->enabled = enabled;
}

bool CbPercRepertoire::isEnabled(int indexPattern) {
	return items[indexPattern]->enabled;
}

void CbPercRepertoire::sort(const char* roleName, ECbRpEvalCriteria criteria,
								 const CbRhythmPattern* rp, bool ascending)
{
	std::string key = roleName;
	Vector_RepertItem* v = rolesItems[key];
	RepertItem *item;

	for (int i = 0; i < v->size(); i ++) {
		item = v->at(i);
		item->eval = item->pattern->evaluationFunction(criteria, rp);
	}

	sorting_algorithm(v, ascending);
}

void CbPercRepertoire::sort(const char* roleName, CbRpEvalCriteria criteriaFunc,
									const CbRhythmPattern* rp, bool ascending)
{
	std::string key = roleName;
	Vector_RepertItem* v = rolesItems[key];
	RepertItem *item;

	for (int i = 0; i < v->size(); i ++) {
		item = v->at(i);
		item->eval = criteriaFunc(item->pattern, rp);
	}

	sorting_algorithm(v, ascending);
}

void CbPercRepertoire::sortWithPenalties(const char* roleName, ECbRpEvalCriteria criteria, const CbRhythmPattern* rp, bool ascending) {
	const CbPercPerfInfo* perfInfo = history->searchBackward(roleName);
	int lastPatternIndex = (perfInfo == NULL) ? -1 : perfInfo->patternIndex;

	std::string key = roleName;
	Vector_RepertItem* v = rolesItems[key];
	RepertItem *item;

	double avgTimesChosen = this->getAvgTimesChosen(roleName);
	double penalty;

	for (int i = 0; i < v->size(); i ++) {
		item = v->at(i);
		item->eval = item->pattern->evaluationFunction(criteria, rp);

		if (item->index != lastPatternIndex) {
			int times = history->getTimesChosen(item->index);

			if (times >= 4*avgTimesChosen) {
				penalty = 0.2;
			} else if (times >= 2*avgTimesChosen) {
				penalty = 0.1;
			}

			if (ascending == false) penalty *= -1;

			item->eval += penalty * item->eval;
		}
	}

	sorting_algorithm(v, ascending);
}

void CbPercRepertoire::sortWithPenalties(const char* roleName, CbRpEvalCriteria criteriaFunc, const CbRhythmPattern* rp, bool ascending) {
	const CbPercPerfInfo* perfInfo = history->searchBackward(roleName);
	int lastPatternIndex = (perfInfo == NULL) ? -1 : perfInfo->patternIndex;

	std::string key = roleName;
	Vector_RepertItem* v = rolesItems[key];
	RepertItem *item;

	double avgTimesChosen = this->getAvgTimesChosen(roleName);
	double penalty;

	for (int i = 0; i < v->size(); i ++) {
		item = v->at(i);
		item->eval = criteriaFunc(item->pattern, rp);

		if (item->index != lastPatternIndex) {
			int times = history->getTimesChosen(item->index);

			if (times >= 4*avgTimesChosen) {
				penalty = 0.2;
			} else if (times >= 2*avgTimesChosen) {
				penalty = 0.1;
			}

			if (ascending == false) penalty *= -1;

			item->eval += penalty * item->eval;
		}
	}

	sorting_algorithm(v, ascending);
}

void CbPercRepertoire::sorting_algorithm(Vector_RepertItem *v, bool ascending) {
	RepertItem *aux;
	int size = v->size();

	for (int i = 0; i < size; i ++) {
		for (int j = i + 1; j < size; j ++) {
			if ((v->at(j)->eval < v->at(i)->eval) == ascending) {
				aux = v->at(i);
				v->at(i) = v->at(j);
				v->at(j) = aux;
			}
		}
	}
}

