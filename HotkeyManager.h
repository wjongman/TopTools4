#ifndef HotkeyManagerH
#define HotkeyManagerH

#include "HotkeyInfo.h"

enum THotkeyId
{
  hkDoubleclick = 1,
  hkColorCopy = 2,
  hkRulerToggle = 4,
  hkCaptureStart = 8,
  hkZoomIn = 16,
  hkZoomOut = 32
};

//---------------------------------------------------------------------------
class THotkeyManager
{
public:
  THotkeyManager(HWND listener);
  ~THotkeyManager();

  void LoadHotkeys();
  void AssignHotkeys();
  void SaveHotkeys();
  void EnableHotkeys();
  void DisableHotkeys();

private:
  void InitHotkeys();

  THotkeyInfo LoadHotkeyInfo(const String& sHotKeyName);
  void SaveHotkeyInfo(const String& sHotKeyName, const THotkeyInfo& HotkeyInfo);

  // Handle of window that is to receive hotkey events
  HWND m_hwndListener;

  THotkey* m_CopyColorHotkey;
  THotkey* m_ShowHideHotkey;
  THotkey* m_GrabScreenHotkey;
  THotkey* m_RulerToggleHotkey;
  THotkey* m_ZoomInHotkey;
  THotkey* m_ZoomOutHotkey;
};

//---------------------------------------------------------------------------
#endif
