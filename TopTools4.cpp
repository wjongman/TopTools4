//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <Registry.hpp>

// Global String for registry access
//extern String g_RegBaseKey;

// Global flag to hold runmode
//#include "ToolOptions.h"
//extern TRunMode g_RunMode;
#include "PersistOptions.h"

//---------------------------------------------------------------------------
USE("doc\ChangeLog.txt", File);
USE("doc\ToDo.txt", File);
USEFORM("About.cpp", AboutBox);
USEFORM("BaseConv.cpp", BaseConvForm);
USEFORM("Control.cpp", ControlForm);
USEFORM("Float.cpp", FloatForm);
USEFORM("Info.cpp", InfoForm);
USEFORM("Loupe.cpp", LoupeForm);
USEFORM("Main.cpp", MainForm);
USEFORM("Ruler.cpp", RulerForm);
USEFORM("Tool.cpp", ToolForm);
USELIB("gif\gif.lib");
USELIB("png\png.lib");
USERC("Graphics.rc");
USERES("Cursors.res");
USERES("TopTools4.res");
USEUNIT("Grabber.cpp");
USEUNIT("LoupePanel.cpp");
USEUNIT("Monitor.cpp");
USEUNIT("MouseTracker.cpp");
USEUNIT("TopToolBar.cpp");
USEUNIT("TrayIcon.cpp");
USEUNIT("hotkey\HotkeyInfo.cpp");
USEFORM("OptionDlg.cpp", ToolOptionsDialog);
USEUNIT("hotkey\HotkeyManager.cpp");
USEFORM("AutoSaveDlg.cpp", AutoSaveDialog);
USEUNIT("AutoSaveOptions.cpp");
USEFORM("ImageView.cpp", ImageViewer);
USEFORM("ScreenForm.cpp", ScreenForm);
USEUNIT("GrabbedImage.cpp");
USEUNIT("PersistOptions.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  bool IsSingleton = false;
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  Reg->Access = KEY_READ;
  try
  {
    // See if the user wants to run us in portable mode
    if (ParamCount() > 1 && ParamStr(1) == "-p")
    {
        g_RunMode = rmPortable;
    }
    else if (ParamCount() > 1 && ParamStr(1) == "-i")
    {
        g_RunMode = rmIniFile;
    }
    else
    {
        g_RunMode = rmRegistry;
    }

    if (Reg->OpenKey(g_RegBaseKey + "main", false))
    {
      if (Reg->ValueExists("singleton"))
        IsSingleton = Reg->ReadBool("singleton");

      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }

  // This mutex provides the means to determine if a TopTools instance is
  // already running.
  // It is used by the "allow only a single instance per desktop" option
  // It is also used by the installer to prevent the user from installing
  // new versions when the application is still running, and to prevent
  // uninstalling a running application.
  ::CreateMutex(NULL, FALSE, "TopTools_4_Mutex");

  if (IsSingleton)
  {
    // If an instance of this program is already running an error will
    // have occurred by now and GetLastError() will reveal that fact
    if (ERROR_ALREADY_EXISTS == GetLastError())
    {
      // We are not alone, find the other instance

      // First change the title so we don't find ourselves
      Application->Title = "Second Instance";
      HWND FirstWnd = FindWindow("TApplication", "TopTools 4");

      // If the other instance is minimized, we first
      // restore it to it's default dimensions
      if (IsIconic(FirstWnd))
        ShowWindow(FirstWnd, SW_SHOWDEFAULT);

      // Then bring the window to top
      SetForegroundWindow(FirstWnd);

      // And silently exit
      return 0;
    }
  }

  try
  {
     Application->Initialize();
     Application->Title = "TopTools";
     Application->CreateForm(__classid(TMainForm), &MainForm);
         Application->ShowMainForm = false;
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }

  return 0;
}
//---------------------------------------------------------------------------
