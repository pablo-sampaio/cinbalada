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

#include "MaxMidiScore.h"
#include "MaxMidiTrack.h"
#include "MaxMidiOut.h"

//---------------------------------------------------------------------------
//	TrackMerge structure
//---------------------------------------------------------------------------

#define MERGE_BUFFER_SIZE 512 // should be the size of the output device buffer

class TrackMerge {
public:
  TrackMerge( TMaxMidiTrack* pTrackX = NULL )
  : pTrack( pTrackX ),
    pAbsBuf( 0 ),
    bufSize( MERGE_BUFFER_SIZE),
    thisEvent( 0 ),
    lastEvent( 0 ),
    fInSysex( false )
  {
    if( !pTrack )
    {
      pTrack = new TMaxMidiTrack;
    }
  }
  ~TrackMerge() { delete pTrack; }

	TMaxMidiTrack* pTrack;    // track object
	LPMIDIEVENT    pAbsBuf;   // track buffer in absolute time
	DWORD          bufSize;   // number of events in abs buffer
	DWORD          thisEvent; // index in AbsBuf of next event to read
	DWORD          lastEvent; // index of last event output from this track
	bool           fInSysex;  // true if currently in sysex in this track
};


//---------------------------------------------------------------------------
__fastcall TMaxMidiScore::TMaxMidiScore( int numTracks )
{
  m_pTrackList = new TList;
  m_pTrackList->Capacity = numTracks;
  m_numEvents = 0;
  m_lpMerge = NULL;

  for( int track = 0; track < numTracks; track++ )
  {
    m_pTrackList->Add( new TrackMerge );
  }
}
//---------------------------------------------------------------------------
__fastcall TMaxMidiScore::~TMaxMidiScore()
{
  Clear();
  delete m_pTrackList;
}
//---------------------------------------------------------------------------
void TMaxMidiScore::Clear(void)
{
  for( int track = 0; track < Count; track++ )
  {
    delete static_cast< TrackMerge* >( m_pTrackList->Items[ track ] );
    m_pTrackList->Items[ track ] = NULL;
  }

  m_pTrackList->Clear();
}
//---------------------------------------------------------------------------
void TMaxMidiScore::Insert(int Index, TMaxMidiTrack* Item)
{
  TrackMerge* pTrackMerge = new TrackMerge( Item );
  m_pTrackList->Insert( Index, pTrackMerge );
}
//---------------------------------------------------------------------------
TMaxMidiTrack* __fastcall TMaxMidiScore::Get( int Index )
{
  TrackMerge* pTrackMerge =
    static_cast< TrackMerge* >( m_pTrackList->Items[ Index ] );

  return( pTrackMerge->pTrack );
}
//---------------------------------------------------------------------------
void __fastcall TMaxMidiScore::Put( int Index, TMaxMidiTrack* Item )
{
  TrackMerge* pTrackMerge =
    static_cast< TrackMerge* >( m_pTrackList->Items[ Index ] );

  pTrackMerge->pTrack = Item;
}
//---------------------------------------------------------------------------
int TMaxMidiScore::Add( TMaxMidiTrack* pTrack )
{
  TrackMerge* pTrackMerge = new TrackMerge( pTrack );
  return( m_pTrackList->Add( pTrackMerge ) );
}
//---------------------------------------------------------------------------
int TMaxMidiScore::Remove( TMaxMidiTrack* pTrack )
{
  for( int track = 0; track < m_pTrackList->Count; track++ )
  {
    TrackMerge* pTrackMerge =
      static_cast< TrackMerge* >( m_pTrackList->Items[ track ] );

    if( pTrackMerge->pTrack == pTrack )
    {
      return( m_pTrackList->Remove( pTrackMerge ) );
    }
  }
  return( -1 );
}
//---------------------------------------------------------------------------
int TMaxMidiScore::Find( TMaxMidiTrack* pTrack )
{
  for( int track = 0; track < m_pTrackList->Count; track++ )
  {
    TrackMerge* pTrackMerge =
      static_cast< TrackMerge* >( m_pTrackList->Items[ track ] );

    if( pTrackMerge->pTrack == pTrack )
    {
      return( track );
    }
  }
  return( -1 );
}
//---------------------------------------------------------------------------
LPMIDIEVENT TMaxMidiScore::MergeTracks()
{
	int track;
	DWORD time;
	int minTrack;
	int outEvents;
	bool tracksDone = FALSE;
	bool fAnyInSysex = FALSE;
	int SysexTrack;
	DWORD NewLastAbs;

	// create the merge buffer that will receive the merged events
	if(m_lpMerge == NULL)
		m_lpMerge = (LPMIDIEVENT)GlobalAlloc(GPTR, MERGE_BUFFER_SIZE * sizeof(MidiEvent));

	// get a block of events from each of the tracks, in absolute time
	for(track = 0; track < m_pTrackList->Count; track++)
	{
    TrackMerge* pTrackMerge =
      static_cast< TrackMerge* >( m_pTrackList->Items[ track ] );

    // always try to read something, even if the buffer was empty last time
		pTrackMerge->bufSize = MERGE_BUFFER_SIZE;
		pTrackMerge->pAbsBuf = pTrackMerge->pTrack->GetAbsBuffer(pTrackMerge->lastEvent, &pTrackMerge->bufSize);

		// initialize the track structs for the search
		pTrackMerge->thisEvent = 0;

		// if this track is currently playing sysex, it blocks all other tracks
		if(pTrackMerge->pAbsBuf && pTrackMerge->fInSysex)
		{
			fAnyInSysex = TRUE;
			SysexTrack = track;
		}
	}

	// search through the tracks, merging events in ascending order, based on
	// abs time, into the merge buffer
	outEvents = 0;

	while(outEvents < MERGE_BUFFER_SIZE && !tracksDone)
	{

		// assume the tracks are empty to start
		tracksDone = TRUE;

		// if one of the tracks is in the middle of a sysex, stick on that track
		if(fAnyInSysex)
		{
			minTrack = SysexTrack;

      TrackMerge* pTrackMerge =
        static_cast< TrackMerge* >( m_pTrackList->Items[ minTrack ] );

			if(pTrackMerge->pAbsBuf != NULL && pTrackMerge->thisEvent < pTrackMerge->bufSize)
			{
				time = pTrackMerge->pAbsBuf[pTrackMerge->thisEvent].time;
				tracksDone = FALSE;
			}
		}
		else
    {
      // get the time of the first event in the first track that has an event
      for(minTrack = 0; minTrack < m_pTrackList->Count; minTrack++)
      {
        TrackMerge* pTrackMerge =
          static_cast< TrackMerge* >( m_pTrackList->Items[ minTrack ] );

        if(pTrackMerge->pAbsBuf != NULL && pTrackMerge->thisEvent < pTrackMerge->bufSize)
        {
          time = pTrackMerge->pAbsBuf[pTrackMerge->thisEvent].time;
          tracksDone = FALSE;
          break;
        }
      }
    }

		if(!tracksDone)
		{
			// start the search from the next track, but
			// if one of the tracks is in the middle of a sysex,
			// remain on that track
			if(!fAnyInSysex)
      {
				for( track = minTrack + 1; track < m_pTrackList->Count; track++)
				{
          TrackMerge* pTrackMerge =
            static_cast< TrackMerge* >( m_pTrackList->Items[ track ] );

					// skip tracks that are empty
					if(pTrackMerge->pAbsBuf != NULL && pTrackMerge->thisEvent != pTrackMerge->bufSize)
					{
						// if the time of the next event in this track is less than
						// the current time, this is the new event
						if(pTrackMerge->pAbsBuf[pTrackMerge->thisEvent].time < time)
						{
							minTrack = track;
							time = pTrackMerge->pAbsBuf[pTrackMerge->thisEvent].time;
						}
					}
				}
      }

			// found the next event, copy it over
      TrackMerge* pTrackMerge =
        static_cast< TrackMerge* >( m_pTrackList->Items[ minTrack ] );

			m_lpMerge[outEvents] = pTrackMerge->pAbsBuf[pTrackMerge->thisEvent];
			pTrackMerge->thisEvent++;

			// is this the start or end of a sysex in this track?
			switch(m_lpMerge[outEvents].data1)
			{
				case SYSEX:
					pTrackMerge->fInSysex = TRUE;
					fAnyInSysex = TRUE;
					SysexTrack = minTrack;
					break;

				case EOX:
					pTrackMerge->fInSysex = FALSE;
					fAnyInSysex = FALSE;
					break;
			}

			outEvents++;
		}
	}

	// update all of the lastEvent indices and free the abs buffers
	for(track = 0; track < m_pTrackList->Count; track++)
	{
    TrackMerge* pTrackMerge =
      static_cast< TrackMerge* >( m_pTrackList->Items[ track ] );

		pTrackMerge->lastEvent += pTrackMerge->thisEvent;
		if(pTrackMerge->pAbsBuf)
			GlobalFree(pTrackMerge->pAbsBuf);
	}

	// convert the merge buffer back to delta times
	if(outEvents)
	{
		NewLastAbs = m_lpMerge[outEvents - 1].time;

		for( int i = outEvents - 1; i >= 1; i-- )
		{
			m_lpMerge[i].time -= m_lpMerge[i - 1].time;

			// if the one of the tracks was playing a large sysex, the timestamp
			// for the next event in the stream might end up with a negative
			// timestamp, since its time would logically be during the sysex.
			// We have to catch up the tracks here... it stinks, I know, but it
			// is the best we can do under the circumstances (sysex, that is).
			if((int)m_lpMerge[i].time < 0)
				m_lpMerge[i].time = 0;
		}

		m_lpMerge[0].time -= m_LastAbs;
		m_LastAbs = NewLastAbs;
	}

	m_numEvents = outEvents;
	return m_lpMerge;
}
//---------------------------------------------------------------------------
void TMaxMidiScore::MergeOut( TMaxMidiOut* pMaxMidiOut )
{
	if( m_pTrackList->Count )
	{
		// any events still in merge buffer?
		if( m_numEvents == m_outPtr )
		{
			// no, need to merge some more tracks
			// m_numEvents is set in MergeTracks()
			MergeTracks();
			m_outPtr = 0;
		}

		// pump out as much data as the MidiOut will accept
		while( m_outPtr < m_numEvents && pMaxMidiOut->Put( &m_lpMerge[ m_outPtr ] ) )
		{
			m_outPtr++;

			// merge more if needed
			if( m_numEvents == m_outPtr )
			{
				MergeTracks();
				m_outPtr = 0;
			}
		}

	}
}
//---------------------------------------------------------------------------
void TMaxMidiScore::StartOut( TMaxMidiOut* pMaxMidiOut )
{
	int track;

	// rewind the tracks and start again
	for(track = 0; track < m_pTrackList->Count; track++)
	{
    TrackMerge* pTrackMerge =
      static_cast< TrackMerge* >( m_pTrackList->Items[ track ] );
		pTrackMerge->lastEvent = 0;
		pTrackMerge->fInSysex = FALSE;
		pTrackMerge->bufSize = MERGE_BUFFER_SIZE;
	}

	m_LastAbs = 0;
	m_numEvents = m_outPtr = 0;
	MergeOut(pMaxMidiOut);
}
//---------------------------------------------------------------------------

