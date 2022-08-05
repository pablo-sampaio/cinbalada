//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrameExecutionPlan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFrameExecPlan *FrameExecPlan;

using namespace std;

//---------------------------------------------------------------------------
__fastcall TFrameExecPlan::TFrameExecPlan(TComponent* Owner)
	: TFrame(Owner),
	COL_WIDTH(71),
	ROW_HEIGHT(22)
{
	this->ClearAgents();
	this->SetNumBars(0);

	this->SetVertIdent(10);
	this->SetFirstColWidth(100);

	TGridRect rect;
	rect.Left = -1;
	DrawGrid->Selection = rect;

	this->facade = CbFacade::getInstance();

	// adiciona os papeis no menu pop-up...
	for (int i = 0; i < facade->getNumRoles(); i ++) {
		InsertPopupOption(i, facade->getRole(i)->getName());
	}

}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::SetVertIdent(int sz) {
	this->vertIdent = sz;

	for (int i = 0; i < DrawGrid->RowCount; i += 2) {
		DrawGrid->RowHeights[i] = sz;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::SetFirstColWidth(int width) {
	this->firstColWidth = width;
	DrawGrid->ColWidths[0] = width;
}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::InsertPopupOption(int roleIndex, const char* role) {
	TMenuItem *newItem = new TMenuItem(PopupRoles);
	PopupRoles->Items->Add(newItem);
	newItem->Tag = roleIndex;
	newItem->OnClick = PopupOptionChosen;
	newItem->Caption = role;
	newItem->AutoCheck = true;
	newItem->RadioItem = true;
	newItem->GroupIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::CheckFacadeUpdate() {
	bool newOpenProject = (this->projectFile != this->facade->getProjectFileName());

    if (newOpenProject == true) {
    	this->projectFile = this->facade->getProjectFileName();
    }

	int newNumAgents = this->facade->getNumAgents();

	if (newOpenProject || (newNumAgents != this->GetNumAgents())) {
		this->ClearAgents();

		int defaultRoleIndex;
		for (int i = 0; i < facade->getNumAgents(); i ++) {
			defaultRoleIndex = facade->getAgentDefaultRole(i);
			this->AddAgent(facade->getAgent(i)->getName().c_str(), defaultRoleIndex);
		}

		DrawGrid->Invalidate();
	}

	int newNumBars = this->facade->getBarsCycle();

	if (newOpenProject || (newNumBars != this->GetNumBars())) {
		this->SetNumBars(newNumBars);
		DrawGrid->Invalidate();
	}

}
//---------------------------------------------------------------------------

int __fastcall TFrameExecPlan::GetSelectedAgent() {
	int agentIndex = -1;
	TGridRect rect = DrawGrid->Selection;

	// se houver linha selecionada e nao for a linha de separacao
	if ((rect.Top != -1) && ((rect.Top % 2) != 0)) {
		// se for a primeira coluna
		if (rect.Left == 0) {
			agentIndex = (rect.Top - 1) / 2;
		}
	}

	return agentIndex;
}
//---------------------------------------------------------------------------

int __fastcall TFrameExecPlan::GetNumAgents() {
	return ((DrawGrid->RowCount -1) / 2);
}
//---------------------------------------------------------------------------

int __fastcall TFrameExecPlan::GetNumBars() {
	return DrawGrid->ColCount - 1;
}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::SetNumBars(int newNumBars) {
	int oldLength = DrawGrid->ColCount;

	DrawGrid->ColCount = newNumBars + 1;

	// atualiza os tamanhos das colunas do grid
	for (int i = oldLength; i <= newNumBars; i ++) {
		DrawGrid->ColWidths[i] = COL_WIDTH;
	}

	DrawGrid->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::ClearAgents() {
	DrawGrid->RowCount = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::AddAgent(const char *instrName, int roleIndex) {
	int prevRowCount = DrawGrid->RowCount;

	DrawGrid->RowCount += 2;

	DrawGrid->RowHeights[prevRowCount] = ROW_HEIGHT;
	DrawGrid->RowHeights[prevRowCount + 1] = this->vertIdent;

	DrawGrid->Canvas->Brush->Color = DrawGrid->Color;
	DrawGrid->Canvas->Brush->Style = bsSolid;

	TRect rect (0, 0, DrawGrid->Width, DrawGrid->Height);
	DrawGrid->Canvas->FillRect(rect);
}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::PopupOptionChosen(TObject *Sender)
{
	int plan = (DrawGrid->Selection.Top - 1) / 2;
	int bar = DrawGrid->Selection.Left - 1;

	// encontra qual foi a opção escolhida...
	TMenuItem *ClickedItem = dynamic_cast<TMenuItem *>(Sender);
	if (ClickedItem != NULL) {
		ClickedItem->Checked = true;
		SetAgentRole(plan, bar, ClickedItem->Tag);
	}

}
//---------------------------------------------------------------------------
void __fastcall TFrameExecPlan::SetAgentRole(int agent, int bar, int newRoleIndex) {
	int currRoleIndex = this->facade->getAgentRole(agent, bar);

	if (newRoleIndex != currRoleIndex) {
		this->facade->setAgentRole(agent, bar, newRoleIndex);
		DrawGrid->Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrameExecPlan::DrawGridContextPopup(TObject *Sender,
	  TPoint &MousePos, bool &Handled)
{
	int col, row;

	DrawGrid->MouseToCell(MousePos.x, MousePos.y, col, row);

	if (((row % 2) == 0) || (col == 0) || (row == -1)) {
		Handled = true;

	} else {
		TGridRect rect;

		rect.Left = col;
		rect.Top = row;
		rect.Right = col;
		rect.Bottom = row;

		DrawGrid->Selection = rect;

		CheckPopupOptions((row-1)/2, col-1);
	}

}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::CheckPopupOptions(int agentIndex, int bar) {
	int roleIndex = this->facade->getAgentRole(agentIndex, bar);

	for (int i = PopupRoles->Items->Count - 1; i >= 0; i --) {
		if (PopupRoles->Items->Items[i]->Tag == roleIndex) {
			PopupRoles->Items->Items[i]->Checked = true;
		}

		AnsiString role = PopupRoles->Items->Items[i]->Caption;
		role.Delete(role.Pos("&"), 1);
		PopupRoles->Items->Items[i]->Enabled = facade->agentHasPatternOfRole(agentIndex, role.c_str());
	}

}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::DrawGridSelectCell(TObject *Sender,
	  int ACol, int ARow, bool &CanSelect)
{
	// se forem as linhas de separacao
	//if ((ARow % 2) == 0) {
	//	CanSelect = false;
	//}
}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::DrawGridDrawCell(TObject *Sender, int ACol,
	  int ARow, TRect &Rect, TGridDrawState State)
{
	// para evitar de desenhar assim q a fachada foi mudada...
	CheckFacadeUpdate();

	// se nao for uma linha de separação
	if ((ARow % 2) != 0) {

		// se for coluna que identifica o agente
		if (ACol == 0) {
			DrawAgentIdentificationCell(ACol, ARow, Rect, State);
		} else {
			DrawAgentRoleCell(ACol, ARow, Rect, State);
		}

	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::DrawAgentIdentificationCell(int ACol, int ARow, TRect &Rect, TGridDrawState State) {
	int agentIndex = (ARow - 1) / 2;

	// limpa area da celula
	DrawGrid->Canvas->Brush->Color = DrawGrid->Color;
	DrawGrid->Canvas->FillRect(Rect);

	if (State.Contains(gdSelected)) {
		TRect aux (Rect.left + 1, Rect.top + 1, Rect.right - 1, Rect.bottom - 1);
		DrawGrid->Canvas->Brush->Color = clWhite;
		DrawGrid->Canvas->FillRect(aux);
	}

	AnsiString title = this->facade->getAgent(agentIndex)->getName().c_str();
	DrawGrid->Canvas->TextOutA(Rect.left + 5, Rect.top + 2, title);
	DrawGrid->Canvas->Brush->Color = clWhite;
	DrawGrid->Canvas->DrawFocusRect(Rect);
}
//---------------------------------------------------------------------------

void __fastcall TFrameExecPlan::DrawAgentRoleCell(int ACol, int ARow, TRect &Rect, TGridDrawState State) {
	int agentIndex = (ARow - 1) / 2;
	int bar = ACol - 1;
	int roleIndex = this->facade->getAgentRole(agentIndex, bar);

	TRect aux (Rect.left + 1, Rect.top + 1, Rect.right - 1, Rect.bottom - 1);

	// limpa a area da celula
	DrawGrid->Canvas->Brush->Color = DrawGrid->Color;
	DrawGrid->Canvas->FrameRect(Rect);

	if (State.Contains(gdSelected)) {
		DrawGrid->Canvas->Brush->Color = clWhite;
		DrawGrid->Canvas->DrawFocusRect(Rect);
	}

	TColor color;
	Graphics::TBitmap* bitmap = new Graphics::TBitmap;

	// colocar como atributo ??
	TColor roleColors[] = {clAqua, clBlue, clPurple, clRed, clWhite, clBlack, clYellow};

	color = roleColors[roleIndex];

	if (roleIndex < ImageList1->Count) {
		ImageList1->GetBitmap(roleIndex, bitmap);
	}

	DrawGrid->Canvas->Brush->Color = color;
	DrawGrid->Canvas->FillRect(aux);

//    >> se quiser fazer estilo peças q encaixam
//    bitmap->LoadFromFile("images\\emoticon.bmp");
//    bitmap->Transparent = true;

	//DrawGrid->Canvas->Draw(aux.left, aux.top, bitmap);

	delete bitmap;
}
//---------------------------------------------------------------------------
