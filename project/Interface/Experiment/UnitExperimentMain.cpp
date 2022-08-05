//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitExperimentMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "UnitFrameExperiment"
#pragma link "UnitFrameExperimentResult"
#pragma resource "*.dfm"
TFormExperimentMain *FormExperimentMain;
//---------------------------------------------------------------------------
__fastcall TFormExperimentMain::TFormExperimentMain(TComponent* Owner)
	: TForm(Owner)
{
	CbExperiment experiment;

	experiment.load("D:\\Projetos\\CInBaladaXE\\experiment.txt");

	OpenDialog1->InitialDir = ExpandFileName(".");
}
//---------------------------------------------------------------------------
void __fastcall TFormExperimentMain::ButtonOpenExpFileClick(TObject *Sender)
{
	if (OpenDialog1->Execute() == true) {
		CbExperiment experiment (OpenDialog1->FileName);
		FrameExperiment1->loadExperiment(&experiment);
	}
}
//---------------------------------------------------------------------------


