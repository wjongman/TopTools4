//---------------------------------------------------------------------------
#ifndef PresetManagerH
#define PresetManagerH

#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>

#include "Preset.h"

/////////////////////////////////////////////////////////////////////////////
class TPresetManager : public TForm
{
__published:  // IDE-managed Components
    TListView *ListView;
    TButton *bnEdit;
    TButton *bnAdd;
    TButton *bnDelete;
    TButton *bnUp;
    TButton *bnDown;
    TPanel *Panel1;
    TButton *bnOk;
    TButton *bnCancel;
    TButton *bnImport;
    TButton *bnExport;
    TPopupMenu *ListViewMenu;
    TMenuItem *miEdit;
    TMenuItem *miAdd;
    TMenuItem *miDelete;
    TMenuItem *miUp;
    TMenuItem *miDown;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnAddClick(TObject *Sender);
    void __fastcall bnDeleteClick(TObject *Sender);
    void __fastcall bnEditClick(TObject *Sender);
    void __fastcall bnImportClick(TObject *Sender);
    void __fastcall bnExportClick(TObject *Sender);
    void __fastcall bnUpClick(TObject *Sender);
    void __fastcall bnDownClick(TObject *Sender);
    void __fastcall ListViewDragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
    void __fastcall ListViewDragDrop(TObject *Sender, TObject *Source,
          int X, int Y);
    void __fastcall ListViewEditing(TObject *Sender, TListItem *Item,
          bool &AllowEdit);
    void __fastcall ListViewChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
    void __fastcall ListViewDblClick(TObject *Sender);
    void __fastcall ListViewMenuPopup(TObject *Sender);
    void __fastcall ListViewKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:  // User declarations

    String m_ToolName;
    std::vector<TPreset> m_PresetList;

    void __fastcall LoadPresets();
    void __fastcall SavePresets();
    void __fastcall UpdateList();
    void __fastcall InitListView();
    void __fastcall UpdateListView();
    void __fastcall MovePresetItem(size_t src, size_t dest);
    void __fastcall AdjustListViewColumns();
    void __fastcall UpdateButtonState();
    void __fastcall LoadFromIniFile(String const& filepath);
    void __fastcall SaveToIniFile(String const& filepath);

public:   // User declarations
    __fastcall TPresetManager(TComponent* Owner);
    __fastcall ~TPresetManager();
};

//---------------------------------------------------------------------------
extern PACKAGE TPresetManager *PresetManager;
//---------------------------------------------------------------------------
#endif
