//---------------------------------------------------------------------------
#ifndef MouseTrackerH
#define MouseTrackerH

#ifdef _DEBUG
#define TRACE(s) OutputDebugString(s)
#else
#define TRACE(s)
#endif

//#define WM_TRACKING_STOPPED (WM_USER + 10) 

/////////////////////////////////////////////////////////////////////////////
// Class to supply full screen mouse-capture for Win32 applications
//
//
// The idea of using a journal hook to keep receiving mouse messages is
// from Jeffrey Richter's Win32 Q&A column in Microsoft Systems Journal
// April 1995 — Vol 10 No 4
//
/////////////////////////////////////////////////////////////////////////////

class TMouseTracker : public TCustomControl
{
public:
  __fastcall TMouseTracker(TComponent* Owner);
  __fastcall ~TMouseTracker();

  void __fastcall StartTracking();
  void __fastcall StopTracking();

__published:
//  __property TCursor Cursor = { read = FTrackCursor, write = FTrackCursor };
  __property OnMouseMove;
  __property OnMouseDown;
  __property OnMouseUp;
  __property TNotifyEvent OnCaptureLost = { read = FOnCaptureLost, write = FOnCaptureLost };
  __property TNotifyEvent OnCancel = { read = FOnCancel, write = FOnCancel };

protected:
  BEGIN_MESSAGE_MAP
    VCL_MESSAGE_HANDLER(WM_CAPTURECHANGED, TWMNoParams, WMCaptureChanged);
    VCL_MESSAGE_HANDLER(WM_CANCELJOURNAL, TWMNoParams, WMCancelJournal);
  END_MESSAGE_MAP(TCustomControl);

  MESSAGE void __fastcall WMCaptureChanged(TWMNoParams &Message);
  MESSAGE void __fastcall WMCancelJournal(TWMNoParams &Message);

private:
  // Handle for journal record hook.
  // The handle is NULL when the hook is not installed.
  static HHOOK m_hJournalRecordHook;

  // Callback function for SetWindowsHookEx
  // This journal record function doesn't need to do
  // anything so we just pass the hook notification on
  // to any other installed journal record hooks.
  static LRESULT CALLBACK JournalRecordProc (int nCode, WPARAM wParam, LPARAM lParam)
  {
    return CallNextHookEx(TMouseTracker::m_hJournalRecordHook,  nCode, wParam, lParam);
  }

  HWND SetCapture32(HWND hCaptureWnd);
  void ReleaseCapture32();

  TNotifyEvent FOnCaptureLost;
  TNotifyEvent FOnCancel;
  HCURSOR m_TrackCursor;
//  TCursor FSavedCursor;
//  TCursor FTrackCursor;
};

  #endif
