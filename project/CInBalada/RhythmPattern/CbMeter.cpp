
#include "CbMeter.h"
#include "RhythmPattern\CbRhythmPattern.h"

#include "Util\MathUtil.h"


/* Duration constraints in milliseconds,
 * according to J. London (2000).
 */
#define MIN_TATUM_DURATION       100
#define MAX_BAR_DURATION        5000
#define MIN_PREF_BEAT_DURATION   500
#define MAX_PREF_BEAT_DURATION   900

#define TATUM_DURATION_TO_BPM(d) int( (60000.0 / (d)) / tatumsPerQNote )


//.........................//
// CLASS CbMeter
//.........................//

const char* CbMeter::tempoLabels[] = {
	"Very Slow",
	"Slow",
	"Medium-Slow",
	"Moderate",
	"Medium-Fast",
	"Fast",
	"Very Fast"
};

const CbMeter CbMeter::SIMPLE_DUPLE (2, 4);
const CbMeter CbMeter::SIMPLE_TRIPLE (3, 4);
const CbMeter CbMeter::SIMPLE_QUADR (4, 4);
const CbMeter CbMeter::COMPOUND_DUPLE (2, 3);
const CbMeter CbMeter::COMPOUND_TRIPLE (3, 3);
const CbMeter CbMeter::COMPOUND_QUADR (4, 3);


CbMeter::CbMeter(int numBeats, int beatTatums) {
	this->regular = true;
	this->beatsRp = NULL;
	this->numBeats = numBeats;
	this->beats = new CbBeat*[numBeats];

	for (int i = 0; i < numBeats; i ++) {
		this->beats[i] = new CbBeat(beatTatums);
	}

	switch (numBeats) {
	case 1:
		this->beats[0] = 0;
		break;
	case 2:
		this->beats[0]->beatMetricity = 1;
		this->beats[1]->beatMetricity = 0;
		break;
	case 3:
		this->beats[0]->beatMetricity = 2;
		this->beats[1]->beatMetricity = 0;
		this->beats[2]->beatMetricity = 1;
		break;
	case 4:
		this->beats[0]->beatMetricity = 2;
		this->beats[1]->beatMetricity = 0;
		this->beats[2]->beatMetricity = 1;
		this->beats[3]->beatMetricity = 0;
		break;
	default:
		for (int i = 1; i < numBeats; i ++) {
			this->beats[i]->beatMetricity = 0;
		}
		if ((numBeats % 2) == 0) {
			this->beats[0]->beatMetricity = 2;
			this->beats[numBeats/2]->beatMetricity = 1;
		} else {
			this->beats[0]->beatMetricity = 1;
		}
	}

	// convencoes...
	switch (beatTatums) {
	case 4:
		this->tatumsPerQNote = 4;
		break;
	case 3:
	case 2:
	default:
		this->tatumsPerQNote = 2;
		break;
	}

}

CbMeter::CbMeter(CbMeter* array, int size) {
	this->regular = false;
	this->beatsRp = NULL;
	// ...
}

CbMeter::CbMeter(int* beatTatums, int size) {
	this->regular = false;
	this->beatsRp = NULL;
	// ...
}

CbMeter::CbMeter(const CbMeter& m) {
	this->regular = m.regular;
	this->tatumsPerQNote = m.tatumsPerQNote;
	this->beatsRp = NULL;  // nao precisa copiar, ele será re-criado na
						   // chamada ao 'get', qdo necessário

	this->numBeats = m.numBeats;
	this->beats = new CbBeat*[numBeats];
	for (int i = 0; i < numBeats; i ++) {
		this->beats[i] = new CbBeat(*m.beats[i]);
	}
}

CbMeter::~CbMeter() {
	if (this->beatsRp == NULL) {
		delete this->beatsRp;
	}

	for (int i = 0; i < numBeats; i ++) {
		delete this->beats[i];
	}

	delete beats;
}

bool CbMeter::isRegular() const {
	return regular;
}

int CbMeter::getNumBeats() const {
	return this->numBeats;
}

const CbBeat* CbMeter::getBeat(int b) const {
	return this->beats[b];
}

int CbMeter::getNumTatums() const {
	int sum = 0;
	for (int i = 0; i < this->numBeats; i ++) {
		sum += this->beats[i]->numTatums;
	}
	return sum;
}

int CbMeter::getMaxTatumsInBeat() const {
	int div = 0;

	if (regular == true) {
		div = this->beats[0]->numTatums;

	} else {
		for (int i = 0; i < this->numBeats; i ++) {
			if (div < this->beats[i]->numTatums) {
				div = this->beats[i]->numTatums;
			}
		}
	}

	return div;
}

int CbMeter::getMinTatumsInBeat() const {
	int div = 0;

	if (regular == true) {
		div = this->beats[0]->numTatums;

	} else {
		for (int i = 0; i < this->numBeats; i ++) {
			if (div > this->beats[i]->numTatums) {
				div = this->beats[i]->numTatums;
			}
		}
	}

	return div;
}

int CbMeter::getTatumsInBeat(int b) const {
	if (b >= 0 && b < this->numBeats) {
		return this->beats[b]->numTatums;
	} else {
		return -1;
	}
}

int CbMeter::getBeatOfTatum(int tatum) const {
	int nbeat;

	if (tatum < 0 || tatum >= getNumTatums()) {
		return -1;
	}

	if (this->regular == true) {
		nbeat = tatum / getTatumsInBeat(0);
	} else {
		// ...
	}

	return nbeat;
}

int CbMeter::meterTatumToBeatTatum(int _beat, int meterTatum) const {

	if (meterTatum < 0 || meterTatum >= getNumTatums()) {
		return -1;
	}

	int nbeat;
	int beatTatum = -1;

	if (this->regular == true) {
		int nbeat = meterTatum / getTatumsInBeat(0);

		if (_beat == nbeat) {
			beatTatum = meterTatum % getTatumsInBeat(0);
		}

	} else {
		// ...
	}

	return beatTatum;
}

int CbMeter::beatTatumToMeterTatum(int b, int beatTatum) const {

	if (b < 0 || b >= this->numBeats
			|| beatTatum < 0 || beatTatum >= this->beats[b]->numTatums) {
		return -1;
	}

	int meterTatum = -1;

	if (this->regular == true) {
		meterTatum = b * this->getTatumsInBeat(0);
		meterTatum += beatTatum;
	} else {
		// ...
	}

	return meterTatum;
}

int CbMeter::getMetricity(int tatum) const {
	int metr = -1;

	if (tatum < 0 || tatum >= getNumTatums()) {
		return -1;
	}

	if (regular == true) {
		int nbeat = tatum / getMaxTatumsInBeat();
		int pos = tatum % getMaxTatumsInBeat();
		metr = this->beats[nbeat]->getMetricity(pos);
	} else {
		// ...
	}

	return metr;
}

const int CbMeter::getTatumsPerQuarterNote() const {
	return this->tatumsPerQNote;
}

const double CbMeter::getNumQuarterNotes() const {
	return double(this->getNumTatums()) / double(this->tatumsPerQNote);
}

const CbRhythmPattern* CbMeter::getBeatsRhythmPattern() const {
	if (this->beatsRp == NULL) {
		int resolution = 2 * tatumsPerQNote;
		FmMidiFileTrack* track = new FmMidiFileTrack(resolution);

		int tick = 0;
		for (int i = 0; i < this->numBeats; i ++) {
			track->addEvent( FmMidiEvent::createNoteOn(75, 80 + (10 * this->beats[i]->beatMetricity), tick, 9) );
			track->addEvent( FmMidiEvent::createNoteOff(75, 80, tick + 1, 9) );
			tick += 2 * this->beats[i]->numTatums;
		}

		// acoxambre ok
		const_cast<CbMeter*>(this)->beatsRp  = new CbRhythmPattern(*track, *this);
		delete track;
	}

	return this->beatsRp;
}

int CbMeter::getMinTempo() const {
	double maxTatumDuration = MAX_BAR_DURATION / this->getNumTatums();
	return TATUM_DURATION_TO_BPM( maxTatumDuration );
}

int CbMeter::getMaxTempo() const {
	return TATUM_DURATION_TO_BPM( MIN_TATUM_DURATION );
}

int CbMeter::getMinPreferedTempo() const {
	double maxPrefTatumDur = MAX_PREF_BEAT_DURATION / this->getMaxTatumsInBeat();
	int minPrefTempo = TATUM_DURATION_TO_BPM( maxPrefTatumDur );
	return MAX( minPrefTempo, getMinTempo());
}

int CbMeter::getMaxPreferedTempo() const {
	double minPrefTatumDur = MIN_PREF_BEAT_DURATION / this->getMinTatumsInBeat();
	int maxPrefTempo = TATUM_DURATION_TO_BPM( minPrefTatumDur );
	return MIN( maxPrefTempo, getMaxTempo());
}

const char* CbMeter::tempoLabelToStr(CbETempoLabel label) {
	if (label >= TL_VERY_SLOW && label <= TL_VERY_FAST) {
		return CbMeter::tempoLabels[label];
	} else {
		return "invalid";
	}
}

CbETempoLabel CbMeter::strToTempoLabel(const char* str) {
	int res = -1;
	for (int t = TL_VERY_SLOW; t <= TL_VERY_FAST; t ++) {
		if (strcmp(CbMeter::tempoLabels[t],str) == 0) {
			res = t;
			break;
		}
	}
	return CbETempoLabel(res);
}

CbETempoLabel CbMeter::getTempoLabel(int tempo) const {
	CbETempoLabel label;

	if (tempo < getMinTempo() || tempo > getMaxTempo()) {
		label = TL_INVALID;

	} else if (tempo < getMinPreferedTempo()) {
		label = TL_VERY_SLOW;

	} else if (tempo > getMaxPreferedTempo()) {
		label = TL_VERY_FAST;

	} else {
		double labelSlice = double(getMaxPreferedTempo() - getMinPreferedTempo()) / 5.0;

		int i = 0;
		double labelMin = getMinPreferedTempo();
		while (tempo >= labelMin) {
			i ++;
			labelMin += labelSlice;
		}

		label = CbETempoLabel(TL_SLOW + (i-1));
	}

	return label;
}

int CbMeter::getTypicalTempo(CbETempoLabel label) const {
	int tempo = -1;

	if (label <= TL_VERY_SLOW) {
		tempo = (getMinTempo() + getMinPreferedTempo()) / 2;

	} else if (label >= TL_VERY_FAST) {
		tempo = (getMaxTempo() + getMaxPreferedTempo()) / 2;

	} else {
		double labelSlice = double(getMaxPreferedTempo() - getMinPreferedTempo()) / 5.0;
		double numSlices = (label - TL_SLOW);
		tempo = getMinPreferedTempo() + int((numSlices * labelSlice) + (labelSlice/2));

	}

	return tempo;
}

bool CbMeter::isCompatibleWith(const CbMeter* meter) const {
	int quant_a = this->getNumTatums();
	int quant_b = meter->getNumTatums();
	return ((quant_b == quant_a) || (quant_b % quant_a) == 0);
}


//.........................//
// CLASS CbBeat
//.........................//

CbBeat::CbBeat(int subdiv) {
	this->numTatums = subdiv;
	this->beatMetricity = 1;
	this->metricity = new int[subdiv];

	switch (subdiv) {
	case 2:
		this->metricity[0] = 2;
		this->metricity[1] = 1;
		break;
	case 3:
		this->metricity[0] = 3;
		this->metricity[1] = 1;
		this->metricity[2] = 2;
		break;
	case 4:
		this->metricity[0] = 3;
		this->metricity[1] = 1;
		this->metricity[2] = 2;
		this->metricity[3] = 1;
		break;
	default:
		for (int i = 1; i < numTatums; i ++)  this->metricity[i] = 1;
		if ((numTatums % 2) == 0) {
			this->metricity[0] = 3;
			this->metricity[numTatums / 2] = 2;
		} else {
			this->metricity[0] = 2;
		}
	}

}

CbBeat::CbBeat(const CbBeat &b) {
	this->numTatums = b.numTatums;
	this->beatMetricity = b.beatMetricity;
	this->metricity = new int[numTatums];

	for (int i = 0; i < numTatums; i ++) {
		this->metricity[i] = b.metricity[i];
	}

}

CbBeat::~CbBeat() {
	delete metricity;
}

int CbBeat::getMetricity(int tatum) const {
	int downBeatInc = ((tatum == 0) ? beatMetricity : 0);
	return this->metricity[tatum] + downBeatInc;
}

int CbBeat::getNumTatums() const {
	return this->numTatums;
}

int CbBeat::getBeatMetricity() const {
	return this->beatMetricity;
}

