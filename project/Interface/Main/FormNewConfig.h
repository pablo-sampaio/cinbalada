//---------------------------------------------------------------------------

#ifndef FormNewConfigH
#define FormNewConfigH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#include <vector>
#include "CInBalada\CbFacade.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFormNewConfiguration : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox;
    TButton *ButtonOk;
    TButton *ButtonCancel;
    TLabel *LabelCycle;
    TComboBox *ComboBoxCycle;
	TLabel *LabelTempo;
    TLabel *LabelMeter;
    TComboBox *ComboBoxTempo;
	TImage *Image;
	TComboBox *ComboBoxMeter;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CheckEnableButtonOk(TObject *Sender);
	void __fastcall ComboBoxTempoChange(TObject *Sender);
	void __fastcall ComboBoxCycleChange(TObject *Sender);
private:	// User declarations
	AnsiString __fastcall filterNumbers(const AnsiString& string);
	bool __fastcall testTempoLimits(int tempo);
public:		// User declarations
	__fastcall TFormNewConfiguration(TComponent* Owner);

	int getNumBars();
	int getTempo();
	int getMeterIndex();
	const CbMeter* getMeter();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormNewConfiguration *FormNewConfiguration;
//---------------------------------------------------------------------------
#endif
