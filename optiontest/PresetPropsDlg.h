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
    TGroupBox *GroupBox2;
    TGroupBox *GroupBox3;
    TEdit *edTitle;
    TEdit *edX;
    TEdit *edY;
    TEdit *edW;
    TEdit *edH;
    TLabel *lbX;
    TLabel *lbY;
    TLabel *lbW;
    TLabel *lbH;
    TUpDown *udX;
    TUpDown *udY;
    TUpDown *udW;
    TUpDown *udH;
    void __fastcall FormShow(TObject *Sender);

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
