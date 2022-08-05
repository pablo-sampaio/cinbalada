
#ifndef __CBRHYTHMPATTERNINFO_H__
#define __CBRHYTHMPATTERNINFO_H__

#include <string>
#include <vector>

#include "CbMeter.h"


typedef std::vector<std::string> Vector_string;

/**
 * This data type represents the types of rhythm patterns.
 */
typedef enum {RPT_UNKNOWN = 0,
			  RPT_ROLL    = 1,
			  RPT_FILL    = 2,
			  RPT_BASIC   = 3} ECbRpType;


/**
 * This class keeps high-level information about rhythm patterns.
 * The rhythm patterns themselves are stored in a MIDI file, whose full
 * path is given by the attribute 'filePath'.
 */
class CbRpInfo {
private:
    int id;
    std::string name;
    std::string fileName;
    std::string instrument;
    int track;
	ECbRpType type;

	Vector_string styles;

	CbMeter* meter;

	static const char* patternTypesStr[];

public:

	/* Methods to make convertions from 'ECbRhythmPatternType' to strings
	 * and from strings to 'ECbRhythmPatternType'.
	 */
	static const char* rhythmPatternTypeToStr(ECbRpType tp);
	static ECbRpType strToRhythmPatternType(const char* str);

	/* Constructors. The parameter 'name' is the only that can be null.
	 * {deallocation of parameter 'meter'}: this class
	 */
	CbRpInfo(int id, const char* name, const char* path, const char* instr,
				ECbRpType tp, CbMeter* metr, int trkNum = 1);
	CbRpInfo(const CbRpInfo& pattern);

	/* Destructor.
	 */
	virtual ~CbRpInfo();

    /* Name of the pattern.
     */
    std::string getName() const;

    /* Name of the audio file containing the pattern rhythm itself.
     */
    std::string getFileName() const;

    /* Number of the track in the MIDI file where the pattern itself can be found.
     */
    int getTrack() const;

    /* Name of the instrument played in the rhythm pattern file.
     * (There should be only one instrument in each rhythm pattern).
     */
    std::string getInstrument() const;

    /* Identifier of the rhythm pattern. In a repository, the identifier
     * is unique to each pattern.
     */
    int getId() const;

    /* Type (or role) of the pattern rhythm in typical musical performances.
     */
	ECbRpType getType() const;
    const char* getTypeStr() const;

	/* Number of musical styles stored. Styles can ben retrieved calling
	 * the method 'getStyle' using integer indexes in the interval [0;n-1],
	 * where 'n' is the number of styles stored.
	 */
	int getNumStyles() const;

	/* Returns the musical styles in the given index. The musical styles
	 * stored are only those where the rhythm pattern is commonly found.
	 */
	std::string getStyle(int index) const;

	/* Indicates if the given musical style is present.
	 */
	bool hasStyle(const std::string &str) const;

	/* The musical meter.
	 */
	const CbMeter* getMeter() const;

	/* Sets the (unique) indetifier of the RhythmPattern. Can only be used
	 * by the Repository.
	 */
	void setId(int id);

	/* Sets the name.
	 */
	void setName(const char* name);

	/* Sets the name of the rhythm pattern file.
	 */
	void setFileName(const char* file);

	/* Sets the track where the pattern is stored.
	 */
	void setTrack(int tr);

	/* Sets the name of the instrument.
	 */
	void setInstrument(const char* instrument);

	/* Defines the type (or role) of the pattern rhythm.
	 */
	void setType(ECbRpType tp);

	/* Adds a musical style in which the given pattern rhythm is
	 * commonly found.
	 */
	void addStyle(const char* style);

	/* Removes all the stored styles.
	 */
	void removeAllStyles();

	/* Changes the meter.
	 */
	void setMeter(const CbMeter& mtr);

	/* Assignment operator.
	 */
	CbRpInfo& operator=(const CbRpInfo& rightSide);
};


typedef std::vector<CbRpInfo*> Vector_pRpInfo;


#endif