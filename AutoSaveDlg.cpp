//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <FileCtrl.hpp>

#include "AutoSaveDlg.h"
//---------------------------------------------------------------------
#pragma link "PathEllipsisLabel"
#pragma resource "*.dfm"

TAutoSaveDialog *AutoSaveDialog;

//---------------------------------------------------------------------
__fastcall TAutoSaveDialog::TAutoSaveDialog(TComponent* AOwner)
  : TForm(AOwner)
{
  Initialized = false;
  LoadOptions();
  Initialized = true;
}

//---------------------------------------------------------------------
void TAutoSaveDialog::LoadOptions()
{
  autosaver.LoadOptions();

//  ckByPass->Checked = autosaver.Bypass;
//  ckContinuous->Checked = autosaver.Continuous;
  cbType->ItemIndex = autosaver.ImageType;
  edDirectory->Text = autosaver.Directory;
  edPrefix->Text = autosaver.Prefix;
  udDigits->Position = (short) autosaver.Digits;
  udOffset->Position = (short) autosaver.NextValue;
  rbPrompt->Checked = autosaver.ExistAction == 0;
  rbOverwrite->Checked = autosaver.ExistAction == 1;
  rbRename->Checked = autosaver.ExistAction == 2;
}

//---------------------------------------------------------------------
void TAutoSaveDialog::SaveOptions()
{
  RefreshOptions();

  autosaver.SaveOptions();
}

//---------------------------------------------------------------------
void TAutoSaveDialog::RefreshOptions()
{
//  autosaver.Bypass = ckByPass->Checked;
//  autosaver.Continuous = ckContinuous->Checked;
  autosaver.ImageType = cbType->ItemIndex;
  autosaver.Directory = edDirectory->Text;
  autosaver.Prefix = edPrefix->Text;
  autosaver.Digits = udDigits->Position;
  autosaver.NextValue = udOffset->Position;
  autosaver.ExistAction = rbPrompt->Checked * 0 +
                        rbOverwrite->Checked * 1 +
                        rbRename->Checked * 2;
}

//---------------------------------------------------------------------
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
void __fastcall TAutoSaveDialog::InputChange(TObject *Sender)
{
  if (Initialized)
  {
    RefreshOptions();
    elbPreview->Caption = autosaver.GetFullPathName();
  }
}

//---------------------------------------------------------------------------
void __fastcall TAutoSaveDialog::edDigitsChange(TObject *Sender)
{
  // For filenames with no digits, offset is meaningless..
  lbOffset->Enabled = (udDigits->Position != 0);
  edOffset->Enabled = (udDigits->Position != 0);
  udOffset->Enabled = (udDigits->Position != 0);
  InputChange(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TAutoSaveDialog::ckByPassClick(TObject *Sender)
{
//  ckContinuous->Enabled = ckByPass->Checked;
}

//---------------------------------------------------------------------------
void __fastcall TAutoSaveDialog::bnOkClick(TObject *Sender)
{
  SaveOptions();
}

//---------------------------------------------------------------------
int BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
  if (uMsg == BFFM_INITIALIZED)
  {
    // Move dialog to the center of the screen
    TRect rcDialog;
    GetWindowRect(hwnd, &rcDialog);

    int dialogLeft = (Screen->Width - rcDialog.Width()) / 2;
    int dialogTop = (Screen->Height - rcDialog.Height()) / 2;

    MoveWindow(hwnd, dialogLeft, dialogTop,
    rcDialog.Width(), rcDialog.Height(), true);
  }
  return 0;
}

//---------------------------------------------------------------------------

