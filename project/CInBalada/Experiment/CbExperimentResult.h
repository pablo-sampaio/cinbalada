
#ifndef CB_EXPERIMENT_RESULT
#define CB_EXPERIMENT_RESULT

#include <vector>
#include "CbExperiment.h"


class CbExperimentResult {
private:

	typedef std::vector<CbExperiment*> ExperimentsVector;

	ExperimentsVector experiments;

public:

	CbExperimentResult();
	CbExperimentResult(AnsiString dir);

	~CbExperimentResult();
	void clear();

	void add(AnsiString experimentFile);

	int getNumExperiments();

	int countBest(ECbEvaluationType type);
	int countBest(ECbEvaluationType type, int round);

	int countWorst(ECbEvaluationType type);
	int countWorst(ECbEvaluationType type, int round);

	int countSecond(ECbEvaluationType type);
	int countSecond(ECbEvaluationType type, int round);

	int countQuality(ECbEvaluationType type, char quality);
	int countQuality(ECbEvaluationType type, int round, char quality);

	double getAverageVariety(ECbEvaluationType type);
	double getNormalizedAverageVariety(ECbEvaluationType type);

	int countMostVariated(ECbEvaluationType type);

};



#endif

