//---------------------------------------------------------------------------
#ifndef RulerH
#define RulerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "Tool.h"

#include "Float.h"
#include "Tool.h"
//#include "ToolOptions.h"
//---------------------------------------------------------------------------
class TRulerForm : public TToolForm
{
__published:	// IDE-managed Components
  TPopupMenu *RulerMenu;
  TMenuItem *miHide;
  TMenuItem *N1;
  TMenuItem *miOrientation;
  TMenuItem *miZero;
  TMenuItem *N2;
  TMenuItem *miExit;
  TMenuItem *miTransparent;
  TMenuItem *N3;

  void __fastcall FormPaint(TObject *Sender);
  void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                TShiftState Shift, int X, int Y);
  void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
                                                   int X, int Y);
  void __fastcall miHideClick(TObject *Sender);
  void __fastcall miOrientationClick(TObject *Sender);
  void __fastcall miSlidetoZeroClick(TObject *Sender);
  void __fastcall miExitClick(TObject *Sender);
  void __fastcall miTransparentClick(TObject *Sender);
  void __fastcall RulerMenuPopup(TObject *Sender);

protected:
  void __fastcall FloatMouseDown(TObject *Sender, TMouseButton Button,
                                 TShiftState Shift, int X, int Y);
  void __fastcall FloatMouseMove(TObject *Sender, TShiftState Shift,
                                                    int X, int Y);
private:
  void __fastcall InitCanvasAttribs(TCanvas* canvas);
  void __fastcall RenderVerticalRuler();
  void __fastcall RenderHorizontalRuler();
  void __fastcall SetCursorShape(int x, int y);
  void __fastcall Move(int dx, int dy);

  TCursor OldCursor;
  int m_MouseOldX;
  int m_MouseOldY;

  int m_breadth;
  int m_center;
//  TRulerOptions m_Options;

  TColor m_RulerColor;
  TFloatForm* FloatForm;
  Graphics::TBitmap* m_HorRulerBmp;
  Graphics::TBitmap* m_VertRulerBmp;

  // State flags
  bool inMenu;
  bool inSizeMove;

public:
  __fastcall TRulerForm(TComponent* Owner);
  __fastcall ~TRulerForm();

  void __fastcall UpdateUI();
  void __fastcall TimerEvent(TPoint ptMouse);
  void __fastcall HandleArrowKeys(WORD &Key, TShiftState Shift);
  void __fastcall ToggleOrientation();
  void __fastcall ToggleTransparency();

protected:
  // Here we sink key-events of the floating indicator
  // so all these events can be handled by the ruler form
  TKeyEvent FOnKeyDown;

  void __fastcall SetOnKeyDown(TKeyEvent handler)
  {
    TForm::OnKeyDown = handler;
    FloatForm->OnKeyDown = handler;
    FOnKeyDown = handler;
  }

__published:
  __property TKeyEvent OnKeyDown = { read = FOnKeyDown, write = SetOnKeyDown };

};

#if 0
// Experimental stuff
//---------------------------------------------------------------------------
class TTrackLine
{
private:

  TPoint m_ptLastStart, m_ptLastEnd;
  HDC DesktopDC;
  TCanvas* ScreenCanvas;
  Graphics::TBitmap* SavedLineBitmap;
  bool m_visible;
  bool m_ishorizontal;

public:

  TTrackLine(bool ishorizontal)
  {
    m_ishorizontal = ishorizontal;
    DesktopDC = GetDC(NULL);
    ScreenCanvas = new TCanvas();
    ScreenCanvas->Handle = DesktopDC;
    ScreenCanvas->Pen->Mode = (TPenMode)pmNot;

    // Only non-paletted displays for now..
    SavedLineBitmap = new Graphics::TBitmap();

    m_visible = false;
  }
/*
  TTrackLine()
  {
    DesktopDC = GetDC(NULL); // Probably not wise to cache the desktop dc in the constructor...
    ScreenCanvas = new TCanvas();
    ScreenCanvas->Handle = DesktopDC;
    ScreenCanvas->Pen->Mode = pmNot;
    visible = false;
  }
*/
  ~TTrackLine()
  {
    Hide();

    delete ScreenCanvas;
    delete SavedLineBitmap;
    ReleaseDC(NULL, DesktopDC);
  }

  void __fastcall Show(const TPoint& ptMouse)
  {
    Move(ptMouse);
  }

  void __fastcall Move(const TPoint& ptMouse) //, bool horizontal)
  {
/*
void __fastcall CopyRect(const Windows::TRect &Dest,
                         TCanvas* Canvas,
                         const Windows::TRect &Source);

    TRect rcDest(ptStart.x, ptStart.y, ptEnd.x + 1,  ptEnd.y + 1);

    TRect rcFrom(TPoint(0, 0), TPoint(Screen->DesktopWidth, SavedLineBitmap->Height));
    // Save screenline to bitmap
    SavedLineBitmap->Canvas->CopyRect(rcDest, ScreenCanvas, rcFrom);
    // Restore screenline from saved bitmap
    ScreenCanvas->CopyRect(RECT(),
                           m_bmSavedHorizontal->Canvas,
                           RECT());
*/

    Hide();

    TPoint ptStart, ptEnd;

    if (m_ishorizontal)
    {
      ptStart = TPoint(ptMouse.x, Screen->DesktopTop);
      ptEnd = TPoint(ptMouse.x, Screen->DesktopTop + Screen->DesktopHeight);
    }
    else
    {
      ptStart = TPoint(Screen->DesktopLeft, ptMouse.y);
      ptEnd = TPoint(Screen->DesktopLeft + Screen->DesktopWidth, ptMouse.y);
    }

    Draw(ptStart, ptEnd);
  }

  void __fastcall Draw(const TPoint& ptStart, const TPoint& ptEnd)
  {
    if (!m_visible)
    {
      m_visible = true;
      ScreenCanvas->MoveTo(ptStart.x, ptStart.y);
      ScreenCanvas->LineTo(ptEnd.x, ptEnd.y);
      m_ptLastStart = ptStart;
      m_ptLastEnd = ptEnd;
    }
  }

  void __fastcall Hide()
  {
    if (m_visible)
    {
      m_visible = false;
      ScreenCanvas->MoveTo(m_ptLastStart.x, m_ptLastStart.y);
      ScreenCanvas->LineTo(m_ptLastEnd.x, m_ptLastEnd.y);
      m_ptLastStart = TPoint(-1, -1);
      m_ptLastEnd = TPoint(-1, -1);
    }
  }
};
#endif // experimental stuff

#endif
