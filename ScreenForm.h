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

#ifdef _DEBUG
    #define TRACE(s) OutputDebugString(s)
#else
    #define TRACE(s)
#endif

#include "tool.h"
#include "ToolTip.h"

//---------------------------------------------------------------------------
class TScreenForm : public TToolForm
{
public:
    __fastcall TScreenForm(TComponent* Owner);
    virtual __fastcall ~TScreenForm();

private:
    bool m_TrackingMouse;

    TTimer* m_Timer;
    TToolTip* m_pToolTip;

    void UpdateToolTip();
    void ConstrainWindow(TRect& rcWindow);

protected:
    virtual void __fastcall WndProc(TMessage &Msg);
    void __fastcall OnTimerTick(TObject *Sender);

BEGIN_MESSAGE_MAP
   VCL_MESSAGE_HANDLER(WM_NCHITTEST, TWMNCHitTest, OnNCHitTest)
END_MESSAGE_MAP(TForm)

    void __fastcall OnNCHitTest(TWMNCHitTest &Message);

__published:  // IDE-managed Components
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);
    void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);

__published:
    __property TMouseEvent OnRightButtonClick = { read = FOnRightButtonClick, write = FOnRightButtonClick };

protected:
    TMouseEvent FOnRightButtonClick;

};

//---------------------------------------------------------------------------
extern PACKAGE TScreenForm *ScreenForm;

//---------------------------------------------------------------------------
#endif
