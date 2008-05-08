//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#pragma package(smart_init)
#pragma link "HotkeyPanel"
#pragma resource "*.dfm"

//TToolOptionsDialog *ToolOptionsDialog;

#include "OptionDlg.h"
#include "AutoSaveDlg.h"

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

////---------------------------------------------------------------------------
//void TToolOptionsDialog::InitOptions()
//{
//  // General
//  TopTools::Options mainoptions(g_RegBaseKey, "main");
//
//  ckOnTop->Checked = mainoptions.Get("stayontop", true);
//  ckAutoStart->Checked = mainoptions.Get("autostart", false);
//  ckSaveToolstate->Checked = mainoptions.Get("rememberstate", true);
//  ckSingleton->Checked = mainoptions.Get("singleton", false);
//  ckTrayApp->Checked = mainoptions.Get("istrayapp", false);
//  // Tray icon doubleclick action
//  int doubleclickaction = mainoptions.Get("doubleclick", dcoControl);
//  ckOpenToolbar->Checked = doubleclickaction & dcoControl;
//  ckOpenRuler->Checked = doubleclickaction & dcoRuler;
//  ckOpenLoupe->Checked = doubleclickaction & dcoLoupe;
//  ckOpenInfo->Checked = doubleclickaction & dcoInfo;
//  ckOpenBaseconv->Checked = doubleclickaction & dcoBaseconv;
//
////  mainoptions.Set("savedstate", dcoControl);
////  mainoptions.Load();
//
//  // Ruler
//  TopTools::Options ruleroptions(g_RegBaseKey, "ruler");
//  udLength->Position = (short) ruleroptions.Get("length", Screen->Width);
//  ckNudgeRuler->Checked = ruleroptions.Get("arrownudge", true);
//  cbTransparent->Checked = ruleroptions.Get("transparent", false);
//  udTransparency->Position = ruleroptions.Get("transparency", 50);
////  ruleroptions.Set("horizontal", true);
////  ruleroptions.Load();
//
//  // Base converter
//  TopTools::Options baseconvoptions(g_RegBaseKey, "baseconv");
//  ckBinary->Checked = baseconvoptions.Get("showbinary", true);
////  baseconvoptions.Load();
//
//  // Loupe
//  TopTools::Options loupeoptions(g_RegBaseKey, "loupe");
//  udRefresh->Position = (short) loupeoptions.Get("refreshrate", 250);
////  loupeoptions.Set("centerbox", false);
////  loupeoptions.Set("crosshair", false);
////  loupeoptions.Set("grid", false);
////  loupeoptions.Set("height", 200);
////  loupeoptions.Set("selfmagnify", false);
////  loupeoptions.Set("width", 200);
////  loupeoptions.Set("zoom", 4);
////  loupeoptions.Load();
//
//  // Info
//  TopTools::Options infooptions(g_RegBaseKey, "info");
//  ckPrefix->Checked = infooptions.Get("prefix", false);
//  ckQuotes->Checked = infooptions.Get("quotes", false);
//
//
//  // Grabber
//  TopTools::Options captureoptions(g_RegBaseKey, "capture");
//
//  ckAutosave->Checked = captureoptions.Get("autosave", false);
//  ckShowLoupeOnGrab->Checked = captureoptions.Get("showloupe", false);
//
//}

//---------------------------------------------------------------------------
void TToolOptionsDialog::InitOptions()
{
  TTopToolOptions options;
  options.LoadFromRegistry();

  // General
  ckOnTop->Checked = options.MainOptions.StayOnTop;
  ckAutoStart->Checked = options.MainOptions.AutoStart;
  ckSaveToolstate->Checked = options.MainOptions.RememberState;
  ckSingleton->Checked = options.MainOptions.IsSingleton;
  ckTrayApp->Checked = options.MainOptions.IsTrayApp;

  // Ruler
  udLength->Position = (short) options.RulerOptions.Length;
  ckNudgeRuler->Checked = options.RulerOptions.ArrowNudge;
  cbTransparent->Checked = options.RulerOptions.Transparent;
  udTransparency->Position = (short) options.RulerOptions.Transparency;

  // Base converter
  ckBinary->Checked = options.BaseconvOptions.ShowBinary;

  // Loupe
  udRefresh->Position = (short) options.LoupeOptions.RefreshRate;

  // Info
  ckPrefix->Checked = options.ColorCopyOptions.Prefix;
  ckQuotes->Checked = options.ColorCopyOptions.Quotes;

  // Grabber
  ckAutosave->Checked = options.CaptureOptions.AutoSave;
  ckShowLoupeOnGrab->Checked = options.CaptureOptions.ShowLoupe;

  // Tray icon
  ckOpenToolbar->Checked = options.MainOptions.DoubleClick & dcoControl;
  ckOpenRuler->Checked = options.MainOptions.DoubleClick & dcoRuler;
  ckOpenLoupe->Checked = options.MainOptions.DoubleClick & dcoLoupe;
  ckOpenInfo->Checked = options.MainOptions.DoubleClick & dcoInfo;
  ckOpenBaseconv->Checked = options.MainOptions.DoubleClick & dcoBaseconv;
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::SaveOptions()
{
  TTopToolOptions options;

  options.RulerOptions.Length = udLength->Position;
  options.RulerOptions.ArrowNudge = ckNudgeRuler->Checked;
  options.RulerOptions.Transparent = cbTransparent->Checked;
  options.RulerOptions.Transparency = udTransparency->Position;

  options.MainOptions.IsSingleton = ckSingleton->Checked;
  options.MainOptions.IsTrayApp = ckTrayApp->Checked;
  options.MainOptions.StayOnTop = ckOnTop->Checked;
  options.MainOptions.AutoStart = ckAutoStart->Checked;
  options.MainOptions.RememberState = ckSaveToolstate->Checked;
  options.MainOptions.DoubleClick = ckOpenToolbar->Checked * dcoControl +
                             ckOpenRuler->Checked * dcoRuler +
                             ckOpenLoupe->Checked * dcoLoupe +
                             ckOpenInfo->Checked * dcoInfo +
                             ckOpenBaseconv->Checked * dcoBaseconv;

  options.BaseconvOptions.ShowBinary = ckBinary->Checked;

  options.ColorCopyOptions.Prefix = ckPrefix->Checked;
  options.ColorCopyOptions.Quotes = ckQuotes->Checked;

  options.LoupeOptions.RefreshRate = udRefresh->Position;

  options.CaptureOptions.AutoSave = ckAutosave->Checked;
  options.CaptureOptions.ShowLoupe = ckShowLoupeOnGrab->Checked;

  options.SaveToRegistry();
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::InitOptions0()
{
  TTopToolOptions options;
  options.LoadFromRegistry();

  // General
  ckOnTop->Checked = options.MainOptions.StayOnTop;
  ckAutoStart->Checked = options.MainOptions.AutoStart;
  ckSaveToolstate->Checked = options.MainOptions.RememberState;
  ckSingleton->Checked = options.MainOptions.IsSingleton;
  ckTrayApp->Checked = options.MainOptions.IsTrayApp;
  // Ruler
  udLength->Position = (short) options.RulerOptions.Length;
  ckNudgeRuler->Checked = options.RulerOptions.ArrowNudge;
  cbTransparent->Checked = options.RulerOptions.Transparent;
  udTransparency->Position = (short) options.RulerOptions.Transparency;
  // Base converter
  ckBinary->Checked = options.BaseconvOptions.ShowBinary;
  // Loupe
  udRefresh->Position = (short) options.LoupeOptions.RefreshRate;
  // Info
  ckPrefix->Checked = options.ColorCopyOptions.Prefix;
  ckQuotes->Checked = options.ColorCopyOptions.Quotes;
  // Grabber
  ckAutosave->Checked = options.CaptureOptions.AutoSave;
  ckShowLoupeOnGrab->Checked = options.CaptureOptions.ShowLoupe;
  // Tray icon
  ckOpenToolbar->Checked = options.MainOptions.DoubleClick & dcoControl;
  ckOpenRuler->Checked = options.MainOptions.DoubleClick & dcoRuler;
  ckOpenLoupe->Checked = options.MainOptions.DoubleClick & dcoLoupe;
  ckOpenInfo->Checked = options.MainOptions.DoubleClick & dcoInfo;
  ckOpenBaseconv->Checked = options.MainOptions.DoubleClick & dcoBaseconv;
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::SaveOptions0()
{
  TTopToolOptions options;

  options.RulerOptions.Length = udLength->Position;
  options.RulerOptions.ArrowNudge = ckNudgeRuler->Checked;
  options.RulerOptions.Transparent = cbTransparent->Checked;
  options.RulerOptions.Transparency = udTransparency->Position;

  options.MainOptions.IsSingleton = ckSingleton->Checked;
  options.MainOptions.IsTrayApp = ckTrayApp->Checked;
  options.MainOptions.StayOnTop = ckOnTop->Checked;
  options.MainOptions.AutoStart = ckAutoStart->Checked;
  options.MainOptions.RememberState = ckSaveToolstate->Checked;
  options.MainOptions.DoubleClick = ckOpenToolbar->Checked * dcoControl +
                             ckOpenRuler->Checked * dcoRuler +
                             ckOpenLoupe->Checked * dcoLoupe +
                             ckOpenInfo->Checked * dcoInfo +
                             ckOpenBaseconv->Checked * dcoBaseconv;

  options.BaseconvOptions.ShowBinary = ckBinary->Checked;

  options.ColorCopyOptions.Prefix = ckPrefix->Checked;
  options.ColorCopyOptions.Quotes = ckQuotes->Checked;

  options.LoupeOptions.RefreshRate = udRefresh->Position;

  options.CaptureOptions.AutoSave = ckAutosave->Checked;
  options.CaptureOptions.ShowLoupe = ckShowLoupeOnGrab->Checked;

  options.SaveToRegistry();
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::InitHotkeyPanels()
{
  String RegBase = g_RegBaseKey + "hotkeys\\";

  hkpDoubleClick->Load(RegBase + "doubleclick");
  hkpZoomIn->Load(RegBase + "zoomin");
  hkpZoomOut->Load(RegBase + "zoomout");
  hkpColorCopy->Load(RegBase + "colorcopy");
  hkpGrabScreen->Load(RegBase + "capturestart");
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::SaveHotkeyPanels()
{
  hkpDoubleClick->Save();
  hkpZoomIn->Save();
  hkpZoomOut->Save();
  hkpColorCopy->Save();
  hkpGrabScreen->Save();
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


