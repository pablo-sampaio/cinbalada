//----------------------------------------------------------------------------
#ifndef FormAddInstrH
#define FormAddInstrH
//----------------------------------------------------------------------------
#include "CInBalada\CbFacade.h"

#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TFormAddInstrument : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
    TImage *Image;
    TLabel *LabelInstrument;
    TComboBox *ComboBoxInstrument;
    TLabel *LabelRole;
    TComboBox *ComboBoxRole;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ComboBoxInstrumentChange(TObject *Sender);
	void __fastcall ComboBoxRoleChange(TObject *Sender);
private:
	void CheckOkButton();
public:
	virtual __fastcall TFormAddInstrument(TComponent* AOwner);
	AnsiString getInstrument();
	AnsiString getRole();
};
//----------------------------------------------------------------------------
extern PACKAGE TFormAddInstrument *FormAddInstrument;
//----------------------------------------------------------------------------
#endif    
