//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Interface\TestsInterface.cpp", FormTests);
USEFORM("Interface\Controls\FrameExecutionPlan.cpp", FrameExecPlan); /* TFrame: File Type */
USEFORM("Interface\Main\UserMainInterface.cpp", FormUserMain);
USEFORM("Interface\Main\AboutBox.cpp", FormAboutBox);
USEFORM("Interface\Main\FormAddInstr.cpp", FormAddInstrument);
USEFORM("Interface\Main\FormChngeMeter.cpp", FormChangeMeter);
USEFORM("Interface\Main\FormChngeTempo.cpp", FormChangeTempo);
USEFORM("Interface\Main\FormNewConfig.cpp", FormNewConfiguration);
USEFORM("Interface\Main\FormSplashScreen.cpp", FormSplashScreen);
USEFORM("Interface\Main\FormChngeBars.cpp", FormChangeBars);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
		Application->Initialize();
		Application->Title = "CInBalada";
		Application->CreateForm(__classid(TFormUserMain), &FormUserMain);
		Application->CreateForm(__classid(TFormTests), &FormTests);
		Application->CreateForm(__classid(TFormAboutBox), &FormAboutBox);
		Application->CreateForm(__classid(TFormAddInstrument), &FormAddInstrument);
		Application->CreateForm(__classid(TFormChangeMeter), &FormChangeMeter);
		Application->CreateForm(__classid(TFormChangeTempo), &FormChangeTempo);
		Application->CreateForm(__classid(TFormNewConfiguration), &FormNewConfiguration);
		Application->CreateForm(__classid(TFormSplashScreen), &FormSplashScreen);
		Application->CreateForm(__classid(TFormChangeBars), &FormChangeBars);
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
