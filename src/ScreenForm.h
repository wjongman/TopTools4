//---------------------------------------------------------------------------
#ifndef ScreenFormH
#define ScreenFormH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "Tool.h"
#include "ToolTip.h"

//---------------------------------------------------------------------------
class TScreenForm : public TToolForm
{
public:
    __fastcall TScreenForm(TComponent* Owner);
    virtual __fastcall ~TScreenForm();

private:
    TColor m_BorderColor;

    TToolTip* m_pToolTip;

    void UpdateToolTip();
    void ConstrainWindow(TRect& rcWindow);

    int m_WidthAtStartOfSize;
    int m_HeightAtStartOfSize;

protected:
    virtual void __fastcall WndProc(TMessage &Msg);

BEGIN_MESSAGE_MAP
   VCL_MESSAGE_HANDLER(WM_NCHITTEST, TWMNCHitTest, OnNCHitTest)
END_MESSAGE_MAP(TForm)

    void __fastcall OnNCHitTest(TWMNCHitTest &Message);
    void __fastcall OnEnterSizeMove(TMessage &Message);
    void __fastcall OnSizing(TMessage &Message);

__published:  // IDE-managed Components
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);

__published:
    __property TMouseEvent OnRightButtonClick = { read = FOnRightButtonClick, write = FOnRightButtonClick };

protected:
    TMouseEvent FOnRightButtonClick;

};

//---------------------------------------------------------------------------
extern PACKAGE TScreenForm *ScreenForm;

//---------------------------------------------------------------------------
#endif
