//---------------------------------------------------------------------------


#ifndef FrameExecutionPlanH
#define FrameExecutionPlanH
//---------------------------------------------------------------------------
#include "CInBalada\CbFacade.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>

#include <vector>
#include <string>

#include <ActnList.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ActnList.hpp>

//---------------------------------------------------------------------------
class TFrameExecPlan : public TFrame
{
__published:	// IDE-managed Components
	TDrawGrid *DrawGrid;
	TPopupMenu *PopupRoles;
	TPanel *MainPanel;
	TImageList *ImageListRoles;
	TImageList *ImageList1;
	void __fastcall DrawGridContextPopup(TObject *Sender, TPoint &MousePos,
		  bool &Handled);
	void __fastcall DrawGridSelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect);
	void __fastcall DrawGridDrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State);

private:	// User declarations

	const int COL_WIDTH;
	const int ROW_HEIGHT;

	CbFacade* facade;
    AnsiString projectFile;

	int vertIdent;      // distancia da area util às bordas superior e inferior
	int firstColWidth;  // separacao entre a primeira linha e as demais

	int __fastcall GetNumBars();
	void __fastcall SetNumBars(int newNumBars);

	void __fastcall AddAgent(const char *instrName, int roleIndex);
	int __fastcall GetNumAgents();
	void __fastcall ClearAgents(); 

	void __fastcall InsertPopupOption(int roleIndex, const char* role);
	void __fastcall PopupOptionChosen(TObject *Sender);
	void __fastcall CheckPopupOptions(int agentIndex, int bar);

	void __fastcall SetAgentRole(int agentIndex, int bar, int roleIndex);

	void __fastcall SetVertIdent(int sz);
	void __fastcall SetFirstColWidth(int sz);

	void __fastcall DrawAgentIdentificationCell(int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall DrawAgentRoleCell(int ACol, int ARow, TRect &Rect, TGridDrawState State);

public:		// User declarations

	__fastcall TFrameExecPlan(TComponent* Owner);

	int __fastcall GetSelectedAgent();
	void __fastcall CheckFacadeUpdate();

	__property int RowSpacing = {read=vertIdent, write=SetVertIdent, default=10};
	__property int FirstColumnWidth = {read=firstColWidth, write=SetFirstColWidth, default=100};
};
//---------------------------------------------------------------------------
extern PACKAGE TFrameExecPlan *FrameExecPlan;
//---------------------------------------------------------------------------
#endif
