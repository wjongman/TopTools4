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
: TToolForm(Owner, "capture"), InitCalled(false), m_TrackingMouse(false), m_pToolTip(NULL)//, m_hwndTooltip(NULL)
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
    m_Timer->Enabled = false;
}

//---------------------------------------------------------------------------
__fastcall TScreenForm::~TScreenForm()
{
    g_ToolOptions.Set(m_ToolName, "left", Left);
    g_ToolOptions.Set(m_ToolName, "top", Top);
    g_ToolOptions.Set(m_ToolName, "width", Width);
    g_ToolOptions.Set(m_ToolName, "height", Height);

    delete m_Timer;
    delete m_pToolTip;
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::WndProc(Messages::TMessage &Message)
{
    switch (Message.Msg)
    {
// //    case WM_NCMOUSELEAVE:
//     case WM_MOUSELEAVE:
//         m_TrackingMouse = false;
//         UpdateToolTip();
//         break;
//
//     case WM_NCMOUSEHOVER:
// //    case WM_MOUSEHOVER:
//     {
//         switch (Message.WParam)
//         {
//         case HTTOPLEFT:
//         }
//
//     }
//         m_TrackingMouse = true;
//         UpdateToolTip();
//         break;

    case WM_KEYDOWN:

        // Default action is to move the form
        bool resize = false;

        // Resize form when control key is down
        if (::GetKeyState(VK_CONTROL) & 0x8000)
            resize = true;

        // Default increment is 1
        int delta = 1;

        // Increment is 10 when shift key is down
        if (::GetKeyState(VK_SHIFT) & 0x8000)
            delta = 10;

        switch (Message.WParam)
        {
        case VK_LEFT:  resize ? Width -= delta : Left -= delta; UpdateToolTip(); return;
        case VK_RIGHT: resize ? Width += delta : Left += delta; UpdateToolTip(); return;
        case VK_UP:    resize ? Height -= delta : Top -= delta; UpdateToolTip(); return;
        case VK_DOWN:  resize ? Height += delta : Top += delta; UpdateToolTip(); return;

        case VK_ESCAPE: Close(); return;
        }

    }

    // Resume normal processing
    TToolForm::WndProc(Message);
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::FormShow(TObject *Sender)
{
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

    if (!m_pToolTip)
    {
        m_pToolTip = new TToolTip(Handle);
    }

    m_TrackingMouse = true;
    m_Timer->Enabled = true;
//    m_pToolTip->Show();
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::FormCloseQuery(TObject *Sender,
                                            bool &CanClose)
{
    m_pToolTip->Hide();
    m_Timer->Enabled = false;

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

        UpdateToolTip();
    }

//     if (!m_TrackingMouse)
//     {
//         TRACKMOUSEEVENT tme;
//         tme.cbSize = sizeof(TRACKMOUSEEVENT);
//         tme.dwFlags = TME_HOVER | TME_NONCLIENT; // | TME_LEAVE
//         tme.hwndTrack = Handle;
//         if (::_TrackMouseEvent(&tme))
//         {
//             m_TrackingMouse = true;
//         }
//     }

    // Make sure the mouse has actually moved, the
    // presence of the ToolTip causes Windows to
    // send the message continuously.
    static int oldX, oldY;

    if ((X != oldX) || (Y != oldY))
    {
        oldX = X;
        oldY = Y;

        UpdateToolTip();
    }
}

//---------------------------------------------------------------------------
void TScreenForm::UpdateToolTip()
{
    if (m_pToolTip)
    {
        if (m_TrackingMouse)
        {
            m_pToolTip->Update(TRect(Left, Top, Left + Width,  Top + Height));
        }
        else
        {
            m_pToolTip->Hide();
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::SetSticky(bool sticky)
{
//    bool bUseSticky = true; //false;
//    if (bUseSticky)
    {
        FSticky = sticky;
//        m_Timer->Enabled = sticky;
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::OnTimerTick(TObject *Sender)
{
    TPoint ptMouse;
    GetCursorPos(&ptMouse);
    TRect rcClient = GetClientRect();

    if (FSticky)
    {
        int CenterX = rcClient.Width() / 2;
        int CenterY = rcClient.Height() / 2;

        // Move the form
        Left = ptMouse.x - CenterX;
        Top = ptMouse.y - CenterY;

        POINT pt = ScreenToClient(ptMouse);
        m_MouseOldX = pt.x;
        m_MouseOldY = pt.y;
    }
    //else
    {
        // Hide tooltip when mouse is not above the form
        POINT pt = ScreenToClient(ptMouse);
        if (PtInRect(&rcClient, pt))
        {
            m_TrackingMouse = true;
        }
        else
        {
            m_TrackingMouse = false;
        }

        UpdateToolTip();



        // todo: Initially flash borders until mouseclick
//        m_Timer->Enabled = false;
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
    UpdateToolTip();
    Invalidate();
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


