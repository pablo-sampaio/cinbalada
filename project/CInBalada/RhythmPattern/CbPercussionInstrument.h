


#ifndef ______CBPERCUSSIONINSTRUMENT_H_______
#define ______CBPERCUSSIONINSTRUMENT_H_______

#include <string>
#include <vector>

/* Possible "frequency classes" of an instrument.
 */
typedef enum {IFC_UNKNOWN = -1,
              IFC_LOW_BAND = 0,
              IFC_MED_BAND,
			  IFC_HIGH_BAND} ECbInstrFreqClass;

/* Relative pitches of notes of an instrument.
 */
typedef enum {NP_NONE = -1,
              NP_LOW = 0,
              NP_HIGH = 1} ECbPercNotePitch;


/* Notes a percussion instrument may play.
 */
struct CbPercNote {
	int midiNote;
	ECbPercNotePitch pitch;
	CbPercNote(int mdNt, ECbPercNotePitch ptch = NP_LOW)
		: midiNote(mdNt), pitch(ptch)
	{ }
};

typedef std::vector<CbPercNote> Vector_PercNotes;


/**
 * A percussion instrument with a frequency class
 * and a finite set of possible MIDI notes.
 */
class CbPercInstrument {
private:
	int index;
	std::string name;
	ECbInstrFreqClass freqClass;

	Vector_PercNotes notes;

	static const char* frequencyClassesStr[];
	static const char* percNotePitchesStr[];

public:

	/* Methods to make convertions from 'ECbInstrFreqClass' to strings
	 * and from strings to 'ECbInstrFreqClass'.
	 */
	static const char* frequencyClassToStr(ECbInstrFreqClass tp);
	static ECbInstrFreqClass strToFrequencyClass(const char* str);

	/* Methods to make convertions from 'ECbPercNotePitch' to strings
	 * and from strings to 'ECbPercNotePitch'.
	 */
	static const char* notePitchToStr(ECbPercNotePitch tp);
	static ECbPercNotePitch strToNotePitch(const char* str);

	CbPercInstrument(const char* name, ECbInstrFreqClass fq);
	CbPercInstrument(const std::string &name, ECbInstrFreqClass fq);

	std::string getName() const;

	ECbInstrFreqClass getFrequencyClass() const;
	void setFrequencyClass(ECbInstrFreqClass fc);

	int getNumNotes() const;
	CbPercNote getNote(int i) const;

    void addNote(const CbPercNote &nt);
    void removeNote(int midiNote);  // MUDAR: indice ?

};

typedef std::vector<CbPercInstrument*> Vector_pPercInstrument;


#endif /* ______CBPERCUSSIONINSTRUMENT_H_______ */
