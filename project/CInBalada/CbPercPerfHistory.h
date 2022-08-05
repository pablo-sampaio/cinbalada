//---------------------------------------------------------------------------

#ifndef __CB_PERC_PERF_HISTORY_H______
#define __CB_PERC_PERF_HISTORY_H______

#include "FAMA\FmAID.h"
#include "RhythmPattern\CbRhythmicRole.h"
//---------------------------------------------------------------------------


/**
 * Keeps historical information about the agent's
 * performance in a specific bar.
 */
struct CbPercPerfInfo {
	int bar;                    // number of the bar in the performance
	std::string role;           // role performed in the bar
	AID agentRef;               // reference agent (with the reference role)
	int patternIndex;           // main index (not the index by role)
	int patternBar;             // number of the bar in the pattern

	CbPercPerfInfo(int b, const CbRhythmicRole* r, AID ref, int ptrn, int ptrnBar)
		: bar(b), agentRef(ref), patternIndex(ptrn), patternBar(ptrnBar)
	{
		role = r->getName();
	}

};


/**
 * Keeps the history of a percussion performance.
 */
class CbPercPerfHistory {
private:
	std::vector<CbPercPerfInfo*> history;
	std::map<int,int> timesChosen;

public:

	CbPercPerfHistory(int size=16);
	virtual ~CbPercPerfHistory();

	void set(int bar, const CbRhythmicRole* role, AID refAg, int ptrnMainIndex, int patternBar=0);
	const CbPercPerfInfo* get(int bar) const;
	bool has(int bar);

	/* Search for the n-th entry for the given role, starting from the first
	 * or the last position.
	 */
	const CbPercPerfInfo* search(const char* role, int n = 0) const;
	const CbPercPerfInfo* searchBackward(const char* role, int n = 0) const;

	/* Erases an entry in the history
	 */
	void erase(int bar);

	int getTimesChosen(int ptrnMainIndex) const;

	void clear();

};


#endif