//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <dos.h>
#include <direct.h>
#include "FormChooseDir.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cdiroutl"
#pragma resource "*.dfm"
TFormChooseDir *FormChooseDir;
//---------------------------------------------------------------------------
__fastcall TFormChooseDir::TFormChooseDir(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TFormChooseDir::FormShow(TObject *Sender)
{
    int currDrive = _getdrive();
    char aux;

    ComboBoxDrive->Items->Clear();
    ComboBoxDrive->Items->Add('A');

    /* If we can switch to the drive, it exists. */
    for(int drive = 2; drive <= 26; drive++ ) {
        if (! _chdrive(drive)) {
            aux = 'A' + drive - 1;
            ComboBoxDrive->Items->Add(aux);
            if (drive == currDrive) {
                ComboBoxDrive->ItemIndex = ComboBoxDrive->Items->Count - 1;
            }
        }
    }
    _chdrive(currDrive);

    /*/
    //ULONG uDriveMask = _getdrives();
    int drive = 0;
    while (uDriveMask) {
        if (uDriveMask & 1) {
            ComboBoxDrive->Items->Add('A' + drive - 1);
            if (drive == curDrive) {
                ComboBoxDrive->ItemIndex = ComboBoxDrive->Items->Count - 1;
            }
        }

        drive;
    }
    //*/

}

//---------------------------------------------------------------------------
void __fastcall TFormChooseDir::ComboBoxDriveChange(TObject *Sender)
{
    int index = ComboBoxDrive->ItemIndex;
    CDirectoryOutline1->Directory =
        ComboBoxDrive->Items->Strings[index] + ":\\";
}

//---------------------------------------------------------------------------
AnsiString __fastcall TFormChooseDir::getChosenDirectory() {
    return CDirectoryOutline1->Directory;
}

//---------------------------------------------------------------------------
