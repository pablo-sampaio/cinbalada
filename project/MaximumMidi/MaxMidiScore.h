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
#ifndef MaxMidiTrackListH
#define MaxMidiTrackListH
//---------------------------------------------------------------------------

#include <vcl\SysUtils.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Forms.hpp>

#include "MaxMidiTrack.h"

class TMaxMidiOut;

//---------------------------------------------------------------------------

class TMaxMidiScore
{
  TList*      m_pTrackList;
  
  LPMIDIEVENT m_lpMerge;    // pointer to the merged output data
  DWORD       m_numEvents;  // number of events in the merged buffer
  DWORD       m_outPtr;     // index to retrieve next event from merge buffer
  DWORD       m_LastAbs;    // last absolute timestamp used during track merge

  int __fastcall getCount() { return m_pTrackList->Count; }

protected:
	TMaxMidiTrack* __fastcall Get( int Index );
	void __fastcall Put( int Index, TMaxMidiTrack* Item );

  LPMIDIEVENT MergeTracks();

public:
  __fastcall TMaxMidiScore( int numTracks = 0 );
  __fastcall ~TMaxMidiScore();

  void Clear(void);
  void Insert(int Index, TMaxMidiTrack* Item);
            
  int Add( TMaxMidiTrack* pTrack );
  int Remove( TMaxMidiTrack* pTrack );

  int Find( TMaxMidiTrack* pTrack );

  void MergeOut( TMaxMidiOut* pMaxMidiOut );
  void StartOut( TMaxMidiOut* pMaxMidiOut );

	__property int Count = { read = getCount };
	__property TMaxMidiTrack* Tracks[ int Index ] = { read = Get, write = Put };
};

#endif
