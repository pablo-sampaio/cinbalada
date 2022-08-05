
#ifndef __XML_RHYTHMPATTERNINFO_REP_H__
#define __XML_RHYTHMPATTERNINFO_REP_H__

#include "CbRhythmPatternInfoRepository.h"

#include <map>
#include "TinyXml\tinyxml.h"

/* XML node "RhythmPattern" */
#define XML_NODE_RHYTHMPATTERN         "RhythmPattern"
#define XML_ATTR_RHYTHMPATTERN_NAME    "name"
#define XML_ATTR_RHYTHMPATTERN_TRACK   "track"
#define XML_ATTR_RHYTHMPATTERN_TYPE    "type"
#define XML_ATTR_RHYTHMPATTERN_INSTR   "instrument"
#define XML_ATTR_RHYTHMPATTERN_FILE    "filepath"

/* XML node "MusicalStyles" */
#define XML_NODE_MUSICALSTYLES     "MusicalStyles"
#define XML_NODE_MUSICALSTYLE      "MusicalStyle"
#define XML_ATTR_MUSICALSTYLE_NAME "name"

/* XML node "Meter" */
#define XML_NODE_METER             "Meter"
#define XML_ATTR_METER_BEATS       "beats"
#define XML_ATTR_METER_DEF_BTATUMS "def_beat_tatums"
#define XML_NODE_BEAT              "Beat"
#define XML_ATTR_BEAT_TATUMS       "tatums"

/* XML node "Instrument" */
#define XML_NODE_INSTRUMENT            "PercussionInstrument"
#define XML_ATTR_INSTRUMENT_NAME       "name"
#define XML_ATTR_INSTRUMENT_FREQ_CLASS "fclass"

/* XML node "Notes" */
#define XML_NODE_NOTES          "Notes"
#define XML_NODE_NOTE           "Note"
#define XML_ATTR_NOTE_MIDI      "midinote"
#define XML_ATTR_NOTE_REL_PITCH "relpitch"


/**
 * This class is a concrete implementation of a repository of RpInfo
 * objects. The objects are stored as XML files. The file is open when the
 * repository is created and it is saved (with updated information) when the
 * object is destroyed (i.e, when the destructor is called).
 */
class CbXmlRpInfoRepository : public ICbRpInfoRepository {
private:
	typedef std::map<std::string,CbPercInstrument*> MapInstruments;

	TiXmlDocument document;

	std::vector<CbRpInfo*> rhythms;  // guarda os padroes ritmicos
    int  nextId;  // proximo 'id' a ser usado em novos padroes

    MapInstruments instruments; // instrumentos

    bool changed; // indica se o repositorio foi alterado

	CbRpInfo* loadRhythmPattern(TiXmlElement* node);
	TiXmlElement* loadXmlNode(CbRpInfo* rhythm);
	CbMeter* loadMeter(TiXmlElement* node);
	TiXmlElement* loadXmlNode(const CbMeter* meter);

    CbPercInstrument* loadInstrument(TiXmlElement* node);
    TiXmlElement* loadXmlNode(CbPercInstrument* rhythm);

    void saveXmlFile();

public:
	CbXmlRpInfoRepository(const char* filename);
	virtual ~CbXmlRpInfoRepository();

	int getNumRpInfos() const;
    void getAllRpInfos(Vector_pRpInfo&) const;
    void getRpInfosByInstrument(const std::string &instrument, Vector_pRpInfo& result) const;
    int getNumPatternsOfInstrument(std::string& str) const;

    const CbRpInfo* getRpInfoById(int id) const;

    void addRpInfo(const CbRpInfo& info);
	void changeRpInfo(int id, const CbRpInfo& info);
	void deleteRpInfo(int id);

    void getAllInstrumentNames(std::vector<std::string>&) const;
    void getAllInstruments(Vector_pPercInstrument& result) const;
    int  getNumInstruments() const;

    const CbPercInstrument* getInstrument(std::string instrName) const;

	void addInstrument(const CbPercInstrument &instr);
    void changeInstrument(const CbPercInstrument &instr);
    void deleteInstrument(std::string name);

};

#endif