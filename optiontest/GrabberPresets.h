//---------------------------------------------------------------------------
#ifndef GrabberPresetsH
#define GrabberPresetsH

#include <Classes.hpp>
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
        // TODO: replace TStringList by custom split function
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

/////////////////////////////////////////////////////////////////////////////
class TPresetList
{
    TPresetList();

    std::vector<TPreset> m_entries;
    String m_inifilepath;

//    PresetList __fastcall LoadFromIniFile(String const& path);
//    void __fastcall SaveToIniFile(String const& path, PresetList const& entries);



};

typedef std::vector<TPreset> PresetList;

#endif
