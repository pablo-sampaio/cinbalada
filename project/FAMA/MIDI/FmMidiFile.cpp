
#include "FmMidiFile.h"

// mudar aqui para habilitar/desabilitar log
#define LOGGING

// definicao do arquivo de LOG
#ifdef LOGGING
	#include <fstream>
	static std::ofstream logFile("logs\\FmMidiFile.txt", std::ios::out);
	#define LOG logFile
#else
	#define LOG cout
#endif

using namespace std;


FmMidiFile::FmMidiFile(int nTracks, int _resolution)
    : resolution(_resolution), numerator(4), denominator(2), tempo(100)
{
    for (int i = 0; i < nTracks; i ++) {
		this->tracks.push_back(new FmMidiFileTrack(_resolution));
    }
}

FmMidiFile::FmMidiFile(const char* file, int _resolution)
	: resolution(_resolution), tempo(100)
{
	this->numerator = -1;
	loadFromFile(file);
	if (numerator == -1) {
		this->numerator = 4;
		this->denominator = 2;
	}
}

// 'this->numerator' : -1, to load meter from file (is there's one); otherwise, current meter is kept
// 'this->tracks'    : should be empty
// 'this->resolution': -1, to use file resolution; value > 0, to keep current resolution
void FmMidiFile::loadFromFile(const AnsiString& midiFile) {
	TMaxMidiSMF* maxMidiSMF = new TMaxMidiSMF(NULL);
	TMaxMidi* maxMidi =  new TMaxMidi(NULL);

	maxMidiSMF->MaxMidi = maxMidi;
	maxMidiSMF->Open(midiFile, READ);

	TMaxMidiScore* maxMidiScore = new TMaxMidiScore(maxMidiSMF->NumTracks());

	maxMidiSMF->Attach(maxMidiScore);
	maxMidiSMF->Load(NULL);

	int fileResolution = maxMidiSMF->Resolution();

	if (this->resolution < 0) {
		this->resolution = fileResolution;
	}

	for (int i = 0; i < maxMidiSMF->NumTracks(); i ++) {
		this->tracks.push_back(new FmMidiFileTrack(this->resolution));
	}

	LPMIDIEVENT midiEvent;
	FmMidiEvent* mdEvent;
	int newTick;
	char *buffer;
	DWORD szBuffer;

	this->totalTicks = 0;

	LOG << endl << "--- CARREGANDO <<" << midiFile.c_str() << ">> ---" << endl;

	for (int t = 0; t < maxMidiScore->Count; t ++) {
		TMaxMidiTrack *track = maxMidiScore->Tracks[t];
		int numEvents = track->GetNumEvents();
		this->tracks[t]->setResolution(this->resolution);
		LOG << ">> Track " << t << endl;

		int midiTick = 0;

		for (int i = 0; i < numEvents; i++) {
			midiEvent = track->GetEvent(i);
			midiTick += midiEvent->time;
			newTick = (midiTick*this->resolution) / fileResolution;

			mdEvent = convertToFmMidiEvent(midiEvent, newTick);

			if (mdEvent->isMetaEvent()) {
				this->tempo = static_cast<FmMidiMetaEvent*>(mdEvent)->getTempoBPM(); // so pode ser um tempo-change...
				LOG << "Encontrado um tempo change!!! Novo tempo: " << this->tempo << endl;
			}

			this->tracks[t]->addEvent(mdEvent);
		}

		DWORD tickTimeSig = maxMidiSMF->ReadMeta(track, META_TIME_SIG, &buffer, &szBuffer);
		if (tickTimeSig != -1) {
			LOG << "Encontrado meta-evento com assinatura de tempo !!!" << endl;
			// o primeiro 'time-sig' eh setado nos atributos num/den
			if (this->numerator == -1) {
				this->numerator = buffer[0];
				this->denominator = buffer[1];
			}
			// demais 'time-sig' ficam como eventos
			else {
				this->tracks[t]->addEvent(FmMidiEvent::loadMetaEvent(tickTimeSig, META_TIME_SIG, szBuffer, buffer));
			}
			GlobalFree(buffer);
		}

		if (this->tracks[t]->getMaxTick() > this->totalTicks) {
			this->totalTicks = this->tracks[t]->getMaxTick();
		}

	} // for

	//fecha arquivo
	maxMidiSMF->Close();
	delete maxMidiScore;
	delete maxMidiSMF;
	delete maxMidi;
}

FmMidiEvent* FmMidiFile::convertToFmMidiEvent(LPMIDIEVENT event, DWORD tick) {
	LOG << "Carregando evento : tick(rel): " << event->time << ", status: 0x" << hex << int(event->status) << endl << dec;

	if (event->status == 0) {
		byte data[] = {event->data1, event->data2, event->data3};
		return FmMidiEvent::loadMetaEvent(tick, MD_META_SET_TEMPO, 3, data);
	} else {
		return FmMidiEvent::loadChannelEvent(tick, event->status, event->data1, event->data2);
	}
}

LPMIDIEVENT FmMidiFile::convertToMaxMidiEvent(FmMidiChannelEvent* event, DWORD tick) {
	LPMIDIEVENT eventx = new MidiEvent;
	eventx->time = tick;
	eventx->status = event->getStatus();
	eventx->data1 = event->getData1();
	eventx->data2 = event->getData2();
	LOG << "Salvando evento : tick(rel): " << dec << tick << ", status: 0x" << hex << int(event->getStatus()) << endl;
	return eventx;
}

LPMIDIEVENT FmMidiFile::convertToMaxMidiEvent(FmMidiMetaEvent* event, DWORD tick) {
	LPMIDIEVENT eventx = NULL;

	if (event->getMetaEventType() == MD_META_SET_TEMPO) {
		eventx = new MidiEvent;

		byte *data;
		event->getData(&data);

		eventx->time = tick;
		eventx->status = 0;  // coisas do MaximumMidi...
		eventx->data1 = data[0];
		eventx->data2 = data[1];
		eventx->data3 = data[2];

	}

	return eventx;
}

FmMidiFile::~FmMidiFile() {
	free();
}

void FmMidiFile::free() {
	int i;

	this->totalTicks = 0;

	for (int i = 0; i < this->tracks.size(); i ++) {
		delete this->tracks[i];
	}
	this->tracks.clear();
}

void FmMidiFile::loadFile(const char* midiFile, int resolution) {
	free();
	this->resolution = resolution;
	this->numerator = -1;
	loadFromFile(midiFile);
	if (numerator == -1) {
		this->numerator = 4;
		this->denominator = 2;
	}
}

void FmMidiFile::saveToFile(const char* midiFile) {
	TMaxMidiSMF* midiSMF = new TMaxMidiSMF(NULL);
	TMaxMidi* maxMidi =  new TMaxMidi(NULL);

	midiSMF->MaxMidi = maxMidi;
	midiSMF->Open(midiFile, WRITE, 1);
	midiSMF->Resolution(this->resolution);

	// cria evento da assinatura inicial
	FmMidiMetaEvent* timeSigEv = FmMidiEvent::createTimeSignature(this->numerator, this->denominator, 0);
	this->addEvent(timeSigEv, 0);
	delete timeSigEv;

	TMaxMidiScore* maxMidiScore = new TMaxMidiScore(this->tracks.size());
	midiSMF->Attach(maxMidiScore);
	loadMaxMidiScore(*maxMidiScore, *midiSMF);

	midiSMF->Save(NULL);
	midiSMF->Close();
	delete maxMidiScore;
	delete maxMidi;
	delete midiSMF;
}

void FmMidiFile::loadMaxMidiScore(TMaxMidiScore& score, TMaxMidiSMF& midiSMF) {
	int i, j;
	int numEvents;
	DWORD lastTick;
	Vector_pMidiEvent evts;
	LPMIDIEVENT mxMidiEv;

	LOG << endl << "--- SALVANDO ---" << endl;
	for (i = this->tracks.size() - 1; i >= 0; i --) {
		score.Tracks[i]->CreateBuffer(this->tracks[i]->getNumEvents());

		this->tracks[i]->getAllEvents(evts);
		numEvents = 0;
		lastTick = 0;

		LOG << ">> Track " << i << endl;
		for (j = 0; j < evts.size(); j ++) {

			if (evts[j]->isChannelEvent()) {
				mxMidiEv = convertToMaxMidiEvent(static_cast<FmMidiChannelEvent*>(evts[j]), evts[j]->getTick() - lastTick);
				score.Tracks[i]->SetEvent(mxMidiEv, numEvents);
				lastTick = evts[j]->getTick();
				numEvents ++;
				delete mxMidiEv;

			} else if (evts[j]->isMetaEvent()) {
				FmMidiMetaEvent* metaEv = static_cast<FmMidiMetaEvent*>(evts[j]);
				FmEMidiMetaEventTp metaEvTp = metaEv->getMetaEventType();

				// track 0: meta-eventos 'time_sig' e 'set-tempo'
				if (i == 0) {
					if (metaEvTp == MD_META_SET_TEMPO) {
						mxMidiEv = convertToMaxMidiEvent(metaEv, metaEv->getTick() - lastTick);
						score.Tracks[i]->SetEvent(mxMidiEv, numEvents);
						lastTick = metaEv->getTick();
						numEvents ++;
						delete mxMidiEv;
					} else if (metaEvTp == MD_META_TIME_SIGNAT) {
						byte *data;
						metaEv->getData(&data);
						midiSMF.WriteMeta(score.Tracks[i], META_TIME_SIG, data, metaEv->getTick() - lastTick);
						lastTick = metaEv->getTick();
						numEvents ++;
					} else {
						LOG << "Meta-evento desconhecido !" << endl;
					}
				}
				// outras tracks
				else {
					if (metaEvTp == MD_META_SET_TEMPO || metaEvTp == MD_META_TIME_SIGNAT) {
						this->tracks[0]->addEvent(metaEv); // readiciona na track 0
					} else {
						LOG << "Meta-evento desconhecido !" << endl;
					}
				}

			}
		}

		//LOG << "[Teste]: " << evts.size() << " == " << tracks[i]->getNumEvents() << endl;
		score.Tracks[i]->SetNumEvents(numEvents);
	}
}

int FmMidiFile::createTrack() {
	this->tracks.push_back(new FmMidiFileTrack(this->resolution));
	return this->tracks.size() - 1;
}

void FmMidiFile::addEvent(const FmMidiEvent* event, int track, int offsetTicks) {
	FmMidiEvent* eventClone;

	if (track < tracks.size()) {
		eventClone = event->clone();
		eventClone->setTick( eventClone->getTick() + offsetTicks );
		this->tracks[track]->addEvent(eventClone);

		if (this->tracks[track]->getMaxTick() > this->totalTicks) {
			this->totalTicks = this->tracks[track]->getMaxTick();
		}
	}
}

void FmMidiFile::truncate(int maxTick) {
	if (maxTick < 0 || maxTick >= this->totalTicks) {
		return;
	}

	this->totalTicks = 0;  // para ser calculado novamente

	for (int i = 0; i < tracks.size(); i ++) {
		tracks[i]->truncate(maxTick);

		if (this->tracks[i]->getMaxTick() > this->totalTicks) {
			this->totalTicks = this->tracks[i]->getMaxTick();
		}
	}
}

void FmMidiFile::addEvents(const FmMidiFileTrack& phrase, int destTrack, int offsetTicks) {
	if (destTrack < tracks.size()) {
        this->tracks[destTrack]->merge(phrase, offsetTicks);
        if (this->tracks[destTrack]->getMaxTick() > this->totalTicks) {
	        this->totalTicks = this->tracks[destTrack]->getMaxTick();
        }
    }
}

void FmMidiFile::getMidiEvents(DWORD tick, int track, Vector_pMidiEvent& result) const {
    if (track < tracks.size() ) {
        const Vector_pMidiEvent* evts = this->tracks[track]->getEvents(tick);

        result.clear();
        if (evts != NULL) {
            for (int i = 0; i < evts->size(); i ++) {
                result.push_back((*evts)[i]);
            }
        }
    }
}

void FmMidiFile::getMidiEvents(DWORD tick, Vector_pMidiEvent& result) const {
    const Vector_pMidiEvent * evts;
    int i, j;

    result.clear();

    for (int i =0; i < tracks.size(); i ++) {
        evts = this->tracks[i]->getEvents(tick);
        if (evts != NULL) {
            for (int j = 0; j < evts->size(); j ++) {
                result.push_back((*evts)[j]);
            }
        }
    }

}

DWORD FmMidiFile::getMaxTick() const {
    return totalTicks;
}

int FmMidiFile::getNumTracks() const {
    return this->tracks.size();
}

const FmMidiFileTrack* FmMidiFile::getTrack(int i) const {
    if (i < this->tracks.size()) {
        return this->tracks[i];
    } else {
        return NULL;
    }
}

int FmMidiFile::getResolution() const {
    return resolution;
}

void FmMidiFile::setResolution(int res) {
    if (res != this->resolution && res > 0) {
        this->resolution = res;
        for (int i =0; i < tracks.size(); i ++) {
            tracks[i]->setResolution(res);
        }
    }

    // recalcula totalTicks...
    this->totalTicks = 0;
    for (int i =0; i < tracks.size(); i ++) {
        if (tracks[i]->getMaxTick() > this->totalTicks) {
            this->totalTicks = tracks[i]->getMaxTick();
        }
    }

}

int FmMidiFile::getNumQuarterNotes() const {
	return (this->totalTicks + resolution - 1) / resolution; //arredonda a divisão pra cima
		// BUG: nao era pra precisar desse '-1' (totalticks eh baseado em zero) (RETESTAR!)
}

int FmMidiFile::getNumBars() const {
	int realDen = 1 << this->denominator;
	int wholeNoteRes = 4 * resolution;
	int barRes = (numerator * wholeNoteRes) / realDen;

	return (this->totalTicks + barRes - 1) / barRes;
}

int FmMidiFile::getNumerator() const {
	return this->numerator;
}

void FmMidiFile::setNumerator(int n) {
	if (n > 0 && n < 256) {
		this->numerator = n;
	}
}

int FmMidiFile::getDenominator() const {
	return this->denominator;
}

void FmMidiFile::setDenominator(int d) {
	if (d >= 0 && d < 256) {
		this->denominator = d;
	}
}

int FmMidiFile::getTempo() const {
	return this->tempo;
}

/*void FmMidiFile::printLog() {
	Vector_pMidiEvent allEvents;
	FmMidiChannelEvent* event;

	logFile << endl << "Lendo arquivo MIDI (resolution " << this->resolution << ")" << endl;

	for (int t = 0; t < this->tracks.size(); t ++) {
		logFile << "Lendo track [" << t << "]" << endl;
		logFile << " >> " << *(this->tracks[i]) << endl;
	}

}*/
