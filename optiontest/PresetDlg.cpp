//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <inifiles.hpp>
#include "PresetDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TPresetDialog *PresetDialog;

//---------------------------------------------------------------------------
__fastcall TPresetDialog::TPresetDialog(TComponent* Owner)
    : TForm(Owner)
{
    //m_presetList = new TStringList();
    m_currentRowIndex = 0;
}

//---------------------------------------------------------------------------
__fastcall TPresetDialog::~TPresetDialog()
{
    //delete m_presetList;
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::FormCreate(TObject *Sender)
{
//    Grid->RowCount = 2;
//    Grid->Rows[0]->CommaText = "Description,  X, Y, W, H";
    Grid->Width = ClientWidth;
    LoadFromIniFile("..\\presets.ini");
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::LoadBtnClick(TObject *Sender)
{
    LoadFromIniFile("..\\presets.ini");
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::OKBtnClick(TObject *Sender)
{
    SaveToIniFile("..\\presets2.ini");
//    Close();
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::CancelBtnClick(TObject *Sender)
{
    Close();
}

//-------------------------------------------------------------------------
void __fastcall TPresetDialog::WriteGrid(TStringList* entries)
{
    Grid->RowCount = entries->Count + 1;
    Grid->Rows[0]->CommaText = "Description,  X, Y, W, H";
    for (int i = 0; i < entries->Count; i++)
    {
        TPreset preset;
        if (preset.SetCommaText(entries->Strings[i]))
            Grid->Rows[i+1]->CommaText = preset.GetCommaText();
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::ReadGrid(TStringList* entries)
{
    entries->Clear();
    for (int i = 1; i < Grid->RowCount; i++)
    {
        String commaText = Grid->Rows[i]->CommaText;
        // Only add rows with a non-empty description
        if (!Grid->Cells[0][i].IsEmpty())
            entries->Add(commaText);
    }
}

//-------------------------------------------------------------------------
bool __fastcall TPresetDialog::LoadFromIniFile(String const& path)
{
    // Fill m_presetList with stored values
    TIniFile *inifile = new TIniFile(path);
    if (!inifile)
        return false;

    TStringList* m_presetList = new TStringList();
//    m_presetList->Clear();
    String sectionName = "grabber.presets";

    TStringList *sectionList = new TStringList;
    inifile->ReadSection(sectionName, sectionList);
    for (int i = 0; i < sectionList->Count; i++)
    {
        m_presetList->Add(inifile->ReadString(sectionName, IntToStr(i+1), ""));
    }
    delete sectionList;
    delete inifile;

    WriteGrid(m_presetList);
    delete m_presetList;
    return true;
}

//-------------------------------------------------------------------------
bool __fastcall TPresetDialog::SaveToIniFile(String const& path)
{
    TIniFile *inifile = new TIniFile(path);
    if (!inifile)
        return false;

    String sectionName = "grabber.presets";
    for (int i = 1; i < Grid->RowCount; i++)
    {
        String commaText = Grid->Rows[i]->CommaText;
        // Only add rows with a non-empty description
        if (!Grid->Cells[0][i].IsEmpty())
        {
            String optionName = IntToStr(i);
            inifile->WriteString(sectionName, optionName, commaText);
        }
    }

    delete inifile;
    return true;
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::GridSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
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
  if (ACol > 0)
    Value =  "999999";
}
//---------------------------------------------------------------------------

void __fastcall TPresetDialog::GridGetEditText(TObject *Sender, int ACol,
      int ARow, AnsiString &Value)
{
    m_test = 1;
}
//---------------------------------------------------------------------------

