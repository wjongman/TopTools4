//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("OptionTestApp.res");
USEFORM("TestWindow.cpp", Form1);
USEFORM("..\AutoSaveDlg.cpp", AutoSaveDialog);
USEUNIT("..\hotkey\HotkeyInfo.cpp");
USELIB("C:\Program Files\Borland\CBuilder5\Lib\Release\vclx50.lib");
USEUNIT("..\AutoSaveOptions.cpp");
USEFORM("..\OptionDlg.cpp", ToolOptionsDialog);
USEUNIT("..\ToolOptions.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->CreateForm(__classid(TForm1), &Form1);
     Application->CreateForm(__classid(TAutoSaveDialog), &AutoSaveDialog);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------
