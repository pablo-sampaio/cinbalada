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

#include "MaxMidiTrack.h"
#include "MaxMidiIn.h"
#include "MaxMidiOut.h"
#include "MaxMidiSync.h"
#include "MaxMidiSMF.h"

//---------------------------------------------------------------------------
__fastcall TMaxMidiTrack::TMaxMidiTrack()
{
	m_lpBuffer = NULL;
#ifdef _SMF
	FSMF = NULL;
#endif
#ifdef _MIDIIN
	FMidiIn = NULL;
#endif
	m_inPtr = m_outPtr = 0;
	IsRecording(FALSE);
	Mute(FALSE);

	CreateBuffer();
}
//---------------------------------------------------------------------------
__fastcall TMaxMidiTrack::~TMaxMidiTrack()
{
	// detach the track from any input, or output devices
	Detach();
	FreeBuffer();
}
//---------------------------------------------------------------------------
#ifdef _SMF
void TMaxMidiTrack::Attach(TMaxMidiSMF* pSMF)
{
	FSMF = pSMF;
}
//---------------------------------------------------------------------------
void TMaxMidiTrack::Detach(TMaxMidiSMF* pSMF)
{
	if(pSMF == FSMF)
		FSMF = NULL;
}
#endif
//---------------------------------------------------------------------------
#ifdef _MIDIIN
void TMaxMidiTrack::Attach(TMaxMidiIn* pMidiIn)
{
	FMidiIn = pMidiIn;
}
//---------------------------------------------------------------------------
void TMaxMidiTrack::Detach(TMaxMidiIn* pMidiIn)
{
	if(pMidiIn == FMidiIn)
		FMidiIn = NULL;
}
#endif
//---------------------------------------------------------------------------
void TMaxMidiTrack::Detach()
{
	// detach from any attached devices
#ifdef _MIDIIN
	if(FMidiIn)
	{
		FMidiIn->Detach(this);
		FMidiIn = NULL;
		IsRecording(FALSE);
	}
#endif
}
//---------------------------------------------------------------------------
AnsiString TMaxMidiTrack::GetTrackName()
{
  
	if(!m_sName.IsEmpty())
		return m_sName;

#ifdef _SMF
	DWORD cbSize;

	// get the name from the smf, if it exists
	if(FSMF && FSMF->GetMode() == READ)
	{
    char* pName;
		FSMF->ReadMeta(this, META_NAME, &pName, &cbSize);
    m_sName = pName;
		return m_sName;
	}
#endif

	// currently no name
	return "";
}
//---------------------------------------------------------------------------
void TMaxMidiTrack::SetTrackName( const AnsiString &sName )
{
  m_sName = sName;
}
//---------------------------------------------------------------------------
LPMIDIEVENT TMaxMidiTrack::GetEvent(DWORD eventNum)
{
	if(eventNum >= m_dwBufSize)
		return NULL;

	return &m_lpBuffer[eventNum];
}
//---------------------------------------------------------------------------
void TMaxMidiTrack::SetEvent(LPMIDIEVENT lpEvent, DWORD eventNum)
{
	if(eventNum >= m_dwBufSize)
		return;

	m_lpBuffer[eventNum] = *lpEvent;
}
//---------------------------------------------------------------------------
bool TMaxMidiTrack::CreateBuffer(DWORD dwBufEvents)
{
	// this can only be called once per open
	if(m_lpBuffer)
		return FALSE;

	// create buffers for data from each track
	m_lpBuffer = (LPMIDIEVENT)GlobalAlloc(GPTR, sizeof(MidiEvent) * dwBufEvents);
	m_dwBufSize = dwBufEvents;

	return TRUE;
}
//---------------------------------------------------------------------------
void TMaxMidiTrack::FreeBuffer()
{
	if(m_lpBuffer)
		GlobalFree(m_lpBuffer);

	m_lpBuffer = NULL;
}
//---------------------------------------------------------------------------
LPMIDIEVENT TMaxMidiTrack::Read()
{
	// don't do anything if at end of buffer, or recording or muted
	if(m_outPtr == m_inPtr || m_fMute || m_fRecord)
		return NULL;

	return &m_lpBuffer[m_outPtr++];
}
//---------------------------------------------------------------------------
void TMaxMidiTrack::Write(LPMIDIEVENT lpEvent)
{
	// don't write into the track if not recording
	if(!m_fRecord)
		return;

	InsertEvent(lpEvent, (DWORD)-1);
}
//---------------------------------------------------------------------------
void TMaxMidiTrack::InsertEvent(LPMIDIEVENT lpEvent, DWORD beforeEvent)
{
	// does the buffer need to grow in order to add this event?
	if(m_inPtr == m_dwBufSize)
	{
		m_dwBufSize += BUFFER_GROW_SIZE;
		m_lpBuffer = (LPMIDIEVENT)GlobalReAlloc(m_lpBuffer, m_dwBufSize * sizeof(MidiEvent), GPTR | GMEM_MOVEABLE);
	}

	// insert at the end (append) if beforeEvent is -1
	if(beforeEvent == (DWORD)-1)
	{
		// put it in the buffer
		m_lpBuffer[m_inPtr] = *lpEvent;
		m_inPtr++;
		return;
	}

	// else, move all of the events following this new one up by one slot
	memmove(&m_lpBuffer[beforeEvent + 1], &m_lpBuffer[beforeEvent], m_inPtr - beforeEvent);

	// add the event to the track by putting in the specified location
	// without adjusting the timestamp of the following event
	m_lpBuffer[beforeEvent] = *lpEvent;
	m_inPtr++;
}
//---------------------------------------------------------------------------
void TMaxMidiTrack::DeleteEvent(DWORD eventNum)
{
	// eventNum == -1 will delete the last event
	m_inPtr--;
	if(eventNum < m_inPtr)
	{
		// move all of the events following the specfied event down
		// in the track by one, thus deleteing the event
		memmove(&m_lpBuffer[eventNum], &m_lpBuffer[eventNum + 1], m_inPtr - eventNum);
	}
}
//---------------------------------------------------------------------------
void TMaxMidiTrack::SlideTrack(DWORD eventNum, int delta)
{
	// add the delta time to the specified event
	if(eventNum < m_inPtr)
	{
		m_lpBuffer[eventNum].time += delta;
		if((int)m_lpBuffer[eventNum].time < 0)
			m_lpBuffer[eventNum].time = 0;
	}
}
//---------------------------------------------------------------------------
bool TMaxMidiTrack::Load()
{
#ifdef _SMF
	// if the SMF object is attached, read data into this track
	if(FSMF)
		return FSMF->Read(this);
#endif

	return FALSE;
}
//---------------------------------------------------------------------------
bool TMaxMidiTrack::Save()
{
#ifdef _SMF
	// if the SMF object is attached, save this track
	// this assumes that the buffer is properly attached to the smf object
	// NOTE: this should only be done in ascending order of tracks
	// muted tracks are not written
	if(FSMF && !m_fMute)
	{
		// write the name as the first event in the track, if it exists
		if(!m_sName.IsEmpty())
    {
			FSMF->WriteMeta(this, META_NAME, m_sName.c_str(), 0);
    }

		return FSMF->Write(this);
	}
#endif

	// if not attached, return false
	return FALSE;
}
//---------------------------------------------------------------------------
LPMIDIEVENT TMaxMidiTrack::GetAbsBuffer(DWORD startEvent, DWORD* numEvents)
{
	LPMIDIEVENT lpBuf;

	// sanity check: make sure the requested start event is in the buffer
	// and the track is not muted or recording
	if(startEvent >= m_inPtr || m_fMute || m_fRecord)
	{
		*numEvents = 0;
		return NULL;
	}

	// the buffer size will be either the requested number of events
	// or the number remaining in the track, whichever is lower
	*numEvents = ((m_inPtr - startEvent) < *numEvents ? (m_inPtr - startEvent) : *numEvents);

	// create the buffer
	lpBuf = (LPMIDIEVENT)GlobalAlloc(GPTR, *numEvents * sizeof(MidiEvent));
	memcpy(lpBuf, &m_lpBuffer[startEvent], *numEvents * sizeof(MidiEvent));

	// convert it to absolute time
	DeltaToAbs(lpBuf, startEvent, *numEvents);

	return lpBuf;
}
//---------------------------------------------------------------------------
DWORD TMaxMidiTrack::AbsNow(DWORD eventNum)
{
	DWORD time;
	int i;

	// sanity check: make sure the specified event is in the buffer
	// if not, return zero as the time
	if(eventNum >= m_inPtr)
		return 0;

	// add up the elapsed number of ticks since the beginning
	// of the track till this event
	time = m_lpBuffer[0].time;
	for(i = 1; i <= (int)eventNum; i++)
		time += m_lpBuffer[i].time;

	return time;
}
//---------------------------------------------------------------------------
void TMaxMidiTrack::AbsToDelta(LPMIDIEVENT lpBuf, DWORD startEvent, DWORD numEvents)
{
	DWORD now;
	DWORD start;
	int i;

	// get the abs time of the previous event (since we need the delta from it
	// for the time for the first event is this buffer)
	// if this is the first event in the track (i.e. startEvent == 0) then we know
	// the start time is zero
	if(startEvent == 0)
		start = 0;
	else
		start = AbsNow(startEvent - 1);

	// convert each abs time to delta time by subtracting the abs time from
	// the start time of the event prior event
	for(i = 0; i < (int)numEvents; i++)
	{
		// save the abs time of this event
		// it will become the start for the next event
		now = m_lpBuffer[i].time;
		m_lpBuffer[i].time -= start;
		start = now;
	}
}
//---------------------------------------------------------------------------
void TMaxMidiTrack::DeltaToAbs(LPMIDIEVENT lpBuf, DWORD startEvent, DWORD numEvents)
{
	DWORD now;
	int i;

	// get the abs time of the first event in the buffer
	now = AbsNow(startEvent);

	// we know the abs time for the first one -- its the startEvent
	lpBuf[0].time = now;

	// convert each time stamp to abs by setting it to the 
	// time since the beginning of the track
	for(i = 1; i < (int)numEvents; i++)
	{
		now += lpBuf[i].time;
		lpBuf[i].time = now;
	}
}
//---------------------------------------------------------------------------
DWORD TMaxMidiTrack::GetTime(DWORD eventNum)
{
	// sanity check: make sure the specified event is in the buffer
	// if not, return zero as the time
	if(eventNum >= m_inPtr)
		return 0;

	// return the time for the requested event in track
	return m_lpBuffer[eventNum].time;
}
//---------------------------------------------------------------------------
#if 0
static inline TMaxMidiTrack *ValidCtrCheck()
{
	return new TMaxMidiTrack(NULL);
}
//---------------------------------------------------------------------------
namespace Maxmiditrack
{
	void __fastcall Register()
	{
		TComponentClass classes[1] = {__classid(TMaxMidiTrack)};
		RegisterComponents("Trinitro", classes, 0);
	}
}
//---------------------------------------------------------------------------
#endif
