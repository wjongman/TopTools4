//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PresetDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPresetDialog *PresetDialog;
//---------------------------------------------------------------------------
__fastcall TPresetDialog::TPresetDialog(TComponent* Owner)
    : TForm(Owner)
{
    m_presetList = new TStringList();
    m_currentRowIndex = 0;
}

//---------------------------------------------------------------------------
__fastcall TPresetDialog::~TPresetDialog()
{
    delete m_presetList;
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::Load(String const& filename)
{
    Grid->RowCount = 2;
    m_presetList->LoadFromFile(filename);
    int count = m_presetList->Count;
    Grid->RowCount = count;
    int i = 0;
    while (i < count)
    {
        Grid->Rows[i+1]->CommaText = m_presetList->Strings[i];
        i++;
    }
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::Save(String const& filename)
{
    // Read the grid into a stringlist and save to file
    TStringList* presets = new TStringList();
    for (int i = 1; i < Grid->RowCount; i++)
    {
        String commaText = Grid->Rows[i]->CommaText;
        // Only add rows with a non-empty description
        if (!Grid->Cells[0][i].IsEmpty())
            presets->Add(commaText);
    }
    presets->SaveToFile(filename);
    delete presets;
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::FormCreate(TObject *Sender)
{

    Grid->RowCount = 2;
    Grid->Rows[0]->CommaText = "Description,  X, Y, W, H";
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::GridSetEditText(TObject *Sender, int ACol,
      int ARow, const AnsiString Value)
{
    // See if we need to add a row
    if (ARow == Grid->RowCount - 1 && !Value.IsEmpty())
    {
        Grid->RowCount++;
    }
    // Save the changed row


}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::GridSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    // See if we need to add a row
    if (ARow == Grid->RowCount - 1 && CanSelect)
    {
        Grid->RowCount++;
    }

}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::LoadBtnClick(TObject *Sender)
{
    Load("..\\presets.ini");
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::OKBtnClick(TObject *Sender)
{
    Save("..\\presets.ini");
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::CancelBtnClick(TObject *Sender)
{
    Close();
}

//---------------------------------------------------------------------------
