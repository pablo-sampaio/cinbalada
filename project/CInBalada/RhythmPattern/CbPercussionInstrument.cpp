

#include "CbPercussionInstrument.h"

const char* CbPercInstrument::frequencyClassesStr[] = {"none", "low-band", "med-band", "high-band"};
const char* CbPercInstrument::percNotePitchesStr[] = {"none", "low", "high"};


const char* CbPercInstrument::frequencyClassToStr(ECbInstrFreqClass fc) {
	int ifc = int(fc);
	if (ifc >= 0 && ifc < 3) {
    	return frequencyClassesStr[ifc + 1];
    } else {
    	return frequencyClassesStr[0];
    }
}

ECbInstrFreqClass CbPercInstrument::strToFrequencyClass(const char* str) {
    int res = IFC_UNKNOWN;

    if (str != NULL) {
        for (int i = 1; i <= 3; i ++) {
            if (strcmp(str,frequencyClassesStr[i]) == 0) {
                res = i - 1;
                break;
			}
        }
    }

    return ECbInstrFreqClass(res);
}

const char* CbPercInstrument::notePitchToStr(ECbPercNotePitch np) {
	int inp = int(np);
    if (inp >= 0 && inp < 2) {
    	return percNotePitchesStr[inp + 1];
    } else {
    	return percNotePitchesStr[0];
    }
}

ECbPercNotePitch CbPercInstrument::strToNotePitch(const char* str) {
    int res = NP_NONE;

    if (str != NULL) {
        for (int i = 1; i <= 2; i ++) {
            if (strcmp(str,percNotePitchesStr[i]) == 0) {
                res = i - 1;
                break;
            }
        }
    }

    return ECbPercNotePitch(res);
}


CbPercInstrument::CbPercInstrument(const char* __name, ECbInstrFreqClass fq) {
	this->name = __name;
    this->freqClass = fq;
}

CbPercInstrument::CbPercInstrument(const std::string &__name, ECbInstrFreqClass fq) {
	this->name = __name;
    this->freqClass = fq;
}

std::string CbPercInstrument::getName() const {
	return this->name;
}

ECbInstrFreqClass CbPercInstrument::getFrequencyClass() const {
	return this->freqClass;
}

void CbPercInstrument::setFrequencyClass(ECbInstrFreqClass fc) {
	this->freqClass = fc;
}

int CbPercInstrument::getNumNotes() const {
	return notes.size();
}

CbPercNote CbPercInstrument::getNote(int i) const {
	return notes[i];
}

void CbPercInstrument::addNote(const CbPercNote &nt) {
	notes.push_back(nt);
}

void CbPercInstrument::removeNote(int midiNote) {
	for (int i = 0; i < this->notes.size(); i ++) {
    	if (notes[i].midiNote == midiNote) {
        	notes.erase(notes.begin() + i);
            break;
        }
    }
}

