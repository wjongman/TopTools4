//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ScreenForm.h"
#include <wingdi.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Tool"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TScreenForm::TScreenForm(TComponent* Owner)
: TToolForm(Owner, "capture"), InitCalled(false), m_TrackingMouse(false), m_hwndTooltip(NULL)
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

     if (!m_TrackingMouse)
     // The mouse has just entered the window.
     {
//         // Request notification when the mouse leaves.
//         TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
//         tme.hwndTrack = Handle;
//         tme.dwFlags = TME_LEAVE;
//         TrackMouseEvent(&tme);
//
        // Activate the ToolTip.
        ::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE,
                    (WPARAM)true, (LPARAM)&m_ToolInfo);
        m_TrackingMouse = true;
    }

    // Make sure the mouse has actually moved. The presence of the ToolTip
    // causes Windows to send the message continuously.
    static int oldX, oldY;

    if ((X != oldX) || (Y != oldY))
    {
        oldX = X;
        oldY = Y;

        // Update the text.
        String sCoords;
        sCoords.printf("X: %d  Y: %d  W: %d  H: %d", Left, Top, Width, Height);

        // Calculate the space required for our tooltip
        SIZE tipsize;
        HDC dcTooltip = ::GetDC(m_hwndTooltip);
        ::GetTextExtentPoint32(dcTooltip, sCoords.c_str(), sCoords.Length(), &tipsize);
        ::ReleaseDC(m_hwndTooltip, dcTooltip);

//         String sTipCoords;
//         sTipCoords.printf("W: %d  H: %d", tipsize.cx, tipsize.cy);
//         InfoLabel->Caption = sTipCoords;

        // Update the tooltip text
        m_ToolInfo.lpszText = sCoords.c_str();
        ::SendMessage(m_hwndTooltip, TTM_SETTOOLINFO, 0, (LPARAM)&m_ToolInfo);

       // Position the ToolTip.
       // Tooltip shows above left-top of window unless it is
       // off-screen, in which case we position it at the screen edge
       POINT pt = { Left, Top - tipsize.cy };

        // Stay on screen
        if (Left < 0)
        {
            pt.x = 0;
        }
        if (Left > Screen->Width - tipsize.cx)
        {
            pt.x = Screen->Width - tipsize.cx;
        }

        if (Top < tipsize.cy)
        {
            pt.y = Top + Height - 1;
        }

        ::SendMessage(m_hwndTooltip, TTM_TRACKPOSITION,
                    0, (LPARAM)MAKELONG(pt.x, pt.y));
    }
}

//---------------------------------------------------------------------------
HWND TScreenForm::CreateTrackingToolTip()
{
    // Create a ToolTip.
    HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST,
        TOOLTIPS_CLASS, NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        Handle, NULL, Application->Handle, NULL);

    if (!hwndTT)
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
    ::SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &m_ToolInfo);
    return hwndTT;
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
    if (m_hwndTooltip)
    {
//         String sText;
//         //sText.printf("X:\t%d\tY:\t%d\tW:\t%d\tH:\t%d", Left, Top, Width, Height);
//         sText.printf("X: %d\r\nY: %d\r\nW: %d\r\nH: %d", Left, Top, Width, Height);
//         FInfoText = sText;
//         InfoLabel->Caption = sText;
        // Update the text.
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

        m_hwndTooltip = CreateTrackingToolTip();
        //m_ToolTip->Show();
    }
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
    //m_ToolTip->Close();
    CanClose = true;
}

#if WITH_WINPROC
//---------------------------------------------------------------------------
void __fastcall TScreenForm::WndProc(Messages::TMessage &Message)
{
    switch (Message.Msg)
    {
    case WM_KEYDOWN:
        break;
//     case WM_INITDIALOG:
//         InitCommonControls();
//         m_hwndTooltip = CreateTrackingToolTip(IDC_BUTTON1, hDlg, L"");
//         return TRUE;

//     case WM_MOUSELEAVE:
//         // The mouse pointer has left our window.
//         // Deactivate the ToolTip.
//         SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&m_ToolInfo);
//         m_TrackingMouse = FALSE;
//         return FALSE;
//
//     case WM_MOUSEMOVE:
//         static int oldX, oldY;
//         int newX, newY;
//
//         if (!m_TrackingMouse)
//         // The mouse has just entered the window.
//         {
//             // Request notification when the mouse leaves.
//             TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
//             tme.hwndTrack = Handle;
//             tme.dwFlags = TME_LEAVE;
//             TrackMouseEvent(&tme);
//
//             // Activate the ToolTip.
//             SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE,
//                         (WPARAM)TRUE, (LPARAM)&m_ToolInfo);
//             m_TrackingMouse = TRUE;
//         }
//
//         newX = GET_X_LPARAM(lParam);
//         newY = GET_Y_LPARAM(lParam);
//
//         // Make sure the mouse has actually moved. The presence of the ToolTip
//         // causes Windows to send the message continuously.
//         if ((newX != oldX) || (newY != oldY))
//         {
//             oldX = newX;
//             oldY = newY;
//
//             // Update the text.
//             WCHAR coords[12];
//             swprintf_s(coords, ARRAYSIZE(coords), L"%d, %d", newX, newY);
//             m_ToolInfo.lpszText = coords;
//             SendMessage(m_hwndTooltip, TTM_SETTOOLINFO, 0, (LPARAM)&m_ToolInfo);
//
//             // Position the ToolTip.
//             // The coordinates are adjusted so that the ToolTip does not
//             // overlap the mouse pointer.
//             POINT pt = { newX, newY};
//             ClientToScreen(hDlg, &pt);
//             SendMessage(m_hwndTooltip, TTM_TRACKPOSITION,
//                         0, (LPARAM)MAKELONG(pt.x + 10, pt.y - 20));
//         }
//         return FALSE;

    }

    TToolForm::WndProc(Message);
}
#endif

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
/*
void TScreenForm::InitTooltip(String sText)
{
    if (!InitCalled)
    {
        TOOLINFO ti;
        // CREATE A TOOLTIP WINDOW
        HWND hwndTooltip = CreateWindowEx(0, // WS_EX_TOPMOST,
                                          TOOLTIPS_CLASS,
                                          NULL,
                                          TTS_ALWAYSTIP | TTS_BALLOON, // | TTS_CLOSE, // TTS_NOPREFIX | TTS_ALWAYSTIP,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          Application->MainForm->Handle,
                                          0,
                                          Application->Handle,
                                          0);

        SetWindowPos(hwndTooltip,
                     HWND_TOPMOST,
                     0,
                     0,
                     0,
                     0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        TSize TextSize = Canvas->TextExtent(sText);

        // INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE
        ti.cbSize = sizeof(TOOLINFO);
        ti.uFlags = TTF_TRACK;
        ti.hwnd = Handle;
        ti.hinst = Application->Handle;
        ti.uId = 1; //Handle;
        ti.lpszText = sText.c_str();  // mustn't be "", otherwise the tooltip never appears
        // ToolTip control will cover the whole window
        ti.rect.left = ClientRect.left;
        ti.rect.top = ClientRect.top;
        ti.rect.right = ClientRect.right;
        ti.rect.bottom = ClientRect.bottom;

//         int CRHMaxLen = strlen(BigString);
//         CDC *pDC = GetDC();
//         CSize vCSize = pDC->GetTextExtent(BigString, CRHMaxLen);
//         ReleaseDC(pDC);


        TPoint CursorPos;
        ::GetCursorPos(&CursorPos);

        // SEND AN ADDTOOL MESSAGE TO THE TOOLTIP CONTROL WINDOW
        SendMessage(Handle, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO) &ti);

        //SetToolTipTitle(Handle, NULL, "dit is een test");
        String sTitle("dit is een test");
        SendMessage(Handle, TTM_SETTITLE, 0, (LPARAM) sTitle.c_str());

        SendMessage(Handle, TTM_TRACKPOSITION, 0, (LPARAM) MAKELONG((CursorPos.x - TextSize.cx), (CursorPos.y - TextSize.cy)));
        // the "- vCSize.*"s are so that the tooltip is not obscured by the mouse pointer
        SendMessage(Handle, TTM_TRACKACTIVATE, true, (LPARAM) &ti);

        InitCalled = true;
    }
}
*/
/*
#if 0
//m_ToolInfo is a global TOOLITEM structure.

HWND CreateTrackingToolTip(int toolID, HWND hDlg, WCHAR* pText)
{
    // Create a ToolTip.
    HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST,
        TOOLTIPS_CLASS, NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        hDlg, NULL, g_hInst,NULL);

    if (!hwndTT)
    {
        return NULL;
    }

    // Set up tool information.
    // In this case, the "tool" is the entire parent window.
    m_ToolInfo.cbSize = sizeof(TOOLINFO);
    m_ToolInfo.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
    m_ToolInfo.hwnd = hDlg;
    m_ToolInfo.hinst = g_hInst;
    m_ToolInfo.lpszText = pText;
    m_ToolInfo.uId = (UINT_PTR)hDlg;
    GetClientRect (hDlg, &m_ToolInfo.rect);

    // Associate the ToolTip with the tool window.
    SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &m_ToolInfo);
    return hwndTT;
}

 //m_hwndTooltip is a global HWND variable

case WM_INITDIALOG:
        InitCommonControls();
        m_hwndTooltip = CreateTrackingToolTip(IDC_BUTTON1, hDlg, L"");
        return TRUE;

case WM_MOUSELEAVE:
    // The mouse pointer has left our window.
    // Deactivate the ToolTip.
    SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&m_ToolInfo);
    m_TrackingMouse = FALSE;
    return FALSE;

case WM_MOUSEMOVE:
    static int oldX, oldY;
    int newX, newY;

    if (!m_TrackingMouse)
    // The mouse has just entered the window.
    {
        // Request notification when the mouse leaves.
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
        tme.hwndTrack = hDlg;
        tme.dwFlags = TME_LEAVE;
        TrackMouseEvent(&tme);

        // Activate the ToolTip.
        SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE,
            (WPARAM)TRUE, (LPARAM)&m_ToolInfo);
        m_TrackingMouse = TRUE;
    }

    newX = GET_X_LPARAM(lParam);
    newY = GET_Y_LPARAM(lParam);

    // Make sure the mouse has actually moved. The presence of the ToolTip
    // causes Windows to send the message continuously.
    if ((newX != oldX) || (newY != oldY))
    {
        oldX = newX;
        oldY = newY;

        // Update the text.
        WCHAR coords[12];
        swprintf_s(coords, ARRAYSIZE(coords), L"%d, %d", newX, newY);
        m_ToolInfo.lpszText = coords;
        SendMessage(m_hwndTooltip, TTM_SETTOOLINFO, 0, (LPARAM)&m_ToolInfo);

        // Position the ToolTip.
        // The coordinates are adjusted so that the ToolTip does not
        // overlap the mouse pointer.
        POINT pt = { newX, newY };
        ClientToScreen(hDlg, &pt);
        SendMessage(m_hwndTooltip, TTM_TRACKPOSITION,
            0, (LPARAM)MAKELONG(pt.x + 10, pt.y - 20));
    }
    return FALSE;

#endif
*/

