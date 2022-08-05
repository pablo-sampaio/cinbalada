//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormAgntDetails.h"
#include "CbFacade.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAgentDetails *FormAgentDetails;
//---------------------------------------------------------------------------
__fastcall TFormAgentDetails::TFormAgentDetails(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormAgentDetails::FormShow(TObject *Sender)
{
	// show hint for dynamics
	TrackBarDynamics->Hint = AnsiString(TrackBarDynamics->Position) + "%";

	// show hint for temporal deviations
	AnsiString theHint;
	double left = 1.0 + (TrackBarTemporalDev->Position / 10.0);
	double right = 2.0 - (TrackBarTemporalDev->Position / 10.0);
	theHint.sprintf("%3.1f : %3.1f", left, right);
	TrackBarTemporalDev->Hint = theHint;

}
//---------------------------------------------------------------------------

void __fastcall TFormAgentDetails::TrackBarDynamicsChange(TObject *Sender)
{
	TrackBarDynamics->Hint = AnsiString(TrackBarDynamics->Position) + "%";
}
//---------------------------------------------------------------------------

void __fastcall TFormAgentDetails::TrackBarTemporalDevChange(
	  TObject *Sender)
{
	AnsiString theHint;

	double left = 1.0 + (TrackBarTemporalDev->Position / 10.0);
	double right = 2.0 - (TrackBarTemporalDev->Position / 10.0);

	theHint.sprintf("%3.1f : %3.1f", left, right);

	TrackBarTemporalDev->Hint = theHint;
}
//---------------------------------------------------------------------------

void __fastcall TFormAgentDetails::TrackBarAnticipationChange(
	  TObject *Sender)
{
	AnsiString theHint;
	theHint.sprintf("%3.1f tatum", float(TrackBarAnticipation->Position - 10)/10.0);
	TrackBarAnticipation->Hint = theHint;
}
//---------------------------------------------------------------------------

void __fastcall TFormAgentDetails::TrackBarVertHorizRatioChange(
      TObject *Sender)
{
	double trackBarLength = TrackBarVertHorizRatio->Max - TrackBarVertHorizRatio->Min;

	double vertical = TrackBarVertHorizRatio->Position / trackBarLength;
   	double horizontal = (TrackBarVertHorizRatio->Max - TrackBarVertHorizRatio->Position) / trackBarLength;

	AnsiString theHint;
	theHint.sprintf("%2.0f%% : %2.0f%%", 100.0 * vertical, 100.0 * horizontal);

	TrackBarVertHorizRatio->Hint = theHint;

	// seta para todos os agentes
	CbFacade* facade = CbFacade::getInstance();
    for (int i = facade->getNumAgents() - 1; i >= 0; i --) {
    	facade->getAgent(i)->setEvaluationWeigths(vertical, horizontal);
    }

}
//---------------------------------------------------------------------------

