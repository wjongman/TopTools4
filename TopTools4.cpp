//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PersistOptions.h"

//---------------------------------------------------------------------------
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
USELIB("hotkey\TopToolParts.lib");
USERES("Cursors.res");
USERES("TopTools4.res");
USEUNIT("AutoSave.cpp");
USEUNIT("Grabber.cpp");
USEUNIT("hotkey\HotkeyInfo.cpp");
USEUNIT("HotkeyManager.cpp");
USEUNIT("LoupePanel.cpp");
USEUNIT("Monitor.cpp");
USEUNIT("PersistImage.cpp");
USEUNIT("TopToolBar.cpp");
USEUNIT("TrayIcon.cpp");
USERES("Graphics.RES");
USERES("appicon.RES");
USEFORM("PresetManager.cpp", PresetManager);
USEFORM("PresetDlg.cpp", PresetDlg);
USEFORM("CustomCopyDlg.cpp", CustomCopyDlg);

#ifndef DPI_ENUMS_DECLARED

typedef enum PROCESS_DPI_AWARENESS
{
    PROCESS_DPI_UNAWARE = 0,
    PROCESS_SYSTEM_DPI_AWARE = 1,
    PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

typedef enum MONITOR_DPI_TYPE
{
    MDT_EFFECTIVE_DPI = 0,
    MDT_ANGULAR_DPI = 1,
    MDT_RAW_DPI = 2,
    MDT_DEFAULT = MDT_EFFECTIVE_DPI
} MONITOR_DPI_TYPE;

#define DPI_ENUMS_DECLARED
#endif // (DPI_ENUMS_DECLARED)

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

    HMODULE hUser32 = LoadLibrary(_T("user32.dll"));
    if (hUser32)
    {
        typedef BOOL (*SetProcessDPIAwareFunc)();
        SetProcessDPIAwareFunc setDPIAware =
            (SetProcessDPIAwareFunc)GetProcAddress(hUser32, "SetProcessDPIAware");

        if (setDPIAware)
            setDPIAware();

        FreeLibrary(hUser32);
    }

    HMODULE hShcore = LoadLibrary(_T("Shcore.dll"));
    if (hShcore)
    {
        typedef HRESULT WINAPI (*SetProcessDPIAwareFunc)(PROCESS_DPI_AWARENESS);
        SetProcessDPIAwareFunc setDPIAware =
            (SetProcessDPIAwareFunc)GetProcAddress(hShcore, "SetProcessDpiAwareness");

        if (setDPIAware)
            setDPIAware(PROCESS_SYSTEM_DPI_AWARE);

        FreeLibrary(hShcore);
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

