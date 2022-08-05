

#include "FmMidiEvent.h"

#define MICROSECONDS_PER_MINUTE 60000000.0


/////// Classe FmMidiEvent ///////

const char* FmMidiEvent::EVENTS_TP_STR[] = {
	"unknown",
	"note-off",
    "note-on",
    "note-aftertouch",
    "control-change",
    "program-change",
    "channel-aftertouch",
    "pitch-bend",
    "meta-event"
};

const char* FmMidiEvent::EVENTS_META_STR[] = {
	"unknown",
	"set-tempo",
    "time-signature"
};

const char* FmMidiEvent::eventTpToStr(FmEMidiEventTp tp) {
	int index = 0;

    switch (tp) {
    case MD_NOTE_OFF:
    	index = 1;
        break;
    case MD_NOTE_ON:
    	index = 2;
        break;
    case MD_NOTE_AFTERTOUCH:
    	index = 3;
        break;
    case MD_CTRL_CHANGE:
    	index = 4;
        break;
    case MD_PROG_CHANGE:
    	index = 5;
        break;
    case MD_CHAN_AFTERTOUCH:
    	index = 6;
        break;
    case MD_PITCH_BEND:
    	index = 7;
        break;
    case MD_META_EVENT:
    	index = 8;
        break;
    }

    return EVENTS_TP_STR[index];
}

const char* FmMidiEvent::metaEventTpToStr(FmEMidiMetaEventTp mtTp) {
	int index = 0;

    switch (mtTp) {
    case MD_META_SET_TEMPO:
    	index = 1;
    	break;
    case MD_META_TIME_SIGNAT:
    	index = 2;
        break;
    }

    return EVENTS_META_STR[index];
}

FmMidiEvent::FmMidiEvent(DWORD __tick) {
	this->status = 0;
    this->time = __tick;
}

FmMidiEvent::FmMidiEvent(byte __status, DWORD __tick) {
	this->status = __status;
    this->time = __tick;
}

FmMidiEvent::FmMidiEvent(const FmMidiEvent &ev) {
	throw "Cannot create copies of FmMidiEvent! Use method 'clone', instead.";
}

FmMidiEvent::~FmMidiEvent() {
}

byte FmMidiEvent::getStatus() const {
    return status;
}

FmEMidiEventTp FmMidiEvent::getType() const {
	if (status == 0xFF || status == 0xF0 || status == 0xF7) {
    	return FmEMidiEventTp(status);
    } else {
    	return FmEMidiEventTp(status & 0xF0);
    }
}

DWORD FmMidiEvent::getTick() const {
    return time;
}

void FmMidiEvent::setTick(DWORD tc) {
    time = tc;
}

bool FmMidiEvent::isChannelEvent() const {
	return false;
}

bool FmMidiEvent::isMetaEvent() const {
	return false;
}

FmMidiEvent* FmMidiEvent::createNoteOn(int nt, int vel, DWORD tic, byte ch) {
    return new FmMidiChannelEvent(MD_NOTE_ON, ch, tic, nt, vel);
}

FmMidiEvent* FmMidiEvent::createNoteOff(int nt, int vel, DWORD tic, byte ch) {
    return new FmMidiChannelEvent(MD_NOTE_OFF, ch, tic, nt, vel);
}

FmMidiEvent* FmMidiEvent::createProgChange(int patch, DWORD tic, byte ch) {
    return new FmMidiChannelEvent(MD_NOTE_ON, ch, tic, patch);
}

FmMidiMetaEvent* FmMidiEvent::createSetTempo(double tempoBPM, DWORD tick) {
	return createSetTempo((DWORD)(MICROSECONDS_PER_MINUTE / tempoBPM), tick);
}

FmMidiMetaEvent* FmMidiEvent::createSetTempo(DWORD tempo, DWORD tick) {
	byte tempoData[3];

	tempoData[2] = byte(0x00FF & tempo);
	tempoData[1] = byte(0x00FF & (tempo >> 8));
	tempoData[0] = byte(0x00FF & (tempo >> 16));

	return new FmMidiMetaEvent(MD_META_SET_TEMPO, tempoData, 3, tick);
}

FmMidiMetaEvent* FmMidiEvent::createTimeSignature(int num, int den, DWORD tick, int metr, int n32nts) {
	byte tempoData[4];

	tempoData[0] = byte(0x00FF & num);
	tempoData[1] = byte(0x00FF & den);
	tempoData[2] = byte(0x00FF & metr);
	tempoData[3] = byte(0x00FF & n32nts);

	return new FmMidiMetaEvent(MD_META_TIME_SIGNAT, tempoData, 4, tick);
}

FmMidiEvent* FmMidiEvent::loadChannelEvent(DWORD absTick, byte st, byte data1, byte data2) {
    int fnibble = st & 0xF0;
    int snibble = st & 0x0F;
    FmMidiEvent* event = NULL;

    switch (fnibble) {
    case MD_NOTE_OFF:
        event = createNoteOff(data1, data2, absTick, snibble);
        break;
    case MD_NOTE_ON:
    	if (data2 == 0) {
	        event = createNoteOff(data1, 120, absTick, snibble);
        } else {
	        event = createNoteOn(data1, data2, absTick, snibble);
        }
        break;
    case MD_PROG_CHANGE:
        event = createProgChange(data1, absTick, snibble);
        break;
    case MD_NOTE_AFTERTOUCH:
    case MD_CTRL_CHANGE:
    case MD_CHAN_AFTERTOUCH:
    case MD_PITCH_BEND:
    default:
        event = new FmMidiChannelEvent((FmEMidiEventTp)fnibble, snibble, absTick, data1, data2);
        break;
    }

    return event;
}

FmMidiEvent* FmMidiEvent::loadMetaEvent(DWORD absTick, byte metaType, int dataLen, byte* data) {
    FmMidiEvent* event = new FmMidiMetaEvent((FmEMidiMetaEventTp)metaType, data, dataLen, absTick);

    return event;
}

/////// Classe FmMidiChannelEvent ///////

FmMidiChannelEvent::FmMidiChannelEvent(FmEMidiEventTp tp, byte ch, DWORD tick, byte dt1, byte dt2)
	: FmMidiEvent(tick), data1(dt1), data2(dt2)
{
    this->status = byte(tp | (ch & 0x0F));
}

FmMidiChannelEvent::~FmMidiChannelEvent() {
}

bool FmMidiChannelEvent::isChannelEvent() const {
	return true;
}

FmMidiEvent* FmMidiChannelEvent::clone() const {
	FmMidiChannelEvent *copy = new FmMidiChannelEvent(getType(), getChannel(), this->time, this->data1, this->data2);
	return copy;
}

byte FmMidiChannelEvent::getChannel() const {
    return byte(this->status & 0x0F);
}

byte FmMidiChannelEvent::getData1() const {
    return data1;
}

byte FmMidiChannelEvent::getData2() const {
    return data2;
}

int FmMidiChannelEvent::getPitch() const {
    return data1;
}

int FmMidiChannelEvent::getVelocity() const {
    return data2;
}

int FmMidiChannelEvent::getPatchNumber() const {
    return data1;
}


/////// Classe FmMidiMetaEvent ///////

FmMidiMetaEvent::FmMidiMetaEvent(FmEMidiMetaEventTp eventType, const byte* dt, int dataLength, DWORD tick)
	: FmMidiEvent(MD_META_EVENT, tick)
{
	this->metaEventType = eventType;
    this->length = dataLength;
    this->data = new byte[dataLength];

    memcpy(this->data, dt, dataLength);
}

FmMidiMetaEvent::~FmMidiMetaEvent() {
	delete data;
}

bool FmMidiMetaEvent::isMetaEvent() const {
	return true;
}

FmMidiEvent* FmMidiMetaEvent::clone() const {
	FmMidiMetaEvent *copy = new FmMidiMetaEvent(getMetaEventType(), this->data, this->length, this->time);
    return copy;
}

FmEMidiMetaEventTp FmMidiMetaEvent::getMetaEventType() const {
	return this->metaEventType;
}

int FmMidiMetaEvent::getData(byte** dataPtr) const {
	(*dataPtr) = this->data;
    return this->length;
}

DWORD FmMidiMetaEvent::getTempo() const {
	DWORD res = (data[0] << 16) | (data[1] << 8) | (data[2]);
    return res;
}

double FmMidiMetaEvent::getTempoBPM() const {
	int t = getTempo();
	return ( t == 0 ? -1 : (MICROSECONDS_PER_MINUTE / double(t)) );
}

byte FmMidiMetaEvent::getNumerator() const {
	return data[0];
}

byte FmMidiMetaEvent::getDenominator() const {
	return data[1];
}

byte FmMidiMetaEvent::getMetronomePer24Clocks() const {
	return data[2];
}

byte FmMidiMetaEvent::get32ndNotesPer24Clocks() const {
	return data[3];
}

