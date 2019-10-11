// Windows Vista introduces the concept of physical coordinates.
// Desktop Window Manager (DWM) scales non-dots per inch (dpi) aware windows
// when the display is high dpi.
//
// The window seen on the screen corresponds to the physical coordinates.
// The application continues to work in logical space.
//
// Therefore, the application's view of the window is different from that
// which appears on the screen;
// For scaled windows, logical and physical coordinates are different.

// This code tries to to set DPI-awarenes as high as possible.
// Since we don't know which OS-version we are running on, we
// test for available functions by using LoadLibrary/GetProcAddress.

/////////////////////////////////////////////////////////////////////////////
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
BOOL WINAPI SetProcessDPIAware(void)
{
    // Sets the current process as dots per inch (dpi) aware.
    // If the function fails the return value is zero.
    // SetProcessDPIAware is available for use in Windows Vista.
    // It may be altered or unavailable in subsequent versions.
    // Instead, use SetProcessDpiAwareness.
    BOOL result = 0;
    HMODULE hUser32 = LoadLibrary(_T("user32.dll"));
    if (hUser32)
    {
        typedef BOOL (*SetProcessDPIAwareFunc)();
        SetProcessDPIAwareFunc setDPIAware =
            (SetProcessDPIAwareFunc)GetProcAddress(hUser32, "SetProcessDPIAware");

        if (setDPIAware)
            result = setDPIAware();

        FreeLibrary(hUser32);
    }
    return result;
}

//---------------------------------------------------------------------------
HRESULT WINAPI SetProcessDpiAwareness(PROCESS_DPI_AWARENESS value)
{
    // Sets the current process to a specified dots per inch awareness level.
    // The DPI awareness levels are from the PROCESS_DPI_AWARENESS enumeration.
    HRESULT result = E_NOTIMPL;
    HMODULE hShcore = LoadLibrary(_T("Shcore.dll"));
    if (hShcore)
    {
        typedef HRESULT WINAPI (*SetProcessDPIAwareFunc)(PROCESS_DPI_AWARENESS);
        SetProcessDPIAwareFunc setDPIAware =
            (SetProcessDPIAwareFunc)GetProcAddress(hShcore, "SetProcessDpiAwareness");

        if (setDPIAware)
            result = setDPIAware(value);

        FreeLibrary(hShcore);
    }
    return result;
}

//---------------------------------------------------------------------------
PROCESS_DPI_AWARENESS SetSystemDpiAwareness()
{
    // Tries to set awareness to be system-dpi aware
    // This function must be executed before any DPI related function is called!
    // Returns actual awareness

    // Try to make us system-aware via Win10 API
    if (SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE) == S_OK)
    {
        return PROCESS_PER_MONITOR_DPI_AWARE;
    }
    // Try to set via Vista API
    if (SetProcessDPIAware())
    {
        return PROCESS_SYSTEM_DPI_AWARE;
    }
    // Default is unaware (pre-Vista)
    return PROCESS_DPI_UNAWARE;
}

//-------------------------------------------------------------------------
int GetSystemDPI()
{
    // Pre Windows 10 way of getting DPI settings
    HDC screen = GetDC(0);
    // Assume horizontal == vertical resolution
    int dpi = GetDeviceCaps(screen, LOGPIXELSY);
    ReleaseDC(0, screen);
    return dpi;
}

//---------------------------------------------------------------------------
UINT WINAPI GetDpiForSystem()
{
    // Return 96 for DPI unaware systems. For any other DPI_AWARENESS
    // value, the return value will be the actual system DPI.
    UINT result = 96;
    HMODULE hUser32 = LoadLibrary(_T("user32.dll"));
    if (hUser32)
    {
        typedef UINT (*Func)();
        Func getDPI = (Func)GetProcAddress(hUser32, "GetDpiForSystem");

        if (getDPI)
            // Windows 10
            result = getDPI();
        else
            // Fallback to old method
            result = GetSystemDPI();

        FreeLibrary(hUser32);
    }
    return result;
}

///////////////////////////////////////////////////////////////////////////////
// DPI Helper Class - functions to manage the state of DPI awareness,
// and scale values used for window size and rendering.
//
// Scale, GetScale, SetScale
// GetAwareness, SetAwareness
// SetDPIChanged, GetDPIChanged
class CDPI
{
    UINT m_nScaleFactor;
    UINT m_nScaleFactorSDA;
    PROCESS_DPI_AWARENESS m_Awareness;

public:
    //-------------------------------------------------------------------------
    CDPI()
    {
        m_nScaleFactor = 0;
        m_nScaleFactorSDA = 0;
        m_Awareness = PROCESS_DPI_UNAWARE;
    }

    //-------------------------------------------------------------------------
    UINT GetScale()
    {
        if (m_Awareness == PROCESS_DPI_UNAWARE)
        {
            return 100;
        }

        if (m_Awareness == PROCESS_SYSTEM_DPI_AWARE)
        {
            return m_nScaleFactorSDA;
        }

        return m_nScaleFactor;
    }

    //-------------------------------------------------------------------------
    void SetScale(UINT iDPI)
    {
        m_nScaleFactor = MulDiv(iDPI, 100, 96);
        if (m_nScaleFactorSDA == 0)
        {
            // Save the first scale factor, which is all that SDA apps know about
            m_nScaleFactorSDA = m_nScaleFactor;
        }
        return;
    }

    //-------------------------------------------------------------------------
    // Scale rectangle from raw pixels to relative pixels.
    void ScaleRect(RECT *pRect)
    {
        pRect->left = Scale(pRect->left);
        pRect->right = Scale(pRect->right);
        pRect->top = Scale(pRect->top);
        pRect->bottom = Scale(pRect->bottom);
    }

    //-------------------------------------------------------------------------
    // Scale Point from raw pixels to relative pixels.
    void ScalePoint(POINT *pPoint)
    {
        pPoint->x = Scale(pPoint->x);
        pPoint->y = Scale(pPoint->y);
    }

    //-------------------------------------------------------------------------
    int Scale(int x)
    {
        // DPI Unaware:  Return the input value with no scaling.
        // These apps are always virtualized to 96 DPI and scaled by
        // the system for the DPI of the monitor where shown.
        if (m_Awareness == PROCESS_DPI_UNAWARE)
        {
            return x;
        }

        // System DPI Aware:  Return the input value scaled by the factor
        // determined by the system DPI when the app was launched.
        // These apps render themselves according to the DPI of the display
        // where they are launched, and they expect that scaling to remain
        // constant for all displays on the system.
        // These apps are scaled up or down when moved to a display
        // with a different DPI from the system DPI.
        if (m_Awareness == PROCESS_SYSTEM_DPI_AWARE)
        {
            return MulDiv(x, m_nScaleFactorSDA, 100);
        }

        // Per-Monitor DPI Aware:  Return the input value scaled by
        // the factor for the display which contains most of the window.
        // These apps render themselves for any DPI, and re-render when
        // the DPI changes (as indicated by the WM_DPICHANGED window message).
        return MulDiv(x, m_nScaleFactor, 100);
    }

};
