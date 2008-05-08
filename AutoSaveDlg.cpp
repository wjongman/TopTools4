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

////---------------------------------------------------------------------
//void TAutoSaveDialog::LoadOptions()
//{
//  m_Options.Load();
//
//  // Init default options
////  m_Options.Init(g_RegBaseKey, "capture\\autosave");
//
//  edDirectory->Text = m_Options.Get("directory", GetSpecialFolderPath(CSIDL_DESKTOPDIRECTORY));
//  edPrefix->Text = m_Options.Get("filename", "Snapshot");
//  udDigits->Position = (short) m_Options.Get("digits", 2);
//  udOffset->Position = (short) m_Options.Get("nextvalue", 1);
//  cbType->ItemIndex = m_Options.Get("imagetype", 0);
//
//  int existaction = m_Options.Get("existaction", 0);
//  rbPrompt->Checked = (existaction == 0);
//  rbOverwrite->Checked = (existaction == 1);
//  rbRename->Checked = (existaction == 2);
//
//  ckByPass->Checked = m_Options.Get("bypassmenu", false);
//  ckContinuous->Checked = m_Options.Get("continuous", false);
//}

//---------------------------------------------------------------------
void TAutoSaveDialog::LoadOptions()
{
  m_Options.Load();
/*
  edDirectory->Text = m_Options.GetString("directory");
  edPrefix->Text = m_Options.GetString("filename");
  udDigits->Position = (short) m_Options.GetInt("digits");
  udOffset->Position = (short) m_Options.GetInt("nextvalue");
  cbType->ItemIndex = m_Options.GetInt("imagetype");

  int existaction = m_Options.GetInt("existaction");
  rbPrompt->Checked = (existaction == 0);
  rbOverwrite->Checked = (existaction == 1);
  rbRename->Checked = (existaction == 2);

  ckByPass->Checked = m_Options.GetBool("bypassmenu");
  ckContinuous->Checked = m_Options.GetBool("continuous");
  */
}

//---------------------------------------------------------------------
void TAutoSaveDialog::SaveOptions()
{
  UpdateOptions();
  m_Options.Save();
}

//---------------------------------------------------------------------
void TAutoSaveDialog::UpdateOptions()
{
  m_Options.Set("directory", edDirectory->Text);
  m_Options.Set("filename", edPrefix->Text);
  m_Options.Set("digits", udDigits->Position);
  m_Options.Set("nextvalue", udOffset->Position);
  m_Options.Set("imagetype", cbType->ItemIndex);
  m_Options.Set("bypassmenu", ckByPass->Checked);
  m_Options.Set("continuous", ckContinuous->Checked);
  int existaction = rbPrompt->Checked * 0 +
                    rbOverwrite->Checked * 1 +
                    rbRename->Checked * 2;
  m_Options.Set("existaction", existaction);
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
    UpdateOptions();
    elbPreview->Caption = m_Options.GetFullPathName();
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

/*/---------------------------------------------------------------------
function BrowseDialog
 (const Title: string; const Flag: integer): string;

  String sResult = "";

  // Get the shell's IMalloc interface, we must use this
  // interface to free the memory that is allocated by
  // the shell when it passes us an ITEMIDLIST in *pidl
  LPMALLOC Allocator;
  if (::SHGetMalloc(&Allocator) == NOERROR)
  {
    BROWSEINFO bi = { 0 };
    ITEMIDLIST *pidl;
    HWND hwndOwner = NULL;
    if (::SHGetSpecialFolderLocation(hwndOwner, FolderSpec, &pidl) == NOERROR)
    {
      char folderpath[MAX_PATH];
      if (::SHGetPathFromIDList(pidl, folderpath))
      {
        sResult = folderpath;
        sResult += "\\";
      }
      Allocator->Free(pidl);
    }
    Allocator->Release();
  }
  return sResult;

 var
  lpItemID : PItemIDList;
  BrowseInfo : TBrowseInfo;
  DisplayName : array[0..MAX_PATH] of char;
  TempPath : array[0..MAX_PATH] of char;
begin
  Result:='';
  FillChar(BrowseInfo, sizeof(TBrowseInfo), #0);
  with BrowseInfo do begin
    hwndOwner := Application.Handle;
    pszDisplayName := @DisplayName;
    lpszTitle := PChar(Title);
    ulFlags := Flag;
    lpfn := BrowseDialogCallBack;
  end;
  lpItemID := SHBrowseForFolder(BrowseInfo);
  if lpItemId <> nil then begin
    SHGetPathFromIDList(lpItemID, TempPath);
    Result := TempPath;
    GlobalFreePtr(lpItemID);
  end;
end;
*/
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



