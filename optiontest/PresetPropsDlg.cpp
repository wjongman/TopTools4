//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PresetPropsDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

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
TPreset TPresetPropsDlg::GetPreset()
{
    m_preset.description = edTitle->Text;
//    m_preset.x = edX->Text.ToInt();
//    m_preset.y = edY->Text.ToInt();
//    m_preset.w = edW->Text.ToInt();
//    m_preset.h = edH->Text.ToInt();
    m_preset.x = udX->Position;
    m_preset.y = udY->Position;
    m_preset.w = udW->Position;
    m_preset.h = udH->Position;

    return m_preset;
}
