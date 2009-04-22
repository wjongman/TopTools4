//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>

#include "TrayIcon.h"
#include "Control.h"
#include "BaseConv.h"
#include "Info.h"
#include "Loupe.h"
#include "Ruler.h"
#include "Grabber.h"
#include "HotkeyManager.h"
//#include "ToolOptions.h"
#include "TopToolBar.h"
#include "Tool.h"

#define RUNNING_ON_VISTA 0

#if RUNNING_ON_VISTA
#include "capture.h"
#endif

//---------------------------------------------------------------------------
class TMainForm : public TToolForm
{
__published:  // IDE-managed Components
  TActionList *TrayActions;
  TAction *actCapture;
  TAction *actRuler;
  TAction *actLoupe;
  TAction *actBaseConv;
  TAction *actInfo;
  TAction *actAbout;
  TAction *actExit;
  TAction *actHelp;
  TAction *actOnTop;
  TAction *actTrayIcon;
  TAction *actOrientation;
  TAction *actZero;
  TAction *actOptions;
  TAction *actControl;
  TAction *actGrab;
  TPopupMenu *TrayMenu;
  TMenuItem *miRuler;
  TMenuItem *miLoupe;
  TMenuItem *miInfo;
  TMenuItem *miBaseConv;
  TMenuItem *miGrabber;
  TMenuItem *N2;
  TMenuItem *miOptions;
  TMenuItem *miToolBar;
  TMenuItem *N3;
  TMenuItem *miAbout;
  TMenuItem *N1;
  TMenuItem *miExit;
  TTimer *Timer;
    TImageList *MainIcons;
  void __fastcall HandleTimerEvent(TObject *Sender);
  void __fastcall actExitExecute(TObject *Sender);
  void __fastcall actCommandExecute(TObject *Sender);
  void __fastcall actOptionsExecute(TObject *Sender);
  void __fastcall actAboutExecute(TObject *Sender);
  void __fastcall actCaptureExecute(TObject *Sender);
    void __fastcall TrayMenuPopup(TObject *Sender);

public:   // User declarations
  __fastcall TMainForm(TComponent* Owner);
  __fastcall ~TMainForm();

protected:
  virtual void __fastcall WndProc(TMessage &Msg);
  void __fastcall LoadSettings();
  void __fastcall UpdateSettings();
  void __fastcall SaveSettings();

protected:
  void __fastcall HandleAppDeactivate(TObject *Sender);
  void __fastcall HandleAppRestore(TObject *Sender);
  void __fastcall HandleControlBarMinimize(TObject *Sender);
  void __fastcall HandleControlBarClose(TObject *Sender, TCloseAction &Action);
  void __fastcall HandleTrayMessage(TMessage &Message);
  void __fastcall HandleKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall HandleToolWindowClose(TObject *Sender, TCloseAction &Action);
  void __fastcall HandleCaptureNext(TObject *Sender);
  void __fastcall HandleCaptureComplete(TObject *Sender);
  void __fastcall HandleHotkey(THotkeyId id);
  void __fastcall HandleToolWindowOptions(TObject *Sender);

private:  // User declarations
  void __fastcall SetTopMost(bool ontop);
  void __fastcall ShowTrayIcon(bool show);
  void __fastcall ShowTaskbarIcon(bool show);
  String GetColorFormatString();

  bool TimerNeeded();
  bool m_bStayOnTop;
  bool m_bRememberState;

  THotkeyManager* m_HotkeyManager;

  enum UIMode { uiNormal, uiTrayApp };
  UIMode m_UIMode;

  void __fastcall SetUI(bool isTrayApp);
  void __fastcall ActAsSingleton(bool singleton);
  int __fastcall GetTaskbarRect(LPRECT lprect);

  cTrayIcon* m_pTrayIcon;
  TRulerForm* m_pRuler;
  TLoupeForm* m_pLoupe;
  TInfoForm* m_pInfo;
  TBaseConvForm* m_pBaseConv;
  TControlForm* m_pControlBar;
#if RUNNING_ON_VISTA
  TCaptureForm* m_pCapture;
#else
  //TScreenForm* m_pCapture;
  TScreenGrabber* m_pCapture;
#endif

  void CopyWebColorToClipboard();
  void ToggleOpenTools();
  void RestoreToolState(int toolstate);
  void HideAll();
  int GetToolState();
  bool AnyToolVisible();

  bool m_SavedLoupeState;

  TToolForm* GetRulerForm();
  TToolForm* GetLoupeForm();
  TToolForm* GetInfoForm();
  TToolForm* GetBaseConvForm();
  TToolForm* GetControlBar();
  TToolForm* GetCaptureForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
