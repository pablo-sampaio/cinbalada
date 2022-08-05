//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AboutBox.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TFormAboutBox *FormAboutBox;
//---------------------------------------------------------------------
__fastcall TFormAboutBox::TFormAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TFormAboutBox::ProductLinkClick(TObject *Sender)
{
    ShellExecute(NULL, "open", ProductLink->Caption.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

