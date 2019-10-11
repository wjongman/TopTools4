#include <vcl.h>
#pragma hdrstop

#include "HotkeyInfo.h"
/////////////////////////////////////////////////////////////////////////////
//
// class THotkeyInfo
//
/////////////////////////////////////////////////////////////////////////////
THotkeyInfo::THotkeyInfo(WORD vk, TShiftState Shift)
: enabled(true), virtkey(0), modifiers(0)
{
  if (vk != 0)
  {
    if (vk != VK_CONTROL && vk != VK_SHIFT && vk != VK_MENU)
      virtkey = vk;

    if (Shift.Contains(ssCtrl))
      modifiers |= MOD_CONTROL;

    if (Shift.Contains(ssShift))
      modifiers |= MOD_SHIFT;

    if (Shift.Contains(ssAlt))
      modifiers |= MOD_ALT;
  }
}

//---------------------------------------------------------------------------
String THotkeyInfo::GetHotkeyText()
{
  DWORD vk = virtkey;
  DWORD mod = modifiers;
  String keystr = "";
  String modstr = "";

  if (vk != VK_CONTROL && vk != VK_SHIFT && vk != VK_MENU)
  // Skip modifier keys, we will collect them later in modKeyStr
  {
    DWORD dwScanCode = MapVirtualKey(vk, 0) << 16;
    // Don't distinguish between left and right keys (set bit 25)
    dwScanCode |= (1 << 25);
    // How can we know we have to do with an extended key?
    // We empirically established this mapping:
    if ((vk >= 0x21 && vk <= 0x5D) || // VK_PRIOR .. VK_APPS
    // There is something weird with the next two, they act as if the
    // NumLock status is negated, might be my keyboard driver though...
    //      (vk == 0x6E) ||               // VK_DECIMAL
    //      (vk == 0x6F) ||               // VK_DIVIDE
        (vk == 0x90) ||               // VK_NUMLOCK
        (vk >= 0x92 && vk <= 0x9F) || // VK_OEM_XXX
        (vk >= 0xA2))                 // VK_LCONTROL and above
    {
      // Set extended-key flag for these keys (bit 24)
      dwScanCode |= (1 << 24);
    }
    // Get the localized name of this key
    TCHAR szKey[32] = "";
    ::GetKeyNameText(dwScanCode, szKey, sizeof szKey);
    keystr = String(szKey);// + " - " + String(vk);
  }

  // Get the localized name of the modifier keys
  TCHAR modKeyStr[20];
  if (mod & MOD_CONTROL)
  {
    DWORD scan = ::MapVirtualKey(VK_CONTROL, 0) << 16;
    GetKeyNameText(scan, modKeyStr, sizeof modKeyStr);
    modstr = String(modKeyStr) + " + ";
  }
  if (mod & MOD_SHIFT)
  {
    DWORD scan = ::MapVirtualKey(VK_SHIFT, 0) << 16;
    GetKeyNameText(scan, modKeyStr, sizeof modKeyStr);
    modstr += String(modKeyStr) + " + ";
  }
  if (mod & MOD_ALT)
  {
    DWORD scan = ::MapVirtualKey(VK_MENU, 0) << 16;
    GetKeyNameText(scan, modKeyStr, sizeof modKeyStr);
    modstr += String(modKeyStr) + " + ";
  }
  // return the complete localized name
  return modstr + keystr;
}

/*
//---------------------------------------------------------------------------
void THotkeyInfo::LoadFromRegistry(const String& regkeypath)
{
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(regkeypath, false))
    {
      if (Reg->ValueExists("enabled"))
        enabled = Reg->ReadBool("enabled");
      if (Reg->ValueExists("keycode"))
        virtkey = Reg->ReadInteger("keycode");
      if (Reg->ValueExists("modifiers"))
        modifiers = Reg->ReadInteger("modifiers");

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
void THotkeyInfo::SaveToRegistry(const String& regkeypath)
{
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(regkeypath, true))
    {
      Reg->WriteBool("enabled", enabled && virtkey != 0);
      Reg->WriteInteger("keycode", virtkey);
      Reg->WriteInteger("modifiers", modifiers);
      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}
*/
//---------------------------------------------------------------------------

