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
#ifndef MaxMidiSMFH
#define MaxMidiSMFH
//---------------------------------------------------------------------------
#include <vcl\SysUtils.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Forms.hpp>

#include "MaxMidiDLL.h"
//---------------------------------------------------------------------------

#define READ 'r'
#define WRITE 'w'

class TMaxMidiScore;
class TMaxMidiTrack;
class TMaxMidiSync;


class TMaxMidiSMF : public TComponent
{
private:
protected:
  TMaxMidi* FMaxMidi;

	HMIN	m_hSMF;				// handle to the SMF
	bool	m_fIsOpen;			// true if device is open
	char	m_mode;				// 'r' for read, 'w' for write
	int		m_format;				// SMF format type
	int		m_nTracksInSMF;		// number of tracks in SMF

  TMaxMidiScore* m_pScore;

public:
	__fastcall TMaxMidiSMF(TComponent* Owner);
//	__fastcall TMaxMidiSMF( const AnsiString& rsfilename, const char Mode);
	__fastcall ~TMaxMidiSMF();

// Implementation
	void Attach( TMaxMidiScore* pScore );
	bool Detach( TMaxMidiScore* pScore );

	bool IsOpen() { return m_fIsOpen; };	// returns true if device is open
	int NumTracks() { return m_nTracksInSMF; };
	int GetFormat() { return m_format; };
	char GetMode() { return m_mode; };

	bool Open(const AnsiString& rsFilename, const char Mode, int Format = 1);
	void Close();			// close the device without destroying class object

	WORD Resolution() { return FMaxMidi->GetSMFResolution(m_hSMF); };
	WORD Resolution(WORD res) { return FMaxMidi->SetSMFResolution(m_hSMF, res); };
	virtual bool Read(TMaxMidiTrack* pTrack);
	virtual bool Write(TMaxMidiTrack* pTrack);
	DWORD ReadMeta(TMaxMidiTrack* pTrack, BYTE type, LPSTR* Value, DWORD* cbSize);
	bool WriteMeta(TMaxMidiTrack* pTrack, BYTE type, LPSTR Value, DWORD time);
	bool Rewind() { return FMaxMidi->RewindSMF(m_hSMF) == 0; };

	bool Load( TMaxMidiSync* pSync );
	bool Save( TMaxMidiSync* pSync );

__published:
  __property TMaxMidi* MaxMidi = { read = FMaxMidi, write = FMaxMidi };
};
#endif
