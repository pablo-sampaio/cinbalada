//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitFrameExperimentResult.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrameExperimentResult *FrameExperimentResult;

//---------------------------------------------------------------------------

__fastcall TFrameExperimentResult::TFrameExperimentResult(TComponent* Owner)
	: TFrame(Owner)
{
	TListItem* listItem;

	listItem = ListViewNotesFreqAll->Items->Add();
	listItem->Caption = CbExperiment::PESSIMO;

	listItem = ListViewNotesFreqAll->Items->Add();
	listItem->Caption = CbExperiment::RUIM;

	listItem = ListViewNotesFreqAll->Items->Add();
	listItem->Caption = CbExperiment::BOM;

	listItem = ListViewNotesFreqAll->Items->Add();
	listItem->Caption = CbExperiment::EXCELENTE;
}
//---------------------------------------------------------------------------

void TFrameExperimentResult::Clear() {
	experiments.clear();

	ListViewBestFreq->Clear();
	ListViewBestFreqAll->Clear();

	ListViewNotesFreq->Clear();

	for (int i = 0; i < ListViewNotesFreqAll->Items->Count; i ++) {
		ListViewNotesFreqAll->Items->Item[i]->SubItems->Clear();
	}

	for (int i = 0; i < ListViewVariation->Items->Count; i ++) {
		ListViewVariation->Items->Item[i]->SubItems->Clear();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameExperimentResult::ButtonAddExperimentClick(
	  TObject *Sender)
{
	if (OpenDialog1->Execute() == true) {
		this->AddExperiment(OpenDialog1->FileName);
	}
}
//---------------------------------------------------------------------------

void TFrameExperimentResult::AddExperiment(AnsiString fileName) {
	experiments.add(fileName);
	EditNumFiles->Text = experiments.getNumExperiments();

	fillBestFrequencies();
	fillBestFrequenciesSummary();
	fillNotesFrequencies();
	fillNotesFrequenciesSummary();
	fillVariation();
}
//---------------------------------------------------------------------------

void TFrameExperimentResult::fillBestFrequencies() {
	ListViewBestFreq->Clear();

	TListItem* listItem;

	for (int round = 1; round <= CbExperiment::NUM_ROUNDS; round ++) {
		listItem = ListViewBestFreq->Items->Add();

		listItem->Caption = round;

		listItem->SubItems->Add(this->countAccordingToSelection(ET_RANDOM, round));
		listItem->SubItems->Add(this->countAccordingToSelection(ET_SINGLE, round));
		listItem->SubItems->Add(this->countAccordingToSelection(ET_ROLE, round));
	}
}
//---------------------------------------------------------------------------

int TFrameExperimentResult::countAccordingToSelection(ECbEvaluationType type, int round) {
	int result = 0;
	if (ComboBoxBestFreq->ItemIndex == 0) {
		result = experiments.countBest(type, round);
	} else if (ComboBoxBestFreq->ItemIndex == 1) {
		result = experiments.countSecond(type, round);
	} else if (ComboBoxBestFreq->ItemIndex == 2) {
		result = experiments.countWorst(type, round);
	}
	return result;
}
//---------------------------------------------------------------------------

void TFrameExperimentResult::fillBestFrequenciesSummary() {
	ListViewBestFreqAll->Clear();

	TListItem* listItem;

	listItem = ListViewBestFreqAll->Items->Add();
	listItem->Caption = "First";
	listItem->SubItems->Add(this->experiments.countBest(ET_RANDOM));
	listItem->SubItems->Add(this->experiments.countBest(ET_SINGLE));
	listItem->SubItems->Add(this->experiments.countBest(ET_ROLE));

	listItem = ListViewBestFreqAll->Items->Add();
	listItem->Caption = "Second";
	listItem->SubItems->Add(this->experiments.countSecond(ET_RANDOM));
	listItem->SubItems->Add(this->experiments.countSecond(ET_SINGLE));
	listItem->SubItems->Add(this->experiments.countSecond(ET_ROLE));

	listItem = ListViewBestFreqAll->Items->Add();
	listItem->Caption = "Third";
	listItem->SubItems->Add(this->experiments.countWorst(ET_RANDOM));
	listItem->SubItems->Add(this->experiments.countWorst(ET_SINGLE));
	listItem->SubItems->Add(this->experiments.countWorst(ET_ROLE));
}
//---------------------------------------------------------------------------

void TFrameExperimentResult::fillNotesFrequencies() {
	ListViewNotesFreq->Clear();

	int countPe;
	int countRu;
	int countBo;
	int countEx;
	AnsiString string;

	TListItem* listItem;

	for (int round = 1; round <= CbExperiment::NUM_ROUNDS; round ++) {
		listItem = ListViewNotesFreq->Items->Add();

		listItem->Caption = round;

		for (int type = ET_RANDOM; type <= ET_ROLE; type ++) {
			countPe = this->experiments.countQuality((ECbEvaluationType)type, round, CbExperiment::PESSIMO);
			countRu = this->experiments.countQuality((ECbEvaluationType)type, round, CbExperiment::RUIM);
			countBo = this->experiments.countQuality((ECbEvaluationType)type, round, CbExperiment::BOM);
			countEx = this->experiments.countQuality((ECbEvaluationType)type, round, CbExperiment::EXCELENTE);

			if (countPe != 0) {
				string.printf("%dp ", countPe);
			}
			if (countRu != 0) {
				string.cat_printf("%dr ", countRu);
			}
			if (countBo != 0) {
				string.cat_printf("%db ", countBo);
			}
			if (countEx != 0) {
				string.cat_printf("%de", countEx);
			}

			listItem->SubItems->Add(string);
			string = "";
		}

	}

}
//---------------------------------------------------------------------------

void TFrameExperimentResult::fillNotesFrequenciesSummary() {
	TListItem* listItem;
	char quality;

	for (int i = 0; i < ListViewNotesFreqAll->Items->Count; i ++) {
		listItem = ListViewNotesFreqAll->Items->Item[i];
		quality = listItem->Caption[1];

		listItem->SubItems->Clear();

		listItem->SubItems->Add(this->experiments.countQuality(ET_RANDOM, quality));
		listItem->SubItems->Add(this->experiments.countQuality(ET_SINGLE, quality));
		listItem->SubItems->Add(this->experiments.countQuality(ET_ROLE, quality));
	}

}
//---------------------------------------------------------------------------

void TFrameExperimentResult::fillVariation() {
	TListItem* listItem;

	const char* format = "%3.1lf";
	AnsiString string;

	// average

	listItem = ListViewVariation->Items->Item[0];
	listItem->SubItems->Clear();

	string.printf(format, this->experiments.getAverageVariety(ET_RANDOM));
	listItem->SubItems->Add(string);

	string.printf(format, this->experiments.getAverageVariety(ET_ROLE));
	listItem->SubItems->Add(string);

	// most variated

	listItem = ListViewVariation->Items->Item[1];
	listItem->SubItems->Clear();

	listItem->SubItems->Add(this->experiments.countMostVariated(ET_RANDOM));

	listItem->SubItems->Add(this->experiments.countMostVariated(ET_ROLE));

	// normalized average

	listItem = ListViewVariation->Items->Item[2];
	listItem->SubItems->Clear();

	AnsiString formated;

	formated.printf("%3.1lf%%", 100.0 * this->experiments.getNormalizedAverageVariety(ET_RANDOM));
	listItem->SubItems->Add(formated);

	formated.printf("%3.1lf%%", 100.0 * this->experiments.getNormalizedAverageVariety(ET_ROLE));
	listItem->SubItems->Add(formated);

}
//---------------------------------------------------------------------------
void __fastcall TFrameExperimentResult::ComboBoxBestFreqChange(
      TObject *Sender)
{
	fillBestFrequencies();
}
//---------------------------------------------------------------------------

