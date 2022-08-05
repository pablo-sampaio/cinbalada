//---------------------------------------------------------------------------

#ifndef UserMainInterfaceH
#define UserMainInterfaceH
//---------------------------------------------------------------------------
#include "CInBalada\CbFacade.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>
#include "FrameExecutionPlan.h"
#include <StdActns.hpp>
#include <Dialogs.hpp>
#include <AppEvnts.hpp>

//---------------------------------------------------------------------------
class TFormUserMain : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
	TMenuItem *mnFile1;
	TMenuItem *mnNew1;
	TMenuItem *mnOpen1;
	TMenuItem *mnSave1;
    TMenuItem *N1;
	TMenuItem *mnExit1;
	TMenuItem *mnInstrument1;
	TMenuItem *mnAdd1;
	TMenuItem *mnRemove1;
	TMenuItem *mnDetails1;
	TMenuItem *mnPlayback1;
	TMenuItem *mnPlay1;
	TMenuItem *mnPause1;
	TMenuItem *mnStop1;
    TMenuItem *N2;
	TMenuItem *mnMidiOut;
	TMenuItem *mnHelp1;
    TActionList *ActionList1;
    TStatusBar *StatusBar1;
    TImageList *ImageList1;
    TToolBar *ToolBar1;
    TToolButton *ToolButton1;
    TToolButton *ToolButton2;
    TToolButton *ToolButton3;
    TToolButton *ToolButton4;
    TToolButton *ToolButton5;
    TToolButton *ToolButton6;
    TToolButton *ToolButton7;
    TToolButton *ToolButton8;
    TToolButton *ToolButton9;
    TToolButton *ToolButton10;
    TToolButton *ToolButton11;
	TAction *NewProject;
    TAction *AddInstrument;
    TAction *RemoveInstrument;
    TAction *InstrumentDetails;
    TAction *MixerPlay;
    TAction *MixerPause;
	TAction *MixerStop;
	TMenuItem *mnAbout1;
    TAction *ShowAbout;
    TFileExit *ExitApplication;
	TFileOpen *OpenProject;
	TFileSaveAs *SaveProjectAs;
	TAction *SaveProject;
    TAction *ChangeMeter;
    TAction *ChangeTempo;
	TMenuItem *mnSetTempo1;
	TMenuItem *mnSetMeter1;
    TAction *ChangeMidiOut;
	TFrameExecPlan *AgentsExecutionPanel;
	TAction *MuteMidiOut;
	TMenuItem *mnMute1;
	TAction *ChangeNumBars;
	TMenuItem *mnProject1;
	TMenuItem *mnBars1;
	TMenuItem *SaveProjectAs1;
	TMenuItem *N3;
	TMenuItem *ExportMIDIFile1;
	TMenuItem *mnRepeat1;
	TMenuItem *N4;
	TAction *MixerRepeat;
	TFileSaveAs *ExportMidiFile;
	TApplicationEvents *ApplicationEvents;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ShowAboutExecute(TObject *Sender);
    void __fastcall ShowHint(TObject *Sender);
	void __fastcall AddInstrumentExecute(TObject *Sender);
    void __fastcall NewProjectExecute(TObject *Sender);
    void __fastcall ChangeMeterExecute(TObject *Sender);
    void __fastcall ChangeMidiOutExecute(TObject *Sender);
    void __fastcall ChangeTempoExecute(TObject *Sender);
	void __fastcall ChangeNumBarsExecute(TObject *Sender);
	void __fastcall MixerPlayExecute(TObject *Sender);
	void __fastcall MixerPauseExecute(TObject *Sender);
	void __fastcall MixerStopExecute(TObject *Sender);
	void __fastcall MuteMidiOutExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall MixerRepeatExecute(TObject *Sender);
	void __fastcall OpenProjectAccept(TObject *Sender);
	void __fastcall SaveProjectAsAccept(TObject *Sender);
	void __fastcall SaveProjectExecute(TObject *Sender);
	void __fastcall ExportMidiFileAccept(TObject *Sender);
	void __fastcall ApplicationEventsIdle(TObject *Sender, bool &Done);
	void __fastcall RemoveInstrumentExecute(TObject *Sender);
	void __fastcall SaveProjectAsBeforeExecute(TObject *Sender);
	void __fastcall InstrumentDetailsExecute(TObject *Sender);
private:	// User declarations
	CbFacade* facade;
	void SetupPlaybackOptions(CbMixerAgent *mixer);
	void OnMixerStatusChanged(CbMixerAgent *mixer);
	bool __fastcall CheckSaveCurrentProject();
public:		// User declarations
	__fastcall TFormUserMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormUserMain *FormUserMain;
//---------------------------------------------------------------------------
#endif
