//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

#include <Registry.hpp>
#include <map>

#include "ToolOptions.h"
#include "PersistOptions.h"

/////////////////////////////////////////////////////////////////////////////
void __fastcall TRulerOptions::Init()
{
  m_ToolName = "ruler";

  Horizontal = true;
  ArrowNudge = true;
  Transparent = false;
  Transparency = 50;
  Length = Screen->Width;

  Load();
}

/////////////////////////////////////////////////////////////////////////////
/*
void __fastcall TRulerOptions::Load()
{
  TToolSettings settings("ruler");

  Horizontal = settings.ReadBool("horizontal", Horizontal);
  Length = settings.ReadInteger("length", Length);
  ArrowNudge = settings.ReadBool("arrownudge", ArrowNudge);
  Transparent = settings.ReadBool("transparent", Transparent);
  Transparency = settings.ReadInteger("transparency", Transparency);
}
*/

void __fastcall TRulerOptions::Load()
{
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  Reg->Access = KEY_READ;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, false))
    {
      if (Reg->ValueExists("horizontal"))
        Horizontal = Reg->ReadInteger("horizontal");
      if (Reg->ValueExists("length"))
        Length = Reg->ReadInteger("length");
      if (Reg->ValueExists("arrownudge"))
        ArrowNudge = Reg->ReadBool("arrownudge");
      if (Reg->ValueExists("transparent"))
        Transparent = Reg->ReadBool("transparent");
      if (Reg->ValueExists("transparency"))
        Transparency = Reg->ReadInteger("transparency");

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
/*
void __fastcall TRulerOptions::Save()
{
  TToolSettings settings("ruler");

  settings.WriteBool("horizontal", Horizontal);
  settings.WriteInteger("length", Length);
  settings.WriteInteger("arrownudge", ArrowNudge);
  settings.WriteBool("transparent", Transparent);
  settings.WriteInteger("transparency", Transparency);
}
*/
//---------------------------------------------------------------------------
void __fastcall TRulerOptions::Save()
{
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, true))
    {
      Reg->WriteInteger("horizontal", Horizontal);
      Reg->WriteInteger("length", Length);
      Reg->WriteInteger("arrownudge", ArrowNudge);
      Reg->WriteBool("transparent", Transparent);
      Reg->WriteInteger("transparency", Transparency);

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

/////////////////////////////////////////////////////////////////////////////
void __fastcall TLoupeOptions::Init()
{
  m_ToolName = "loupe";

  RefreshRate = 250;
  Zoom = 4;
  CenterboxVisible = false;
  CrosshairVisible = false;
  GridVisible = false;
  MagnifySelf = false;
  Width = 200;
  Height = 200;

  Load();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeOptions::Load()
{
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  Reg->Access = KEY_READ;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, false))
    {
      if (Reg->ValueExists("centerbox"))
        CenterboxVisible = Reg->ReadInteger("centerbox");
      if (Reg->ValueExists("crosshair"))
        CrosshairVisible = Reg->ReadInteger("crosshair");
      if (Reg->ValueExists("grid"))
        GridVisible = Reg->ReadInteger("grid");
      if (Reg->ValueExists("height"))
        Height = Reg->ReadInteger("height");
      if (Reg->ValueExists("refresh"))
        RefreshRate = Reg->ReadInteger("refresh");
      if (Reg->ValueExists("selfmagnify"))
        MagnifySelf = Reg->ReadInteger("selfmagnify");
      if (Reg->ValueExists("width"))
        Width = Reg->ReadInteger("width");
      if (Reg->ValueExists("zoom"))
        Zoom = Reg->ReadInteger("zoom");

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
void __fastcall TLoupeOptions::Save()
{
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, true))
    {
      Reg->WriteInteger("centerbox", CenterboxVisible);
      Reg->WriteInteger("crosshair", CrosshairVisible);
      Reg->WriteInteger("grid", GridVisible);
      Reg->WriteInteger("height", Height);
      Reg->WriteInteger("refresh", RefreshRate);
      Reg->WriteInteger("selfmagnify", MagnifySelf);
      Reg->WriteInteger("width", Width);
      Reg->WriteInteger("zoom", Zoom);

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

/////////////////////////////////////////////////////////////////////////////
void __fastcall TMainOptions::Init()
{
  m_ToolName = "main";

  DoubleClick = dcoControl;
  SavedState = dcoControl;
  RememberState = true;
  StayOnTop = true;
  AutoStart = false;
  IsTrayApp = false;
  IsSingleton = false;
  Load();
}

//---------------------------------------------------------------------------
void __fastcall TMainOptions::Load()
{
  AutoStart = GetAutoRun();

  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  Reg->Access = KEY_READ;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, false))
    {
      if (Reg->ValueExists("doubleclick"))
        DoubleClick = Reg->ReadInteger("doubleclick");
      if (Reg->ValueExists("rememberstate"))
        RememberState = Reg->ReadBool("rememberstate");
      if (Reg->ValueExists("savedstate"))
        SavedState =Reg->ReadInteger("savedstate");
      if (Reg->ValueExists("stayontop"))
        StayOnTop = Reg->ReadBool("stayontop");
      if (Reg->ValueExists("istrayapp"))
        IsTrayApp = Reg->ReadBool("istrayapp");
      if (Reg->ValueExists("singleton"))
        IsSingleton = Reg->ReadBool("singleton");

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
void __fastcall TMainOptions::Save()
{
  SetAutoRun();

  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, true))
    {
      Reg->WriteInteger("doubleclick", DoubleClick);
      Reg->WriteBool("rememberstate", RememberState);
      Reg->WriteInteger("savedstate", SavedState);
      Reg->WriteBool("stayontop", StayOnTop);
      Reg->WriteBool("istrayapp", IsTrayApp);
      Reg->WriteBool("singleton", IsSingleton);

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
void __fastcall TMainOptions::SetAutoRun()
{
  if (GetAutoRun() == AutoStart)
    // Nothing to do
    return;

  String KeyName = "TopTools";

  String RunKeyStr = "\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(RunKeyStr, true))
    {
      if (AutoStart)
      {
        // Register for Autostart
        Reg->WriteString(KeyName, "\"" + ParamStr(0) + "\"");
      }
      else
      {
        // Unregister for Autostart
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
bool __fastcall TMainOptions::GetAutoRun()
{
  String KeyName = "TopTools";

  // See if our program is registered for auto-start
  bool keyexists = false;
  String RunKeyStr = "\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(RunKeyStr, false))
    {
      if (Reg->ValueExists(KeyName))
      {
        keyexists = true;
      }
      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
  return keyexists;
}

/////////////////////////////////////////////////////////////////////////////
void __fastcall TBaseconvOptions::Init()
{
  m_ToolName = "baseconv";

  ShowBinary = true;

  Load();
}

//---------------------------------------------------------------------------
void __fastcall TBaseconvOptions::Load()
{
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  Reg->Access = KEY_READ;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, false))
    {
      if (Reg->ValueExists("showbinary"))
        ShowBinary = Reg->ReadInteger("showbinary");

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
void __fastcall TBaseconvOptions::Save()
{
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, true))
    {
      Reg->WriteInteger("showbinary", ShowBinary);

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

/////////////////////////////////////////////////////////////////////////////
void __fastcall TColorCopyOptions::Init()
{
  m_ToolName = "colorcopy";

  Prefix = false;
  Quotes = false;

  Load();
}

//---------------------------------------------------------------------------
void __fastcall TColorCopyOptions::Load()
{
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  Reg->Access = KEY_READ;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, false))
    {
      if (Reg->ValueExists("prefix"))
        Prefix = Reg->ReadBool("prefix");
      if (Reg->ValueExists("quotes"))
        Quotes = Reg->ReadBool("quotes");

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
void __fastcall TColorCopyOptions::Save()
{
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, true))
    {
      Reg->WriteBool("prefix", Prefix);
      Reg->WriteBool("quotes", Quotes);

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
String TColorCopyOptions::GetFormatString()
{
  String Format = "";

  if (Quotes)
    Format += "\"";

  if (Prefix)
    Format += "#";

  Format += "%02X%02X%02X";

  if (Quotes)
    Format += "\"";

  return Format;
}

/////////////////////////////////////////////////////////////////////////////
void __fastcall TCaptureOptions::Init()
{
  m_ToolName = "capture";

  AutoSave = false;
  ShowLoupe = false;
  FilterIndex = 1;
  LastDir = "";

  Load();
}

//---------------------------------------------------------------------------
void __fastcall TCaptureOptions::Load()
{
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  Reg->Access = KEY_READ;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, false))
    {
      if (Reg->ValueExists("autosave"))
        AutoSave = Reg->ReadInteger("autosave");
      if (Reg->ValueExists("showloupe"))
        ShowLoupe = Reg->ReadInteger("showloupe");
      if (Reg->ValueExists("filterindex"))
        FilterIndex = Reg->ReadInteger("filterindex");
      if (Reg->ValueExists("lastdir"))
        LastDir = Reg->ReadString("lastdir");

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
void __fastcall TCaptureOptions::Save()
{
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, true))
    {
      Reg->WriteInteger("autosave", AutoSave);
      Reg->WriteBool("showloupe", ShowLoupe);
      Reg->WriteString("lastdir", LastDir);
      Reg->WriteInteger("filterindex", FilterIndex);

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

/////////////////////////////////////////////////////////////////////////////
TTopToolOptions::TTopToolOptions()
{
}

//---------------------------------------------------------------------------
void __fastcall TTopToolOptions::LoadFromRegistry()
{
  MainOptions.Load();
  CaptureOptions.Load();
  RulerOptions.Load();
  BaseconvOptions.Load();
  LoupeOptions.Load();
  ColorCopyOptions.Load();
}

//---------------------------------------------------------------------------
void __fastcall TTopToolOptions::SaveToRegistry()
{
  MainOptions.Save();
  CaptureOptions.Save();
  RulerOptions.Save();
  BaseconvOptions.Save();
  LoupeOptions.Save();
  ColorCopyOptions.Save();
}

//---------------------------------------------------------------------------

