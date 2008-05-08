//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Options.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TOptionForm::TOptionForm(TComponent* Owner)
: TForm(Owner)
{
  LoadSettings();

  ckOnTop->Checked = m_Options.OnTop;
  ckStart->Checked = GetAutoRun();
  udLength->Position = (short) m_Options.RulerLength;

  HotkeyEdit->Text = m_Options.Hotkey.GetHotkeyText();
  if (HotkeyEdit->Text == "")
  {
    m_Options.Hotkey.enabled = false;
  }

  ckEnableHK->Checked = m_Options.Hotkey.enabled;
  HotkeyEdit->Enabled = m_Options.Hotkey.enabled;
  lbHotkey->Enabled = m_Options.Hotkey.enabled;

  ckPrefix->Checked = m_Options.Prefix;
  ckQuotes->Checked = m_Options.Quotes;

  ckToolbar->Checked = m_Options.DoubleClickOpen & dcoToolbar;
  ckRuler->Checked = m_Options.DoubleClickOpen & dcoRuler;
  ckLoupe->Checked = m_Options.DoubleClickOpen & dcoLoupe;
  ckInfo->Checked = m_Options.DoubleClickOpen & dcoInfo;
  ckBaseconv->Checked = m_Options.DoubleClickOpen & dcoBaseconv;

  Application->OnShowHint = MakeMultilineHint;
}

//---------------------------------------------------------------------------
void __fastcall TOptionForm::HotkeyEditKeyDown(TObject *Sender, WORD &Key,
                                               TShiftState Shift)
{
  m_Options.Hotkey = THotkeyInfo(Key, Shift);
  HotkeyEdit->Text = m_Options.Hotkey.GetHotkeyText();
  Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TOptionForm::bnOkClick(TObject *Sender)
{
  m_Options.OnTop = ckOnTop->Checked;
  m_Options.StartUp = ckStart->Checked;
  m_Options.RulerLength = udLength->Position;
  m_Options.Hotkey.enabled = ckEnableHK->Checked;
  m_Options.Prefix = ckPrefix->Checked;
  m_Options.Quotes = ckQuotes->Checked;

  m_Options.DoubleClickOpen = ckToolbar->Checked * dcoToolbar +
                              ckRuler->Checked * dcoRuler +
                              ckLoupe->Checked * dcoLoupe +
                              ckInfo->Checked * dcoInfo +
                              ckBaseconv->Checked * dcoBaseconv;

  SetAutoRun(ckStart->Checked);

  SaveSettings();
}

//---------------------------------------------------------------------------
void __fastcall TOptionForm::LoadSettings()
{
  // Set defaults
  m_Options.RulerLength = Screen->Width;
  m_Options.OnTop = true;

  // Retrieve saved settings from the registry
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + "ruler", false))
    {
      if (Reg->ValueExists("length"))
        m_Options.RulerLength = Reg->ReadInteger("length");

      Reg->CloseKey();
    }

    if (Reg->OpenKey(g_RegBaseKey + "main", false))
    {
      if (Reg->ValueExists("stayontop"))
        m_Options.OnTop = Reg->ReadBool("stayontop");
      if (Reg->ValueExists("doubleclick"))
        m_Options.DoubleClickOpen = Reg->ReadInteger("doubleclick");
      else
        m_Options.DoubleClickOpen = dcoToolbar;

      Reg->CloseKey();
    }

    if (Reg->OpenKey(g_RegBaseKey + "colorcopy", false))
    {
      if (Reg->ValueExists("enabled"))
        m_Options.Hotkey.enabled = Reg->ReadBool("enabled");
      if (Reg->ValueExists("keycode"))
        m_Options.Hotkey.virtkey = Reg->ReadInteger("keycode");
      if (Reg->ValueExists("modifiers"))
        m_Options.Hotkey.modifiers = Reg->ReadInteger("modifiers");
      if (Reg->ValueExists("prefix"))
        m_Options.Prefix = Reg->ReadBool("prefix");
      if (Reg->ValueExists("quotes"))
        m_Options.Quotes = Reg->ReadBool("quotes");

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
void __fastcall TOptionForm::SaveSettings()
{
  // Save settings in the registry
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + "ruler", true))
    {
      Reg->WriteInteger("length", m_Options.RulerLength);
      Reg->CloseKey();
    }
    if (Reg->OpenKey(g_RegBaseKey + "main", true))
    {
      Reg->WriteBool("stayontop", m_Options.OnTop);
      Reg->WriteInteger("doubleclick", m_Options.DoubleClickOpen);
      Reg->CloseKey();
    }
    if (Reg->OpenKey(g_RegBaseKey + "colorcopy", true))
    {
      Reg->WriteBool("enabled", m_Options.Hotkey.enabled);
      Reg->WriteInteger("keycode", m_Options.Hotkey.virtkey);
      Reg->WriteInteger("modifiers", m_Options.Hotkey.modifiers);
      Reg->WriteBool("prefix", m_Options.Prefix);
      Reg->WriteBool("quotes", m_Options.Quotes);
      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
void __fastcall TOptionForm::ckEnableHKClick(TObject *Sender)
{
  if (ckEnableHK->Checked)
  {
    HotkeyEdit->Enabled = true;
    lbHotkey->Enabled = true;
    m_Options.Hotkey.enabled = true;
  }
  else
  {
    HotkeyEdit->Enabled = false;
    lbHotkey->Enabled = false;
    m_Options.Hotkey.enabled = false;
  }
}

//---------------------------------------------------------------------------
void __fastcall TOptionForm::ckEnableHKMouseUp(TObject *Sender, TMouseButton Button,
                                               TShiftState Shift, int X, int Y)
{
  if (HotkeyEdit->Enabled)
  {
    // Make editbox ready for input
    HotkeyEdit->SetFocus();
  }
}

//---------------------------------------------------------------------------
bool __fastcall TOptionForm::GetAutoRun()
// See if our program is registered for auto-start
{
  bool exists = false;
  String RunKeyStr = "\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
  String KeyName = "TopTools";
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(RunKeyStr, false))
    {
      if (Reg->ValueExists(KeyName))
      {
        exists = true;
      }
      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
  return exists;
}

//---------------------------------------------------------------------------
void __fastcall TOptionForm::SetAutoRun(bool enable)
// Update the "run" section of the current user's registry section
{
  String RunKeyStr = "\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
  String KeyName = "TopTools";
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(RunKeyStr, true))
    {
      if (enable)
      {
        // Write quoted pathname of our app
        Reg->WriteString(KeyName, "\"" + ParamStr(0) + "\"");
      }
      else
      {
        Reg->DeleteValue(KeyName);
      }
      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
void __fastcall TOptionForm::MakeMultilineHint(AnsiString &HintStr, bool &CanShow,
                                               THintInfo &HintInfo)
{
  HintStr = StringReplace(HintStr, "~", "\n", TReplaceFlags() << rfReplaceAll);
}




