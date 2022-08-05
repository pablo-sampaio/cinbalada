//----------------------------------------------------------------------------
#ifndef FormChngeTempoH
#define FormChngeTempoH
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
#include <ComCtrls.hpp>
//----------------------------------------------------------------------------
class TFormChangeTempo : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
    TTrackBar *TrackBarTempo;
	TComboBox *ComboBoxTempo;
    void __fastcall TrackBarTempoChange(TObject *Sender);
	void __fastcall OKBtnClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ComboBoxTempoExit(TObject *Sender);
	void __fastcall ComboBoxTempoChange(TObject *Sender);
private:
    bool __fastcall CheckComboBoxTempo();
public:
	virtual __fastcall TFormChangeTempo(TComponent* AOwner);
	int __fastcall getTempo();  //in BPM
};
//----------------------------------------------------------------------------
extern PACKAGE TFormChangeTempo *FormChangeTempo;
//----------------------------------------------------------------------------
#endif    
