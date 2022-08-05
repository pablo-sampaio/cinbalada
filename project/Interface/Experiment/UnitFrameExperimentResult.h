//---------------------------------------------------------------------------


#ifndef UnitFrameExperimentResultH
#define UnitFrameExperimentResultH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include "CbExperimentResult.h"

#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFrameExperimentResult : public TFrame
{
__published:	// IDE-managed Components
	TLabel *LabelNumFiles;
	TEdit *EditNumFiles;
	TGroupBox *GroupBox1;
	TListView *ListViewBestFreq;
	TGroupBox *GroupBox2;
	TListView *ListViewNotesFreqAll;
	TGroupBox *GroupBox3;
	TListView *ListViewVariation;
	TGroupBox *GroupBox4;
	TListView *ListViewBestFreqAll;
	TGroupBox *GroupBox5;
	TListView *ListViewNotesFreq;
	TButton *ButtonAddExperiment;
	TOpenDialog *OpenDialog1;
	TComboBox *ComboBoxBestFreq;
	void __fastcall ButtonAddExperimentClick(TObject *Sender);
	void __fastcall ComboBoxBestFreqChange(TObject *Sender);
private:	// User declarations
	CbExperimentResult experiments;

	void fillBestFrequencies();
	void fillBestFrequenciesSummary();
	void fillNotesFrequencies();
	void fillNotesFrequenciesSummary();
	void fillVariation();

	int countAccordingToSelection(ECbEvaluationType type, int round);

public:		// User declarations
	__fastcall TFrameExperimentResult(TComponent* Owner);

	void AddExperiment(AnsiString fileName);
	void Clear();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameExperimentResult *FrameExperimentResult;
//---------------------------------------------------------------------------
#endif
