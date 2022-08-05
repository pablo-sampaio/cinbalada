
#include "CbAgentChoicesStats.h"

#include <fstream>
#include <algorithm>

using namespace std;


///////////  CbAgentChoicesStats  ///////////

CbAgentChoicesStats::CbAgentChoicesStats() {
}

CbAgentChoicesStats::~CbAgentChoicesStats() {
	clear();
}

void CbAgentChoicesStats::addTuple(const Tuple& tp) {
	bool inserted = false;
	Tuple* curr;

	for (int i = 0; i < this->tuplesFreq.size(); i ++) {
		curr = this->tuplesFreq[i];
		if ((*curr) == tp) {
			curr->frequency ++;
			inserted = true;
			break;
		}
	}

	if (! inserted) {
		this->tuplesFreq.push_back(new Tuple(tp));
	}
}

void CbAgentChoicesStats::clear() {
	for (int i = tuplesFreq.size() - 1; i >= 0; i --) {
		delete tuplesFreq[i];
	}
	this->tuplesFreq.clear();
}

bool CbAgentChoicesStats::lessFrequent(const Tuple* &a, const Tuple* &b) {
	return a->frequency < b->frequency;
}

bool CbAgentChoicesStats::coordinatesComparation(const Tuple* &a, const Tuple* &b) {
	bool equal = true;
	bool result = false;

	int i = 0;
	while ((i < a->size()) && (equal == true)) {
		equal = (a->get(i) == b->get(i));
		result = (a->get(i) < b->get(i));
		i ++;
	}

	return result;
}

bool CbAgentChoicesStats::closestToOrigin(const Tuple* &a, const Tuple* &b) {
	return a->originDistance() < b->originDistance();
}

void CbAgentChoicesStats::saveFile(const char* fileName) {
	ofstream outFile (fileName, ios::out);

	sort(this->tuplesFreq.begin(), this->tuplesFreq.end(), lessFrequent);

	for (int i = 0; i < tuplesFreq.size(); i ++) {
		outFile << *(this->tuplesFreq[i]) << "\t" << this->tuplesFreq[i]->frequency << endl;
	}
}

void CbAgentChoicesStats::saveFile(const char* fileName, CbAgentChoicesStats::TuplesComparator comparator) {
	ofstream outFile (fileName, ios::out);

	sort(this->tuplesFreq.begin(), this->tuplesFreq.end(), comparator);

	for (int i = 0; i < tuplesFreq.size(); i ++) {
		outFile << *(this->tuplesFreq[i]) << "\t" << this->tuplesFreq[i]->frequency << endl;
	}
}


///////////  CbAgentChoicesStats::Tuple  ///////////

CbAgentChoicesStats::Tuple::Tuple(int cnt) {
	this->count = cnt;
	this->values = new int[cnt];

	for (int i = 0; i < this->count; i ++) {
		this->values[i] = -1;
	}

	this->frequency = 1;
}

CbAgentChoicesStats::Tuple::Tuple(const Tuple& tp) {
	this->count = tp.count;
	this->values = new int[tp.count];

	for (int i = 0; i < tp.count; i ++) {
		this->values[i] = tp.values[i];
	}

	this->frequency = tp.frequency;
}

CbAgentChoicesStats::Tuple::~Tuple() {
	delete[] this->values;
}

void CbAgentChoicesStats::Tuple::set(int pos, int v) {
	this->values[pos] = v;
}

int CbAgentChoicesStats::Tuple::get(int pos) const {
	return this->values[pos];
}

int CbAgentChoicesStats::Tuple::size() const {
	return this->count;
}

int CbAgentChoicesStats::Tuple::originDistance() const {
	int sum = 0;

	for (int i = 0; i < this->count; i ++) {
		sum += this->values[i] * this->values[i];
	}

	return sum;
}

bool CbAgentChoicesStats::Tuple::operator==(const Tuple& tp) const {
	bool result = true;

	int i = 0;
	while ((i < count) && (result == true)) {
		result = (this->values[i] == tp.values[i]);
		i ++;
	}

	return result;
}

ostream& operator<<(ostream& o, const CbAgentChoicesStats::Tuple& tp) {
	o << "(";

	for (int i = 0; i < tp.count; i ++) {
		o << tp.values[i];
		if (i != (tp.count - 1)) {
			o << ", ";
		}
	}

	o << ")";

	return o;
}
