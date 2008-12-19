//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ScreenForm.h"
#include <wingdi.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Tool"
#pragma resource "*.dfm"

// Suppress warning concerning the MAKELONG macro
// W8084 Suggest parentheses to clarify precedence
#pragma warn -8084

//---------------------------------------------------------------------------
__fastcall TScreenForm::TScreenForm(TComponent* Owner)
: TToolForm(Owner, "capture"), InitCalled(false), m_TrackingMouse(false), m_hwndTooltip(NULL)
{
    BorderStyle = bsNone;
    Color = clWhite;
    SetTransparency(true, 50);

    DraggableForm = true;
    Cursor = crSizeAll;
    FSticky = false;

    Left = g_ToolOptions.Get(m_ToolName, "left", Left);
    Top = g_ToolOptions.Get(m_ToolName, "top", Top);
    Width = g_ToolOptions.Get(m_ToolName, "width", Width);
    Height = g_ToolOptions.Get(m_ToolName, "height", Height);

    m_Timer = new TTimer(this);
    m_Timer->Interval = 100; // milliseconds
    m_Timer->OnTimer = OnTimerTick;
    m_Timer->Enabled = true;
}

//---------------------------------------------------------------------------
__fastcall TScreenForm::~TScreenForm()
{
    g_ToolOptions.Set(m_ToolName, "left", Left);
    g_ToolOptions.Set(m_ToolName, "top", Top);
    g_ToolOptions.Set(m_ToolName, "width", Width);
    g_ToolOptions.Set(m_ToolName, "height", Height);

    delete m_Timer;

}

#if WITH_WINPROC
//---------------------------------------------------------------------------
void __fastcall TScreenForm::WndProc(Messages::TMessage &Message)
{
    switch (Message.Msg)
    {
    case WM_KEYDOWN:
        // Default action is to move the form
        bool resize = false;
        // Resize form when control key is down
        if (::GetKeyState(VK_CONTROL) &0x8000)
            resize = true;

        // Default increment is 1
        int delta = 1;
        // Increment is 10 when shift key is down
        if (::GetKeyState(VK_SHIFT) &0x8000)
            delta = 10;

        switch (Message.WParam)
        {
        case VK_LEFT:
            if (resize)
                Width -= delta;
            else
                Left -= delta;
            break;

        case VK_RIGHT:
            if (resize)
                Width += delta;
            else
                Left += delta;
            break;

        case VK_UP:
            if (resize)
                Height -= delta;
            else
                Top -= delta;
            break;

        case VK_DOWN:
            if (resize)
                Height += delta;
            else
                Top += delta;
            break;
        }

    }

    // Resume normal processing
    TToolForm::WndProc(Message);
}
#endif

//---------------------------------------------------------------------------
void __fastcall TScreenForm::FormShow(TObject *Sender)
{
//    FSticky = !g_ToolOptions.Get("capture", "rememberpos", false));

    if (FSticky)
    {
        // Show with form center below mouse
        TPoint ptMouse;
        GetCursorPos(&ptMouse);

        TRect rcClient = GetClientRect();
        int CenterX = rcClient.Width() / 2;
        int CenterY = rcClient.Height() / 2;

        Left = ptMouse.x - CenterX;
        Top = ptMouse.y - CenterY;

        POINT pt = ScreenToClient(ptMouse);
        m_MouseOldX = pt.x;
        m_MouseOldY = pt.y;
    }
#ifdef _DEBUG
//    m_hwndTooltip = CreateTrackingToolTip();
#endif
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::FormCloseQuery(TObject *Sender,
                                            bool &CanClose)
{
    if (m_hwndTooltip)
    {
        SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, (WPARAM)false, (LPARAM)&m_ToolInfo);
        m_TrackingMouse = false;

    }
    CanClose = true;
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::MouseDown(TMouseButton Button,
                                       TShiftState Shift, int X, int Y)
{
    // Release stickiness
    FSticky = false;

    if (Button == mbLeft)
    {
        // Left button down, start a drag-operation.
        // todo: allow for moving and sizing with the arrow keys
        // todo: show tooltip that indicates position and size of selection
        // todo: show a zoomed closeup of the target area
        m_MouseOldX = X;
        m_MouseOldY = Y;
    }
    else if (Button == mbRight && FOnRightButtonClick)
    {
        // Signal right-button event
        FOnRightButtonClick(this, Button, Shift, X, Y);
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::MouseMove(TShiftState Shift, int X, int Y)
{
    if (Shift.Contains(ssLeft) || FSticky)
    {
        // We are dragging, move the form
        Left += X - m_MouseOldX;
        Top  += Y - m_MouseOldY;

        UpdateInfoLabel();
    }

    if (!m_TrackingMouse)
    {
        if (m_hwndTooltip)
        {
            // Activate the ToolTip.
            ::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE,
                          (WPARAM)true, (LPARAM)&m_ToolInfo);
        }
        m_TrackingMouse = true;
    }

    // Make sure the mouse has actually moved, the
    // presence of the ToolTip causes Windows to
    // send the message continuously.
    static int oldX, oldY;

    if ((X != oldX) || (Y != oldY))
    {
        oldX = X;
        oldY = Y;

        if (m_hwndTooltip)
        {
            // Update tooltip position and text.
            String sCoords;
            sCoords.printf("X: %d  Y: %d  W: %d  H: %d", Left, Top, Width, Height);

            // Calculate the space required for our tooltip
            SIZE tipsize;
            HDC dcTooltip = ::GetDC(m_hwndTooltip);
            ::GetTextExtentPoint32(dcTooltip, sCoords.c_str(), sCoords.Length(), &tipsize);
            ::ReleaseDC(m_hwndTooltip, dcTooltip);

            // Set the tooltip text
            m_ToolInfo.lpszText = sCoords.c_str();
            ::SendMessage(m_hwndTooltip, TTM_SETTOOLINFO, 0, (LPARAM)&m_ToolInfo);

            // Set the tooltip position.
            // Tooltip shows above left-top of window unless it is
            // off-screen, in which case we position it at the screen edge
            POINT pt = { Left, Top - tipsize.cy - 1};

            // Stay on screen
            if (Left < 0)
            {
                pt.x = 0;
            }
            if (Left > Screen->DesktopWidth - tipsize.cx)
            {
                pt.x = Screen->DesktopWidth - tipsize.cx;
            }

            if (Top < tipsize.cy)
            {
                pt.y = Top + Height;
            }

            ::SendMessage(m_hwndTooltip, TTM_TRACKPOSITION,
                          0, (LPARAM)MAKELONG(pt.x, pt.y));
        }
    }
}

//---------------------------------------------------------------------------
HWND TScreenForm::CreateTrackingToolTip()
{
    // Create a tooltip window.
    HWND hwndToolTip = CreateWindowEx(
                                     WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                     WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                     CW_USEDEFAULT, CW_USEDEFAULT,
                                     CW_USEDEFAULT, CW_USEDEFAULT,
                                     Handle, NULL, Application->Handle, NULL);

    if (!hwndToolTip)
    {
        return NULL;
    }

    // Set up tool information.
    // In this case, the "tool" is the entire parent window.
    m_ToolInfo.cbSize = sizeof(TOOLINFO);
    m_ToolInfo.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
    m_ToolInfo.hwnd = Handle;
    m_ToolInfo.hinst = Application->Handle;
    m_ToolInfo.lpszText = "pText";
    m_ToolInfo.uId = (UINT_PTR)Handle;
    ::GetClientRect (Handle, &m_ToolInfo.rect);

    // Associate the ToolTip with the tool window.
    ::SendMessage(hwndToolTip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &m_ToolInfo);

    return hwndToolTip;
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::SetSticky(bool sticky)
{
    bool bUseSticky = true; //false;
    if (bUseSticky)
    {
        FSticky = sticky;
        m_Timer->Enabled = sticky;
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::OnTimerTick(TObject *Sender)
{
    if (FSticky)
    {
        // Move the form
        TPoint ptMouse;
        GetCursorPos(&ptMouse);

        TRect rcClient = GetClientRect();
        int CenterX = rcClient.Width() / 2;
        int CenterY = rcClient.Height() / 2;

        Left = ptMouse.x - CenterX;
        Top = ptMouse.y - CenterY;

        POINT pt = ScreenToClient(ptMouse);
        m_MouseOldX = pt.x;
        m_MouseOldY = pt.y;
    }
    else
    {
        // todo: Initially flash borders until mouseclick
        m_Timer->Enabled = false;
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::OnNCHitTest(TWMNCHitTest &Message)
{
    // Make our borders behave as resize area
    TCustomForm::Dispatch(&Message);

    if (Message.Result == HTCLIENT)
    {
        // Make maximum space for showing resize
        // cursors proportional to window dimensions.
        // Minimum space is 4 (for now).
        int x_margin = 4 + Width / 10;
        int y_margin = 4 + Height / 10;

        POINT pt = ScreenToClient(Point(Message.XPos, Message.YPos));

        TRect rcTopLeft = GetClientRect();
        rcTopLeft.Right = rcTopLeft.Left + x_margin;
        rcTopLeft.Bottom  = rcTopLeft.Top + y_margin;
        if (PtInRect(&rcTopLeft, pt))
        {
            Message.Result = HTTOPLEFT;
            return;
        }

        TRect rcTopRight = GetClientRect();
        rcTopRight.Left = rcTopRight.Right - x_margin;
        rcTopRight.Bottom  = rcTopRight.Top + y_margin;
        if (PtInRect(&rcTopRight, pt))
        {
            Message.Result = HTTOPRIGHT;
            return;
        }

        TRect rcBottomRight = GetClientRect();
        rcBottomRight.Left = rcBottomRight.Right - x_margin;
        rcBottomRight.Top  = rcBottomRight.Bottom - y_margin;
        if (PtInRect(&rcBottomRight, pt))
        {
            Message.Result = HTBOTTOMRIGHT;
            return;
        }

        TRect rcBottomLeft = GetClientRect();
        rcBottomLeft.Right = rcBottomLeft.Left + x_margin;
        rcBottomLeft.Top  = rcBottomLeft.Bottom - y_margin;
        if (PtInRect(&rcBottomLeft, pt))
        {
            Message.Result = HTBOTTOMLEFT;
            return;
        }

        TRect rcTop = GetClientRect();
        rcTop.Bottom  = rcTop.Top + y_margin;
        if (PtInRect(&rcTop, pt))
        {
            Message.Result = HTTOP;
            return;
        }

        TRect rcLeft = GetClientRect();
        rcLeft.Right = rcLeft.Left + x_margin;
        if (PtInRect(&rcLeft, pt))
        {
            Message.Result = HTLEFT;
            return;
        }

        TRect rcBottom = GetClientRect();
        rcBottom.Top = rcBottom.Bottom - y_margin;
        if (PtInRect(&rcBottom, pt))
        {
            Message.Result = HTBOTTOM;
            return;
        }

        TRect rcRight = GetClientRect();
        rcRight.Left = rcRight.Right - x_margin;
        if (PtInRect(&rcRight, pt))
        {
            Message.Result = HTRIGHT;
            return;
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::FormResize(TObject *Sender)
{
    UpdateInfoLabel();
    Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::UpdateInfoLabel()
{
    if (m_hwndTooltip)
    {
        // Update the tool-tip text.
        String sCoords;
        sCoords.printf("X: %d  Y: %d  W: %d  H: %d", Left, Top, Width, Height);

        m_ToolInfo.lpszText = sCoords.c_str();

        ::SendMessage(m_hwndTooltip, TTM_SETTOOLINFO, 0, (LPARAM)&m_ToolInfo);
    }
}

#define INC 4
//---------------------------------------------------------------------------
void __fastcall TScreenForm::FormPaint(TObject *Sender)
{
    TRect rc = GetClientRect();

    TColor OldPenColor = Canvas->Pen->Color;
    TColor OldBrushColor = Canvas->Brush->Color;
    TPenMode OldPenMode = Canvas->Pen->Mode;

    Canvas->Pen->Mode = pmCopy;
    Canvas->Pen->Color = clBlack;
    Canvas->Brush->Color = clInfoBk;

    // Borders
    Canvas->MoveTo(rc.Left, rc.Top);
    Canvas->LineTo(rc.Right - 1, rc.Top);
    Canvas->LineTo(rc.Right - 1, rc.Bottom - 1);
    Canvas->LineTo(rc.Left, rc.Bottom - 1);
    Canvas->LineTo(rc.Left, rc.Top);

    // Determine size of sizegrip
    int x_incr = GetSystemMetrics(SM_CXVSCROLL) / INC;
    int y_incr = GetSystemMetrics(SM_CXHSCROLL) / INC;

    // Top-Left sizegrip
    for (int i = 1; i <= INC; i++)
    {
        Canvas->MoveTo(rc.Left, rc.Top + i * y_incr);
        Canvas->LineTo(rc.Left + i * x_incr, rc.Top);
    }

    for (int i = 1; i <= INC; i++)
    {
        // Top-Right sizegrip
        Canvas->MoveTo(rc.Right, rc.Top + i * y_incr);
        Canvas->LineTo(rc.Right - i * x_incr, rc.Top);
    }

    for (int i = 1; i <= INC; i++)
    {
        // Bottom-Right sizegrip
        Canvas->MoveTo(rc.Right, rc.Bottom - i * y_incr);
        Canvas->LineTo(rc.Right - i * x_incr, rc.Bottom);
    }

    for (int i = 1; i <= INC; i++)
    {
        // Bottom-Left sizegrip
        Canvas->MoveTo(rc.Left, rc.Bottom - i * y_incr);
        Canvas->LineTo(rc.Left + i * x_incr, rc.Bottom);
    }

    // Restore original settings
    Canvas->Brush->Color = OldBrushColor;
    Canvas->Pen->Color = OldPenColor;
    Canvas->Pen->Mode = OldPenMode;
}

//---------------------------------------------------------------------------


