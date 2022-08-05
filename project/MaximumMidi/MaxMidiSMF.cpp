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
#include <vcl\vcl.h>
#pragma hdrstop

#include "MaxMidiSMF.h"
#include "MaxMidiTrack.h"
#include "MaxMidiScore.h"
#include "MaxMidiIn.h"
#include "MaxMidiOut.h"
//---------------------------------------------------------------------------
__fastcall TMaxMidiSMF::TMaxMidiSMF( TComponent* Owner )
: TComponent( Owner )
{
	m_fIsOpen = FALSE;
	m_nTracksInSMF = 0;
	m_pScore = NULL;
}
//---------------------------------------------------------------------------
#if 0
//Must add MaxMidi to parameter list
__fastcall TMaxMidiSMF::TMaxMidiSMF( const AnsiString& rsfilename, const char Mode)
{
	m_fIsOpen = FALSE;
	m_nTracksInSMF = 0;
	m_pScore = NULL;

	Open(rsfilename, Mode);
}
#endif
//---------------------------------------------------------------------------
__fastcall TMaxMidiSMF::~TMaxMidiSMF()
{
	if( m_fIsOpen )
  {
    Close();
  }
}
//---------------------------------------------------------------------------
void TMaxMidiSMF::Attach( TMaxMidiScore* pScore )
{
  m_pScore = pScore;
}
//---------------------------------------------------------------------------
bool TMaxMidiSMF::Detach( TMaxMidiScore* pScore )
{
  if( m_pScore != pScore )
  {
    return( false );
  }

  m_pScore = NULL;
  return( true );
}
//---------------------------------------------------------------------------
bool TMaxMidiSMF::Open(const AnsiString& rsFilename, const char Mode, int Format)
{
  if( !FMaxMidi )
  {
    throw EMaxMidiException( "No MaxMidi" );
  }

	// make sure there are no open files attached to this object
	if( m_fIsOpen )
  {
    Close();
  }

	// open the file
	m_mode = Mode;
	m_format = Format;
	if((m_hSMF = FMaxMidi->OpenSMF(rsFilename.c_str(), &m_format, m_mode, &m_nTracksInSMF)) != 0)
		m_fIsOpen = TRUE;

	return m_fIsOpen;
}
//---------------------------------------------------------------------------
void TMaxMidiSMF::Close()
{
  if( !m_fIsOpen )
  {
    throw EMaxMidiException( "SMF not open" );
  }

  FMaxMidi->CloseSMF(m_hSMF);

  // detach any tracks from this file
  if( m_pScore )
  {
    m_pScore = NULL;
  }

  m_nTracksInSMF = 0;

	m_fIsOpen = FALSE;
}
//---------------------------------------------------------------------------
bool TMaxMidiSMF::Read(TMaxMidiTrack* pTrack)
{
	// only valid if opened for read
  if( !m_fIsOpen )
  {
    throw EMaxMidiException( "SMF not open" );
  }
  if( m_mode != READ || !m_pScore )
  {
    throw EMaxMidiException( "SMF cant read" );
  }

  // find the track number corresponding to this track
  int Track = m_pScore->Find( pTrack );
  if( Track == -1 )
  {
    throw EMaxMidiException( "SMF bad track" );
  }

	DWORD dwEventsRead = 0;
	DWORD dwRead;

  DWORD dwTrackSize = pTrack->GetBufferSize();
  LPMIDIEVENT pBuffer = pTrack->GetBuffer();

  do {
    // expand the track buffer, if necessary
    if(dwTrackSize - dwEventsRead < DEFAULT_BUFFER_SIZE)
    {
      dwTrackSize += dwEventsRead;
      pBuffer = (LPMIDIEVENT)GlobalReAlloc(pBuffer, dwTrackSize * sizeof(MidiEvent), GPTR | GMEM_MOVEABLE);
    }

    // read a block of events
    dwRead = FMaxMidi->ReadSMF(m_hSMF, Track, &pBuffer[dwEventsRead], DEFAULT_BUFFER_SIZE);
    dwEventsRead += dwRead;

  } while(dwRead);

  pTrack->SetNumEvents(dwEventsRead);
  pTrack->SetBufferSize(dwTrackSize);
  pTrack->SetBuffer(pBuffer);
  pTrack->GetTrackName();

	return dwEventsRead != 0;
}
//---------------------------------------------------------------------------
bool TMaxMidiSMF::Write(TMaxMidiTrack* pTrack)
{
	// only valid if opened for write
  if( m_mode != WRITE || !m_pScore )
  {
    throw EMaxMidiException( "SMF cant write" );
  }

  // find the track number corresponding to this track
  int Track = m_pScore->Find( pTrack );
  if( Track == -1 )
  {
    throw EMaxMidiException( "SMF bad track" );
  }

  AnsiString sName = pTrack->GetTrackName();
  if( !sName.IsEmpty() )
  {
    WriteMeta(pTrack, META_NAME, sName.c_str(), 0);
  }

  unsigned int numEventsWritten =
    FMaxMidi->WriteSMF(m_hSMF, Track, pTrack->GetBuffer(), pTrack->GetNumEvents());
  return( numEventsWritten == pTrack->GetNumEvents());
}
//---------------------------------------------------------------------------
bool TMaxMidiSMF::Save( TMaxMidiSync* pSync )
{
	// only valid if opened for write
  if( m_mode != WRITE || !m_pScore )
  {
    throw EMaxMidiException( "SMF cant write" );
  }

  if( pSync )
  {
    Resolution(pSync->Resolution());
  }

	// save all of the tracks that are attached
	for(int trackNum = 0; trackNum < m_pScore->Count; trackNum++)
  {
		if( !Write( m_pScore->Tracks[ trackNum ] ) )
    {
      throw EMaxMidiException( "SMF write failed" );
    }
  }

	return TRUE;
}
//---------------------------------------------------------------------------
bool TMaxMidiSMF::Load( TMaxMidiSync* pSync )
{
	// only valid if opened for read
  if( m_mode != READ || !m_pScore )
  {
    throw EMaxMidiException( "SMF cant read" );
  }

	// read all of the tracks that are attached
	FMaxMidi->RewindSMF(m_hSMF);
	for( int i = 0; i < m_pScore->Count; i++)
	{
		m_pScore->Tracks[i]->Flush();
		Read(m_pScore->Tracks[i]);
	}

  if( pSync )
  {
			pSync->Resolution(Resolution());
  }

	return TRUE;
}
//---------------------------------------------------------------------------
DWORD TMaxMidiSMF::ReadMeta(TMaxMidiTrack* pTrack, BYTE type, LPSTR* Value, DWORD* cbSize)
{
	LPSTR valbuf;
	DWORD valbufsize;
	DWORD time = (DWORD)-1;

	// only valid if opened for read
  if( m_mode != READ || !m_pScore )
  {
    throw EMaxMidiException( "SMF cant read" );
  }

  // find the track number corresponding to this track
  for(int Track = 0; Track < m_pScore->Count; Track++)
    if(m_pScore->Tracks[Track] == pTrack)
    {
      // look for the event in the track
      time = FMaxMidi->ReadMetaEvent(m_hSMF, Track, type, &valbuf, &valbufsize);
      if(time != (DWORD)-1)
      {
        // create a buffer for the event value and copy it over
        *Value = (LPSTR)GlobalAlloc(GPTR, valbufsize);
        *cbSize = valbufsize;
        memcpy(*Value, valbuf, valbufsize);
      } else {
      	*Value = NULL;
        *cbSize = -1;
      }
    }

	return time;
}
//---------------------------------------------------------------------------
bool TMaxMidiSMF::WriteMeta(TMaxMidiTrack* pTrack, BYTE type, LPSTR Value, DWORD time)
{
	UINT rc = 1;

	// only valid if opened for write
  if( m_mode != WRITE || !m_pScore )
  {
    throw EMaxMidiException( "SMF cant write" );
  }

  // find the track number corresponding to this track
  for(int Track = 0; Track < m_pScore->Count; Track++)
    if(m_pScore->Tracks[Track] == pTrack)
      rc = FMaxMidi->WriteMetaEvent(m_hSMF, Track, type, Value, time);

	return rc == 0;
}
//---------------------------------------------------------------------------
#if 0
static inline TMaxMidiSMF *ValidCtrCheck()
{
	return new TMaxMidiSMF(NULL);
}
#endif
//---------------------------------------------------------------------------
namespace Maxmidismf
{
	void __fastcall Register()
	{
		TComponentClass classes[1] = {__classid(TMaxMidiSMF)};
		RegisterComponents("MaxMidi", classes, 0);
	}
}
//---------------------------------------------------------------------------

