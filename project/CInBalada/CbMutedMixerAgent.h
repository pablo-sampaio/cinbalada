
#ifndef __CB_MUTED_MIXERAGENT_H___
#define __CB_MUTED_MIXERAGENT_H___

#include "CbMixerAgent.h"


/**
 * This class implements a mixer agent that outputs musical information (represented
 * as MIDI events). It does the same that a CbMixerAgent does, except by the fact that
 * a CbMutedMixerAgent doesn't output to soundcard.
 */
class CbMutedMixerAgent : public CbMixerAgent {
protected:

	int pendingAnswers;

	// overridings to CbMixer functions
	virtual void __fastcall executeMidiTick();
	virtual void __fastcall collateBars(FmAclMessage* msg);
	virtual void __fastcall informNextBar();

public:

	CbMutedMixerAgent();
	virtual ~CbMutedMixerAgent();

	virtual void setup();
	virtual void takeDown();

};


#endif
