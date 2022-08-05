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
#ifndef MaxMidiDllH
#define MaxMidiDllH

//---------------------------------------------------------------------------
#include <vcl\SysUtils.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Forms.hpp>

#include <mmsystem.h>

#define _MIDIIN
#define _MIDIOUT
#define _SYNC
#define _TRACK
#define _SMF

//---------------------------------------------------------------------------
//	midi buffer sizes
#define QUEUE_64	(0 << 12)
#define QUEUE_128	(1 << 12)
#define QUEUE_256	(2 << 12)
#define QUEUE_512	(3 << 12)
#define QUEUE_1K	(4 << 12)
#define QUEUE_2K	(5 << 12)
#define QUEUE_4K	(6 << 12)
#define QUEUE_8K	(7 << 12)
#define QUEUE_16K	(8 << 12)
#define QUEUE_32K	(9 << 12)

#define SXBUF_64	(0 << 8)
#define SXBUF_128	(1 << 8)
#define SXBUF_256	(2 << 8)
#define SXBUF_512	(3 << 8)
#define SXBUF_1K	(4 << 8)
#define SXBUF_2K	(5 << 8)
#define SXBUF_4K	(6 << 8)
#define SXBUF_8K	(7 << 8)
#define SXBUF_16K	(8 << 8)
#define SXBUF_32K	(9 << 8)

//	midi sync status messages
#define NOTEOFF			0x80
#define	NOTEON			0x90
#define SYSEX			0xF0
#define	MTC_QFRAME		0xF1
#define EOX				0xF7
#define	MIDI_CLOCK		0xF8
#define	MIDI_START		0xFA
#define	MIDI_CONTINUE	0xFB
#define	MIDI_STOP		0xFC

//	maxmidi messages
#define WM_MAXMIDI		(WM_USER + 0x504D)	// 'PM'
#define MOM_LONGDATA	(WM_MAXMIDI+3)
#define MIDI_BEAT		(WM_MAXMIDI+5)
#define OUTBUFFER_READY	(WM_MAXMIDI+6)
#define SYNC_DONE		(WM_MAXMIDI+10)
#define MIDI_DATA 		MIM_DATA

//	external (user settable) flags
#define ENABLE_SYSEX		0x00010000L
#define DISABLE_SYSEX 		(~ENABLE_SYSEX)
#define SYNC_INPUT			0x00020000L
#define SYNC_OUTPUT			0x00020000L

// MxMidi DLL error values
#define MXMIDIERR_NOERROR		0
#define MXMIDIERR_BADDEVICEID	MMSYSERR_BADDEVICEID
#define	MXMIDIERR_NOMEM			MMSYSERR_NOMEM
#define MXMIDIERR_BADHANDLE		MMSYSERR_ALLOCATED
#define MXMIDIERR_BADTEMPO		30
#define MXMIDIERR_MAXERR		32
#define ERR_NOMATCH				0xFFF0

//-----------------------------------------------------------------------------
//	Event structures
//-----------------------------------------------------------------------------
#pragma pack(1)
typedef struct {
	DWORD	time;		// time in ticks since last event
	BYTE	status;		// status byte of this midi message
	BYTE	data1;		// first data byte of message
	BYTE	data2;		// second data byte of message
	BYTE	data3;		// third data byte, used for tempo changes
} MidiEvent;
#pragma pack()

typedef MidiEvent* LPMIDIEVENT;

//-----------------------------------------------------------------------------
//	Midi In Definitions
//-----------------------------------------------------------------------------
#define MIDIIN_DEFAULT (QUEUE_512|SXBUF_512|ENABLE_SYSEX|32)
typedef DWORD HMIN;

//-----------------------------------------------------------------------------
//	MIDI Out Definitions
//-----------------------------------------------------------------------------
#define MIDIOUT_DEFAULT (QUEUE_512|SXBUF_512|ENABLE_SYSEX|32)
typedef DWORD HMOUT;

//-----------------------------------------------------------------------------
//	Sync Timer definitions
//-----------------------------------------------------------------------------
#define USE_CURRENT 0xFFFF
#define	DEFAULT_TIMERPERIOD 10
#define	MAX_RESOLUTION 960
#define	S_INT		0
#define S_MIDI		1
#define POS_TICKS	0
#define POS_MS		1
typedef DWORD HSYNC;

//-----------------------------------------------------------------------------
//	Standard MIDI File definitions
//-----------------------------------------------------------------------------
#define META					0xFF
#define META_SEQUENCE_NUMBER	0x00
#define META_TEXT				0x01
#define META_COPYRIGHT			0x02
#define META_NAME				0x03
#define	META_INST_NAME			0x04
#define META_LYRIC				0x05
#define META_MARKER				0x06
#define META_CUE_POINT			0x07
#define META_CHAN_PREFIX		0x20
#define META_EOT				0x2F
#define	META_TEMPO				0x51
#define META_SMPTE_OFFSET		0x54
#define	META_TIME_SIG			0x58
#define	META_KEY_SIG			0x59
#define	META_SEQ_SPECIFIC		0x7F
#define MAX_META_EVENT			0x80

typedef DWORD HSMF;


#undef WINAPI
#define WINAPI __stdcall

typedef WORD  (WINAPI *GetMaxMidiVersionType)(void);

typedef UINT  (WINAPI *GetNumOutDevicesType)(void);
typedef BOOL  (WINAPI *GetMidiOutDescriptionType)(WORD wDeviceID, LPSTR lpzDesc);
typedef HMOUT (WINAPI *OpenMidiOutType)(HWND hWnd, WORD wDeviceID, HSYNC hSync, DWORD dwFlags);
typedef WORD  (WINAPI *ResetMidiOutType)(HMOUT hMidiOut);
typedef void  (WINAPI *FlushMidiOutType)(HMOUT hMidiOut);
typedef WORD  (WINAPI *CloseMidiOutType)(HMOUT hMidiOut);
typedef WORD  (WINAPI *PutMidiOutType)(HMOUT hMidiOut, LPMIDIEVENT lpMidiEvent);

typedef UINT  (WINAPI *GetNumInDevicesType)(void);
typedef BOOL  (WINAPI *GetMidiInDescriptionType)(WORD wDeviceID, LPSTR lpzDesc);
typedef HMIN  (WINAPI *OpenMidiInType)(HWND hWnd, WORD wDeviceID, HSYNC hSync, DWORD dwFlags);
typedef WORD  (WINAPI *StartMidiInType)(HMIN lpMidiIn);
typedef WORD  (WINAPI *StopMidiInType)(HMIN lpMidiIn);
typedef WORD  (WINAPI *CloseMidiInType)(HMIN lpMidiIn);
typedef LPMIDIEVENT  (WINAPI *GetMidiInType)(HMIN lpMidiIn);

typedef HSYNC (WINAPI *OpenSyncType)(HSYNC hSync, HWND hWnd, WORD mode, WORD timerPeriod);
typedef WORD  (WINAPI *CloseSyncType)(HSYNC hSync);
typedef void  (WINAPI *StopSyncType)(HSYNC hSync);
typedef void  (WINAPI *StartSyncType)(HSYNC hSync);
typedef void  (WINAPI *PauseSyncType)(HSYNC hSync, BOOL reset);
typedef void  (WINAPI *ReStartSyncType)(HSYNC hSync);
typedef WORD  (WINAPI *SetTempoType)(HSYNC hSync, DWORD uSPerBeat);
typedef void  (WINAPI *SetResolutionType)(HSYNC hSync, WORD resolution);
typedef DWORD (WINAPI *GetTempoType)(HSYNC hSync);
typedef WORD  (WINAPI *GetResolutionType)(HSYNC hSync);
typedef DWORD (WINAPI *GetPositionType)(HSYNC hSync, WORD units);
//Acrescentado para Anisprite
typedef void (WINAPI *SetBeatRatio)(HSYNC hSync, WORD framerate, WORD beatrate);
//Fim do acrescimo
typedef HSMF  (WINAPI *OpenSMFType)(LPSTR filename, int *Format, const char mode, int *nTracks);
typedef void  (WINAPI *CloseSMFType)(HSMF hSMF);
typedef BOOL  (WINAPI *RewindSMFType)(HSMF hSMF);
typedef DWORD (WINAPI *ReadSMFType)(HSMF hSMF, int wTrack, LPMIDIEVENT lpMidiEventBuffer, DWORD dwBufferLen);
typedef DWORD (WINAPI *WriteSMFType)(HSMF hSMF, int wTrack, LPMIDIEVENT lpMidiEventBuffer, DWORD dwBufferLen);
typedef WORD  (WINAPI *GetSMFResolutionType)(HSMF hSMF);
typedef WORD  (WINAPI *SetSMFResolutionType)(HSMF hSMF, WORD resolution);
typedef DWORD (WINAPI *ReadMetaEventType)(HSMF hSMF, int wTrack, BYTE MetaEvent, LPSTR *EventValue, DWORD *EventSize);
typedef int   (WINAPI *WriteMetaEventType)(HSMF hSMF, int wTrack, BYTE MetaEvent, LPSTR EventValue, DWORD dwTime);

//---------------------------------------------------------------------------

class EMaxMidiException : public Exception
{
public:
  __fastcall EMaxMidiException(const System::AnsiString Msg)
    : Exception( Msg ) {}
  __fastcall ~EMaxMidiException() {}
};

//---------------------------------------------------------------------------

class TMaxMidi : public TComponent
{
private:
  HINSTANCE m_hinst;
  WORD      m_version;

  GetMaxMidiVersionType     m_GetMaxMidiVersion;
protected:
public:
  __fastcall TMaxMidi( TComponent* Owner );
  __fastcall ~TMaxMidi();

  WORD GetMaxMidiVersion();

  GetNumOutDevicesType      GetNumOutDevices;
  GetMidiOutDescriptionType GetMidiOutDescription;
  OpenMidiOutType           OpenMidiOut;
  ResetMidiOutType          ResetMidiOut;
  FlushMidiOutType          FlushMidiOut;
  CloseMidiOutType          CloseMidiOut;
  PutMidiOutType            PutMidiOut;

  GetNumInDevicesType      GetNumInDevices;
  GetMidiInDescriptionType GetMidiInDescription;
  OpenMidiInType           OpenMidiIn;
  StartMidiInType          StartMidiIn;
  StopMidiInType           StopMidiIn;
  CloseMidiInType          CloseMidiIn;
  GetMidiInType            GetMidiIn;

  OpenSyncType             OpenSync;
  CloseSyncType            CloseSync;
  StopSyncType             StopSync;
  StartSyncType            StartSync;
  PauseSyncType            PauseSync;
  ReStartSyncType          ReStartSync;
  SetTempoType             SetTempo;
  SetResolutionType        SetResolution;
  GetTempoType             GetTempo;
  GetResolutionType        GetResolution;
  GetPositionType          GetPosition;
  //SetBeatRatioType	   SetBeatRatio;

  OpenSMFType              OpenSMF;
  CloseSMFType             CloseSMF;
  RewindSMFType            RewindSMF;
  ReadSMFType              ReadSMF;
  WriteSMFType             WriteSMF;
  GetSMFResolutionType     GetSMFResolution;
  SetSMFResolutionType     SetSMFResolution;
  ReadMetaEventType        ReadMetaEvent;
  WriteMetaEventType       WriteMetaEvent;

__published:
};

//---------------------------------------------------------------------------
#endif
