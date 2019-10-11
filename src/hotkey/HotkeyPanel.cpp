//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HotkeyPanel.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall THotkeyPanel::THotkeyPanel(TComponent* Owner)
: TPanel(Owner)
{
  TPanel::Caption = "";

  Height = 67;
  Width = 250;
  BevelOuter = bvNone;

  m_pCheckbox = new TCheckBox(this);
  m_pCheckbox->Parent = this;
  m_pCheckbox->Left = 8;
  m_pCheckbox->Top =  8;
  m_pCheckbox->Width = 225;
  m_pCheckbox->Caption = "Caption";
  m_pCheckbox->OnClick = CheckboxClick;
  m_pCheckbox->OnMouseUp = CheckboxMouseUp;

  m_pLabel = new TLabel(this);
  m_pLabel->Parent = this;
  m_pLabel->Left = 27;
  m_pLabel->Top = 34;
  m_pLabel->Caption = "Hotkey:";

  m_pEdit = new TEdit(this);
  m_pEdit->Parent = this;
  m_pEdit->Left = 72;
  m_pEdit->Top = 32;
  m_pEdit->Width = 167;
  m_pEdit->ReadOnly = true;
  m_pEdit->OnKeyDown = EditboxKeyDown;
}

//---------------------------------------------------------------------------
__fastcall THotkeyPanel::~THotkeyPanel()
{
  delete m_pCheckbox;
  delete m_pLabel;
  delete m_pEdit;
}

//---------------------------------------------------------------------
void __fastcall THotkeyPanel::EditboxKeyDown(TObject *Sender, WORD &Key,
                                             TShiftState Shift)
{
  TShiftState emptyset;
  if (Key == VK_DELETE && Shift == emptyset)
  {
    // Delete key clears the editbox
    m_KeyInfo.Clear();
    m_pEdit->Text = "";
  }
  else
  {
    m_KeyInfo = THotkeyInfo(Key, Shift);
    m_pEdit->Text = m_KeyInfo.GetHotkeyText();
  }
  Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall THotkeyPanel::CheckboxClick(TObject *Sender)
{
  m_pEdit->Enabled = m_pCheckbox->Checked;
  m_pLabel->Enabled = m_pCheckbox->Checked;
  m_KeyInfo.enabled = m_pCheckbox->Checked;
}

//---------------------------------------------------------------------------
void __fastcall THotkeyPanel::CheckboxMouseUp(TObject *Sender, TMouseButton Button,
                                               TShiftState Shift, int X, int Y)
{
  if (m_pEdit->Enabled)
  {
    m_pEdit->SetFocus();
  }
}

//---------------------------------------------------------------------------
void __fastcall THotkeyPanel::SetCheckboxText(String caption)
{
  FCheckboxText = caption;
  m_pCheckbox->Caption = caption;
}

//---------------------------------------------------------------------------
void THotkeyPanel::SetKeyInfo(const THotkeyInfo& KeyInfo)
{
  m_KeyInfo = KeyInfo;

  m_pEdit->Text = m_KeyInfo.GetHotkeyText();
  m_pCheckbox->Checked = m_KeyInfo.enabled;
  m_pEdit->Enabled = m_KeyInfo.enabled;
  m_pLabel->Enabled = m_KeyInfo.enabled;
}

//---------------------------------------------------------------------------
THotkeyInfo THotkeyPanel::GetKeyInfo()
{
  return m_KeyInfo;
}

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(THotkeyPanel *)
{
  new THotkeyPanel(NULL);
}

//---------------------------------------------------------------------------
namespace Hotkeypanel
{
  void __fastcall PACKAGE Register()
  {
    TComponentClass classes[1] = {__classid(THotkeyPanel)};
    RegisterComponents("TopTools", classes, 0);
  }
}
//---------------------------------------------------------------------------
