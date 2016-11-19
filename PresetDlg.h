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
class TPresetDlg : public TForm
{
__published:  // IDE-managed Components
    TButton *bnOk;
    TButton *bnCancel;
    TEdit *edTitle;
    TLabel *lbX;
    TEdit *edX;
    TUpDown *udX;
    TLabel *lbY;
    TEdit *edY;
    TUpDown *udY;
    TLabel *lbW;
    TEdit *edW;
    TUpDown *udW;
    TLabel *lbH;
    TEdit *edH;
    TUpDown *udH;
    TLabel *lbName;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall edTitleChange(TObject *Sender);

private:  // User declarations
    TPreset m_preset;
    TPoint m_ptLeftTop;

public:   // User declarations
    __fastcall TPresetDlg(TComponent* Owner);
    __fastcall TPresetDlg(TComponent* Owner, const TPreset& preset, const TPoint& ptLeftTop);

    TPreset GetPreset();
};

//---------------------------------------------------------------------------
extern PACKAGE TPresetDlg *PresetDlg;
//---------------------------------------------------------------------------
#endif
