//-----------------------------------------------------------------------------
//
// MaxMidiBuilder C++ Builder Components
// 
// Kevin Goodman (goodmank@magi.com)
// Ottawa, Canada
//
// This software is heavily based on MFC classes provided with:
//
// Maximum MIDI Programmer's ToolKit
//
// Copyright (c) Paul A. Messick, 1994-1996
//
// Written by Paul A. Messick
//
//-----------------------------------------------------------------------------
#ifndef MaxMidiSyncH
#define MaxMidiSyncH
//---------------------------------------------------------------------------
#include <vcl\SysUtils.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Forms.hpp>

#include "MaxMidiDLL.h"
//---------------------------------------------------------------------------

class TMaxMidiSync : public TComponent
{
private:
  TNotifyEvent FProcessMidiBeatEvent;
  TNotifyEvent FProcessSyncDoneEvent;

  HWND m_hWnd;
  void __fastcall ControlWndProc( TMessage &Message );

protected:
  TMaxMidi* FMaxMidi;

	HSYNC		m_hDevice;		// handle to the Sync device
	bool		m_fIsOpen;		// true if device is open
	WORD		m_CurrentMode;	// current sync mode
	WORD		m_CurrentPeriod;	// current timer period
	bool		m_fRunning;		// true is sync is active

public:
	__fastcall TMaxMidiSync( TComponent* Owner );
	__fastcall ~TMaxMidiSync();			// destructor

// Implementation
	bool IsOpen();			// returns true if device is open
	bool IsRunning() { return m_fRunning; };
	HSYNC GetHSync() { return m_hDevice; };

	bool Open( WORD mode = S_INT, WORD timerPeriod = 10 );
	void Close();			// close the device without destroying class object

	void Mode( WORD mode );		// set new sync mode
	WORD Mode() { return m_CurrentMode; };
	void Period( WORD period );	// set new timer period
	WORD Period() { return m_CurrentPeriod; };

	void Start();			// start sync
	void ReStart();			// restart sync after pause
	void Stop();			// stop sync
	void Pause( bool reset = FALSE ); // pause sync, send note offs if reset = true

	bool Tempo( DWORD tempo );	// set the tempo in uS/beat
	DWORD Tempo();			// get the current tempo in uS/beat
	DWORD ConvertFromBPM( double tempo ); // convert bpm to uS/beat
	double ConvertToBPM( DWORD tempo );   // convert uS/beat to bpm

	WORD Resolution();		// get the current resolution in tpb
	void Resolution( WORD res );	// set the current resolution in tpb

__published:
  __property TMaxMidi* MaxMidi        = { read = FMaxMidi, write = FMaxMidi };
  __property TNotifyEvent OnMidiBeat  = { read = FProcessMidiBeatEvent, write = FProcessMidiBeatEvent };
  __property TNotifyEvent OnSyncDone  = { read = FProcessSyncDoneEvent, write = FProcessSyncDoneEvent };
};
//---------------------------------------------------------------------------
#endif
