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
        : TToolForm(Owner, "grabscreen")
{
  BorderStyle = bsNone;
  Color = clInfoBk;//White;
  SetTransparency(true, 50);
  DraggableForm = true;
  Cursor = crSizeAll;
}

//---------------------------------------------------------------------------
__fastcall TScreenForm::~TScreenForm()
{
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::MouseDown(TMouseButton Button,
                                       TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  // Start a drag-operation
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
  if (Shift.Contains(ssLeft))
  // We are dragging, move the form
  {
    Left += X - m_MouseOldX;
    Top  += Y - m_MouseOldY;
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
  TPenMode OldPenMode = Canvas->Pen->Mode;

  Canvas->Pen->Mode = pmCopy;
  Canvas->Pen->Color = clActiveCaption;

  // Borders
  Canvas->MoveTo(rc.Left, rc.Top);
  Canvas->LineTo(rc.Right - 1, rc.Top);
  Canvas->LineTo(rc.Right - 1, rc.Bottom - 1);
  Canvas->LineTo(rc.Left, rc.Bottom - 1);
  Canvas->LineTo(rc.Left, rc.Top);

  int x_incr = GetSystemMetrics(SM_CXVSCROLL) / INC;
  int y_incr = GetSystemMetrics(SM_CXHSCROLL) / INC;

  for (int i = 1; i <= INC; i++)
  {
    // Top-Left sizegrip
    Canvas->MoveTo(rc.Left, rc.Top + i * y_incr);
    Canvas->LineTo(rc.Left + i * x_incr, rc.Top);
  }

  // Top-Right sizegrip
  for (int i = 1; i <= INC; i++)
  {
    Canvas->MoveTo(rc.Right, rc.Top + i * y_incr);
    Canvas->LineTo(rc.Right - i * x_incr, rc.Top);
  }

  // Bottom-Right sizegrip
  for (int i = 1; i <= INC; i++)
  {
    Canvas->MoveTo(rc.Right, rc.Bottom - i * y_incr);
    Canvas->LineTo(rc.Right - i * x_incr, rc.Bottom);
  }

  // Bottom-Left sizegrip
  for (int i = 1; i <= INC; i++)
  {
    Canvas->MoveTo(rc.Left, rc.Bottom - i * y_incr);
    Canvas->LineTo(rc.Left + i * x_incr, rc.Bottom);
  }

  // Restore original settings
  Canvas->Pen->Color = OldPenColor;
  Canvas->Pen->Mode = OldPenMode;
}

//---------------------------------------------------------------------------
void __fastcall TScreenForm::FormShow(TObject *Sender)
{
  TPoint ptMouse;
  GetCursorPos(&ptMouse);

  TRect rcClient = GetClientRect();
  int CenterX = rcClient.Width() / 2;
  int CenterY = rcClient.Height() / 2;


  Left = ptMouse.x - CenterX;
  Top = ptMouse.y - CenterY;
    
}
//---------------------------------------------------------------------------

