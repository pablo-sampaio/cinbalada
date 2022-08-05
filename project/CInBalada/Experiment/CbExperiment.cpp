
#include "CbExperiment.h"

using namespace std;


ECbEvaluationType CbExperiment::evalTypes[6][3] = {
//	       [A]         [B]         [C]
	  { ET_RANDOM,	ET_ROLE,	ET_SINGLE 	},	// 1
	  { ET_RANDOM,	ET_SINGLE,	ET_ROLE 	},	// 2
	  { ET_SINGLE,	ET_RANDOM,	ET_ROLE 	},	// 3
	  { ET_ROLE,	ET_RANDOM,	ET_SINGLE 	},	// 4
	  { ET_ROLE,	ET_SINGLE,	ET_RANDOM 	},	// 5
	  { ET_SINGLE,	ET_ROLE,	ET_RANDOM 	} 	// 6
/*/
	  { ET_RANDOM,	ET_SINGLE,	ET_ROLE 	},	// 1
	  { ET_RANDOM,	ET_SINGLE,	ET_ROLE 	},	// 2
	  { ET_RANDOM,	ET_SINGLE,	ET_ROLE 	},	// 3
	  { ET_RANDOM,	ET_SINGLE,	ET_ROLE 	},	// 4
	  { ET_RANDOM,	ET_SINGLE,	ET_ROLE 	},	// 5
	  { ET_RANDOM,	ET_SINGLE,	ET_ROLE 	},	// 6
//*/
};

ECbEvaluationType CbExperiment::getType(int round, char sample) {
	return evalTypes[round-1][sample - 'A'];
}

char CbExperiment::getSample(int round, ECbEvaluationType tp) {
	char sample = 'X';

	for (char s = 'A'; s <= 'C'; s ++) {
		if (getType(round, s) == tp) {
			sample = s;
			break;
		}
	}

	return sample;
}


CbExperiment::CbExperiment() {
}

CbExperiment::CbExperiment(AnsiString file) {
	load(file);
}

void CbExperiment::load(AnsiString fileName) {
	FILE* file = fopen(fileName.c_str(), "r");
	int part = -1;

	while (feof(file) == 0) {
		fscanf(file, "%d\n", &part);

		if (part == 1) {
			loadFirstPart(file);
		} else if (part == 2) {
			loadSecondPart(file);
		}
	}

}

void CbExperiment::loadFirstPart(FILE* file) {
	int round = -1;

	char qualityA = -1;
	char qualityB = -1;
	char qualityC = -1;

	char best = -1;
	char worst = -1;

	for (int i = 0; i < NUM_ROUNDS; i ++) {
		fscanf(file, "%d %c%c%c %c %c\n", &round, &qualityA,
										  &qualityB, &qualityC, &best, &worst);

		setQuality(round, 'A', qualityA);
		setQuality(round, 'B', qualityB);
		setQuality(round, 'C', qualityC);
		setBestSample(round, best);
		setWorstSample(round, worst);
	}

}

void CbExperiment::loadSecondPart(FILE* file) {
	int set = -1;
	int variety = -1;

	int mostVariated = -1;

	for (int i = 0; i < NUM_SETS; i ++) {
		fscanf(file, "%d %d\n", &set, &variety);
		setVariety(set, variety);
	}

	fscanf(file, "%d", &mostVariated);
	setMostVariated(mostVariated);

}

CbExperiment::~CbExperiment() {
	RoundsMap::iterator it;
	for (it = firstPart.begin(); it != firstPart.end(); it ++) {
		delete it->second;
		it->second = NULL;
	}
}

void CbExperiment::setQuality(int round, char sample, char quality) {
	if (firstPart.find(round) == firstPart.end()) {
		firstPart[round] = new FirstPartRound();
	}

	firstPart[round]->setQuality(sample, quality);
}

void CbExperiment::setBestSample(int round, char bestsample) {
	if (firstPart.find(round) == firstPart.end()) {
		firstPart[round] = new FirstPartRound();
	}

	firstPart[round]->best = bestsample;
}

void CbExperiment::setWorstSample(int round, char worstsample) {
	if (firstPart.find(round) == firstPart.end()) {
		firstPart[round] = new FirstPartRound();
	}

	firstPart[round]->worst = worstsample;
}

void CbExperiment::setVariety(int set, int variety) {
	secondPart.variety[set - 1] = variety;
}

void CbExperiment::setMostVariated(int set) {
	secondPart.indexMostVariated = set - 1;
}

void CbExperiment::save(AnsiString file) {
	//...
}

char CbExperiment::getQuality(int round, char sample) {
	return this->firstPart[round]->getQuality(sample);
}

char CbExperiment::getQuality(int round, ECbEvaluationType type) {
	char sample = getSample(round, type);
	return this->firstPart[round]->getQuality(sample);
}

char CbExperiment::getBestSample(int round) {
	return this->firstPart[round]->best;
}

ECbEvaluationType CbExperiment::getBestSampleEvalType(int round) {
	return getType(round, getBestSample(round));
}

char CbExperiment::getWorstSample(int round) {
	return this->firstPart[round]->worst;
}

ECbEvaluationType CbExperiment::getWorstSampleEvalType(int round) {
	return getType(round, getWorstSample(round));
}

char CbExperiment::getSecondSample(int round) {
	char sample = 'X';

	for (char c = 'A'; c <= 'C'; c ++) {
		if ((this->firstPart[round]->best != c)
			&& (this->firstPart[round]->worst != c)) {
			sample = c;
		}
	}

	return sample;
}

ECbEvaluationType CbExperiment::getSecondSampleEvalType(int round) {
	return getType(round, getSecondSample(round));
}


int CbExperiment::getVariety(int set) {
	return this->secondPart.variety[set - 1];
}

int CbExperiment::getVariety(ECbEvaluationType type) {
	int var = -1;

	if (type == ET_RANDOM) {
		var = secondPart.variety[0];
	} else if (type == ET_ROLE) {
		var = secondPart.variety[1];
	}

	return var;
}

int CbExperiment::countBest(ECbEvaluationType type) {
	int count = 0;
	char bestSample;

	for (int r = 1; r <= NUM_ROUNDS; r ++) {
		bestSample = this->firstPart[r]->best;
		if (getType(r, bestSample) == type) {
			count ++;
		}
	}

	return count;
}

int CbExperiment::countWorst(ECbEvaluationType type) {
	int count = 0;
	char worstSample;

	for (int r = 1; r <= NUM_ROUNDS; r ++) {
		worstSample = this->firstPart[r]->worst;
		if (getType(r, worstSample) == type) {
			count ++;
		}
	}

	return count;
}

int CbExperiment::countSecond(ECbEvaluationType type) {
	int count = 0;

	for (int r = 1; r <= NUM_ROUNDS; r ++) {
		if (getSecondSampleEvalType(r) == type) {
			count ++;
		}
	}

	return count;
}

int CbExperiment::countQuality(ECbEvaluationType type, char quality) {
	int count = 0;
	char sampleOfType;

	for (int r = 1; r <= 6; r ++) {
		sampleOfType = getSample(r, type);
		if (firstPart[r]->getQuality(sampleOfType) == quality) {
			count ++;
		}
	}

	return count;
}

int CbExperiment::getMostVariated() {
	return secondPart.indexMostVariated + 1;
}

ECbEvaluationType CbExperiment::getMostVariatedEvalType() {
	ECbEvaluationType type = ET_UNKNOWN;

	switch (secondPart.indexMostVariated) {
	case 0:
		type = ET_RANDOM;
		break;
	case 1:
		type = ET_ROLE;
		break;
	}

	return type;
}
