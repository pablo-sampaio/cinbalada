//----------------------------------------------------------------------------
#ifndef FormChngeMeterH
#define FormChngeMeterH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//----------------------------------------------------------------------------
#include <vector>
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
class TFormChangeMeter : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
    TComboBox *ComboBoxMeter;
    TLabel *LabelMeter;
	void __fastcall ComboBoxMeterChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:
	std::vector<CbMeter*> meters;
public:
	virtual __fastcall TFormChangeMeter(TComponent* AOwner);
	int getMeterIndex();
	void setMeterIndex(int i);
};
//----------------------------------------------------------------------------
extern PACKAGE TFormChangeMeter *FormChangeMeter;
//----------------------------------------------------------------------------
#endif    
