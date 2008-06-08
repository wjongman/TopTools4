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
  options.Load();

  ckByPass->Checked = options.Bypass;
  ckContinuous->Checked = options.Continuous;
  cbType->ItemIndex = options.ImageType;
  edDirectory->Text = options.Directory;
  edPrefix->Text = options.Prefix;
  udDigits->Position = (short) options.Digits;
  udOffset->Position = (short) options.NextValue;
  rbPrompt->Checked = options.ExistAction == 0;
  rbOverwrite->Checked = options.ExistAction == 1;
  rbRename->Checked = options.ExistAction == 2;
}

//---------------------------------------------------------------------
void TAutoSaveDialog::SaveOptions()
{
  RefreshOptions();

  options.Save();
}

//---------------------------------------------------------------------
void TAutoSaveDialog::RefreshOptions()
{
  options.Bypass = ckByPass->Checked;
  options.Continuous = ckContinuous->Checked;
  options.ImageType = cbType->ItemIndex;
  options.Directory = edDirectory->Text;
  options.Prefix = edPrefix->Text;
  options.Digits = udDigits->Position;
  options.NextValue = udOffset->Position;
  options.ExistAction = rbPrompt->Checked * 0 +
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
    elbPreview->Caption = options.GetFullPathName();
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
  ckContinuous->Enabled = ckByPass->Checked;
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

