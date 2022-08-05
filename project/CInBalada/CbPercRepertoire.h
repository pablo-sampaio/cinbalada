
#ifndef __CBAGENTREPERTOIRE_H__
#define __CBAGENTREPERTOIRE_H__

#include "RhythmPattern\CbRhythmPattern.h"
#include "RhythmPattern\CbRhythmPatternInfoRepository.h"
#include "RhythmPattern\CbRhythmicRole.h"
#include "CbPercPerfHistory.h"

/* An evaluation function to be used as an ordering criteria.
 * The first parameter is the pattern to which the evaluation
 * is to be done. The second parameter is a fixed rhythm pattern
 * to which all the repertoire is compared. Such parameter may
 * be ignored.
 */
typedef double (*CbRpEvalCriteria)(const CbRhythmPattern*, const CbRhythmPattern*);


/**
 * This class is used to hold a collection of rhythm patterns of a percussion
 * instrument. A percussionist agent uses (an object of) this class to hold
 * the repertoire it can play.
 * The rhythmic patterns are stored with two different kinds of indexes:
 *    - main index, that is unique for each pattern
 *    - secondary indexes, that are specific to the role that the pattern
 *      adheres to, i.e. the pair (role, secondary-index) identifies the
 *      pattern uniquely (although the pattern may be in more than one role)
 * Furthemore, the primary index is fixed during the repertoire lifecycle,
 * while the secondary index may change (e.g. when 'sort' is called).
 */
class CbPercRepertoire {
private:

	struct RepertItem {
		int index;
		CbRhythmPattern* pattern;
		bool enabled;
		int timesChosen;
		double eval;

		RepertItem(CbRhythmPattern* __rp, bool __enabled=true);
		virtual ~RepertItem();
	};

	typedef std::vector<RepertItem*> Vector_RepertItem;
	typedef std::map<std::string,Vector_RepertItem*> MapRolesItems;
	typedef std::map<std::string,int> MapRolesNum;

	CbPercInstrument* instrument;
	Vector_RepertItem items;           // all items, main indexing

	MapRolesItems rolesItems;          // items for each role, secondary indexing
	CbMeter* meter;

	const CbPercPerfHistory* history;  // used in the ordering penalties

	// bubble-sort
	void sorting_algorithm(Vector_RepertItem *v, bool ascending);

	// average number of times that all currently enabled pattern were chosen
	double getAvgTimesChosen(const char* role) const;

public:

	CbPercRepertoire(std::string instrName, const ICbRpInfoRepository& repository,
						const char* midiDirectory);
	virtual ~CbPercRepertoire();

	/* The patterns are filtered by meter compatibility and (re)classified by
	 * rhythmic roles. This method should be called if a role has been changed
	 * or added, or if meter has changed, for instance.
	 */
	void rearrange(const Vector_pRRoles& roles, const CbMeter* meter);
	const CbMeter* getMeter() const;

	/* Instrument of the rhythm patterns.
	 */
	std::string getInstrumentName() const;
	const CbPercInstrument* getInstrument() const;

	/* Methods to add/access all the patterns in the repertoire.
	 */
	void addRhythmPattern(const CbRpInfo& rpi, const char* midiDirectory = "");
	void addRhythmPattern(CbRhythmPattern* pattern);
	int getNumRhythmPatterns() const;
	const CbRhythmPattern* getRhythmPattern(int mainIndex) const;

	/* Sets the history of the performance.
	 */
	void setHistory(const CbPercPerfHistory* h);

	/* Returns the main index of a pattern (from its secondary index, by role).
	 */
	int getMainIndex(const char* roleName, int roleIndex) const;

	/* Returns the secondary index of a pattern in a given role.
	 */
	int getRoleIndex(const char* roleName, int mainIndex) const;

	/* To access the set of patterns of a specific rhythmic role.
	 * Indexes may change after a 'sort' operation.
	 */
	int getNumRhythmPatterns(const char* roleName) const;
	const CbRhythmPattern* getRhythmPattern(const char* roleName, int index);

	/* True if at least one rhythm pattern is eligible for the rhythmic role.
	 */
	bool hasPatternInRole(const CbRhythmicRole* role);

	/* Musical styles stored in (any of) the rhythm patterns.
	 */
	void getStyles(Vector_string& results) const;

	/* Filters to individual rhythm patterns (by its main indexes).
	 */
	void setEnabled(int indexPattern, bool enabled);
	bool isEnabled(int indexPattern);

	/* Sorts all elements, according to the given criteria. The criteria may
	 * me a comparation with a reference pattern.
	 */
	void sort(const char* roleName, ECbRpEvalCriteria criteria, const CbRhythmPattern* rp = NULL, bool ascending = true);
	void sort(const char* roleName, CbRpEvalCriteria criteriaFunc, const CbRhythmPattern* rp = NULL, bool ascending = true);

	/* Like previous 'sort' operations, but the evaluation of the pattern takes
	 * into account the number of times it was previously chosen to assign
	 * penalties to the final evaluation of the pattern. If the pattern is the
	 * last choice (for the role), it doesn't receive penalties.
	 * The penalties may be of 10% or 20% of the value given by the function.
	 * The can be added or subtracted, depending on the 'ascending' parameter,
	 * so that the pattern with penalties are shifted to the last positions.
	 */
	void sortWithPenalties(const char* roleName, ECbRpEvalCriteria criteria, const CbRhythmPattern* rp = NULL, bool ascending = true);
	void sortWithPenalties(const char* roleName, CbRpEvalCriteria criteriaFunc, const CbRhythmPattern* rp = NULL, bool ascending = true);

};


#endif /* __CBAGENTREPERTOIRE_H__ */
