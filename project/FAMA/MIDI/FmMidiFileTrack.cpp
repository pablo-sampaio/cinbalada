
#include "FmMidiFile.h"

using namespace std;


FmMidiFileTrack::FmMidiFileTrack(int __resolution)
    : resolution(__resolution), countEvents(0), maxTick(0)
{}

FmMidiFileTrack::~FmMidiFileTrack() {
    map<DWORD,Vector_pMidiEvent*>::iterator it;
    Vector_pMidiEvent* evts;

    for (it = events.begin(); it != events.end(); it ++) {
        evts = it->second;
        for (int k = 0; k < evts->size(); k ++) {
            delete (*evts)[k];
        }
        delete evts;
    }
}

const Vector_pMidiEvent* FmMidiFileTrack::getEvents(int tick) const {
    if (events.find(tick) == events.end()) {
        return NULL;
    } else {
        return events.find(tick)->second;
    }
}

void FmMidiFileTrack::getAllEvents(Vector_pMidiEvent& result) const {
    map<DWORD,Vector_pMidiEvent*>::const_iterator it;
    Vector_pMidiEvent* evts;

    result.clear();
    it = events.lower_bound(0);
    while (it != events.end()) {
        evts = it->second;
        for (int i = 0; i < evts->size(); i ++) {
            result.push_back((*evts)[i]);
        }
        it = events.lower_bound(it->first+1);
    }
}

void FmMidiFileTrack::addEvent(FmMidiEvent* ev) {
    int tick = ev->getTick();
    Vector_pMidiEvent* evts;

	if (this->events.find(tick) == events.end()) {
		this->events[tick] = new Vector_pMidiEvent;
	}

	this->events[tick]->push_back(ev);
	this->countEvents ++;

	if (tick > this->maxTick) {
		this->maxTick = tick;
	}
}

DWORD FmMidiFileTrack::getMaxTick() const {
	return this->maxTick;
}

int FmMidiFileTrack::getResolution() const {
	return this->resolution;
}

void FmMidiFileTrack::setResolution(int newRes) {
	Vector_pMidiEvent allEvents;
	FmMidiEvent* ev;

	if (this->resolution == newRes) {
		return ;
	}

	this->getAllEvents(allEvents);

	//prepara para reinserir os eventos
	this->countEvents = 0;
	this->maxTick = 0;

	//deleta os vetores
	map<DWORD,Vector_pMidiEvent*>::iterator it;
	for (it = events.begin(); it != events.end(); it++) {
		delete it->second;
	}
	events.clear();

	// converte e reinsere os eventos
	for (int i = 0; i < allEvents.size(); i ++) {
		allEvents[i]->setTick(CONVERTTICK (allEvents[i]->getTick(),this->resolution,newRes));
		this->addEvent(allEvents[i]);
	}

	this->resolution = newRes;

}

void FmMidiFileTrack::shiftEvents(int offsetTicks) {
	map<DWORD,Vector_pMidiEvent*>::iterator it;
	FmMidiEvent* ev;
	int i;

	for (it = events.begin(); it != events.end(); it++) {
		for (i = 0; i < it->second->size(); i ++) {
			ev = (*it->second)[i];
			ev->setTick(ev->getTick() + offsetTicks);
		}
	}
	this->maxTick += offsetTicks;
}

void FmMidiFileTrack::merge(const FmMidiFileTrack& track, int offsetTicks) {
	map<DWORD,Vector_pMidiEvent*>::const_iterator it;
	FmMidiEvent* ev;
	int i;

	for (it = track.events.begin(); it != track.events.end(); it++) {
		for (i = 0; i < it->second->size(); i ++) {
			ev = ((*it->second)[i])->clone();
			ev->setTick( CONVERTTICK (ev->getTick(), track.resolution, this->resolution)
							+ offsetTicks );
			this->addEvent(ev);    // ok, os eventos serao contados
		}
	}
}

int FmMidiFileTrack::getNumQuarterNotes() const {
	return (this->maxTick + resolution - 1) / resolution; //arredonda a divisão pra cima
		//precisa desse '-1'? maxTick ja eh um a menos q a contagem de ticks
}

int FmMidiFileTrack::getNumEvents() const {
	return this->countEvents;
}

void FmMidiFileTrack::truncate(int maxTick__) {
	if (maxTick__ < 0 || maxTick__ >= this->maxTick) {
		return;
	}

	Vector_pMidiEvent allEvents;
	FmMidiEvent* ev;

	this->getAllEvents(allEvents);

	//prepara para reinserir os eventos
	this->countEvents = 0;
	this->maxTick = 0;

	//deleta os vetores
	map<DWORD,Vector_pMidiEvent*>::iterator it;
	for (it = events.begin(); it != events.end(); it++) {
		delete it->second;
	}
	events.clear();

	// converte e reinsere os eventos
	for (int i = 0; i < allEvents.size(); i ++) {
		if (allEvents[i]->getTick() <= maxTick__ ) {
			this->addEvent(allEvents[i]);
		} else {
			delete allEvents[i];
		}
	}

}

// implementacao alternativa (nao testada)
/*void FmMidiFileTrack::truncate(int maxTick__) {
	if (maxTick__ <= this->maxTick) {
		return;
	}

	Vector_pMidiEvent allEvents;
	FmMidiEvent* ev;
	int tick;

	//deleta os vetores que passam do tick
	map<DWORD,Vector_pMidiEvent*>::iterator it;
	for (it = events.begin(); it != events.end(); it++) {
		if (it->first >= maxTick__) {
			this->countEvents -= it->second->size();
			delete it->second;
			it->second = NULL;
		}
	}
	events.clear();

	// recalcula maxTick
	this->maxTick = 0;
	this->getAllEvents(allEvents);

	for (int i = 0; i < allEvents.size(); i ++) {
		tick = allEvents[i]->getTick();
		if (tick > this->maxTick) {
			this->maxTick = tick;
		}
	}

}*/


ostream& operator<<(ostream& o, const FmMidiFileTrack& t) {
	Vector_pMidiEvent result;

	t.getAllEvents(result);

	o << "[ ";

	for (int i = 0; i < result.size(); i ++) {
		if (result[i]->getType() == MD_NOTE_ON) {
			if (i > 0) o << ", ";
			o << result[i]->getTick();
		}
	}
	o << " ] (resolution = " << t.resolution << ")";

    return o;
}

