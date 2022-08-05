//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormChngeMeter.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TFormChangeMeter *FormChangeMeter;
//---------------------------------------------------------------------
__fastcall TFormChangeMeter::TFormChangeMeter(TComponent* AOwner)
	: TForm(AOwner)
{
	CbFacade* facade = CbFacade::getInstance();
	for (int i = 0; i < facade->getNumMeters(); i ++) {
		ComboBoxMeter->Items->Add(facade->getMeterName(i));
	}
}
//---------------------------------------------------------------------
void __fastcall TFormChangeMeter::FormShow(TObject *Sender)
{
	ComboBoxMeter->ItemIndex = CbFacade::getInstance()->getCurrentMeterIndex();
	//OKBtn->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormChangeMeter::ComboBoxMeterChange(TObject *Sender)
{
	OKBtn->Enabled = (ComboBoxMeter->ItemIndex != -1);
}
//---------------------------------------------------------------------------
int TFormChangeMeter::getMeterIndex() {
	return ComboBoxMeter->ItemIndex;
}
//---------------------------------------------------------------------------
void TFormChangeMeter::setMeterIndex(int i) {
	ComboBoxMeter->ItemIndex = i;
}
//---------------------------------------------------------------------------
