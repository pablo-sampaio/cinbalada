//---------------------------------------------------------------------------

#ifndef ___CB_RHYTHMIC_ROLE_______
#define ___CB_RHYTHMIC_ROLE_______

#include "CbRhythmPattern.h"

//---------------------------------------------------------------------------

// forward declarations
class CbPercPerfHistory;
class CbPercRepertoire;



/**
 * Abstract class for rhythmic roles.
 */
class CbRhythmicRole
{
protected:
	std::string name;
	std::string reference;

	double evalPatternThreshold;

	CbRhythmicRole(const char* __name, const char* __reference, double evalThresh);

public:

	/* Name that uniquely identifies the rhythmic role.
	 */
	const char* getName() const;

	/* The name of the role (whose patterns are) used as reference to this
	 * role in a performance. Can be NULL if the role has no reference role.
	 */
	const char* getReferenceRole() const;

	/* Evaluates the degree to wich the given rhythmic pattern is
	 * suitable for the role.
	 */
	virtual double evaluate(const CbRhythmPattern &rp) const = 0;

	/* The evaluation threshold above which the patterns are
	 * considered unsuitable for the role.
	 */
	virtual double getEvaluationThreshold() const;
	virtual void setEvaluationThreshold(double t);

	/* Indicates if a pattern fits the role.
	 */
	bool patternIsSuitable(const CbRhythmPattern &rp) const;

	/* Indicates the degree to which two patterns of the role match
	 * (i.e. indicates how well the two patterns sound together).
	 */
	virtual double evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const = 0;

	/* The degree to which the pattern of this role is considered
	 * to match the pattern of the reference role.
	 */
	virtual double evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const = 0;

	/* Evaluate the pattern adherence to the history of the performance.
	 */
	virtual double evaluateMatchHistory(const CbRhythmPattern &rp, CbPercPerfHistory* history,
										CbPercRepertoire* rep, int bar) const;

	/* Tests whether rhythmic role 'this' is the reference of the
	 * the role given as parameter.
	 */
	bool isReferenceTo(const CbRhythmicRole&) const;

	/* Compares the rhythmic roles by its names.
	 */
	bool operator==(const CbRhythmicRole& r) const;


};

typedef std::vector<CbRhythmicRole*> Vector_pRRoles;


/**
 * "Basic Role", for maintaining the whole performance, as a kind of a metronome.
 * Its patterns have high regularity and low activity.
 */
class CbBasicRole : public CbRhythmicRole
{
private:
	double evaluateQuaternary(const CbRhythmPattern &rp) const;
	double evaluateTernary(const CbRhythmPattern &rp) const;

public:
	CbBasicRole();

	virtual double evaluate(const CbRhythmPattern &rp) const;

	virtual double evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const;

	virtual double evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const;

	virtual double evaluateMatchHistory(const CbRhythmPattern &rp, CbPercPerfHistory* history,
										CbPercRepertoire* rep, int bar) const;

};


/**
 * "Complementary-Basic Role", for maintaining the performance 'complementing'
 * or 'filling' the tatums not played by a 'basic-role' pattern (the reference
 * role). Its patterns have high regularity and relatively high activity.
 */
class CbComplBasicRole : public CbRhythmicRole
{
private:
	double evaluateQuaternary(const CbRhythmPattern &rp) const;
	double evaluateTernary(const CbRhythmPattern &rp) const;

public:
	CbComplBasicRole();

	virtual double evaluate(const CbRhythmPattern &rp) const;

	virtual double evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const;

	virtual double evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const;

	virtual double evaluateMatchHistory(const CbRhythmPattern &rp, CbPercPerfHistory* history,
										CbPercRepertoire* rep, int bar) const;

};


/**
 * "Solo Role", to embellish the performance with less predictable patterns.
 *  The patterns are irregular and, preferably, complex (activity may vary).
 */
class CbSoloRole : public CbRhythmicRole
{
private:
	double evaluateQuaternary(const CbRhythmPattern &rp) const;
	double evaluateTernary(const CbRhythmPattern &rp) const;

public:
	CbSoloRole();

	virtual double evaluate(const CbRhythmPattern &rp) const;

	virtual double evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const;

	virtual double evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const;

	virtual double evaluateMatchHistory(const CbRhythmPattern &rp, CbPercPerfHistory* history,
										CbPercRepertoire* rep, int bar) const;

};


/**
 * "Fill Role", to make a (high intensity) transition between phrases.
 * The patterns have a very high activity and, usually, end with a
 * strong single stroke.
 */
class CbFillRole : public CbRhythmicRole
{
public:
	CbFillRole();

	virtual double evaluate(const CbRhythmPattern &rp) const;

	virtual double evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const;

	virtual double evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const;

};


/**
 * A rhythmic role that only allows a pattern with no onsets.
 */
class CbMuteRole : public CbRhythmicRole
{
public:
	CbMuteRole();

	virtual double evaluate(const CbRhythmPattern &rp) const;

	virtual double evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const;

	virtual double evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const;

};


/**
 * A rhythmic role with no restriction uppon the rhythm patterns.
 */
class CbFreeRole : public CbRhythmicRole
{
public:
	CbFreeRole();

	virtual double evaluate(const CbRhythmPattern &rp) const;

	virtual double evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const;

	virtual double evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const;

};


/**
 * A rhythmic role that has no filter over single patterns and doesn't
 * have a reference role, but that uses a criteria for horizontal evaluation.
 */
class CbUniformRole : public CbRhythmicRole
{
public:
	CbUniformRole();

	virtual double evaluate(const CbRhythmPattern &rp) const;

	virtual double evaluateMatch(const CbRhythmPattern &a, const CbRhythmPattern &b) const;

	virtual double evaluateMatchRef(const CbRhythmPattern &rp, const CbRhythmPattern &ref) const;

	virtual double evaluateMatchHistory(const CbRhythmPattern &rp, CbPercPerfHistory* history,
	                                    CbPercRepertoire* rep, int bar) const;

};


//---------------------------------------------------------------------------
#endif
