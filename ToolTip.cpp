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
   m_TipText = "Uninitialized";
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
    Width = 32;
    Height = 32;
}

//---------------------------------------------------------------------------
void __fastcall TToolTipForm::FormPaint(TObject *Sender)
{
  RECT rcClient = ClientRect;

  rcClient.left += 2;
  rcClient.top += 2;

  Canvas->Font->Color = clInfoText;
  DrawText(Canvas->Handle, m_TipText.c_str(), -1, &rcClient, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK);

}
//---------------------------------------------------------------------------
void __fastcall TToolTipForm::SetText(const String& sIn)
{
    m_TipText = sIn;
}


