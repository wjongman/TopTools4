//---------------------------------------------------------------------------

#ifndef PresetDlgH
#define PresetDlgH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#include <vector>

/////////////////////////////////////////////////////////////////////////////
struct TPreset
{
    String description;
    int x, y, w, h;

    //-----------------------------------------------------------------------
    TPreset() : description(""), x(0), y(0), w(0), h(0)
    {
    }

    //-----------------------------------------------------------------------
    TPreset(String const& commaText)
    {
        SetCommaText(commaText);
    }

    //-----------------------------------------------------------------------
    bool SetCommaText(String const& commaText)
    {
        bool success = false;
        TStringList* list = new TStringList();
        try
        {
            list->CommaText = commaText;
            description = list->Strings[0];
            try
            {
                x = StrToInt(list->Strings[1]);
                y = StrToInt(list->Strings[2]);
                w = StrToInt(list->Strings[3]);
                h = StrToInt(list->Strings[4]);
                success = true;
            }
            catch(const EConvertError &E)
            {
                description = "";
                x = y = w = h = 0;
                success = false;
            }
        }
        __finally
        {
            delete list;
        }
        return success;
    }

    //-----------------------------------------------------------------------
    String GetCommaText() const
    {
        String commaText;
        commaText.printf("\"%s\",%d,%d,%d,%d", description, x, y, w, h);
        return commaText;
    }
};

typedef std::vector<TPreset> PresetList;

/////////////////////////////////////////////////////////////////////////////
class TPresetDialog : public TForm
{
__published:  // IDE-managed Components
    TStringGrid *Grid;
    TPanel *Panel1;
    TButton *LoadBtn;
    TButton *OKBtn;
    TButton *CancelBtn;
    void __fastcall LoadBtnClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall CancelBtnClick(TObject *Sender);
    void __fastcall OKBtnClick(TObject *Sender);
    void __fastcall GridSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
    void __fastcall GridSetEditText(TObject *Sender, int ACol, int ARow,
          const AnsiString Value);
    void __fastcall GridGetEditMask(TObject *Sender, int ACol, int ARow,
          AnsiString &Value);
    void __fastcall GridGetEditText(TObject *Sender, int ACol, int ARow,
          AnsiString &Value);
private:  // User declarations

    int m_test;

    PresetList __fastcall ReadGrid();
    void __fastcall WriteGrid(PresetList const& entries);

    PresetList __fastcall LoadFromIniFile(String const& path);
    void __fastcall SaveToIniFile(String const& path, PresetList const& entries);

public:   // User declarations
    __fastcall TPresetDialog(TComponent* Owner);
    __fastcall ~TPresetDialog();
};
//---------------------------------------------------------------------------
extern PACKAGE TPresetDialog *PresetDialog;
//---------------------------------------------------------------------------
#endif
