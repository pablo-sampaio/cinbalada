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
#ifndef MaxMidiTrackH
#define MaxMidiTrackH
//---------------------------------------------------------------------------
#include <vcl\SysUtils.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Forms.hpp>

#include "MaxMidiDLL.h"

#define DEFAULT_BUFFER_SIZE 8192
#define BUFFER_GROW_SIZE DEFAULT_BUFFER_SIZE
//---------------------------------------------------------------------------

class TMaxMidiSMF;
class TMaxMidiIn;


class TMaxMidiTrack
{
private:
protected:
#ifdef _SMF
	TMaxMidiSMF* FSMF;				// SMF connected to this track
#endif
#ifdef _MIDIIN
	TMaxMidiIn*  FMidiIn;			// MidiIn device connected to this track
#endif

	LPMIDIEVENT m_lpBuffer;			// buffer for events
	DWORD		m_dwBufSize;			// size of buffer, in events
	DWORD		m_inPtr;				// buffer write index
	DWORD		m_outPtr;				// buffer read index
	bool		m_fRecord;			// true if recording into this track
	bool		m_fMute;				// true if playback is muted
  AnsiString m_sName;			// track name string

public:
	__fastcall TMaxMidiTrack();
  __fastcall ~TMaxMidiTrack();				// destructor

// Implementation
// smf-dependent functions
#ifdef _SMF
	void Attach(TMaxMidiSMF* pSMF);
	void Detach(TMaxMidiSMF* pSMF);
#endif

// midi in-dependent functions
#ifdef _MIDIIN
	void Attach(TMaxMidiIn* pMidiIn);
	void Detach(TMaxMidiIn* pMidiIn);
	TMaxMidiIn* GetMidiIn() { return FMidiIn; };
#endif

	void Detach();

	bool IsEmpty() { return (m_inPtr - m_outPtr) == 0; };
	bool IsRecording() { return m_fRecord; };
	void IsRecording(bool record) { m_fRecord = record; };

	void Mute(bool mute) { m_fMute = mute; };
	bool Mute() { return m_fMute; };

	AnsiString GetTrackName();
	void SetTrackName( const AnsiString &sName );

	DWORD GetNumEvents() { return (DWORD)(m_inPtr - m_outPtr); };
	void SetNumEvents(DWORD nEvents) { Flush(); m_inPtr = nEvents; };

	bool CreateBuffer(DWORD dwBufEvents = DEFAULT_BUFFER_SIZE);
	LPMIDIEVENT GetBuffer() { return m_lpBuffer; };
	void SetBuffer(LPMIDIEVENT lpNewBuf) { m_lpBuffer = lpNewBuf; };
	DWORD GetBufferSize() { return m_dwBufSize; };
	void SetBufferSize(DWORD dwBufEvents) { m_dwBufSize = dwBufEvents; };
	void FreeBuffer();

	LPMIDIEVENT GetEvent(DWORD eventNum);
	void SetEvent(LPMIDIEVENT lpEvent, DWORD eventNum);
	DWORD GetTime(DWORD eventNum);

	LPMIDIEVENT Read();
	void Write(LPMIDIEVENT lpEvent);
	void Flush() { m_inPtr = m_outPtr = 0; };
	void Rewind() { m_outPtr = 0; };

	bool Load();
	bool Save();

	LPMIDIEVENT GetAbsBuffer(DWORD startEvent, DWORD* numEvents);
	DWORD AbsNow(DWORD eventNum);
	void AbsToDelta(LPMIDIEVENT lpBuf, DWORD startEvent, DWORD numEvents);
	void DeltaToAbs(LPMIDIEVENT lpBuf, DWORD startEvent, DWORD numEvents);

	void InsertEvent(LPMIDIEVENT lpEvent, DWORD beforeEvent);
	void DeleteEvent(DWORD eventNum);
	void SlideTrack(DWORD eventNum, int delta);

#if 0
__published:
#ifdef _SMF
	__property TMaxMidiSMF* SMF = { read = FSMF, write = FSMF };
#endif
#ifdef _MIDIIN
	__property TMaxMidiIn*  MidiIn = { read = FMidiIn, write = FMidiIn };
#endif
#endif
};
//---------------------------------------------------------------------------
#endif
