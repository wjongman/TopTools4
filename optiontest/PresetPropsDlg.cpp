//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PresetPropsDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPresetPropsDlg *PresetPropsDlg;
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

