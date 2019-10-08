//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#pragma package(smart_init)
#pragma link "HotkeyPanel"
#pragma resource "*.dfm"

#include "About.h"
#include "OptionDlg.h"
#include "AutoSaveDlg.h"
#include "PresetManager.h"
#include "PersistOptions.h"
#include "HotkeyManager.h"
#include "CustomCopyDlg.h"

// TODO: Load from resource
const char* szHomepageUrl = "https://toptools.org";

//---------------------------------------------------------------------------
__fastcall TToolOptionsDialog::TToolOptionsDialog(TComponent* Owner,
                                                  const String& sPageName = "")
: TForm(Owner)
{
    // Set Form dimensions relative to plMarker so we scale properly
    // independent of display settings (ie. "Large fonts")
    ClientHeight = plMarker->Top;
    ClientWidth = plMarker->Left;

    // Reference panel is only for positioning
    plRef->Visible = false;

    if (sPageName == "")
    {
        m_sActivePage = g_ToolOptions.Get("options", "activepage", "General");
    }
    else
    {
        m_sActivePage = sPageName;
    }
}

//---------------------------------------------------------------------------
__fastcall TToolOptionsDialog::~TToolOptionsDialog()
{
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    g_ToolOptions.Set("options", "activepage", m_sActivePage);
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::FormCreate(TObject *Sender)
{
    ActivatePage(m_sActivePage);

    InitListView();

    // Set focus to currently visible item
    if (lvOptionSelector->Items->Count > 0)
    {
        TListItem *item = lvOptionSelector->FindCaption(0, m_sActivePage, false, true, false);
        if (item)
        {
            item->Selected = true;
            item->Focused = true;
        }
    }

    InitHotkeyPanels();
    InitOptions();
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::FormShow(TObject *Sender)
{
    SetForegroundWindow(Handle);
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::lvOptionSelectorChange(TObject *Sender,
                                                           TListItem *Item, TItemChange Change)
{
    if (lvOptionSelector->ItemFocused)
        ActivatePage(lvOptionSelector->ItemFocused->Caption);
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
    else if (sActive == "About")
        ActivePanel = plAbout;

    if (ActivePanel)
    {
        m_sActivePage = sActive;
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

    plAbout->Visible = false;
    plAbout->Left = plRef->Left;
    plAbout->Top = plRef->Top;
}

//---------------------------------------------------------------------------
void TToolOptionsDialog::InitListView()
{
    int w = lvOptionSelector->ClientWidth;
    lvOptionSelector->ViewStyle = vsReport;
    lvOptionSelector->RowSelect = true;

    TListColumn* pColumn = lvOptionSelector->Columns->Add();
    pColumn->Width = w; //lvOptionSelector->ClientWidth;

    lvOptionSelector->Items->Clear();
    TListItem *pItem;

    pItem = lvOptionSelector->Items->Add();
    pItem->Caption = "General";

    pItem = lvOptionSelector->Items->Add();
    pItem->Caption = "Ruler";

    pItem = lvOptionSelector->Items->Add();
    pItem->Caption = "Loupe";

    pItem = lvOptionSelector->Items->Add();
    pItem->Caption = "Info";

    pItem = lvOptionSelector->Items->Add();
    pItem->Caption = "Base Converter";

    pItem = lvOptionSelector->Items->Add();
    pItem->Caption = "Screen Grabber";

    pItem = lvOptionSelector->Items->Add();
    pItem->Caption = "Tray Icon";

    pItem = lvOptionSelector->Items->Add();
    pItem->Caption = "About";
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

    rbInifile->Hint = g_ToolOptions.GetIniFilePath();
    rbRegistry->Hint = g_ToolOptions.GetRegPath();

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
    ckGrabScreen->Checked = doubleclickaction & dcoGrab;
    //ckOpenInfo->Checked = doubleclickaction & dcoInfo;
    //ckOpenBaseconv->Checked = doubleclickaction & dcoBaseconv;

    // Ruler
    udLength->Position = (short) g_ToolOptions.Get("ruler", "length", 1024);
    ckNudgeRuler->Checked = g_ToolOptions.Get("ruler", "arrownudge", true);
    udTransparency->Position = (short) g_ToolOptions.Get("ruler", "transparency", 50);
    cbTransparent->Checked = g_ToolOptions.Get("ruler", "transparent", false);

    lbTransparency->Enabled = cbTransparent->Checked;
    edTransparency->Enabled = cbTransparent->Checked;
    udTransparency->Enabled = cbTransparent->Checked;
    lbPercent->Enabled = cbTransparent->Checked;

    // Base converter
    ckBinary->Checked = g_ToolOptions.Get("baseconv", "showbinary", true);

    // Loupe
    udRefresh->Position = (short) g_ToolOptions.Get("loupe", "refresh", 250);

    // Info
    ckPrefix->Checked = g_ToolOptions.Get("info", "prefix", false);
    ckQuotes->Checked = g_ToolOptions.Get("info", "quotes", false);
    edTemplate->Text = g_ToolOptions.Get("info", "mask", "");

    bool custom = g_ToolOptions.Get("info", "custom", false);
    rbCustomCopy->Checked = custom;
    edTemplate->Enabled = custom;
    bnEditTemplate->Enabled = custom;
    rbStandard->Checked = !custom;
    ckPrefix->Enabled = !custom;
    ckQuotes->Enabled = !custom;

    // About
    TPNGObject* PngImage = new TPNGObject();
    PngImage->LoadFromResourceName((int)HInstance, "PNG_MAINICON");
    Logo->Picture->Assign(PngImage);
    delete PngImage;

    lbVersion->Caption = "Version: " + GetVersionString() + "  (" + g_sBuildDate + ")";
    lbCopy->Caption = "© 1998-2019 Willem Jongman";
    lbUrl->Hint = szHomepageUrl;
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
                          //ckOpenInfo->Checked * dcoInfo +
                          //ckOpenBaseconv->Checked * dcoBaseconv +
                          ckGrabScreen->Checked * dcoGrab;

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
            // Delete all traces from registry
            g_ToolOptions.ClearRegistry();
        }
    }
    g_ToolOptions.SetRunMode(runmode);

    g_ToolOptions.Set("main", "rememberstate", ckRememberSettings->Checked);

    g_ToolOptions.Set("baseconv", "showbinary", ckBinary->Checked);

    g_ToolOptions.Set("info", "prefix", ckPrefix->Checked);
    g_ToolOptions.Set("info", "quotes", ckQuotes->Checked);
    g_ToolOptions.Set("info", "custom", rbCustomCopy->Checked);
    g_ToolOptions.Set("info", "mask", edTemplate->Text);

    g_ToolOptions.Set("loupe", "refresh", udRefresh->Position);
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
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::bnOkClick(TObject *Sender)
{
    SaveOptions();
    SaveHotkeyPanels();
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::bnManagePresetsClick(TObject *Sender)
{
    TPresetManager* pm = new TPresetManager(this);
    if (pm)
    {
        Hide();
        pm->ShowModal();
        delete pm;
        Show();
    }
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::bnAutosaveOptionsClick(TObject *Sender)
{
    TAutoSaveDialog *AutoSaveDialog = new TAutoSaveDialog(this);
    if (AutoSaveDialog)
    {
        Hide();
        AutoSaveDialog->ShowModal();
        delete AutoSaveDialog;
        Show();
    }
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::ckRememberSettingsClick(TObject *Sender)
{
    rbRegistry->Enabled = ckRememberSettings->Checked;
    rbInifile->Enabled = ckRememberSettings->Checked;
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::cbTransparentClick(TObject *Sender)
{
    lbTransparency->Enabled = cbTransparent->Checked;
    edTransparency->Enabled = cbTransparent->Checked;
    udTransparency->Enabled = cbTransparent->Checked;
    lbPercent->Enabled = cbTransparent->Checked;
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::bnAboutClick(TObject *Sender)
{
    TAboutBox *AboutBox = new TAboutBox(this);
    if (AboutBox)
    {
        Hide();
        AboutBox->ShowModal();
        Show();
        delete AboutBox;
    }
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::rbCustomCopyClick(TObject *Sender)
{
    bool custom = (Sender == rbCustomCopy);

    edTemplate->Enabled = custom;
    bnEditTemplate->Enabled = custom;
    ckPrefix->Enabled = !custom;
    ckQuotes->Enabled = !custom;
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::bnEditTemplateClick(TObject *Sender)
{
    g_ToolOptions.Set("info", "mask", edTemplate->Text);
    TCustomCopyDlg* dlg = new TCustomCopyDlg(this);
    if (dlg)
    {
        Hide();
        dlg->ShowModal();
        Show();
        delete dlg;
    }
    edTemplate->Text = g_ToolOptions.Get("info", "mask", "");
}

//---------------------------------------------------------------------------
void __fastcall TToolOptionsDialog::lbUrlClick(TObject *Sender)
{
    ::ShellExecute(0, "open", szHomepageUrl, NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
String __fastcall TToolOptionsDialog::GetVersionString(void)
{
    String result = "";
    DWORD handle;
    DWORD size = ::GetFileVersionInfoSize(Application->ExeName.c_str(), &handle);
    if (size != 0)
    {
        void *buf = malloc(size);
        if (buf)
        {
            ::GetFileVersionInfo(Application->ExeName.c_str(), 0, size, buf);
            VS_FIXEDFILEINFO *ffip;
            unsigned ffisize;
            ::VerQueryValue( buf, "\\", (void**)&ffip, &ffisize);
            int Ver1 = ffip->dwFileVersionMS >> 16;
            int Ver2 = ffip->dwFileVersionMS & 0xFFFF;
            int Ver3 = ffip->dwFileVersionLS >> 16;
            int Ver4 = ffip->dwFileVersionLS & 0xFFFF;
            result = String(Ver1) + "." + String(Ver2) + "." + String(Ver3) + "." + String(Ver4);
            free(buf);
        }
    }
    return result;
}


