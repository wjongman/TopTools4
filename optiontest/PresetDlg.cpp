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
}

//---------------------------------------------------------------------------
__fastcall TPresetDialog::~TPresetDialog()
{
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::FormCreate(TObject *Sender)
{
    Grid->Width = ClientWidth;
    PresetList presets = LoadFromIniFile("..\\presets.ini");
    WriteGrid(presets);
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::LoadBtnClick(TObject *Sender)
{
    PresetList presets = LoadFromIniFile("..\\presets.ini");
    WriteGrid(presets);
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::OKBtnClick(TObject *Sender)
{
    PresetList presets = ReadGrid();
    SaveToIniFile("..\\presets2.ini", presets);
//    Close();
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::CancelBtnClick(TObject *Sender)
{
    Close();
}

//-------------------------------------------------------------------------
void __fastcall TPresetDialog::WriteGrid(PresetList const& presets)
{
    Grid->RowCount = presets.size() + 1;
    Grid->Rows[0]->CommaText = "Description,  X, Y, W, H";
    for (size_t i = 0; i < presets.size(); i++)
    {
        Grid->Rows[i+1]->CommaText = presets[i].GetCommaText();
    }
}

//---------------------------------------------------------------------------
PresetList __fastcall TPresetDialog::ReadGrid()
{
    PresetList presets;
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
PresetList __fastcall TPresetDialog::LoadFromIniFile(String const& path)
{
    PresetList presets;

    TIniFile *inifile = new TIniFile(path);
    if (inifile)
    {
        String sectionName = "grabber.presets";

        TStringList *sectionList = new TStringList;
        if (sectionList)
        {
            inifile->ReadSection(sectionName, sectionList);
            for (int i = 0; i < sectionList->Count; i++)
            {
                TPreset preset;
                String commaText = inifile->ReadString(sectionName, IntToStr(i+1), "");
                if (preset.SetCommaText(commaText))
                    presets.push_back(preset);
            }
            delete sectionList;
        }
        delete inifile;
    }
    return presets;
}

//-------------------------------------------------------------------------
void __fastcall TPresetDialog::SaveToIniFile(String const& path, PresetList const& presets)
{
    TIniFile *inifile = new TIniFile(path);
    if (inifile)
    {
        String sectionName = "grabber.presets";
        for (size_t i = 0; i < presets.size(); i++)
        {
            String optionName = IntToStr(i+1);
            String commaText = presets[i].GetCommaText();
            inifile->WriteString(sectionName, optionName, commaText);
        }

        delete inifile;
    }
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

