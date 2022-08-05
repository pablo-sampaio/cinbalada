
#ifndef __FMMIDIEVENT_H__
#define __FMMIDIEVENT_H__

#include "MaximumMidi\maxmidi.h"
#include <vector>

// MIDI Event types
typedef enum {
        MD_NOTE_OFF = 0x80,         MD_NOTE_ON = 0x90,
        MD_NOTE_AFTERTOUCH = 0xA0,  MD_CTRL_CHANGE = 0xB0,
        MD_PROG_CHANGE = 0xC0,      MD_CHAN_AFTERTOUCH = 0xD0,
        MD_PITCH_BEND = 0xE0,       MD_META_EVENT = 0xFF
        }
FmEMidiEventTp;

// MIDI Meta-Events types
typedef enum {
        MD_META_SET_TEMPO = 0x51,
        MD_META_TIME_SIGNAT = 0x58
        }
FmEMidiMetaEventTp;

class FmMidiMetaEvent;

/**
 * Class to represent all MIDI events (also called MIDI messages).
 */
class FmMidiEvent {
protected:
	DWORD	time;		// absolute tick !
	byte	status;		// status byte of this message

    static const char* EVENTS_TP_STR[];
    static const char* EVENTS_META_STR[];

    FmMidiEvent(DWORD tick);
    FmMidiEvent(byte status, DWORD tick);
    FmMidiEvent(const FmMidiEvent &ev); //nao usar !!!

public:
	static const char* eventTpToStr(FmEMidiEventTp);
	static const char* metaEventTpToStr(FmEMidiMetaEventTp);

    virtual ~FmMidiEvent();

    byte getStatus() const;
    DWORD getTick() const;
    void setTick(DWORD tc);

	// type of message (like status, but it ignores channel for channel-events)
    FmEMidiEventTp getType() const;

	// creates a copy of the object in the heap
	virtual FmMidiEvent* clone() const = 0;

    // use to make adequate casts to subclasses
    virtual bool isChannelEvent() const;
    virtual bool isMetaEvent() const;

    // channel events
    static FmMidiEvent* createNoteOn(int nt, int vel, DWORD tic, byte ch);
    static FmMidiEvent* createNoteOff(int nt, int vel, DWORD tic, byte ch);
    static FmMidiEvent* createProgChange(int patch, DWORD tic, byte ch);

    // meta events
    static FmMidiMetaEvent* createSetTempo(double tempoBPM, DWORD tick);
	static FmMidiMetaEvent* createSetTempo(DWORD tempo, DWORD tick);
	static FmMidiMetaEvent* createTimeSignature(int num, int den, DWORD tk, int metr = 24, int n32nts = 8); // 'den' as expoent in base 2

    static FmMidiEvent* loadChannelEvent(DWORD absTick, byte st, byte data1, byte data2);
    static FmMidiEvent* loadMetaEvent(DWORD absTick, byte metaType, int dataLen, byte* data);

};


/**
 * MIDI events directed to a specific channel.
 */
class FmMidiChannelEvent : public FmMidiEvent {
protected:
	friend class FmMidiEvent;

	// lower level
	byte	data1;		// first data byte of message
    byte	data2;		// second data byte of message

	FmMidiChannelEvent(FmEMidiEventTp tp, byte ch, DWORD tick, byte data1, byte data2=0);

public:

    virtual ~FmMidiChannelEvent();

	// overrides super-class method
    virtual bool isChannelEvent() const;
	virtual FmMidiEvent* clone() const;

	// for every channel event
    byte getChannel() const;

    // low-level
    byte getData1() const;
    byte getData2() const;

    // note on / note off
    int getPitch() const;
    int getVelocity() const;

    // program change
    int getPatchNumber() const;

};


/**
 * Events that carry extra information in MIDI files.
 */
class FmMidiMetaEvent : public FmMidiEvent {
protected:
	friend class FmMidiEvent;

    FmEMidiMetaEventTp metaEventType;
    int length;
    byte* data;

   	FmMidiMetaEvent(FmEMidiMetaEventTp eventType, const byte* data, int dataLength, DWORD tick);

public:

	virtual ~FmMidiMetaEvent();

	// overrides super-class method
    bool isMetaEvent() const;
	virtual FmMidiEvent* clone() const;

    // all meta-events
    FmEMidiMetaEventTp getMetaEventType() const;
	int getData(byte** dataPtr) const;  // no deallocation needed

    // set-tempo event
    DWORD getTempo() const;
    double getTempoBPM() const;

    // time-signature event
    byte getNumerator() const;
    byte getDenominator() const;
	byte getMetronomePer24Clocks() const;
	byte get32ndNotesPer24Clocks() const;

};


// Vector of MIDI events
typedef std::vector<FmMidiEvent*> Vector_pMidiEvent;


#endif
