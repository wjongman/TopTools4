//---------------------------------------------------------------------------

#ifndef MonitorH
#define MonitorH
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
#define MONITOR_CENTER   0x0001        // center rect to monitor
#define MONITOR_CLIP     0x0000        // clip rect to monitor
#define MONITOR_WORKAREA 0x0002        // use monitor work area
#define MONITOR_AREA     0x0000        // use monitor entire area

void ClipOrCenterRectToMonitor(LPRECT prc, UINT flags);
LPRECT ConstrainBounds(LPRECT prc);
void CenterWindowToMonitor(HWND hwnd);
void ClipWindowToMonitor(HWND hwnd);

#endif
