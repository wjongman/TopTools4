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
: TToolForm(Owner, "capture"), InitCalled(false) //, m_ToolTip(NULL)
{
    BorderStyle = bsNone;
    Color = clWhite;
    SetTransparency(true, 50);
    //SetColorKey(true, RGB(255,255, 255));
    DraggableForm = true;
    Cursor = crSizeAll;
    FSticky = false;

    //m_ToolTip = new TToolTipForm(this);

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
    g_ToolOptions.Set(m_ToolName, "width", Width);
    g_ToolOptions.Set(m_ToolName, "height", Height);

    delete m_Timer;
    //delete m_ToolTip;
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

        //m_ToolTip->SetDisplayPosition(Left, Top, Width, Height);

        UpdateInfoLabel();
//        m_ToolTip->SetPosition(Left, Top);
    }

}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::SetSticky(bool sticky)
{
    FSticky = sticky;
    m_Timer->Enabled = sticky;
    InitTooltip("dit is een test");
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
    UpdateInfoLabel();
    Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::UpdateInfoLabel()
{
    //if (m_ToolTip)
    {
        String sText;
        //sText.printf("X:\t%d\tY:\t%d\tW:\t%d\tH:\t%d", Left, Top, Width, Height);
        sText.printf("X: %d\r\nY: %d\r\nW: %d\r\nH: %d", Left, Top, Width, Height);
        FInfoText = sText;
        InfoLabel->Caption = sText;
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

//     if (1)
//     {
//         //Canvas->Brush->Color = this->Color;
//         //Canvas->Brush->Style = bsSolid;
//         //  Canvas->FillRect(ClientRect);
//
//         RECT rcText = ClientRect;
//
//         rcText.left += 2;
//         rcText.top += 2;
//
//         //Canvas->Font->Color = clInfoText;
//         DrawText(Canvas->Handle, m_TipText.c_str(), -1, &rcText, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK | DT_EXPANDTABS);
//     }

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

        //m_ToolTip->Show();
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::FormCloseQuery(TObject *Sender,
                                            bool &CanClose)
{
    //m_ToolTip->Close();
    CanClose = true;
}


/*

//---------------------------------------------------------------------------
procedure ShowBalloonTip(Control: HWnd; Icon: integer; Title: pchar; Text: PWideChar; BackCL, TextCL: TColor);
const
 TOOLTIPS_CLASS = 'tooltips_class32';
 TTS_ALWAYSTIP = $01;
 TTS_NOPREFIX = $02;
 TTS_BALLOON = $40;
 TTF_SUBCLASS = $0010;
 TTF_TRANSPARENT = $0100;
 TTF_CENTERTIP = $0002;
 TTM_ADDTOOL = $0400 + 50;
 TTM_SETTITLE = (WM_USER + 32);
 ICC_WIN95_CLASSES = $000000FF;
type
 TOOLINFO = packed record
   cbSize: Integer;
   uFlags: Integer;
   hwnd: THandle;
   uId: Integer;
   rect: TRect;
   hinst: THandle;
   lpszText: PWideChar;
   lParam: Integer;
 end;
var
 hWndTip: THandle;
 ti: TOOLINFO;
 hWnd: THandle;
begin
 hWnd := Control;
 hWndTip := CreateWindow(TOOLTIPS_CLASS, nil,
   WS_POPUP or TTS_NOPREFIX or TTS_BALLOON or TTS_ALWAYSTIP,
   0, 0, 0, 0, hWnd, 0, HInstance, nil);
 if hWndTip <> 0 then
 begin
   SetWindowPos(hWndTip, HWND_TOPMOST, 0, 0, 0, 0,
     SWP_NOACTIVATE or SWP_NOMOVE or SWP_NOSIZE);
   ti.cbSize := SizeOf(ti);
   ti.uFlags := TTF_CENTERTIP or TTF_TRANSPARENT or TTF_SUBCLASS;
   ti.hwnd := hWnd;
   ti.lpszText := Text;
   Windows.GetClientRect(hWnd, ti.rect);
   SendMessage(hWndTip, TTM_SETTIPBKCOLOR, BackCL, 0);
   SendMessage(hWndTip, TTM_SETTIPTEXTCOLOR, TextCL, 0);
   SendMessage(hWndTip, TTM_ADDTOOL, 1, Integer(@ti));
   SendMessage(hWndTip, TTM_SETTITLE, Icon mod 4, Integer(Title));
 end;
end;
*/
void TScreenForm::InitTooltip(String sText)
{
    if (!InitCalled)
    {
        TOOLINFO ti;
        // CREATE A TOOLTIP WINDOW
        HWND hwndTooltip = CreateWindowEx(
                 WS_EX_TOPMOST,
                 TOOLTIPS_CLASS,
                 NULL,
                 TTS_NOPREFIX | TTS_ALWAYSTIP,
                 CW_USEDEFAULT,
                 CW_USEDEFAULT,
                 CW_USEDEFAULT,
                 CW_USEDEFAULT,
                 Application->MainForm->Handle,
                 0,
                 Application->Handle,
                 0);

        TSize TextSize = Canvas->TextExtent(sText);

        // INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE
        ti.cbSize = sizeof(TOOLINFO);
        ti.uFlags = TTF_TRACK;
        ti.hwnd = Handle;
        ti.hinst = Application->Handle;
        ti.uId = 0;
        ti.lpszText = sText.c_str();  // mustn't be "", otherwise the tooltip never appears
        // ToolTip control will cover the whole window
        ti.rect.left = 0;
        ti.rect.top = 0;
        ti.rect.right = 60;
        ti.rect.bottom = 60;

        SetWindowPos(
                hwndTooltip,
                HWND_TOPMOST,
                0,
                0,
                0,
                0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

//         int CRHMaxLen = strlen(BigString);
//         CDC *pDC = GetDC();
//         CSize vCSize = pDC->GetTextExtent(BigString, CRHMaxLen);
//         ReleaseDC(pDC);


        TPoint CursorPos;
        ::GetCursorPos(&CursorPos);

        // SEND AN ADDTOOL MESSAGE TO THE TOOLTIP CONTROL WINDOW
        SendMessage(Handle, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO) &ti);

        SendMessage(Handle, TTM_TRACKPOSITION, 0, (LPARAM)(DWORD) MAKELONG((CursorPos.x - TextSize.cx), (CursorPos.y - TextSize.cy)));
            // the "- vCSize.*"s are so that the tooltip is not obscured by the mouse pointer
        SendMessage(Handle, TTM_TRACKACTIVATE, true, (LPARAM)(LPTOOLINFO) &ti);

        InitCalled = true;
    }
}


