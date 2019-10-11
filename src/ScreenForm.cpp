//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

// Suppress warning concerning the MAKELONG macro
// W8084 Suggest parentheses to clarify precedence
#pragma warn -8084

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Tool"
#pragma resource "*.dfm"

#include "ScreenForm.h"

//---------------------------------------------------------------------------
__fastcall TScreenForm::TScreenForm(TComponent* Owner)
  : TToolForm(Owner, "capture"),
    m_pToolTip(NULL)
{
    BorderStyle = bsNone;
    Color = clWhite;
    SetTransparency(true, 50);

    SnapScreenEdges = false;
    Cursor = crSizeAll;

    Width = g_ToolOptions.Get(m_ToolName, "width", Width);
    Height = g_ToolOptions.Get(m_ToolName, "height", Height);

    m_pToolTip = new TToolTip(Handle);
}

//---------------------------------------------------------------------------
__fastcall TScreenForm::~TScreenForm()
{
    g_ToolOptions.Set(m_ToolName, "width", Width);
    g_ToolOptions.Set(m_ToolName, "height", Height);

    delete m_pToolTip;
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::WndProc(Messages::TMessage &Message)
{
    switch (Message.Msg)
    {
    case WM_SHOWWINDOW:
        UpdateToolTip();
        break;

    case WM_SETFOCUS:
        m_BorderColor = clFuchsia;//Navy;
        m_pToolTip->Show();
        Invalidate();
        break;

    case WM_KILLFOCUS:
        m_BorderColor = clGray;
        m_pToolTip->Hide();
        Invalidate();
        break;

    case WM_MOVE:
        UpdateToolTip();
        break;

    case WM_ENTERSIZEMOVE:
        OnEnterSizeMove(Message);
        break;

    case WM_SIZING:
        OnSizing(Message);
        break;

    case WM_MOVING:
        {
            // Constrain window to virtual desktop
            TRect* prcWindow = (TRect*)Message.LParam;

            if (prcWindow->left < Screen->DesktopLeft)
                OffsetRect(prcWindow, Screen->DesktopLeft - prcWindow->left, 0);

            if (prcWindow->top < Screen->DesktopTop)
                OffsetRect(prcWindow, 0, Screen->DesktopTop - prcWindow->top);

            if (prcWindow->right > Screen->DesktopWidth - Screen->DesktopLeft)
                OffsetRect(prcWindow, Screen->DesktopWidth - Screen->DesktopLeft - prcWindow->right, 0);

            if (prcWindow->bottom > Screen->DesktopHeight - Screen->DesktopTop)
                OffsetRect(prcWindow, 0, Screen->DesktopHeight - Screen->DesktopTop - prcWindow->bottom);

            break;
        }
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
        case VK_LEFT:
            resize ? Width -= delta : Left -= delta;
            UpdateToolTip();
            return;
        case VK_RIGHT:
            resize ? Width += delta : Left += delta;
            UpdateToolTip();
            return;
        case VK_UP:
            resize ? Height -= delta : Top -= delta;
            UpdateToolTip();
            return;
        case VK_DOWN:
            resize ? Height += delta : Top += delta;
            UpdateToolTip();
            return;
        }
    }
    // Let base class handle the rest
    TToolForm::WndProc(Message);
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::FormMouseDown(TObject *Sender, TMouseButton Button,
                                           TShiftState Shift, int X, int Y)
{
    if (Button == mbRight && FOnRightButtonClick)
    {
        // We don't want the tooltip to corrupt our snapshot
        m_pToolTip->Hide();

        // Signal right-button event
        FOnRightButtonClick(this, Button, Shift, X, Y);

        m_pToolTip->Show();
    }
}

//---------------------------------------------------------------------------
void TScreenForm::UpdateToolTip()
{
    if (m_pToolTip)
    {
        // Map (0,0) to top-left corner of virtual screen
        TPoint ptOrigin(Screen->DesktopLeft, Screen->DesktopTop);
        TRect rcNew(Left, Top, Left + Width, Top + Height);
        m_pToolTip->Update(rcNew, ptOrigin);
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

        if (::GetAsyncKeyState(MK_LBUTTON) < 0)
        {
            // We are being dragged
            Message.Result = HTCAPTION;
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

    Canvas->Pen->Color = m_BorderColor;
    Canvas->Pen->Mode = pmCopy;
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
void __fastcall TScreenForm::OnEnterSizeMove(TMessage &Message)
{
    // Record windowsize before sizing
    m_WidthAtStartOfSize = Width;
    m_HeightAtStartOfSize = Height;
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::OnSizing(TMessage &Message)
{
    // Constrain dimensions to square when shift key is down
    bool shift = ::GetKeyState(VK_SHIFT) & 0x8000;
    if (shift)
    {
        // https://stackoverflow.com/questions/20682975/resize-form-and-maintain-aspect-ratio
        TRect* prcNew = (TRect*)Message.LParam;
        int FAspectRatio = 1;
        switch (Message.WParam)
        {
        case WMSZ_LEFT:
        case WMSZ_RIGHT:
            prcNew->Bottom = prcNew->Top + (prcNew->Width() / FAspectRatio);
            break;
        case WMSZ_TOP:
        case WMSZ_BOTTOM:
            prcNew->Right = prcNew->Left + (prcNew->Height() * FAspectRatio);
            break;

        case WMSZ_TOPLEFT:
        case WMSZ_TOPRIGHT:
        case WMSZ_BOTTOMLEFT:
        case WMSZ_BOTTOMRIGHT:
            {
                bool SizeBasedOnWidth;
                if (prcNew->Width() > m_WidthAtStartOfSize)
                {
                    SizeBasedOnWidth = prcNew->Height() < MulDiv(m_HeightAtStartOfSize, prcNew->Width(), m_WidthAtStartOfSize);
                }
                else
                {
                    SizeBasedOnWidth = prcNew->Width() > MulDiv(m_WidthAtStartOfSize, prcNew->Height(), m_HeightAtStartOfSize);
                }
                if (SizeBasedOnWidth)
                {
                    int NewHeight = prcNew->Width() / FAspectRatio;
                    switch (Message.WParam)
                    {
                    case WMSZ_TOPLEFT:
                    case WMSZ_TOPRIGHT:
                        prcNew->Top = prcNew->Bottom - NewHeight;
                        break;
                    case WMSZ_BOTTOMLEFT:
                    case WMSZ_BOTTOMRIGHT:
                        prcNew->Bottom = prcNew->Top + NewHeight;
                        break;
                    }
                }
                else
                {
                    int NewWidth = prcNew->Height() * FAspectRatio;
                    switch (Message.WParam)
                    {
                    case WMSZ_TOPLEFT:
                    case WMSZ_BOTTOMLEFT:
                        prcNew->Left = prcNew->Right - NewWidth;
                        break;
                    case WMSZ_TOPRIGHT:
                    case WMSZ_BOTTOMRIGHT:
                        prcNew->Right = prcNew->Left + NewWidth;
                        break;
                    }
                }
            }
        }
    }
}



