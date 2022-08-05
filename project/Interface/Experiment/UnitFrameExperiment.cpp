//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitFrameExperiment.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrameExperiment *FrameExperiment;

//---------------------------------------------------------------------------

__fastcall TFrameExperiment::TFrameExperiment(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------

void TFrameExperiment::loadExperiment(CbExperiment* experiment) {
	ListViewFirst->Clear();
	ListViewSecond->Clear();

	TListItem* listItem;

	for (int i = 1; i <= CbExperiment::NUM_ROUNDS; i ++) {
		listItem = ListViewFirst->Items->Add();

		listItem->Caption = i;

		listItem->SubItems->Add(experiment->getQuality(i, 'A'));
		listItem->SubItems->Add(experiment->getQuality(i, 'B'));
		listItem->SubItems->Add(experiment->getQuality(i, 'C'));
		listItem->SubItems->Add(experiment->getBestSample(i));
		listItem->SubItems->Add(experiment->getWorstSample(i));
	}

	listItem = ListViewSecond->Items->Add();
	listItem->Caption = experiment->getVariety(1);

	for (int i = 2; i <= CbExperiment::NUM_SETS; i ++) {
		listItem->SubItems->Add(experiment->getVariety(i));
	}

	listItem->SubItems->Add(experiment->getMostVariated());
}
//---------------------------------------------------------------------------
