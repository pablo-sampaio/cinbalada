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

#include "MaxMidiOut.h"
#include "MaxMidiSync.h"
#include "MaxMidiScore.h"
//---------------------------------------------------------------------------
__fastcall TMaxMidiOut::TMaxMidiOut(TComponent* Owner)
	: TComponent(Owner),
#ifdef MAXOUT_CLOSE_EVENT
  FProcessMidiCloseEvent( 0 ),
#endif
  FProcessOutBufferReadyEvent( 0 )
{
  m_hDevice = 0;
  m_bIsOpen = FALSE;
#ifdef _SYNC
  FSync = NULL;
#endif
#ifdef _TRACK
  m_pScore = NULL;
#endif
 m_hWnd = AllocateHWnd( this->ControlWndProc );
}
//---------------------------------------------------------------------------
__fastcall TMaxMidiOut::~TMaxMidiOut()
{
  if( m_bIsOpen )
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
WORD TMaxMidiOut::GetIDFromName( const AnsiString& sName )
{
  char thisDesc[MAXPNAMELEN];
  int  MaxDevs = FMaxMidi->GetNumInDevices();

  for( WORD id = 0; id < MaxDevs; id++ )
  {
    FMaxMidi->GetMidiOutDescription( id, thisDesc );
    if( sName == thisDesc )
    {
      return( id );
    }

    id++;
  }

  return( ERR_NOMATCH );
}
#ifdef _SYNC
//---------------------------------------------------------------------------
void TMaxMidiOut::Attach( TMaxMidiSync* pSync )
{
	FSync = pSync;
}
//---------------------------------------------------------------------------
void TMaxMidiOut::Detach( TMaxMidiSync* pSync )
{
  if( pSync != FSync )
  {
    throw EMaxMidiException( "sync not found" );
  }

  FSync = NULL;

	// close and reopen the device
	if( m_bIsOpen )
	{
		Close();
		Open( m_wDeviceID, m_dwFlags );
	}
}
#endif
#ifdef _TRACK
//---------------------------------------------------------------------------
void TMaxMidiOut::Attach( TMaxMidiScore* pScore )
{
  m_pScore = pScore;
}
//---------------------------------------------------------------------------
void TMaxMidiOut::Detach( TMaxMidiScore* pScore )
{
  if( m_pScore != pScore )
  {
    throw EMaxMidiException( "score not found" );
  }

  m_pScore = NULL;
}
#endif
//---------------------------------------------------------------------------
bool TMaxMidiOut::Open(WORD wDeviceID, DWORD dwFlags)
{
  if( !FMaxMidi )
  {
    throw EMaxMidiException( "No MaxMidi" );
  }
  if( m_bIsOpen )
  {
    throw EMaxMidiException( "already open" );
  }
	HSYNC hSync = 0;

	// get the description of the device
  char desc[ MAXPNAMELEN ];
  FMaxMidi->GetMidiInDescription( wDeviceID, desc );
  m_sDesc = desc;

#ifdef _SYNC
	// get the sync device handle
	if(FSync)
		hSync = FSync->GetHSync();
#endif

  m_dwFlags = dwFlags;
  m_wDeviceID = wDeviceID;
  m_hDevice = FMaxMidi->OpenMidiOut( m_hWnd, wDeviceID, hSync, dwFlags );

	if(m_hDevice > MXMIDIERR_MAXERR)
		m_bIsOpen = TRUE;
	else
		m_hDevice = 0;

	return m_bIsOpen;
}
//---------------------------------------------------------------------------
void TMaxMidiOut::Close()
{
  if( !m_bIsOpen )
  {
    throw EMaxMidiException( "midiOut not open" );
  }

  m_sDesc = "";
  Reset();
  FMaxMidi->CloseMidiOut(m_hDevice);
	m_bIsOpen = FALSE;
}
//---------------------------------------------------------------------------
bool TMaxMidiOut::IsOpen()
{
	return( m_bIsOpen );
}
//---------------------------------------------------------------------------
AnsiString TMaxMidiOut::GetDescription()
{
	return( m_sDesc );
}
//---------------------------------------------------------------------------
int TMaxMidiOut::GetNumDevices()
{
	return( FMaxMidi->GetNumOutDevices() );
}
//---------------------------------------------------------------------------
bool TMaxMidiOut::Put( LPMIDIEVENT lpEvent )
{
	if( m_bIsOpen )
  {
		return( FMaxMidi->PutMidiOut( m_hDevice, lpEvent ) == 0 );
  }

  // act as though it accepts everything
  // in case Put is called when not open
	return TRUE;
}
//---------------------------------------------------------------------------
void TMaxMidiOut::Reset()
{
	if( !m_bIsOpen )
  {
    throw EMaxMidiException( "midiOut not open" );
  }

  FMaxMidi->ResetMidiOut( m_hDevice );
}
//---------------------------------------------------------------------------
void TMaxMidiOut::Flush()
{
	if( !m_bIsOpen )
  {
    throw EMaxMidiException( "midiOut not open" );
  }
	FMaxMidi->FlushMidiOut( m_hDevice );
}
//---------------------------------------------------------------------------
void __fastcall TMaxMidiOut::ControlWndProc( TMessage &Message )
{
  switch( Message.Msg )
  {
    case OUTBUFFER_READY:
#ifdef _TRACK
      if( m_pScore )
      {
        // output events from any attached tracks
        m_pScore->MergeOut( this );
      }
#endif
      if( FProcessOutBufferReadyEvent )
      {
        FProcessOutBufferReadyEvent( this );
      }
      break;

#ifdef MAXOUT_CLOSE_EVENT
    case MOM_CLOSE:
      if( FProcessMidiCloseEvent )
      {
        FProcessMidiCloseEvent( this );
      }
      break;
#endif

    default:
      DefWindowProc( m_hWnd, Message.Msg, Message.WParam, Message.LParam);
  }
}
//---------------------------------------------------------------------------
#if 0
static inline TMaxMidiOut *ValidCtrCheck()
{
	return new TMaxMidiOut(NULL);
}
#endif
//---------------------------------------------------------------------------
namespace Maxmidiout
{
	void __fastcall Register()
	{
		TComponentClass classes[1] = {__classid(TMaxMidiOut)};
		RegisterComponents("MaxMidi", classes, 0);
	}
}
//---------------------------------------------------------------------------


