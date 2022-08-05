//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormSplashScreen.h"

#include "CInBalada\CbFacade.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormSplashScreen *FormSplashScreen;

//---------------------------------------------------------------------------
__fastcall TFormSplashScreen::TFormSplashScreen(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormSplashScreen::Timer1Timer(TObject *Sender)
{
	CbFacade::getInstance()->fillRoleSupportMatrix(); // inicializacoes...
	Timer1->Enabled = false;
	this->ModalResult = mrOk;
	this->Visible = false;
}
//---------------------------------------------------------------------------
