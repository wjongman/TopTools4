//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <FileCtrl.hpp>

#include "AutoSaveDlg.h"
//---------------------------------------------------------------------------
#pragma link "PathEllipsisLabel"
#pragma resource "*.dfm"

TAutoSaveDialog *AutoSaveDialog;

//---------------------------------------------------------------------------
__fastcall TAutoSaveDialog::TAutoSaveDialog(TComponent* AOwner)
: TForm(AOwner)
{
    LoadOptions();
}

//---------------------------------------------------------------------------
void __fastcall TAutoSaveDialog::bnOkClick(TObject *Sender)
{
    SaveOptions();
}

//---------------------------------------------------------------------------
void TAutoSaveDialog::LoadOptions()
{
    autosaver.LoadOptions();

    loading = true;
    cbType->ItemIndex = autosaver.ImageType;
    edDirectory->Text = autosaver.Directory;
    edPrefix->Text = autosaver.Prefix;
    udDigits->Position = (short) autosaver.Digits;
    udOffset->Position = (short) autosaver.NextValue;
    rbPrompt->Checked = autosaver.ExistAction == 0;
    rbOverwrite->Checked = autosaver.ExistAction == 1;
    rbRename->Checked = autosaver.ExistAction == 2;
    loading = false;
}

//---------------------------------------------------------------------------
void TAutoSaveDialog::SaveOptions()
{
    RefreshOptions();
    autosaver.SaveOptions();
}

//---------------------------------------------------------------------------
void TAutoSaveDialog::RefreshOptions()
{
    autosaver.ImageType = cbType->ItemIndex;
    autosaver.Directory = edDirectory->Text;
    autosaver.Prefix = edPrefix->Text;
    autosaver.Digits = udDigits->Position;
    autosaver.NextValue = udOffset->Position;
    autosaver.ExistAction = rbPrompt->Checked * 0 +
                            rbOverwrite->Checked * 1 +
                            rbRename->Checked * 2;
}

//---------------------------------------------------------------------------
void __fastcall TAutoSaveDialog::InputChange(TObject *Sender)
{
    if (loading)
       return;
        
    RefreshOptions();
    elbPreview->Caption = autosaver.GetFullPathName();
}

//---------------------------------------------------------------------------
void __fastcall TAutoSaveDialog::bnBrowseDirClick(TObject *Sender)
{
    String SelectedDir;
    if (SelectDirectory("Select folder where images will be saved:",
                        "", SelectedDir))
    {
        edDirectory->Text = IncludeTrailingBackslash(SelectedDir);
    }
}

//---------------------------------------------------------------------------


