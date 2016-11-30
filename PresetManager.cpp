//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PersistOptions.h"
#include "PresetManager.h"
#include "PresetDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TPresetManager *PresetManager;

//---------------------------------------------------------------------------
__fastcall TPresetManager::TPresetManager(TComponent* Owner)
    : TForm(Owner)
{
    m_ToolName = "capture\\presets";
}

//---------------------------------------------------------------------------
__fastcall TPresetManager::~TPresetManager()
{
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::FormShow(TObject *Sender)
{
    LoadPresets();
    UpdateButtonState();
    UpdateListView();
    if (ListView->Items->Count > 0)
        ListView->Items->Item[0]->Selected = true;
    ListView->SetFocus();
    SetForegroundWindow(Handle);
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::FormCreate(TObject *Sender)
{
    InitListView();
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::InitListView()
{
    ListView->ViewStyle = vsReport;
    ListView->RowSelect = true;
    ListView->DragMode = dmAutomatic;
    ListView->GridLines = true;

    TListColumn* pColumn;
    pColumn = ListView->Columns->Add();
    pColumn->Caption = "Name";

    pColumn = ListView->Columns->Add();
    pColumn->Caption = "X";
    pColumn->Alignment = taRightJustify ;

    pColumn = ListView->Columns->Add();
    pColumn->Caption = "Y";
    pColumn->Alignment = taRightJustify ;

    pColumn = ListView->Columns->Add();
    pColumn->Caption = "W";
    pColumn->Alignment = taRightJustify ;

    pColumn = ListView->Columns->Add();
    pColumn->Caption = "H";
    pColumn->Alignment = taRightJustify ;
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::LoadPresets()
{
    m_PresetList.clear();
    for (int i = 1; i < 99; i++) // 99 presets should be enough for everyone :-)
    {
        String commatext = g_ToolOptions.Get(m_ToolName, IntToStr(i), "");
        if (commatext.IsEmpty())
            break;  // Only consecutive numbers
        m_PresetList.push_back(TPreset(commatext));
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::SavePresets()
{
    g_ToolOptions.ClearOptions(m_ToolName);
    for (size_t i = 1; i <= m_PresetList.size(); i++)
    {
        g_ToolOptions.Set(m_ToolName, IntToStr(i), m_PresetList[i-1].GetCommaText());
    }
}

//-------------------------------------------------------------------------
void __fastcall TPresetManager::UpdateListView()
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
    AdjustListViewColumns();
}

//-------------------------------------------------------------------------
void __fastcall TPresetManager::AdjustListViewColumns()
{
    int cellwidth = 45;
    int clientwidth = ListView->ClientRect.Width();
    ListView->Columns->Items[0]->Width = clientwidth - 4 * cellwidth;

    int count = ListView->Columns->Count;
    for (int i = 1; i < count; i++)
    {
        ListView->Columns->Items[i]->Width = cellwidth;
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::bnEditClick(TObject *Sender)
{
    if (!ListView->Selected)
        return;

    int index = ListView->Selected->Index;
    if (index >= 0)
    {
        int offset = GetSystemMetrics(SM_CYCAPTION);
        TPreset preset = m_PresetList[index];
        TPresetDlg* dlg = new TPresetDlg(this, preset, Point(Left + offset, Top + offset));
        dlg->Caption = "Edit Preset";
        if (dlg->ShowModal() == mrOk)
        {
            m_PresetList[index] = dlg->GetPreset();
            UpdateListView();
        }
        delete dlg;
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::bnAddClick(TObject *Sender)
{
    TPreset preset;
    int offset = GetSystemMetrics(SM_CYCAPTION);
    TPresetDlg* dlg = new TPresetDlg(this, preset, Point(Left + offset, Top + offset));
    dlg->Caption = "Add Preset";
    if (dlg->ShowModal() == mrOk)
    {
        m_PresetList.push_back(dlg->GetPreset());
        UpdateListView();
    }
    delete dlg;
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::bnDeleteClick(TObject *Sender)
{
    if (!ListView->Selected)
        return;

    int index = ListView->Selected->Index;
    if (index >= 0)
    {
        m_PresetList.erase(m_PresetList.begin() + index);
        UpdateListView();
        // Select next item (if any)
        int count = ListView->Items->Count;
        if (count > 0 && index < count)
        {
            ListView->Items->Item[index]->Selected = true;
            ListView->SetFocus();
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::bnUpClick(TObject *Sender)
{
    // Rearrange underlying PresetList
    int indexFrom = ListView->Selected->Index;
    if (indexFrom > 0)
    {
        MovePresetItem(indexFrom, indexFrom - 1);
    }
    // Now rebuild the ListView
    UpdateListView();
    ListView->Items->Item[indexFrom - 1]->Selected = true;
    ListView->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::bnDownClick(TObject *Sender)
{
    // Rearrange underlying PresetList
    int indexFrom = ListView->Selected->Index;
    if (indexFrom < ListView->Items->Count - 1)
    {
        MovePresetItem(indexFrom, indexFrom + 1);
    }
    // Now rebuild the ListView
    UpdateListView();
    ListView->Items->Item[indexFrom + 1]->Selected = true;
    ListView->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::bnImportClick(TObject *Sender)
{
    TOpenDialog* dlg = new TOpenDialog(this);
    dlg->Title = "Select Preset File";
    dlg->Options.Clear();
    dlg->Options << ofFileMustExist<< ofHideReadOnly << ofEnableSizing ;
    dlg->Filter = "Preset Files (*.ini)|*.ini|All Files (*.*)|*.*";
    if (dlg->Execute())
    {
        String filename = dlg->Files->Strings[0];
        LoadFromIniFile(filename);
        UpdateListView();
    }
    delete dlg;
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::bnExportClick(TObject *Sender)
{
    TSaveDialog* dlg = new TSaveDialog(this);
    dlg->Title = "Save Preset File";
    dlg->Filter = "Preset Files (*.ini)|*.ini|All Files (*.*)|*.*";
    if (dlg->Execute())
    {
        String filename = dlg->Files->Strings[0];
        SaveToIniFile(filename);
    }
    delete dlg;
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::bnOkClick(TObject *Sender)
{
    SavePresets();
    ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::bnCancelClick(TObject *Sender)
{
    ModalResult = mrCancel;
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::ListViewDblClick(TObject *Sender)
{
    if (ListView->Selected)
    {
        bnEditClick(Sender);
    }
    else
    {
        bnAddClick(Sender);
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::ListViewEditing(TObject *Sender,
      TListItem *Item, bool &AllowEdit)
{
    // Read only
    AllowEdit = false;
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::ListViewDragOver(TObject *Sender,
      TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
    // We only accept drops from within the listview
    Accept = (Source == ListView) && PtInRect(&(ListView->ClientRect), Point(X, Y));
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::ListViewDragDrop(TObject *Sender,
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
    ListView->Items->Item[indexTo]->Selected = true;
    ListView->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::ListViewKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if (Key == VK_DELETE)
       bnDeleteClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::ListViewMenuPopup(TObject *Sender)
{
    miEdit->Enabled = ListView->Selected;
    miDelete->Enabled = ListView->Selected;
    miUp->Enabled = ListView->Selected && ListView->Selected->Index > 0;
    miDown->Enabled = ListView->Selected && ListView->Selected->Index < ListView->Items->Count - 1;
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::ListViewChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
//    AdjustListViewColumns();
    UpdateButtonState();
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::UpdateButtonState()
{

    bnExport->Enabled = ListView->Items->Count > 0;
    bnEdit->Enabled = ListView->Selected;
    bnDelete->Enabled = ListView->Selected;
    bnUp->Enabled = ListView->Selected && ListView->Selected->Index > 0;
    bnDown->Enabled = ListView->Selected && ListView->Selected->Index < ListView->Items->Count - 1;
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::MovePresetItem(size_t src, size_t dest)
{
    if (dest > m_PresetList.size())
        dest = m_PresetList.size();

    TPreset tmp = m_PresetList[src];
    m_PresetList.erase(m_PresetList.begin() + src);
    m_PresetList.insert(m_PresetList.begin() + dest, tmp);
}

//---------------------------------------------------------------------------
void __fastcall TPresetManager::LoadFromIniFile(String const& filepath)
{
    TIniFile *inifile = new TIniFile(filepath);
    if (inifile)
    {
        TStringList *sectionList = new TStringList;
        if (sectionList)
        {
            inifile->ReadSection(m_ToolName, sectionList);
            m_PresetList.clear();
            for (int i = 0; i < sectionList->Count; i++)
            {
                TPreset preset;
                String commaText = inifile->ReadString(m_ToolName, IntToStr(i+1), "");
                if (preset.SetCommaText(commaText))
                    m_PresetList.push_back(preset);
            }
            delete sectionList;
        }
        delete inifile;
    }
}

//-----------------------------------------------------------------------------
void __fastcall TPresetManager::SaveToIniFile(String const& filepath)
{
    TIniFile *inifile = new TIniFile(filepath);
    if (inifile)
    {
        inifile->EraseSection(m_ToolName);
        for (size_t i = 0; i < m_PresetList.size(); i++)
        {
            String optionName = IntToStr(i+1);
            String commaText = m_PresetList[i].GetCommaText();
            inifile->WriteString(m_ToolName, optionName, commaText);
        }
        delete inifile;
    }
}




