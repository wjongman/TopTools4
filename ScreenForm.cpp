//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ScreenForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Tool"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TScreenForm::TScreenForm(TComponent* Owner)
: TToolForm(Owner, "capture")
{
    BorderStyle = bsNone;
    Color = clWhite;
    SetTransparency(true, 50);
    //SetColorKey(true, RGB(255,255, 255));
    DraggableForm = true;
    Cursor = crSizeAll;
    FSticky = false;

    Width = g_ToolOptions.Get(m_ToolName, "width", Width);
    Height = g_ToolOptions.Get(m_ToolName, "height", Height);

    m_Timer = new TTimer(this);
    m_Timer->Interval = 100; // milliseconds
    m_Timer->OnTimer = OnTimerTick;
    m_Timer->Enabled = true;

    m_ToolTip = new TToolTipForm(this);
}

//---------------------------------------------------------------------------
__fastcall TScreenForm::~TScreenForm()
{
    g_ToolOptions.Set(m_ToolName, "width", Width);
    g_ToolOptions.Set(m_ToolName, "height", Height);

    delete m_Timer;
    delete m_ToolTip;
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::MouseDown(TMouseButton Button,
                                       TShiftState Shift, int X, int Y)
{
    // Release stickiness
    FSticky = false;

    if (Button == mbLeft)
    // Start a drag-operation
    // todo: allow for moving and sizing with the arrow keys
    // todo: show tooltip that indicates position and size of selection
    // todo: show a zoomed closeup of the target area
    {
        m_MouseOldX = X;
        m_MouseOldY = Y;
    }
    else if (Button == mbRight && FOnRightButtonClick)
    // Signal right-button event
    {
        FOnRightButtonClick(this, Button, Shift, X, Y);
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::MouseMove(TShiftState Shift, int X, int Y)
{
    if (Shift.Contains(ssLeft) || FSticky)
    // We are dragging, move the form
    {
        Left += X - m_MouseOldX;
        Top  += Y - m_MouseOldY;

        m_ToolTip->Left = Left;
        m_ToolTip->Top = Top - 32;

        AnsiString sText;
        sText.printf("left: %d\r\n, top: %d\r\n, width: %d\r\n, height: %d", Left, Top, Width, Height);

        m_ToolTip->SetText(sText);
    }

}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::SetSticky(bool sticky)
{
    FSticky = sticky;
    m_Timer->Enabled = sticky;
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::OnTimerTick(TObject *Sender)
{
    if (FSticky)
    // Move the form
    {
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
//        Left += X - m_MouseOldX;
//        Top  += Y - m_MouseOldY;
    }
    else
    {
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
        //int scrollbarwidth = GetSystemMetrics(SM_CXHSCROLL);
        //int scrollbarheight = GetSystemMetrics(SM_CYHSCROLL);

        // Minimum space for showing resize cursors is 4 (by now),
        // maximum space is proportional to window dimensions
        int x_margin = 4 + Width / 10; // < 64 ? 5 : scrollbarwidth;
        int y_margin = 4 + Height/ 10; // < 64 ? 5 : scrollbarheight;

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

    // Center square
    //int x_center = (rc.Right - rc.Left)/ 2;
    //int y_center = (rc.Bottom - rc.Top)/ 2;
    //Canvas->FillRect(Rect(x_center - 10, y_center - 10, x_center + 10, y_center + 10));
    //Canvas->FillRect(Rect(rc.Left, rc.Top, rc.Right - 1, rc.Bottom - 1));

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

//        m_ToolTip->Left = ptMouse.x;
//        m_ToolTip->Top = ptMouse.y;
//        m_ToolTip->Show();
    }
}

//---------------------------------------------------------------------------


