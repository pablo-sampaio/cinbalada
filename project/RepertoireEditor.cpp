//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Interface\RepertoireEditor\FormChooseDir.cpp", FormChooseDir);
USEFORM("Interface\RepertoireEditor\FormRepertoireMain.cpp", FormRepertoireMain);
USEFORM("Interface\Main\FormSplashScreen.cpp", FormSplashScreen);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->Title = "CInBalada Repertoire Editor";
		Application->CreateForm(__classid(TFormRepertoireMain), &FormRepertoireMain);
		Application->CreateForm(__classid(TFormChooseDir), &FormChooseDir);
		Application->CreateForm(__classid(TFormSplashScreen), &FormSplashScreen);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
