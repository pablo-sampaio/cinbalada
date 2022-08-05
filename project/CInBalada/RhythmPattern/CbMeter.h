
#ifndef ____CBMETER_H________
#define ____CBMETER_H________

class CbRhythmPattern;
class CbBeat;

typedef enum { TL_INVALID = -1, TL_VERY_SLOW, TL_SLOW, TL_MED_SLOW,
					 TL_MODERATE, TL_MED_FAST, TL_FAST, TL_VERY_FAST } CbETempoLabel;


/**
 * Regular meters (with beats of same size) and complex meter
 * (with beats of different sizes).  The meters are based on grids
 * of atomic pulses called 'tatums'. Each beat is a grouping of
 * tatums (the tatums are subdivisions of the beats). The beats
 * have different relative weights and, inside the beat, the tatums
 * also receive different relative weights.
 */
class CbMeter {
protected:
	static const char* tempoLabels[];

	bool regular;      // indicates wheter its a regular meter
	int numBeats;      // number of beats
	CbBeat **beats;    // beats array

	CbRhythmPattern *beatsRp;  // the beats represented as a rhythm pattern

	int tatumsPerQNote;        // to make conversions to/from standard notation and MIDI

public:

	/* Most typical meters in ocidental music. 
	 */
	static const CbMeter SIMPLE_DUPLE;
	static const CbMeter SIMPLE_TRIPLE;
	static const CbMeter SIMPLE_QUADR;
	static const CbMeter COMPOUND_DUPLE;
	static const CbMeter COMPOUND_TRIPLE;
	static const CbMeter COMPOUND_QUADR;

	/* Constructor for regular meters. Allowed beat values: 2, 3 and 4.
	 * Allowed values for beat-tatums are 4 (simple meter) and 3
	 * (compound meter). Beat tatums are the subdivisions of the beat in atomic
	 * pulses, i.e. the number of 8th-note, for compound meters; or 16th-note,
	 * for simple meters.
	 */
	CbMeter(int numBeats, int beatTatums);

	/* Constructor for irregular (complex) meters. The irregular
	 * meter here is created as a sequence of regular meters.
	 */
	CbMeter(CbMeter* array, int size);

	/* Constructor for irregular (complex) meters. The irregular
	 * meter is created from the given beat sizes (in number of tatums).
	 */
	CbMeter(int *beatTatums, int size);

	/* Copy constructor.
	 */
	CbMeter(const CbMeter&);

	/* Destructor.
	 */
	virtual ~CbMeter();

	bool isRegular() const;
	int getNumTatums() const;
	int getMetricity(int tatum) const;

	/* Beats information.
	 */
	const CbBeat* getBeat(int b) const;
	int getNumBeats() const;
	int getTatumsInBeat(int b) const;
	int getMaxTatumsInBeat() const;
	int getMinTatumsInBeat() const;
	int getBeatOfTatum(int tatum) const;
	int meterTatumToBeatTatum(int beat, int meterTatum) const;
	int beatTatumToMeterTatum(int beat, int beatTatum) const;

	/* Conversions.
	 */
	const CbRhythmPattern* getBeatsRhythmPattern() const;
	const int getTatumsPerQuarterNote() const;
	const double getNumQuarterNotes() const;

	/* Minimum/maximum tempo in bpm wich is perceptually acceptable.
	 * Values of tempo that lie outside these limits cause changes
	 * in metric perception.
	 */
	int getMinTempo() const;
	int getMaxTempo() const;

	/* Range of tempo in bpm that are preferred by humam perception.
	 * These values are inside the interval defined by getMinTempo()
	 * and getMaxTempo().
	 */
	int getMinPreferedTempo() const;
	int getMaxPreferedTempo() const;

	/* Conversions between tempo label and strings.
	 */
	static const char* tempoLabelToStr(CbETempoLabel lbl);
	static CbETempoLabel strToTempoLabel(const char* str);

	/* Conversions between tempo in BPM and tempo labels.
	 */
	CbETempoLabel getTempoLabel(int tempoBpm) const;
	int getTypicalTempo(CbETempoLabel label) const;

	/* A meter 'a' is compatible with a meter 'b' if the number of
	 * tatums of 'b' is a multiple of the tatums of 'a'.
	 */
	bool isCompatibleWith(const CbMeter*) const;
};


/**
 * Beats (big pulses) of a meter.
 */
class CbBeat {
private:
	friend CbMeter;

	int *metricity;    // metricity for each atomic pulse (tatum)
	int numTatums;     // number of tatums

	int beatMetricity;

	CbBeat(int subdiv);
	CbBeat(const CbBeat&);

public:
	virtual ~CbBeat();

	int getMetricity(int tatum) const;
	int getNumTatums() const;
	int getBeatMetricity() const;

};


#endif
