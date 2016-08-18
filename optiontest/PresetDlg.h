//---------------------------------------------------------------------------

#ifndef PresetDlgH
#define PresetDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPresetDialog : public TForm
{
__published:	// IDE-managed Components
    TStringGrid *Grid;
    TPanel *Panel1;
    TButton *LoadBtn;
    TButton *OKBtn;
    TButton *CancelBtn;
    void __fastcall LoadBtnClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall CancelBtnClick(TObject *Sender);
    void __fastcall OKBtnClick(TObject *Sender);
private:	// User declarations

    TStringList* m_presetList;
    int m_currentRowIndex;
    bool __fastcall LoadFromIniFile(String const& path);
    bool __fastcall SaveToIniFile(String const& path);

public:		// User declarations
    __fastcall TPresetDialog(TComponent* Owner);
    __fastcall ~TPresetDialog();
};
//---------------------------------------------------------------------------
extern PACKAGE TPresetDialog *PresetDialog;
//---------------------------------------------------------------------------
#endif
