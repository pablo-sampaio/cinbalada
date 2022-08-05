
#ifndef __FMMIDIFILE_H__
#define __FMMIDIFILE_H__

#include "FmMidiEvent.h"

#include "MaximumMidi\maxmidi.h"

#include <map>
#include <iostream>


// default value of resolution in ticks per beat
#define DEFAULT_MIDIFILE_RESOLUTION  16

// converts a tick value 't', given in resolution 'from', to a tick value in resolution 'to'
#define CONVERTTICK(t,from,to)   ( (from) == (to) ? (t) : ((t)*(to))/(from) )


/**
 * Representation of a midi file track.
 */
class FmMidiFileTrack {
private:
    std::map<DWORD,Vector_pMidiEvent*> events;

    DWORD maxTick;
    int countEvents;
    int resolution;

public:
	FmMidiFileTrack(int __resolution=DEFAULT_MIDIFILE_RESOLUTION);

    virtual ~FmMidiFileTrack();

    /* Returns the events that occur at a certain tick. May return NULL.
     * {deallocation of result}: this class
     */
    const Vector_pMidiEvent* getEvents(int tick) const;

    /* Returns all events ordered by their ticks.
     * {deallocation}: parameter 'result' -> user,
     *                 events in 'result' -> this class
     */
    void getAllEvents(Vector_pMidiEvent& result) const;

    /* {deallocation of parameter}: this class
     */
    void addEvent(FmMidiEvent* ev);

    /* Maximum value reached by the ticks of the events.
     */
    DWORD getMaxTick() const;

    /* Resolution in ticks per beat (quarter-note).
     */
    int getResolution() const;

    /* Sets the resolution and converts all events to it.
     */
    void setResolution(int res);

    /* Shifts the time (tick) of all events by the given
     * offset (in number of ticks).
     */
    void shiftEvents(int offsetTicks);

    /* Adds the events of the given source track to 'this' track.
	 * Optionally, the ticks of the source track can be shifted by an
	 * offset, given in number of ticks of the current track (this).
	 */
    void merge(const FmMidiFileTrack& src, int offsetTicks=0);

    /* Number of beats (quarter-notes).
     */
    int getNumQuarterNotes() const;

    /* Number of events.
     */
	int getNumEvents() const;

	/* Truncate the track so that events with tick times higher
	 * than 'maxTick' are lost.
	 */
	void truncate(int maxTick);

   	friend std::ostream& operator<<(std::ostream& o, const FmMidiFileTrack& t);

};


/**
 * This class is used to manipulate MIDI files.
 */
class FmMidiFile {
private:

    std::vector<FmMidiFileTrack*> tracks;

    int   resolution;  // ticks per beat
    DWORD totalTicks;  // maximum number of ticks (in all tracks)

    int numerator;     // numerator of time signature
    int denominator;   // denominator of time signature, as expoent of 2

    int tempo;         // in bpm

	void loadFromFile(const AnsiString& midiFile);
	FmMidiEvent* convertToFmMidiEvent(LPMIDIEVENT event, DWORD tick);
  	LPMIDIEVENT convertToMaxMidiEvent(FmMidiChannelEvent* event, DWORD tick);
	LPMIDIEVENT convertToMaxMidiEvent(FmMidiMetaEvent* event, DWORD tick);
	void loadMaxMidiScore(TMaxMidiScore& score, TMaxMidiSMF& midiSMF);

    void free();

public:

    /* Creates a FmSimpleMidi with the given (maximum) number of tracks and,
	 * optionally, with the given resolution (as ticks per quarter-note).
	 */
	FmMidiFile(int numTracks=2, int resolution=DEFAULT_MIDIFILE_RESOLUTION);

    /* Creates a FmSimpleMidi from a MIDI file, converting it to the
     * given resolution. If resolution is set to -1, the MIDI file resolution
     * is used, instead.
     */
	FmMidiFile(const char* file, int resolution=-1);

    /* Destructor. Destroys allocated data.
     */
    virtual ~FmMidiFile();

    /* Loads a MIDI file, converting its events to the given resolution.
     * If resolution is set to -1, the MIDI file reoslution is used.
     * All previous information in the FmSimpleMidiFile is destroyed.
     */
	void loadFile(const char* midiFile, int resolution=-1);

    /* Saves all events to a MIDI file.
     */
    void saveToFile(const char* midiFile);

	/* Creates a new track after existing tracks. Returns the index
	 * of the created track.
     */
    int createTrack();

    /* Adds a midi event (midi message) to the given track.
     * {deallocation of parameter}: caller
     */
	void addEvent(const FmMidiEvent* event, int track, int offsetTicks=0);

	/* Loads a set of midi events into the track indicated by 'desTrack'.
	 * Optionally, the ticks of the source track can be shifted by an
	 * offset, given in number of ticks of the current file (this).
	 * Previous events in the track indicated by 'destTrack' are not lost.
	 */
	void addEvents(const FmMidiFileTrack& phrase, int desTrack, int offsetTicks=0);

    /* Returns all events of the track occuring in the given tick.
     */
	void getMidiEvents(DWORD tick, int track, Vector_pMidiEvent& result) const;

    /* Returns all events, in all tracks, that occur in the given tick.
     */
	void getMidiEvents(DWORD tick, Vector_pMidiEvent& result) const;

	/* Maximum tick found in the events of MIDI file.
	 */
	DWORD getMaxTick() const;

	/* Erases events after 'maxTick'.
	 */
	void truncate(int maxTick);

	/* Number of tracks.
	 */
	int getNumTracks() const;

	/* Returns a pointer to the track. The tracks should not be directly
	 * changed. Use FmMidiFile methods instead.
	 */
	const FmMidiFileTrack* getTrack(int i) const;

	/* Number of ticks per beat (quarter-note).
	 */
	int getResolution() const;

	/* Changes the resolution, converting all events to the new resolution.
	 */
	void setResolution(int res);

	/* Number of quarter-notes.
	 */
	int getNumQuarterNotes() const;

	/* Number of bars.
	 */
	int getNumBars() const;

	/* Numerator of time signture.
	 */
	int getNumerator() const;
	void setNumerator(int n);

	/* Denominator of time signature. Given as an expoent in base 2
	 * (e.g., quarter-note is notated as '2' and eight-note as '3').
	 */
	int getDenominator() const;
	void setDenominator(int d);

	/* Tempo in beats per minute.
     */
	int getTempo() const;

	/* Print to the log file (for debugging purposes).
	 */
	//void printLog();

};


#endif
