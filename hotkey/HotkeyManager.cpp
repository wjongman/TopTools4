//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "HotkeyManager.h"

extern const String g_RegBaseKey;

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
  THotkeyInfo HotkeyInfo;

  HotkeyInfo.LoadFromRegistry(g_RegBaseKey + "hotkeys\\colorcopy");
  m_CopyColorHotkey->Assign(HotkeyInfo);

  HotkeyInfo.LoadFromRegistry(g_RegBaseKey + "hotkeys\\doubleclick");
  m_ShowHideHotkey->Assign(HotkeyInfo);

  HotkeyInfo.LoadFromRegistry(g_RegBaseKey + "hotkeys\\capturestart");
  m_GrabScreenHotkey->Assign(HotkeyInfo);

  HotkeyInfo.LoadFromRegistry(g_RegBaseKey + "hotkeys\\rulertoggle");
  m_RulerToggleHotkey->Assign(HotkeyInfo);

  HotkeyInfo.LoadFromRegistry(g_RegBaseKey + "hotkeys\\zoomin");
  m_ZoomInHotkey->Assign(HotkeyInfo);

  HotkeyInfo.LoadFromRegistry(g_RegBaseKey + "hotkeys\\zoomout");
  m_ZoomOutHotkey->Assign(HotkeyInfo);
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

