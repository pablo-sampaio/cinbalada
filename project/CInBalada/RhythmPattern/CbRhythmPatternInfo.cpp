

#include "CbRhythmPatternInfo.h"

const char* CbRpInfo::patternTypesStr[] = { "unknown", "roll", "fill", "basic" };


ECbRpType CbRpInfo::strToRhythmPatternType(const char* str) {
	int res = RPT_UNKNOWN;

	if (str != NULL) {
		for (int i = 3; i > 0; i --) {
			if (strcmp(str,patternTypesStr[i]) == 0) {
				res = i;
				break;
			}
		}
	}

	return ECbRpType(res);
}

const char* CbRpInfo::rhythmPatternTypeToStr(ECbRpType tp) {
	if (tp >= 1 && tp <= 3) {
		return patternTypesStr[tp];
	} else {
		return patternTypesStr[0];
	}
}

CbRpInfo::CbRpInfo(int id_, const char* name_, const char* file, const char* instr,
					ECbRpType tp, CbMeter* metr_, int trkNum) :
	id(id_),
	fileName(file),
	instrument(instr),
	type(tp),
	track(trkNum)
{
	this->name = (name_ == NULL) ? "" : name_;
	this->meter = (metr_ == NULL) ? new CbMeter(CbMeter::SIMPLE_QUADR) : metr_;  // se null: 4/4
}

CbRpInfo::CbRpInfo(const CbRpInfo& pattern) {
	(*this) = pattern;
}

CbRpInfo::~CbRpInfo() {
	delete this->meter;
}

void CbRpInfo::setId(int id) {
	this->id = id;
}

void CbRpInfo::setName(const char* name_) {
	this->name = (name_ == NULL) ? "" : name_;
}

void CbRpInfo::setFileName(const char* file) {
	this->fileName = file;
}

void CbRpInfo::setTrack(int tr) {
	this->track = tr;
}

void CbRpInfo::setInstrument(const char* instr) {
    this->instrument = instr;
}

void CbRpInfo::setType(ECbRpType tp) {
    this->type = tp;
}

void CbRpInfo::addStyle(const char* style) {
    this->styles.push_back(style);
}

std::string CbRpInfo::getName() const {
    return this->name;
}

std::string CbRpInfo::getFileName() const {
    return this->fileName;
}

int CbRpInfo::getTrack() const {
	return track;
}

std::string CbRpInfo::getInstrument() const {
    return this->instrument;
}

int CbRpInfo::getId() const {
    return this->id;
}

ECbRpType CbRpInfo::getType() const {
    return this->type;
}

const char* CbRpInfo::getTypeStr() const {
	return rhythmPatternTypeToStr(this->type);
}

int CbRpInfo::getNumStyles() const {
	return this->styles.size();
}

std::string CbRpInfo::getStyle(int index) const {
    return this->styles[index];
}

bool CbRpInfo::hasStyle(const std::string &str) const {
    bool has = false;

    for (int i = 0; i < styles.size(); i ++) {
        if (this->styles[i] == str) {
            has = true;
            break;
        }
    }

    return has;
}

void CbRpInfo::removeAllStyles() {
    this->styles.clear();
}

const CbMeter* CbRpInfo::getMeter() const {
	return this->meter;
}

void CbRpInfo::setMeter(const CbMeter& mtr) {
	delete this->meter;
	this->meter = new CbMeter(mtr);
}

CbRpInfo& CbRpInfo::operator=(const CbRpInfo& rightSide) {
    this->id = rightSide.id;
    this->name = rightSide.name;
    this->fileName = rightSide.fileName;
    this->instrument = rightSide.instrument;
    this->track = rightSide.track;
    this->type = rightSide.type;
	this->styles = rightSide.styles;
	this->meter = new CbMeter(*rightSide.meter);

    return (*this);
}
