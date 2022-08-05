//---------------------------------------------------------------------------

#ifndef UnitFormRepertoireMainH
#define UnitFormRepertoireMainH
//---------------------------------------------------------------------------
#include "CInbalada\RhythmPattern\CbXmlRhythmPatternInfoRepository.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <Buttons.hpp>
#include "Interface\Controls\RhythmPatternControl.h"
//---------------------------------------------------------------------------
class TFormRepertoireMain : public TForm
{
__published:	// IDE-managed Components
    TPanel *PanelMain;
    TPageControl *PageControl;
    TTabSheet *TabSheetFiles;
    TToolBar *ToolBarFiles;
    TToolButton *ToolButtonAddFile;
    TToolButton *ToolButtonAddDir;
    TToolButton *ToolButtonSeparator;
    TToolButton *ToolButtonRemoveFile;
    TListView *ListViewFiles;
    TTabSheet *TabSheetEditFile;
    TTabSheet *TabSheetComparator;
    TStatusBar *StatusBar;
    TOpenDialog *OpenDialogFiles;
    TImageList *ImageList1;
    TGroupBox *GroupBoxPatterns;
    TPanel *PanelPatternComp2;
    TLabel *LabelPatternComp2;
    TComboBox *ComboBoxPatternComp2;
    TPanel *PanelPatternComp1;
    TLabel *LabelPatternComp1;
    TComboBox *ComboBoxPatternComp1;
    TBitBtn *BitBtnPlayComp1;
    TGroupBox *GroupBoxVisualization;
    TPanel *PanelVisualizationTp;
	TLabel *LabelGraphRepr;
	TComboBox *ComboBoxGraphRepr;
	TRhythmPatternControl *RhythmPatternControl;
	TTabSheet *TabSheetInstruments;
	TGroupBox *GroupBoxInstrumentInfo;
	TLabel *LabelInstrName;
	TLabel *LabelFreqClass;
	TEdit *EditInstrName;
	TComboBox *ComboBoxFreqClass;
	TComboBox *ComboBoxMidiNote;
	TButton *ButtonAddInstrNote;
	TLabel *LabelInstrNotes;
	TButton *ButtonRemoveInstrNote;
	TListView *ListViewInstruments;
	TBevel *Bevel2;
	TBevel *Bevel3;
	TButton *ButtonNewInstrSave;
	TListView *ListViewInstrNotes;
	TButton *ButtonDeleteInstr;
	TLabel *LabelMdNote;
	TLabel *LabelRelPitch;
	TComboBox *ComboBoxNoteRelPitch;
	TListView *ListViewEditFiles;
	TGroupBox *GroupBoxRPInfo;
	TLabel *LabelType;
	TComboBox *ComboBoxType;
	TLabel *LabelMeter;
	TLabel *LabelInstrument;
	TComboBox *ComboBoxInstrument;
	TBevel *Bevel1;
	TBitBtn *ButtonSave;
	TLabel *LabelMusicalStyles;
	TCheckListBox *CheckListBoxStyles;
	TButton *ButtonAddStyle;
	TEdit *EditAddStyle;
	TButton *ButtonRemoveStyle;
	TEdit *EditMidiFile;
	TLabel *Label1;
	TEdit *EditNumTrack;
	TLabel *LabelNumTrack;
	TBitBtn *BitBtnPlayComp2;
	TBitBtn *BitBtnPauseComp1;
	TBitBtn *BitBtnPauseComp2;
	TTabSheet *TabSheetMidiInfo;
	TGroupBox *GroupBoxMiChEvents;
	TListView *ListViewMiChEvents;
	TGroupBox *GroupBoxMiGeneralInfo;
	TEdit *EditMiTempo;
	TLabel *LabelMiTracks;
	TEdit *EditMiTracks;
	TLabel *LabelMiTempo;
	TLabel *LabelMiResolution;
	TEdit *EditMiResolution;
	TBevel *Bevel4;
	TLabel *LabelMiMeter;
	TLabel *LabelMiMaxTick;
	TEdit *EditMiMeter;
	TEdit *EditMiMaxTick;
	TLabel *LabelMiBars;
	TEdit *EditMiBars;
	TGroupBox *GroupBoxAvaliation;
	TPanel *PanelMiSuperiorPanel;
	TLabel *LabelMiFile;
	TComboBox *ComboBoxMiFile;
	TLabel *LabelAvCriteria;
	TComboBox *ComboBoxAvCriteria;
	TEdit *EditAvaliationResult;
	TLabel *LabelGraphAlignment;
	TComboBox *ComboBoxGraphAlignment;
	TLabel *LabelName;
	TEdit *EditName;
	TComboBox *ComboBoxMeter;
    void __fastcall FormCreate(TObject *Sender);
	void __fastcall ToolButtonAddFileClick(TObject *Sender);
    void __fastcall ToolButtonAddDirClick(TObject *Sender);
    void __fastcall TabSheetFilesShow(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ToolButtonRemoveFileClick(TObject *Sender);
	void __fastcall TabSheetEditFileShow(TObject *Sender);
	void __fastcall ListViewEditFilesSelectItem(TObject *Sender,
		  TListItem *Item, bool Selected);
	void __fastcall ButtonAddStyleClick(TObject *Sender);
	void __fastcall ButtonRemoveStyleClick(TObject *Sender);
	void __fastcall EditNamexChange(TObject *Sender);
	void __fastcall ComboBoxTypeChange(TObject *Sender);
	void __fastcall ButtonSaveClick(TObject *Sender);
	void __fastcall ListViewFilesSelectItem(TObject *Sender,
		  TListItem *Item, bool Selected);
	void __fastcall TabSheetComparatorShow(TObject *Sender);
    void __fastcall ComboBoxGraphReprChange(TObject *Sender);
    void __fastcall ComboBoxPatternComp1Change(TObject *Sender);
    void __fastcall ComboBoxPatternComp2Change(TObject *Sender);
    void __fastcall BitBtnPlayComp1Click(TObject *Sender);
    void __fastcall BitBtnPlayComp2Click(TObject *Sender);
    void __fastcall TabSheetComparatorHide(TObject *Sender);
	void __fastcall TabSheetInstrumentsShow(TObject *Sender);
	void __fastcall ButtonNewInstrSaveClick(TObject *Sender);
	void __fastcall ListViewInstrumentsSelectItem(TObject *Sender,
          TListItem *Item, bool Selected);
	void __fastcall ButtonAddInstrNoteClick(TObject *Sender);
	void __fastcall ButtonRemoveInstrNoteClick(TObject *Sender);
	void __fastcall EditInstrNameChange(TObject *Sender);
	void __fastcall ComboBoxFreqClassChange(TObject *Sender);
	void __fastcall ListViewInstrNotesSelectItem(TObject *Sender,
          TListItem *Item, bool Selected);
	void __fastcall ButtonDeleteInstrClick(TObject *Sender);
	void __fastcall ComboBoxInstrumentChange(TObject *Sender);
	void __fastcall EditNumTrackChange(TObject *Sender);
	void __fastcall BitBtnPauseComp1Click(TObject *Sender);
	void __fastcall BitBtnPauseComp2Click(TObject *Sender);
	void __fastcall TabSheetMidiInfoShow(TObject *Sender);
	void __fastcall ComboBoxMiFileChange(TObject *Sender);
	void __fastcall TabSheetMidiInfoHide(TObject *Sender);
	void __fastcall ComboBoxAvCriteriaChange(TObject *Sender);
	void __fastcall ComboBoxGraphAlignmentChange(TObject *Sender);
	void __fastcall ComboBoxMeterChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations

	AnsiString applicationDirectory;
	ICbRpInfoRepository* rhythmRepository;
	int midiFile1, midiFile2;   // indexes of the rhythm patterns in the sequencer (sheet "Comparator")
	FmMidiFile* miMidiFile;     // file chosen in "Midi Info" sheet
	int numDigits;              // number of digits used when showing the ids of the patterns

	ICbRpInfoRepository* getRhythmRepository();
	void deleteRhythmRepository();

	void addFileToRepertoire(AnsiString filePath);
	AnsiString copyFileToDir(AnsiString filePath, AnsiString destDirPath);

	void updateStatusBar();
	void updateVisualizationComboBoxes();

public:		// User declarations
	__fastcall TFormRepertoireMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormRepertoireMain *FormRepertoireMain;
//---------------------------------------------------------------------------
#endif
