//---------------------------------------------------------------------------


#ifndef UnitFrameExperimentH
#define UnitFrameExperimentH
//---------------------------------------------------------------------------
#include "CInBalada\Experiment\CbExperiment.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFrameExperiment : public TFrame
{
__published:	// IDE-managed Components
	TListView *ListViewFirst;
	TListView *ListViewSecond;
private:	// User declarations
public:		// User declarations
	__fastcall TFrameExperiment(TComponent* Owner);

	void loadExperiment(CbExperiment* experiment);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameExperiment *FrameExperiment;
//---------------------------------------------------------------------------
#endif
