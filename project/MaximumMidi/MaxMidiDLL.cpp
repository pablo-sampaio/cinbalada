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

#include "MaxMidiDLL.h"

//---------------------------------------------------------------------------
#define CloseMidiInName             "_CloseMidiIn@4"
#define CloseMidiInExName           "_CloseMidiInEx@4"
#define CloseMidiOutName            "_CloseMidiOut@4"
#define CloseMidiOutExName          "_CloseMidiOutEx@4"
#define CloseSMFName                "_CloseSMF@4"
#define CloseSyncName               "_CloseSync@4"
#define CloseSyncExName             "_CloseSyncEx@4"
#define FlushMidiOutName            "_FlushMidiOut@4"
#define FlushMidiOutExName          "_FlushMidiOutEx@4"
#define GetMaxMidiVersionName       "_GetMaxMidiVersion@0"
#define GetMaxMidiVersionExName     "_GetMaxMidiVersionEx@0"
#define GetMidiInName               "_GetMidiIn@4"
#define GetMidiInDescriptionName    "_GetMidiInDescription@8"
#define GetMidiInDescriptionExName  "_GetMidiInDescriptionEx@8"
#define GetMidiInExName             "_GetMidiInEx@4"
#define GetMidiOutDescriptionName   "_GetMidiOutDescription@8"
#define GetMidiOutDescriptionExName "_GetMidiOutDescriptionEx@8"
#define GetNumInDevicesName         "_GetNumInDevices@0"
#define GetNumOutDevicesName        "_GetNumOutDevices@0"
#define GetPositionName             "_GetPosition@8"
#define GetPositionExName           "_GetPositionEx@8"
//Acrescido Anisprite
#define SetBeatRatioName	    "_SetBeatRatio@8"
#define SetBeatRatioExName	    "_SetBeatRatioEx@8"

#define GetResolutionName           "_GetResolution@4"
#define GetResolutionExName         "_GetResolutionEx@4"
#define GetSMFResolutionName        "_GetSMFResolution@4"
#define GetTempoName                "_GetTempo@4"
#define GetTempoExName              "_GetTempoEx@4"
#define MidiInCallbackName          "_MidiInCallback@20"
#define MidiInProcName              "_MidiInProc@16"
#define MidiOutCallbackName         "_MidiOutCallback@20"
#define MidiOutProcName             "_MidiOutProc@16"
#define OpenMidiInName              "_OpenMidiIn@16"
#define OpenMidiInExName            "_OpenMidiInEx@16"
#define OpenMidiOutName             "_OpenMidiOut@16"
#define OpenMidiOutExName           "_OpenMidiOutEx@16"
#define OpenSMFName                 "_OpenSMF@16"
#define OpenSyncName                "_OpenSync@16"
#define OpenSyncExName              "_OpenSyncEx@16"
#define PauseSyncName               "_PauseSync@8"
#define PauseSyncExName             "_PauseSyncEx@8"
#define PutMidiOutName              "_PutMidiOut@8"
#define PutMidiOutExName            "_PutMidiOutEx@8"
#define ReStartSyncName             "_ReStartSync@4"
#define ReStartSyncExName           "_ReStartSyncEx@4"
#define ReadMetaEventName           "_ReadMetaEvent@20"
#define ReadSMFName                 "_ReadSMF@16"
#define ResetMidiOutName            "_ResetMidiOut@4"
#define ResetMidiOutExName          "_ResetMidiOutEx@4"
#define RewindSMFName               "_RewindSMF@4"
#define SetResolutionName           "_SetResolution@8"
#define SetResolutionExName         "_SetResolutionEx@8"
#define SetSMFResolutionName        "_SetSMFResolution@8"
#define SetTempoName                "_SetTempo@8"
#define SetTempoExName              "_SetTempoEx@8"
#define StartMidiInName             "_StartMidiIn@4"
#define StartMidiInExName           "_StartMidiInEx@4"
#define StartSyncName               "_StartSync@4"
#define StartSyncExName             "_StartSyncEx@4"
#define StopMidiInName              "_StopMidiIn@4"
#define StopMidiInExName            "_StopMidiInEx@4"
#define StopSyncName                "_StopSync@4"
#define StopSyncExName              "_StopSyncEx@4"
#define WriteMetaEventName          "_WriteMetaEvent@20"
#define WriteSMFName                "_WriteSMF@16"
#define syncTimerName               "_syncTimer@20"
//---------------------------------------------------------------------------
template< class x >
void loadProc( HINSTANCE inst, x* proc, char* pName )
{
  *proc = (x)GetProcAddress( inst, pName );
  if( !*proc )
  {
    throw EMaxMidiException( AnsiString( "Failed to loadproc:" + AnsiString( pName ) ) );
  }
}

//---------------------------------------------------------------------------
__fastcall TMaxMidi::TMaxMidi(TComponent* Owner)
	: TComponent(Owner), m_hinst( 0 )
{
  //load the max midi dll
  m_hinst = LoadLibrary( "MxMidi32" );
  if( !m_hinst )
  {
    throw EMaxMidiException( "Load of MxMidi32.dll failed" );
  }

  try
  {
    loadProc( m_hinst, &m_GetMaxMidiVersion, GetMaxMidiVersionName );

    loadProc( m_hinst, &GetNumOutDevices, GetNumOutDevicesName );
    loadProc( m_hinst, &GetMidiOutDescription, GetMidiOutDescriptionName );
    loadProc( m_hinst, &OpenMidiOut, OpenMidiOutName );
    loadProc( m_hinst, &ResetMidiOut, ResetMidiOutName );
    loadProc( m_hinst, &FlushMidiOut, FlushMidiOutName );
    loadProc( m_hinst, &CloseMidiOut, CloseMidiOutName );
    loadProc( m_hinst, &PutMidiOut, PutMidiOutName );

    loadProc( m_hinst, &GetNumInDevices, GetNumInDevicesName );
    loadProc( m_hinst, &GetMidiInDescription, GetMidiInDescriptionName );
    loadProc( m_hinst, &OpenMidiIn, OpenMidiInName );
    loadProc( m_hinst, &StartMidiIn, StartMidiInName );
    loadProc( m_hinst, &StopMidiIn, StopMidiInName );
    loadProc( m_hinst, &CloseMidiIn, CloseMidiInName );
    loadProc( m_hinst, &GetMidiIn, GetMidiInName );

    loadProc( m_hinst, &OpenSync, OpenSyncName );
    loadProc( m_hinst, &CloseSync, CloseSyncName );
    loadProc( m_hinst, &StopSync, StopSyncName );
    loadProc( m_hinst, &StartSync, StartSyncName );
    loadProc( m_hinst, &PauseSync, PauseSyncName );
    loadProc( m_hinst, &ReStartSync, ReStartSyncName );
    loadProc( m_hinst, &SetTempo, SetTempoName );
    loadProc( m_hinst, &SetResolution, SetResolutionName );
    loadProc( m_hinst, &GetTempo, GetTempoName );
    loadProc( m_hinst, &GetResolution, GetResolutionName );
    loadProc( m_hinst, &GetPosition, GetPositionName );
//Acrescido AniSprite
 
    loadProc( m_hinst, &OpenSMF, OpenSMFName );
    loadProc( m_hinst, &CloseSMF, CloseSMFName );
    loadProc( m_hinst, &RewindSMF, RewindSMFName );
    loadProc( m_hinst, &ReadSMF, ReadSMFName );
    loadProc( m_hinst, &WriteSMF, WriteSMFName );
    loadProc( m_hinst, &GetSMFResolution, GetSMFResolutionName );
    loadProc( m_hinst, &SetSMFResolution, SetSMFResolutionName );
    loadProc( m_hinst, &ReadMetaEvent, ReadMetaEventName );
    loadProc( m_hinst, &WriteMetaEvent, WriteMetaEventName );
  }
  catch( Exception& e )
  {
    FreeLibrary( m_hinst );
    throw;
  }

  //initializes MaxMidi library
  m_version = m_GetMaxMidiVersion();
}
//---------------------------------------------------------------------------
__fastcall TMaxMidi::~TMaxMidi()
{
  if( m_hinst )
  {
    if( !FreeLibrary( m_hinst ) )
    {
      throw EMaxMidiException( "FreeLibrary failed" );
    }
  }
}
//---------------------------------------------------------------------------
WORD TMaxMidi::GetMaxMidiVersion()
{
  return( m_version );
}
//---------------------------------------------------------------------------
#if 0
static inline TMaxMidi *ValidCtrCheck()
{
	return new TMaxMidi(NULL);
}
#endif
//---------------------------------------------------------------------------
namespace Maxmididll
{
	void __fastcall Register()
	{
		TComponentClass classes[1] = {__classid(TMaxMidi)};
		RegisterComponents("MaxMidi", classes, 0);
	}
}
//---------------------------------------------------------------------------
