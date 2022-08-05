//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormRepertoireMain.h"
#include "FormChooseDir.h"
#include "Interface\Main\FormSplashScreen.h"

#include "CInBalada\CbFacade.h"
#include "CInBalada\RhythmPattern\CbRhythmPatternSequencer.h"

#include "Util\MathUtil.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RhythmPatternControl"
#pragma resource "*.dfm"

TFormRepertoireMain *FormRepertoireMain;

const char* NOTES[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

//---------------------------------------------------------------------------
__fastcall TFormRepertoireMain::TFormRepertoireMain(TComponent* Owner)
    : TForm(Owner)
{
	applicationDirectory = ExpandFileName(".");
    rhythmRepository = NULL;
    midiFile1 = midiFile2 = -1;
    miMidiFile = NULL;
	numDigits = 1;
}

//---------------------------------------------------------------------------
// Retorna o repositorio. Se nao estiver alocado, aloca-o.
ICbRpInfoRepository* TFormRepertoireMain::getRhythmRepository() {
	if (this->rhythmRepository == NULL) {
		this->rhythmRepository = CbFacade::getInstance()->getRpInfoRepository();
		int numRpInfos = rhythmRepository->getNumRpInfos();
		if (numRpInfos > 0) {
			this->numDigits = int( ceil(log(numRpInfos,10)) );
		} else {
			this->numDigits = 1;
		}
	}
	return this->rhythmRepository;
}

//---------------------------------------------------------------------------
void TFormRepertoireMain::deleteRhythmRepository() {
	this->rhythmRepository = NULL;   // repositorio eh desalocado pela fachada
}

//---------------------------------------------------------------------------
// Chamado quando o Form principal eh criado.
void __fastcall TFormRepertoireMain::FormCreate(TObject *Sender)
{
	//CbRhythmPatternSequencer::getSequencer()->start();

	ComboBoxMeter->Items->Add("Ternary (12 pulses)");
	ComboBoxMeter->Items->Add("Quaternary (16 pulses)");

	ComboBoxType->Items->Add(CbRpInfo::rhythmPatternTypeToStr(RPT_UNKNOWN));
	ComboBoxType->Items->Add(CbRpInfo::rhythmPatternTypeToStr(RPT_ROLL));
	ComboBoxType->Items->Add(CbRpInfo::rhythmPatternTypeToStr(RPT_FILL));
	ComboBoxType->Items->Add(CbRpInfo::rhythmPatternTypeToStr(RPT_BASIC));

	ComboBoxFreqClass->Items->Add(CbPercInstrument::frequencyClassToStr(IFC_LOW_BAND));
	ComboBoxFreqClass->Items->Add(CbPercInstrument::frequencyClassToStr(IFC_MED_BAND));
	ComboBoxFreqClass->Items->Add(CbPercInstrument::frequencyClassToStr(IFC_HIGH_BAND));

	// adiciona notas MIDI de percussão (no canal 9)
	AnsiString noteCaption;
	for (int i = 35; i <= 81; i ++) {
		noteCaption.sprintf("%s%d (%d)", NOTES[i % 12], (i / 12), i);
		ComboBoxMidiNote->Items->Add(noteCaption);
	}

	ComboBoxNoteRelPitch->Items->Add(CbPercInstrument::notePitchToStr(NP_LOW));
	ComboBoxNoteRelPitch->Items->Add(CbPercInstrument::notePitchToStr(NP_HIGH));

	CbFacade* facade = CbFacade::getInstance();
	AnsiString aux;

	// funcoes de avaliação dos papeis
	for (int i = 0; i < facade->getNumRoles(); i ++) {
		aux.printf("Avaliacao de '%s'", facade->getRole(i)->getName());
		ComboBoxAvCriteria->Items->Add(aux);
	}

	// funcoes de avaliação/comparação gerais
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_GEBER));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_CHRONOT_DIST));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_HAMMING_DIST));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_DERIVATION));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_OFFBEATNESS));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_ODDITY));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_METRICITY));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_COGNIT_COMPLEXITY));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_SYNCOPATION));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_CIRC_CHORD_EVENNESS));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_LIN_REG_EVENNESS));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_IRREGULARITY));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_ACTIVITY));
	ComboBoxAvCriteria->Items->Add(CbRhythmPattern::evaluationCriteriaToStr(AVC_AVERAGE_INTENSITY));

	ComboBoxGraphRepr->Items->Add("Tubs");
	ComboBoxGraphRepr->Items->Add("Circle");
	ComboBoxGraphRepr->Items->Add("Tedas");

	ComboBoxGraphAlignment->Items->Add("None (independent representations)");
	ComboBoxGraphAlignment->Items->Add("Tatum (tatums are aligned)");
	ComboBoxGraphAlignment->Items->Add("Bar (bars are aligned)");

	//RhythmPatternControl->AttachSequencer(CbRhythmPatternSequencer::getSequencer());

	updateStatusBar();
}

//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::FormShow(TObject *Sender)
{
	FormSplashScreen->ShowModal();
}
//---------------------------------------------------------------------------

void TFormRepertoireMain::updateStatusBar() {
	ICbRpInfoRepository* rep = getRhythmRepository();
	AnsiString aux;

	aux = rep->getNumInstruments();
	StatusBar->Panels->Items[0]->Text = aux + " instruments";
	aux = rep->getNumRpInfos();
	StatusBar->Panels->Items[1]->Text = aux + " rhythm patterns";
}

//---------------------------------------------------------------------------
// Chamado quando o Form eh finalizado.
void __fastcall TFormRepertoireMain::FormDestroy(TObject *Sender)
{
	deleteRhythmRepository();
}

//---------------------------------------------------------------------------
// Sheet FILES
//---------------------------------------------------------------------------
// Prepara a pagina 'Files' para exibiçao.
void __fastcall TFormRepertoireMain::TabSheetFilesShow(TObject *Sender)
{
	TListItem *listItem;
	CbRpInfo* pattern;
	Vector_pRpInfo rhythmPatterns;

	this->getRhythmRepository()->getAllRpInfos(rhythmPatterns);

    ListViewFiles->Items->Clear();
    for (int i = 0; i < rhythmPatterns.size(); i ++) {
        pattern = rhythmPatterns[i];
        if (pattern != NULL) {
            listItem = ListViewFiles->Items->Add();
			listItem->Caption = pattern->getId();
            listItem->SubItems->Add(pattern->getFileName().c_str());
            listItem->SubItems->Add(pattern->getInstrument().c_str());
            listItem->SubItems->Add(pattern->getTypeStr());
        }
	}

	ToolButtonRemoveFile->Enabled = false;
}

//---------------------------------------------------------------------------
// Aconteceu um clique no botão 'AddFile' da pagina 'Files'
void __fastcall TFormRepertoireMain::ToolButtonAddFileClick(TObject *Sender)
{
	if (OpenDialogFiles->Execute()) {

        //adiciona arquivo(s) ao repertorio
        for (int i = 0; i < OpenDialogFiles->Files->Count; i ++) {
            addFileToRepertoire(OpenDialogFiles->Files->Strings[i]);
        }

        //atualiza tabela
        TabSheetFilesShow(NULL);
	}
}

//---------------------------------------------------------------------------
// metodo auxiliar, adiciona um arquivo ao repertorio
void TFormRepertoireMain::addFileToRepertoire(AnsiString filePath) {
	CbRpInfo pattern (-1, NULL, "", "", RPT_UNKNOWN, NULL);
	ICbRpInfoRepository* repository = getRhythmRepository();

	AnsiString file = copyFileToDir(filePath, applicationDirectory + "\\" + MIDI_REPERT_DIR);
	AnsiString nameOnly = ExtractFileName(file);

	pattern.setName(nameOnly.SubString(0, nameOnly.LastDelimiter(".")-1).c_str());
    pattern.setFileName(file.c_str());

	repository->addRpInfo(pattern);
	this->numDigits = int( ceil(log(repository->getNumRpInfos(),10)) );

	updateStatusBar();
}

// metodo auxiliar, copia um arquivo para o diretorio do sistema onde
// ficam os padrões ritmicos renomeando o arquivo, se necessario
AnsiString TFormRepertoireMain::copyFileToDir(AnsiString filePath, AnsiString destDirPath) {
    int tries = 0;
    const int MAX_TRIES = 9;

    AnsiString aux = ExtractFileName(filePath);
    AnsiString destFileExt = ExtractFileExt(aux);
    AnsiString destFileName = aux.SubString(1, aux.Length() - destFileExt.Length());

    destDirPath = ExcludeTrailingBackslash(destDirPath) + "\\";

    AnsiString destFileNameExt = destFileName + destFileExt;
    AnsiString destPath = destDirPath + destFileNameExt;

	while ( CopyFileA(filePath.c_str(),destPath.c_str(),TRUE) == 0
                && tries < MAX_TRIES ) {
		destFileNameExt = destFileName + ((tries < 10)? "0" : "") + tries + destFileExt;
		destPath = destDirPath + destFileNameExt;
        tries ++;
    }

    return destFileNameExt;
}

//---------------------------------------------------------------------------
// Occorencia de clique no botão 'AddDir' da pagina 'Files'.
// Adiciona todos os arquivos com extensao *.MID ao repertorio.
void __fastcall TFormRepertoireMain::ToolButtonAddDirClick(TObject *Sender)
{
    if (FormChooseDir->ShowModal() == mrOk) {
        AnsiString dir = FormChooseDir->getChosenDirectory();
        TSearchRec sr;

        if (FindFirst(dir + "\\*.mid", 0, sr) == 0) {
            //adiciona os arquivos ao repertorio
            do {
                addFileToRepertoire(sr.Name);
            } while (FindNext(sr) == 0);
            FindClose(sr);

			//atualiza tabela
			TabSheetFilesShow(NULL);
		}
    }
}

//---------------------------------------------------------------------------
// Clique no botão 'Remover'. Remove um elemento do repositorio.
void __fastcall TFormRepertoireMain::ToolButtonRemoveFileClick(
      TObject *Sender)
{
	TListItem *listItem;
	bool deleted = false;
	int id;

	int result = -1;
	bool deleteAll = false;
	for (int i = 0; i < ListViewFiles->Items->Count; i ++) {
		listItem = ListViewFiles->Items->Item[i];
		if (listItem->Selected == true) {
			id = listItem->Caption.ToInt();
			getRhythmRepository()->deleteRpInfo(id);
			deleted = true;

			AnsiString fullPath = applicationDirectory + "\\" + MIDI_REPERT_DIR + "\\"
							+ listItem->SubItems->Strings[0];

			// pergunta ao usuario se ele deseja remover
			if (deleteAll == false) {
				result = MessageDlg("Entry for the rhythm pattern was removed. Do you also want to delete the file "
							+ fullPath + "?", mtConfirmation
							, TMsgDlgButtons() << mbYes << mbYesToAll << mbNo, 0);
				deleteAll = (result == mrYesToAll);
			}

			if (deleteAll == true || result == mrYes) {
				DeleteFile(fullPath);
			}
		}
	}

	if (deleted == true) {
        TabSheetFilesShow(NULL);  //atualiza tabela
		updateStatusBar();
    }

}
//---------------------------------------------------------------------------
void __fastcall TFormRepertoireMain::ListViewFilesSelectItem(
	  TObject *Sender, TListItem *listItem, bool Selected)
{
	if (listItem == NULL || Selected == false) {
        ToolButtonRemoveFile->Enabled = false;
    } else if (ToolButtonRemoveFile->Enabled == false) {
		ToolButtonRemoveFile->Enabled = true;
	}
}

//---------------------------------------------------------------------------
// Sheet EDIT PATTERN
//---------------------------------------------------------------------------
// Chamado quando a pagina 'Edit PAttern' eh mostrada.
void __fastcall TFormRepertoireMain::TabSheetEditFileShow(TObject *Sender)
{
	TListItem *listItem;
	CbRpInfo* pattern;
    Vector_pRpInfo rhythmPatterns;

    this->getRhythmRepository()->getAllRpInfos(rhythmPatterns);

    /* Mostra a lista de arquivos */
    ListViewEditFiles->Items->Clear();
	for (int i = 0; i < rhythmPatterns.size(); i ++) {
		pattern = rhythmPatterns[i];
		listItem = ListViewEditFiles->Items->Add();
		listItem->Caption = pattern->getId();
		listItem->SubItems->Add(pattern->getFileName().c_str());
		listItem->SubItems->Add(pattern->getName().c_str());
		listItem->SubItems->Add(pattern->getInstrument().c_str());
		listItem->SubItems->Add(pattern->getTypeStr());
    }

	/* Preenche o combo-box de instrumentos */
    Vector_string instruments;

	this->getRhythmRepository()->getAllInstrumentNames(instruments);
	ComboBoxInstrument->Clear();
	for (int i = 0; i < instruments.size(); i ++) {
		ComboBoxInstrument->Items->Add(instruments[i].c_str());
	}

	/* Desabilita os controles de edicao dos padroes */
	EditMidiFile->Text = "--- choose a file to edit ---";
	EditMidiFile->Enabled = false;
	EditName->Text = "";
	EditName->Enabled = false;
	EditNumTrack->Text = "";
	EditNumTrack->Enabled = false;
	ComboBoxMeter->ItemIndex = -1;
	ComboBoxMeter->Enabled = false;
	ComboBoxInstrument->Text = "";
	ComboBoxInstrument->ItemIndex = -1;
	ComboBoxInstrument->Enabled = false;
	ComboBoxType->ItemIndex = -1;
	ComboBoxType->Enabled = false;
	ButtonSave->Enabled = false;
	EditAddStyle->Text = "";
	EditAddStyle->Enabled = false;
	ButtonAddStyle->Enabled = false;
	CheckListBoxStyles->Clear();
	ButtonRemoveStyle->Enabled = false;

}
//---------------------------------------------------------------------------
// Chamado quando o foco muda de item, inclusive quando o foco fica
// fora de qualquer dos itens (nesse caso, Selected == false).
void __fastcall TFormRepertoireMain::ListViewEditFilesSelectItem(
	  TObject *Sender, TListItem *listItem, bool Selected)
{
	const CbRpInfo* pattern;
	int id;

	if (listItem == NULL || Selected == false) {
		/* Desabilita os controles de edicao dos padroes */
		EditMidiFile->Text = "--- choose a file to edit ---";
		EditMidiFile->Enabled = false;
		EditName->Text = "";
		EditName->Enabled = false;
		EditNumTrack->Text = "";
		EditNumTrack->Enabled = false;
		ComboBoxMeter->ItemIndex = -1;
		ComboBoxMeter->Enabled = false;
		ComboBoxInstrument->Text = "";
		ComboBoxInstrument->ItemIndex = -1;
		ComboBoxInstrument->Enabled = false;
		ComboBoxType->ItemIndex = -1;
		ComboBoxType->Enabled = false;
		ButtonSave->Enabled = false;
		EditAddStyle->Text = "";
		EditAddStyle->Enabled = false;
		ButtonAddStyle->Enabled = false;
		CheckListBoxStyles->Clear();
		ButtonRemoveStyle->Enabled = false;

	} else {

		/*if (ButtonSave->Enabled == true) {
			int choice = MessageDlg("The current item hasn't been saved yet! Do you want to save it?",
			   mtWarning , TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);
			switch (choice) {
			case mrOk:
				ButtonSaveClick(NULL); // pode dar bug !!! qual o item atual !?
				break;
			case mrCancel:
				//ListViewEditFiles->Selected = //anterior ??;
				break;
			}
		}*/

		id = listItem->Caption.ToInt();
		pattern = getRhythmRepository()->getRpInfoById(id);

		EditMidiFile->Enabled = true;
		EditMidiFile->Text = pattern->getFileName().c_str();

		EditName->Enabled = true;
		EditName->Text = pattern->getName().c_str();

		ComboBoxMeter->Enabled = true;
		const CbMeter* meter = pattern->getMeter();
		if (meter->isRegular()) {
			if (meter->getNumTatums() == 12) {
				ComboBoxMeter->ItemIndex = 0;
			} else if (meter->getNumTatums() == 16) {
				ComboBoxMeter->ItemIndex = 1;
			}
		} else {
			ComboBoxMeter->ItemIndex = -1;
		}

		EditNumTrack->Enabled = true;
		EditNumTrack->Text = pattern->getTrack();

		ComboBoxInstrument->Enabled = true;
		ComboBoxInstrument->Text = pattern->getInstrument().c_str();

		ComboBoxType->Enabled = true;
		ComboBoxType->ItemIndex = pattern->getType();

		ButtonSave->Enabled = false;

		EditAddStyle->Enabled = true;
		EditAddStyle->Text = "";

		ButtonAddStyle->Enabled = true;
		ButtonRemoveStyle->Enabled = true;

		//insere estilos...
		CheckListBoxStyles->Clear();
		for (int i = 0; i < pattern->getNumStyles(); i ++) {
			CheckListBoxStyles->Items->Add(pattern->getStyle(i).c_str());
        }

    }

}

//---------------------------------------------------------------------------
// Clique no botão 'Add Style'.
void __fastcall TFormRepertoireMain::ButtonAddStyleClick(TObject *Sender)
{
    if (EditAddStyle->Text.IsEmpty() == false
            && CheckListBoxStyles->Items->IndexOf(EditAddStyle->Text) == -1) {

	    CheckListBoxStyles->Items->Add(EditAddStyle->Text);
        EditAddStyle->Text = "";
		ButtonSave->Enabled = true;

	} else {

		MessageDlg("Invalid musical style ( empty or repeated ) !",
               mtError, TMsgDlgButtons() << mbOK, -1);
    }

}

//---------------------------------------------------------------------------
// Clique no botão 'Remove Selected Styles'.
void __fastcall TFormRepertoireMain::ButtonRemoveStyleClick(
      TObject *Sender)
{
    bool deleted = false;

    for (int i = 0; i < CheckListBoxStyles->Items->Count; i ++) {
		if (CheckListBoxStyles->Checked[i]) {
            CheckListBoxStyles->Items->Delete(i);
            deleted = true;
            i --; // para compensar a rearrumação dos itens apos a delecao
        }
    }

	if (deleted == true) {
        ButtonSave->Enabled = true;
    } else {
		MessageDlg("No musical style selected !",
               mtError, TMsgDlgButtons() << mbOK, -1);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::EditNamexChange(TObject *Sender)
{
    if (EditName->Modified) {
        ButtonSave->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::EditNumTrackChange(TObject *Sender)
{
    if (EditNumTrack->Modified) {
        ButtonSave->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::ComboBoxInstrumentChange(TObject *Sender)
{
	ButtonSave->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::ComboBoxTypeChange(TObject *Sender)
{
	ButtonSave->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::ComboBoxMeterChange(TObject *Sender)
{
	ButtonSave->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormRepertoireMain::ButtonSaveClick(TObject *Sender)
{
	TListItem* listItem = ListViewEditFiles->Selected;
    int id = listItem->Caption.ToInt();
	ICbRpInfoRepository* rep = getRhythmRepository();
	const CbRpInfo *old = rep->getRpInfoById(id);

	ECbRpType type = CbRpInfo::strToRhythmPatternType(ComboBoxType->Text.c_str());
	int trackNum = 	EditNumTrack->Text.ToInt();

	CbMeter* meter = NULL;
	if (ComboBoxMeter->ItemIndex == 0) {
		meter = new CbMeter(CbMeter::COMPOUND_QUADR);  // MIDIs ternarios notados em 12/8
	} else {
		meter = new CbMeter(CbMeter::SIMPLE_QUADR);
	}

	CbRpInfo info (-1, EditName->Text.c_str(), old->getFileName().c_str(),
				ComboBoxInstrument->Text.c_str(), type, meter, trackNum);

	//adiciona os estilos...
	for (int i = 0; i < CheckListBoxStyles->Items->Count; i ++) {
		info.addStyle(CheckListBoxStyles->Items->Strings[i].c_str());
    }

    rep->changeRpInfo(id, info);

    //desabilita o botão
    ButtonSave->Enabled = false;

	//altera a linha da list view
	//listItem->Caption = id; //(nao precisa)
    listItem->SubItems->Strings[0] = info.getFileName().c_str();
    listItem->SubItems->Strings[1] = info.getName().c_str();
    listItem->SubItems->Strings[2] = info.getInstrument().c_str();
	listItem->SubItems->Strings[3] = info.getTypeStr();

}

//---------------------------------------------------------------------------
// Sheet COMPARATOR
//---------------------------------------------------------------------------
void __fastcall TFormRepertoireMain::TabSheetComparatorShow(
	  TObject *Sender)
{
	ICbRpInfoRepository* rep = getRhythmRepository();
	Vector_pRpInfo vinfos;
	CbRpInfo* rpi;

	//iniciar os combo-boxes
	ComboBoxPatternComp1->Clear();
	ComboBoxPatternComp2->Clear();
	ComboBoxPatternComp1->Items->Add("");
	ComboBoxPatternComp2->Items->Add("");

	// botoes para tocar os padroes
	BitBtnPlayComp1->Enabled = false;
	BitBtnPauseComp1->Enabled = false;
	BitBtnPlayComp2->Enabled = false;
	BitBtnPauseComp2->Enabled = false;

	this->midiFile1 = this->midiFile2 = -1;

	AnsiString format = "(%0" + AnsiString(this->numDigits) + "d) - ";
	AnsiString aux;
	int id;

	rep->getAllRpInfos(vinfos);
	for (int i = 0; i < vinfos.size(); i ++) {
		rpi = vinfos[i];
		id = rpi->getId();
		aux.printf(format.c_str(), id);
		aux = aux + rpi->getFileName().c_str();
		ComboBoxPatternComp1->Items->Add(aux);
		ComboBoxPatternComp2->Items->Add(aux);
	}

	ComboBoxAvCriteria->ItemIndex = -1;
	EditAvaliationResult->Text = "";

	RhythmPatternControl->setRhythmPattern1(NULL, "");
	RhythmPatternControl->setRhythmPattern2(NULL, "");

	ComboBoxGraphRepr->ItemIndex = 0;
	ComboBoxGraphRepr->Enabled = false;
	ComboBoxGraphAlignment->ItemIndex = 1; // tatum aligned
	ComboBoxGraphAlignment->Enabled = false;

	RhythmPatternControl->PatternRepresentation =
							ERpGraphicRepr(ComboBoxGraphRepr->ItemIndex);
	RhythmPatternControl->PatternsAlignment =
							ERpGraphicReprAlignment(ComboBoxGraphAlignment->ItemIndex);

}
//---------------------------------------------------------------------------
void TFormRepertoireMain::updateVisualizationComboBoxes() {
/*	if (ComboBoxPatternComp1->ItemIndex <= 0
			&& ComboBoxPatternComp2->ItemIndex <= 0) {
		ComboBoxGraphRepr->Enabled = false;
		ComboBoxGraphAlignment->Enabled = false;
	} else {
		ComboBoxGraphRepr->Enabled = true;
		ComboBoxGraphAlignment->Enabled = true;
	}*/

	ComboBoxGraphRepr->Enabled = false;
	ComboBoxGraphAlignment->Enabled = false;

	if (ComboBoxPatternComp1->ItemIndex > 0) {
		ComboBoxGraphRepr->Enabled = true;
		if (ComboBoxPatternComp2->ItemIndex > 0) {
			ComboBoxGraphAlignment->Enabled = true;
		}
	}

}
//---------------------------------------------------------------------------
void __fastcall TFormRepertoireMain::TabSheetComparatorHide(TObject *Sender)
{
	CbRhythmPatternSequencer::getSequencer()->removeAll();
	this->midiFile1 = this->midiFile2 = -1;
}
//---------------------------------------------------------------------------
void __fastcall TFormRepertoireMain::ComboBoxPatternComp1Change(TObject *Sender)
{
	ICbRpInfoRepository* rep = getRhythmRepository();
	int id;

	if (this->midiFile1 != -1) {
		CbRhythmPatternSequencer::getSequencer()->remove(this->midiFile1);
		this->midiFile1 = -1;
	}

	if (ComboBoxPatternComp1->ItemIndex == 0) {
		RhythmPatternControl->setRhythmPattern1(NULL, "");
		if (ComboBoxPatternComp2->ItemIndex > 0) {
			ComboBoxPatternComp1->ItemIndex = ComboBoxPatternComp2->ItemIndex;
			ComboBoxPatternComp2->ItemIndex = 0;
			BitBtnPlayComp1->Enabled = BitBtnPlayComp2->Enabled;
			BitBtnPauseComp1->Enabled = BitBtnPauseComp2->Enabled;
			if (this->midiFile2 != -1) {
				this->midiFile1 = this->midiFile2;
				this->midiFile2 = -1;
			}
			BitBtnPlayComp2->Enabled = false;
			BitBtnPauseComp2->Enabled = false;
		} else {
			BitBtnPlayComp1->Enabled = false;
			BitBtnPauseComp1->Enabled = false;
		}
	} else if (ComboBoxPatternComp1->ItemIndex != -1) {
		AnsiString optionStr = ComboBoxPatternComp1->Items->Strings[ComboBoxPatternComp1->ItemIndex];
		AnsiString patternId = optionStr.SubString(2, this->numDigits);

		RhythmPatternControl->setRhythmPattern1(rep->getRpInfoById(patternId.ToInt()),
					applicationDirectory + "\\" + MIDI_REPERT_DIR + "\\");
		BitBtnPlayComp1->Enabled = true;
		BitBtnPauseComp1->Enabled = false;
	}

	ComboBoxAvCriteriaChange(NULL);
	updateVisualizationComboBoxes();
}
//---------------------------------------------------------------------------
void __fastcall TFormRepertoireMain::ComboBoxPatternComp2Change(TObject *Sender)
{
	ICbRpInfoRepository* rep = getRhythmRepository();
	int id;

	if (this->midiFile2 != -1) {
		CbRhythmPatternSequencer::getSequencer()->remove(this->midiFile2);
		this->midiFile2 = -1;
	}

	if (ComboBoxPatternComp2->ItemIndex == 0) {
		RhythmPatternControl->setRhythmPattern2(NULL, "");
		BitBtnPlayComp2->Enabled = false;
		BitBtnPauseComp2->Enabled = false;
	} else if (ComboBoxPatternComp2->ItemIndex != -1) {
		AnsiString optionStr = ComboBoxPatternComp2->Items->Strings[ComboBoxPatternComp2->ItemIndex];
		AnsiString patternId = optionStr.SubString(2, this->numDigits);

		RhythmPatternControl->setRhythmPattern2(rep->getRpInfoById(patternId.ToInt()),
					applicationDirectory + "\\" + MIDI_REPERT_DIR + "\\");
		BitBtnPlayComp2->Enabled = true;
		BitBtnPauseComp2->Enabled = false;
	}

	ComboBoxAvCriteriaChange(NULL);
	updateVisualizationComboBoxes();
}

//---------------------------------------------------------------------------
// Changes mode of visualization of the rhythm patterns
void __fastcall TFormRepertoireMain::ComboBoxGraphReprChange(TObject *Sender)
{
	RhythmPatternControl->PatternRepresentation =
							ERpGraphicRepr(ComboBoxGraphRepr->ItemIndex);
}
//---------------------------------------------------------------------------
void __fastcall TFormRepertoireMain::ComboBoxGraphAlignmentChange(
	  TObject *Sender)
{
	RhythmPatternControl->PatternsAlignment =
							ERpGraphicReprAlignment(ComboBoxGraphAlignment->ItemIndex);
}

//---------------------------------------------------------------------------
void __fastcall TFormRepertoireMain::BitBtnPlayComp1Click(TObject *Sender)
{
	if (ComboBoxPatternComp1->ItemIndex != -1) {
		this->midiFile1 = CbRhythmPatternSequencer::getSequencer()->add(
								RhythmPatternControl->getRhythmPattern1());

		BitBtnPlayComp1->Enabled = false;
		BitBtnPauseComp1->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormRepertoireMain::BitBtnPlayComp2Click(TObject *Sender)
{
	if (ComboBoxPatternComp2->ItemIndex != -1) {
		this->midiFile2 = CbRhythmPatternSequencer::getSequencer()->add(
								RhythmPatternControl->getRhythmPattern2());

		BitBtnPlayComp2->Enabled = false;
		BitBtnPauseComp2->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormRepertoireMain::BitBtnPauseComp1Click(TObject *Sender)
{
	if (ComboBoxPatternComp1->ItemIndex != -1) {
		CbRhythmPatternSequencer::getSequencer()->remove(this->midiFile1);
		this->midiFile1 = -1;

		BitBtnPlayComp1->Enabled = true;
		BitBtnPauseComp1->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::BitBtnPauseComp2Click(TObject *Sender)
{
    if (ComboBoxPatternComp2->ItemIndex != -1) {
		CbRhythmPatternSequencer::getSequencer()->remove(this->midiFile2);
		this->midiFile2 = -1;

		BitBtnPlayComp2->Enabled = true;
        BitBtnPauseComp2->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::ComboBoxAvCriteriaChange(
	  TObject *Sender)
{
	AnsiString res1, res2;
	const char* doubleFormat = "%.2f";
	double arg = 1.0;

	const CbRhythmPattern *p1 = RhythmPatternControl->getRhythmPattern1();
	const CbRhythmPattern *p2 = RhythmPatternControl->getRhythmPattern2();

	CbFacade* facade = CbFacade::getInstance();

	int index = ComboBoxAvCriteria->ItemIndex;

	if (index >= 0 && index < facade->getNumRoles()) {
		if (p1 != NULL) {
			arg = facade->getRole(index)->evaluate(*p1);
			res1.printf(doubleFormat, arg);
		}
		if (p2 != NULL) {
			arg = facade->getRole(index)->evaluate(*p2);
			res2.printf(doubleFormat, arg);
		}
		EditAvaliationResult->Text = "\"" + res1 + "\" / \"" + res2 + "\"";

	} else if (index != -1) {
		AnsiString criteriaStr = ComboBoxAvCriteria->Items->Strings[index];
		ECbRpEvalCriteria criteria = CbRhythmPattern::strToEvaluationCriteria(criteriaStr.c_str());
		bool comparation = CbRhythmPattern::isComparation(criteria);

		if (comparation == true) {
			if (p1 != NULL && p2 != NULL) {
				arg = (float)p1->evaluationFunction(criteria, p2);
				res1.printf(doubleFormat, arg);
			}
			EditAvaliationResult->Text = "\"" + res1 + "\"";

		} else {
			if (p1 != NULL) {
				arg = (float)p1->evaluationFunction(criteria);
				res1.printf(doubleFormat, arg);
			}
			if (p2 != NULL) {
				arg = (float)p2->evaluationFunction(criteria);
				res2.printf(doubleFormat, arg);
			}
			EditAvaliationResult->Text = "\"" + res1 + "\" / \"" + res2 + "\"";

		}

	}

}

//---------------------------------------------------------------------------
// Sheet INSTRUMENTS
//---------------------------------------------------------------------------
void __fastcall TFormRepertoireMain::TabSheetInstrumentsShow(TObject *Sender)
{
	/* desabilita os controles de edicao */
    EditInstrName->Text = "--- choose instrument or create new ---";
    EditInstrName->Enabled = false;
    ComboBoxFreqClass->ItemIndex = -1;
	ComboBoxFreqClass->Enabled = false;
	ListViewInstrNotes->Clear();
    ListViewInstrNotes->Enabled = false;
    ButtonAddInstrNote->Enabled = false;
    ButtonRemoveInstrNote->Enabled = false;
	ComboBoxMidiNote->ItemIndex = -1;
    ComboBoxMidiNote->Enabled = false;
    ComboBoxNoteRelPitch->ItemIndex = -1;
	ComboBoxNoteRelPitch->Enabled = false;
	ButtonDeleteInstr->Enabled = false;

    ButtonNewInstrSave->Caption = "New Instrument";
    ButtonNewInstrSave->Enabled = true;

    /* preenche a listview */

	ICbRpInfoRepository* repository = getRhythmRepository();
	Vector_pPercInstrument instruments;
	TListItem *listItem;
	std::string aux;

    repository->getAllInstruments(instruments);

	ListViewInstruments->Clear();
	for (int i = 0; i < instruments.size(); i ++) {
		listItem = ListViewInstruments->Items->Add();
		listItem->Caption = instruments[i]->getName().c_str();
		listItem->SubItems->Add(
        	CbPercInstrument::frequencyClassToStr(instruments[i]->getFrequencyClass()) );
		aux = instruments[i]->getName();
		listItem->SubItems->Add(instruments[i]->getNumNotes());
		listItem->SubItems->Add(repository->getNumPatternsOfInstrument(aux));
    }

}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::ButtonNewInstrSaveClick(
      TObject *Sender)
{
    if (ButtonNewInstrSave->Caption == "New Instrument") {
		/* habilita os controles de edicao */
	    EditInstrName->Text = "";
		EditInstrName->Color = clWindow;
		EditInstrName->ReadOnly = false;
	    EditInstrName->Enabled = true;
	    ComboBoxFreqClass->ItemIndex = -1;
	    ComboBoxFreqClass->Enabled = true;
		ListViewInstrNotes->Clear();
		ListViewInstrNotes->Enabled = true;
	    ButtonAddInstrNote->Enabled = true;
	    ButtonRemoveInstrNote->Enabled = false;
		ComboBoxMidiNote->ItemIndex = -1;
	    ComboBoxMidiNote->Enabled = true;
        ComboBoxNoteRelPitch->ItemIndex = -1;
		ComboBoxNoteRelPitch->Enabled = true;
		ButtonDeleteInstr->Enabled = false;

		ButtonNewInstrSave->Enabled = false;
		ButtonNewInstrSave->Caption = "Save";

	    EditInstrName->SetFocus();

    } else if (ButtonNewInstrSave->Caption == "Save") {

		if (EditInstrName->Text == "") {
		    MessageDlg("Instrument must have a name !",
               mtError, TMsgDlgButtons() << mbOK, -1);
			return;
        }
        if (ComboBoxFreqClass->ItemIndex == -1) {
		    MessageDlg("Instrument must have a frequency class !",
			   mtError, TMsgDlgButtons() << mbOK, -1);
			return;
        }

    	int index = ListViewInstruments->ItemIndex;

		AnsiString freqClass = ComboBoxFreqClass->Items->Strings[ComboBoxFreqClass->ItemIndex];
        ECbInstrFreqClass fc =
	        CbPercInstrument::strToFrequencyClass(freqClass.c_str());
		CbPercInstrument* instr =
			new CbPercInstrument(EditInstrName->Text.c_str(), fc);

        // adiciona notas ...
        CbPercNote note(-1);
		TListItem *listItem;

        for (int i = 0; i < ListViewInstrNotes->Items->Count; i ++) {
			listItem = ListViewInstrNotes->Items->Item[i];

			sscanf(listItem->Caption.c_str(), "%*s (%d)", &note.midiNote);

			note.pitch = CbPercInstrument::strToNotePitch(listItem->SubItems->Strings[0].c_str());
            instr->addNote(note);
        }

		// se nao tiver item selecionado, entao eh novo ...
		if (index == -1) {
			getRhythmRepository()->addInstrument(*instr);
			updateStatusBar();
        } else {
			getRhythmRepository()->changeInstrument(*instr);
        }

        delete instr;

		TabSheetInstrumentsShow(NULL);
}

}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::ButtonDeleteInstrClick(TObject *Sender)
{
	getRhythmRepository()->deleteInstrument(EditInstrName->Text.c_str());
	TabSheetInstrumentsShow(NULL);
	updateStatusBar();
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::ListViewInstrumentsSelectItem(
	  TObject *Sender, TListItem *listItem, bool Selected)
{
    if (listItem == NULL || Selected == false) {
        /* Desabilita os controles de edicao do instrumento */
	    EditInstrName->Text =  "--- choose instrument or create new ---";
	    EditInstrName->Color = clBtnFace;
	    EditInstrName->ReadOnly = true;
	    EditInstrName->Enabled = false;
		ComboBoxFreqClass->ItemIndex = -1;
	    ComboBoxFreqClass->Enabled = false;
	    ListViewInstrNotes->Clear();
	    ListViewInstrNotes->Enabled = false;
	    ButtonAddInstrNote->Enabled = false;
	    ButtonRemoveInstrNote->Enabled = false;
		ComboBoxMidiNote->ItemIndex = -1;
	    ComboBoxMidiNote->Enabled = false;
        ComboBoxNoteRelPitch->ItemIndex = -1;
		ComboBoxNoteRelPitch->Enabled = false;
		ButtonDeleteInstr->Enabled = false;

		ButtonNewInstrSave->Enabled = true;
		ButtonNewInstrSave->Caption = "New Instrument";

	} else {
		/* mostra informaçoes nos controles de ediçao */

        const CbPercInstrument *instr = getRhythmRepository()->getInstrument(listItem->Caption.c_str());

	    EditInstrName->Enabled = true;
        EditInstrName->Color = clBtnFace;
	    EditInstrName->ReadOnly = true;
   	    EditInstrName->Text = listItem->Caption;

	    ComboBoxFreqClass->Enabled = true;
        ComboBoxFreqClass->ItemIndex = instr->getFrequencyClass();

	    ListViewInstrNotes->Enabled = true;

        TListItem* ntitem;

		ListViewInstrNotes->Clear();
        CbPercNote nt(-1);
        for (int i = 0; i < instr->getNumNotes(); i++) {
            ntitem = ListViewInstrNotes->Items->Add();

            AnsiString noteCaption;
            nt = instr->getNote(i);
			noteCaption.sprintf("%s%d (%d)", NOTES[nt.midiNote % 12], (nt.midiNote / 12), nt.midiNote);
            ntitem->Caption = noteCaption;

			ntitem->SubItems->Add(CbPercInstrument::notePitchToStr(nt.pitch));
		}

	    ButtonAddInstrNote->Enabled = true;
	    ButtonRemoveInstrNote->Enabled = false;
		ComboBoxMidiNote->ItemIndex = -1;
	    ComboBoxMidiNote->Enabled = true;
        ComboBoxNoteRelPitch->ItemIndex = -1;
		ComboBoxNoteRelPitch->Enabled = true;

		ButtonDeleteInstr->Enabled = true;

		ButtonNewInstrSave->Caption = "Save";
		ButtonNewInstrSave->Enabled = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::ListViewInstrNotesSelectItem(
      TObject *Sender, TListItem *listItem, bool Selected)
{
    if (listItem == NULL || Selected == false) {
		ButtonRemoveInstrNote->Enabled = false;
    } else {
		ButtonRemoveInstrNote->Enabled = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::ButtonAddInstrNoteClick(
      TObject *Sender)
{
	int noteIndex = ComboBoxMidiNote->ItemIndex;

	if (noteIndex == -1) {
		MessageDlg("Must choose a MIDI Note !",
               mtError, TMsgDlgButtons() << mbOK, -1);
        return;
    }

    int relPitchIndex = ComboBoxNoteRelPitch->ItemIndex;

	if (relPitchIndex == -1) {
		MessageDlg("Must choose a pitch !",
               mtError, TMsgDlgButtons() << mbOK, -1);
        return;
    }

	// FAZER: e se ja tem na lista !?!

	TListItem *listItem = ListViewInstrNotes->Items->Add();

	listItem->Caption = ComboBoxMidiNote->Items->Strings[noteIndex];
	listItem->SubItems->Add(ComboBoxNoteRelPitch->Items->Strings[relPitchIndex]);

	ComboBoxMidiNote->ItemIndex = -1;
	ComboBoxNoteRelPitch->ItemIndex = -1;

	ButtonNewInstrSave->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::ButtonRemoveInstrNoteClick(
      TObject *Sender)
{
  	ListViewInstrNotes->Items->Delete(ListViewInstrNotes->Selected->Index);
	ButtonNewInstrSave->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::EditInstrNameChange(TObject *Sender)
{
    ButtonNewInstrSave->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::ComboBoxFreqClassChange(
      TObject *Sender)
{
    ButtonNewInstrSave->Enabled = true;
}

//---------------------------------------------------------------------------
// Sheet MIDI INFO
//---------------------------------------------------------------------------
void __fastcall TFormRepertoireMain::TabSheetMidiInfoShow(TObject *Sender)
{
    Vector_pRpInfo patterns;
	this->getRhythmRepository()->getAllRpInfos(patterns);

	// preenche combo-box com os arquivos
    AnsiString format = "(%0" + AnsiString(this->numDigits) + "d) - ";
    AnsiString aux;
    int id;

	ComboBoxMiFile->Clear();
    for (int i = 0; i < patterns.size(); i ++) {
		id = patterns[i]->getId();
		aux.printf(format.c_str(), id);
        aux = aux + patterns[i]->getFileName().c_str();
		ComboBoxMiFile->Items->Add(aux);
    }

    EditMiTracks->Text = "";
	EditMiResolution->Text = "";
    EditMiTempo->Text = "";
	EditMiMeter->Text = "";
    EditMiMaxTick->Text = "";
    EditMiBars->Text = "";

    ListViewMiChEvents->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::ComboBoxMiFileChange(TObject *Sender)
{
	if (ComboBoxMiFile->ItemIndex == -1) {
    	return ;
    }

	AnsiString optionStr = ComboBoxMiFile->Items->Strings[ComboBoxMiFile->ItemIndex];
	int prefixSz = this->numDigits + 5;  // tamanho do prefixo: "(x) - ";
	AnsiString fileName = optionStr.SubString(prefixSz + 1, optionStr.Length() - prefixSz);
	AnsiString fullPath = applicationDirectory + "\\" + MIDI_REPERT_DIR + "\\" + fileName;
	AnsiString meterStr;

	if (this->miMidiFile != NULL) {
    	delete this->miMidiFile;
	}

	this->miMidiFile = new FmMidiFile(fullPath.c_str());

    // preenche campos de texto ...
    EditMiTracks->Text = this->miMidiFile->getNumTracks();;
	EditMiResolution->Text = this->miMidiFile->getResolution();
    EditMiTempo->Text = this->miMidiFile->getTempo();
    meterStr.printf("%d/", this->miMidiFile->getNumerator());
	meterStr.cat_printf("%d", 1 << this->miMidiFile->getDenominator());
	EditMiMeter->Text = meterStr;

    EditMiMaxTick->Text = this->miMidiFile->getMaxTick();
	EditMiBars->Text = this->miMidiFile->getNumBars();

    // preenche listview ...
    const FmMidiFileTrack* track;
	Vector_pMidiEvent events;
	TListItem* listItem;

    ListViewMiChEvents->Clear();
    for (int t = 0; t < miMidiFile->getNumTracks(); t ++) {
		track = miMidiFile->getTrack(t);
        track->getAllEvents(events);

        for (int e = 0; e < events.size(); e ++) {
			listItem = ListViewMiChEvents->Items->Add();
            listItem->Caption = t;

            listItem->SubItems->Add(events[e]->getTick());

            AnsiString aux = FmMidiEvent::eventTpToStr(events[e]->getType());

			if (events[e]->isMetaEvent()) {
               	FmMidiMetaEvent* mtEv = static_cast<FmMidiMetaEvent*>(events[e]);
               	aux.cat_printf(" (%s)", FmMidiEvent::metaEventTpToStr(mtEv->getMetaEventType()));
                listItem->SubItems->Add(aux);

                switch (mtEv->getMetaEventType()) {
                case MD_META_SET_TEMPO:
					aux.printf("%u µs/b ", mtEv->getTempo());
					aux.cat_printf("(%.2f b/min)", mtEv->getTempoBPM());
					break;
                case MD_META_TIME_SIGNAT:
                   	aux.printf("%d ", mtEv->getNumerator());
                   	aux.cat_printf("%d ", mtEv->getDenominator());
                    aux.cat_printf("%d ", mtEv->getMetronomePer24Clocks());
                    aux.cat_printf("%d", mtEv->get32ndNotesPer24Clocks());
                   	break;
                default:
                   	aux = "";
                }
                listItem->SubItems->Add(aux);

            } else if (events[e]->isChannelEvent()) {
               	FmMidiChannelEvent* chEv = static_cast<FmMidiChannelEvent*>(events[e]);
               	aux.cat_printf(" (ch: %02d)", chEv->getChannel());
                listItem->SubItems->Add(aux);

                switch (chEv->getType()) {
                case MD_NOTE_ON:
                case MD_NOTE_OFF:
                	aux.printf("ptch: %d  ", chEv->getPitch());
                   	aux.cat_printf("vel: %d", chEv->getVelocity());
                  	break;
				default:
					aux.printf("0x%02x ", chEv->getData1());
                   	aux.cat_printf("0x%02x", chEv->getData2());
                }
                listItem->SubItems->Add(aux);

            } else {
               	aux.cat_printf(" (error!)", 0); // na deveria chegar aqui !
                listItem->SubItems->Add(aux);
			}

        } //end for

    } //end for

}
//---------------------------------------------------------------------------

void __fastcall TFormRepertoireMain::TabSheetMidiInfoHide(TObject *Sender)
{
	if (this->miMidiFile != NULL) {
		delete this->miMidiFile;
        this->miMidiFile = NULL;
    }
}
//---------------------------------------------------------------------------
