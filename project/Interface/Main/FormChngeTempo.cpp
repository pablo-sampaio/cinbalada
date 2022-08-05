//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormChngeTempo.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TFormChangeTempo *FormChangeTempo;
//---------------------------------------------------------------------
__fastcall TFormChangeTempo::TFormChangeTempo(TComponent* AOwner)
	: TForm(AOwner)
{
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_VERY_SLOW));
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_SLOW));
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_MED_SLOW));
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_MODERATE));
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_MED_FAST));
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_FAST));
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_VERY_FAST));

	ComboBoxTempo->Text = TrackBarTempo->Position;
}
//---------------------------------------------------------------------

void __fastcall TFormChangeTempo::FormShow(TObject *Sender)
{
	CbFacade* facade = CbFacade::getInstance();

	TrackBarTempo->Min = facade->getCurrentMeter()->getMinTempo();
	TrackBarTempo->Max = facade->getCurrentMeter()->getMaxTempo();
	TrackBarTempo->Position = facade->getMixer()->getTempo();

	ComboBoxTempo->Text = TrackBarTempo->Position;
}
//---------------------------------------------------------------------------
int __fastcall TFormChangeTempo::getTempo() {
	return TrackBarTempo->Position;
}
//---------------------------------------------------------------------

void __fastcall TFormChangeTempo::TrackBarTempoChange(TObject *Sender)
{
	ComboBoxTempo->Text = TrackBarTempo->Position;
}
//---------------------------------------------------------------------------

void __fastcall TFormChangeTempo::ComboBoxTempoChange(TObject *Sender)
{
	AnsiString newTxt = ComboBoxTempo->Text;
	byte c;

	if (ComboBoxTempo->ItemIndex == -1) {

		for (int i = newTxt.Length(); i > 0 ; i --) {
			c = newTxt[i];
			if (c < '0' || c > '9') {
				newTxt.Delete(i, 1);
			}
		}
		ComboBoxTempo->Text = newTxt;

	} else {
		CbFacade* facade = CbFacade::getInstance();

		AnsiString labelStr = ComboBoxTempo->Items->Strings[ComboBoxTempo->ItemIndex];
		CbETempoLabel tl = CbMeter::strToTempoLabel(labelStr.c_str());

		TrackBarTempo->Position = facade->getCurrentMeter()->getTypicalTempo(tl);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormChangeTempo::ComboBoxTempoExit(TObject *Sender)
{
	if (ActiveControl != CancelBtn) {
		if (CheckComboBoxTempo() == false) {
			ComboBoxTempo->Text = TrackBarTempo->Position;
		}
	}
}
//---------------------------------------------------------------------------

bool __fastcall TFormChangeTempo::CheckComboBoxTempo() {
	bool result;
	AnsiString msg;
	int n = ComboBoxTempo->Text.ToIntDef(-1);

	if (ComboBoxTempo->ItemIndex == -1) {
		if (n < TrackBarTempo->Min) {
			msg.sprintf("Value given is too low ! Mininum is %d.", TrackBarTempo->Min);
			ShowMessage(msg);
			ComboBoxTempo->Text = TrackBarTempo->Position;
			ComboBoxTempo->SetFocus();
		} else if (n > TrackBarTempo->Max) {
			msg.sprintf("Value given is too high ! Maximum is %d.", TrackBarTempo->Max);
			ShowMessage(msg);
			ComboBoxTempo->Text = TrackBarTempo->Position;
			ComboBoxTempo->SetFocus();
		} else {
			TrackBarTempo->Position = n;
		}

		result = (n >= TrackBarTempo->Min) && (n <= TrackBarTempo->Max);

	} else {
		result = true;

	}

	return result;
}

//---------------------------------------------------------------------------
void __fastcall TFormChangeTempo::OKBtnClick(TObject *Sender)
{
	if (CheckComboBoxTempo()) {
		this->ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
