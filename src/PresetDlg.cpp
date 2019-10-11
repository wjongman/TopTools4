//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PresetDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TPresetDlg::TPresetDlg(TComponent* Owner, const TPreset& preset)
    : TForm(Owner), m_preset(preset)
{
    Position = poOwnerFormCenter;
}

//---------------------------------------------------------------------------
__fastcall TPresetDlg::TPresetDlg(TComponent* Owner, const TPreset& preset, const TPoint& ptLeftTop)
    : TForm(Owner), m_preset(preset), m_ptLeftTop(ptLeftTop)
{
}

//---------------------------------------------------------------------------
void __fastcall TPresetDlg::FormShow(TObject *Sender)
{
    Left = m_ptLeftTop.x;
    Top = m_ptLeftTop.y;

    edName->Text = m_preset.description;
    edX->Text = m_preset.x;
    edY->Text = m_preset.y;
    edW->Text = m_preset.w;
    edH->Text = m_preset.h;

    bnOk->Enabled = !edName->Text.IsEmpty();
    SetForegroundWindow(Handle);
}

//---------------------------------------------------------------------------
void __fastcall TPresetDlg::edNameChange(TObject *Sender)
{
    // Only enable OK button when title is non-empty
    bnOk->Enabled = !edName->Text.IsEmpty();
}

//---------------------------------------------------------------------------
TPreset TPresetDlg::GetPreset()
{
    m_preset.description = edName->Text;
    m_preset.x = udX->Position;
    m_preset.y = udY->Position;
    m_preset.w = udW->Position;
    m_preset.h = udH->Position;

    return m_preset;
}


