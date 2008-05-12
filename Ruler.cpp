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

const RULERWIDTH = 50;

//---------------------------------------------------------------------------
__fastcall TRulerForm::TRulerForm(TComponent* Owner)
: TToolForm(Owner, "ruler"),
  inMenu(false),
  inSizeMove(false)
{
  // Load the custom cursors
  Screen->Cursors[crHorUp] = LoadCursor((void*)HInstance, "HORUP");
  Screen->Cursors[crHorDown] = LoadCursor((void*)HInstance, "HORDOWN");
  Screen->Cursors[crVertLeft] = LoadCursor((void*)HInstance, "VERTLEFT");
  Screen->Cursors[crVertRight] = LoadCursor((void*)HInstance, "VERTRIGHT");

  // Set scaled to false so we look decent also with large fonts
  Scaled = false;

  // Here we determine the color of our ruler
  m_RulerColor = clWhite; //  m_RulerColor = clInfoBk;
  Color = m_RulerColor;

  // Initialize the offsetindicator window
  FloatForm = new TFloatForm(this);
  FloatForm->OffsetLabel->OnMouseDown = FloatMouseDown;
  FloatForm->OffsetLabel->OnMouseMove = FloatMouseMove;
  FloatForm->Color = Color;

  m_breadth = RULERWIDTH;
  m_center = RULERWIDTH / 2;

  SnapScreenEdges = false;

  UpdateUI();
}

//---------------------------------------------------------------------------
__fastcall TRulerForm::~TRulerForm()
{
  delete FloatForm;
  delete m_HorRulerBmp;
  delete m_VertRulerBmp;
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::UpdateUI()
{
  bool isTransparent = g_ToolOptions.Get(m_ToolName, "transparent", false);
  int Transparency = g_ToolOptions.Get(m_ToolName, "transparency", 50);
  SetTransparency(isTransparent, Transparency);

  bool isHorizontal = g_ToolOptions.Get(m_ToolName, "horizontal", true);
  if (isHorizontal)
  {
    Width = g_ToolOptions.Get(m_ToolName, "length", 1280);
    Height = m_breadth;
  }
  else
  {
    Width = m_breadth;
    Height = g_ToolOptions.Get(m_ToolName, "length", 1280);
  }

  RenderHorizontalRuler();
  RenderVerticalRuler();

  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::TimerEvent(TPoint ptMouse)
{
  if (inMenu || inSizeMove)
  {
    // We are processing a popupmenu , bail out..
    return;
  }

  // Get mouse position in client coordinates
  POINT ptRelMouse = ScreenToClient(ptMouse);
  // Get the clientwindow dimensions
  RECT rcClient = ClientRect;

  // See if mousepointer is inside our window
  if (PtInRect(&rcClient, ptRelMouse))
  {
    // Bring FloatForm on top of the ruler
    SetWindowPos(Handle, FloatForm->Handle, 0, 0, 0, 0,
                 SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
  }
  else // No mouse in our window
  {
    // Hide the coordinate window
    FloatForm->Visible = false;
  }
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::FloatMouseDown(TObject *Sender,
                                           TMouseButton Button, TShiftState Shift, int X, int Y)
{
  // Transform X and Y to ruler coordinates
  POINT ptAbs = FloatForm->ClientToScreen(Point(X, Y));
  POINT ptRel = ScreenToClient(ptAbs);
  // Let the ruler handle the event
  FormMouseDown(Sender, Button, Shift, ptRel.x, ptRel.y);
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::FloatMouseMove(TObject *Sender,
                                           TShiftState Shift, int X, int Y)
{
  // This event is triggered when the mouse
  // moves over the floating indicator window
  // Transform X and Y to ruler coordinates
  POINT ptAbs = FloatForm->ClientToScreen(Point(X, Y));
  POINT ptRel = ScreenToClient(ptAbs);
  // Let the ruler handle the event
  FormMouseMove(Sender, Shift, ptRel.x, ptRel.y);
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::FormMouseDown(TObject *Sender, TMouseButton Button,
                                          TShiftState Shift, int X, int Y)
{
  FloatForm->Visible = false;

  if (Button == mbLeft)
  // Start a drag-operation
  {
    m_MouseOldX = X;
    m_MouseOldY = Y;
  }

  else if (Button == mbRight)
  {
    // Popup-menu request
    inMenu = true;
    POINT ptAbs = ClientToScreen(Point(X, Y));
    RulerMenu->Popup(ptAbs.x, ptAbs.y);
    inMenu = false;
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
/*
  // See if mouse is above a resize area
//    POINT pt = ScreenToClient(Point(Message.XPos, Message.YPos));
    POINT pt = Point(X, Y);
    TRect rc = ClientRect;
    if (m_horizontal)
    {
      rc.Left = rc.Right - GetSystemMetrics(SM_CXHSCROLL);
      if (PtInRect(&rc, pt))
      {
        Cursor = TCursor(crSizeWE);
      }
    }
    else
    {
      rc.Top  = rc.Bottom - GetSystemMetrics(SM_CYVSCROLL);
      if (PtInRect(&rc, pt))
      {
        Cursor = TCursor(crSizeNS);
      }
    }
*/
  // Bail-out if we are processing the popup-menu
  if (inMenu || inSizeMove)
    return;

  if (Shift.Contains(ssLeft))
  // We are dragging, move the ruler and the coordinate window
  {
    Left += X - m_MouseOldX;
    Top  += Y - m_MouseOldY;
    FloatForm->MoveRelative(X - m_MouseOldX, Y - m_MouseOldY);
  }
  else
  // Normal mouse movement, update coordinates
  {
    // Get mouse position in screen coordinates
    TPoint ptMouse = ClientToScreen(Point(X, Y));

    SetCursorShape(X, Y);

    // Adjust position of floating offset-window
    FloatForm->MoveAbsolute(ptMouse, TPoint(Left, Top), g_ToolOptions.GetBool(m_ToolName, "horizontal"));
    // Show offset-window
    FloatForm->Visible = true;
  }
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::SetCursorShape(int x, int y)
// get the proper cursor for both the ruler
// and the floating offset window
{
  if (g_ToolOptions.GetBool(m_ToolName, "horizontal"))
  {
    if (y < m_center)
    {
      Cursor = TCursor(crHorUp);
      FloatForm->Cursor = TCursor(crHorUp);
    }
    else
    {
      Cursor = TCursor(crHorDown);
      FloatForm->Cursor = TCursor(crHorDown);
    }
  }
  else // Vertical
  {
    if (x < m_center)
    {
      Cursor = TCursor(crVertRight);
      FloatForm->Cursor = TCursor(crVertRight);
    }
    else
    {
      Cursor = TCursor(crVertLeft);
      FloatForm->Cursor = TCursor(crVertLeft);
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::Move(int dx, int dy)
{
  Left += dx;
  Top += dy;
  FloatForm->Left += dx;
  FloatForm->Top += dy;
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
  if (g_ToolOptions.GetBool(m_ToolName, "arrownudge"))
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

  FloatForm->Visible = false;

  if (g_ToolOptions.GetBool(m_ToolName, "horizontal"))
  {
    int newleft = pt.x - m_center;
    int newtop = pt.y - (pt.x - Left);
    g_ToolOptions.Set(m_ToolName, "horizontal", false);
    SetBounds(newleft, newtop, m_breadth, g_ToolOptions.GetInt(m_ToolName, "length"));
  }
  else
  {
    int newleft = pt.x - (pt.y - Top);
    int newtop = pt.y - m_center;
    g_ToolOptions.Set(m_ToolName, "horizontal", true);
    SetBounds(newleft, newtop, g_ToolOptions.GetInt(m_ToolName, "length"), m_breadth);
  }

  // Force a redraw
  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::ToggleTransparency()
{
  bool isTransparent = g_ToolOptions.GetBool(m_ToolName, "transparent");
  g_ToolOptions.Set(m_ToolName, "transparent", !isTransparent);

  int Transparency = g_ToolOptions.GetInt(m_ToolName, "transparency");

  SetTransparency(isTransparent, Transparency);
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::FormPaint(TObject *Sender)
{
  if (g_ToolOptions.GetBool(m_ToolName, "horizontal"))
    Canvas->CopyRect(ClientRect, m_HorRulerBmp->Canvas, ClientRect);
  else
    Canvas->CopyRect(ClientRect, m_VertRulerBmp->Canvas, ClientRect);
}

//----------------------------------------------------------------------------
void __fastcall TRulerForm::InitCanvasAttribs(TCanvas* canvas)
{
  canvas->Pen->Color = clBlack;
  canvas->Font->Name = "Microsoft Sans Serif";
  canvas->Font->Height = -9;
  canvas->Brush->Color = m_RulerColor;
}

//----------------------------------------------------------------------------
void __fastcall TRulerForm::RenderVerticalRuler()
{
  if (!m_VertRulerBmp)
  {
    // Set up the buffer bitmap
    m_VertRulerBmp = new Graphics::TBitmap;
    InitCanvasAttribs(m_VertRulerBmp->Canvas);
  }

  int rulerlength = g_ToolOptions.GetInt(m_ToolName, "length");

  m_VertRulerBmp->Width = m_breadth;
  m_VertRulerBmp->Height = rulerlength;

  TCanvas* canvas = m_VertRulerBmp->Canvas;
  SetTextAlign(canvas->Handle, TA_CENTER);

  // draw the left ruler
  for (int i = 0; i < rulerlength; i += 2)
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
  for (int i = 0; i < rulerlength; i += 2)
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
  for (int i = 0; i < rulerlength; i += 20)
    canvas->TextOut(m_center + 1, i - 6, itoa(i, szVal, 10));
}

//----------------------------------------------------------------------------
void __fastcall TRulerForm::RenderHorizontalRuler()
{
  if (!m_HorRulerBmp)
  {
    // Set up the buffer bitmap
    m_HorRulerBmp = new Graphics::TBitmap;
    InitCanvasAttribs(m_HorRulerBmp->Canvas);
  }

  int rulerlength = g_ToolOptions.GetInt(m_ToolName, "length");

  m_HorRulerBmp->Height = m_breadth;
  m_HorRulerBmp->Width = rulerlength;

  TCanvas* canvas = m_HorRulerBmp->Canvas;
  SetTextAlign(canvas->Handle, TA_CENTER);

  // Draw the background
  canvas->FillRect(TRect(0, 0, rulerlength, m_breadth));

  // Draw the top ruler
  for (int i = 0; i < rulerlength; i += 2)
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
  for (int i = 0; i < rulerlength; i += 2)
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
  for (int i = 0; i < rulerlength; i += 20)
    canvas->TextOut(i+1, (m_breadth - abs(Font->Height)) / 2, itoa(i, szVal, 10));
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::miSlidetoZeroClick(TObject *Sender)
{
  if (g_ToolOptions.GetBool(m_ToolName, "horizontal"))
    Left = 0;
  else
    Top = 0;
}

//---------------------------------------------------------------------------
void __fastcall TRulerForm::RulerMenuPopup(TObject *Sender)
{
  miOrientation->Caption = g_ToolOptions.GetBool(m_ToolName, "horizontal") ? "Vertical" : "Horizontal";
  miTransparent->Checked = g_ToolOptions.GetBool(m_ToolName, "transparent");
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

