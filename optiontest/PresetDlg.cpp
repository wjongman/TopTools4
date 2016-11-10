//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PresetDlg.h"
#include "PresetPropsDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TPresetDialog *PresetDialog;

//---------------------------------------------------------------------------
__fastcall TPresetDialog::TPresetDialog(TComponent* Owner)
    : TForm(Owner), m_bDragging(false)
{
}

//---------------------------------------------------------------------------
__fastcall TPresetDialog::~TPresetDialog()
{
    delete m_CaptureMenu;
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::FormCreate(TObject *Sender)
{
    ListView->ViewStyle = vsReport;
    ListView->RowSelect = true;
    ListView->DragMode = dmAutomatic;
    ListView->GridLines = true;
    TListColumn* pColumn;
    pColumn = ListView->Columns->Add();
    pColumn->Caption = "Name";
    pColumn->Width = 100;
    pColumn = ListView->Columns->Add();
    pColumn->Caption = "X";
    pColumn->Width = 45;
    pColumn = ListView->Columns->Add();
    pColumn->Caption = "Y";
    pColumn->Width = 45;
    pColumn = ListView->Columns->Add();
    pColumn->Caption = "W";
    pColumn->Width = 45;
    pColumn = ListView->Columns->Add();
    pColumn->Caption = "H";
    pColumn->Width = 45;

    TGrabberPresets gp;
    m_PresetList = gp.LoadFromIniFile("..\\presets.ini");
    UpdateList();
}

//-------------------------------------------------------------------------
void __fastcall TPresetDialog::UpdateList()
{
    ListBox->Items->Clear();
    for (size_t i = 0; i < m_PresetList.size(); i++)
    {
        ListBox->Items->Add(m_PresetList[i].description);
    }
    UpdateListView();
}

//-------------------------------------------------------------------------
void __fastcall TPresetDialog::UpdateListView()
{
    ListView->Items->Clear();

    for (size_t i = 0; i < m_PresetList.size(); i++)
    {
        TListItem* li = ListView->Items->Add();
        li->Caption = m_PresetList[i].description;
        li->SubItems->Add(m_PresetList[i].x);
        li->SubItems->Add(m_PresetList[i].y);
        li->SubItems->Add(m_PresetList[i].w);
        li->SubItems->Add(m_PresetList[i].h);
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::bnEditClick(TObject *Sender)
{
    int index = ListBox->ItemIndex;
    if (index >= 0)
    {
        TPreset preset = m_PresetList[index];
        TPresetPropsDlg* dlg = new TPresetPropsDlg(this, preset);
        if (dlg->ShowModal() == mrOk)
        {
            m_PresetList[index] = dlg->GetPreset();
            UpdateList();
        }
        delete dlg;
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::bnAddClick(TObject *Sender)
{
    TPresetPropsDlg* dlg = new TPresetPropsDlg(this);
    if (dlg->ShowModal() == mrOk)
    {
        m_PresetList.push_back(dlg->GetPreset());
        UpdateList();
    }
    delete dlg;
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::bnRemoveClick(TObject *Sender)
{
    int index = ListBox->ItemIndex;
    if (index >= 0)
    {
        m_PresetList.erase(m_PresetList.begin() + index);
        UpdateList();
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::bnImportClick(TObject *Sender)
{
    String FileName = SelectFileName();
    if (FileName != "")
    {
        TGrabberPresets gp;
        m_PresetList = gp.LoadFromIniFile(FileName);
        UpdateList();
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::bnExportClick(TObject *Sender)
{
    TSaveDialog* dlg = new TSaveDialog(this);
    dlg->Title = "Save Preset File";
    dlg->Filter = "Preset Files (*.ini)|*.ini|All Files (*.*)|*.*";
    if (dlg->Execute())
    {
        String sFile = dlg->Files->Strings[0].LowerCase();
        TGrabberPresets gp;
        gp.SaveToIniFile(sFile, m_PresetList);
    }
    delete dlg;
}

//---------------------------------------------------------------------------
String __fastcall TPresetDialog::SelectFileName()
{
    String sFile = "";

    TOpenDialog* dlg = new TOpenDialog(this);
    dlg->Title = "Select Preset File";
    dlg->Options.Clear();
    dlg->Options << ofFileMustExist<< ofHideReadOnly << ofEnableSizing ;
    dlg->Filter = "Preset Files (*.ini)|*.ini|All Files (*.*)|*.*";
    if (dlg->Execute())
    {
        sFile = dlg->Files->Strings[0].LowerCase();
    }
    delete dlg;
    return sFile;
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::CaptureMenuClick(TObject *Sender)
{
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::PresetMenuClick(TObject *Sender)
{
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::bnOkClick(TObject *Sender)
{
    Close();
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::bnCancelClick(TObject *Sender)
{
    Close();
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::FormContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
    PopulateCaptureMenu();
    TPoint ptAbs = ClientToScreen(MousePos);
    m_CaptureMenu->Popup(ptAbs.x, ptAbs.y);
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::ListBoxMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button != mbLeft)
        return;

    // Make sure we are on a list item
    if (ListBox->ItemAtPos(Point(X, Y), true) >= 0 && !m_bDragging)
    {
        ListBox->BeginDrag(false, ListBox->ItemHeight);
        m_bDragging = true;
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::ListBoxMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  //
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::ListBoxMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    m_test = 1;
  //
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::ListBoxStartDrag(TObject *Sender,
      TDragObject *&DragObject)
{
    m_test = 1;
//
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::ListBoxEndDrag(TObject *Sender,
      TObject *Target, int X, int Y)
{
    m_bDragging = false;
    m_test = 1;
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::ListBoxDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
    // We only accept drops within the listbox
    int index = ListBox->ItemAtPos(Point(X, Y), true);
    Accept = (index >= 0 && Source == ListBox);
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::ListBoxDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
    int index = ListBox->ItemAtPos(Point(X, Y), true);
    if (index >= 0 && index != ListBox->ItemIndex)
    {
        // Move item at ItemIndex above item at index
        MovePresetItem(ListBox->ItemIndex, index);
        ListBox->Items->Move(ListBox->ItemIndex, index);
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::MovePresetItem(size_t src, size_t dest)
{
    if (dest > m_PresetList.size())
        dest = m_PresetList.size();

    TPreset tmp = m_PresetList[src];
    m_PresetList.erase(m_PresetList.begin() + src);
    m_PresetList.insert(m_PresetList.begin() + dest, tmp);
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::PopulateCaptureMenu()
{
    if (!m_CaptureMenu)
    {
        m_CaptureMenu = new TPopupMenu(this);
    }

    // Start with an empty menu
    m_CaptureMenu->Items->Clear();

    // Populate the menu
    TMenuItem *NewItem;

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Take Snapshot";
    NewItem->Default = true;
    NewItem->Hint = "View";
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Copy To Clipboard";
    NewItem->Hint = "Copy";
//    NewItem->ShortCut = ShortCut(Word('C'), TShiftState() << ssCtrl);
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Save To File...";
    NewItem->Hint = "Save";
//    NewItem->ShortCut = ShortCut(Word('S'), TShiftState() << ssCtrl);
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Save && Grab More...";
    NewItem->Hint = "SaveOn";
//    NewItem->ShortCut = ShortCut(Word('S'), TShiftState() << ssCtrl << ssShift);
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Print...";
    NewItem->Hint = "Print";
//    NewItem->ShortCut = ShortCut(Word('P'), TShiftState() << ssCtrl);
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Print && Grab More...";
    NewItem->Hint = "PrintOn";
//    NewItem->ShortCut = ShortCut(Word('P'), TShiftState() << ssCtrl << ssShift);
    NewItem->Enabled = false;
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Auto Save";
    NewItem->Hint = "AutoSave";
//    NewItem->ShortCut = ShortCut(Word('A'), TShiftState() << ssCtrl);
    //NewItem->Enabled = m_AutoSaver.Enabled;
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Auto Save && Grab More";
    NewItem->Hint = "AutoSaveOn";
//    NewItem->ShortCut = ShortCut(Word('A'), TShiftState() << ssCtrl << ssShift);
    //NewItem->Enabled = m_AutoSaver.Enabled;
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Auto Save Options...";
    NewItem->Hint = "AutoSaveOptions";
//    NewItem->Enabled = m_CaptureOptions.AutoSave;
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    // Presets Submenu ------------------
    TMenuItem* PresetMenu = new TMenuItem(m_CaptureMenu);
    PresetMenu->Caption = "Presets";
    m_CaptureMenu->Items->Add(PresetMenu);

    NewItem = new TMenuItem(PresetMenu);
 //   NewItem->OnClick = PresetMenuClick;
    NewItem->Caption = "Add Preset";
    NewItem->Hint = "AddPreset";
    PresetMenu->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(PresetMenu);
    NewItem->Caption = "-";
    PresetMenu->Add(NewItem);

    for (size_t i = 0; i < m_PresetList.size(); i++)
    {
        NewItem = new TMenuItem(PresetMenu);
        NewItem->OnClick = PresetMenuClick;
        NewItem->Caption = m_PresetList[i].description;
        NewItem->Tag = i;
        PresetMenu->Add(NewItem);
    }


    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Cancel";
    //NewItem->Default = true;
    NewItem->Hint = "Hide";
    m_CaptureMenu->Items->Add(NewItem);


#ifdef _DEBUG
    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

#endif
}


//---------------------------------------------------------------------------
void __fastcall TPresetDialog::ListViewDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
    // We only accept drops from within the listview
    Accept = (Source == ListView) && PtInRect(&(ListView->ClientRect), Point(X, Y));
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::ListViewDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
    // We only accept drops from within the listview
    if (Source != ListView)
        return;

    // Find item we are hovering above
    TListItem* liTarget = ListView->GetItemAt(X, Y);
    if (!liTarget)
    {
        // We are either on the header or on an empty row
        TListItem* liFirst = ListView->Items->Item[0];
        TRect rcFirstItem = liFirst->DisplayRect(drBounds);
        if (Y < rcFirstItem.top)
            liTarget = liFirst;

        TListItem* liLast = ListView->Items->Item[ListView->Items->Count - 1];
        TRect rcLastItem = liLast->DisplayRect(drBounds);
        if (Y > rcFirstItem.bottom)
            liTarget = liLast;
    }
    if (liTarget == ListView->Selected)
        // We are above the dragged item itself
        return;

    // Rearrange underlying PresetList
    int indexFrom = ListView->Selected->Index;
    int indexTo = liTarget->Index;
    MovePresetItem(indexFrom, indexTo);

    // Now rebuild the ListView
    UpdateListView();
}

//---------------------------------------------------------------------------

