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

//---------------------------------------------------------------------------
class TRulerForm : public TToolForm
{
    __published:  // IDE-managed Components
    TPopupMenu *RulerMenu;
    TMenuItem *miHide;
    TMenuItem *N1;
    TMenuItem *miOrientation;
    TMenuItem *miZero;
    TMenuItem *N2;
    TMenuItem *miExit;
    TMenuItem *miTransparent;
    TMenuItem *N3;
    TMenuItem *miOptions;
    TMenuItem *N4;

    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);
    void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
                                  int X, int Y);
    void __fastcall RulerMenuPopup(TObject *Sender);
    void __fastcall miHideClick(TObject *Sender);
    void __fastcall miOrientationClick(TObject *Sender);
    void __fastcall miSlidetoZeroClick(TObject *Sender);
    void __fastcall miExitClick(TObject *Sender);
    void __fastcall miTransparentClick(TObject *Sender);
    void __fastcall miOptionsClick(TObject *Sender);

protected:
    virtual void __fastcall WndProc(TMessage &Msg);
private:
    void __fastcall RenderVerticalRuler(TCanvas* canvas);
    void __fastcall RenderHorizontalRuler(TCanvas* canvas);
    void __fastcall RenderIndicator(TCanvas* canvas);

    void __fastcall SetCursorShape();
    void __fastcall Move(int dx, int dy);

    Graphics::TBitmap* m_pBufferBmp;
    TPoint m_ptMouse;

    bool m_inMenu;
    bool m_inSizeMove;
    bool m_mouseHover;

    int m_breadth;
    int m_length;
    int m_center;

    int m_transparency;
    bool m_isTransparent;
    bool m_isHorizontal;
    bool m_useArrowNudge;
    TColor m_RulerColor;

public:
    __fastcall TRulerForm(TComponent* Owner);
    __fastcall ~TRulerForm();

    void __fastcall UpdateSettings();
    void __fastcall TimerEvent(TPoint ptMouse);
    void __fastcall HandleArrowKeys(WORD &Key, TShiftState Shift);
    void __fastcall ToggleOrientation();
    void __fastcall ToggleTransparency();

};

#endif
