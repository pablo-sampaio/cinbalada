//----------------------------------------------------------------------------
#ifndef FormChngeBarsH
#define FormChngeBarsH
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
class TFormChangeBars : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
	TComboBox *ComboBoxBars;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ComboBoxBarsChange(TObject *Sender);
private:
	AnsiString __fastcall filterNumbers(const AnsiString& str);
public:
	virtual __fastcall TFormChangeBars(TComponent* AOwner);
	int getNumBars();
};
//----------------------------------------------------------------------------
extern PACKAGE TFormChangeBars *FormChangeBars;
//----------------------------------------------------------------------------
#endif    
