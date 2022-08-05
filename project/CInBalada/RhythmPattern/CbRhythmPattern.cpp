
#include "CbRhythmPattern.h"

#include "Util\MathUtil.h"
#include <fstream>

using namespace std;

static ofstream logFile("logs\\CbRhythmPattern.txt", ios::out);


//.................................................//
// CLASSE CbRhythmPattern
//.................................................//

const char* CbRhythmPattern::criteriaStr[] = {
	"invalid",
	"Geber Function",
	"Chronotonic Distance",
	"Hamming Distance",
	"Derivation",
	"Off-beatness",
	"Oddity",
	"Cognitive Complexity",
	"PS-Complexity",
	"Metricity",
	"Syncopation",
	"Circle-Chord Evenness",
	"Linear-Regression Evenness",
	"Irregularity",
	"Activity",
	"Average Intensity"
};

const char* CbRhythmPattern::evaluationCriteriaToStr(ECbRpEvalCriteria c) {
	int ifc = int(c);
	if (ifc >= 0 && ifc < 15) {
		return criteriaStr[ifc + 1];
	} else {
		return criteriaStr[0];
	}
}

ECbRpEvalCriteria CbRhythmPattern::strToEvaluationCriteria(const char* str) {
	int res = AVC_INVALID;

	if (str != NULL) {
		for (int i = 1; i <= 15; i ++) {
			if (strcmp(str,criteriaStr[i]) == 0) {
				res = i - 1;
				break;
			}
		}
	}

	return ECbRpEvalCriteria(res);
}

bool CbRhythmPattern::isComparation(ECbRpEvalCriteria c) {
	return c == AVC_GEBER || c == AVC_CHRONOT_DIST
			|| c == AVC_HAMMING_DIST || c == AVC_DERIVATION;
}

// construtor
CbRhythmPattern::CbRhythmPattern(const CbMeter &meter) {
	this->patternInfo = new CbRpInfo(-1, "mute-pattern", "<runtime>", "", RPT_BASIC, new CbMeter(meter));
	this->pattern = new FmMidiFileTrack;
	this->meter = NULL;   // ja esta no patternInfo...

	this->tubs = new CbTubs(*this, meter);
	this->onsets = new CbOnsetsVector(this->tubs);
	this->interonsets = new CbInterOnsetVector(*onsets);
	this->chronotonic = new CbChronotonicVector(*interonsets, this->tubs->tubsSize);
	//this->onsetsDistFreq = new CbFullIntervalHistogram(*onsets);  // NAO USADO !!! (BUG JA FOI RESOLVIDO)
}

// construtor
CbRhythmPattern::CbRhythmPattern(const CbRpInfo& rpi, const char* midiDir) {
	std::string fullPath (midiDir);

	this->patternInfo = new CbRpInfo(rpi);
	this->meter = NULL;  // ja esta no patternInfo...

	fullPath = fullPath + rpi.getFileName();

	FmMidiFile* midiFile = new FmMidiFile(fullPath.c_str());
	logFile << "Antes: " << *(midiFile->getTrack(rpi.getTrack())) << endl;

	this->pattern = filterMidiEvents( *(midiFile->getTrack(rpi.getTrack())) );
	logFile << "Depois: " << *(this->pattern) << endl;

	delete midiFile;

	this->tubs = new CbTubs(*this, *rpi.getMeter());
	this->onsets = new CbOnsetsVector(this->tubs);
	this->interonsets = new CbInterOnsetVector(*onsets);
	this->chronotonic = new CbChronotonicVector(*interonsets, this->tubs->tubsSize);
	//this->onsetsDistFreq = new CbFullIntervalHistogram(*onsets);  // NAO USADO !!! (BUG JA FOI RESOLVIDO)

	logFile << *this << endl;
}

CbRhythmPattern::CbRhythmPattern(const FmMidiFileTrack& track, const CbMeter& meter) {
	this->patternInfo = NULL;
	this->meter = new CbMeter(meter);

	this->pattern = filterMidiEvents(track);

	this->tubs = new CbTubs(*this, meter);
	this->onsets = new CbOnsetsVector(tubs);
	this->interonsets = new CbInterOnsetVector(*onsets);
	this->chronotonic = new CbChronotonicVector(*interonsets, this->tubs->tubsSize);
	//this->onsetsDistFreq = new CbFullIntervalHistogram(*onsets);

	logFile << *this << endl;
}

CbRhythmPattern::CbRhythmPattern(const CbRhythmPattern& ptrn) {
	if (ptrn.patternInfo != NULL) {
		this->patternInfo = new CbRpInfo(*ptrn.patternInfo);
		this->meter = NULL;
	} else {
		this->patternInfo = NULL;
		this->meter = new CbMeter(*ptrn.meter);
	}

	this->pattern = filterMidiEvents(*ptrn.pattern);   // nao tem copy-constructor...

	this->tubs = new CbTubs(*ptrn.tubs);
	this->onsets = new CbOnsetsVector(tubs);
	this->interonsets = new CbInterOnsetVector(*onsets);
	this->chronotonic = new CbChronotonicVector(*interonsets, this->tubs->tubsSize);
	//this->onsetsDistFreq = new CbFullIntervalHistogram(*onsets);

}

FmMidiFileTrack* CbRhythmPattern::filterMidiEvents(const FmMidiFileTrack& track) {
	FmMidiFileTrack *result = new FmMidiFileTrack(track.getResolution());
	Vector_pMidiEvent evts;
	FmMidiChannelEvent *ev, *aux;

	int ticksPerTatum = track.getResolution() / getMeter()->getTatumsPerQuarterNote();

	// distancia menor do que meio tatum !
	int ntOffDist = MAX( 4*ticksPerTatum/10, 1 );

	track.getAllEvents(evts);

	for (int i = 0; i < evts.size(); i ++) {
		if (evts[i]->isChannelEvent()) {
			ev = static_cast<FmMidiChannelEvent*>(evts[i]);

			switch (ev->getType()) {
			case MD_NOTE_ON:
				result->addEvent(ev->clone());
				// adiciona note-off de modo que a duracao seja sempre menor que um tatum
				result->addEvent( FmMidiEvent::createNoteOff(ev->getPitch(), ev->getVelocity(),
									ev->getTick() + ntOffDist, ev->getChannel()) );
				break;
			default:
				break;
			}
		}
	}

	return result;
}

CbRhythmPattern::~CbRhythmPattern() {
	//ofstream logFile("\\CbRhythmPattern-Finalization.txt", ios::out | ios::app);

	if (this->patternInfo != NULL) {
		//logFile << "Apagando padrao \"" << patternInfo->getName() << "\"... ";
		delete this->patternInfo;
	} else {
		//logFile << "Apagando padrao " << *tubs << "... ";
	}

	if (this->meter != NULL) {
		delete this->meter;
	}

	delete this->pattern;

//	delete onsetsDistFreq;
	delete this->chronotonic;
	delete this->interonsets;
	delete this->onsets;
	delete this->tubs;

   	//logFile << "OK !" << endl;
}

const CbRpInfo* CbRhythmPattern::getInfo() const {
	return patternInfo;
}

const CbMeter* CbRhythmPattern::getMeter() const {
	if (this->patternInfo != NULL) {
		return this->patternInfo->getMeter();
	} else {
		return meter;
	}
}

const FmMidiFileTrack* CbRhythmPattern::getPattern() const {
	return pattern;
}

int CbRhythmPattern::getTatumsPerBar() const {
	return getMeter()->getNumTatums();
}

int CbRhythmPattern::getNumTatums() const {
	return tubs->getSize();
}

int CbRhythmPattern::getNumBars() const {
	return tubs->getSize() / tubs->getTatumsPerBar();
}

bool CbRhythmPattern::areMetricallyCompatible(const CbRhythmPattern* pattern) const {
	int quant_a = this->tubs->getTatumsPerBar();
	int quant_b = pattern->tubs->getTatumsPerBar();
	return ((quant_a % quant_b) == 0) || ((quant_b % quant_a) == 0);
}

CbRhythmPattern* CbRhythmPattern::getBar(int b) const {

	if (b < 0 || b >= getNumBars()) {
		return NULL;
	}

	FmMidiFileTrack *btrack = new FmMidiFileTrack(this->pattern->getResolution());
	Vector_pMidiEvent evts;
	FmMidiEvent* aux;

	int ticksPerTatum = this->pattern->getResolution() / this->getMeter()->getTatumsPerQuarterNote();
	int ticksPerBar = ticksPerTatum * this->getTatumsPerBar();
	int start_tick = b * ticksPerBar;
	int end_tick = start_tick + ticksPerBar - 1;
	int tick;

	this->pattern->getAllEvents(evts);
	for (int i = 0; i < evts.size(); i ++) {
		tick = evts[i]->getTick();
		if (tick >= start_tick && tick <= end_tick) {
			aux = evts[i]->clone();
			aux->setTick(tick - start_tick);
			btrack->addEvent(aux);
		}
	}

	CbRhythmPattern *bar = new CbRhythmPattern(*btrack, *getMeter());
	delete btrack;

	return bar;
}

bool CbRhythmPattern::operator==(const CbRhythmPattern& rp) const {
	bool result = false;

	if (this->tubs->tubsSize == rp.tubs->tubsSize) {
		int i = 0;

		result = true;
		while ((i < this->tubs->tubsSize) && (result == true)) {
			result = (this->tubs->get(i) == rp.tubs->get(i));
			i ++;
		}
	}

	return result;
}

ostream& operator<<(ostream& o, const CbRhythmPattern& rp) {
	o << "Rhythm Pattern";
	if (rp.patternInfo != NULL) {
		o << " \"" << rp.patternInfo->getName() << "\"";
	}
	o << endl;
	o << " - TUBS: " << *(rp.tubs) << endl;
	o << " - ONST: " << *(rp.onsets) << endl;
	o << " - IOST: " << *(rp.interonsets) << endl;
	o << " - CHRN: " << *(rp.chronotonic) << endl;
	//o << " - FLIH: " << *(rp.onsetsDistFreq) << endl;

	return o;
}

std::string& operator<< (std::string& out, const CbRhythmPattern& rp) {
	out += "TUBS: ";
	out << *(rp.tubs);
	return out;
}

const CbTubs* CbRhythmPattern::getTubs() const {
	return this->tubs;
}

const CbOnsetsVector* CbRhythmPattern::getOnsetsVector() const {
	return this->onsets;
}

const CbInterOnsetVector* CbRhythmPattern::getInterOnsetVector() const {
	return this->interonsets;
}

const CbChronotonicVector* CbRhythmPattern::getChronotonicVector() const {
	return this->chronotonic;
}

const CbFullIntervalHistogram* CbRhythmPattern::getFullIntervalHistogram() const {
	return this->onsetsDistFreq;
}

double CbRhythmPattern::evaluationFunction(ECbRpEvalCriteria crit, const CbRhythmPattern *phrase) const {
	double res = -1;

	switch (crit) {
	case AVC_GEBER:
		res = this->geberFunction(phrase);
		break;
	case AVC_CHRONOT_DIST:
		res = this->chronotonicDistance(phrase);
		break;
	case AVC_HAMMING_DIST:
		res = this->hammingDistance(phrase);
		break;
	case AVC_DERIVATION:
		res = this->derivationMeasure(phrase);
		break;
	case AVC_OFFBEATNESS:
		res = this->offbeatness();
		break;
	case AVC_ODDITY:
		res = this->oddity();
		break;
	case AVC_COGNIT_COMPLEXITY:
		res = this->cognitiveComplexity();
		break;
	case AVC_PS_COMPLEXITY:
		res = this->psComplexity();
		break;
	case AVC_METRICITY:
		res = metricity();
		break;
	case AVC_SYNCOPATION:
		res = wnbdSyncopation();
		break;
	case AVC_CIRC_CHORD_EVENNESS:
		res = this->circleChordsEvenness();
		break;
	case AVC_LIN_REG_EVENNESS:
		res = this->linearRegEvenness();
		break;
	case AVC_IRREGULARITY:
		res = this->irregularity();
		break;
	case AVC_ACTIVITY:
		res = this->activity();
		break;
	case AVC_AVERAGE_INTENSITY:
		res = averageIntensity();
		break;
	}

	return res;
}

double CbRhythmPattern::geberFunction(const CbRhythmPattern* phrase) const {

	if (this->areMetricallyCompatible(phrase) == false) {
		return -1;
	}

	int iguais = 0;
	int diferentes = 0;
	int maxSize = MAX(this->tubs->getSize(), phrase->tubs->getSize());

	bool atk1, atk2;

	for (int i = 0; i < maxSize; i ++) {
		atk1 = this->tubs->hasOnset(i % this->tubs->getSize());
		atk2 = phrase->tubs->hasOnset(i % phrase->tubs->getSize());

		// se aconteceu algum ataque
		if (atk1 || atk2) {
			if (atk1 && atk2) {
				iguais ++;
			} else {
				diferentes ++;
			}
		}

	}

	return double(ABS(iguais - diferentes)); // / double(maxSize);
}

double CbRhythmPattern::chronotonicDistance(const CbRhythmPattern *phrase) const {

	if (this->areMetricallyCompatible(phrase) == false) {
		return -1;
	}

	int areaDiff = -1;
	int maxSize = MAX(this->chronotonic->getSize(), phrase->chronotonic->getSize());

	//logFile << "Chronotonic Distance:" << endl;
	//logFile << " - tamanhos: " << this->chronotonic->getSize() << " e " << phrase->chronotonic->getSize() << endl;

	areaDiff = 0;
	for (int i = 0; i < maxSize; i ++) {
		int chronV1 = this->chronotonic->get(i % this->chronotonic->getSize());
		int chronV2 = phrase->chronotonic->get(i % phrase->chronotonic->getSize());
		areaDiff += ABS(chronV1 - chronV2);
	}

	//logFile << ">> Resultado (nao-normalizado): " << areaDiff;
	//logFile << ">> Resultado (normalizado)    : " << areaDiff / maxSize;

	return double(areaDiff) / double(maxSize);
}

double CbRhythmPattern::hammingDistance(const CbRhythmPattern *phrase) const {

	if (this->areMetricallyCompatible(phrase) == false) {
		return -1;
	}

	int maxSize = MAX(this->tubs->getSize(), phrase->tubs->getSize());
	int distance = 0;

	for (int i = 0; i < maxSize; i++) {
		if (this->tubs->hasOnset(i % this->tubs->getSize())
				!= phrase->tubs->hasOnset(i % phrase->tubs->getSize())) {
			distance ++;
		}
	}

	return double(distance) / double(maxSize);
}

double CbRhythmPattern::offbeatness() const {
	double cnt = 0;

	// off-beatness nao se define para numeros
	// impar de pulsos atomicos
	if ((tubs->getTatumsPerBar() % 2) != 0) {
		return -1;
	}

	// a posicao zero nunca eh off-beat
	for (int i = 1; i < this->onsets->getSize(); i ++) {
		// se sao primos entre si (a posicao do ataque e a quantizacao)
		if (gcd(this->onsets->get(i), this->tubs->getTatumsPerBar()) == 1) {
			cnt ++;
		}
	}

	return cnt / double(this->getNumBars());
}

double CbRhythmPattern::oddity() const {

	if ((this->tubs->getTatumsPerBar() % 2) != 0) {
		return -1;
	}

	int halfBar = this->tubs->getTatumsPerBar() / 2;
	double cntPairs = 0;
	int aux;

	for (int b = 0; b < getNumBars(); b ++) {
		aux = b * tubs->getTatumsPerBar();
		for (int i = aux; i < (aux + halfBar); i ++) {
			if (tubs->hasOnset(i) && tubs->hasOnset((i + halfBar) % tubs->getSize())) {
				cntPairs ++;
			}
		}
	}

	return cntPairs / double(this->getNumBars());
}

double CbRhythmPattern::metricity() const {
	const CbMeter* meter = getMeter();

	if (meter->isRegular() == false) {
		return -1;
	}

	double metr = 0;
	int numTatums = meter->getNumTatums();
	for (int i = 0; i < onsets->getSize(); i ++) {
		metr += meter->getMetricity(onsets->get(i) % numTatums);
	}

	return metr / onsets->getSize(); //double(this->getNumBars());
}

double CbRhythmPattern::cognitiveComplexity() const {
	const CbMeter* meter = this->getMeter();

	// so tem o calculo dessa medida para metricas com
	// batidas q subdividem em 4 tatums !!!
	if (meter->isRegular() == false ||  meter->getTatumsInBeat(0) != 4) {
		return -1;
	}

	double compl = 0;
	double inc;

	//logFile << "Congitive Complexity:" << endl;
	for (int i = 0; i < tubs->getSize(); i += 4) {
		inc = 0;
		if (tubs->hasOnset(i)) {
			if (tubs->hasOnset(i + 1)) {
				inc = 2.5;
			} else if (tubs->hasOnset(i + 2)) {
				inc = 1;
			} else if (tubs->hasOnset(i + 3)) {
				inc = 4.5;
			}
		} else if (tubs->hasOnset(i + 1)) {
			if (tubs->hasOnset(i + 2)) {
				inc = 6.5;
			} else if (tubs->hasOnset(i + 3)) {
				inc = 10;
			} else {
				inc = 7.5;
			}
		} else if (tubs->hasOnset(i + 2)) {
			if (tubs->hasOnset(i + 3)) {
				inc = 5.5;
			} else {
				inc = 5;
			}
		} else if (tubs->hasOnset(i + 3)) {
			inc = 7.5;
		}
		//logFile << " >> inc = " << inc << endl;
		compl += inc;
	}

	return compl / double(this->getNumBars());
}

double CbRhythmPattern::psComplexity() const {
	const CbMeter* meter = getMeter();
	int numTatums = meter->getNumTatums();
	int i, j, k;

	// cScore computation
	double cScore = 0;
	int beatPos;
	int s = 0;

	for (i = 0; i < meter->getNumBeats(); i ++) {
		beatPos = meter->beatTatumToMeterTatum(i, 0);

		for (j = beatPos; j < tubs->getSize(); j += numTatums) {
			if (! tubs->hasOnset(j)) {
				s ++;
			}
			// se nao for acentuado: u ++
		}
	}

	cScore = 1.169 * s;  // parametro 'u' foi ignorado !

	// dScore computation
	double dScore = 0;
	const CbBeat* beat;

	for (i = 0; i < tubs->getNumBars(); i ++) {
		for (j = 0; j < meter->getNumBeats(); j ++) {
			beat = meter->getBeat(j);
			beatPos = i*numTatums + meter->beatTatumToMeterTatum(j, 0);

			for (k = 0; k < beat->getNumTatums(); k ++) {
				// classificar intervalo...
			}

			// se vazio...  (d1 = 0.023)
			// se inicia com silencio... (d4 = 0.073)
			// se divide igualmente... (d2 = 1.272)
			// se divide desigual... (d3 = 1.295)

			// se mudou o tipo... (d5 = 0.793)
		}
	}

	return  (0.222 * cScore) + (0.778 * dScore); // normalizar por compassos ? testar calculando compasso a compasso ?
}

double CbRhythmPattern::wnbdSyncopation() const {
	const CbMeter* meter = getMeter();
	int noteStart, noteEnd, beatNtStart, beatNtEnd;
	int beatNtStart_p1; //, beatNtStart_p2;
	int distBeat, distNextBeat;
	bool ntEndIsDownBeat;

	int numOnsets = onsets->getSize();
	int numTatums = meter->getNumTatums();

	double tx;
	double sumDist = 0;

	for (int i = 0; i < numOnsets; i ++) {
		noteStart = onsets->get(i);
		if (i < (numOnsets-1)) {
			noteEnd = onsets->get(i+1);
		} else {
			noteEnd = tubs->getSize() + onsets->get(0);
		}

		noteStart = noteStart % numTatums;
		noteEnd = noteEnd % numTatums;

		beatNtStart = meter->getBeatOfTatum(noteStart);  // beat do inicio da nota
		beatNtEnd = meter->getBeatOfTatum(noteEnd);      // beat do fim da nota

		ntEndIsDownBeat = (meter->meterTatumToBeatTatum(beatNtEnd, noteEnd) == 0);
		distBeat = meter->meterTatumToBeatTatum(beatNtStart, noteStart); // dist ao beat da nota

		// (a)
		// se 'startNote' nao eh a batida
		if (distBeat != 0) {

			// auxiliares
			beatNtStart_p1 = (beatNtStart + 1) % meter->getNumBeats(); // um beat depois do inicio
			//beatNtStart_p2 = (beatNtStart + 2) % meter->getNumBeats(); // dois beats depois do inicio
			distNextBeat = meter->getTatumsInBeat(beatNtStart) - distBeat; // dist ao proximo beat

			int aux = MIN (distBeat, distNextBeat);
			int closestBeat = (aux == distBeat ? beatNtStart : beatNtStart_p1);

			// distancia da nota à batida mais proxima (proporcional ao tamanho da batida)
			tx = double(aux) / double (meter->getTatumsInBeat(closestBeat));

			// (b)
			// se 'endNote' acontece no beat de 'startNote'
			// ou se acontece precisamente no beat seguinte
			if ( (beatNtEnd == beatNtStart)
					|| (beatNtEnd == beatNtStart_p1 && ntEndIsDownBeat) )
			{
				sumDist += 1.0 / tx;
			}
			// (c')
			// se 'endNote' acontece no beat seguinte de 'startNote' (mas nao no inicio)
			else if (beatNtEnd == beatNtStart_p1)
					//|| (beatNtEnd == beatNtStart_p2 && ntEndIsDownBeat) ) // ou se acontece dois beats depois
			{
				sumDist += 2.0 / tx;
			}
			// (d')
			// se acontece no segundo beat ou depois
			else
			{
				sumDist += 1.0 / tx;
			}
		}
	}

	return sumDist / numOnsets;
}

double CbRhythmPattern::linearRegEvenness() const {
	double averageError = -1.0;
	double numNotes = onsets->getSize();

	if (numNotes > 0) {
		double idealDist = double(tubs->getSize()) / numNotes;  // distancia "ideal" entre notas
		double idealPos = 0;
		double error = 0;

		for (int i = 0; i < onsets->getSize(); i ++) {
			idealPos = i * idealDist;
			error += ABS(double(onsets->get(i)) - idealPos);  // ABS(observado - esperado)
		}

		averageError = error / numNotes;
	}

	return averageError;
}

double CbRhythmPattern::circleChordsEvenness() const {
	struct st_point { double x, y; };

	int numOnsets = onsets->getSize();
	st_point* points = new st_point[numOnsets];

	double angInc = (2.0 * M_PI) / tubs->getSize();
	int i, j, atkPos;

	for (i = 0; i < numOnsets; i ++) {
		atkPos = onsets->get(i);
		points[i].y = sin( angInc * atkPos );
		points[i].x = cos( angInc * atkPos );
	}

	double aux;
	double distSum = 0;

	for (i = 0; i < numOnsets; i ++) {
		for (j = i + 1; j < numOnsets; j ++) {
			aux  = (points[i].x - points[j].x) * (points[i].x - points[j].x);
			aux += (points[i].y - points[j].y) * (points[i].y - points[j].y);
			distSum += sqrt(aux);
		}
	}
	delete points;

	return distSum;
}

double CbRhythmPattern::irregularity() const {
	double avg = 0;
	double stdDev = 0;
	int size = interonsets->getSize() - 1; 	// posicao zero eh ignorada !

	for (int i = 1 ; i <= size; i ++) {
		avg += interonsets->get(i);
	}
	avg = avg / size;

	double aux;
	for (int i = 1 ; i <= size; i ++) {
		aux = (interonsets->get(i) - avg);
		stdDev += (aux * aux);
	}
	stdDev = stdDev / size;   // biased std-dev

	return sqrt(stdDev);
}

double CbRhythmPattern::activity() const {
	return double(onsets->getSize()) / double(this->getNumBars());
}

double CbRhythmPattern::derivationMeasure(const CbRhythmPattern *super) const {
	if (this->areMetricallyCompatible(super) == false) {
		return -1;
	}

	double absSum = 0, notAbsSum = 0;  // absSum é a distancia cronotonica

	int thisChronotSize = this->chronotonic->getSize();
	int superChronotSize = super->chronotonic->getSize();
	int maxSize = MAX(thisChronotSize, superChronotSize);
	int aux;

	for (int i = 0; i < maxSize; i ++) {
		aux = super->chronotonic->get(i % superChronotSize)
			  - this->chronotonic->get(i % thisChronotSize);

		notAbsSum += aux;
		absSum += ABS(aux);
	}

	//logFile << "Derivation measure:" << endl;
	//logFile << ">> sum: " << notAbsSum << endl;
	//logFile << ">> absolute sum: " << absSum << endl;

	return (absSum == 0)? 1 : (notAbsSum / absSum);
}

bool CbRhythmPattern::isDerivatedFrom(const CbRhythmPattern *super) const {
	return this->derivationMeasure(super) == 1.0;
}

double CbRhythmPattern::averageIntensity() const {
	Vector_pMidiEvent events;
	FmMidiEvent* ev;

	this->pattern->getAllEvents(events);

	double sumIntensities = 0;
	int count = 0;

	for (int i = events.size() - 1; i >= 0; i --) {
		ev = events[i];
		if (ev->isChannelEvent() && ev->getType() == MD_NOTE_ON) {
			sumIntensities += static_cast<FmMidiChannelEvent*>(ev)->getVelocity();
			count ++;
		}
	}

	return sumIntensities / double(count);
}


//................................//
// CLASSE CbTubs
//................................//

CbTubs::CbTubs(const CbRhythmPattern& pattern, const CbMeter& meter) {
	int i;

	this->tatumsBar = meter.getNumTatums();

	const FmMidiFileTrack *mdfile = pattern.getPattern();

	double numQuarterNotes = double(mdfile->getMaxTick() - 1.0) / double(mdfile->getResolution());

	if (numQuarterNotes <= 0) {
		numQuarterNotes = 0.1;  // para o TUBS nao ficar com tamanho zero
	}

	double tatumsInFile = numQuarterNotes * meter.getTatumsPerQuarterNote();
	int numBars = ROUND_UP( tatumsInFile / tatumsBar );

	this->tubsSize = numBars * this->tatumsBar;
	this->tubs = new ECbPercNotePitch[tubsSize];
	for (i = 0; i < tubsSize; i++) {
		tubs[i] = NP_NONE;
	}

	int oldRes = mdfile->getResolution();
	int newRes = meter.getTatumsPerQuarterNote();

	Vector_pMidiEvent evts;
	FmMidiEvent* ev;

	mdfile->getAllEvents(evts);

	int onsetBox;
	int onsetTiming;
	ECbPercNotePitch ntPitch;
	double ticksPerBox = double(oldRes) / double(newRes);

//	int avgTiming = 0;
//	int cnt = 0;

	for (i = 0; i < evts.size(); i ++) {
		ev = evts[i];
		if (ev->getType() == MD_NOTE_ON) {
			onsetBox = int(ev->getTick() / ticksPerBox);
			onsetTiming = ev->getTick() - int(ticksPerBox * onsetBox);
			ntPitch = NP_LOW;
			if (onsetTiming < (0.6 * ticksPerBox)) {
				tubs[onsetBox] = ntPitch;
			} else {
				tubs[onsetBox + 1] = ntPitch;
			}
			//avgTiming += onsetTiming;
			//cnt ++;
		}
	}

//	avgTiming = avgTiming / cnt;
//	logFile << "Timing Medio: " << avgTiming << endl;
}

CbTubs::CbTubs(const CbTubs &t) {
	this->tatumsBar = t.tatumsBar;
	this->tubsSize = t.tubsSize;
	this->tubs = new ECbPercNotePitch[this->tubsSize];
	for (int i = 0; i < this->tubsSize; i ++) {
		this->tubs[i] = t.tubs[i];
	}
}

CbTubs::~CbTubs() {
	delete tubs;
}

int CbTubs::getTatumsPerBar() const {
	return tatumsBar;
}

int CbTubs::getSize() const {
	return tubsSize;
}

int CbTubs::getNumBars() const {
	return tubsSize / tatumsBar;
}

bool CbTubs::hasOnset(int pos) const {
	return get(pos) != NP_NONE;
}

ECbPercNotePitch CbTubs::get(int pos) const {
	if (pos >= 0 && pos < tubsSize) {
		return tubs[pos];
	} else {
		return NP_NONE;
	}
}

int CbTubs::getNumOnsets() const {
	int num = 0;

	for (int i = 0; i < tubsSize; i ++) {
		if (tubs[i] != NP_NONE) {
			num ++;
		}
	}

	return num;
}

char getPercNotePitchChar(ECbPercNotePitch note) {
	char c ;

	switch (note) {
	case NP_NONE:
		c =  '.';
		break;
	case NP_HIGH:
		c = 'X';
		break;
	case NP_LOW:
		c = 'x';
		break;
	default:
		c = '?';
	}

	return c;
}

std::ostream& operator<<(std::ostream& o, const CbTubs& rp) {
	o << "[";
	for (int i = 0; i < rp.tubsSize; i ++) {
		o << getPercNotePitchChar(rp.tubs[i]);
	}
	return o << "]";
}

std::string& operator<< (std::string& out, const CbTubs& rp) {
	out += "[";

	for (int i = 0; i < rp.tubsSize; i ++) {
		out += getPercNotePitchChar(rp.tubs[i]);
	}

	out += "]";

	return out;
}


//.......................................//
// CLASSE CbOnsetsVector
//.......................................//

CbOnsetsVector::CbOnsetsVector(const CbTubs* __tubs) {
	this->tubs = __tubs;
	this->numOnsets = tubs->getNumOnsets();
	this->vonsets = new int[numOnsets];

	//logFile << "OnsetsSize: " << numOnsets << endl;

	int vpos = 0;
	for (int i = 0; i < tubs->getSize(); i ++) {
		if (tubs->hasOnset(i)) {
			this->vonsets[vpos] = i;
			vpos ++;
		}
	}
}

CbOnsetsVector::~CbOnsetsVector() {
	delete vonsets;
	//obs: nao deletar o tubs
}

int CbOnsetsVector::getSize() const {
	return numOnsets;
}

int CbOnsetsVector::get(int vpos) const {
	return vonsets[vpos];
}

ECbPercNotePitch CbOnsetsVector::getPitch(int vpos) const {
	return tubs->get(vonsets[vpos]);
}

const CbTubs* CbOnsetsVector::getTubs() const {
	return tubs;
}

std::ostream& operator<<(std::ostream& o, const CbOnsetsVector& rp) {
	o << "(";
	for (int i = 0; i < rp.numOnsets; i ++) {
		o << rp.vonsets[i];
		if (i != rp.numOnsets - 1) {
			o << ", ";
		}
	}
	return o << ")";
}


//..........................................//
// CLASSE CbInterOnsetVector
//..........................................//

CbInterOnsetVector::CbInterOnsetVector(const CbOnsetsVector& onsets) {
	this->size = onsets.getSize() + 1;
	this->vintervals = new int[size];
	//logFile << "InterOnsetsSize: " << size << endl;

	if (onsets.getSize() > 0) {
		vintervals[0] = onsets.get(0);

		for (int i = 1; i < size - 1; i ++) {
			vintervals[i] = onsets.get(i) - onsets.get(i - 1);
		}

		// distancia do ultimo ataque ao primeiro (apos um ciclo)
		vintervals[size-1] = onsets.getTubs()->getSize() + onsets.get(0)
							 - onsets.get(onsets.getSize() - 1);

	} else {
		vintervals[0] = 0;

	}

}

CbInterOnsetVector::~CbInterOnsetVector() {
	delete vintervals;
}

int CbInterOnsetVector::getSize() const {
	return size;
}

int CbInterOnsetVector::get(int vpos) const {
	return vintervals[vpos];
}

std::ostream& operator<<(std::ostream& o, const CbInterOnsetVector& rp) {
	o << "(";
	for (int i = 0; i < rp.size; i ++) {
		if (i == 0)  o << "[";
		o << rp.vintervals[i];
		if (i == 0)  o << "]";

		if (i != rp.size - 1) o << ", ";
	}
	return o << ")";
}


//...........................................//
// CLASSE CbChronotonicVector
//...........................................//

CbChronotonicVector::CbChronotonicVector(const CbInterOnsetVector& interonset, int tubsSize) {
	int i, j;

/*	this->size = 0;
	for (i = 1; i < interonset.getSize(); i ++) {
		this->size += interonset.get(i);
	}
	//logFile << "ChronotonicSize: " << size << endl;  // deve dar o tamanho do tubs !
*/
	this->size = tubsSize;
	this->vector = new int[this->size];
	int vpos, interval;

	vpos = 0;

	// preenche as primeiras posicoes com o ultimo intervalo
	for (j = 0; j < interonset.get(0); j ++) {
		this->vector[vpos] = interonset.get(interonset.getSize() - 1);
		vpos ++;
	}

	// preenche as demais posiçoes
	for (i = 1; i < interonset.getSize(); i ++) {
		for (j = 0; j < interonset.get(i) && vpos < size; j ++) {
		   this->vector[vpos] = interonset.get(i);
		   vpos ++;
		}
	}

}

CbChronotonicVector::~CbChronotonicVector() {
	delete vector;
}

int CbChronotonicVector::getSize() const {
	return size;
}

int CbChronotonicVector::get(int pos) const {
	return vector[pos];
}

int CbChronotonicVector::getMax() const {
	int max = vector[0];
	for (int i = 1; i < size; i ++) {
		if (vector[i] > max) {
			max = vector[i];
		}
	}
	return max;
}

int CbChronotonicVector::getMin() const {
	int min = vector[0];
	for (int i = 1; i < size; i ++) {
		if (vector[i] < min) {
			min = vector[i];
		}
	}
	return min;
}

std::ostream& operator<<(std::ostream& o, const CbChronotonicVector& rp) {
	o << "(";
	for (int i = 0; i < rp.size; i ++) {
		o << rp.vector[i];
		if (i != rp.size - 1) o << ", ";
	}
	return o << ")";
}


//...............................................//
// CLASSE CbFullIntervalHistogram
//...............................................//

CbFullIntervalHistogram::CbFullIntervalHistogram(const CbOnsetsVector& onsets) {
	this->size = (onsets.getTubs()->getSize() / 2) + 1;
	this->histogram = new int [this->size];

	int i, j;

	histogram[0] = onsets.getSize();
	for (i = 1; i < size; i ++) histogram[i] = 0;

	int distf, distb;
	int rpSize = onsets.getTubs()->getSize();

	for (int i = 0; i < onsets.getSize(); i ++) {
		for (int j = i + 1; j < onsets.getSize(); j ++) {
			distf = onsets.get(j) - onsets.get(i);
			distb = (rpSize + onsets.get(i)) - onsets.get(j);
			if (distf <= distb) {
				this->histogram[distf] ++;
			} else {
				this->histogram[distb] ++;
			}
		}
	}
}

CbFullIntervalHistogram::~CbFullIntervalHistogram() {
	delete histogram;
}

int CbFullIntervalHistogram::getMaxDistance() const {
	return size - 1;
}

int CbFullIntervalHistogram::getFrequency(int distance) const {
	if (distance >= 0 && distance < size) {
		return histogram[distance];
	} else {
		return -1;
	}
}

std::ostream& operator<<(std::ostream& o, const CbFullIntervalHistogram& rp) {
	o << "(";

	for (int i = 0; i < rp.size; i ++) {
		o << i << ":" << rp.histogram[i];
		if (i != rp.size - 1) o << ", ";
	}

	return o << ")";
}




