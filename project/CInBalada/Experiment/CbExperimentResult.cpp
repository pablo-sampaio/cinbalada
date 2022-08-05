
#include "CbExperimentResult.h"

using namespace std;

CbExperimentResult::CbExperimentResult() {
}

CbExperimentResult::CbExperimentResult(AnsiString dir) {
	//...
}

CbExperimentResult::~CbExperimentResult() {
	clear();
}

void CbExperimentResult::clear() {
	ExperimentsVector::iterator it;
	for (it = experiments.begin(); it != experiments.end(); it ++) {
		delete (*it);
		(*it) = NULL;
	}
	experiments.clear();
}

void CbExperimentResult::add(AnsiString experimentFile) {
	experiments.push_back(new CbExperiment(experimentFile));
}

int CbExperimentResult::getNumExperiments() {
	return experiments.size();
}

int CbExperimentResult::countBest(ECbEvaluationType type) {
	int count = 0;

	ExperimentsVector::iterator it;
	for (it = experiments.begin(); it != experiments.end(); it ++) {
		count += (*it)->countBest(type);
	}

	return count;
}

int CbExperimentResult::countBest(ECbEvaluationType type, int round) {
	int count = 0;

	ExperimentsVector::iterator it;
	for (it = experiments.begin(); it != experiments.end(); it ++) {
		if ((*it)->getBestSampleEvalType(round) == type) {
			count ++;
		}
	}

	return count;
}

int CbExperimentResult::countWorst(ECbEvaluationType type) {
	int count = 0;

	ExperimentsVector::iterator it;
	for (it = experiments.begin(); it != experiments.end(); it ++) {
		count += (*it)->countWorst(type);
	}

	return count;
}

int CbExperimentResult::countWorst(ECbEvaluationType type, int round) {
	int count = 0;

	ExperimentsVector::iterator it;
	for (it = experiments.begin(); it != experiments.end(); it ++) {
		if ((*it)->getWorstSampleEvalType(round) == type) {
			count ++;
		}
	}

	return count;
}


int CbExperimentResult::countSecond(ECbEvaluationType type) {
	int count = 0;

	ExperimentsVector::iterator it;
	for (it = experiments.begin(); it != experiments.end(); it ++) {
		count += (*it)->countSecond(type);
	}

	return count;
}

int CbExperimentResult::countSecond(ECbEvaluationType type, int round) {
	int count = 0;

	ExperimentsVector::iterator it;
	for (it = experiments.begin(); it != experiments.end(); it ++) {
		if (((*it)->getSecondSampleEvalType(round) == type)) {
			count ++;
		}
	}

	return count;
}

int CbExperimentResult::countQuality(ECbEvaluationType type, char quality) {
	int count = 0;

	ExperimentsVector::iterator it;
	for (it = experiments.begin(); it != experiments.end(); it ++) {
		count += (*it)->countQuality(type, quality);
	}

	return count;
}

int CbExperimentResult::countQuality(ECbEvaluationType type, int round, char quality) {
	int count = 0;

	ExperimentsVector::iterator it;
	for (it = experiments.begin(); it != experiments.end(); it ++) {
		if ((*it)->getQuality(round, type) == quality) {
			count ++;
		}
	}

	return count;
}

double CbExperimentResult::getAverageVariety(ECbEvaluationType type) {
	int size = experiments.size();
	double sum = 0.0;

	for (int i = 0; i < size; i ++) {
		sum += experiments[i]->getVariety(type);
	}

	return sum / double(size);
}

double CbExperimentResult::getNormalizedAverageVariety(ECbEvaluationType type) {
	int size = experiments.size();
	double normalizedDiff;
	double sum = 0.0;

	for (int i = 0; i < size; i ++) {
		//normalizedDiff = experiments[i]->getVariety(ET_RANDOM) - experiments[i]->getVariety(type);
		normalizedDiff = experiments[i]->getVariety(type) / double(experiments[i]->getVariety(ET_RANDOM));
		sum += normalizedDiff;
	}

	return sum / double(size);
}

int CbExperimentResult::countMostVariated(ECbEvaluationType type) {
	int count = 0;

	ExperimentsVector::iterator it;
	for (it = experiments.begin(); it != experiments.end(); it ++) {
		if ((*it)->getMostVariatedEvalType() == type) {
			count ++;
		}
	}

	return count;
}

