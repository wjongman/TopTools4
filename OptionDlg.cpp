//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#pragma package(smart_init)
#pragma link "HotkeyPanel"
#pragma resource "*.dfm"

//TToolOptionsDialog *ToolOptionsDialog;

#include "OptionDlg.h"
#include "AutoSaveDlg.h"
#include "PersistOptions.h"
#include "HotkeyManager.h"

//---------------------------------------------------------------------------
__fastcall TToolOptionsDialog::TToolOptionsDialog(TComponent* Owner)
: TForm(Owner)
{
  // Set Form dimensions relative to plMarker so we scale properly
  // independent of display settings (ie. "Large fonts")
  ClientHeight = plMarker->Top;
  ClientWidth = plMarker->Left;

  plRef->Visible = false;
  ActivatePage("General");
}

//---------------------------------------------------------------------------
__fastcall TToolOptionsDialog::TToolOptionsDialog(TComponent* Owner,
                                                  const String& PageName)
: TForm(Owner)
{
  // Set Form dimensions relative to plMarker so we scale properly
  // independent of display settings (ie. "Large fonts")
  ClientHeight = plMarker->Top;
  ClientWidth = plMarker->Left;

  plRef->Visible = false;
  ActivatePage(PageName);
}

//---------------------------------------------------------------------------
__fastcall TToolOptionsDialog::~TToolOptionsDialog()
{
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::ActivatePage(const String sActive)
{
  TPanel* ActivePanel = NULL;

  if (sActive == "General")
    ActivePanel = plGeneral;
  else if (sActive == "Ruler")
    ActivePanel = plRuler;
  else if (sActive == "Loupe")
    ActivePanel = plLoupe;
  else if (sActive == "Info")
    ActivePanel = plInfo;
  else if (sActive == "Base Converter")
    ActivePanel = plBaseconv;
  else if (sActive == "Screen Grabber")
    ActivePanel = plGrabber;
  else if (sActive == "Tray Icon")
    ActivePanel = plTrayicon;

  if (ActivePanel)
  {
    ActivePage = sActive;
    HideAll();
    ActivePanel->Left = plRef->Left;
    ActivePanel->Top = plRef->Top;
    ActivePanel->Visible = true;
  }
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::HideAll()
{
  plGeneral->Visible = false;
  plGeneral->Left = plRef->Left;
  plGeneral->Top = plRef->Top;

  plRuler->Visible = false;
  plRuler->Left = plRef->Left;
  plRuler->Top = plRef->Top;

  plLoupe->Visible = false;
  plLoupe->Left = plRef->Left;
  plLoupe->Top = plRef->Top;

  plInfo->Visible = false;
  plInfo->Left = plRef->Left;
  plInfo->Top = plRef->Top;

  plBaseconv->Visible = false;
  plBaseconv->Left = plRef->Left;
  plBaseconv->Top = plRef->Top;

  plGrabber->Visible = false;
  plGrabber->Left = plRef->Left;
  plGrabber->Top = plRef->Top;

  plTrayicon->Visible = false;
  plTrayicon->Left = plRef->Left;
  plTrayicon->Top = plRef->Top;
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::FormShow(TObject *Sender)
{
  // Set focus to currently visible item
  if (lvSelectOption->Items->Count > 0)
  {
    TListItem *item = lvSelectOption->Items->Item[m_CurrentItem];
    item->Selected = true;
    item->Focused = true;
  }

  InitHotkeyPanels();
  InitOptions();
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::lvSelectOptionChange(TObject *Sender,
                                                         TListItem *Item, TItemChange Change)
{
  if (lvSelectOption->ItemFocused)
    ActivatePage(lvSelectOption->ItemFocused->Caption);
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::InitOptions()
{
  // General
  ckAutoStart->Checked = g_ToolOptions.Get("main", "autostart", false);
  ckTrayApp->Checked = g_ToolOptions.Get("main", "istrayapp", false);

  ckRememberSettings->Checked = g_ToolOptions.Get("main", "rememberstate", true);

  TRunMode runmode = g_ToolOptions.GetRunMode();
  switch (runmode)
  {
  case rmPortable:
    ckRememberSettings->Checked = false;
    rbRegistry->Enabled = false;
    rbInifile->Enabled = false;
    break;
  case rmRegistry:
    ckRememberSettings->Checked = true;
    rbRegistry->Enabled = true;
    rbInifile->Enabled = true;
    rbRegistry->Checked = true;
    rbInifile->Checked = false;
    break;
  case rmIniFile:
    ckRememberSettings->Checked = true;
    rbRegistry->Enabled = true;
    rbInifile->Enabled = true;
    rbRegistry->Checked = false;
    rbInifile->Checked = true;
    break;
  }

  if (!g_ToolOptions.ProgramDirIsWriteable())
  {
    rbRegistry->Checked = true;
    rbInifile->Checked = false;
    rbInifile->Enabled = false;
  }

  //  ckRememberSettings->Checked = (runmode != rmPortable);

  ckSingleton->Checked = g_ToolOptions.Get("main", "singleton", false);
  ckOnTop->Checked = g_ToolOptions.Get("main", "stayontop", true);

  int doubleclickaction = g_ToolOptions.Get("main", "doubleclick", dcoControl);
  ckOpenToolbar->Checked = doubleclickaction & dcoControl;
  ckOpenRuler->Checked = doubleclickaction & dcoRuler;
  ckOpenLoupe->Checked = doubleclickaction & dcoLoupe;
  ckOpenInfo->Checked = doubleclickaction & dcoInfo;
  ckOpenBaseconv->Checked = doubleclickaction & dcoBaseconv;

  // Ruler
  udLength->Position = (short) g_ToolOptions.Get("ruler", "length", 1024);
  ckNudgeRuler->Checked = g_ToolOptions.Get("ruler", "arrownudge", true);
  udTransparency->Position = (short) g_ToolOptions.Get("ruler", "transparency", 50);
  cbTransparent->Checked = g_ToolOptions.Get("ruler", "transparent", false);

  // Base converter
  ckBinary->Checked = g_ToolOptions.Get("baseconv", "showbinary", true);

  // Loupe
  udRefresh->Position = (short) g_ToolOptions.Get("loupe", "refresh", 250);

  // Info
  ckPrefix->Checked = g_ToolOptions.Get("info", "prefix", false);
  ckQuotes->Checked = g_ToolOptions.Get("info", "quotes", false);

  // Grabber
  ckAutosave->Checked = g_ToolOptions.Get("capture", "autosave", false);
  ckShowLoupeOnGrab->Checked = g_ToolOptions.Get("capture", "showloupe", false);
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::SaveOptions()
{
  g_ToolOptions.Set("ruler", "length", udLength->Position);
  g_ToolOptions.Set("ruler", "arrownudge", ckNudgeRuler->Checked);
  g_ToolOptions.Set("ruler", "transparency", udTransparency->Position);
  g_ToolOptions.Set("ruler", "transparent", cbTransparent->Checked);

  int doubleclickopen = ckOpenToolbar->Checked * dcoControl +
                        ckOpenRuler->Checked * dcoRuler +
                        ckOpenLoupe->Checked * dcoLoupe +
                        ckOpenInfo->Checked * dcoInfo +
                        ckOpenBaseconv->Checked * dcoBaseconv;

  g_ToolOptions.Set("main", "doubleclick", doubleclickopen);
  g_ToolOptions.Set("main", "autostart", ckAutoStart->Checked);
  g_ToolOptions.Set("main", "istrayapp", ckTrayApp->Checked);
  g_ToolOptions.Set("main", "singleton", ckSingleton->Checked);
  g_ToolOptions.Set("main", "stayontop", ckOnTop->Checked);

  TRunMode runmode = rmPortable;
  if (ckRememberSettings->Checked)
  {
    // See what kind of persistence is requested
    if (rbRegistry->Checked)
    {
      runmode = rmRegistry;
    }
    else if (rbInifile->Checked)
    {
      runmode = rmIniFile;
    }
  }
  g_ToolOptions.SetRunMode(runmode);

  g_ToolOptions.Set("main", "rememberstate", ckRememberSettings->Checked);

  g_ToolOptions.Set("baseconv", "showbinary", ckBinary->Checked);

  g_ToolOptions.Set("info", "prefix", ckPrefix->Checked);
  g_ToolOptions.Set("info", "quotes", ckQuotes->Checked);

  g_ToolOptions.Set("loupe", "refresh", udRefresh->Position);

  g_ToolOptions.Set("capture", "autosave", ckAutosave->Checked);
  g_ToolOptions.Set("capture", "showloupe", ckShowLoupeOnGrab->Checked);
}

//---------------------------------------------------------------------------
THotkeyInfo TToolOptionsDialog::LoadHotkeyInfo(const String& sHotKeyName)
{
  THotkeyInfo HotkeyInfo;
  HotkeyInfo.enabled = g_ToolOptions.Get("hotkeys\\" + sHotKeyName, "enabled", false);
  HotkeyInfo.virtkey = g_ToolOptions.Get("hotkeys\\" + sHotKeyName, "keycode", 0);
  HotkeyInfo.modifiers = g_ToolOptions.Get("hotkeys\\" + sHotKeyName, "modifiers", 0);

  return HotkeyInfo;
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::SaveHotkeyInfo(const String& sHotKeyName, const THotkeyInfo& HotkeyInfo)
{
  g_ToolOptions.Set("hotkeys\\" + sHotKeyName, "enabled", HotkeyInfo.enabled);
  g_ToolOptions.Set("hotkeys\\" + sHotKeyName, "keycode", HotkeyInfo.virtkey);
  g_ToolOptions.Set("hotkeys\\" + sHotKeyName, "modifiers", HotkeyInfo.modifiers);
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::InitHotkeyPanels()
{
  hkpDoubleClick->SetKeyInfo(LoadHotkeyInfo("doubleclick"));
  hkpZoomIn->SetKeyInfo(LoadHotkeyInfo("zoomin"));
  hkpZoomOut->SetKeyInfo(LoadHotkeyInfo("zoomout"));
  hkpColorCopy->SetKeyInfo(LoadHotkeyInfo("colorcopy"));
  hkpGrabScreen->SetKeyInfo(LoadHotkeyInfo("capturestart"));
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::SaveHotkeyPanels()
{
  SaveHotkeyInfo("doubleclick", hkpDoubleClick->GetKeyInfo());
  SaveHotkeyInfo("zoomin", hkpZoomIn->GetKeyInfo());
  SaveHotkeyInfo("zoomout", hkpZoomOut->GetKeyInfo());
  SaveHotkeyInfo("colorcopy", hkpColorCopy->GetKeyInfo());
  SaveHotkeyInfo("capturestart", hkpGrabScreen->GetKeyInfo());
  //g_ToolOptions.Save();
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::bnOkClick(TObject *Sender)
{
  SaveOptions();
  SaveHotkeyPanels();
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::bnAutosaveOptionsClick(TObject *Sender)
{
  TAutoSaveDialog *AutoSaveDialog = new TAutoSaveDialog(this);
  if (AutoSaveDialog)
  {
    ::ShowWindow(Handle, SW_HIDE);
    AutoSaveDialog->ShowModal();
    delete AutoSaveDialog;
    ::ShowWindow(Handle, SW_SHOW);
  }

}
//---------------------------------------------------------------------------



void __fastcall TToolOptionsDialog::ckRememberSettingsClick(
      TObject *Sender)
{
    rbRegistry->Enabled = ckRememberSettings->Checked;
    rbInifile->Enabled = ckRememberSettings->Checked;
}

//---------------------------------------------------------------------------

