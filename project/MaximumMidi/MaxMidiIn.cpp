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

#include "MaxMidiIn.h"
#include "MaxMidiSync.h"
#include "MaxMidiTrack.h"
//---------------------------------------------------------------------------
__fastcall TMaxMidiIn::TMaxMidiIn(TComponent* Owner)
	: TComponent(Owner), m_hWnd( NULL ),
#ifdef MAXIN_CLOSE_EVENT
      FProcessMidiCloseEvent( 0 ),
#endif
      FProcessMidiDataEvent( 0 )
{
  m_hDevice = 0;
  m_timestamp = 0;
  m_bIsOpen = FALSE;

#ifdef _SYNC
  FSync = NULL;
#endif
#ifdef _TRACK
  m_pTrack = NULL;
#endif
 m_hWnd = AllocateHWnd( this->ControlWndProc );
}
//---------------------------------------------------------------------------
__fastcall TMaxMidiIn::~TMaxMidiIn()
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
WORD TMaxMidiIn::GetIDFromName( const AnsiString& sName )
{
  char thisDesc[ MAXPNAMELEN ];
  int  MaxDevs = FMaxMidi->GetNumInDevices();

  for( WORD id = 0; id < MaxDevs; id++ )
  {
    FMaxMidi->GetMidiInDescription( id, thisDesc );
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
void TMaxMidiIn::Attach( TMaxMidiSync* pSync )
{
  FSync = pSync;
}
//---------------------------------------------------------------------------
void TMaxMidiIn::Detach( TMaxMidiSync* pSync )
{
  // detach this sync device, if it is attached
  if( pSync != FSync )
  {
    throw EMaxMidiException( "sync not found" );
  }

  // close and reopen the device
  if( m_bIsOpen )
  {
    Close();
    Open( m_wDeviceID, m_dwFlags );
  }
}
#endif
//---------------------------------------------------------------------------
#ifdef _TRACK
void TMaxMidiIn::Attach( TMaxMidiTrack* pTrack)
{
  // detach the track if one was already attached
  if( m_pTrack != NULL)
  {
    m_pTrack->Detach( this );
  }

  // attach the new track
  m_pTrack = pTrack;

  // tell the track about this input device
  pTrack->Attach( this );
}
//---------------------------------------------------------------------------
void TMaxMidiIn::Detach( TMaxMidiTrack* pTrack)
{
  // detach this track, if it is attached
  if( pTrack != m_pTrack )
  {
    throw EMaxMidiException( "track not found" );
  }

  m_pTrack->Detach( this );
  m_pTrack = NULL;
}
#endif
//---------------------------------------------------------------------------
bool TMaxMidiIn::Open( WORD wDeviceID, DWORD dwFlags )
{
  if( m_bIsOpen )
  {
    throw EMaxMidiException( "already open" );
  }

  HSYNC hSync = 0;
  m_timestamp = 0;

#ifdef _SYNC
  // get the sync device handle
  if( FSync )
  {
    hSync = FSync->GetHSync();
  }
#endif

  // get the description of the device
  char desc[ MAXPNAMELEN ];
  FMaxMidi->GetMidiInDescription( wDeviceID, desc );
  m_sDesc = desc;

  m_dwFlags = dwFlags;
  m_wDeviceID = wDeviceID;
  m_hDevice = FMaxMidi->OpenMidiIn( m_hWnd, m_wDeviceID, hSync, m_dwFlags);

  if( m_hDevice > MXMIDIERR_MAXERR )
  {
    m_bIsOpen = TRUE;
  }
  else
  {
    m_hDevice = 0;
  }

  return( m_bIsOpen );
}
//---------------------------------------------------------------------------
void TMaxMidiIn::Close()
{
  if( !m_bIsOpen )
  {
    throw EMaxMidiException( "midiIn not open" );
  }

  Stop();
  FMaxMidi->CloseMidiIn( m_hDevice );
  m_sDesc = "";

  m_bIsOpen = FALSE;
}
//---------------------------------------------------------------------------
bool TMaxMidiIn::IsOpen()
{
  return( m_bIsOpen );
}
//---------------------------------------------------------------------------
AnsiString TMaxMidiIn::GetDescription()
{
  return( m_sDesc );
}
//---------------------------------------------------------------------------
int TMaxMidiIn::GetNumDevices()
{
  return( FMaxMidi->GetNumInDevices() );
}
//---------------------------------------------------------------------------
void TMaxMidiIn::Start()
{
  if( !m_bIsOpen || m_bIsStarted )
  {
    throw EMaxMidiException( "cant start midiIn" );
  }

  m_bIsStarted = TRUE;
  m_timestamp = 0;
  FMaxMidi->StartMidiIn( m_hDevice );
}
//---------------------------------------------------------------------------
void TMaxMidiIn::Stop()
{
  if( !m_bIsOpen )
  {
    throw EMaxMidiException( "midiIn not open" );
  }

  m_bIsStarted = FALSE;
  FMaxMidi->StopMidiIn( m_hDevice );
}
//---------------------------------------------------------------------------
void TMaxMidiIn::Reset()
{
  if( !m_bIsOpen || !m_bIsStarted )
  {
    throw EMaxMidiException( "midiIn cant reset" );
  }
  m_timestamp = 0;
  FMaxMidi->StopMidiIn( m_hDevice );
  FMaxMidi->StartMidiIn( m_hDevice );
}
//---------------------------------------------------------------------------
LPMIDIEVENT TMaxMidiIn::Get()
{
	return( FMaxMidi->GetMidiIn( m_hDevice ) );
}
//---------------------------------------------------------------------------
void __fastcall TMaxMidiIn::ControlWndProc( TMessage &Message )
{
  MidiEvent* lpMsg;
  switch( Message.Msg )
  {
    case MIDI_DATA:
      while( ( lpMsg = Get() ) != NULL)  {
        /* process the message pointed to by lpMsg */
        lpMsg->time += m_timestamp;

        //if we need to filter alot of msgs should use a table here
        bool bAccept;
        if( lpMsg->status == 0xFE )    //ignore "Active Sense" msgs
        {
          bAccept = false;
        }
        else
        {
          bAccept = true;
          if( FProcessMidiDataEvent )
          {
            FProcessMidiDataEvent( lpMsg, &bAccept );
          }
        }

        if( bAccept )
        {
          m_timestamp = 0;
        }
        else
        {
          m_timestamp = lpMsg->time;
        }
  #ifdef _TRACK
        if( bAccept && m_pTrack )
        {
          m_pTrack->Write( lpMsg );
        }
  #endif
      }
      break;

#ifdef MAXIN_CLOSE_EVENT
    case MIM_CLOSE:
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
static inline TMaxMidiIn *ValidCtrCheck()
{
	return new TMaxMidiIn(NULL);
}
#endif
//---------------------------------------------------------------------------
namespace Maxmidiin
{
	void __fastcall Register()
	{
		TComponentClass classes[1] = {__classid(TMaxMidiIn)};
		RegisterComponents("MaxMidi", classes, 0);
	}
}
//---------------------------------------------------------------------------

