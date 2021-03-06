//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <vcl\Clipbrd.hpp>

#include "Main.h"
#include "About.h"
#include "OptionDlg.h"
#include "InfoFormatter.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Tool"
#pragma link "Tool"
#pragma resource "*.dfm"
TMainForm *MainForm;

// Global option store
TPersistOptions g_ToolOptions;
// Global string to automaticly update build date
String g_sBuildDate = String(__DATE__);

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
  : TToolForm(Owner, "main"),
    m_pRuler(NULL),
    m_pLoupe(NULL),
    m_pBaseConv(NULL),
    m_pInfo(NULL),
    m_pControlBar(NULL),
    m_pCapture(NULL),
    m_pTrayIcon(NULL),
    m_UIMode(uiNormal)
{
    Application->OnDeactivate = HandleAppDeactivate;
    Application->OnRestore = HandleAppRestore;

    LoadSettings();

    m_HotkeyManager = new THotkeyManager(Handle);

    Timer->Enabled = TimerNeeded();
}

//---------------------------------------------------------------------------
__fastcall TMainForm::~TMainForm()
{
    SaveSettings();

    // Tool destuctors will save settings for each tool
    delete m_pTrayIcon;
    delete m_HotkeyManager;

    delete m_pRuler;
    delete m_pLoupe;
    delete m_pBaseConv;
    delete m_pInfo;
    delete m_pControlBar;
    delete m_pCapture;

    g_ToolOptions.Save();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::WndProc(Messages::TMessage &Message)
{
    // We want to be notified when the taskbar restarts, to make our
    // trayicon reappear after a restart of Explorer
    // MSDN Home -> MSDN Library -> User Interface Design and Development ->
    // Shell Programmers Guide ->  Intermediate Shell Techniques ->
    // Programming the Shell -> The Taskbar
    static unsigned int WM_TASKBARCREATED = 0;

    switch (Message.Msg)
    {
    case WM_CREATE:
        WM_TASKBARCREATED = ::RegisterWindowMessage("TaskbarCreated");
        break;

    case TRAYICONNOTIFY:
        HandleTrayMessage(Message);
        break;

    case WM_HOTKEY:
        HandleHotkey((THotkeyId)Message.WParam);
        break;

    case WM_QUERYENDSESSION:
        // About to shut down Windows, persist tool settings
        SaveSettings();
        break;

    case WM_SETFOCUS:
        OnGetFocus();
        break;

    case WM_NCACTIVATE:
        OnLoseFocus();
        break;
    }

    if (m_pTrayIcon && Message.Msg == WM_TASKBARCREATED && m_UIMode == uiTrayApp)
        m_pTrayIcon->Restore();

    TToolForm::WndProc(Message);
}

void __fastcall TMainForm::OnGetFocus()
{
    int test = 1;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::SetUI(bool isTrayApp)
{
    m_UIMode = isTrayApp ? uiTrayApp : uiNormal;

    if (isTrayApp)
    {
        GetControlBar()->WindowStyle = twsTool;
        ShowTrayIcon(true);
        ShowTaskbarIcon(false);
    }
    else
    {
        GetControlBar()->WindowStyle = twsNormal;
        ShowTrayIcon(false);
        ShowTaskbarIcon(true);
        GetControlBar()->Show();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ShowTrayIcon(bool show)
{
    if (show)
    {
        if (!m_pTrayIcon)
        {
            HICON hTrayIcon = ::LoadImage(HInstance, "TRAYICON", IMAGE_ICON, 16, 16, NULL);
            m_pTrayIcon = new cTrayIcon(Handle, hTrayIcon, "TopTools 4");
        }
        m_pTrayIcon->Add();
    }
    else
    {
        if (m_pTrayIcon)
        {
            m_pTrayIcon->Remove();
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ShowTaskbarIcon(bool show)
//  From: http://www.bcbdev.com/faqs/faq4.htm
//
//  You can remove a program's taskbar icon and prevent it from ever
//  appearing again by making the hidden application window a tool window.
//  Tool windows never have a taskbar icon.
//  Making the application window a tool window has the side effect of
//  removing the program from the list of programs that appear when the
//  user presses ALT-TAB.
//  You can make the application window a tool window by calling the API
//  GetWindowLong and SetWindowLong functions.
{
    ::ShowWindow(Application->Handle, SW_HIDE);

    if (show)
    {
        // Show taskbar icon by making the hidden application window a normal window
        DWORD dwExStyle = GetWindowLong(Application->Handle, GWL_EXSTYLE);
        dwExStyle &= ~WS_EX_TOOLWINDOW;
        SetWindowLong(Application->Handle, GWL_EXSTYLE, dwExStyle);

        ::ShowWindow(Application->Handle, SW_SHOW);
    }
    else
    {
        // Hide taskbar icon by making the hidden application window a toolwindow
        DWORD dwExStyle = GetWindowLong(Application->Handle, GWL_EXSTYLE);
        dwExStyle |= WS_EX_TOOLWINDOW;
        SetWindowLong(Application->Handle, GWL_EXSTYLE, dwExStyle);
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::HandleAppDeactivate(TObject *Sender)
{
    // Prevent windows from disappearing
    Application->RestoreTopMosts();
//    if (m_pCapture)
//        m_pCapture->RetoreLastFocus();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::HandleAppRestore(TObject *Sender)
{
    // Restore Toolbar
    GetControlBar()->Show();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::HandleHotkey(THotkeyId id)
{
    switch (id)
    {
    case hkDoubleclick:
        ToggleOpenTools();
        break;

    case hkColorCopy:
        CopyToClipboard();
        break;

    case hkRulerToggle:
        if (m_pRuler)
            m_pRuler->ToggleOrientation();
        break;

    case hkZoomIn:
        if (m_pLoupe)
            m_pLoupe->ZoomIn();
        break;

    case hkZoomOut:
        if (m_pLoupe)
            m_pLoupe->ZoomOut();
        break;

    case hkCaptureStart:
        actCaptureExecute(this);
        break;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::HandleTrayMessage(TMessage &Message)
{
    switch (Message.LParam)
    {
    case WM_RBUTTONUP:
        {
            TPoint ptMouse;
            GetCursorPos(&ptMouse);

            // See MS KB article Q135788
            // PRB: Menus for Notification Icons Do Not Work Correctly
            ::SetForegroundWindow (Handle);
            TrayMenu->Popup(ptMouse.x, ptMouse.y);
            ::PostMessage (Handle, WM_NULL, 0, 0);
            break;
        }
    case WM_LBUTTONDBLCLK:
        {
            ToggleOpenTools();
            break;
        }
    case WM_LBUTTONDOWN:
        {
            // Toggle stay-on-top
            SetTopMost(!m_bStayOnTop);
            break;
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::HandleKeyDown(TObject *Sender, WORD &Key,
                                         TShiftState Shift)
{
    switch (Key)
    {
    case VK_LEFT:     // Arrow keys move the ruler (cursor)
    case VK_RIGHT:
    case VK_UP:
    case VK_DOWN:
        if (m_pRuler)
            m_pRuler->HandleArrowKeys(Key, Shift);
        break;

    case VK_SPACE:    // Spacebar (un)locks the loupe position
        if (m_pLoupe)
            m_pLoupe->ToggleLock();
        break;

    case VK_ADD:      // numeric '+' zooms in
        if (m_pLoupe)
            m_pLoupe->ZoomIn();
        break;

    case VK_SUBTRACT: // numeric '-' zooms out
        if (m_pLoupe)
            m_pLoupe->ZoomOut();
        break;

    // TODO: Find a way to toggle ruler orientation around center
    // instead of around curent mouse position (for this case)
    //case VK_DIVIDE:   // numeric '/' toggles ruler orientation
    //    if (m_pRuler)
    //       m_pRuler->ToggleOrientation();
    //    break;

    case 'C':         // Control-C copies #webcolor
        if (Shift.Contains(ssCtrl))
            CopyToClipboard();
        break;
    }
    if (m_pLoupe)
        m_pLoupe->Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::HandleControlBarMinimize(TObject *Sender)
{
    // This should only happen when we are in uiNormal mode
    if (m_UIMode == uiNormal)
    {
        Application->Minimize();
    }
#ifdef _DEBUG
    else
    {
        ShowMessage("Minimize event in TrayApp mode!");
    }
#endif
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::HandleControlBarClose(TObject *Sender, TCloseAction &Action)
{
    if (m_UIMode == uiNormal)
    {
        // Act as if our (hidden) main window was closed
        Application->Terminate();
    }
    else if (m_UIMode == uiTrayApp)
    {
        // Hide the controlbar
        Action = caHide;
        // Update UI
        actControl->Checked = false;
        // Stop the timer when it isn't needed anymore
        Timer->Enabled = TimerNeeded();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::HandleToolWindowClose(TObject *Sender, TCloseAction &Action)
// Update the UI after a toolwindow has closed
{
    Action = caHide;

    if (Sender == m_pControlBar || Sender == this)
    {
        actControl->Checked = false;
    }
    else if (Sender == m_pBaseConv)
    {
        actBaseConv->Checked = false;
    }
    else if (Sender == m_pInfo)
    {
        actInfo->Checked = false;
    }
    else if (Sender == m_pLoupe)
    {
        actLoupe->Checked = false;
    }
    else if (Sender == m_pRuler)
    {
        actRuler->Checked = false;
    }
    else if (Sender == m_pCapture)
    {
        actCapture->Checked = false;
    }
    if (GetControlBar() && m_pControlBar->Visible)
    {
        m_pControlBar->UpdateFocus();
    }
    // Stop the timer when it isn't needed anymore
    Timer->Enabled = TimerNeeded();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::HandleTimerEvent(TObject *Sender)
{
    static POINT ptLastMouse = TPoint(-1, -1);

    POINT ptCurMouse;
    GetCursorPos(&ptCurMouse);

    // Only update ruler if mouse has moved
    if (ptCurMouse.x != ptLastMouse.x || ptCurMouse.y != ptLastMouse.y)
    {
        ptLastMouse.x = ptCurMouse.x;
        ptLastMouse.y = ptCurMouse.y;

        if (m_pRuler)
            m_pRuler->TimerEvent(ptCurMouse);
    }

    if (m_pInfo)
        m_pInfo->TimerEvent(ptCurMouse);

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadSettings()
{
    SetUI(g_ToolOptions.Get("main", "istrayapp", false));
    m_bRememberState = g_ToolOptions.Get("main", "rememberstate", true);
    RestoreToolState(g_ToolOptions.Get("main", "savedstate", dcoControl));
    m_bStayOnTop = g_ToolOptions.Get("main", "stayontop", true);
    SetTopMost(m_bStayOnTop);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::UpdateSettings()
{
    SetUI(g_ToolOptions.Get("main", "istrayapp", false));
    m_bRememberState = g_ToolOptions.Get("main", "rememberstate", true);
    m_bStayOnTop = g_ToolOptions.Get("main", "stayontop", true);
    SetTopMost(m_bStayOnTop);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveSettings()
{
    if (m_bRememberState)
        g_ToolOptions.Set("main", "savedstate", GetToolState());
    else
        g_ToolOptions.Set("main", "savedstate", 0);

    g_ToolOptions.Set("main", "istrayapp", (m_UIMode == uiTrayApp));
    g_ToolOptions.Set("main", "stayontop", m_bStayOnTop);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ActAsSingleton(bool singleton)
{
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::SetTopMost(bool ontop)
{
    if (ontop)
        SetWindowPos(Application->Handle, HWND_TOPMOST, 0, 0, 0, 0,
                     SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
    else
        SetWindowPos(Application->Handle, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

    m_bStayOnTop = ontop;
}

//---------------------------------------------------------------------------
bool TMainForm::TimerNeeded()
{
    // We only enable the timer when there is something to keep track of
    return( (m_pInfo && m_pInfo->Visible) ||
            (m_pRuler && m_pRuler->Visible) );
}

//---------------------------------------------------------------------------
void TMainForm::ToggleOpenTools()
{
    if (AnyToolVisible())
    {
        HideAll();
    }
    else
    {
        RestoreToolState(g_ToolOptions.Get("main", "doubleclick", dcoControl));
    }
}

//---------------------------------------------------------------------------
void TMainForm::RestoreToolState(int opentools)
{
    // Show toolwindows according to the bits set in opentools
    if (opentools & dcoControl)
        actCommandExecute(actControl);

    if (opentools & dcoRuler)
        actCommandExecute(actRuler);

    if (opentools & dcoLoupe)
        actCommandExecute(actLoupe);

    if (opentools & dcoInfo)
        actCommandExecute(actInfo);

    if (opentools & dcoBaseconv)
        actCommandExecute(actBaseConv);

    if (opentools & dcoGrab)
        actCommandExecute(actCapture);
}

//---------------------------------------------------------------------------
int TMainForm::GetToolState()
{
    // Return map of visible toolwindows as a bit-field
    return( (GetControlBar()->Visible) * dcoControl +
            (m_pRuler && m_pRuler->Visible) * dcoRuler +
            (m_pLoupe && m_pLoupe->Visible) * dcoLoupe +
            (m_pInfo && m_pInfo->Visible) * dcoInfo +
            (m_pBaseConv && m_pBaseConv->Visible) * dcoBaseconv +
            (m_pCapture && m_pCapture->Visible) * dcoGrab);
}

//---------------------------------------------------------------------------
bool TMainForm::AnyToolVisible()
{
    return (GetToolState() != 0);
}

//---------------------------------------------------------------------------
void TMainForm::HideAll()
{
    if (m_pRuler)
    {
        m_pRuler->Visible = false;
        actRuler->Checked = false;
    }
    if (m_pLoupe)
    {
        m_pLoupe->Visible = false;
        actLoupe->Checked = false;
    }
    if (m_pInfo)
    {
        m_pInfo->Visible = false;
        actInfo->Checked = false;
    }
    if (m_pBaseConv)
    {
        m_pBaseConv->Visible = false;
        actBaseConv->Checked = false;
    }
    if (m_pCapture)
    {
        m_pCapture->Visible = false;
        actCapture->Checked = false;
    }

    GetControlBar()->Hide();
    actControl->Checked = false;
}

//---------------------------------------------------------------------------
void TMainForm::CopyToClipboard()
{
    if (g_ToolOptions.Get("info", "custom", false))
    {
        CopyInfoToClipboard();
    }
    else
    {
        CopyWebColorToClipboard();
    }
}

//---------------------------------------------------------------------------
void TMainForm::CopyInfoToClipboard()
{
    // Probe color under the mouse
    TPoint ptMouse;
    GetCursorPos(&ptMouse);
    TPixelInfo pi(ptMouse.x, ptMouse.y);

    // Format the info string
    String mask = g_ToolOptions.Get("info", "mask", "");
    InfoFormatter inf(mask.c_str());
    String formatted = inf.GetFormattedString(pi).c_str();

    // Copy info string to clipboard
    Clipboard()->SetTextBuf(formatted.c_str());
}

//---------------------------------------------------------------------------
void TMainForm::CopyWebColorToClipboard()
{
    // Probe the color under the mouse
    TPoint ptMouse;
    GetCursorPos(&ptMouse);
    TPixelInfo pi(ptMouse.x, ptMouse.y);

    // Format the info string
    bool quotes = g_ToolOptions.Get("info", "quotes", false);
    bool prefix = g_ToolOptions.Get("info", "prefix", false);

    std::stringstream mask;

    if (quotes)
        mask << "\"";
    if (prefix)
        mask << "#";

    mask << std::setfill('0') << std::setw(2) << std::hex << std::uppercase;
    mask << pi.r << pi.g << pi.b;

    if (quotes)
        mask << "\"";

    String formatted = mask.str().c_str();
    Clipboard()->SetTextBuf(formatted.c_str());
}

//---------------------------------------------------------------------------
TToolForm* TMainForm::GetRulerForm()
{
    if (!m_pRuler)
    {
        m_pRuler = new TRulerForm(this);
        m_pRuler->OnClose = HandleToolWindowClose;
        m_pRuler->OnKeyDown = HandleKeyDown;
        m_pRuler->OnOptions = actOptionsExecute;
    }
    return m_pRuler;
}

//---------------------------------------------------------------------------
TToolForm* TMainForm::GetLoupeForm()
{
    if (!m_pLoupe)
    {
        m_pLoupe = new TLoupeForm(this);
        m_pLoupe->OnClose = HandleToolWindowClose;
        m_pLoupe->OnOptions = actOptionsExecute;
    }
    return m_pLoupe;
}

//---------------------------------------------------------------------------
TToolForm* TMainForm::GetInfoForm()
{
    if (!m_pInfo)
    {
        m_pInfo = new TInfoForm(this);
        m_pInfo->OnClose = HandleToolWindowClose;
        m_pInfo->OnKeyDown = HandleKeyDown;
        m_pInfo->OnOptions = actOptionsExecute;
    }
    return m_pInfo;
}

//---------------------------------------------------------------------------
TToolForm* TMainForm::GetBaseConvForm()
{
    if (!m_pBaseConv)
    {
        m_pBaseConv = new TBaseConvForm(this);
        m_pBaseConv->OnClose = HandleToolWindowClose;
        m_pBaseConv->OnKeyDown = HandleKeyDown;
        m_pBaseConv->OnOptions = actOptionsExecute;
    }
    return m_pBaseConv;
}

//---------------------------------------------------------------------------
TToolForm* TMainForm::GetCaptureForm()
{
    if (!m_pCapture)
    {
        m_pCapture = new TScreenGrabber(this);
        m_pCapture->OnClose = HandleToolWindowClose;
        m_pCapture->OnKeyDown = HandleKeyDown;
        m_pCapture->OnOptions = actOptionsExecute;
    }

    return m_pCapture;
}

//---------------------------------------------------------------------------
TToolForm* TMainForm::GetControlBar()
{
    if (!m_pControlBar)
    {
        m_pControlBar = new TControlForm(this);
        m_pControlBar->OnKeyDown = HandleKeyDown;
        m_pControlBar->OnMinimize = HandleControlBarMinimize;
        m_pControlBar->OnClose = HandleControlBarClose;
    }
    return m_pControlBar;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::actCommandExecute(TObject *Sender)
{
    // todo: find out why this is needed
    SetTopMost(m_bStayOnTop);

    TAction* pAction = reinterpret_cast<TAction*>(Sender);
    if (pAction)
    {
        TToolForm* pToolForm = NULL;

        if (pAction == actBaseConv)
        {
            pToolForm = GetBaseConvForm();
        }

        else if (pAction == actInfo)
        {
            pToolForm = GetInfoForm();
        }

        else if (pAction == actLoupe)
        {
            pToolForm = GetLoupeForm();
        }

        else if (pAction == actRuler)
        {
            pToolForm = GetRulerForm();
        }

        else if (pAction == actCapture)
        {
            pToolForm = GetCaptureForm();
        }

        else if (pAction == actControl)
        {
            if (m_UIMode == uiTrayApp)
            {
                pToolForm = GetControlBar();
            }
        }

        if (!pToolForm)
            return;

        // Toggle tool visibility
        if (pToolForm->Visible)
            pToolForm->Close();
        else
            pToolForm->Show();

        // Update UI
        pAction->Checked = pToolForm->Visible;
    }

    // Only enable the timer when there is something to keep track of
    // TODO: implement a timer that gets triggered only by an OnPaint event
    Timer->Enabled = TimerNeeded();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::actCaptureExecute(TObject *Sender)
{
    if (!m_pCapture)
    {
        GetCaptureForm();
    }

    m_pCapture->Show();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::actOptionsExecute(TObject *Sender)
{
    // See what page we need to show
    String sPageToShow = "";
    TToolForm *Tool = dynamic_cast<TToolForm*>(Sender);

    if (Tool)
    {
        if (Sender == this)
        {
            sPageToShow = "";
        }
        else if (Sender == m_pControlBar)
        {
            sPageToShow = "";
        }
        else if (Sender == m_pBaseConv)
        {
            sPageToShow = "Base Converter";
        }
        else if (Sender == m_pInfo)
        {
            sPageToShow = "Info";
        }
        else if (Sender == m_pLoupe)
        {
            sPageToShow = "Loupe";
        }
        else if (Sender == m_pRuler)
        {
            sPageToShow = "Ruler";
        }
        else if (Sender == m_pCapture)
        {
            sPageToShow = "Screen Grabber";
        }
    }

    // We disable the Hotkeys while this modal dialog is showing, we don't
    // want to steal keypresses while recording a new key combination
    m_HotkeyManager->DisableHotkeys();

    TToolOptionsDialog *OptionForm = new TToolOptionsDialog(this, sPageToShow);
    if (OptionForm->ShowModal() == mrOk)
    {
        // Changes are communicated through g_ToolOptions
        UpdateSettings();

        m_HotkeyManager->AssignHotkeys();

        if (m_pRuler)
        {
            m_pRuler->UpdateSettings();
        }
        if (m_pLoupe)
        {
            m_pLoupe->UpdateSettings();
        }
        if (m_pBaseConv)
        {
            m_pBaseConv->UpdateSettings();
        }
        if (m_pCapture)
        {
            m_pCapture->UpdateSettings();
        }
    }
    else
    {
        m_HotkeyManager->EnableHotkeys();
    }

    delete OptionForm;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::actExitExecute(TObject *Sender)
{
    Application->Terminate();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TrayMenuPopup(TObject *Sender)
{
    if (m_UIMode == uiNormal)
    {
        PopupMenu->Items->Items[0]->Visible = false;
        PopupMenu->Items->Items[1]->Visible = false;
    }
    else
    {
        PopupMenu->Items->Items[0]->Visible = true;
        PopupMenu->Items->Items[1]->Visible = true;
    }

}

//---------------------------------------------------------------------------

