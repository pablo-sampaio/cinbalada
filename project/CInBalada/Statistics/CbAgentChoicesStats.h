
#ifndef ____CB_AGENT_CHOICES_STATS___
#define ____CB_AGENT_CHOICES_STATS___

#include <vector>
#include <ostream>


/**
 * This class holds basic statics about the tuples of patterns
 * choose by CInBalada agentes. Each position in such tuples hold
 * the pattern main index chosen by a specific agent.
 */
class CbAgentChoicesStats {
public:
	class Tuple;

protected:
	typedef std::vector<Tuple*> TuplesVector;

	TuplesVector tuplesFreq;

public:

	// comparations between tuples
	static bool lessFrequent(const Tuple* &a, const Tuple* &b);
	static bool coordinatesComparation(const Tuple* &a, const Tuple* &b);
	static bool closestToOrigin(const Tuple* &a, const Tuple* &b);
	typedef bool (*TuplesComparator)(const Tuple* &a, const Tuple* &b);

	// construction/destruction
	CbAgentChoicesStats();
	virtual ~CbAgentChoicesStats();

	void addTuple(const Tuple& tp);
	void clear();

	void saveFile(const char* fileName);
	void saveFile(const char* fileName, TuplesComparator comparator);

};


class CbAgentChoicesStats::Tuple {
protected:
	friend CbAgentChoicesStats;

	int count;
	int* values;

	int frequency; // usado apenas por 'CbAgentChoicesStats'

public:

	Tuple(int c);
	Tuple(const Tuple& tp);

	virtual ~Tuple();

	void set(int pos, int v);
	int get(int pos) const;
	int size() const;

	int originDistance() const;
	bool operator==(const Tuple& tp) const;
	friend std::ostream& operator<<(std::ostream& o, const Tuple& tp);
};


#endif
