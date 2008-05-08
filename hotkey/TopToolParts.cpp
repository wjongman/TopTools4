//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("TopToolParts.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("HotkeyPanel.cpp");
USEUNIT("PathEllipsisLabel.cpp");
USEUNIT("HotkeyInfo.cpp");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
  return 1;
}
//---------------------------------------------------------------------------

