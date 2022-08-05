//---------------------------------------------------------------------------

#ifndef UnitExperimentMainH
#define UnitExperimentMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "UnitFrameExperiment.h"
#include <Dialogs.hpp>
#include "UnitFrameExperimentResult.h"
//---------------------------------------------------------------------------
class TFormExperimentMain : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TComboBox *ComboBox1;
	TLabel *Label2;
	TComboBox *ComboBox2;
	TLabel *Label3;
	TComboBox *ComboBox3;
	TLabel *Label4;
	TComboBox *ComboBox4;
	TLabel *Label5;
	TComboBox *ComboBox5;
	TButton *ButtonSave1;
	TLabel *Label6;
	TEdit *EditRodada;
	TGroupBox *GroupBox2;
	TLabel *Label7;
	TComboBox *ComboBox6;
	TLabel *Label8;
	TComboBox *ComboBox7;
	TLabel *Label9;
	TComboBox *ComboBox8;
	TButton *ButtonSave2;
	TTabSheet *TabSheet3;
	TGroupBox *GroupBox3;
	TFrameExperiment *FrameExperiment1;
	TButton *ButtonOpenExpFile;
	TOpenDialog *OpenDialog1;
	TFrameExperimentResult *FrameExperimentResult1;
	void __fastcall ButtonOpenExpFileClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormExperimentMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormExperimentMain *FormExperimentMain;
//---------------------------------------------------------------------------
#endif
