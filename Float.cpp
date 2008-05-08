//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdlib.h>

#include "Float.h"
#include "Ruler.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

__fastcall TFloatForm::TFloatForm(TComponent* Owner)
  : TForm(Owner)
{
  // Set to false so we look decent with large fonts
  Scaled = false;

  // Adjust size to empirically found optimum (values should be odd)
  Height = 13;
  Width = 27;
}

//---------------------------------------------------------------------------
void __fastcall TFloatForm::MoveAbsolute(const TPoint& ptMouse,
                                         const TPoint& ptRulerLeftTop,
                                         bool isHorizontal)
{
  if (isHorizontal)
  {
    Left = ptMouse.x - Width/2;
    Top = ptRulerLeftTop.y + 19;
    SetCaption(String(ptMouse.x - ptRulerLeftTop.x));
  }
  else // Vertical
  {
    Left = ptRulerLeftTop.x + 12;
    Top = ptMouse.y - Height/2;
    SetCaption(String(ptMouse.y - ptRulerLeftTop.y));
  }
}

//---------------------------------------------------------------------------
void __fastcall TFloatForm::FormPaint(TObject *Sender)
{
  // Have a grey border
  Canvas->Brush->Color = clSilver;
  Canvas->FrameRect(ClientRect);
}

//---------------------------------------------------------------------------
void __fastcall TFloatForm::SetCaption(int offset)
{
  // Update the indicator
  char szVal[6];
  CaptionString = itoa(offset, szVal, 10);
}

//---------------------------------------------------------------------------
void __fastcall TFloatForm::SetCaption(const String& newcaption)
{
  // Update the indicator
  OffsetLabel->Caption = newcaption;
}

//---------------------------------------------------------------------------
void __fastcall TFloatForm::MoveRelative(int dx, int dy)
{
  Left += dx;
  Top += dy;
}




