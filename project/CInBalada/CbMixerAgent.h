
#ifndef __FMMIXERAGENT_H___
#define __FMMIXERAGENT_H___

#include <syncobjs.hpp>

#include "FAMA\FAMA.h"
#include "FAMA\MIDI\FmMidiOutput.h"
#include "FAMA\MIDI\FmMidiFile.h"

#include "RhythmPattern\CbMeter.h"

#include "Util\SparseArray.h"
#include "TinyXml\tinyxml.h"


/**
 * Conductor service. These constants should be used to find the
 * mixer agent within the plataform.
 */
#define MIXER_AG_SERVC_TYPE  "Musical Output"
#define MIXER_AG_SERVC_NAME  "MIDI Output Service"

/* Tags for XML node "Phrase" */
#define XML_NODE_PHRASE             "Phrase"
#define XML_ATTR_PHRASE_RESOLUTION  "resolution"
#define XML_ATTR_PHRASE_PATCH       "patch"

/* Tags for XML node "Note" */
#define XML_NODE_NOTE           "Note"
#define XML_ATTR_NOTE_PITCH     "pitch"
#define XML_ATTR_NOTE_DYNAMIC   "dynamic"
#define XML_ATTR_NOTE_STRTICK   "startTick"
#define XML_ATTR_NOTE_ENDTICK   "endTick"

/* Exceptions */
#define EXCP_MIXER_PROTOCOL     "Mixer Agent: Protocol broken !"       // remover !?
#define EXCP_INVALID_XML        "Mixer Agent: Invalid XML message !"   // remover !?

/* Language of mixer-agent messages */
#define MIXER_LANGUAGE               "mixer-agent-sl"

/* Messages to Mixer-Agent */
#define MSG_REQ_REG_MUSPRODUCER      "register as music producer"
#define MSG_REQ_REG_MUSLISTENER_OF   "register as music listener of "
#define MSG_REQ_UNREG_MUSPRODUCER    "unregister as music producer"
#define MSG_REQ_UNREG_MUSLISTENER_OF "unregister as music listener of "

/* Messages from Mixer-Agent */
#define MSG_INF_MIXER_START          "sync"

/**
 * Auxiliary methods for writing and reading musical phrases from/to XML.
 * Both methods return NULL when an error occurs in reading/writing.
 */
extern TiXmlElement* writeXmlPhrase(const FmMidiFileTrack& midifile, int startTick=0, int endTick=-1);
extern FmMidiFileTrack* readXmlPhrase(const TiXmlElement& el);

class CbMixerAgent;

// events invoked by the mixer-agent
typedef void (__closure *CbMixerEvent)(CbMixerAgent*);

// execution (sequencing) status
typedef enum {ES_PLAYING, ES_PAUSED, ES_STOPPED} ECbExecutionStatus;


/**
 * This class implements an agent that outputs musical information (represented
 * as MIDI events) to the file system and to soundcard. The "mixer-agent" is
 * responsible for distributing the music produced to any agents interested.
 *
 * Only one mixer-agent may exist in the plataform. To find the mixer-agent,
 * agents in the plataform have to search the service defined by constants
 * MIXER_AG_SERVC_TYPE and MIXER_AG_SERVC_NAME.
 *
 * An agent may register itself in the mixer agent as a 'music producer' or
 * 'music listener'. A 'music listener' is asked to send (periodically) musical
 * phrases to the mixer-agent, while a 'music listener' receives messages with
 * the phrases produced by a specific agent.
 */
class CbMixerAgent : public FmAgent {
protected:

	// array of notes to be sequenced
	typedef SparseArray<Vector_pMidiEvent*> ArrayNotes;

	/* Double-buffer of MIDI events. Used for sequencing.
	 */
	class SequencerBuffer {
	private:
		ArrayNotes* buffer[2];                 // buffer de notas, cada buffer guarda um compasso completo
		int currBuffer;                        // buffer atual, em sequenciamento
	public:
		SequencerBuffer();
		virtual ~SequencerBuffer();
		inline ArrayNotes* getCurrBuffer();
		inline ArrayNotes* getNextBuffer();
		inline void changeBuffer();
		inline void clear();
		void addPhrase(const FmMidiFileTrack& track, bool current = true);
	};

	/* Information about music producers.
	 */
	struct ProducerInfo {
		int track;              // track of midi file, when recording
		Vector_AID listeners;   // listeners of the agent
		ProducerInfo() : track(-1)
		{}
	};

	std::map<AID, ProducerInfo*>  players;    // map player agents to their indexes (tracks)

	FmMidiOutput* midiOutput;                     // MIDI output (sound card)
	SequencerBuffer sequencerBuffer;             // buffer of MIDI events to be played
	ECbExecutionStatus executionStatus;
	bool muted;

	bool         recording;
	FmMidiFile*  fileOutput;      // MIDI file where the whole performance is recorded

	TCriticalSection* semaphore;  // used to block simultaneous access

	int       resolution;         // number of ticks per beat

	UINT      timerId;
	int       timerPeriod;        // time interval between ticks (related to tempo)
	int       timerResolution;

	CbMeter*  meter;              // indicates the period of the sequencer

	volatile DWORD  currentTick;
	volatile int    currentBar;
	int             maxBars;

	bool __fastcall startMMTimer();
	bool __fastcall stopMMTimer();

	static void CALLBACK timerCallBack(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	virtual void __fastcall executeMidiTick();
	void __fastcall executeEvents(FmMidiOutput* out, const Vector_pMidiEvent* evts);

	virtual void __fastcall registerPlayer(FmAclMessage* msg);
	virtual void __fastcall unregisterPlayer(FmAclMessage* msg);
	virtual void __fastcall registerListener(FmAclMessage* msg);
	virtual void __fastcall unregisterListener(FmAclMessage* msg);
	virtual void __fastcall collateBars(FmAclMessage* msg);

	virtual void __fastcall informStart();
	virtual void __fastcall informNextBar();

    /**
	 * Implements the "MIDI Output Service".
     */
    class MixerBehaviour : public FmCyclicBehaviour {
    private:
        CbMixerAgent* mixer;
    public:
        MixerBehaviour(CbMixerAgent* agent);
        void action();
    };

    friend class MixerBehaviour;

public:

	/* The agent is created in 'active' state and remains active untill
	 * the container is finished (when the application is closed). It is,
	 * then, automatically deleted.
	 */
	CbMixerAgent();
	virtual ~CbMixerAgent();

	CbMixerEvent OnSetup;
	CbMixerEvent OnStatusChange;

	virtual void setup();
	virtual void takeDown();

	/* Methods to read the current point of the sequencing.
	 */
	int getCurrentBar();
	int getCurrentTick();

	/* This method changes the tempo. Values of 't' are given in beats
	 * (quarter-notes) per minute.
	 */
	void setTempo(int t);
	int getTempo();

	/* Ticks per beat.
	 */
	int getResolution();
	void setResolution(int res);

	/* Methods to get/set the meter. Meter can only be set
	 * when sequencing status is ES_STOPPED.
	 */
	const CbMeter* getMeter();
	void setMeter(const CbMeter& mtr);

	/* Methods to get/set the number of bars that the sequencer
	 * plays before automatically stopping. Value -1 indicates that
	 * the sequencer will stop only when 'stop' method is called.
	 */
	int getBarsCycle();
	void setBarsCycle(int n);

	/* Starts sequencing and playing. If parameter 'numBars' is specified,
	 * mixer will stop sequencing after the given number of bars is played.
	 */
	void start(bool recording = false);

	/* Stops playing, sequencing and recording. When stopped, agents still
	 * can register/unregister, but they won't be asked to send phrases.
	 * When resumed (calling 'play'), the mixer will start from the beginning.
	 */
	void stop();

	/* Temoporarily stops sequencing. Sequencing process can be resumed
	 * from the point it was stopped.
	 */
	void pause();

	/* Returns the status of the sequencing process.
	 */
	ECbExecutionStatus getStatus();

	/* Stops playing (in soundcard), but not sequencing. Agents are still
	 * asked to send phrases and the output is still recorded.
	 */
	void toggleMute();
	bool isMuted();

	/* Methods to record MIDI events file. When recording is stopped, the
	 * MIDI file can be retrieved calling 'getRecord' (deallocation of
	 * result is done by this class).
	 */
	void startRecording();
	void stopRecording();
	bool isRecording();
	FmMidiFile* getRecord();

	/* Returns (in the parameter 'resultList') the names of available
	 * MIDI output drivers.
	 */
	void getDriversList(TStrings &resultList);
	int getNumDrivers();
	AnsiString getDriverName(int i);

	/* Methods to reads/set the MIDI output driver. The driver index is the index
	 * of the name of the driver in the list returned by 'getDriversList()'.
	 */
	int getDriver();
	void setDriver(int driverIndex);

};


#endif
