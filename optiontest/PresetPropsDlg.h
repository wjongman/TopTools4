//---------------------------------------------------------------------------

#ifndef PresetPropsDlgH
#define PresetPropsDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

#include "GrabberPresets.h"

//---------------------------------------------------------------------------
class TPresetPropsDlg : public TForm
{
__published:  // IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TGroupBox *GroupBox1;
    TEdit *edTitle;
    TGroupBox *GroupBox2;
    TLabel *Label4;
    TLabel *Label5;
    TEdit *edW;
    TEdit *edH;
    TUpDown *udW;
    TUpDown *udH;
    TGroupBox *GroupBox3;
    TLabel *Label2;
    TEdit *edX;
    TUpDown *udX;
    TLabel *Label3;
    TEdit *edY;
    TUpDown *udY;
private:  // User declarations
    TPreset m_preset;
public:   // User declarations
    __fastcall TPresetPropsDlg(TComponent* Owner);
    __fastcall TPresetPropsDlg(TComponent* Owner, const TPreset& preset);
};
//---------------------------------------------------------------------------
extern PACKAGE TPresetPropsDlg *PresetPropsDlg;
//---------------------------------------------------------------------------
#endif
