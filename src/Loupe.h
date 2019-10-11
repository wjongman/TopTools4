//---------------------------------------------------------------------------

#ifndef Loupe2H
#define Loupe2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>

#include "Tool.h"
#include "LoupePanel.h"
//---------------------------------------------------------------------------
class TLoupeForm : public TToolForm
{
    __published:  // IDE-managed Components
    TPopupMenu *LoupeMenu;
    TMenuItem *miHide;
    TMenuItem *N1;
    TMenuItem *miZoomIn;
    TMenuItem *miZoomOut;
    TMenuItem *N2;
    TMenuItem *miCrosshair;
    TMenuItem *miCenterbox;
    TMenuItem *miGrid;
    TMenuItem *N3;
    TMenuItem *miLockPos;
    TMenuItem *miFreezeView;
    TMenuItem *miMagnifySelf;
    TMenuItem *miExit;

    TToolBar *LoupeToolBar;
    TToolButton *bnZoomIn;
    TToolButton *bnZoomOut;
    TToolButton *Separator1;
    TToolButton *bnCrosshair;
    TToolButton *bnGrid;
    TToolButton *bnCenterbox;
    TToolButton *Separator2;
    TToolButton *bnLockPos;
    TToolButton *bnFreezeView;
    TMenuItem *N5;
    TMenuItem *miView;
    TMenuItem *miCopyToClipboard;
    TMenuItem *miSaveToFile;
    TMenuItem *miPrint;
    TMenuItem *N6;
    TMenuItem *miOptions;

    void __fastcall LoupeMenuPopup(TObject *Sender);

    void __fastcall miHideClick(TObject *Sender);
    void __fastcall miSaveViewClick(TObject *Sender);
    void __fastcall miZoomInClick(TObject *Sender);
    void __fastcall miZoomOutClick(TObject *Sender);
    void __fastcall miCrosshairClick(TObject *Sender);
    void __fastcall miCenterboxClick(TObject *Sender);
    void __fastcall miGridClick(TObject *Sender);
    void __fastcall miLockPosClick(TObject *Sender);
    void __fastcall miFreezeViewClick(TObject *Sender);
    void __fastcall miMagnifySelfClick(TObject *Sender);
    void __fastcall miExitClick(TObject *Sender);

    void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift,
                                     TPoint &MousePos, bool &Handled);
    void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift,
                                       TPoint &MousePos, bool &Handled);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
                                TShiftState Shift);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall miOptionsClick(TObject *Sender);

private:
    TLoupePanel* m_pLoupe;

    void __fastcall UpdateUI();

    TImageList *m_pToolbarImageList;
    void __fastcall LoadGraphics();

    void __fastcall SetSelfMagnify(bool magnify);
    bool m_bMagnifySelf;

protected:
    virtual void __fastcall WndProc(TMessage &Msg);

    virtual void __fastcall LoadSettings();
    virtual void __fastcall SaveSettings();

    void __fastcall SaveToFile();

public:
    __fastcall TLoupeForm(TComponent* Owner);
    __fastcall ~TLoupeForm();

    void __fastcall Freeze();
    void __fastcall ToggleLock();
    void __fastcall UnLock();
    void __fastcall ToggleMagnifySelf();
    void __fastcall UpdateSettings();
    void __fastcall ZoomIn()
    {
        miZoomInClick(this);
    }
    void __fastcall ZoomOut()
    {
        miZoomOutClick(this);
    }
};
//---------------------------------------------------------------------------
#endif
