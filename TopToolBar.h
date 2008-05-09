//---------------------------------------------------------------------------

#ifndef TopToolButtonH
#define TopToolButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>

#include "png/pngimage.hpp"
#include <vector>

#define TOOLBUTTONWIDTH 30

class Comctrls::TToolButton;
//---------------------------------------------------------------------------
class TTopToolButton : public TToolButton
{
private:
  TPNGObject* FPngImage;
  TColor FColor;
  bool m_Focussed;

  void __fastcall MakeImageHalfTransparent(TPNGObject* Obj);

protected:
  void __fastcall SetColor(TColor Value);

  TPNGObject* __fastcall GetPngImage();
  void __fastcall SetPngImage(TPNGObject* Value);

  virtual void __fastcall Paint(void);

  void __fastcall HandleMouseUp(TObject *Sender, TMouseButton Button,
                                TShiftState Shift, int X, int Y);

public:
  __fastcall TTopToolButton(TComponent* Owner);
  __fastcall ~TTopToolButton();

  void __fastcall Flatten(const TPoint& P);
  void __fastcall SetFocussed(bool focus) { m_Focussed = focus; }

__published:
  __property TPNGObject* PngImage = { read = GetPngImage, write = SetPngImage };
  __property TColor Color = { read = FColor, write = SetColor, default = clBtnFace };

protected:
  bool MouseInControl;
  void __fastcall CMMouseEnter(TMessage &Message);
  void __fastcall CMMouseLeave(TMessage &Message);

BEGIN_MESSAGE_MAP
  VCL_MESSAGE_HANDLER(CM_MOUSEENTER, TMessage, CMMouseEnter);
  VCL_MESSAGE_HANDLER(CM_MOUSELEAVE, TMessage, CMMouseLeave);
END_MESSAGE_MAP(TToolButton);
};

/////////////////////////////////////////////////////////////////////////////
typedef std::vector<TTopToolButton*> TToolButtonArray;

//---------------------------------------------------------------------------
class TTopToolBar : public TPanel
{
public:
  __fastcall TTopToolBar(TComponent* Owner);
  __fastcall ~TTopToolBar();

  void Refresh() { FlattenButtons(); }

__published:
  __property bool Focussed = { write = OnFocusChange };

private:
  TToolButtonArray m_buttons;
  TTimer* m_timer;

  void __fastcall InitButtons();
  void __fastcall OnFocusChange(bool focus);
  void __fastcall OnTimerTick(TObject *Sender);
  void __fastcall FlattenButtons();
};


#endif

