//---------------------------------------------------------------------------
#ifndef HotkeyInfoH
#define HotkeyInfoH

#include <Registry.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// A class to encapsulate a keyboard key combination
//
// Besides a default constructor it has a constructor that
// takes a (VCL compatible) virtual keycode and modifier pair.
//
// The GetHotkeyText function queries the localized text of the key
// combination using the MapVirtualKey and GetKeyNameText API functions.
//
// LoadFromRegistry and SaveToRegistry add persistence
//
/////////////////////////////////////////////////////////////////////////////

class THotkeyInfo
{
public:
  // Default constructor
  THotkeyInfo() : enabled(false),
                  virtkey(0),
                  modifiers(0) {}

  // Construct from Win32 parameters
  THotkeyInfo::THotkeyInfo(WORD vk, WORD modf) : enabled(true),
                                                 virtkey(vk),
                                                 modifiers(modf) {}

  // Construct from VCL types
  THotkeyInfo(WORD vk, TShiftState Shift);

  // Copy constructor
  THotkeyInfo(const THotkeyInfo& rhs) : enabled(rhs.enabled),
                                        virtkey(rhs.virtkey),
                                        modifiers(rhs.modifiers) {}

  THotkeyInfo& operator=(const THotkeyInfo& rhs)
  {
    enabled = rhs.enabled;
    virtkey = rhs.virtkey;
    modifiers = rhs.modifiers;
    return *this;
  }

  void Clear()
  {
    enabled = false;
    virtkey = 0;
    modifiers = 0;
  }

  String GetHotkeyText();

//  void LoadFromRegistry(const String& regkeypath);
//  void SaveToRegistry(const String& regkeypath);
  void Load(const String& hotkeyname);
  void Save(const String& hotkeyname);

  bool enabled;
  int virtkey;
  int modifiers;
};

/////////////////////////////////////////////////////////////////////////////
class THotkey
{
public:
  THotkey(HWND hWnd, int id)
    : m_hWnd(hWnd), m_id(id)
  {
  }

  THotkey(HWND hWnd, int id, THotkeyInfo& keyinfo)
    : m_hWnd(hWnd), m_id(id), m_keyinfo(keyinfo)
  {
    Enable();
  }

  THotkey(HWND hWnd, int id, const String& hotkeyname)
    : m_hWnd(hWnd), m_id(id), m_hotkeyname(hotkeyname)
  {
//    m_keyinfo.LoadFromRegistry(m_hotkeyname);
    m_keyinfo.Load(m_hotkeyname);
    Enable();
  }

  ~THotkey()
  {
    Disable();
  }

  void Assign(THotkeyInfo& keyinfo)
  {
    Disable();
    m_keyinfo = keyinfo;
    Enable();
  }

  void Enable()
  {
    if (m_hWnd && m_keyinfo.enabled && m_keyinfo.virtkey != 0)
    {
      ::RegisterHotKey(m_hWnd, m_id, m_keyinfo.modifiers, m_keyinfo.virtkey);
    }
  }

  void Disable()
  {
    // Redundant calls to UnregisterHotKey() shouldn't do any harm
    if (m_hWnd)
    {
      ::UnregisterHotKey(m_hWnd, m_id);
    }
  }

private:
  HWND m_hWnd;  // Handle of window that will receive WM_HOTKEY messages
  int m_id;     // id of this particular hotkey
  THotkeyInfo m_keyinfo;
  String m_hotkeyname;
};

#endif
