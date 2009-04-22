//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TopToolBar.h"
#include "Main.h"

/////////////////////////////////////////////////////////////////////////////
//
// Implementation of class TTopToolButton
//
/////////////////////////////////////////////////////////////////////////////

__fastcall TTopToolButton::TTopToolButton(TComponent* Owner)
  : TToolButton(Owner)
{
  // Have a PNG image container
  FPngImage = new TPNGObject();

  FColor = clWhite;
  Enabled = true;
  AllowAllUp = true;
  ParentShowHint = false;
  ShowHint = true;
  OnMouseUp = HandleMouseUp;
}

//---------------------------------------------------------------------------
__fastcall TTopToolButton::~TTopToolButton()
{
  // Cleanup PNG image container
  delete FPngImage;
}

//---------------------------------------------------------------------------
TPNGObject* __fastcall TTopToolButton::GetPngImage()
{
  // Return PNG image
  return FPngImage;
}

//---------------------------------------------------------------------------
void __fastcall TTopToolButton::SetPngImage(TPNGObject* Value)
{
  // Assign PNG image
  FPngImage->Assign(Value);
}

//---------------------------------------------------------------------------
void __fastcall TTopToolButton::SetColor(TColor Value)
{
  // Only invalidate when color changed
  if (FColor != Value)
  {
    FColor = Value;
    Invalidate();
  }
}

//---------------------------------------------------------------------------
void __fastcall TTopToolButton::Paint(void)
{
  // Set background attributes
  Canvas->Pen->Color = FColor;
  Canvas->Brush->Color = FColor;

  TRect rc = ClientRect;
  InflateRect(&rc, -1, -1);

  if (Down)
  {
    Canvas->Pen->Color = clNavy;
    Canvas->Brush->Color = (TColor) RGB(0xD4, 0xD5, 0xD8);
  }

  if (MouseInControl && m_Focussed)
  {
    Canvas->Pen->Color = clNavy;
//    Canvas->Brush->Color = (TColor) RGB(0xB6, 0xBD, 0xD2);
  }

  // Paint background
  Canvas->Rectangle(rc);

  // Draw PNG image on top
  TPNGObject* Png = new TPNGObject();
  try
  {
    Png->Assign(FPngImage);

    if (!Enabled)
      MakeImageHalfTransparent(Png);

    int dx = (ClientWidth - Png->Width) / 2;
    int dy = (ClientHeight - Png->Height) / 2;
    TRect rcPaint(dx, dy, dx + Png->Width, dy + Png->Height);

    Png->Draw(Canvas, rcPaint);
  }
  __finally
  {
    delete Png;
  }
}

//---------------------------------------------------------------------------
void __fastcall TTopToolButton::HandleMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TTopToolButton::Flatten(const TPoint& pt)
{
  if (MouseInControl && !PtInRect(&BoundsRect, pt))
     Perform(CM_MOUSELEAVE, 0, 0);
}

//---------------------------------------------------------------------------
void __fastcall TTopToolButton::CMMouseEnter(TMessage &Msg)
{
  MouseInControl = true;
  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TTopToolButton::CMMouseLeave(TMessage &Msg)
{
  MouseInControl = false;
  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TTopToolButton::MakeImageHalfTransparent(TPNGObject* Obj)
// Purpose           : Create a 50% transparant PNG for disabled buttons.
// Original author   : Erik Stok
// C++ translation   : Willem Jongman

{
  // Voeg alpha channel toe als het een RGB of GRAYSCALE is
  if (Obj->Header->ColorType == COLOR_RGB ||
      Obj->Header->ColorType == COLOR_GRAYSCALE)
  {
    Obj->CreateAlpha();
  }
  // Zet halve transparency
  if (Obj->Header->ColorType == COLOR_RGBALPHA ||
      Obj->Header->ColorType == COLOR_GRAYSCALEALPHA)
  {
    for (unsigned int x = 0; x < Obj->Header->Height; x++)
      for (unsigned int y = 0; y < Obj->Header->Width; y++)
        Obj->AlphaScanline[x][y] = (BYTE)(Obj->AlphaScanline[x][y] / 2);
  }
}

/////////////////////////////////////////////////////////////////////////////
//
// Implementation of class TTopToolBar
//
/////////////////////////////////////////////////////////////////////////////

__fastcall TTopToolBar::TTopToolBar(TComponent* Owner)
  : TPanel(Owner)
{
  m_timer = new TTimer(this);
  m_timer->Interval = 250;
  m_timer->Enabled = false;
  m_timer->OnTimer = OnTimerTick;

  InitButtons();
  BevelInner = bvNone;
  BevelOuter = bvNone;
  Color = clWhite;
}

//---------------------------------------------------------------------------
__fastcall TTopToolBar::~TTopToolBar()
{
  delete m_timer;

  for (unsigned int i = 0; i < m_buttons.size(); i++)
    delete m_buttons[i];
}

//---------------------------------------------------------------------------
void __fastcall TTopToolBar::OnFocusChange(bool focus)
{
  // Run update-timer only when focussed
  m_timer->Enabled = focus;

  // Tell all buttons about the new focus state
  for (unsigned int i = 0; i < m_buttons.size(); i++)
    m_buttons[i]->SetFocussed(focus);
}

//---------------------------------------------------------------------------
void __fastcall TTopToolBar::OnTimerTick(TObject *Sender)
{
  FlattenButtons();
}

//---------------------------------------------------------------------------
void __fastcall TTopToolBar::FlattenButtons()
{
  // Get mouse position in screen coordinates
  TPoint ptAbs;
  GetCursorPos(&ptAbs);
  TPoint ptRel = ScreenToClient(ptAbs);

  // Flatten all stuck buttons
  for (unsigned int i = 0; i < m_buttons.size(); i++)
    m_buttons[i]->Flatten(ptRel);
}

//---------------------------------------------------------------------------
void __fastcall TTopToolBar::InitButtons()
{
  TTopToolButton* pButton;
  TPNGObject* pImage;

  pButton = new TTopToolButton(this);
  pButton->Parent = this;
  pButton->Name = "bnRuler";
  pButton->Action = MainForm->actRuler;
  pButton->Align = alLeft;
  pButton->Width = TOOLBUTTONWIDTH;
  pButton->PopupMenu = MainForm->PopupMenu;
  pImage = new TPNGObject();
  pImage->LoadFromResourceName((int)HInstance, "PNG_RULER");
  pButton->PngImage = pImage;
  delete pImage;
  m_buttons.push_back(pButton);

  pButton = new TTopToolButton(this);
  pButton->Parent = this;
  pButton->Name = "bnLoupe";
  pButton->Action = MainForm->actLoupe;
  pButton->Align = alLeft;
  pButton->Width = TOOLBUTTONWIDTH;
  pButton->PopupMenu = MainForm->PopupMenu;
  pImage = new TPNGObject();
  pImage->LoadFromResourceName((int)HInstance, "PNG_LOUPE");
  pButton->PngImage = pImage;
  delete pImage;
  m_buttons.push_back(pButton);

  pButton = new TTopToolButton(this);
  pButton->Parent = this;
  pButton->Name = "bnInfo";
  pButton->Action = MainForm->actInfo;
  pButton->Align = alLeft;
  pButton->Width = TOOLBUTTONWIDTH;
  pButton->PopupMenu = MainForm->PopupMenu;
  pImage = new TPNGObject();
  pImage->LoadFromResourceName((int)HInstance, "PNG_INFO");
  pButton->PngImage = pImage;
  delete pImage;
  m_buttons.push_back(pButton);

  pButton = new TTopToolButton(this);
  pButton->Parent = this;
  pButton->Name = "bnBaseConv";
  pButton->Action = MainForm->actBaseConv;
  pButton->Align = alLeft;
  pButton->Width = TOOLBUTTONWIDTH;
  pButton->PopupMenu = MainForm->PopupMenu;
  pImage = new TPNGObject();
  pImage->LoadFromResourceName((int)HInstance, "PNG_BASECONV");
  pButton->PngImage = pImage;
  delete pImage;
  m_buttons.push_back(pButton);

  pButton = new TTopToolButton(this);
  pButton->Parent = this;
  pButton->Name = "bnGrab";
  pButton->Action = MainForm->actCapture;
  pButton->Align = alLeft;
  pButton->Width = TOOLBUTTONWIDTH;
  pButton->PopupMenu = MainForm->PopupMenu;
  pImage = new TPNGObject();
  pImage->LoadFromResourceName((int)HInstance, "PNG_GRAB");
  pButton->PngImage = pImage;
  delete pImage;
  m_buttons.push_back(pButton);

  pButton = new TTopToolButton(this);
  pButton->Parent = this;
  pButton->Name = "bnOptions";
  pButton->Action = MainForm->actOptions;
  pButton->Align = alLeft;
  pButton->Width = TOOLBUTTONWIDTH;
  pImage = new TPNGObject();
  pImage->LoadFromResourceName((int)HInstance, "PNG_OPTIONS");
  pButton->PngImage = pImage;
  delete pImage;
  m_buttons.push_back(pButton);

  pButton = new TTopToolButton(this);
  pButton->Parent = this;
  pButton->Name = "bnExit";
  pButton->Action = MainForm->actExit;
  pButton->Align = alLeft;
  pButton->Width = TOOLBUTTONWIDTH;
  pImage = new TPNGObject();
  pImage->LoadFromResourceName((int)HInstance, "PNG_EXIT");
  pButton->PngImage = pImage;
  delete pImage;
  m_buttons.push_back(pButton);

/*
  pButton = new TTopToolButton(this);
  pButton->Parent = this;
  pButton->Name = "bn";
  pButton->OnClick = ButtonClick;
  pButton->Align = alLeft;
  pButton->Width = TOOLBUTTONWIDTH;
  pButton->GroupIndex = ;
  pImage = new TPNGObject();
  pImage->LoadFromResourceName((int)HInstance, "PNG_");
  pButton->PngImage = pImage;
  delete pImage;
  m_buttons.push_back(pButton);
*/
}

//---------------------------------------------------------------------------

