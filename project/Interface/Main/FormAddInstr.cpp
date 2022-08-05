//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormAddInstr.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TFormAddInstrument *FormAddInstrument;
//---------------------------------------------------------------------
__fastcall TFormAddInstrument::TFormAddInstrument(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
AnsiString TFormAddInstrument::getInstrument() {
	return ComboBoxInstrument->Items->Strings[ComboBoxInstrument->ItemIndex];
}
//---------------------------------------------------------------------
AnsiString TFormAddInstrument::getRole() {
	return ComboBoxRole->Items->Strings[ComboBoxRole->ItemIndex];
}
//---------------------------------------------------------------------
void __fastcall TFormAddInstrument::FormShow(TObject *Sender)
{
	CbFacade* facade = CbFacade::getInstance();
	ICbRpInfoRepository* rep = facade->getRpInfoRepository();

	ComboBoxInstrument->Clear();

	Vector_String names;

	rep->getAllInstrumentNames(names);

	for (int i = 0; i < rep->getNumInstruments(); i ++) {
		ComboBoxInstrument->Items->Add(names[i].c_str());
	}

	ComboBoxRole->Clear();

	OKBtn->Enabled = false;
}
//---------------------------------------------------------------------------
void TFormAddInstrument::CheckOkButton() {
	OKBtn->Enabled = ComboBoxInstrument->ItemIndex != -1
						&& ComboBoxRole->ItemIndex != -1;
}
//---------------------------------------------------------------------------
void __fastcall TFormAddInstrument::ComboBoxInstrumentChange(
	  TObject *Sender)
{
	// monta a lista de papeis de acordo com o instrumento...
	ComboBoxRole->Items->Clear();

	int instrIndex = ComboBoxInstrument->ItemIndex;
	if (instrIndex != -1) {
		CbFacade* facade = CbFacade::getInstance();
		const char* roleName;

		for (int i = 0; i < facade->getNumRoles(); i ++) {
			roleName = facade->getRole(i)->getName();
			if (facade->instrumentHasPatternOfRole(instrIndex,roleName)) {
				ComboBoxRole->Items->Add(roleName);
			}
		}
	}

	CheckOkButton();
}
//---------------------------------------------------------------------------

void __fastcall TFormAddInstrument::ComboBoxRoleChange(TObject *Sender)
{
	CheckOkButton();
}
//---------------------------------------------------------------------------

