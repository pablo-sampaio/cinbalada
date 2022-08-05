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

#include "MaxMidiSync.h"
//---------------------------------------------------------------------------
__fastcall TMaxMidiSync::TMaxMidiSync(TComponent* Owner)
	: TComponent(Owner),
  FProcessMidiBeatEvent( 0 ), FProcessSyncDoneEvent( 0 )
{

	m_hDevice = 0;
	m_fIsOpen = FALSE;
	m_fRunning = FALSE;
	m_hWnd = AllocateHWnd( this->ControlWndProc );
}
//---------------------------------------------------------------------------
__fastcall TMaxMidiSync::~TMaxMidiSync()
{
	Stop();
  if( m_fIsOpen )
  {
	  Close();
  }
  if( m_hWnd != NULL )
  {
    DeallocateHWnd( m_hWnd );
    m_hWnd = NULL;
  }
}
//---------------------------------------------------------------------------
bool TMaxMidiSync::Open(WORD mode, WORD timerPeriod)
{
  if( !FMaxMidi )
  {
    throw EMaxMidiException( "No MaxMidi" );
  }
  
	if(!m_fIsOpen)
		m_hDevice = FMaxMidi->OpenSync(0, m_hWnd, mode, timerPeriod);

	if(m_hDevice > MXMIDIERR_MAXERR)
	{
		m_fIsOpen = TRUE;
		m_CurrentMode = mode;
		m_CurrentPeriod = timerPeriod;
	}
	else
		m_hDevice = 0;

	return m_fIsOpen;
}
//---------------------------------------------------------------------------
void TMaxMidiSync::Close()
{
  if( !m_fIsOpen )
  {
    throw EMaxMidiException( "sync not open" );
  }

	m_fRunning = FALSE;
	m_fIsOpen = FALSE;
}
//---------------------------------------------------------------------------
void TMaxMidiSync::Mode(WORD mode)
{
  if( !m_fIsOpen )
  {
    throw EMaxMidiException( "sync not open" );
  }

  m_hDevice = FMaxMidi->OpenSync(m_hDevice, m_hWnd, mode, USE_CURRENT);

  if(m_hDevice > MXMIDIERR_MAXERR)
  {
    m_fIsOpen = TRUE;
    m_CurrentMode = mode;
  }
  else
  {
    m_hDevice = 0;
    m_fIsOpen = FALSE;
  }
}
//---------------------------------------------------------------------------
void TMaxMidiSync::Period(WORD period)
{
  if( !m_fIsOpen )
  {
    throw EMaxMidiException( "sync not open" );
  }

  m_hDevice = FMaxMidi->OpenSync(m_hDevice, m_hWnd, USE_CURRENT, period);

  if(m_hDevice > MXMIDIERR_MAXERR)
  {
    m_fIsOpen = TRUE;
    m_CurrentPeriod = period;
  }
  else
  {
    m_hDevice = 0;
    m_fIsOpen = FALSE;
  }
}
//---------------------------------------------------------------------------
bool TMaxMidiSync::IsOpen()
{
	return m_fIsOpen;
}
//---------------------------------------------------------------------------
void TMaxMidiSync::Start()
{
  if( !m_fIsOpen )
  {
    throw EMaxMidiException( "sync not open" );
  }

  FMaxMidi->StartSync(m_hDevice);
  m_fRunning = TRUE;
}
//---------------------------------------------------------------------------
void TMaxMidiSync::ReStart()
{
  if( !m_fIsOpen )
  {
    throw EMaxMidiException( "sync not open" );
  }

  FMaxMidi->ReStartSync(m_hDevice);
  m_fRunning = TRUE;
}
//---------------------------------------------------------------------------
void TMaxMidiSync::Stop()
{
	if(m_fIsOpen)
	{
		FMaxMidi->StopSync(m_hDevice);
		m_fRunning = FALSE;
	}
}
//---------------------------------------------------------------------------
void TMaxMidiSync::Pause(bool reset)
{
  if( !m_fIsOpen )
  {
    throw EMaxMidiException( "sync not open" );
  }

  FMaxMidi->PauseSync(m_hDevice, reset);
  m_fRunning = FALSE;
}
//---------------------------------------------------------------------------
DWORD TMaxMidiSync::Tempo()
{
	return FMaxMidi->GetTempo(m_hDevice);
}
//---------------------------------------------------------------------------
bool TMaxMidiSync::Tempo(DWORD tempo)
{
	return (FMaxMidi->SetTempo(m_hDevice, tempo) == 0);
}
//---------------------------------------------------------------------------
WORD TMaxMidiSync::Resolution()
{
	return FMaxMidi->GetResolution(m_hDevice);
}
//---------------------------------------------------------------------------
void TMaxMidiSync::Resolution(WORD res)
{
	FMaxMidi->SetResolution(m_hDevice, res);
}
//---------------------------------------------------------------------------
DWORD TMaxMidiSync::ConvertFromBPM(double tempo)
{
	// can't allow zero tempo!
	if(tempo == 0.0)
		tempo = 120.0;

	return (DWORD)(60000000.0/tempo);
}
//---------------------------------------------------------------------------
double TMaxMidiSync::ConvertToBPM(DWORD tempo)
{
	return (60000000.0/(double)tempo);
}
//---------------------------------------------------------------------------
void __fastcall TMaxMidiSync::ControlWndProc( TMessage &Message )
{
  switch( Message.Msg )
  {
    case MIDI_BEAT:
      if( FProcessMidiBeatEvent )
      {
        FProcessMidiBeatEvent( this );
      }
      break;

    case SYNC_DONE :
      if( FProcessSyncDoneEvent )
      {
        FProcessSyncDoneEvent( this );
      }
      break;

    default:
      DefWindowProc( m_hWnd, Message.Msg, Message.WParam, Message.LParam);
  }
}
//---------------------------------------------------------------------------
#if 0
static inline TMaxMidiSync *ValidCtrCheck()
{
	return new TMaxMidiSync(NULL);
}
#endif
//---------------------------------------------------------------------------
namespace Maxmidisync
{
	void __fastcall Register()
	{
		TComponentClass classes[1] = {__classid(TMaxMidiSync)};
		RegisterComponents("MaxMidi", classes, 0);
	}
}
//---------------------------------------------------------------------------
