//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CInBalada\CbFacade.h"

#include "UserMainInterface.h"
#include "FormSplashScreen.h"
#include "AboutBox.h"
#include "FormNewConfig.h"
#include "FormAddInstr.h"
#include "FormChngeMeter.h"
#include "FormChngeTempo.h"
#include "FormChngeBars.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FrameExecutionPlan"

#pragma resource "*.dfm"

TFormUserMain *FormUserMain;

//---------------------------------------------------------------------------
__fastcall TFormUserMain::TFormUserMain(TComponent* Owner)
	: TForm(Owner)
{
	Application->OnHint = ShowHint;

	this->facade = CbFacade::getInstance();
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::FormShow(TObject *Sender)
{
	FormSplashScreen->ShowModal();

	CbMixerAgent* mixer = facade->getMixer();

	if (mixer->getID()->getStatus() == AG_ACTIVE) {
		this->SetupPlaybackOptions(mixer);
	} else {
		mixer->OnSetup = this->SetupPlaybackOptions;
	}

	mnRepeat1->Checked = this->facade->isRepeatingCycle();

	// dialogs open e save comecam do diretorio da aplicacao
	::ForceCurrentDirectory = true;
}
//---------------------------------------------------------------------------
void TFormUserMain::SetupPlaybackOptions(CbMixerAgent *mixer) {
	TMenuItem *newItem;

	for (int i = 0; i < mixer->getNumDrivers(); i ++) {
		newItem = new TMenuItem(mnPlayback1);
		newItem->Action = ChangeMidiOut;
		newItem->Caption = mixer->getDriverName(i);
		newItem->AutoCheck = true;
		newItem->RadioItem = true;
		newItem->GroupIndex = 0;
		mnMidiOut->Insert(i, newItem);
		if (i == 0) {
			newItem->Checked = true;
		}
	}

//	this->facade->loadProjectFile("Experimentos\\EAAA\\Projeto-EAAA.cib");

	mixer->OnStatusChange = this->OnMixerStatusChanged;

	OnMixerStatusChanged(mixer);
}
//---------------------------------------------------------------------------
void __fastcall TFormUserMain::FormClose(TObject *Sender,
	  TCloseAction &Action)
{
	if (CheckSaveCurrentProject()) {
		facade->getMixer()->stop();
		facade->getMixer()->OnStatusChange = NULL;
	} else {
		Action = caNone;
	}
}
//---------------------------------------------------------------------------
void TFormUserMain::OnMixerStatusChanged(CbMixerAgent *mixer) {

	switch (mixer->getStatus()) {
	case ES_PLAYING:
    	MixerPlay->Checked = true;
		ExportMidiFile->Enabled = false;  // nao pode salvar enquanto toca !
		break;
	case ES_PAUSED:
    	MixerPause->Checked = true;
		ExportMidiFile->Enabled = facade->hasRecorded();
		break;
	case ES_STOPPED:
    	MixerStop->Checked = true;
		ExportMidiFile->Enabled = facade->hasRecorded();
		break;
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::ShowAboutExecute(TObject *Sender)
{
	FormAboutBox->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::ShowHint(TObject *Sender)
{
	StatusBar1->SimpleText = Application->Hint;
}
//----------------------------------------------------------------------------

void __fastcall TFormUserMain::NewProjectExecute(TObject *Sender)
{
	if (CheckSaveCurrentProject()) {
		if (FormNewConfiguration->ShowModal() == mrOk) {
			int numBars = FormNewConfiguration->getNumBars();
			int meterIndex = FormNewConfiguration->getMeterIndex();
			int tempo = FormNewConfiguration->getTempo();

			this->facade->newProject("unnamed", numBars, tempo, meterIndex);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::OpenProjectAccept(TObject *Sender)
{
	if (CheckSaveCurrentProject()) {
		this->facade->loadProjectFile(OpenProject->Dialog->FileName);
		AnsiString projectFile = this->facade->getProjectFileName();
		//ShowMessage("Projeto \"" + projectFile + "\" carregado com sucesso!");
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::SaveProjectAsBeforeExecute(TObject *Sender)
{
	SaveProjectAs->Dialog->FileName = this->facade->getProjectFileName();
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::SaveProjectAsAccept(TObject *Sender)
{
	this->facade->saveProjectFile(SaveProjectAs->Dialog->FileName);
	SaveProject->OnExecute(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::SaveProjectExecute(TObject *Sender)
{
	this->facade->saveProjectFile();
	AnsiString projectFile = this->facade->getProjectFileName();
	//ShowMessage("Projeto \"" + projectFile + "\" salvo com sucesso!");
}
//---------------------------------------------------------------------------

bool __fastcall TFormUserMain::CheckSaveCurrentProject()
{
	bool mayContinue = true;

	if (facade->projectHasChanged()) {
		int result = MessageDlg("Save changes to current project?", mtConfirmation,
								TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

		if (result == mrYes) {
			if (this->facade->getProjectFileName() == "") {
				SaveProjectAs->Execute();
			}
			this->facade->saveProjectFile();
			mayContinue = true;
		} else if (result == mrNo) {
			mayContinue = true;
		} else {
			mayContinue = false;
		}
	}

	return mayContinue;
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::ChangeMeterExecute(TObject *Sender)
{
	if (FormChangeMeter->ShowModal() == mrOk) {
		this->facade->setCurrentMeter(FormChangeMeter->getMeterIndex());
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::ChangeTempoExecute(TObject *Sender)
{
	if (FormChangeTempo->ShowModal() == mrOk) {
		this->facade->setTempo(FormChangeTempo->getTempo());
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::ChangeNumBarsExecute(TObject *Sender)
{
	if (FormChangeBars->ShowModal() == mrOk) {
		int numBars = FormChangeBars->getNumBars();
		this->facade->setBarsCycle(numBars);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::AddInstrumentExecute(TObject *Sender)
{
	if (FormAddInstrument->ShowModal() == mrOk) {
		AnsiString instr = FormAddInstrument->getInstrument();
		AnsiString role = FormAddInstrument->getRole();
		facade->addAgent(instr, instr.c_str(), role.c_str());
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::RemoveInstrumentExecute(TObject *Sender)
{
	int agentIndex = AgentsExecutionPanel->GetSelectedAgent();
	if (agentIndex != -1) {
		this->facade->removeAgent(agentIndex);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::InstrumentDetailsExecute(TObject *Sender)
{
	CbPercussionistAgent *agent = this->facade->getAgent(AgentsExecutionPanel->GetSelectedAgent());
	//FormAgentDetails->Caption = AnsiString("Details of agent ") + agent->getName().c_str();
	//FormAgentDetails->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::ChangeMidiOutExecute(TObject *Sender)
{
	// encontra a 'nova' saida e seta-a no mixer
	for (int i = 0; i < mnMidiOut->Count; i++) {
		if (mnMidiOut->Items[i]->Checked) {
			facade->getMixer()->setDriver(i);
			break;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::MixerPlayExecute(TObject *Sender)
{
	this->facade->getMixer()->start(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::MixerPauseExecute(TObject *Sender)
{
	this->facade->getMixer()->pause();
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::MixerStopExecute(TObject *Sender)
{
	this->facade->getMixer()->stop();
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::MixerRepeatExecute(TObject *Sender)
{
	this->facade->toggleRepeatCycle();
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::MuteMidiOutExecute(TObject *Sender)
{
	this->facade->getMixer()->toggleMute();
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::ExportMidiFileAccept(TObject *Sender)
{
	AnsiString fileToExport = ExportMidiFile->Dialog->FileName;
	this->facade->exportRecordingToMidi(fileToExport.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFormUserMain::ApplicationEventsIdle(TObject *Sender,
	  bool &Done)
{
	if (this->Visible == true) {
		SaveProject->Enabled = (this->facade->getProjectFileName() != "")
		                       && this->facade->projectHasChanged();
		RemoveInstrument->Enabled = (AgentsExecutionPanel->GetSelectedAgent() != -1)
									&& (this->facade->getMixer()->getStatus() == ES_STOPPED);
		InstrumentDetails->Enabled = (AgentsExecutionPanel->GetSelectedAgent() != -1);

		MixerRepeat->Checked = this->facade->isRepeatingCycle();
		MuteMidiOut->Checked = this->facade->getMixer()->isMuted();

		AgentsExecutionPanel->CheckFacadeUpdate();
	}
}
//---------------------------------------------------------------------------

