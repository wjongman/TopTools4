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
  String m_RegKeyPath;

protected:
  String FCheckboxText;
//  void __fastcall SetCheckboxText(const String& caption)
  void __fastcall SetCheckboxText(String caption)
  {
    FCheckboxText = caption;
    m_pCheckbox->Caption = caption;
  }

public:
  __fastcall THotkeyPanel(TComponent* Owner);
  __fastcall ~THotkeyPanel();

  void __fastcall Load(const String& RegKeyPath);
  void __fastcall Save();

__published:
  __property String Caption = {};
	__property String CheckboxText = { read = FCheckboxText, write = SetCheckboxText };
};
//---------------------------------------------------------------------------
#endif
