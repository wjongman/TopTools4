//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MouseTracker.h"

HHOOK TMouseTracker::m_hJournalRecordHook = NULL;

//const int crCrosshair = 10;
//---------------------------------------------------------------------------
__fastcall TMouseTracker::TMouseTracker(TComponent* Owner)
: TCustomControl(Owner),
  FOnCaptureLost(NULL)
{
  TRACE("TMouseTracker::TMouseTracker");
  m_TrackCursor = ::LoadCursor((void*)HInstance, "CROSSHAIR");
}

//---------------------------------------------------------------------------
__fastcall TMouseTracker::~TMouseTracker()
{
  TRACE("TMouseTracker::~TMouseTracker");
  // Just to be sure
  ReleaseCapture32();
}

//---------------------------------------------------------------------------
void __fastcall TMouseTracker::StartTracking()
{
  TRACE("TMouseTracker::StartTracking()");
  SetCapture32(Handle);
  ::SetCursor(m_TrackCursor);
}

//---------------------------------------------------------------------------
void __fastcall TMouseTracker::StopTracking()
{
  TRACE("TMouseTracker::StopTracking()");
  ReleaseCapture32();
}

//---------------------------------------------------------------------------
void __fastcall TMouseTracker::WMCaptureChanged(TWMNoParams &Message)
{
  // The WM_CAPTURECHANGED message is sent to the window that
  // is losing the mouse capture.
  // A window receives this message even if it calls ReleaseCapture itself.

  TRACE("TMouseTracker::WMCaptureChanged");

  // Notify our parent
  if (FOnCaptureLost)
    FOnCaptureLost(this);
}

//---------------------------------------------------------------------------
void __fastcall TMouseTracker::WMCancelJournal(TWMNoParams &Message)
{
  // A user can force asynchronous input processing
  // back on by pressing Ctrl+Esc.  When this happens,
  // the operating system notifies the thread by
  // posting a WM_CANCELJOURNAL message in the
  // thread's message queue.

  TRACE("TMouseTracker::WMCancelJournal");

  if (FOnCancel)
    FOnCancel(this);

  ReleaseCapture32();
}

//---------------------------------------------------------------------------
HWND TMouseTracker::SetCapture32(HWND hCaptureWnd)
{
  TRACE("TMouseTracker::SetCapture32()");
  // If a journal hook is installed, uninstall it.
  if (m_hJournalRecordHook != NULL)
    ReleaseCapture32();

  // Install a journal hook to turn off asynchronous
  // input processing and set capture.
  m_hJournalRecordHook  = SetWindowsHookEx(WH_JOURNALRECORD,
                                           (HOOKPROC)JournalRecordProc,
                                           GetModuleHandle(NULL), 0);
  return SetCapture(hCaptureWnd);
}

//---------------------------------------------------------------------------
void TMouseTracker::ReleaseCapture32()
{
  TRACE("TMouseTracker::ReleaseCapture32()");
  ReleaseCapture();

  // If a journal hook is installed, we must turn
  // asynchronous input processing back on.
  if (m_hJournalRecordHook != NULL)
  {
    UnhookWindowsHookEx(m_hJournalRecordHook);
    m_hJournalRecordHook = NULL;
  }
}

//---------------------------------------------------------------------------

