//---------------------------------------------------------------------------
#ifndef GrabberPresetsH
#define GrabberPresetsH

#include <Classes.hpp>
#include <Inifiles.hpp>
#include <vector>

/////////////////////////////////////////////////////////////////////////////
struct TPreset
{
    String description;
    int x, y, w, h;

    //-----------------------------------------------------------------------
    TPreset()
      : description(""), x(0), y(0), w(0), h(0)
    {
    }

    //-----------------------------------------------------------------------
    TPreset(String const& name, int x, int y, int w, int h)
      : description(name), x(x), y(y), w(w), h(h)
    {
    }

    //-----------------------------------------------------------------------
    TPreset(String const& commaText)
    {
        SetCommaText(commaText);
    }

    //-----------------------------------------------------------------------
    std::vector<std::string> split(const std::string &text, char sep)
    {
        std::vector<std::string> tokens;
        std::size_t start = 0, end = 0;
        while ((end = text.find(sep, start)) != std::string::npos)
        {
            tokens.push_back(text.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(text.substr(start));
        return tokens;
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

typedef std::vector<TPreset> TPresetList;

/////////////////////////////////////////////////////////////////////////////
class TGrabberPresets
{

public:
    //-----------------------------------------------------------------------
    TPresetList LoadFromIniFile(String const& filepath)
    {
        TPresetList presets;
        TIniFile *inifile = new TIniFile(filepath);
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
    void SaveToIniFile(String const& filepath, TPresetList presets)
    {
        TIniFile *inifile = new TIniFile(filepath);
        if (inifile)
        {
            String sectionName = "grabber.presets";
            inifile->EraseSection(sectionName);
            for (size_t i = 0; i < presets.size(); i++)
            {
                String optionName = IntToStr(i+1);
                String commaText = presets[i].GetCommaText();
                inifile->WriteString(sectionName, optionName, commaText);
            }
            delete inifile;
        }
    }

};


#endif
