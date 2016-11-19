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
#include <Menus.hpp>
#include <ActnList.hpp>

/////////////////////////////////////////////////////////////////////////////
class TPresetDialog : public TForm
{
__published:  // IDE-managed Components
    TButton *bnEdit;
    TButton *bnAdd;
    TButton *bnDelete;
    TPanel *Panel1;
    TButton *bnOk;
    TButton *bnCancel;
    TListView *ListView;
    TButton *bnImport;
    TButton *bnExport;
    TPopupMenu *ListViewMenu;
    TMenuItem *miEdit;
    TMenuItem *miAdd;
    TMenuItem *miDelete;
    TButton *bnUp;
    TButton *bnDown;
    TMenuItem *miUp;
    TMenuItem *miDown;
    TActionList *ActionList1;
    TAction *gaView;
    TAction *gaCopy;
    TAction *gaAddPreset;
    TAction *gaSave;
    TAction *gaSaveOn;
    TAction *gaPrint;
    TAction *gaPrintOn;
    TAction *gaAutoSave;
    TAction *gaAutoSaveOn;
    TAction *gaAutoSaveOptions;
    TAction *gaCancel;
    void __fastcall bnImportClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnAddClick(TObject *Sender);
    void __fastcall bnDeleteClick(TObject *Sender);
    void __fastcall bnEditClick(TObject *Sender);
    void __fastcall bnExportClick(TObject *Sender);
    void __fastcall ListViewDragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
    void __fastcall ListViewDragDrop(TObject *Sender, TObject *Source,
          int X, int Y);
    void __fastcall ListViewEditing(TObject *Sender, TListItem *Item,
          bool &AllowEdit);
    void __fastcall ListViewChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
    void __fastcall bnUpClick(TObject *Sender);
    void __fastcall bnDownClick(TObject *Sender);
    void __fastcall ListViewDblClick(TObject *Sender);
    void __fastcall ListViewMenuPopup(TObject *Sender);
    void __fastcall FormContextPopup(TObject *Sender, TPoint &MousePos,
          bool &Handled);
private:  // User declarations

    int m_test;
    TPresetList m_PresetList;
    TPopupMenu* m_CaptureMenu;
    bool m_bDragging;

    void __fastcall UpdateList();
    void __fastcall UpdateListView();
    void __fastcall MovePresetItem(size_t src, size_t dest);
    void __fastcall AdjustListViewColumns();
    void __fastcall UpdateButtonState();
    void __fastcall PopulateCaptureMenu();
    void __fastcall PopulateCaptureMenu2();
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
