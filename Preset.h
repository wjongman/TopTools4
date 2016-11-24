//---------------------------------------------------------------------------
#ifndef GrabberPresetH
#define GrabberPresetH

#include <Classes.hpp>
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
    TPreset(String const& commatext)
    {
        SetCommaText(commatext);
    }

    //-----------------------------------------------------------------------
    bool SetCommaText(String const& commaText)
    {
        bool success = false;
        // Use TStringList to perform the comma splitting
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
        String commatext;
        commatext.printf("\"%s\",%d,%d,%d,%d", description, x, y, w, h);
        return commatext;
    }

};


#endif
