//---------------------------------------------------------------------------

#ifndef OptionsH
#define OptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Tool.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "HotkeyInfo.h"

/////////////////////////////////////////////////////////////////////////////
enum TDoubleClickOpen
{
  dcoToolbar = 1,
  dcoRuler = 2,
  dcoLoupe = 4,
  dcoInfo = 8,
  dcoBaseconv = 16
};

/////////////////////////////////////////////////////////////////////////////
class TToolOptions
{
public:
  TToolOptions()
  {
  }
  THotkeyInfo Hotkey;
  int DoubleClickOpen;
  bool Prefix;
  bool Quotes;
  bool OnTop;
  bool StartUp;
  int RulerLength;
//  bool MagnifySelf;
};

/////////////////////////////////////////////////////////////////////////////
class TOptionForm : public TForm
{
  __published:  // IDE-managed Components
  TCheckBox *ckOnTop;
  TCheckBox *ckStart;
  TEdit *edLength;
  TGroupBox *gbRuler;
  TGroupBox *gbGeneral;
  TLabel *lbLength;
  TPanel *Panel1;
  TUpDown *udLength;
  TGroupBox *gbColorCopy;
  TGroupBox *gbHotkey;
  TLabel *lbHotkey;
  TCheckBox *ckEnableHK;
  TEdit *HotkeyEdit;
  TMemo *Memo1;
  TGroupBox *gbAttributes;
  TCheckBox *ckPrefix;
  TCheckBox *ckQuotes;
  TGroupBox *gbIcon;
  TCheckBox *ckRuler;
  TCheckBox *ckToolbar;
  TCheckBox *ckLoupe;
  TCheckBox *ckInfo;
  TCheckBox *ckBaseconv;
  TButton *bnCancel;
  TButton *bnOk;
  TLabel *Label1;
  void __fastcall bnOkClick(TObject *Sender);
  void __fastcall HotkeyEditKeyDown(TObject *Sender, WORD &Key,
                                    TShiftState Shift);
  void __fastcall ckEnableHKClick(TObject *Sender);
  void __fastcall ckEnableHKMouseUp(TObject *Sender, TMouseButton Button,
                                    TShiftState Shift, int X, int Y);

public:   // User declarations
  __fastcall TOptionForm(TComponent* Owner);

  THotkeyInfo& GetHotkeyInfo()
  {
    return m_Options.Hotkey;
  }
  bool GetStayOnTop()
  {
    return m_Options.OnTop;
  }
  int GetDoubleClickOpen()
  {
    return m_Options.DoubleClickOpen;
  }

protected:
  // Override to do nothing
  virtual void __fastcall LoadPosition()
  {
  }
  virtual void __fastcall SavePosition()
  {
  }

private:  // User declarations

  TToolOptions m_Options;
  void __fastcall LoadSettings();
  void __fastcall SaveSettings();
  void __fastcall SetAutoRun(bool enable);
  bool __fastcall GetAutoRun();

  void __fastcall MakeMultilineHint(AnsiString &HintStr, bool &CanShow,
                                    THintInfo &HintInfo);
};

//---------------------------------------------------------------------------
#endif
