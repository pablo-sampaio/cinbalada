//---------------------------------------------------------------------------

#ifndef FormAgntDetailsH
#define FormAgntDetailsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormAgentDetails : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBoxGeneralInfo;
	TGroupBox *GroupBoxPerformanceHistory;
	TGroupBox *GroupBoxSettings;
	TLabel *LabelName;
	TLabel *LabelInstrument;
	TLabel *LabelStatus;
	TTrackBar *TrackBarDynamics;
	TLabel *LabelDynamics;
	TLabel *LabelTemporalDev;
	TTrackBar *TrackBarTemporalDev;
	TLabel *LabelAnticipation;
	TTrackBar *TrackBarAnticipation;
	TTrackBar *TrackBarVertHorizRatio;
	TLabel *LabelVertHorizRatio;
	TLabel *LabelDefaultRole;
	void __fastcall TrackBarDynamicsChange(TObject *Sender);
	void __fastcall TrackBarTemporalDevChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TrackBarAnticipationChange(TObject *Sender);
	void __fastcall TrackBarVertHorizRatioChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormAgentDetails(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAgentDetails *FormAgentDetails;
//---------------------------------------------------------------------------
#endif
