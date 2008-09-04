//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ToolTip.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TToolTipForm *ToolTipForm;
//---------------------------------------------------------------------------
__fastcall TToolTipForm::TToolTipForm(TComponent* Owner)
    : TForm(Owner)
{
   // Set to false so we look decent with large fonts
   Scaled = false;

   m_TipText = "Uninitialized";
   Label1->Visible = false;
   Label2->Visible = false;
}

//---------------------------------------------------------------------------
void __fastcall TToolTipForm::CreateParams(Controls::TCreateParams &Params)
{
  // Borrowed from Delphi's THintWindow
  TForm::CreateParams(Params);

  Params.Style = WS_POPUP | WS_BORDER;
  Params.WindowClass.style |= CS_SAVEBITS;

  if (NewStyleControls)
      Params.ExStyle = WS_EX_TOOLWINDOW;

  AddBiDiModeExStyle(Params.ExStyle);

}

//---------------------------------------------------------------------------
void __fastcall TToolTipForm::FormShow(TObject *Sender)
{
    Width = 64;
    Height = 64;
}

//---------------------------------------------------------------------------
void __fastcall TToolTipForm::FormPaint(TObject *Sender)
{
  Canvas->Brush->Color = this->Color;
  Canvas->Brush->Style = bsSolid;
//  Canvas->FillRect(ClientRect);

  RECT rcClient = ClientRect;

  rcClient.left += 2;
  rcClient.top += 2;

  Canvas->Font->Color = clInfoText;
  DrawText(Canvas->Handle, m_TipText.c_str(), -1, &rcClient, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK | DT_EXPANDTABS);

}

//---------------------------------------------------------------------------
void __fastcall TToolTipForm::SetDisplayPosition(int left, int top, int width, int height)
{
    Left = left;
    Top = top;
    if (width > 0)
        Width = width;
    if (height > 0)
        Height = height;
//    Width = 200;
//    Height = 16;
}

//---------------------------------------------------------------------------
void __fastcall TToolTipForm::SetText(const String& sIn)
{
    m_TipText = sIn;
    Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TToolTipForm::SetPosition(int X, int Y)
{
    Label1->Caption = "X: " + String(X);
    Label2->Caption = "Y: " + String(Y);
}

//---------------------------------------------------------------------------
void __fastcall TToolTipForm::SetDimensions(int W, int H)
{
    Label1->Caption = "W: " + String(W);
    Label2->Caption = "H: " + String(W);
}


