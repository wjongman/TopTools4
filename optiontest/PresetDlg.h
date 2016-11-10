//---------------------------------------------------------------------------
#ifndef PresetDlgH
#define PresetDlgH

#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>

#include "GrabberPresets.h"

/////////////////////////////////////////////////////////////////////////////
class TPresetDialog : public TForm
{
__published:  // IDE-managed Components
    TButton *bnEdit;
    TButton *bnAdd;
    TButton *bnRemove;
    TPanel *Panel1;
    TButton *bnOk;
    TButton *bnCancel;
    TListView *ListView;
    TButton *bnImport;
    TButton *bnExport;
    void __fastcall bnImportClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall FormContextPopup(TObject *Sender, TPoint &MousePos,
          bool &Handled);
    void __fastcall bnAddClick(TObject *Sender);
    void __fastcall bnRemoveClick(TObject *Sender);
    void __fastcall bnEditClick(TObject *Sender);
    void __fastcall bnExportClick(TObject *Sender);
    void __fastcall ListViewDragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
    void __fastcall ListViewDragDrop(TObject *Sender, TObject *Source,
          int X, int Y);
private:  // User declarations

    int m_test;
    TPresetList m_PresetList;
    TPopupMenu* m_CaptureMenu;
    bool m_bDragging;

    void __fastcall UpdateList();
    void __fastcall UpdateListView();
    void __fastcall MovePresetItem(size_t src, size_t dest);
    void __fastcall AdjustListViewColumns();
    String __fastcall SelectFileName();
    void __fastcall PopulateCaptureMenu();
    void __fastcall CaptureMenuClick(TObject *Sender);
    void __fastcall PresetMenuClick(TObject *Sender);

public:   // User declarations
    __fastcall TPresetDialog(TComponent* Owner);
    __fastcall ~TPresetDialog();
};

//---------------------------------------------------------------------------
extern PACKAGE TPresetDialog *PresetDialog;
//---------------------------------------------------------------------------
#endif
