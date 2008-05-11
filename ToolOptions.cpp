//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

#include <Registry.hpp>
#include <map>

#include "ToolOptions.h"
//#include "ToolSettings.h"

//---------------------------------------------------------------------------
#if 1
TPersistToolOptions::TPersistToolOptions()
{
  Set("capture\\autosave", "bypassmenu", false);
  Set("capture\\autosave", "continuous", false);
  Set("capture\\autosave", "digits", 2);
//  Set("capture\\autosave", "directory", GetSpecialFolderPath(CSIDL_DESKTOPDIRECTORY));
  Set("capture\\autosave", "existaction", 0);
  Set("capture\\autosave", "filename", "Snapshot");
  Set("capture\\autosave", "imagetype", 0);
  Set("capture\\autosave", "nextvalue", 1);

//   TopTools::Options mainoptions(g_RegBaseKey, "main");
  Set("main", "autostart", false);
  Set("main", "doubleclick", dcoControl);
  Set("main", "istrayapp", false);
  Set("main", "rememberstate", true);
  Set("main", "savedstate", dcoControl);
  Set("main", "singleton", false);
  Set("main", "stayontop", true);
//   mainoptions.Load();
//
//   // Ruler
//   TopTools::Options ruleroptions(g_RegBaseKey, "ruler");
  Set("ruler", "arrownudge", true);
  Set("ruler", "horizontal", true);
  Set("ruler", "length", Screen->Width);
  Set("ruler", "transparency", 50);
  Set("ruler", "transparent", false);
//   ruleroptions.Load();
//
//   // Base converter
//   TopTools::Options baseconvoptions(g_RegBaseKey, "baseconv");
  Set("baseconv", "showbinary", true);
//   baseconvoptions.Load();
//
//   // Loupe
//   TopTools::Options loupeoptions(g_RegBaseKey, "loupe");
  Set("loupe", "centerbox", false);
  Set("loupe", "crosshair", false);
  Set("loupe", "grid", false);
  Set("loupe", "height", 200);
  Set("loupe", "refresh", 250);
  Set("loupe", "selfmagnify", false);
  Set("loupe", "width", 200);
  Set("loupe", "zoom", 4);

  Set("info", "prefix", false);
  Set("info", "quotes", false);

  Set("capture", "autosave", false);
  Set("capture", "showloupe", false);
/*
  // Init default options
  //m_Options.Init(g_RegBaseKey, "capture\\autosave");

  m_Options.Set("capture\\autosave", "bypassmenu", false);
  m_Options.Set("capture\\autosave", "continuous", false);
  m_Options.Set("capture\\autosave", "digits", 2);
  m_Options.Set("capture\\autosave", "directory", GetSpecialFolderPath(CSIDL_DESKTOPDIRECTORY));
  m_Options.Set("capture\\autosave", "existaction", 0);
  m_Options.Set("capture\\autosave", "filename", "Snapshot");
  m_Options.Set("capture\\autosave", "imagetype", 0);
  m_Options.Set("capture\\autosave", "nextvalue", 1);

//   TopTools::Options mainoptions(g_RegBaseKey, "main");
  m_Options.Set("main", "autostart", false);
  m_Options.Set("main", "doubleclick", dcoControl);
  m_Options.Set("main", "istrayapp", false);
  m_Options.Set("main", "rememberstate", true);
  m_Options.Set("main", "savedstate", dcoControl);
  m_Options.Set("main", "singleton", false);
  m_Options.Set("main", "stayontop", true);
//   mainoptions.Load();
//
//   // Ruler
//   TopTools::Options ruleroptions(g_RegBaseKey, "ruler");
  m_Options.Set("ruler", "arrownudge", true);
  m_Options.Set("ruler", "horizontal", true);
  m_Options.Set("ruler", "length", Screen->Width);
  m_Options.Set("ruler", "transparency", 50);
  m_Options.Set("ruler", "transparent", false);
//   ruleroptions.Load();
//
//   // Base converter
//   TopTools::Options baseconvoptions(g_RegBaseKey, "baseconv");
  m_Options.Set("baseconv", "showbinary", true);
//   baseconvoptions.Load();
//
//   // Loupe
//   TopTools::Options loupeoptions(g_RegBaseKey, "loupe");
  m_Options.Set("loupe", "centerbox", false);
  m_Options.Set("loupe", "crosshair", false);
  m_Options.Set("loupe", "grid", false);
  m_Options.Set("loupe", "height", 200);
  m_Options.Set("loupe", "refresh", 250);
  m_Options.Set("loupe", "selfmagnify", false);
  m_Options.Set("loupe", "width", 200);
  m_Options.Set("loupe", "zoom", 4);

  m_Options.Set("info", "prefix", false);
  m_Options.Set("info", "quotes", false);

  m_Options.Set("capture", "autosave", false);
  m_Options.Set("capture", "showloupe", false);
*/
//   // General
//   ckOnTop->Checked = mainoptions.GetBool("stayontop");
//   ckAutoStart->Checked = mainoptions.GetBool("autostart");
//   ckSaveToolstate->Checked = mainoptions.GetBool("rememberstate");
//   ckSingleton->Checked = mainoptions.GetBool("singleton");
//   ckTrayApp->Checked = mainoptions.GetBool("istrayapp");
//
//   // Tray icon doubleclick action
//   int doubleclickaction = mainoptions.GetInt("doubleclick");
//   ckOpenToolbar->Checked = doubleclickaction & dcoControl;
//   ckOpenRuler->Checked = doubleclickaction & dcoRuler;
//   ckOpenLoupe->Checked = doubleclickaction & dcoLoupe;
//   ckOpenInfo->Checked = doubleclickaction & dcoInfo;
//   ckOpenBaseconv->Checked = doubleclickaction & dcoBaseconv;
//
//   // Ruler
//   udLength->Position = (short) ruleroptions.GetInt("length");
//   ckNudgeRuler->Checked = ruleroptions.GetBool("arrownudge");
//   cbTransparent->Checked = ruleroptions.GetBool("transparent");
//   udTransparency->Position = ruleroptions.GetBool("transparency");
//
//   // Base converter
//   ckBinary->Checked = baseconvoptions.GetBool("showbinary");
//
//   // Loupe
//   udRefresh->Position = (short) loupeoptions.GetInt("refreshrate");
//
//   // Info
//   ckPrefix->Checked = infooptions.GetBool("prefix");
//   ckQuotes->Checked = infooptions.GetBool("quotes");
//
//   // Grabber
//   ckAutosave->Checked = captureoptions.GetBool("autosave");
//   ckShowLoupeOnGrab->Checked = captureoptions.GetBool("showloupe");

}

//---------------------------------------------------------------------------
void TPersistToolOptions::Load()
{
  for (option_map_iterator iter = m_OptionMaps.begin(); iter != m_OptionMaps.end(); iter++)
  {
    String ToolName = iter->first;
    (iter->second).Load();
  }
}

//---------------------------------------------------------------------------
void TPersistToolOptions::Save()
{
  for (option_map_iterator iter = m_OptionMaps.begin(); iter != m_OptionMaps.end(); iter++)
  {
    String ToolName = iter->first;
    (iter->second).Save();
  }
}

//---------------------------------------------------------------------------
int TPersistToolOptions::GetInt(const String& ToolName, const String& OptionName)
{
  option_map_iterator iter = m_OptionMaps.find(ToolName);
  if (iter != m_OptionMaps.end())
  {
    TopTools::TOptionMap OptionMap = iter->second;
    return OptionMap.Get(OptionName, 0);
  }

  return 0;
}

//---------------------------------------------------------------------------
String TPersistToolOptions::GetString(const String& ToolName, const String& OptionName)
{
  option_map_iterator iter = m_OptionMaps.find(ToolName);
  if (iter != m_OptionMaps.end())
  {
    TopTools::TOptionMap OptionMap = iter->second;
    return OptionMap.Get(OptionName, "");
  }

  return "";
}

//---------------------------------------------------------------------------
bool TPersistToolOptions::GetBool(const String& ToolName, const String& OptionName)
{
  option_map_iterator iter = m_OptionMaps.find(ToolName);
  if (iter != m_OptionMaps.end())
  {
    TopTools::TOptionMap OptionMap = iter->second;
    return OptionMap.Get(OptionName, false);
  }

  return false;
}

//---------------------------------------------------------------------------
void TPersistToolOptions::Set(const String& ToolName, const String& OptionName, int Option)
{
  option_map_iterator iter = m_OptionMaps.find(ToolName);
  if (iter == m_OptionMaps.end())
  {
    TopTools::TOptionMap OptionMap(ToolName);
    OptionMap.Set(OptionName, Option);
    m_OptionMaps[ToolName] = OptionMap;
  }
  else
  {
    (iter->second).Set(OptionName, Option);
  }
}

//---------------------------------------------------------------------------
void TPersistToolOptions::Set(const String& ToolName, const String& OptionName, String Option)
{
  option_map_iterator iter = m_OptionMaps.find(ToolName);
  if (iter == m_OptionMaps.end())
  {
    TopTools::TOptionMap OptionMap(ToolName);
    OptionMap.Set(OptionName, Option);
    m_OptionMaps[ToolName] = OptionMap;
  }
  else
  {
    (iter->second).Set(OptionName, Option);
  }
}

//---------------------------------------------------------------------------
void TPersistToolOptions::Set(const String& ToolName, const String& OptionName, bool Option)
{
  option_map_iterator iter = m_OptionMaps.find(ToolName);
  if (iter == m_OptionMaps.end())
  {
    TopTools::TOptionMap OptionMap(ToolName);
    OptionMap.Set(OptionName, Option);
    m_OptionMaps[ToolName] = OptionMap;
  }
  else
  {
    (iter->second).Set(OptionName, Option);
  }
}

//---------------------------------------------------------------------------
#endif


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

