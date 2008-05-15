//---------------------------------------------------------------------------

#ifndef HotkeyPanelH
#define HotkeyPanelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "HotkeyInfo.h"

//---------------------------------------------------------------------------
class PACKAGE THotkeyPanel : public TPanel
{
private:
  void __fastcall EditboxKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall CheckboxClick(TObject *Sender);
  void __fastcall CheckboxMouseUp(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);

  TCheckBox* m_pCheckbox;
  TLabel* m_pLabel;
  TEdit* m_pEdit;

  THotkeyInfo m_KeyInfo;

protected:
  String FCheckboxText;
  void __fastcall SetCheckboxText(String caption);

public:
  __fastcall THotkeyPanel(TComponent* Owner);
  __fastcall ~THotkeyPanel();

  void SetKeyInfo(const THotkeyInfo& KeyInfo);
  THotkeyInfo GetKeyInfo();

__published:
  __property String Caption = {};
  __property String CheckboxText = { read = FCheckboxText, write = SetCheckboxText };
};
//---------------------------------------------------------------------------
#endif
