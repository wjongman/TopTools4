//---------------------------------------------------------------------------
#ifndef TransparencyH
#define TransparencyH

#include <tchar.h>

#ifndef WS_EX_LAYERED
    #define WS_EX_LAYERED           0x00080000
    #define LWA_COLORKEY            0x00000001
    #define LWA_ALPHA               0x00000002
#endif

//---------------------------------------------------------------------------
/*

A singleton class to control transparency of any window

Adapted from "Win2K transparent dialogs" By Per-Erik Nordlund
http://www.codeproject.com/w2k/win2k_transparent.asp

usage:

#include "Transparency.h" in your form's cpp file

example wrapper functions:

  bool MyWindow::TransparencyIsSupported()
  {
    return TTransparencyHandler::Instance().Supported();
  }

  void MyWindow::SetTransparency(bool layered, int percent)
  {
    TTransparencyHandler::Instance().SetTransparency(m_hwnd, layered, percent);
  }

  void MyWindow::SetColorKey(bool layered, COLORREF colorkey)
  {
    TTransparencyHandler::Instance().SetColorKey(m_hwnd, layered, colorkey);
  }

*/

//---------------------------------------------------------------------------
class TTransparencyHandler
{
public:
    //-------------------------------------------------------------------------
    inline static TTransparencyHandler& Instance()
    {
        // All references to this class are through this function
        // A static object of this class will be instantiated at the
        // first time this function is called (lazy initialisation).
        static TTransparencyHandler instance;
        return instance;
    }

    //-------------------------------------------------------------------------
    void SetColorKey(HWND hWnd, bool layered, COLORREF colorkey)
    {
        if (layered && m_fnSetLayerAttributes)
        {
            // Set layered bit
            SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
            m_fnSetLayerAttributes(hWnd, colorkey, 0, LWA_COLORKEY);
        }
        else
        {
            // Clear layered bit
            SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
            // Ask the window and its children to repaint
            RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
        }
    }

    //-------------------------------------------------------------------------
    void SetTransparency(HWND hWnd, bool layered, int percent)
    {
        if (layered && m_fnSetLayerAttributes)
        {
            if (percent >= 0 && percent <= 100)
            {
                // Set layered bit
                SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
                // Set the window to "percent" % transparency (100 - percent opacity).
                m_fnSetLayerAttributes(hWnd, 0, (BYTE)((255 * (100 - percent)) / 100), LWA_ALPHA);
            }
        }
        else
        {
            // Clear layered bit
            SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
            // Ask the window and its children to repaint
            RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
        }
    }

    //-------------------------------------------------------------------------
    // This function can be used to find out if the OS supports transparency
    bool Supported()
    {
        return (m_fnSetLayerAttributes != NULL);
    }

private:
    //-------------------------------------------------------------------------
    // All constructors are private, an object of this class can only be
    // instantiated or referenced through the static Instance() function
    inline explicit TTransparencyHandler()
    {
        ImportFuncFromUser32();
    }
//  inline ~TTransparencyHandler() {}
    inline explicit TTransparencyHandler(TTransparencyHandler const&)
    {
    }

    inline TTransparencyHandler& operator=(TTransparencyHandler const&)
    {
        return *this;
    }

    //-------------------------------------------------------------------------
    // Prototype for the function we want to import from USER32.DLL
    typedef BOOL (WINAPI *SetLayeredWindowAttributesFunc)(HWND hWnd, COLORREF crKey,
                                                          BYTE bAlpha, DWORD dwFlags);
    // Member pointer to this function
    SetLayeredWindowAttributesFunc m_fnSetLayerAttributes;

    //-------------------------------------------------------------------------
    void ImportFuncFromUser32()
    {
        m_fnSetLayerAttributes = NULL;
        // This wil only work for Windows 2000 and above
        if (GetWindowsVersion() >= 5)
        {
            // Here we import the function from USER32.DLL
            HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
            if (hUser32)
            {
                m_fnSetLayerAttributes =
                (SetLayeredWindowAttributesFunc)GetProcAddress(hUser32,
                                                               "SetLayeredWindowAttributes");
            }
        }
    }

    //-------------------------------------------------------------------------
    int GetWindowsVersion()
    {
        OSVERSIONINFO os;
        os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        GetVersionEx(&os);
        return os.dwMajorVersion;
    }

    //---------------------------------------------------------------------------
    bool DisplayIsPaletted()
    {
        HDC dcDesktop = GetDC(NULL);
        int result = GetDeviceCaps(dcDesktop, RASTERCAPS);
        ReleaseDC(NULL, dcDesktop);
        return(result & RC_PALETTE);
    }

}; // end of class TTransparencyHandler

#endif
