

#ifndef CB_EXPERIMENT
#define CB_EXPERIMENT

#include <system.hpp>
#include <map>

#include <stdio.h>


typedef enum { ET_UNKNOWN = -1,
			   ET_RANDOM = 10,
			   ET_SINGLE = 11,
			   ET_ROLE = 12 } ECbEvaluationType;


class CbExperiment {
public:

	// rounds in the first part
	static const int NUM_ROUNDS = 6;

	// number of sets in the second part
	static const int NUM_SETS = 2;

	// quality values
	static const char PESSIMO   = 'p';
	static const char RUIM      = 'r';
	static const char BOM       = 'b';
	static const char EXCELENTE = 'e';


private:

	static ECbEvaluationType evalTypes[6][3];

	static ECbEvaluationType getType(int round, char sample);
	static char getSample(int round, ECbEvaluationType tp);


	struct FirstPartRound {
		char sampleQualities[3];
		char best;
		char worst;

		FirstPartRound() : best('X'), worst('X')
		{ for (int i = 0; i < 3; i ++) sampleQualities[i] = 'x'; }

		char getQuality(char sample) { return sampleQualities[sample - 'A']; }

		void setQuality(char sample, int quality)
		{ sampleQualities[sample - 'A'] = quality; }
	};

	typedef std::map<int, FirstPartRound*> RoundsMap;

	struct SecondPart {
		int variety[2];
		int indexMostVariated;
		SecondPart() : indexMostVariated(-2) {
			variety[0] = variety[1] = -1;
		}
	};

	RoundsMap firstPart;
	SecondPart secondPart;

	// auxiliary methods

	void loadFirstPart(FILE* file);
	void loadSecondPart(FILE* file);

public:

	CbExperiment();
	CbExperiment(AnsiString file);

	~CbExperiment();

	// atribuicoes

	void setQuality(int round, char sample, char quality);
	void setBestSample(int round, char bestsample);
	void setWorstSample(int round, char worstsample);

	void setVariety(int set, int variety);
	void setMostVariated(int set);

	void load(AnsiString file);
	void save(AnsiString file);

	// acesso

	char getQuality(int round, char sample);
	char getQuality(int round, ECbEvaluationType type);

	char getBestSample(int round);
	ECbEvaluationType getBestSampleEvalType(int round);

	char getWorstSample(int round);
	ECbEvaluationType getWorstSampleEvalType(int round);

	char getSecondSample(int round);
	ECbEvaluationType getSecondSampleEvalType(int round);

	int getVariety(int set);
	int getVariety(ECbEvaluationType type);

	// sumario do experimento

	int countBest(ECbEvaluationType type);
	int countWorst(ECbEvaluationType type);
	int countSecond(ECbEvaluationType type);

	int countQuality(ECbEvaluationType type, char quality);

	int getMostVariated();
	ECbEvaluationType getMostVariatedEvalType();

};


#endif
