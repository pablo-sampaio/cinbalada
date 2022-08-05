
#ifndef __CBRHYTHMPATTERN_H__
#define __CBRHYTHMPATTERN_H__

#include "CbRhythmPatternInfo.h"
#include "CbPercussionInstrument.h"
#include "FAMA\MIDI\FmMidiFile.h"


/* Different representations of a rhythm pattern.
 * Obs: Definitions are below CbRhythmPattern.
 */
class CbTubs;
class CbOnsetsVector;
class CbInterOnsetVector;
class CbChronotonicVector;
class CbFullIntervalHistogram;


/* Criterias of avaliation (or comparation)
 * of (or between) rhythm patterns.
 */
typedef enum  {
	AVC_INVALID = -1,
	AVC_GEBER,
	AVC_CHRONOT_DIST,
	AVC_HAMMING_DIST,
	AVC_DERIVATION,
	AVC_OFFBEATNESS,
	AVC_ODDITY,
	AVC_COGNIT_COMPLEXITY,
	AVC_PS_COMPLEXITY,
	AVC_METRICITY,
	AVC_SYNCOPATION,
	AVC_CIRC_CHORD_EVENNESS,
	AVC_LIN_REG_EVENNESS,
	AVC_IRREGULARITY,
	AVC_ACTIVITY,
	AVC_AVERAGE_INTENSITY
} ECbRpEvalCriteria;

/**
 * This class represents a rhythm pattern with all its relevant informations:
 * high-level informations (from CbRhythmPatternInfo class) and performance
 * informations in MIDI (from FmMidiFile class).
 * The CbRhythmPattern class offers many different representations of the
 * rhythmic pattern, as well as methods to access and change such representations.
 * Attention: Recompilation of RhythmPatternControl may be needed after changes
 * in CbRhythmPattern.
 */
class CbRhythmPattern
{
private:

	CbRpInfo* patternInfo;    // high-level info about the pattern
	CbMeter* meter;           // meter (non-null only when patternInfo is null)
	FmMidiFileTrack* pattern; // performance information

	// alternate representations
	CbTubs* tubs;
	CbOnsetsVector* onsets;
	CbInterOnsetVector* interonsets;
	CbChronotonicVector* chronotonic;
	CbFullIntervalHistogram* onsetsDistFreq;

	static const char* criteriaStr[];

	FmMidiFileTrack* filterMidiEvents(const FmMidiFileTrack& track);

public:

	/* Methods to make convertions from 'ECbRhythmPtrnAvCriteria' to strings
	 * and from strings back to 'ECbRhythmPtrnAvCriteria'.
	 */
	static const char* evaluationCriteriaToStr(ECbRpEvalCriteria c);
	static ECbRpEvalCriteria strToEvaluationCriteria(const char* str);
	static bool isComparation(ECbRpEvalCriteria c);

	/* Constructs a mute rhythm pattern (i.e., without any onsets).
	 */
	CbRhythmPattern(const CbMeter &meter);

	/* Constructor. Receives the info about the pattern. The pattern is
	 * loaded (MIDI File) and the info is copied to the object self.
	 */
	CbRhythmPattern(const CbRpInfo&, const char* midiDirectory = "");

	/* Constructor used when only the MIDI informations is available. The
	 * MIDI information is copied.
	 * {deallocation of parameter 'meter'}: caller
	 */
	CbRhythmPattern(const FmMidiFileTrack& track, const CbMeter& meter);

	/* Copy constructor.
	 */
	CbRhythmPattern(const CbRhythmPattern&);

	virtual ~CbRhythmPattern();

	/* Returns an object with high-level information about the
	 * rhythm pattern. May return NULL.
	 */
	const CbRpInfo* getInfo() const;

	/* Returns the musical meter associated to the rhythm pattern.
	 */
	const CbMeter* getMeter() const;

	/* Returns the performance information of the pattern as MIDI.
	 */
	const FmMidiFileTrack* getPattern() const;

	/* Number of tatums per bar.
	 */
	int getTatumsPerBar() const;

	/* Total number of tatums.
	 */
	int getNumTatums() const;

	/* Number of bars (or measures).
	 */
	int getNumBars() const;

	/* Create and returns a specific bar of the whole pattern.
	 * {deallocation of result}: caller
	 */
	CbRhythmPattern* getBar(int b) const;

	/* Indicate whether the meters of the patterns are 'compatible'.
	 * The are so when the size (in tatums) of one of them is a divisor
	 * of the other meter.
	 */
	bool areMetricallyCompatible(const CbRhythmPattern* pattern) const;

	/* Compares the patterns using their TUBS representations.
	 */
	bool operator==(const CbRhythmPattern& rp) const;

	/* Print CbRhythmPattern objects to stream or to strings.
	 */
	friend std::ostream& operator<<(std::ostream& o, const CbRhythmPattern& rp);
	friend std::string& operator<< (std::string& out, const CbRhythmPattern& rp);


	//////////////////////////////////////
	/////// Representation Schemes ///////
	//////////////////////////////////////

	const CbTubs* getTubs() const;
	const CbOnsetsVector* getOnsetsVector() const;
	const CbInterOnsetVector* getInterOnsetVector() const;
	const CbChronotonicVector* getChronotonicVector() const;
	const CbFullIntervalHistogram* getFullIntervalHistogram() const;


	/////////////////////////////////////////////////
	//////// Comparation/Avaliation Functions ///////
	/////////////////////////////////////////////////

	/* Generic evaluationFunction function. The criteria of avaliation is indicated
	 * as a parameter. The second argument is ignored if its not a comparation
	 * criteria.
	 */
	double evaluationFunction(ECbRpEvalCriteria crit, const CbRhythmPattern *phrase = NULL) const;

	/* Diference between number of matches and number of mismatches between
	 * attack times of the two patterns (self and 'phrase').
	 * The lower the value, the better (supposedly). The final value is divided
	 * by the number of tatums to normalize it (an make results largely comparable)
	 * (TIREI NORMALIZACAO).
	 */
	double geberFunction(const CbRhythmPattern *phrase) const;

	/* Calculates the (dis)similarity between two rhythm patterns as the sum
	 * of the absolute diferences between the elements of their chronotonic
	 * representations. The final value is divided by the number of tatums.
	 */
	double chronotonicDistance(const CbRhythmPattern *phrase) const;

	/* Hamming distance, i.e. number of points of dissimilarity in the tubs
	 * representations. The final value is divided by the number of tatums.
	 */
	double hammingDistance(const CbRhythmPattern *phrase) const;

	/* Offbeats are positions that are relatively prime to the number of
	 * tatums of the bar. The final count is divided by the number of bars.
	 */
	double offbeatness() const;

	/* Counts the number of pairs of onsets separated by a half-bar distance.
	 * The final value is normalized according to the number of beats.
	 * (Slightly different from Toussaint's definition).
	 */
	double oddity() const;

	/* Metricity, defined by Toussaint inspired by Lerdahl & Jackendoff.
	 * The final value is normalized according to the number of bars.
	 */
	double metricity() const;

	/* Pressing's cognitive complexity. Up to now, it is only available for
	 * regular meters of 4-tatum beats. The final value is normalized according
	 * to the number of bars.
	 */
	double cognitiveComplexity() const;

	/* Povel-Shmulevich complexity measure. Still not working !
	 */
	double psComplexity() const;

	/* Toussaint's WNBD (Weighted Note-to-Beat Distance) measure of syncopation.
	 * Works both for regular and irregular meters. Even in the original
	 * computation, the value is divided by the number of notes.
	 */
	double wnbdSyncopation() const;

	/* Measures how equally-spaced the onsets of the pattern are.
	 * The first implementation uses the "regression-evenness" computation
	 * (with the final value divided by the number of notes for normalization
	 * purposes), while the second implementation uses the sum of all
	 * chord-distances of pairs of attack, in the circle (final value is
	 * not normalized in this case).
	 */
	double linearRegEvenness() const;
	double circleChordsEvenness() const;

	/* Measures the standard deviation of the inter-onsets intervals.
	 * It is probably a new measure of (un)evenness.
	 */
	double irregularity() const;

	/* Indicates how 'busy' the pattern is, in number of onsets per bar.
	 */
	double activity() const;

	/* Indicates how much 'this' can be considered a pattern derived
	 * from 'super'. A pattern is really derived if each of its inter-onset
	 * intervals is a sub-interval of a inter-onset interval of the original
	 * pattern.
	 * The values of this measure have the following meanings:
	 *    [+1]   - pattern 'this' is derived from 'super'
	 *    (0;+1) - pattern 'this' has some parts derived from 'super'
	 *    [0]    - patterns are not derived at all
	 *    (-1;0) - pattern 'super' has some parts derived from 'this'
	 *    [-1]   - pattern 'super' is derived from 'this'
	 */
	double derivationMeasure(const CbRhythmPattern *super) const;
	bool isDerivatedFrom(const CbRhythmPattern *super) const;

	/* The average value of the (dynamic) intensity used to play the onsets.
	 */
	double averageIntensity() const;

};


typedef std::vector<CbRhythmPattern*> Vector_pRhythmPattern;


/**
 * Time Unit Box System (TUBS) representation.
 */
class CbTubs
{
private:
	int tatumsBar;   // sub-divisions of the bar
	int tubsSize;    // always multiple of 'tatums'
	ECbPercNotePitch *tubs;

	friend CbRhythmPattern;
	CbTubs(const CbRhythmPattern& mdfile, const CbMeter& meter);
	CbTubs(const CbTubs&);

public:
	virtual ~CbTubs();
	int  getTatumsPerBar() const;
	int  getSize() const;
	int getNumBars() const;
	bool hasOnset(int tatum) const;
	ECbPercNotePitch get(int tatum) const;
	int  getNumOnsets() const;
	friend std::ostream& operator<<(std::ostream& o, const CbTubs& rp);
	friend std::string& operator<< (std::string& out, const CbTubs& rp);
};


/**
 * Vector of onset positions, considering TUBS representation.
 */
class CbOnsetsVector
{
private:
	const CbTubs* tubs;
	int numOnsets;
	int *vonsets;      // onsets positions (in TUBS representation)

	friend CbRhythmPattern;
	CbOnsetsVector(const CbTubs*);

public:
	virtual ~CbOnsetsVector();
	int getSize() const;
	int get(int vpos) const;
	ECbPercNotePitch getPitch(int vpos) const;
	const CbTubs* getTubs() const;
	friend std::ostream& operator<<(std::ostream& o, const CbOnsetsVector& rp);
};


/**
 * Vector of inter-onset intervals, considering the TUBS representation.
 * Obs.: the first position is the diference from the start of the pattern
 * to the first onset, and the last position is the absolute diference from
 * the last onset to the first onset of the next bar.
 */
class CbInterOnsetVector
{
private:
	int size;
	int *vintervals;

	friend CbRhythmPattern;
	CbInterOnsetVector(const CbOnsetsVector& onsets);

public:
	virtual ~CbInterOnsetVector();
	int getSize() const;
	int get(int vpos) const;
	friend std::ostream& operator<<(std::ostream& o, const CbInterOnsetVector& rp);
};


/**
 * Chronotonic vector, defined from the inter-onset intervals.
 */
class CbChronotonicVector
{
private:
	int size;
	int *vector;

	friend CbRhythmPattern;
	CbChronotonicVector(const CbInterOnsetVector& interonset, int tubsSize);

public:
	virtual ~CbChronotonicVector();
	int getSize() const;
	int get(int pos) const;
	int getMax() const;
	int getMin() const;
	friend std::ostream& operator<<(std::ostream& o, const CbChronotonicVector& rp);
};


/**
 * Histogram of distances between each pair of onsets. The
 * distance between each pair is the lowest one, considering rotations
 * in the pattern. For each possible distance, this representation
 * holds the frequency (number of occurences).
 */
class CbFullIntervalHistogram
{
private:
	int size;
	int *histogram;

	friend CbRhythmPattern;
	CbFullIntervalHistogram(const CbOnsetsVector& onsets);

public:
	virtual ~CbFullIntervalHistogram();
	int getMaxDistance() const;
	int getFrequency(int distance) const;
	friend std::ostream& operator<<(std::ostream& o, const CbFullIntervalHistogram& rp);
};


#endif
