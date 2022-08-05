//---------------------------------------------------------------------------

#ifndef TestsInterfaceH
#define TestsInterfaceH
//---------------------------------------------------------------------------
#include "CInBalada\CbFacade.h"
#include "CInBalada\Statistics\CbAgentChoicesStats.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <AppEvnts.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormTests : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *LabelMeter;
	TLabel *LabelA;
	TLabel *LabelB;
	TLabel *LabelC;
	TComboBox *ComboBoxFiles;
	TEdit *EditFullTubs;
	TEdit *EditBars;
	TEdit *EditNumBars;
	TEdit *EditPrefTempo;
	TEdit *EditTempo;
	TEdit *EditMeter;
	TButton *ButtonSave;
	TEdit *EditFileName;
	TButton *Button1;
	TListView *ListViewRoles;
	TButton *ButtonPlusThree;
	TButton *ButtonMinusTwo;
	TEdit *EditAddRole;
	TEdit *EditPrintf;
	TLabel *LabelPrintf;
	TButton *ButtonDoTest;
	TLabel *LabelString;
	TEdit *EditString;
	TTabSheet *TabSheet3;
	TOpenDialog *OpenProjectDialog;
	TSaveDialog *SaveOutputDialog;
	TGroupBox *GroupBox1;
	TEdit *EditProjectName;
	TButton *ButtonChooseProject;
	TGroupBox *GroupBox2;
	TEdit *EditOutputFile;
	TButton *ButtonOutputFile;
	TButton *ButtonStartProcessing;
	TApplicationEvents *ApplicationEvents1;
	TEdit *EditNumIterations;
	TLabel *LabelNumIterations;
	TGroupBox *GroupBox3;
	TLabel *LabelTotalTime;
	TLabel *LabelCurrIteration;
	TEdit *EditCurrIteration;
	TEdit *EditTotalTime;
	TCheckBox *CheckBoxRandomAgent;
	TLabel *LabelRpMute;
	TEdit *EditRpMute;
	TButton *ButtonCreateDir;
	TCheckBox *CheckBoxSingleFuncAgent;
	TRadioGroup *RadioGroup1;
	TTabSheet *TabSheet4;
	TButton *ButtonXmlTest;
	TMemo *MemoXmlTest;
	void __fastcall ComboBoxFilesChange(TObject *Sender);
	void __fastcall ButtonSaveClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall TabSheet2Show(TObject *Sender);
	void __fastcall ButtonPlusThreeClick(TObject *Sender);
	void __fastcall ButtonMinusTwoClick(TObject *Sender);
	void __fastcall ButtonDoTestClick(TObject *Sender);
	void __fastcall ButtonChooseProjectClick(TObject *Sender);
	void __fastcall ButtonOutputFileClick(TObject *Sender);
	void __fastcall ButtonStartProcessingClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonCreateDirClick(TObject *Sender);
	void __fastcall CheckBoxSingleFuncAgentClick(TObject *Sender);
	void __fastcall CheckBoxRandomAgentClick(TObject *Sender);
	void __fastcall RadioGroup1Click(TObject *Sender);
	void __fastcall ButtonXmlTestClick(TObject *Sender);

private:	// User declarations

	int performanceNumBars;

	CbAgentChoicesStats statistics;
	int currentIteration;
	int numIterations;
	DWORD startTime;

	AnsiString applicationDirectory;
	CbFacade* facade;

	void adjustOutputFileName();

	void disableStatisticsControls();
	void enableStatisticsControls();

	void OnMixerStatusChanged(CbMixerAgent *mixer);
	void addPercAgentStatistics();
	void addRandomAgentStatistics();
	void savePerformanceMidiFile(CbAgentChoicesStats::Tuple tuple);

public:		// User declarations
	__fastcall TFormTests(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTests *FormTests;
//---------------------------------------------------------------------------
#endif
