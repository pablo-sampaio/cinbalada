//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormNewConfig.h"

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormNewConfiguration *FormNewConfiguration;

//---------------------------------------------------------------------------
__fastcall TFormNewConfiguration::TFormNewConfiguration(TComponent* Owner)
	: TForm(Owner)
{
	CbFacade* facade = CbFacade::getInstance();

	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_VERY_SLOW));
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_SLOW));
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_MED_SLOW));
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_MODERATE));
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_MED_FAST));
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_FAST));
	ComboBoxTempo->Items->Add(CbMeter::tempoLabelToStr(TL_VERY_FAST));

	for (int i = 0; i < facade->getNumMeters(); i ++) {
		ComboBoxMeter->Items->Add(facade->getMeterName(i));
	}
}
//---------------------------------------------------------------------------
int TFormNewConfiguration::getNumBars() {
	return ComboBoxCycle->Text.ToIntDef(-1);
}
//---------------------------------------------------------------------------
int TFormNewConfiguration::getTempo() {
	int t = -1;
	const CbMeter* meter = getMeter();

	if (ComboBoxTempo->ItemIndex == -1) {
		t = ComboBoxTempo->Text.ToIntDef(-1);

	} else if (meter != NULL) {
		AnsiString labelStr = ComboBoxTempo->Items->Strings[ComboBoxTempo->ItemIndex];
		CbETempoLabel tl = CbMeter::strToTempoLabel(labelStr.c_str());

		t = meter->getTypicalTempo(tl);
	}

	return t;
}
//---------------------------------------------------------------------------
const CbMeter* TFormNewConfiguration::getMeter() {
	const CbMeter* m;

	if (ComboBoxMeter->ItemIndex != -1) {
		m = CbFacade::getInstance()->getMeter(ComboBoxMeter->ItemIndex);
	} else {
		m = NULL;
	}

	return m;
}
//---------------------------------------------------------------------------
int TFormNewConfiguration::getMeterIndex() {
	return ComboBoxMeter->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TFormNewConfiguration::FormShow(TObject *Sender)
{
	ComboBoxCycle->ItemIndex = -1;
	ComboBoxTempo->Text = "";
	ComboBoxTempo->ItemIndex = -1;
	ComboBoxMeter->ItemIndex = -1;
	ButtonOk->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormNewConfiguration::CheckEnableButtonOk(TObject *Sender)
{
	bool cycleSet =
			(ComboBoxCycle->ItemIndex != -1 || ComboBoxCycle->Text.ToIntDef(-1) > 0);

	bool tempoSet =
			(ComboBoxTempo->ItemIndex != -1 || testTempoLimits(ComboBoxTempo->Text.ToIntDef(-1)));

	bool meterSet =
			(ComboBoxMeter->ItemIndex != -1);

	ButtonOk->Enabled = (cycleSet && tempoSet && meterSet);
}
//---------------------------------------------------------------------------

bool  __fastcall TFormNewConfiguration::testTempoLimits(int tempo) {
	const CbMeter* meter = getMeter();

	if (meter == NULL) {
		return false;
	} else {
		return (tempo >= meter->getMinTempo() && tempo <= meter->getMaxTempo());
	}
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormNewConfiguration::filterNumbers(const AnsiString& str) {
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
void __fastcall TFormNewConfiguration::ComboBoxTempoChange(TObject *Sender)
{
	ComboBoxTempo->Text = filterNumbers(ComboBoxTempo->Text);
	CheckEnableButtonOk(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormNewConfiguration::ComboBoxCycleChange(TObject *Sender)
{
	ComboBoxCycle->Text = filterNumbers(ComboBoxCycle->Text);
	CheckEnableButtonOk(Sender);
}
//---------------------------------------------------------------------------

