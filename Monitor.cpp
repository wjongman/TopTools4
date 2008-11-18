//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Monitor.h"

//---------------------------------------------------------------------------
//
//  ClipOrCenterRectToMonitor
//
//  The most common problem apps have when running on a
//  multimonitor system is that they "clip" or "pin" windows
//  based on the SM_CXSCREEN and SM_CYSCREEN system metrics.
//  Because of app compatibility reasons these system metrics
//  return the size of the primary monitor.
//
//  This shows how you use the multi-monitor functions
//  to do the same thing.
//
//  flags:
//
//  MONITOR_CENTER   0x0001        // center rect to monitor
//  MONITOR_CLIP     0x0000        // clip rect to monitor
//  MONITOR_WORKAREA 0x0002        // use monitor work area
//  MONITOR_AREA     0x0000        // use monitor entire area

void ClipOrCenterRectToMonitor(LPRECT prc, UINT flags)
{
    HMONITOR hMonitor;
    MONITORINFO mi;
    RECT        rc;
    int         w = prc->right  - prc->left;
    int         h = prc->bottom - prc->top;

    // Get the nearest monitor to the passed rect.
    hMonitor = MonitorFromRect(prc, MONITOR_DEFAULTTONEAREST);

    // Get the work area or entire monitor rect.
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hMonitor, &mi);

    if (flags & MONITOR_WORKAREA)
        rc = mi.rcWork;
    else
        rc = mi.rcMonitor;

    // Center or clip the passed rect to the monitor rect
    if (flags & MONITOR_CENTER)
    {
        prc->left   = rc.left + (rc.right  - rc.left - w) / 2;
        prc->top    = rc.top  + (rc.bottom - rc.top  - h) / 2;
        prc->right  = prc->left + w;
        prc->bottom = prc->top  + h;
    }
    else
    {
        prc->left   = max(rc.left, min(rc.right-w,  prc->left));
        prc->top    = max(rc.top,  min(rc.bottom-h, prc->top));
        prc->right  = prc->left + w;
        prc->bottom = prc->top  + h;
    }
}

//---------------------------------------------------------------------------
LPRECT ConstrainBounds(LPRECT prc)
{
    ClipOrCenterRectToMonitor(prc, MONITOR_CLIP | MONITOR_WORKAREA);
    return prc;
}

//---------------------------------------------------------------------------
void CenterWindowToMonitor(HWND hwnd)
{
    RECT rc;
    GetWindowRect(hwnd, &rc);
    ClipOrCenterRectToMonitor(&rc, MONITOR_CENTER | MONITOR_WORKAREA);
    SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

//---------------------------------------------------------------------------
void ClipWindowToMonitor(HWND hwnd)
{
    RECT rc;
    GetWindowRect(hwnd, &rc);
    ClipOrCenterRectToMonitor(&rc, MONITOR_CLIP | MONITOR_WORKAREA);
    SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}



