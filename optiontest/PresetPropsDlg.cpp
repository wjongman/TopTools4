//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PresetPropsDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//TPresetPropsDlg *PresetPropsDlg;

//---------------------------------------------------------------------------
__fastcall TPresetPropsDlg::TPresetPropsDlg(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------
__fastcall TPresetPropsDlg::TPresetPropsDlg(TComponent* Owner, const TPreset& preset)
    : TForm(Owner), m_preset(preset)
{
}

//---------------------------------------------------------------------------
void __fastcall TPresetPropsDlg::FormShow(TObject *Sender)
{
    edTitle->Text = m_preset.description;
    edX->Text = m_preset.x;
    edY->Text = m_preset.y;
    edW->Text = m_preset.w;
    edH->Text = m_preset.h;

}
//---------------------------------------------------------------------------

