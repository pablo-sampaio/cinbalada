//---------------------------------------------------------------------------


#pragma hdrstop

#include "CbPercPerfHistory.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;
//---------------------------------------------------------------------------

// mudar aqui para habilitar/desabilitar log
//#define LOGGING

// definicao do arquivo de LOG
#ifdef LOGGING
	#include <fstream>
	static ofstream logFile("logs\\CbPercPerfHistory.txt", ios::out);
	#define LOG logFile
#else
	#define LOG cout
#endif



//...............................//
// CLASSE CbPercPerfHistory
//...............................//

CbPercPerfHistory::CbPercPerfHistory(int size)
	: history(size)
{
	LOG << "Historico criado com tamanho " << size << endl;
}

CbPercPerfHistory::~CbPercPerfHistory() {
	for (int i = 0; i < history.size(); i ++) {
		if (history[i] != NULL) {
			delete history[i];
			history[i] = NULL;
		}
	}
	history.clear();
}

void CbPercPerfHistory::set(int bar, const CbRhythmicRole* role, AID refAg,
								 int ptrnMainIndex, int ptrnBar)
{
	CbPercPerfInfo* info = new CbPercPerfInfo(bar, role, refAg, ptrnMainIndex, ptrnBar);

	if (bar >= history.size()) {
		history.resize(history.size() + 16);
	}

	if (history[bar] != NULL) {
		int ptrn = history[bar]->patternIndex;
		timesChosen[ptrn] = timesChosen[ptrn] --;
		delete history[bar];
	}

	history[bar] = info;
	timesChosen[ptrnMainIndex] = timesChosen[ptrnMainIndex] + 1;

	LOG << "Entrada adicionada {bar:" << history[bar]->bar << ", role:\"" << history[bar]->role
			<< "\", patrn:" << history[bar]->patternIndex << "}" << endl;
}

void CbPercPerfHistory::erase(int bar) {
	if (bar < this->history.size()) {
		delete this->history[bar];
		this->history.erase(history.begin() + bar);
	}
}

const CbPercPerfInfo* CbPercPerfHistory::get(int bar) const {
	if (bar >= 0 && bar < history.size()) {
		return history[bar];
	} else {
		return NULL;
	}
}

bool CbPercPerfHistory::has(int bar) {
	return this->get(bar) != NULL;
}

const CbPercPerfInfo* CbPercPerfHistory::search(const char* role, int n) const {
	std::string key = role;
	CbPercPerfInfo* result = NULL;

	LOG << "Procurando \"" << role << "\" para frente... ";
	for (int i = 0; i < history.size(); i ++) {
		if (history[i] != NULL && history[i]->role == key) {
			if (n > 0) {
				n --;
			} else {
				result = history[i];
				break;
			}
		}
	}

	if (result != NULL) {
		LOG << "achou: " << result->bar << "!!!" << endl;
	} else {
		LOG << "nao achou!" << endl;
	}

	return result;
}

const CbPercPerfInfo* CbPercPerfHistory::searchBackward(const char* role, int n) const {
	std::string key = role;
	CbPercPerfInfo* result = NULL;

	LOG << "Procurando \"" << role << "\" para tras... ";
	for (int i = history.size() - 1; i >= 0; i --) {
		CbPercPerfInfo* test = history[i];
		std::string aux = (test != NULL) ? test->role : string("");
		if (history[i] != NULL && history[i]->role == key) {
			if (n > 0) {
				n --;
			} else {
				result = history[i];
				break;
			}
		}
	}

	if (result != NULL) {
		LOG << "achou: " << result->bar << "!!!" << endl;
	} else {
		LOG << "nao achou!" << endl;
	}

	return result;
}

int CbPercPerfHistory::getTimesChosen(int ptrnMainIndex) const {
	int times = 0;
	map<int,int>::const_iterator it = timesChosen.find(ptrnMainIndex);

	if (it != timesChosen.end()) {
		times = it->second;
	}

	return times;
}

void CbPercPerfHistory::clear() {
	for (int i = 0; i < history.size(); i ++) {
		if (history[i] != NULL) {
			delete history[i];
			history[i] = NULL;
		}
	}
	history.clear();
	history.resize(16);
	timesChosen.clear();
}


