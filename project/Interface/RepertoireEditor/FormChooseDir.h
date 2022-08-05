//---------------------------------------------------------------------------

#ifndef UnitFormChooseDirH
#define UnitFormChooseDirH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "cdiroutl.h"
#include <Grids.hpp>
#include <Outline.hpp>
//---------------------------------------------------------------------------
class TFormChooseDir : public TForm
{
__published:	// IDE-managed Components
    TPanel *PanelBottom;
    TCDirectoryOutline *CDirectoryOutline1;
    TButton *ButtonOk;
    TButton *ButtonCancel;
    TPanel *PanelTop;
    TLabel *LabelDriver;
    TComboBox *ComboBoxDrive;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ComboBoxDriveChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    AnsiString __fastcall getChosenDirectory();
    __fastcall TFormChooseDir(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormChooseDir *FormChooseDir;
//---------------------------------------------------------------------------
#endif
