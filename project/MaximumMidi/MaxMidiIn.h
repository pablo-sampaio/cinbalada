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
#ifndef MaxMidiInH
#define MaxMidiInH
//---------------------------------------------------------------------------
#include <vcl\SysUtils.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Forms.hpp>

#include "MaxMidiDLL.h"
#include "MaxMidiSync.h"

//#define MAXIN_CLOSE_EVENT 
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TProcessMidiDataEvent)( LPMIDIEVENT Event, bool* pbAccept );

class TMaxMidiTrack;


class TMaxMidiIn : public TComponent
{
private:
  TMaxMidi*      FMaxMidi;
#ifdef _SYNC
  TMaxMidiSync* FSync;
#endif

  HMIN       m_hDevice;
  DWORD      m_dwFlags;
  WORD       m_wDeviceID;
  bool       m_bIsOpen;
  AnsiString m_sDesc;
  bool       m_bIsStarted;

  int        m_timestamp;

#ifdef _TRACK
  TMaxMidiTrack* m_pTrack;		// track object associated with this input (only one)
#endif
  HWND m_hWnd;
  void __fastcall ControlWndProc( TMessage &Message );

  TProcessMidiDataEvent FProcessMidiDataEvent;
#ifdef MAXIN_CLOSE_EVENT
  TNotifyEvent          FProcessMidiCloseEvent;
#endif

protected:
  LPMIDIEVENT Get();	// get received event, if any

public:
  __fastcall TMaxMidiIn(TComponent* Owner);
  __fastcall ~TMaxMidiIn();

// sync-dependent functions
#ifdef _SYNC
  void Attach( TMaxMidiSync* pSync );
  void Detach( TMaxMidiSync* pSync );
  TMaxMidiSync* GetSync(void) { return FSync; };
#endif

// track-dependent functions
#ifdef _TRACK
  void Attach( TMaxMidiTrack* pTrack );
  void Detach( TMaxMidiTrack* pTrack );
#endif

// Implementation
  WORD GetIDFromName(const AnsiString& sDesc); // find corresponding ID given string name

  bool  IsOpen();
  AnsiString GetDescription();
  int   GetNumDevices();

  bool Open( WORD wDeviceID, DWORD dwFlags = MIDIIN_DEFAULT );
  void Close();			// close the device without destroying class object

  void Start();
  void Stop();
  void Reset();			// reset the timestamp to zero, if started

__published:
  __property TMaxMidi* MaxMidi = { read = FMaxMidi, write = FMaxMidi };
#ifdef _SYNC
  __property TMaxMidiSync* Sync = { read = FSync, write = FSync };
#endif
  __property TProcessMidiDataEvent OnMidiData   = { read = FProcessMidiDataEvent, write = FProcessMidiDataEvent };
#ifdef MAXIN_CLOSE_EVENT
  __property TNotifyEvent          OnMidiClose  = { read = FProcessMidiCloseEvent, write = FProcessMidiCloseEvent };
#endif

};
//---------------------------------------------------------------------------
#endif
