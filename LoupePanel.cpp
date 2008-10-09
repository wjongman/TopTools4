//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LoupePanel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TLoupePanel::TLoupePanel(TComponent* Owner)
: TCustomPanel(Owner)
{
    m_zoom = 4;
    m_bIsFrozen = false;
    m_bIsLocked = false;
    m_bShowCrosshair = false;
    m_bShowGrid = false;
    m_bShowCenterbox = false;
    m_bMagnifySelf = false;
    m_bDragging = false;

    m_MaskBmp = NULL;
    m_DesktopCopyBmp = NULL;
    m_ScreenCopyBmp = NULL;
    m_BufferBmp = new Graphics::TBitmap();

    m_RefreshRate = 250;
    m_Timer = new TTimer(this);
    m_Timer->Interval = m_RefreshRate;
    m_Timer->OnTimer = TimerTick;
    m_Timer->Enabled = true;
}

//---------------------------------------------------------------------------
__fastcall TLoupePanel::~TLoupePanel()
{
    delete m_Timer;
    delete m_ScreenCopyBmp;
    delete m_BufferBmp;
    delete m_MaskBmp;
    delete m_DesktopCopyBmp;
//  ReleaseDC(NULL, m_DesktopDC);
}

//---------------------------------------------------------------------------
void TLoupePanel::OnNCHitTest(TWMNCHitTest &Message)
{
    TCustomPanel::Dispatch(&Message);

    // See if the mousepointer is above our size grip
    if (Message.Result == HTCLIENT)
    {
        TRect rc = GetClientRect();
        rc.Left = rc.Right - GetSystemMetrics(SM_CXHSCROLL);
        rc.Top  = rc.Bottom - GetSystemMetrics(SM_CYVSCROLL);

        POINT pt = ScreenToClient(Point(Message.XPos, Message.YPos));

        if (PtInRect(&rc, pt))
        {
            Message.Result = HTBOTTOMRIGHT;
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::Resize()
{
    int srcX = ClientWidth / (2 * m_zoom);
    int srcY = ClientHeight / (2 * m_zoom);

    // To compensate for round-off differences we make the
    // source rectangle 2 pixels larger then calculated
    m_rcSource = Rect(-srcX, -srcY, srcX + 2, srcY + 2);

    // Calculate the buffer bitmap dimensions.
    // Because we made the source rectangle bigger than calculated
    // the bitmap will be guaranteed to cover the entire canvas
    // of our panel, overlap will be clipped by the BitBlt function
    m_BufferBmp->Width = m_rcSource.Width() * m_zoom;
    m_BufferBmp->Height = m_rcSource.Height() * m_zoom;

    // Avoid redundant calls to InitMask(),
    // mask is created during first call of Paint()
    if (m_MaskBmp)
    {
        RenderMask(m_MaskBmp);
    }

    Invalidate();
}


//----------------------------------------------------------------------------
void __fastcall TLoupePanel::SetRefreshRate(int rate)
{
    m_RefreshRate = rate;
    m_Timer->Interval = rate;
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::TimerTick(TObject *Sender)
{
    // Timer will be re-enabled during next call of Paint()
    m_Timer->Enabled = false;

    UpdateView();
}

//----------------------------------------------------------------------------
void __fastcall TLoupePanel::SetZoom(int zoom)
{
    if (zoom < 1 || zoom > MAXZOOM)
        return;

    m_zoom = zoom;

    Resize();
}

//----------------------------------------------------------------------------
void __fastcall TLoupePanel::ZoomIn()
{
    if (m_zoom < MAXZOOM)
        m_zoom++;

    Resize();
}

//----------------------------------------------------------------------------
void __fastcall TLoupePanel::ZoomOut()
{
    if (m_zoom > 1)
        m_zoom--;

    Resize();
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::MoveLockPos(int dx, int dy)
{
    m_ptLockPos.x += dx;
    m_ptLockPos.y += dy;
    Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::UpdateView()
{
    if (m_bIsLocked)
    {
        m_ptViewCenter = m_ptLockPos;
        Invalidate();
    }
    else
    {
        ::GetCursorPos(&m_ptViewCenter);
        Invalidate();
    }
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::UpdateView(const TPoint& ptMouse)
{
    if (m_bIsLocked)
    {
        m_ptViewCenter = m_ptLockPos;
        Invalidate();
    }
    else
    {
        m_ptViewCenter = ptMouse;
        Invalidate();
    }
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::Paint(void)
{
    if (!m_bMagnifySelf && PtInRect(&ClientRect, ScreenToClient(m_ptViewCenter)))
    {
        // Avoid magnifying our own canvas, just black out
        Canvas->Brush->Color = clBlack;
        Canvas->FillRect(ClientRect);
    }
    else
    {
        // Position the screen source-rectangle
        int srcX = m_rcSource.left + m_ptViewCenter.x;
        int srcY = m_rcSource.top + m_ptViewCenter.y;

        // Start with an all black buffer bitmap
        m_BufferBmp->Canvas->Brush->Color = clBlack;
        m_BufferBmp->Canvas->FillRect(ClientRect);

        // Have a sized copy of the screen into our buffer bitmap
        HDC TargetDC = m_BufferBmp->Canvas->Handle;

        if (m_bIsFrozen)
        {
            if (!m_DesktopCopyBmp)
            {
                CaptureDesktopBmp();
            }

            HDC SourceDC = m_DesktopCopyBmp->Canvas->Handle;
            StretchBlt(TargetDC, 0, 0, m_BufferBmp->Width, m_BufferBmp->Height,
                       SourceDC, srcX, srcY, m_rcSource.Width(), m_rcSource.Height(),
                       SRCCOPY); // SRCINVERT | CAPTUREBLT);
        }
        else
        {
            HDC SourceDC = GetDC(NULL);
            StretchBlt(TargetDC, 0, 0, m_BufferBmp->Width, m_BufferBmp->Height,
                       SourceDC, srcX, srcY, m_rcSource.Width(), m_rcSource.Height(),
                       SRCCOPY); // SRCINVERT | CAPTUREBLT);
            ReleaseDC(NULL, SourceDC);
        }

        if (!m_MaskBmp)
        {
            m_MaskBmp = new Graphics::TBitmap();
            RenderMask(m_MaskBmp);
        }

        // Overlay the Grid-mask
        TRect r = Rect(0, 0, m_BufferBmp->Width, m_BufferBmp->Height);
        m_BufferBmp->Canvas->CopyMode = cmSrcInvert;
        m_BufferBmp->Canvas->CopyRect(r, m_MaskBmp->Canvas, r);

        // Bitmap is now ready to be displayed, copy it to the screen
        Canvas->CopyRect(ClientRect, m_BufferBmp->Canvas, ClientRect);
    }

    //if (!m_bIsFrozen)
    {
        m_Timer->Enabled = true;
    }
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::OldPaint(void)
{
    if (!m_bMagnifySelf && PtInRect(&ClientRect, ScreenToClient(m_ptViewCenter)))
    {
        // Avoid magnifying our own canvas, just black out
        Canvas->Brush->Color = clBlack;
        Canvas->FillRect(ClientRect);
    }
    else
    {
        // Position the screen source-rectangle
        int srcX = m_rcSource.left + m_ptViewCenter.x;
        int srcY = m_rcSource.top + m_ptViewCenter.y;

        // Have a sized copy of the screen into our buffer bitmap
        HDC BitmapDC = m_BufferBmp->Canvas->Handle;
        HDC DesktopDC = GetDC(NULL);

        StretchBlt(BitmapDC, 0, 0, m_BufferBmp->Width, m_BufferBmp->Height,
                   DesktopDC, srcX, srcY, m_rcSource.Width(), m_rcSource.Height(),
                   SRCCOPY); // SRCINVERT | CAPTUREBLT);

        ReleaseDC(NULL, DesktopDC);

        if (!m_MaskBmp)
        {
            m_MaskBmp = new Graphics::TBitmap();
            RenderMask(m_MaskBmp);
        }

        // Overlay the Grid-mask
        TRect r = Rect(0, 0, m_BufferBmp->Width, m_BufferBmp->Height);
        m_BufferBmp->Canvas->CopyMode = cmSrcInvert;
        m_BufferBmp->Canvas->CopyRect(r, m_MaskBmp->Canvas, r);

        // Bitmap is now ready to be displayed, copy it to the screen
        Canvas->CopyRect(ClientRect, m_BufferBmp->Canvas, ClientRect);
    }

    m_Timer->Enabled = true;
}

//---------------------------------------------------------------------------
Graphics::TBitmap* __fastcall TLoupePanel::GetBitmap()
{
    // Lazy-initialize a bitmap to hold the screen-copy
    if (!m_ScreenCopyBmp)
    {
        m_ScreenCopyBmp = new Graphics::TBitmap();
    }

    m_ScreenCopyBmp->Width = m_BufferBmp->Width;
    m_ScreenCopyBmp->Height = m_BufferBmp->Height;

    // Position the screen source-rectangle
    int srcX = m_rcSource.left + m_ptViewCenter.x;
    int srcY = m_rcSource.top + m_ptViewCenter.y;

//     // Have a sized copy of the screen into our bitmap
//     HDC BitmapDC = m_ScreenCopyBmp->Canvas->Handle;
//     HDC DesktopDC = GetDC(NULL);
    // Have a sized copy of the screen into our buffer bitmap
    HDC TargetDC = m_ScreenCopyBmp->Canvas->Handle;

    if (m_bIsFrozen)
    {
        if (!m_DesktopCopyBmp)
        {
            CaptureDesktopBmp();
        }

        HDC SourceDC = m_DesktopCopyBmp->Canvas->Handle;

        StretchBlt(TargetDC, 0, 0, m_BufferBmp->Width, m_BufferBmp->Height,
                   SourceDC, srcX, srcY, m_rcSource.Width(), m_rcSource.Height(),
                   SRCCOPY);
    }
    else
    {
        HDC SourceDC = GetDC(NULL);

        StretchBlt(TargetDC, 0, 0, m_BufferBmp->Width, m_BufferBmp->Height,
                   SourceDC, srcX, srcY, m_rcSource.Width(), m_rcSource.Height(),
                   SRCCOPY);

        ReleaseDC(NULL, SourceDC);
    }

//     StretchBlt(BitmapDC, 0, 0, m_ScreenCopyBmp->Width, m_ScreenCopyBmp->Height,
//                DesktopDC, srcX, srcY, m_rcSource.Width(), m_rcSource.Height(),
//                SRCCOPY);
//
//     ReleaseDC(NULL, DesktopDC);

    return m_ScreenCopyBmp;
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::CaptureDesktopBmp()
{
    // Lazy-initialize a bitmap to hold a copy of the desktop
    if (!m_DesktopCopyBmp)
    {
        m_DesktopCopyBmp = new Graphics::TBitmap();
    }

    m_DesktopCopyBmp->Width = Screen->DesktopWidth;
    m_DesktopCopyBmp->Height = Screen->DesktopHeight;

    // Have a copy of the desktop into our bitmap
    HDC BitmapDC = m_DesktopCopyBmp->Canvas->Handle;
    HDC DesktopDC = GetDC(NULL);

    BitBlt(BitmapDC, 0, 0, m_DesktopCopyBmp->Width, m_DesktopCopyBmp->Height,
           DesktopDC, Screen->DesktopLeft, Screen->DesktopTop,
           SRCCOPY | CAPTUREBLT);

    ReleaseDC(NULL, DesktopDC);
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::ReleaseDesktopBmp()
{
    // Get rid of the bitmap that holds a copy of the desktop
    if (m_DesktopCopyBmp)
    {
        delete m_DesktopCopyBmp;
        m_DesktopCopyBmp = NULL;
    }
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::RenderMask(Graphics::TBitmap* TargetBmp)
{
    // Have a mask with crosshair and grid etc. already drawn on it
    TRect rcMask = Rect(0, 0, m_BufferBmp->Width, m_BufferBmp->Height);
    TargetBmp->Width = rcMask.Width();
    TargetBmp->Height = rcMask.Height();

    // The background is all black
    TargetBmp->Canvas->CopyMode = cmBlackness;
    TargetBmp->Canvas->CopyRect(rcMask, TargetBmp->Canvas, rcMask);
    TargetBmp->Canvas->CopyMode = cmSrcCopy;

    // The rest is drawn in white
    TargetBmp->Canvas->Pen->Color = clWhite;
    TargetBmp->Canvas->Pen->Mode = pmNot;

    if (m_bShowCrosshair)
        DrawCrosshair(TargetBmp);

    if (m_bShowCenterbox)
        DrawCenterbox(TargetBmp);

    if (m_bShowGrid && m_zoom > 2)
        DrawGrid(TargetBmp);

    // Draw a resize-handle in the lower-right corner
    DrawSizeGrip(TargetBmp);
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::DrawSizeGrip(Graphics::TBitmap* TargetBmp)
{
    TRect rc = GetClientRect();
    int Right = rc.right;
    int Bottom = rc.bottom;

    TColor OldPenColor = TargetBmp->Canvas->Pen->Color;
    TPenMode OldPenMode = TargetBmp->Canvas->Pen->Mode;

    int i = GetSystemMetrics(SM_CXHSCROLL) - 3;
    while (i > 0)
    {
        TargetBmp->Canvas->Pen->Mode = pmCopy;
        TargetBmp->Canvas->Pen->Color = clBtnHighlight;
        TargetBmp->Canvas->MoveTo(Right - i, Bottom);
        TargetBmp->Canvas->LineTo(Right, Bottom - i);

        TargetBmp->Canvas->Pen->Color = clBtnShadow;
        TargetBmp->Canvas->MoveTo(Right + 1 - i, Bottom);
        TargetBmp->Canvas->LineTo(Right, Bottom + 1 - i);

        TargetBmp->Canvas->Pen->Mode = pmNotCopy;
        TargetBmp->Canvas->Pen->Color = clBtnHighlight;
        TargetBmp->Canvas->MoveTo(Right + 2 - i, Bottom);
        TargetBmp->Canvas->LineTo(Right, Bottom + 2 - i);

        i -= 4;
    }

    TargetBmp->Canvas->Pen->Color = OldPenColor;
    TargetBmp->Canvas->Pen->Mode = OldPenMode;

/*
  // Another option would be the DrawFrameControl API function,
  // but for now I didn't find a way to make it draw transparent..
  TRect rc = GetClientRect();
  rc.Left = rc.Right  - GetSystemMetrics(SM_CXHSCROLL);
  rc.Top  = rc.Bottom - GetSystemMetrics(SM_CYVSCROLL);
  DrawFrameControl(Canvas->Handle, &rc, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
*/
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::DrawCrosshair(Graphics::TBitmap* TargetBmp)
{
    int centerX = TargetBmp->Width / 2;
    int centerY = TargetBmp->Height / 2;

    // Horizontal Right Crosshair
    TargetBmp->Canvas->MoveTo(0, centerY);
    TargetBmp->Canvas->LineTo(TargetBmp->Width, centerY);

    // Vertical Lower Crosshair
    TargetBmp->Canvas->MoveTo(centerX, 0);
    TargetBmp->Canvas->LineTo(centerX, TargetBmp->Height);

    centerX -= m_zoom;
    centerY -= m_zoom;

    // Horizontal Left Crosshair
    TargetBmp->Canvas->MoveTo(0, centerY);
    TargetBmp->Canvas->LineTo(TargetBmp->Width, centerY);

    // Vertical Upper Crosshair
    TargetBmp->Canvas->MoveTo(centerX, 0);
    TargetBmp->Canvas->LineTo(centerX, TargetBmp->Height);
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::DrawTickMarks(Graphics::TBitmap* TargetBmp)
{
    // Put a tickmark at every 5 pixels
    int centerX = (TargetBmp->Width - m_zoom) / 2;
    int centerY = (TargetBmp->Height - m_zoom) / 2;

    for (int x = centerX; x < TargetBmp->Width; x += (m_zoom * 5))
    {
        TargetBmp->Canvas->MoveTo(x, centerY);
        TargetBmp->Canvas->LineTo(x+1, centerY+1);
    }
    for (int x = centerX; x > 0; x -= (m_zoom * 5))
    {
        TargetBmp->Canvas->MoveTo(x, centerY);
        TargetBmp->Canvas->LineTo(x+1, centerY+1);
    }

    for (int y = centerY; y < TargetBmp->Height; y += (m_zoom * 5))
    {
        TargetBmp->Canvas->MoveTo(centerX, y);
        TargetBmp->Canvas->LineTo(centerX+1, y+1);
    }
    for (int y = centerY; y > 0; y -= (m_zoom * 5))
    {
        TargetBmp->Canvas->MoveTo(centerX, y);
        TargetBmp->Canvas->LineTo(centerX+1, y+1);
    }
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::DrawCenterbox(Graphics::TBitmap* TargetBmp)
{
    int centerX = TargetBmp->Width / 2;
    int centerY = TargetBmp->Height / 2;

    TargetBmp->Canvas->MoveTo(centerX, centerY);
    TargetBmp->Canvas->LineTo(centerX - m_zoom, centerY);
    TargetBmp->Canvas->LineTo(centerX - m_zoom, centerY - m_zoom);
    TargetBmp->Canvas->LineTo(centerX, centerY - m_zoom);
    TargetBmp->Canvas->LineTo(centerX, centerY);

    // Put a tickmark at every 5 pixels
    DrawTickMarks(TargetBmp);
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::DrawGrid(Graphics::TBitmap* TargetBmp)
{
    COLORREF crColor =  RGB(255, 255, 255);
    for (int x = 0; x < TargetBmp->Width; x += m_zoom)
        for (int y = 0; y < TargetBmp->Height; y += m_zoom)
            SetPixelV(TargetBmp->Canvas->Handle, x, y, crColor);
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::ToggleCrosshair()
{
    m_bShowCrosshair = !m_bShowCrosshair;
    DrawCrosshair(m_MaskBmp);
    Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::ToggleCenterbox()
{
    m_bShowCenterbox = !m_bShowCenterbox;
    DrawCenterbox(m_MaskBmp);
    Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::ToggleGrid()
{
    m_bShowGrid = !m_bShowGrid;
    DrawGrid(m_MaskBmp);
    Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::ToggleFrozen()
{
    m_bIsFrozen = !m_bIsFrozen;

    if (m_bIsFrozen)
    {
        // Save the entire desktop in a bitmap like "Print Screen" does
        CaptureDesktopBmp();
    }
    else
    {
        ReleaseDesktopBmp();
    }

    Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::ToggleLocked()
{
    m_bIsLocked = !m_bIsLocked;

    if (m_bIsLocked)
    {
        // Save the lock position
        GetCursorPos(&m_ptLockPos);
    }

    Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// Support for dragging the locked or frozen view
/////////////////////////////////////////////////////////////////////////////
void __fastcall TLoupePanel::MouseDown(TMouseButton Button, TShiftState Shift,
                                       int X, int Y)
{
    if (Button == mbLeft && (m_bIsLocked || m_bIsFrozen))
    {
        m_ptDragStart = Point(X, Y);
        m_bDragging = true;
    }
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::MouseMove(TShiftState Shift, int X, int Y)
{
    if (m_bDragging && Shift.Contains(ssLeft))
    {
        int deltaX = X - m_ptDragStart.x;
        int deltaY = Y - m_ptDragStart.y;

        if (abs(deltaX) > m_zoom)
        {
            m_ptLockPos.x -= deltaX / m_zoom;
            m_ptDragStart.x = X;
        }
        if (abs(deltaY) > m_zoom)
        {
            m_ptLockPos.y -= deltaY / m_zoom;
            m_ptDragStart.y = Y;
        }
        Invalidate();
    }
}

//---------------------------------------------------------------------------
void __fastcall TLoupePanel::MouseUp(TMouseButton Button, TShiftState Shift,
                                     int X, int Y)
{
    if (m_bDragging)
    {
        m_bDragging = false;
    }
}

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TLoupePanel *)
{
    new TLoupePanel(NULL);
}
//---------------------------------------------------------------------------
namespace Loupepanel
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(TLoupePanel)};
        RegisterComponents("WComponents", classes, 0);
    }
}
//---------------------------------------------------------------------------
