//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdlib.h>
#include "Ruler.h"
#include "Transparency.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Tool"
#pragma resource "*.dfm"

const int RULERWIDTH = 50;

//---------------------------------------------------------------------------
__fastcall TRulerForm::TRulerForm(TComponent* Owner)
  : TToolForm(Owner, "ruler"),
    m_inMenu(false),
    m_inSizeMove(false)
{
    // Load the custom cursors
    Screen->Cursors[crHorUp] = LoadCursor((void*)HInstance, "HORUP");
    Screen->Cursors[crHorDown] = LoadCursor((void*)HInstance, "HORDOWN");
    Screen->Cursors[crVertLeft] = LoadCursor((void*)HInstance, "VERTLEFT");
    Screen->Cursors[crVertRight] = LoadCursor((void*)HInstance, "VERTRIGHT");

    // Set scaled to false so we look decent also with large fonts
    Scaled = false;

    m_RulerColor = clWhite;
    Color = m_RulerColor;

    m_breadth = RULERWIDTH;
    m_center = RULERWIDTH / 2;

    SnapScreenEdges = false;

    // Set up bitmap for buffered drawing
    m_pBufferBmp = new Graphics::TBitmap;
    m_pBufferBmp->Canvas->Font->Name = "Microsoft Sans Serif";
    m_pBufferBmp->Canvas->Font->Height = -9;
}

//---------------------------------------------------------------------------
__fastcall TRulerForm::~TRulerForm()
{
    delete m_pBufferBmp;
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::WndProc(Messages::TMessage &Message)
{
    switch (Message.Msg)
    {
    case WM_ERASEBKGND:
        // Don't erase, to avoid flicker
        Message.Result = 1;
        return;

    case WM_SETCURSOR:
        SetCursorShape();
        break;
    }
    // Let base class handle the rest
    TToolForm::WndProc(Message);
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::FormCreate(TObject *Sender)
{
    UpdateSettings();
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::UpdateSettings()
{
    m_length = g_ToolOptions.Get(m_ToolName, "length", 1024);
    m_isTransparent = g_ToolOptions.Get(m_ToolName, "transparent", false);
    m_transparency = g_ToolOptions.Get(m_ToolName, "transparency", 50);
    SetTransparency(m_isTransparent, m_transparency);

    m_useArrowNudge = g_ToolOptions.Get(m_ToolName, "arrownudge", true);
    m_isHorizontal = g_ToolOptions.Get(m_ToolName, "horizontal", true);
    if (m_isHorizontal)
    {
        Width = m_length;
        Height = m_breadth;
        m_pBufferBmp->Width = m_length;
        m_pBufferBmp->Height = m_breadth;
    }
    else
    {
        Width = m_breadth;
        Height = m_length;
        m_pBufferBmp->Width = m_breadth;
        m_pBufferBmp->Height = m_length;
    }
    Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::TimerEvent(TPoint ptMouse)
{
    if (m_inMenu || m_inSizeMove)
    {
        // We are processing a popupmenu , bail out..
        return;
    }

    if (m_mouseHover)
    {
        // Get mouse position in client coordinates
        POINT ptRelMouse = ScreenToClient(ptMouse);
        // Get the clientwindow dimensions
        RECT rcClient = ClientRect;
        // See if mousepointer is inside our window
        if (!PtInRect(&rcClient, ptRelMouse))
        {
            // Hide the coordinate window
            m_mouseHover = false;
            Invalidate();
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::FormMouseDown(TObject *Sender, TMouseButton Button,
                                          TShiftState Shift, int X, int Y)
{
    if (Button == mbLeft)
    // Start a drag-operation
    {
        m_ptMouse.x = X;
        m_ptMouse.y = Y;
    }

    else if (Button == mbRight)
    {
        // Popup-menu request
        m_inMenu = true;
        POINT ptAbs = ClientToScreen(Point(X, Y));
        RulerMenu->Popup(ptAbs.x, ptAbs.y);
        m_inMenu = false;
    }
    else if (Button == mbMiddle)
    {
        ToggleOrientation();
    }
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::FormMouseMove(TObject *Sender, TShiftState Shift,
                                          int X, int Y)
{
    // Bail-out if we are processing the popup-menu
    if (m_inMenu || m_inSizeMove)
    {
        return;
    }
    if (Shift.Contains(ssLeft))
    // We are dragging, move the ruler and the coordinate window
    {
        Move(X - m_ptMouse.x, Y - m_ptMouse.y);
    }
    else
    // Normal mouse movement, update coordinates
    {
        // Get mouse position in screen coordinates
        m_ptMouse = TPoint(X, Y);
        m_mouseHover = true;
        Invalidate();
    }
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::FormPaint(TObject *Sender)
{
    TCanvas* canvas = m_pBufferBmp->Canvas;

    if (m_isHorizontal)
        RenderHorizontalRuler(canvas);
    else
        RenderVerticalRuler(canvas);

    if (m_mouseHover)
        RenderIndicator(canvas);

    canvas->CopyMode = cmSrcCopy;
    Canvas->CopyRect(ClientRect, canvas, ClientRect);
}

//----------------------------------------------------------------------------
void __fastcall TRulerForm::RenderHorizontalRuler(TCanvas* canvas)
{
    SetTextAlign(canvas->Handle, TA_CENTER);
    canvas->Pen->Color = clBlack;
    canvas->Font->Color = clBlack;

    // Draw the background
    canvas->Brush->Color = m_RulerColor;
    canvas->FillRect(TRect(0, 0, m_length, m_breadth));

    // Draw the top ruler
    for (int i = 0; i < m_length; i += 2)
    {
        canvas->MoveTo(i, 0);
        if (i % 20 == 0)
            canvas->LineTo(i, 15);
        else if (i % 10 == 0)
            canvas->LineTo(i, 12);
        else
            canvas->LineTo(i, 8);
    }
    // Draw the bottom ruler
    for (int i = 0; i < m_length; i += 2)
    {
        canvas->MoveTo(i, m_breadth);
        if (i % 20 == 0)
            canvas->LineTo(i, m_breadth - 15);
        else if (i % 10 == 0)
            canvas->LineTo(i, m_breadth - 12);
        else
            canvas->LineTo(i, m_breadth - 8);
    }
    // Set the ruler text
    char szVal[6];
    for (int i = 0; i < m_length; i += 20)
        canvas->TextOut(i+1, (m_breadth - abs(Font->Height)) / 2, itoa(i, szVal, 10));
}

//----------------------------------------------------------------------------
void __fastcall TRulerForm::RenderVerticalRuler(TCanvas* canvas)
{
    SetTextAlign(canvas->Handle, TA_CENTER);
    canvas->Pen->Color = clBlack;
    canvas->Font->Color = clBlack;

    // Draw the background
    canvas->Brush->Color = m_RulerColor;
    canvas->FillRect(TRect(0, 0, m_breadth, m_length));

    // draw the left ruler
    for (int i = 0; i < m_length; i += 2)
    {
        canvas->MoveTo(0, i);
        if (i % 20 == 0)
            canvas->LineTo(15, i);
        else if (i % 10 == 0)
            canvas->LineTo(12, i);
        else
            canvas->LineTo(8, i);
    }
    // draw the right ruler
    for (int i = 0; i < m_length; i += 2)
    {
        canvas->MoveTo(m_breadth, i);
        if (i % 20 == 0)
            canvas->LineTo(m_breadth - 15, i);
        else if (i % 10 == 0)
            canvas->LineTo(m_breadth - 12, i);
        else
            canvas->LineTo(m_breadth - 8, i);
    }
    // set the ruler text
    char szVal[6];
    for (int i = 0; i < m_length; i += 20)
        canvas->TextOut(m_center + 1, i - 6, itoa(i, szVal, 10));
}

//----------------------------------------------------------------------------
void __fastcall TRulerForm::RenderIndicator(TCanvas* canvas)
{
    canvas->Font->Color = clRed;
    canvas->Pen->Color = clLtGray;
    canvas->Brush->Color = m_RulerColor;
    SetBkMode(canvas->Handle, TRANSPARENT);

    // Size is empirically found optimum (values should be odd)
    int x = m_ptMouse.x;
    int y = m_ptMouse.y;
    int w = 27;
    int h = 13;

    if (m_isHorizontal)
    {
        TRect rcFrame;
        rcFrame.left = x - w / 2;
        rcFrame.top = 19;
        rcFrame.right = rcFrame.left + w;
        rcFrame.bottom = rcFrame.top + h;
        canvas->Rectangle(rcFrame);

        char szVal[8];
        canvas->TextOut(x + 1, (m_breadth - abs(Font->Height)) / 2, itoa(x, szVal, 10));
    }
    else
    {
        TRect rcFrame;
        rcFrame.left = 12;
        rcFrame.top = y - h / 2;
        rcFrame.right = rcFrame.left + w;
        rcFrame.bottom = rcFrame.top + h;
        canvas->Rectangle(rcFrame);

        char szVal[8];
        canvas->TextOut(m_center + 1, y - 6, itoa(y, szVal, 10));
   }
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::SetCursorShape()
{
    if (m_isHorizontal)
    {
        if (m_ptMouse.y < m_center)
        {
            Cursor = TCursor(crHorUp);
        }
        else
        {
            Cursor = TCursor(crHorDown);
        }
    }
    else // Vertical
    {
        if (m_ptMouse.x < m_center)
        {
            Cursor = TCursor(crVertRight);
        }
        else
        {
            Cursor = TCursor(crVertLeft);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::Move(int dx, int dy)
{
    Left += dx;
    Top += dy;
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::HandleArrowKeys(WORD &Key, TShiftState Shift)
{
    // The caret can be moved with the arrow keys,
    // the ruler can be moved with control-arrow keys
    POINT ptMouse;
    GetCursorPos(&ptMouse);

    // Default position-change increment is 1
    int delta = 1;
    // Increment is 10 when shift is pressed
    if (Shift.Contains(ssShift))
        delta = 10;

    // Control-arrow keys move the ruler
    // Plain arrow keys move the cursor
    bool nudgeruler = Shift.Contains(ssCtrl);
    // Unless these functions are inverted
    if (m_useArrowNudge)
        nudgeruler = !nudgeruler;

    switch (Key)
    {
    case VK_LEFT:
        if (nudgeruler)
            Move(-delta, 0);
        else
            SetCursorPos(ptMouse.x - delta, ptMouse.y);
        break;
    case VK_RIGHT:
        if (nudgeruler)
            Move(delta, 0);
        else
            SetCursorPos(ptMouse.x + delta, ptMouse.y);
        break;
    case VK_UP:
        if (nudgeruler)
            Move(0, -delta);
        else
            SetCursorPos(ptMouse.x, ptMouse.y - delta);
        break;
    case VK_DOWN:
        if (nudgeruler)
            Move(0, delta);
        else
            SetCursorPos(ptMouse.x, ptMouse.y + delta);
        break;
    }
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::ToggleOrientation()
{
    // Rotate using current mouse pos as the pivot point
    POINT pt;
    GetCursorPos(&pt);

    if (m_isHorizontal)
    {
        int newleft = pt.x - m_center;
        int newtop = pt.y - (pt.x - Left);
        SetBounds(newleft, newtop, m_breadth, m_length);
        m_pBufferBmp->Width = m_breadth;
        m_pBufferBmp->Height = m_length;
    }
    else
    {
        int newleft = pt.x - (pt.y - Top);
        int newtop = pt.y - m_center;
        SetBounds(newleft, newtop, m_length, m_breadth);
        m_pBufferBmp->Width = m_length;
        m_pBufferBmp->Height = m_breadth;
    }

    m_isHorizontal = !m_isHorizontal;
    g_ToolOptions.Set(m_ToolName, "horizontal", m_isHorizontal);

    // Force a redraw
    Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::ToggleTransparency()
{
    m_isTransparent = !m_isTransparent;
    SetTransparency(m_isTransparent, m_transparency);
    g_ToolOptions.Set(m_ToolName, "transparent", m_isTransparent);
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::RulerMenuPopup(TObject *Sender)
{
    miOrientation->Caption = m_isHorizontal ? "Vertical" : "Horizontal";
    miTransparent->Checked = m_isTransparent;
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::miSlidetoZeroClick(TObject *Sender)
{
    if (m_isHorizontal)
        Left = 0;
    else
        Top = 0;
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::miOrientationClick(TObject *Sender)
{
    ToggleOrientation();
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::miTransparentClick(TObject *Sender)
{
    ToggleTransparency();
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::miHideClick(TObject *Sender)
{
    Close();
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::miExitClick(TObject *Sender)
{
    Application->Terminate();
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::miOptionsClick(TObject *Sender)
{
    if (FOnOptions)
        FOnOptions(this);
}

//---------------------------------------------------------------------------

