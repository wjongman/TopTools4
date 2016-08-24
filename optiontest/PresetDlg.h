//---------------------------------------------------------------------------

#ifndef PresetDlgH
#define PresetDlgH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>

#include "GrabberPresets.h"

/////////////////////////////////////////////////////////////////////////////
class TPresetDialog : public TForm
{
__published:  // IDE-managed Components
    TStringGrid *Grid;
    TPanel *Panel1;
    TButton *LoadBtn;
    TButton *OKBtn;
    TButton *CancelBtn;
    void __fastcall LoadBtnClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall CancelBtnClick(TObject *Sender);
    void __fastcall OKBtnClick(TObject *Sender);
    void __fastcall GridSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
    void __fastcall GridSetEditText(TObject *Sender, int ACol, int ARow,
          const AnsiString Value);
    void __fastcall GridGetEditMask(TObject *Sender, int ACol, int ARow,
          AnsiString &Value);
    void __fastcall GridGetEditText(TObject *Sender, int ACol, int ARow,
          AnsiString &Value);
private:  // User declarations

    int m_test;

    PresetList __fastcall ReadGrid();
    void __fastcall WriteGrid(PresetList const& entries);

    PresetList __fastcall LoadFromIniFile(String const& path);
    void __fastcall SaveToIniFile(String const& path, PresetList const& entries);

public:   // User declarations
    __fastcall TPresetDialog(TComponent* Owner);
    __fastcall ~TPresetDialog();
};
//---------------------------------------------------------------------------
extern PACKAGE TPresetDialog *PresetDialog;
//---------------------------------------------------------------------------
#endif
