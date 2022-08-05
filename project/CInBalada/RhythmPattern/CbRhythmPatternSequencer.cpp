
#include "CbRhythmPatternSequencer.h"

using namespace std;

#include <fstream>
static ofstream logFile("logs\\CbRhythmPatternSequencer.txt", ios::out);

// d - tick duration; r - resolution (ticks per beat)
#define TICKDURATION_TO_BPM(d, r)      ( 60000.0 / ((d) * (r)) )

// t - tempo in BPM; r - resolution (ticks per beat)
#define BPM_TO_TICKDURATION(t, r)     ( 60000.0 / ((t) * (r)) )


CbRhythmPatternSequencer* CbRhythmPatternSequencer::instance = NULL;

CbRhythmPatternSequencer::SmartCleaner CbRhythmPatternSequencer::smartCleaner;

CbRhythmPatternSequencer::SmartCleaner::~SmartCleaner() {
	if (CbRhythmPatternSequencer::instance != NULL) {
		delete CbRhythmPatternSequencer::instance;
	}
	CbRhythmPatternSequencer::instance = NULL;
}

CbRhythmPatternSequencer* CbRhythmPatternSequencer::getSequencer() {
	if (instance == NULL) {
		instance = new CbRhythmPatternSequencer();
	}
	return instance;
}


CbRhythmPatternSequencer::CbRhythmPatternSequencer() {
	this->saida = new FmMidiOutput(3);

	this->currentTick = 0;
	this->resolution = 240;

	this->beatsAlignment = 2;
	this->numPatterns = 0;

	this->semaphore = new TCriticalSection;

	this->OnTick = NULL;

	// configura timer
	this->timerId = NULL; // timer parado
	this->timerPeriod = BPM_TO_TICKDURATION(100,resolution);  // andamento de 100 BPM

	TIMECAPS tc;
	int DESIRED_RES = 2;  // resolucao do timer desejada: 2 ms

//	timeGetDevCaps(&tc,sizeof(TIMECAPS));
//	this->timerResolution = min(max(tc.wPeriodMin,DESIRED_RES),tc.wPeriodMax);
//	timeBeginPeriod(this->timerResolution);

}

CbRhythmPatternSequencer::CbRhythmPatternSequencer(const CbRhythmPatternSequencer& s) {
	throw "Cannot create copies of a CbRhythmPatternSequencer!";
}

CbRhythmPatternSequencer::~CbRhythmPatternSequencer() {
	logFile << "Encerrando..." << endl;

	this->stop();
//	timeEndPeriod(this->timerResolution);

	semaphore->Acquire();
	delete saida;
	semaphore->Release();

	delete semaphore;

	RpSeqInfo* tmp;
	while (midis.size() > 0) {
		tmp = midis.back();
		midis.pop_back();
		if (tmp != NULL) {
			delete tmp;
		}
	}

	logFile << "Sequenciador encerrado!" << endl;
}

int CbRhythmPatternSequencer::getCurrentTick() {
	return this->currentTick;
}

int CbRhythmPatternSequencer::getResolution() {
	return this->resolution;
}

int CbRhythmPatternSequencer::add(const CbRhythmPattern* ptrn, bool paused) {
	int result = -1;

	semaphore->Acquire();

/*	if (ptrn->getBeatsAlignment() > this->beatsAlignment) {
		this->beatsAlignment = ptrn->getBeatsAlignment();
		logFile << "Alignment (beats) = " << beatsAlignment << endl;
	}
*/
	if (numPatterns < midis.size()) {
		for (int i = 0; i < midis.size(); i ++) {
			if (midis[i] == NULL) {
				midis[i] = new RpSeqInfo(ptrn, paused);
				//midis[i]->midif->setResolution(this->resolution);
				result = i;
				this->numPatterns ++;
				break;
			}
		}
	} else {
		midis.push_back(new RpSeqInfo(ptrn, paused));
		result = midis.size() - 1;
		this->numPatterns ++;
	}

	semaphore->Release();

	return result;
}

void CbRhythmPatternSequencer::remove(int index) {
	semaphore->Acquire();

	RpSeqInfo* info;

	if (index >= 0 && index < midis.size() && midis[index] != NULL) {
		info = midis[index];
		midis[index] = NULL;

		delete info;
		this->numPatterns --;

		this->beatsAlignment = 2;
/*		for (int i = 0; i < midis.size(); i ++) {
			if (midis[i] != NULL
					&& midis[i]->pattern->getBeatsAlignment() > beatsAlignment) {
				this->beatsAlignment = midis[i]->pattern->getBeatsAlignment();
			}
		}*/

	}

	semaphore->Release();
}

void CbRhythmPatternSequencer::removeAll() {
	semaphore->Acquire();

	RpSeqInfo* tmp;

	for (int i = 0; i < midis.size(); i ++) {
		if (midis[i] != NULL) {
			delete midis[i];
			midis[i] = NULL;
		}
	}

	this->numPatterns = 0;
	this->beatsAlignment = 2;

	semaphore->Release();
}

int CbRhythmPatternSequencer::getNumPatterns() {
	return this->numPatterns;
}

const CbRhythmPattern* CbRhythmPatternSequencer::getPattern(int index) {
	return midis[index]->pattern;
}

void CbRhythmPatternSequencer::pausePattern(int index) {
	midis[index]->paused = true;
}

void CbRhythmPatternSequencer::resumePattern(int index) {
	midis[index]->paused = false;
}

void CbRhythmPatternSequencer::start() {
	semaphore->Acquire();
	if (! hasStarted()) {
		logFile << "Iniciando sequenciador..." << endl;

//		MMRESULT result = timeSetEvent(this->timerPeriod, this->timerResolution,
//										 timerCallBack, (DWORD)this, TIME_PERIODIC);
//		this->timerId = (UINT)result;
	}
	semaphore->Release();
}

void CbRhythmPatternSequencer::stop() {
	semaphore->Acquire();
	logFile << "Parando sequenciador..." << endl;

	if (hasStarted()) {
//		MMRESULT result = timeKillEvent(this->timerId);
//		if (TIMERR_NOERROR == result) {
//			this->timerId = NULL;
//		}
	}

	semaphore->Release();
}

bool CbRhythmPatternSequencer::hasStarted() {
	return this->timerId != NULL;
}

int CbRhythmPatternSequencer::getTempo() {
	return TICKDURATION_TO_BPM(this->timerPeriod,this->resolution);
}

void CbRhythmPatternSequencer::setTempo(int t) {
	this->stop();
	this->timerPeriod = BPM_TO_TICKDURATION(t,this->resolution);
	this->start();
}

void CALLBACK CbRhythmPatternSequencer::timerCallBack(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) {
	if (CbRhythmPatternSequencer::instance != NULL) {
		CbRhythmPatternSequencer::instance->executeTick();
	}
	//((CbRhythmPatternSequencer*)dwUser)->executeTick();
}

void CbRhythmPatternSequencer::executeTick() {
	int nextBar;
	const Vector_pMidiEvent* events;

	//logFile << "Chegou tick !" << endl;

	semaphore->Acquire();

	int nextTick = (this->currentTick + 1) % (resolution * beatsAlignment);

	// se reiniciou (apos executar o maior arquivo todo)
	if (nextTick < this->currentTick) {
		nextTick = 0;  // previne problemas com mudanças no maxBars (para menos)
	}

	RpSeqInfo *info;
	FmMidiEvent *ev;
	FmMidiChannelEvent *aux;
	int i, j, tickPtrn;

	for (i = 0; i < midis.size(); i ++) {
		info = midis[i];

		if (info != NULL) {
			tickPtrn = currentTick; // % (info->pattern->getBeatsAlignment() * resolution);

			tickPtrn = CONVERTTICK(tickPtrn,resolution,info->pattern->getPattern()->getResolution());
			events = info->pattern->getPattern()->getEvents(tickPtrn);

			if (events != NULL) {
				logFile << "Executando eventos (" << events->size() << ")..." << endl;

				for (j = 0; j < events->size(); j ++) {
					ev = (*events)[j];

					if (ev->isChannelEvent()) {
						aux = static_cast<FmMidiChannelEvent*>(ev);
						switch (ev->getType()) {
						case MD_NOTE_ON:
							saida->playNote(aux->getChannel(), aux->getVelocity(), aux->getPitch());
							break;
						/*case MD_NOTE_OFF:
							saida->stopNote(aux->getChannel(), aux->getPitch());
							break;*/
						}
					}

				} //end for
			}

		} //end if

	} //end for

    this->currentTick = nextTick;

    semaphore->Release();

	if (this->OnTick != NULL) {
		this->OnTick(this);
    }

}



