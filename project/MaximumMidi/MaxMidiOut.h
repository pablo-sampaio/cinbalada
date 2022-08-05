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
#ifndef MaxMidiOutH
#define MaxMidiOutH
//---------------------------------------------------------------------------
#include <vcl\SysUtils.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Forms.hpp>

#include "MaxMidiDLL.h"
#include "MaxMidiSync.h"

//#define MAXOUT_CLOSE_EVENT
//---------------------------------------------------------------------------

class TMaxMidiScore;


class TMaxMidiOut : public TComponent
{
private:
  TMaxMidi* FMaxMidi;
#ifdef _SYNC
  TMaxMidiSync* FSync;    // the sync device object
#endif

  HMOUT		m_hDevice;        // handle to the MidiOut device
  DWORD		m_dwFlags;        // current flags, in case we reopen
  WORD		m_wDeviceID;      // device ID of attached device

  bool		m_bIsOpen;        // true if device is open
  AnsiString m_sDesc;

#ifdef _TRACK
  TMaxMidiScore* m_pScore;
#endif

  HWND m_hWnd;
  void __fastcall ControlWndProc( TMessage &Message );

  TNotifyEvent FProcessOutBufferReadyEvent;
#ifdef MAXOUT_CLOSE_EVENT
  TNotifyEvent FProcessMidiCloseEvent;
#endif

public:
  __fastcall TMaxMidiOut(TComponent* Owner);
  __fastcall ~TMaxMidiOut();

// sync-dependent functions
#ifdef _SYNC
  void Attach( TMaxMidiSync* pSync );
  void Detach( TMaxMidiSync* pSync );
  TMaxMidiSync* GetSync() { return FSync; };
#endif

// track-dependent functions
#ifdef _TRACK
	void Attach( TMaxMidiScore* pTrackList );
	void Detach( TMaxMidiScore* pTrackList );
#endif

// Implementation
  WORD GetIDFromName(const AnsiString& sDesc); // find corresponding ID given string name

  bool IsOpen();			// returns true if device is open
  AnsiString GetDescription();
  int GetNumDevices();	// returns number of output devices available

  bool Open( WORD wDeviceID, DWORD dwFlags = MIDIOUT_DEFAULT );
  void Close();			// close the device without destroying class object

  bool Put( LPMIDIEVENT lpEvent ); // output an event
  void Reset();			// reset the output device
  void Flush();			// flush the output queue

__published:
  __property TMaxMidi* MaxMidi = { read = FMaxMidi, write = FMaxMidi };
#ifdef _SYNC
  __property TMaxMidiSync* Sync = { read = FSync, write = FSync };
#endif
  __property TNotifyEvent OutBufferReady = { read = FProcessOutBufferReadyEvent, write = FProcessOutBufferReadyEvent };
#ifdef MAXOUT_CLOSE_EVENT
  __property TNotifyEvent OnMidiClose    = { read = FProcessMidiCloseEvent, write = FProcessMidiCloseEvent };
#endif
};
//---------------------------------------------------------------------------
#endif
