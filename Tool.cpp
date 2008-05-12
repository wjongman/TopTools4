//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Tool.h"
#include "SnapRect.h"
#include "Monitor.h"
#include "Transparency.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TToolForm::TToolForm(TComponent* Owner, const String& toolname)
: TForm(Owner), m_ToolName(toolname), FSnapEdge(true), FWindowStyle(twsTool),
  FDraggableForm(false)//, m_Transparent(false)
{
////  m_pOptions = new TopTools::Options(toolname);
//  m_Options.Init(toolname);

  //  m_RegBaseKey = g_RegBaseKey;
//  LoadSettings();

  OnShow = OnToolShow;   // OnToolShow calls LoadPosition()
  OnHide = OnToolHide;   // OnToolHide calls SavePosition()
}

//---------------------------------------------------------------------------
__fastcall TToolForm::~TToolForm()
{
  //delete m_pOptions;
//  SaveSettings();
}

//---------------------------------------------------------------------------
void __fastcall TToolForm::CreateParams(TCreateParams &Params)
{
  if (FWindowStyle == twsTool)
  {
    TForm::CreateParams(Params);
    Params.ExStyle |= WS_EX_TOOLWINDOW;
  }
}

//---------------------------------------------------------------------------
void __fastcall TToolForm::ReadState(Classes::TReader* Reader)
{
  TForm::ReadState(Reader);
  OldCreateOrder = false;
}

//---------------------------------------------------------------------------
void __fastcall TToolForm::WndProc0(Messages::TMessage &Message)
{
  if (Message.Msg == WM_DISPLAYCHANGE || Message.Msg == WM_SETTINGCHANGE)
  {
    // Adapt our position to the new settings
    ConstrainPosition();
  }

  else if (Message.Msg == WM_WINDOWPOSCHANGING)
  {
    WINDOWPOS *wndPos = reinterpret_cast<WINDOWPOS*>(Message.LParam);
    // If it is actually our window that moved
    if (wndPos->hwnd == Handle)
    {
      bool controlkeydown = GetKeyState(VK_CONTROL) & 0x8000;
      // If FSnap = true we snap to desktop edges unless Ctrl key is down
      // If FSnap = false we snap to desktop edges only when Ctrl key is down
      bool snap = controlkeydown ^ FSnapEdge;
      if (snap)
      {
        // Get the bounds of our window
        RECT rcWin;
        GetWindowRect(Handle, &rcWin);

        // Get the monitor nearest to the window rect.
        HMONITOR hMonitor;
        hMonitor = MonitorFromRect(&rcWin, MONITOR_DEFAULTTONEAREST);

        // Get the work area or entire monitor rect.
        MONITORINFO mi;
        mi.cbSize = sizeof(mi);
        GetMonitorInfo(hMonitor, &mi);
        RECT rcWorkArea = mi.rcWork;

        // Snap to edges of work area
        int EdgeSnapGap = 15;
        TSnapRect sr(wndPos);
        sr.SnapToInsideOf(rcWorkArea, EdgeSnapGap);

        wndPos->x = sr.left;
        wndPos->y = sr.top;
      }
    }
  }
  else if (Message.Msg == WM_SETFOCUS)
  {
    OnGetFocus();
  }
  else if (Message.Msg == WM_NCACTIVATE)
  {
    OnLoseFocus();
  }
  else if (Message.Msg == WM_SYSCOMMAND)
  {
    switch (Message.WParam & 0xFFF0) // Lowest byte is reserved..
    {
    case SC_MINIMIZE:
      // Someone clicked the minimize button of our window or
      // selected "Minimize" in the system menu.
      if (FOnMinimize)
      {
        OnMinimize(this);
        // Prevent further message processing
        Message.Result = 0;
        return;
      }
      break;
/*
      case SC_CLOSE:
        // Someone clicked the close button of our window or
        // selected "Close" in the system menu.
        if (FOnSysClose)
        {
          OnOnSysClose(this);
          // No further message processing
          Message.Result = 0;
          return;
        }
        break;
*/
    }
  }

  // Resume normal processing
  TForm::WndProc(Message);
}

//---------------------------------------------------------------------------
void __fastcall TToolForm::WndProc(Messages::TMessage &Message)
{
  switch (Message.Msg)
  {
  case WM_DISPLAYCHANGE:
  case WM_SETTINGCHANGE:
    // Adapt our position to the new settings
    ConstrainPosition();
    break;

  case WM_WINDOWPOSCHANGING:
    {
      WINDOWPOS *wndPos = reinterpret_cast<WINDOWPOS*>(Message.LParam);
      // If it is actually our window that moved
      if (wndPos->hwnd == Handle)
      {
        bool controlkeydown = GetKeyState(VK_CONTROL) & 0x8000;
        // If FSnap = true, we snap to desktop edges unless Ctrl key is down
        // If FSnap = false, we snap to desktop edges only when Ctrl key is down
        bool snap = controlkeydown ^ FSnapEdge;
        if (snap)
        {
          // Get the bounds of our window
          RECT rcWin;
          GetWindowRect(Handle, &rcWin);

          // Get the monitor nearest to the window rect.
          HMONITOR hMonitor;
          hMonitor = MonitorFromRect(&rcWin, MONITOR_DEFAULTTONEAREST);

          // Get the work area or entire monitor rect.
          MONITORINFO mi;
          mi.cbSize = sizeof(mi);
          GetMonitorInfo(hMonitor, &mi);
          RECT rcWorkArea = mi.rcWork;

          // Snap to edges of work area
          int EdgeSnapGap = 15;
          TSnapRect sr(wndPos);
          sr.SnapToInsideOf(rcWorkArea, EdgeSnapGap);

          wndPos->x = sr.left;
          wndPos->y = sr.top;
        }
      }
    }
    break;

  case WM_SETFOCUS:
    OnGetFocus();
    break;

  case WM_NCACTIVATE:
    OnLoseFocus();
    break;
/*
  case WM_NCHITTEST:
    if (FDraggableForm == true)
    {
      UINT uHitTest;
      uHitTest = DefWindowProc(Handle, WM_NCHITTEST, Message.WParam, Message.LParam);
      // See if the mousepointer is above our client area
      if (uHitTest == HTCLIENT)
      {
        Message.Result = HTCAPTION;
        return;
      }
      // See if the mousepointer is above our size grip
      else if (Message.Result == HTCLIENT)
      {
        TRect rc = GetClientRect();
        rc.Left = rc.Right - GetSystemMetrics(SM_CXHSCROLL);
        rc.Top  = rc.Bottom - GetSystemMetrics(SM_CYVSCROLL);

        POINT pt = ScreenToClient(Point(Message.WParam, Message.LParam));

        if (PtInRect(&rc, pt))
        {
          Message.Result = HTBOTTOMRIGHT;
          return;
        }
      }
      else
      {
        Message.Result = uHitTest;
        return;
      }
    }
*/
  case WM_SYSCOMMAND:
    switch (Message.WParam & 0xFFF0) // Lowest byte is reserved..
    {
    case SC_MINIMIZE:
      // Someone clicked the minimize button of our window or
      // selected "Minimize" in the system menu.
      if (FOnMinimize)
      {
        OnMinimize(this);
        // Prevent further message processing
        Message.Result = 0;
        return;
      }
    }
    break;
  }

  // Resume normal processing
  TForm::WndProc(Message);
}

//---------------------------------------------------------------------------
bool __fastcall TToolForm::TransparencyIsSupported()
{
  return TTransparencyHandler::Instance().Supported();
}

//---------------------------------------------------------------------------
void __fastcall TToolForm::SetTransparency(bool layered, int percent)
{
  TTransparencyHandler::Instance().SetTransparency(Handle, layered, percent);
}

//---------------------------------------------------------------------------
void __fastcall TToolForm::SetColorKey(bool layered, COLORREF colorkey)
{
  TTransparencyHandler::Instance().SetColorKey(Handle, layered, colorkey);
}

//---------------------------------------------------------------------------
void __fastcall TToolForm::LoadPosition()
{
  // Retrieve saved settings from the registry
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, false))
    {
      int left = 0;
      int top = 0;

      if (Reg->ValueExists("left"))
        left = Reg->ReadInteger("left");
      if (Reg->ValueExists("top"))
        top = Reg->ReadInteger("top");

      Reg->CloseKey();
      SetBounds(left, top, Width, Height);
    }
    else
    {
      SetDefaultPosition();
      return;
    }
  }
  __finally
  {
    delete Reg;
  }

  // Make sure we don't end up outside the desktop area
//  ConstrainPosition();
}

//---------------------------------------------------------------------------
void __fastcall TToolForm::SavePosition()
{
  // Save settings in the registry
  TRegistry *Reg = new TRegistry();
  Reg->RootKey = HKEY_CURRENT_USER;
  try
  {
    if (Reg->OpenKey(g_RegBaseKey + m_ToolName, true))
    {
      Reg->WriteInteger("left", Left);
      Reg->WriteInteger("top", Top);
      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}

//---------------------------------------------------------------------------
void __fastcall TToolForm::ConstrainPosition()
{
  ClipWindowToMonitor(Handle);
}

//---------------------------------------------------------------------------
void __fastcall TToolForm::SetDefaultPosition()
{
  CenterWindowToMonitor(Handle);
}

//---------------------------------------------------------------------------
void SnapRectangles(const RECT rcFix, LPRECT rcVar, bool inside)
// Adjust position of rcVar so it snaps to rcFix.
{
  // When we test for a window to snap to, we only snap right-to-left,
  // left-to-right, top-to-bottom, and bottom-to-top.
  // When we test for the screen we must reverse the order, snapping
  // the right of our window to the right edge of the screen, etc.

  int m_EdgeSnapGap = 15;

  if (inside) // Snap rcVar inside rcFix
  {
    int rcVarWidth = rcVar->right - rcVar->left;
    int rcVarHeight = rcVar->bottom - rcVar->top;

    // Snap X axis
    if (abs(rcVar->left - rcFix.left) <= m_EdgeSnapGap)
    {
      rcVar->left = rcFix.left;
      rcVar->right = rcFix.left + rcVarWidth;
    }
    else if (abs(rcVar->right - rcFix.right) <= m_EdgeSnapGap)
    {
      rcVar->left = rcFix.right - rcVarWidth;
      rcVar->right = rcFix.right;
    }

    // Snap Y axis
    if (abs(rcVar->top - rcFix.top) <= m_EdgeSnapGap)
    {
      rcVar->top = rcFix.top;
      rcVar->bottom = rcFix.top + rcVarHeight;
    }
    else if (abs(rcVar->bottom - rcFix.bottom) <= m_EdgeSnapGap)
    {
      rcVar->top = rcFix.bottom - rcVarHeight;
      rcVar->bottom = rcFix.top;
    }
  }
}

//---------------------------------------------------------------------------
int __fastcall TToolForm::GetTaskbarRect(LPRECT lprect)
{
  // Retrieves the bounding rectangle of the Windows taskbar
  //
  // lprect: structure to contain the bounding rectangle,
  // in screen coordinates, of the Windows taskbar.
  //
  // returns a value that specifies an edge of the screen,
  // these can be one of ABE_LEFT , ABE_RIGHT, ABE_TOP or
  // ABE_BOTTOM
  //
  // returns -1 if an error occured
  //
  APPBARDATA appBarData;
  appBarData.cbSize = sizeof(appBarData);
  if (SHAppBarMessage(ABM_GETTASKBARPOS, &appBarData))
  {
    lprect->top    = appBarData.rc.top;
    lprect->bottom = appBarData.rc.bottom;
    lprect->left   = appBarData.rc.left;
    lprect->right  = appBarData.rc.right;

    return appBarData.uEdge;
  }
  return -1;
}

//---------------------------------------------------------------------------
void __fastcall TToolForm::SetWindowStyle(TToolWindowStyle style)
// Called when "WindowStyle" property is changed
{
  if (FWindowStyle != style)
  {
    if (style == twsTool)
    {
      // Make window a toolwindow
      DWORD dwExStyle = GetWindowLong(Handle, GWL_EXSTYLE);
      dwExStyle |= WS_EX_TOOLWINDOW;
      SetWindowLong(Handle, GWL_EXSTYLE, dwExStyle);

      if (IsIconic(Handle))
        ShowWindow(Handle, SW_RESTORE);
    }
    else
    {
      DWORD dwStyle = GetWindowLong(Handle, GWL_STYLE);
      // Disable the maximize button
      dwStyle &= ~WS_MAXIMIZEBOX;
      // Show minimize button
      dwStyle |= WS_MINIMIZEBOX;
      SetWindowLong(Handle, GWL_STYLE, dwStyle);

      // Make window a normal window
      DWORD dwExStyle = GetWindowLong(Handle, GWL_EXSTYLE);
      dwExStyle &= ~WS_EX_TOOLWINDOW;
      SetWindowLong(Handle, GWL_EXSTYLE, dwExStyle);

      if (IsIconic(Handle))
        ShowWindow(Handle, SW_RESTORE);
    }
    FWindowStyle = style;
  }

  // Force a total redraw
  DWORD dwFlags = SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOZORDER | SWP_NOSIZE;
  ::SetWindowPos(Handle, 0, 0, 0, 0, 0, dwFlags);
}

/*
//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::GetSetting()
{
//  TopTools::Options Options("baseconv");
//  bool bTest = m_ToolOptions.GetBool("baseconv", "showbinary");
  TopTools::TPersistOptions& m_ToolOptions = TopTools::TPersistOptions::Instance();
  m_ToolOptions.Load();
  ShowBinaryField(m_ToolOptions.GetBool("baseconv", "showbinary"));

  //   TBaseconvOptions options;
//   ShowBinaryField(options.ShowBinary);
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::SetSetting()
{
//  TopTools::Options Options("baseconv");
  TopTools::TPersistOptions& m_ToolOptions = TopTools::TPersistOptions::Instance();

  m_ToolOptions.Set("baseconv", "showbinary", plBinary->Visible);
  m_ToolOptions.Save();

//   TBaseconvOptions options;
//   options.ShowBinary = plBinary->Visible;
//   options.Save();
}
*/

