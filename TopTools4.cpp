//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PersistOptions.h"

//---------------------------------------------------------------------------
USE("doc\ChangeLog.txt", File);
USE("doc\ToDo.txt", File);
USEFORM("About.cpp", AboutBox);
USEFORM("AutoSaveDlg.cpp", AutoSaveDialog);
USEFORM("BaseConv.cpp", BaseConvForm);
USEFORM("Control.cpp", ControlForm);
USEFORM("Float.cpp", FloatForm);
USEFORM("ImageView.cpp", ImageViewer);
USEFORM("Info.cpp", InfoForm);
USEFORM("Loupe.cpp", LoupeForm);
USEFORM("Main.cpp", MainForm);
USEFORM("OptionDlg.cpp", ToolOptionsDialog);
USEFORM("Ruler.cpp", RulerForm);
USEFORM("ScreenForm.cpp", ScreenForm);
USEFORM("Tool.cpp", ToolForm);
USELIB("gif\gif.lib");
USELIB("png\png.lib");
USERC("Graphics.rc");
USERES("Cursors.res");
USERES("TopTools4.res");
USEUNIT("AutoSaveOptions.cpp");
USEUNIT("Grabber.cpp");
USEUNIT("hotkey\HotkeyInfo.cpp");
USEUNIT("HotkeyManager.cpp");
USEUNIT("LoupePanel.cpp");
USEUNIT("Monitor.cpp");
USEUNIT("PersistImage.cpp");
USEUNIT("TopToolBar.cpp");
USEUNIT("TrayIcon.cpp");
USEFORM("QuerySaveDlg.cpp", QuerySaveDialog);

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    g_ToolOptions.Load("Software\\TopTools 4\\");
    bool IsSingleton = g_ToolOptions.Get("main", "singleton", false);

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
        Application->Title = "TopTools 4";
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

