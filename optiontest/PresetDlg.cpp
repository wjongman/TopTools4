//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <inifiles.hpp>
#include <dialogs.hpp>

#include "PresetDlg.h"
#include "PresetPropsDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TPresetDialog *PresetDialog;

//---------------------------------------------------------------------------
__fastcall TPresetDialog::TPresetDialog(TComponent* Owner)
    : TForm(Owner), m_inifile("..\\presets.ini")
{
}

//---------------------------------------------------------------------------
__fastcall TPresetDialog::~TPresetDialog()
{
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::FormCreate(TObject *Sender)
{
    Grid->Width = ClientWidth;
    WriteGrid(m_Presets.LoadFromIniFile(m_inifile));
}

//-------------------------------------------------------------------------
void __fastcall TPresetDialog::WriteGrid(TPresetList const& presets)
{
    Grid->RowCount = presets.size() + 1;
    Grid->Rows[0]->CommaText = "Description,  X, Y, W, H";
    for (size_t i = 0; i < presets.size(); i++)
    {
        Grid->Rows[i+1]->CommaText = presets[i].GetCommaText();
    }
}

//---------------------------------------------------------------------------
TPresetList __fastcall TPresetDialog::ReadGrid()
{
    TPresetList presets;
    for (int i = 1; i < Grid->RowCount; i++)
    {
        TPreset preset;
        String commaText = Grid->Rows[i]->CommaText;
        // Only add valid rows
        if (preset.SetCommaText(commaText))
        {
            presets.push_back(preset);
        }
    }
    return presets;
}

//-------------------------------------------------------------------------
void __fastcall TPresetDialog::ResetGrid()
{
    for (int i = 1; i < Grid->RowCount; i++)
    {
        Grid->Rows[i]->Clear();
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::bnLoadClick(TObject *Sender)
{
    String FileName = SelectFile();
    if (FileName != "")
    {
        TPresetList presets = m_Presets.LoadFromIniFile(FileName);
        ResetGrid();
        WriteGrid(presets);
    }
}

//---------------------------------------------------------------------------
String __fastcall TPresetDialog::SelectFile()
{
    String sFile = "";

    TOpenDialog* OpenDialog = new TOpenDialog(this);
    OpenDialog->Title = "Select Preset File";
    OpenDialog->Options.Clear();
    OpenDialog->Options << ofFileMustExist<< ofHideReadOnly << ofEnableSizing ;
    OpenDialog->Filter = "Preset Files (*.ini)|*.ini|All Files (*.*)|*.*";
    if (OpenDialog->Execute())
    {
        sFile = OpenDialog->Files->Strings[0].LowerCase();
    }
    delete OpenDialog;

    return sFile;
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

    TPresetList presets = m_Presets.LoadFromIniFile(m_inifile);
    for (size_t i = 0; i < presets.size(); i++)
    {
        //Grid->Rows[i+1]->CommaText = presets[i].GetCommaText();
        NewItem = new TMenuItem(PresetMenu);
        NewItem->OnClick = PresetMenuClick;
        NewItem->Caption = presets[i].description;
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
    m_Presets.SaveToIniFile(m_inifile, ReadGrid());
    Close();
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::bnCancelClick(TObject *Sender)
{
    Close();
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::bnAddClick(TObject *Sender)
{
    TPreset preset("test", 1, 2, 3, 4);
    TPresetPropsDlg* dlg = new TPresetPropsDlg(this, preset);
    dlg->ShowModal();
    Grid->RowCount++;
    delete dlg;
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::bnRemoveClick(TObject *Sender)
{
    //
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::bnEditClick(TObject *Sender)
{
  //
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::GridSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    // See what cell we are in
    m_test = 1;
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::GridSetEditText(TObject *Sender, int ACol,
      int ARow, const AnsiString Value)
{
    // If cell being edited is in the last row: add a new row
    if (ARow > Grid->RowCount)
    {
        Grid->RowCount++;
    }
}

//---------------------------------------------------------------------------

void __fastcall TPresetDialog::GridGetEditMask(TObject *Sender, int ACol,
      int ARow, AnsiString &Value)
{
  // All columns except the first can only contain numbers
  if (ACol > 1)
    Value =  "999999";
}
//---------------------------------------------------------------------------

void __fastcall TPresetDialog::GridGetEditText(TObject *Sender, int ACol,
      int ARow, AnsiString &Value)
{
    m_test = 1;
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

