//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PresetDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TPresetDlg::TPresetDlg(TComponent* Owner)
    : TForm(Owner)
{
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

    edTitle->Text = m_preset.description;
    edX->Text = m_preset.x;
    edY->Text = m_preset.y;
    edW->Text = m_preset.w;
    edH->Text = m_preset.h;

    bnOk->Enabled = !edTitle->Text.IsEmpty();
    SetForegroundWindow(Handle);
}

//---------------------------------------------------------------------------
void __fastcall TPresetDlg::edTitleChange(TObject *Sender)
{
    // Only enable OK button when title is non-empty
    bnOk->Enabled = !edTitle->Text.IsEmpty();
}

//---------------------------------------------------------------------------
TPreset TPresetDlg::GetPreset()
{
    m_preset.description = edTitle->Text;
    m_preset.x = udX->Position;
    m_preset.y = udY->Position;
    m_preset.w = udW->Position;
    m_preset.h = udH->Position;

    return m_preset;
}


