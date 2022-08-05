//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormChngeBars.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TFormChangeBars *FormChangeBars;
//---------------------------------------------------------------------
__fastcall TFormChangeBars::TFormChangeBars(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TFormChangeBars::FormShow(TObject *Sender)
{
	ComboBoxBars->Text = CbFacade::getInstance()->getBarsCycle();
}
//---------------------------------------------------------------------------
int TFormChangeBars::getNumBars() {
	return ComboBoxBars->Text.ToIntDef(-1);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormChangeBars::filterNumbers(const AnsiString& str) {
	AnsiString newTxt = str;
	byte c;

	for (int i = newTxt.Length(); i > 0 ; i --) {
		c = newTxt[i];
		if (c < '0' || c > '9') {
			newTxt.Delete(i, 1);
		}
	}

	return newTxt;
}
//---------------------------------------------------------------------------

void __fastcall TFormChangeBars::ComboBoxBarsChange(TObject *Sender)
{
	ComboBoxBars->Text = filterNumbers(ComboBoxBars->Text);
	OKBtn->Enabled = (ComboBoxBars->Text.ToIntDef(-1) > 0);
}
//---------------------------------------------------------------------------

