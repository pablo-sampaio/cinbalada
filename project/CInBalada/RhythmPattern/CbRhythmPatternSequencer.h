
#ifndef __CBRHYTHMPATTERNSEQ_H___
#define __CBRHYTHMPATTERNSEQ_H___

#include <syncobjs.hpp>

#include "CbRhythmPattern.h"

#include "FAMA\MIDI\FmMidiOutput.h"
#include "FAMA\MIDI\FmMidiFile.h"

class CbRhythmPatternSequencer;
typedef void (__closure *CbRPSequencerEvent)(CbRhythmPatternSequencer* seq);

/**
 * Sequencer for rhythm pattern. Rhythm pattern are played in loop, synchronized
 * by the longest of them (in number of bars).
 */
class CbRhythmPatternSequencer {
protected:

	static CbRhythmPatternSequencer* instance;
	//friend class SmartCleaner;

	/* This class is used to automatically free the
	 * instance when the program finishes.
	 */
	class SmartCleaner {
	public:
		virtual ~SmartCleaner();
	};
	friend class SmartCleaner;

	static SmartCleaner smartCleaner;

	/* Keeps 'sequencing information' about a rhythm pattern.
	 */
    struct RpSeqInfo {
        const CbRhythmPattern* pattern;
		bool paused;
		RpSeqInfo(const CbRhythmPattern* rp, bool pause)
			: pattern(rp), paused(pause)
		{}
	};

	FmMidiOutput* saida;
    std::vector<RpSeqInfo*> midis;
    int numPatterns;

    int currentTick;
	int resolution;
	int beatsAlignment;

	TCriticalSection *semaphore;

	UINT      timerId;
	int       timerPeriod;       // time interval between ticks (related to tempo)
	int       timerResolution;   // resolution (precision) of the timer


	static void CALLBACK timerCallBack(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	void executeTick();

    /**
     * Private constructor. Only one construtor may exist. The (only)
     * instance of the sequencer can only be acessed by 'getSequencer()'.
     */
    CbRhythmPatternSequencer();
    CbRhythmPatternSequencer(const CbRhythmPatternSequencer&);

public:

	virtual ~CbRhythmPatternSequencer();

    static CbRhythmPatternSequencer* getSequencer();

	/* Callback function to be called on tick events of the sequencer.
	 */
	CbRPSequencerEvent OnTick;

	/* Adds a rhythm pattern in the sequencer and returns the index that
	 * identifies it inside the sequencer.
	 */
	int add(const CbRhythmPattern* midifile, bool paused=false);

	/* Removes the rhythm pattern in the given index.
	 */
	void remove(int index);

	/* Removes all rhythm patterns.
	 */
	void removeAll();

	/* Returns the number of rhythm patterns currently being played.
	 */
	int getNumPatterns();

	/* Returns the rhythm pattern in the given position (paramenter 'index'
	 * in the range from zero to the number of files minus one).
	 */
	const CbRhythmPattern* getPattern(int index);

	/* Pause a rhythm pattern currently being played.
	 */
	void pausePattern(int index);

	/* Resumes a rhythm pattern that was paused.
	 */
	void resumePattern(int index);

	/* Starts/Stops the synchronizer. When the sequencer is stopped,
	 * the patterns previously added are kept in the sequencer and played
	 * again when the sequencer is restarted.
	 */
	void start();
	void stop();

	bool hasStarted();

	int getCurrentTick();
    int getResolution();

	int getTempo();
	void setTempo(int t);

};

#endif /* __CBRHYTHMPATTERNSEQ_H___ */
