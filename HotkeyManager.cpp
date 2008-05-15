//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "HotkeyManager.h"
#include "PersistOptions.h"

//---------------------------------------------------------------------------
THotkeyManager::THotkeyManager(HWND hwnd)
: m_hwndListener(hwnd),
  m_CopyColorHotkey(NULL),
  m_ShowHideHotkey(NULL),
  m_GrabScreenHotkey(NULL),
  m_RulerToggleHotkey(NULL),
  m_ZoomInHotkey(NULL),
  m_ZoomOutHotkey(NULL)
{
  InitHotkeys();
  AssignHotkeys();
}

//---------------------------------------------------------------------------
THotkeyManager::~THotkeyManager()
{
  delete m_CopyColorHotkey;
  delete m_ShowHideHotkey;
  delete m_GrabScreenHotkey;
  delete m_RulerToggleHotkey;
  delete m_ZoomInHotkey;
  delete m_ZoomOutHotkey;
}

//---------------------------------------------------------------------------
void THotkeyManager::InitHotkeys()
{
  m_CopyColorHotkey = new THotkey(m_hwndListener, hkColorCopy);
  m_ShowHideHotkey = new THotkey(m_hwndListener, hkDoubleclick);
  m_GrabScreenHotkey = new THotkey(m_hwndListener, hkCaptureStart);
  m_RulerToggleHotkey = new THotkey(m_hwndListener, hkRulerToggle);
  m_ZoomInHotkey = new THotkey(m_hwndListener, hkZoomIn);
  m_ZoomOutHotkey = new THotkey(m_hwndListener, hkZoomOut);
}

//---------------------------------------------------------------------------
void THotkeyManager::AssignHotkeys()
{
  m_CopyColorHotkey->Assign(LoadHotkeyInfo("colorcopy"));
  m_ShowHideHotkey->Assign(LoadHotkeyInfo("doubleclick"));
  m_GrabScreenHotkey->Assign(LoadHotkeyInfo("capturestart"));
  m_RulerToggleHotkey->Assign(LoadHotkeyInfo("rulertoggle"));
  m_ZoomInHotkey->Assign(LoadHotkeyInfo("zoomin"));
  m_ZoomOutHotkey->Assign(LoadHotkeyInfo("zoomout"));
}

//---------------------------------------------------------------------------
void THotkeyManager::LoadHotkeys()
{
  m_CopyColorHotkey->Assign(LoadHotkeyInfo("colorcopy"));
  m_ShowHideHotkey->Assign(LoadHotkeyInfo("doubleclick"));
  m_GrabScreenHotkey->Assign(LoadHotkeyInfo("capturestart"));
  m_RulerToggleHotkey->Assign(LoadHotkeyInfo("rulertoggle"));
  m_ZoomInHotkey->Assign(LoadHotkeyInfo("zoomin"));
  m_ZoomOutHotkey->Assign(LoadHotkeyInfo("zoomout"));
}

/*/---------------------------------------------------------------------------
void THotkeyManager::SaveHotkeys()
{
  SaveHotkeyInfo("doubleclick", m_ShowHideHotkey->GetKeyInfo());
  SaveHotkeyInfo("zoomin", m_ZoomInHotkey->GetKeyInfo());
  SaveHotkeyInfo("zoomout", m_ZoomOutHotkey->GetKeyInfo());
  SaveHotkeyInfo("colorcopy", m_CopyColorHotkey->GetKeyInfo());
  SaveHotkeyInfo("capturestart", m_GrabScreenHotkey->GetKeyInfo());
  SaveHotkeyInfo("rulertoggle", m_RulerToggleHotkey->GetKeyInfo());
} */

//---------------------------------------------------------------------------
THotkeyInfo THotkeyManager::LoadHotkeyInfo(const String& sHotKeyName)
{
  THotkeyInfo HotkeyInfo;
  HotkeyInfo.enabled = g_ToolOptions.GetBool("hotkeys\\" + sHotKeyName, "enabled");
  HotkeyInfo.virtkey = g_ToolOptions.GetInt("hotkeys\\" + sHotKeyName, "keycode");
  HotkeyInfo.modifiers = g_ToolOptions.GetInt("hotkeys\\" + sHotKeyName, "modifiers");

  return HotkeyInfo;
}

//---------------------------------------------------------------------------
void THotkeyManager::SaveHotkeyInfo(const String& sHotKeyName, const THotkeyInfo& HotkeyInfo)
{
  g_ToolOptions.Set("hotkeys\\" + sHotKeyName, "enabled", HotkeyInfo.enabled);
  g_ToolOptions.Set("hotkeys\\" + sHotKeyName, "keycode", HotkeyInfo.virtkey);
  g_ToolOptions.Set("hotkeys\\" + sHotKeyName, "modifiers", HotkeyInfo.modifiers);
}

//---------------------------------------------------------------------------
void THotkeyManager::EnableHotkeys()
{
  m_CopyColorHotkey->Enable();
  m_ShowHideHotkey->Enable();
  m_GrabScreenHotkey->Enable();
  m_RulerToggleHotkey->Enable();
  m_ZoomInHotkey->Enable();
  m_ZoomOutHotkey->Enable();
}

//---------------------------------------------------------------------------
void THotkeyManager::DisableHotkeys()
{
  m_CopyColorHotkey->Disable();
  m_ShowHideHotkey->Disable();
  m_GrabScreenHotkey->Disable();
  m_RulerToggleHotkey->Disable();
  m_ZoomInHotkey->Disable();
  m_ZoomOutHotkey->Disable();
}

