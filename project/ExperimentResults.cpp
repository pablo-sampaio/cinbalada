//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Interface\Experiment\UnitExperimentMain.cpp", FormExperimentMain);
USEFORM("Interface\Experiment\UnitFrameExperiment.cpp", FrameExperiment); /* TFrame: File Type */
USEFORM("Interface\Experiment\UnitFrameExperimentResult.cpp", FrameExperimentResult); /* TFrame: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TFormExperimentMain), &FormExperimentMain);
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
