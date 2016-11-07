//---------------------------------------------------------------------------

#ifndef PresetDlgH
#define PresetDlgH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>

#include "GrabberPresets.h"
#include <ComCtrls.hpp>

/////////////////////////////////////////////////////////////////////////////
class TPresetDialog : public TForm
{
__published:  // IDE-managed Components
    TStringGrid *Grid;
    TPanel *Panel1;
    TButton *bnLoad;
    TButton *bnOk;
    TButton *bnCancel;
    TButton *bnAdd;
    TButton *bnRemove;
    TButton *bnEdit;
    void __fastcall bnLoadClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall GridSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
    void __fastcall GridSetEditText(TObject *Sender, int ACol, int ARow,
          const AnsiString Value);
    void __fastcall GridGetEditMask(TObject *Sender, int ACol, int ARow,
          AnsiString &Value);
    void __fastcall GridGetEditText(TObject *Sender, int ACol, int ARow,
          AnsiString &Value);
    void __fastcall FormContextPopup(TObject *Sender, TPoint &MousePos,
          bool &Handled);
    void __fastcall bnAddClick(TObject *Sender);
    void __fastcall bnRemoveClick(TObject *Sender);
    void __fastcall bnEditClick(TObject *Sender);
private:  // User declarations

    int m_test;
    String m_inifile;
    TGrabberPresets m_Presets;
    TPresetList __fastcall ReadGrid();
    void __fastcall WriteGrid(TPresetList const& entries);
    void __fastcall ResetGrid();
    String __fastcall SelectFile();

    void __fastcall PopulateCaptureMenu();
    void __fastcall CaptureMenuClick(TObject *Sender);
    void __fastcall PresetMenuClick(TObject *Sender);
    TPopupMenu* m_CaptureMenu;

public:   // User declarations
    __fastcall TPresetDialog(TComponent* Owner);
    __fastcall ~TPresetDialog();
};
//---------------------------------------------------------------------------
extern PACKAGE TPresetDialog *PresetDialog;
//---------------------------------------------------------------------------
#endif
