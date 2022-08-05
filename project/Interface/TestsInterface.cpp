//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TestsInterface.h"

#include <map>
#include <sstream>

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormTests *FormTests;

//---------------------------------------------------------------------------
__fastcall TFormTests::TFormTests(TComponent* Owner)
	: TForm(Owner)
{
	this->performanceNumBars = 2;

	this->currentIteration = 0;
	this->numIterations = 0;

	this->applicationDirectory = ExpandFileName(".");
	this->applicationDirectory = applicationDirectory + "\\midis\\";

	this->facade = NULL;

	PageControl1->ActivePage = TabSheet3;
}
//---------------------------------------------------------------------------
void __fastcall TFormTests::FormShow(TObject *Sender)
{
	this->facade = CbFacade::getInstance();
	this->facade->setDebug();

	Vector_pRpInfo v;
	this->facade->getRpInfoRepository()->getAllRpInfos(v);

	for (int i = 0; i < v.size(); i ++) {
		ComboBoxFiles->Items->Add(v[i]->getFileName().c_str());
	}
}

//---------------------------------------------------------------------------
// TAB SHEET 1
//---------------------------------------------------------------------------

void __fastcall TFormTests::ComboBoxFilesChange(TObject *Sender)
{
	if (ComboBoxFiles->ItemIndex == -1) {
		return;
	}

	const CbRpInfo* rpinfo =
		this->facade->getRpInfoRepository()->getRpInfoById(ComboBoxFiles->ItemIndex);
	CbRhythmPattern* pattern = new CbRhythmPattern(*rpinfo, applicationDirectory.c_str());

	std::ostringstream buffer;

	buffer << (* pattern->getTubs());

	std::string str = buffer.str();
	EditFullTubs->Text = str.c_str();

	buffer.str("");

	int numBars = pattern->getNumBars();
	EditNumBars->Text = numBars;

	CbRhythmPattern *bar;
	for (int i = 0; i < numBars; i ++) {
		bar = pattern->getBar(i);
		buffer << *(bar->getTubs());
		delete bar;
	}

	str = buffer.str();
	EditBars->Text = str.c_str();

	const CbMeter *meter = pattern->getMeter();

	buffer.str("");
	buffer << (* meter->getBeatsRhythmPattern()->getTubs());

	str = buffer.str();
	AnsiString caption = str.c_str();

	EditMeter->Text = caption;

	EditTempo->Text = meter->getMinTempo();
	EditTempo->Text = EditTempo->Text + "-";
	EditTempo->Text = EditTempo->Text + meter->getMaxTempo();

	EditPrefTempo->Text = meter->getMinPreferedTempo();
	EditPrefTempo->Text = EditPrefTempo->Text + "-";
	EditPrefTempo->Text = EditPrefTempo->Text + meter->getMaxPreferedTempo();

	Button1Click(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormTests::ButtonSaveClick(TObject *Sender)
{
	const CbRpInfo* rpinfo =
		this->facade->getRpInfoRepository()->getRpInfoById(ComboBoxFiles->ItemIndex);
	AnsiString filePath = applicationDirectory + AnsiString(rpinfo->getFileName().c_str());

	FmMidiFile* midiFile = new FmMidiFile(filePath.c_str());

	midiFile->setNumerator(midiFile->getNumerator() - 1);

	AnsiString destDir = applicationDirectory + EditFileName->Text;
	midiFile->saveToFile(destDir.c_str());

	ShowMessage("Teoreticamente, o arquivo foi salvo !");
}
//---------------------------------------------------------------------------

void __fastcall TFormTests::Button1Click(TObject *Sender)
{
	const CbRpInfo* rpinfo =
		this->facade->getRpInfoRepository()->getRpInfoById(ComboBoxFiles->ItemIndex);
	CbRhythmPattern* pattern = new CbRhythmPattern(*rpinfo, applicationDirectory.c_str());
	const CbMeter* meter = pattern->getMeter();

	LabelA->Caption = meter->getTypicalTempo(TL_VERY_FAST);
	LabelB->Caption = meter->getTypicalTempo(TL_SLOW);
	LabelC->Caption = meter->getTypicalTempo(TL_MED_SLOW);
}

//---------------------------------------------------------------------------
// TAB SHEET 2
//---------------------------------------------------------------------------

void __fastcall TFormTests::TabSheet2Show(TObject *Sender)
{
	CbFacade* facade = CbFacade::getInstance();
	static bool firstTime = true;

	if (firstTime) {
		facade->setBarsCycle(1);
		CbFacade::getInstance()->addAgent("agogo", "agogo", "solo");
		firstTime = false;
	}

	CbPercussionistAgent* agent = facade->getAgent(0);
	TListItem* item;

	ListViewRoles->Clear();
	for (int i = 0; i < facade->getBarsCycle(); i ++) {
		item = ListViewRoles->Items->Add();
		item->Caption = i;
		item->SubItems->Add(agent->getPlan()->getRole(i)->getName());
	}

}
//---------------------------------------------------------------------------
void __fastcall TFormTests::ButtonPlusThreeClick(TObject *Sender)
{
	CbFacade* facade = CbFacade::getInstance();
	CbPercussionistAgent* agent = facade->getAgent(0);
	int totalBars = facade->getBarsCycle();

	facade->setBarsCycle(totalBars + 3);

	if (EditAddRole->Text != "") {
		AnsiString role = EditAddRole->Text;
		agent->getPlan()->setRole(totalBars, facade->getRole(role.c_str()));
		agent->getPlan()->setRole(totalBars + 1, facade->getRole(role.c_str()));
		agent->getPlan()->setRole(totalBars + 2, facade->getRole(role.c_str()));
	}

	TabSheet2Show(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormTests::ButtonMinusTwoClick(TObject *Sender)
{
	CbFacade* facade = CbFacade::getInstance();
	int totalBars = facade->getBarsCycle();

	facade->setBarsCycle(totalBars - 2);
	TabSheet2Show(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormTests::ButtonDoTestClick(TObject *Sender)
{
	// PRINTF TEST //

	char testStr[50];
	double value;

    sprintf(testStr, "result=%f", double(1.2203));
	sscanf(testStr, "result=%lf", &value);

	EditPrintf->Text = value;

	// MUTE PATTERN TEST //

	CbRhythmPattern rhythmPattern(CbMeter::SIMPLE_QUADR);

	double eval = this->facade->getRole("base")->evaluate(rhythmPattern);
	EditRpMute->Text = int( this->facade->getRole("base")->patternIsSuitable(rhythmPattern) );
}
//---------------------------------------------------------------------------

void __fastcall TFormTests::ButtonCreateDirClick(TObject *Sender)
{
	AnsiString path = "D:\\Projetos\\CInBaladaXE\\Experiments\\";
    AnsiString dir;

	for (int i = 0; i < 7; i ++) {
		dir.printf("Experimento - %02d", i);

		ForceDirectories(path + dir + "\\roles");
		ForceDirectories(path + dir + "\\single");
		ForceDirectories(path + dir + "\\random");
    }
}

//---------------------------------------------------------------------------
// TAB SHEET 3
//---------------------------------------------------------------------------

void __fastcall TFormTests::ButtonChooseProjectClick(
	  TObject *Sender)
{
	if (OpenProjectDialog->Execute()) {
		EditProjectName->Text = OpenProjectDialog->FileName;
		this->adjustOutputFileName();
	}
}
//---------------------------------------------------------------------------

void TFormTests::adjustOutputFileName() {
	AnsiString outputFileName = ExtractFilePath(OpenProjectDialog->FileName);
	AnsiString fileNameOnly = ExtractFileName(OpenProjectDialog->FileName);

	if (CheckBoxRandomAgent->Checked == true) {
		outputFileName += "random\\" + fileNameOnly;
	} else if (CheckBoxSingleFuncAgent->Checked == true)  {
		outputFileName += "single\\" + fileNameOnly;
	} else {
		outputFileName += "roles\\" + fileNameOnly;
	}

	outputFileName = ChangeFileExt(outputFileName, SaveOutputDialog->DefaultExt);

	SaveOutputDialog->FileName = outputFileName;
	EditOutputFile->Text = outputFileName;
}
//---------------------------------------------------------------------------

void __fastcall TFormTests::CheckBoxSingleFuncAgentClick(TObject *Sender)
{
	if (CheckBoxSingleFuncAgent->Checked) {
		CheckBoxRandomAgent->Checked = false;
        CheckBoxRandomAgent->Enabled = false;
    } else {
        CheckBoxRandomAgent->Enabled = true;
    }

	this->adjustOutputFileName();
}
//---------------------------------------------------------------------------

void __fastcall TFormTests::CheckBoxRandomAgentClick(TObject *Sender)
{
	if (CheckBoxRandomAgent->Checked) {
	    CheckBoxSingleFuncAgent->Checked = false;
        CheckBoxSingleFuncAgent->Enabled = false;
    } else {
		CheckBoxSingleFuncAgent->Enabled = true;
    }

	this->adjustOutputFileName();
}
//---------------------------------------------------------------------------

void __fastcall TFormTests::ButtonOutputFileClick(TObject *Sender)
{
	if (SaveOutputDialog->Execute()) {
		EditOutputFile->Text = SaveOutputDialog->FileName;
	}
}
//---------------------------------------------------------------------------
void TFormTests::disableStatisticsControls()
{
	EditProjectName->Enabled = false;
	ButtonChooseProject->Enabled = false;

	LabelNumIterations->Enabled = false;
	EditNumIterations->Enabled = false;
	CheckBoxRandomAgent->Enabled = false;
       CheckBoxSingleFuncAgent->Enabled = false;

	EditOutputFile->Enabled = false;
	ButtonOutputFile->Enabled = false;

	ButtonStartProcessing->Enabled = false;
}
//---------------------------------------------------------------------------

void TFormTests::enableStatisticsControls()
{
	EditProjectName->Enabled = true;
	ButtonChooseProject->Enabled = true;

	LabelNumIterations->Enabled = true;
	EditNumIterations->Enabled = true;

	CheckBoxRandomAgent->Enabled = true;
    CheckBoxSingleFuncAgent->Enabled = true;

	CheckBoxSingleFuncAgentClick(NULL);
	CheckBoxRandomAgentClick(NULL);

	EditOutputFile->Enabled = true;
	ButtonOutputFile->Enabled = true;

	ButtonStartProcessing->Enabled = true;
}
//---------------------------------------------------------------------------
bool FullyEraseDir(AnsiString dirPath) {
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	AnsiString searchKey = dirPath + "*";
    AnsiString fileName;

	hFind = FindFirstFile(searchKey.c_str(), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
    	dwError = GetLastError();
		ShowMessage("Invalid file handle!");
		return false;

	} else {
    	fileName = FindFileData.cFileName;  // o primeiro arquivo

        if (fileName != "." && fileName != "..") {
			DeleteFile(FindFileData.cFileName);
        }

        // deleta o restante
		while (FindNextFile(hFind, &FindFileData) != 0) {
        	fileName = FindFileData.cFileName;

			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
				DeleteFile(dirPath + fileName);
            } else if (fileName != "." && fileName != "..") {
            	FullyEraseDir(dirPath + fileName + "\\");
            }
		}

		dwError = GetLastError();
		FindClose(hFind);

		if (dwError != ERROR_NO_MORE_FILES) {
			ShowMessage("Error in FindNextFile!");
			return false;
		}
	}

	return RemoveDir(dirPath);
}
//---------------------------------------------------------------------------
void __fastcall TFormTests::ButtonStartProcessingClick(
	  TObject *Sender)
{
	int userNumIterations = EditNumIterations->Text.ToIntDef(-1);

	if (EditProjectName->Text != "" && EditOutputFile->Text != "" && userNumIterations != -1) {
		this->facade->loadProjectFile(EditProjectName->Text);
		this->facade->setBarsCycle(1);  // artificio para os agentes se prepararem
                                        // valor correto setado no evento do mixer

		if (this->facade->isRepeatingCycle()) {
			this->facade->toggleRepeatCycle();
		}

		if (CheckBoxRandomAgent->Checked == true) {
			/*Vector_String instrumentNames;
			CbPercussionistAgent* agent;

			for (int i = this->facade->getNumAgents() - 1; i >= 0; i --) {
				agent = this->facade->getAgent(i);
				instrumentNames.push_back(agent->getInstrumentName());
				this->facade->removeAgent(i);
			}

			for (int i = instrumentNames.size() - 1; i >= 0; i --) {
				this->facade->addRandomAgent(instrumentNames[i].c_str(), instrumentNames[i].c_str());
			}*/

            int indexFreeRole = this->facade->getRoleIndex("livre");

			for (int i = this->facade->getNumAgents() - 1; i >= 0; i --) {
                this->facade->setAgentDefaultRole(i, indexFreeRole);
                this->facade->setAgentRole(i, 0, indexFreeRole);
			}


		} else if (CheckBoxSingleFuncAgent->Checked == true) {
            int indexUnifRole = this->facade->getRoleIndex("uniforme");

			for (int i = this->facade->getNumAgents() - 1; i >= 0; i --) {
                this->facade->setAgentDefaultRole(i, indexUnifRole);
                this->facade->setAgentRole(i, 0, indexUnifRole);
			}

        }

		this->numIterations = userNumIterations;
		this->currentIteration = -1;  // artificio para dar tempo dos agentes se prepararem
		this->statistics.clear();

		AnsiString path = ExtractFilePath(EditOutputFile->Text) + "midis\\";

		if (DirectoryExists(path)) {
           	FullyEraseDir(path);
		}

	    if (ForceDirectories(path) == true) {
	        this->disableStatisticsControls();
			this->facade->getMixer()->OnStatusChange = this->OnMixerStatusChanged;
			this->facade->getMixer()->start();
        } else {
			ShowMessage("Diretorio nao pode ser criado!");
	    }

	} else {
		ShowMessage("Nem todos os campos foram informados corretamente!");

	}
}
//---------------------------------------------------------------------------

void TFormTests::OnMixerStatusChanged(CbMixerAgent *mixer) {
	if (mixer->getStatus() == ES_STOPPED && this->currentIteration == -1) {
		this->currentIteration = 0;

		this->startTime = timeGetTime();
		EditTotalTime->Text = "0";

		this->facade->setBarsCycle(this->performanceNumBars);
		this->facade->getMixer()->start(true);

		EditCurrIteration->Text = this->currentIteration + 1;

	} else if (mixer->getStatus() == ES_STOPPED) {

		//if (CheckBoxRandomAgent->Checked == true) {
		//	addRandomAgentStatistics();
		//} else {
			addPercAgentStatistics();
		//}

		this->currentIteration ++;
		EditTotalTime->Text = timeGetTime() - this->startTime;

		//se terminaram todas as iterações...
		if (this->currentIteration == this->numIterations) {
			//salva as estatisticas
			this->statistics.saveFile(EditOutputFile->Text.c_str()); //, CbAgentChoicesStats::coordinatesComparation);

			//para nao receber mais eventos...
			this->facade->getMixer()->OnStatusChange = NULL;

			//habilita os controles
            this->enableStatisticsControls();

		} else {
			this->facade->getMixer()->start(true);
			EditCurrIteration->Text = this->currentIteration + 1;

		}
	}

}
//---------------------------------------------------------------------------
void TFormTests::addPercAgentStatistics() {
	int numAgents = this->facade->getNumAgents();
	CbAgentChoicesStats::Tuple patternsChosen (numAgents);
	CbPercPerfHistory* agentHistory;

	for (int i = 0; i < numAgents; i ++) {
		agentHistory = this->facade->getAgent(i)->getHistory();
		patternsChosen.set(i, agentHistory->get(0)->patternIndex);
	}

	this->statistics.addTuple(patternsChosen);
    savePerformanceMidiFile(patternsChosen);
}
//---------------------------------------------------------------------------

void TFormTests::addRandomAgentStatistics() {
	int numAgents = this->facade->getNumAgents();
	CbAgentChoicesStats::Tuple patternsChosen (numAgents);
	CbPercPerfHistory* agentHistory;

	for (int i = 0; i < numAgents; i ++) {
		patternsChosen.set(i, this->facade->getRandomAgent(i)->getCurrentPattern());
	}

	this->statistics.addTuple(patternsChosen);
	savePerformanceMidiFile(patternsChosen);
}
//---------------------------------------------------------------------------

void TFormTests::savePerformanceMidiFile(CbAgentChoicesStats::Tuple tuple) {
	AnsiString path = ExtractFilePath(EditOutputFile->Text) + "midis\\";
	AnsiString fileName;

	for (int i = 0; i < tuple.size(); i ++) {
		fileName.cat_printf("%02d-", tuple.get(i));
	}
	fileName.SetLength(fileName.Length() - 1);

	fileName = AnsiString(this->currentIteration) + " - " + fileName;

    AnsiString fileFullPath = path + fileName + ".mid";

	if (FileExists(fileFullPath) == false) {
		this->facade->exportRecordingToMidi(fileFullPath.c_str());
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormTests::FormDestroy(TObject *Sender)
{
	Sleep(100);
}
//---------------------------------------------------------------------------

void __fastcall TFormTests::RadioGroup1Click(TObject *Sender)
{
	//RadioGroup1->Items->ItemIndex
}

//---------------------------------------------------------------------------
// TAB SHEET 4
//---------------------------------------------------------------------------

void __fastcall TFormTests::ButtonXmlTestClick(TObject *Sender)
{
	ICbRpInfoRepository* repository = this->facade->getRpInfoRepository();
	int repertoireSize = repository->getNumRpInfos();

	AnsiString midiDirectory = this->facade->getApplicationDir() + "\\" + MIDI_REPERT_DIR + "\\";

	AnsiString fileName = this->facade->getApplicationDir() + "\\logs\\XmlExhaustiveTest.txt";
	ofstream logFile (fileName.c_str(), std::ios::out);

	const CbRpInfo* rpinfo;
	CbRhythmPattern* original;
	CbRhythmPattern* copy;

	TiXmlDocument *xmlDocument;
	TiXmlElement *xmlElement;
	FmMidiFileTrack* track;

	std::string xmlPattern;
	std::string tubs;

	MemoXmlTest->Clear();

	for (int i = 0; i <  repertoireSize; i ++) {
		rpinfo = repository->getRpInfoById(i);
		logFile << "Padrao \"" << rpinfo->getName() << "\"" << endl;

		original = new CbRhythmPattern(*rpinfo, midiDirectory.c_str());
		xmlElement = writeXmlPhrase(*(original->getPattern()));

		xmlPattern.clear();
		xmlPattern << (*xmlElement);
		delete xmlElement;

		xmlDocument = new TiXmlDocument("");
		xmlDocument->Parse(xmlPattern.c_str());
		track = readXmlPhrase(*(xmlDocument->RootElement()));

		copy = new CbRhythmPattern(*track, *(rpinfo->getMeter())); /*/
		copy = new CbRhythmPattern(*original); //*/

		delete track;
		delete xmlDocument;

		if ((*original) == (*copy)) {
			MemoXmlTest->Lines->Add(AnsiString(i) + " -> Ok");

		} else {
			MemoXmlTest->Lines->Add(AnsiString(i) + " -> Falha !");

			tubs.clear();
			tubs << *(original->getTubs());
			MemoXmlTest->Lines->Add(tubs.c_str());
			logFile << *(original->getPattern()) << endl;

			logFile << xmlPattern << endl;

			tubs.clear();
			tubs << *(copy->getTubs());
			MemoXmlTest->Lines->Add(tubs.c_str());
			logFile << *(copy->getPattern()) << endl << endl;

			break;
		}

		delete original;
		delete copy;
	}

	MemoXmlTest->Lines->Add("Finished!");
}
//---------------------------------------------------------------------------

