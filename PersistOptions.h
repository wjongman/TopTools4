//---------------------------------------------------------------------------
#ifndef TopToolsPersistOptionsH
#define TopToolsPersistOptionsH

#include <inifiles.hpp>
#include <registry.hpp>
#include <map>
#include "ToolOptions.h"

// Global String for registry access
extern const String g_RegBaseKey;

/////////////////////////////////////////////////////////////////////////////
enum TDoubleClickOpen
{
    dcoControl = 1,
    dcoRuler = 2,
    dcoLoupe = 4,
    dcoInfo = 8,
    dcoBaseconv = 16
};

/////////////////////////////////////////////////////////////////////////////
//
// Encapsulation of map that holds toolname - TOptionMap pairs
// This class is a collection of all existing tool-options.
// It uses a TOptionMap for each tool so it allows access by
// toolname to each tool's options
//
// Implements run-mode detection (portable / inifile / registry)
// Load and save actions are delegated to all known TOptionMap entries
//
/////////////////////////////////////////////////////////////////////////////
class TPersistOptions
{
private:
    typedef std::map<String, TOption> TOptionMap;
    typedef std::map<String, TOption>::iterator option_iterator;
    typedef std::map<String, TOptionMap> TOptionMaps;
    typedef std::map<String, TOptionMap>::iterator option_map_iterator;

    TOptionMaps m_OptionMaps;
    String m_RegBaseKey;
    String m_IniFilePath;
    TRunMode m_RunMode;

public:
    //-------------------------------------------------------------------------
    TPersistOptions()
    {
        InitOptions();
        m_RegBaseKey = g_RegBaseKey;
        m_IniFilePath = ChangeFileExt(ParamStr(0), ".ini");
    }

    //-------------------------------------------------------------------------
    void Load()
    {
        // See how the user wants to run the app
        if (ParamCount() > 0 && ParamStr(1) == "-p")
        {
            m_RunMode = rmPortable;
        }
        else if (ParamCount() > 0 && ParamStr(1) == "-r")
        {
            m_RunMode = rmRegistry;
        }
        else if (ParamCount() > 0 && ParamStr(1) == "-i")
        {
            m_RunMode = rmIniFile;
            // Optional ParamStr(2) to indicate target inifile
            if (ParamCount() > 1)
            {
                String IniFilePath = ParamStr(2);
                if (FileExists(IniFilePath))
                {
                    m_IniFilePath = IniFilePath;
                }
            }
        }
        else if (RegKeyExists())
        {
            m_RunMode = rmRegistry;
        }
        else if (IniFileExists())
        {
            m_RunMode = rmIniFile;
        }
        else
        {
            // todo: offer dialog at this point?
            m_RunMode = rmPortable;
        }

        switch (m_RunMode)
        {
        case rmIniFile:
            LoadFromIniFile();
            break;
        case rmRegistry:
            LoadFromRegistry();
            break;
        }
    }

    //-------------------------------------------------------------------------
    bool Save()  // Save using last known RunMode
    {
        if (m_RunMode == rmPortable)
            return false;

        else if (m_RunMode == rmIniFile)
            return SaveToIniFile();

        return SaveToRegistry();
    }

    //-------------------------------------------------------------------------
    int Get(const String& ToolName, const String& OptionName, int iDefault)
    {
        TOption Default(iDefault);
        return GetOrCreateOption(ToolName, OptionName, Default).GetIntVal();
    }

    //-------------------------------------------------------------------------
    String Get(const String& ToolName, const String& OptionName, const String& sDefault)
    {
        TOption Default(sDefault);
        return GetOrCreateOption(ToolName, OptionName, Default).GetStringVal();
    }

    //-------------------------------------------------------------------------
    bool Get(const String& ToolName, const String& OptionName, bool bDefault)
    {
        TOption Default(bDefault);
        return GetOrCreateOption(ToolName, OptionName, Default).GetBoolVal();
    }

    //-------------------------------------------------------------------------
    void Set(const String& ToolName, const String& OptionName, int iOption)
    {
        TOption Option(iOption);
        SetOption(ToolName, OptionName, Option);
    }

    //-------------------------------------------------------------------------
    void Set(const String& ToolName, const String& OptionName, String sOption)
    {
        TOption Option(sOption);
        SetOption(ToolName, OptionName, Option);
    }

    //-------------------------------------------------------------------------
    void Set(const String& ToolName, const String& OptionName, bool bOption)
    {
        TOption Option(bOption);
        SetOption(ToolName, OptionName, Option);
    }

private:
    //-------------------------------------------------------------------------
    bool __fastcall LoadFromIniFile()
    {
        TIniFile *Ini = new TIniFile(m_IniFilePath);
        if (!Ini)
            // todo: check why it failed, was it a privilege issue?
            return false;

        TStringList *SectionList = new TStringList;
        Ini->ReadSections(SectionList);

        for (int section = 0; section < SectionList->Count; section++)
        {
            String SectionName = SectionList->Strings[section];

            TStringList *NameList = new TStringList;

            Ini->ReadSection(SectionName, NameList);
            for (int i = 0; i < NameList->Count; i++)
            {
                String OptionName = NameList->Strings[i];
                String OptionValue = Ini->ReadString(SectionName, OptionName, "");

                Set(SectionName, OptionName, OptionValue);
            }
            delete NameList;
        }

        delete Ini;
        return true;
    }

    //-------------------------------------------------------------------------
    bool __fastcall LoadFromRegistry()
    {
        bool bSuccess = false;
        TStringList *NameList = new TStringList;

        TRegistry *Reg = new TRegistry();
        Reg->RootKey = HKEY_CURRENT_USER;
        try
        {
            if (Reg->OpenKey(m_RegBaseKey, false))
            {
                TStringList *KeyList = new TStringList;
                Reg->GetKeyNames(KeyList);

                for (int key = 0; key < KeyList->Count; key++)
                {
                    String KeyName = KeyList->Strings[key];

                    TStringList *NameList = new TStringList;

                    Reg->GetValueNames(NameList);
                    for (int i = 0; i < NameList->Count; i++)
                    {
                        String OptionName = NameList->Strings[i];
                        TRegDataType DataType = Reg->GetDataType(OptionName);
                        switch (DataType)
                        {
                        case rdInteger:
                            {
                                int OptionValue = Reg->ReadInteger(OptionName);
                                Set(KeyName, OptionName, OptionValue);
                            }
                            break;

                        case rdString:
                            {
                                String OptionValue = Reg->ReadString(OptionName);
                                Set(KeyName, OptionName, OptionValue);
                            }
                            break;
                        }
                    }
                    delete NameList;
                }
                delete KeyList;
                Reg->CloseKey();
                bSuccess = true;
            }
        }
        __finally
        {
            delete Reg;
        }
        return bSuccess;
    }

    //-------------------------------------------------------------------------
    bool __fastcall SaveToIniFile()
    {
        TIniFile *Ini = new TIniFile(m_IniFilePath);
        for (option_map_iterator map_iter = m_OptionMaps.begin(); map_iter != m_OptionMaps.end(); map_iter++)
        {
            String SectionName = map_iter->first;
            TOptionMap OptionMap = map_iter->second;
            for (option_iterator option_iter = OptionMap.begin(); option_iter != OptionMap.end(); option_iter++)
            {
                String OptionName = option_iter->first;
                TOption Option = option_iter->second;
                Ini->WriteString(SectionName, OptionName, Option.GetStringVal());
            }
        }
        delete Ini;
        return true;
    }

    //-------------------------------------------------------------------------
    bool __fastcall SaveToRegistry()
    {
        bool bSuccess = false;
        TRegistry *Reg = new TRegistry();
        Reg->RootKey = HKEY_CURRENT_USER;
        try
        {
            for (option_map_iterator map_iter = m_OptionMaps.begin(); map_iter != m_OptionMaps.end(); map_iter++)
            {
                String KeyName = map_iter->first;
                TOptionMap OptionMap = map_iter->second;

                if (Reg->OpenKey(m_RegBaseKey + KeyName, true))
                {
                    for (option_iterator iter = OptionMap.begin(); iter != OptionMap.end(); iter++)
                    {
                        String OptionName = iter->first;
                        TOption Option = iter->second;
                        switch (Option.GetDataType())
                        {
                        case dtInt:
                            Reg->WriteInteger(OptionName, Option.GetIntVal());
                            break;
                        case dtString:
                            Reg->WriteString(OptionName, Option.GetStringVal());
                            break;
                        }
                    }
                }
                Reg->CloseKey();
                bSuccess = true;
            }
        }
        __finally
        {
            delete Reg;
        }
        return bSuccess;
    }

    //-------------------------------------------------------------------------
    bool IniFileExists()
    {
        // See if default ini-file exists
        String IniFileName = ChangeFileExt(ParamStr(0), ".ini");
        //String IniFilePath = "%APPDATA%\\TopTools 4\\" + IniFileName;
        //String IniFilePath = "%USERPROFILE%\\Local Settings\\Application Data\\TopTools 4\\" + IniFileName;
        if (FileExists(IniFileName))// || FileExists(IniFilePath))
        {
            return true;
        }
        return false;
    }

    //-------------------------------------------------------------------------
    bool RegKeyExists()
    {
        bool bSuccess = false;
        TRegistry *Reg = new TRegistry();
        Reg->RootKey = HKEY_CURRENT_USER;
        try
        {
            bSuccess = Reg->KeyExists(m_RegBaseKey);
        }
        __finally
        {
            delete Reg;
        }
        return bSuccess;
    }

    //-------------------------------------------------------------------------
    void InitOptions()
    {
        // For now only sections that appear in this list will be read by
        // this class, in the future we will enumerate the storage medium
        // and populate the OptionMap for each entry in there..

        // Initialize with default settings
//         Set("capture\\autosave", "bypassmenu", false);
//         Set("capture\\autosave", "continuous", false);
//         Set("capture\\autosave", "digits", 2);
//         Set("capture\\autosave", "directory", "");
//         Set("capture\\autosave", "existaction", 0);
//         Set("capture\\autosave", "filename", "Snapshot");
//         Set("capture\\autosave", "imagetype", 0);
//         Set("capture\\autosave", "nextvalue", 1);
//         Set("capture\\autosave", "lastdir", "");
//
//         Set("main", "autostart", false);
//         Set("main", "doubleclick", dcoControl);
//         Set("main", "istrayapp", false);
//         Set("main", "rememberstate", true);
//         Set("main", "savedstate", dcoControl);
//         Set("main", "singleton", false);
//         Set("main", "stayontop", true);
//
//         Set("ruler", "arrownudge", true);
//         Set("ruler", "horizontal", true);
//         Set("ruler", "length", 1024);
//         Set("ruler", "transparency", 50);
//         Set("ruler", "transparent", false);
//
//         Set("baseconv", "showbinary", true);
//
//         Set("loupe", "centerbox", false);
//         Set("loupe", "crosshair", false);
//         Set("loupe", "grid", false);
//         Set("loupe", "height", 200);
//         Set("loupe", "refresh", 250);
//         Set("loupe", "selfmagnify", false);
//         Set("loupe", "width", 200);
//         Set("loupe", "zoom", 4);
//
//         Set("info", "prefix", false);
//         Set("info", "quotes", false);
//
//         Set("capture", "autosave", false);
//         Set("capture", "showloupe", false);
//
//         Set("hotkeys\\capturestart", "enabled", false);
//         Set("hotkeys\\colorcopy", "enabled", false);
//         Set("hotkeys\\doubleclick", "enabled", false);
//         Set("hotkeys\\zoomin", "enabled", false);
//         Set("hotkeys\\zoomout", "enabled", false);

        //Set("control", "top", 0);
    }


    //-------------------------------------------------------------------------
    TOption GetOrCreateOption(const String& ToolName, const String& OptionName, const TOption& Default)
    {
        option_map_iterator map_iter = m_OptionMaps.find(ToolName);
        if (map_iter == m_OptionMaps.end())
        {
            // No map-entry for this tool yet, add one
            TOptionMap OptionMap;
            OptionMap[OptionName] = Default;
            m_OptionMaps[ToolName] = OptionMap;
            return Default;
        }
        else
        {
            // Map-entry found, find option
            option_iterator option_iter = (map_iter->second).find(OptionName);
            if (option_iter == (map_iter->second).end())
            {
                // Option not in map yet, add it
                (map_iter->second)[OptionName] = Default;
                return Default;
            }
            else
            {
                return (option_iter->second);
            }
        }
    }

    //-------------------------------------------------------------------------
    void SetOption(const String& ToolName, const String& OptionName, TOption Option)
    {
        option_map_iterator iter = m_OptionMaps.find(ToolName);
        if (iter != m_OptionMaps.end())
        {
            (iter->second)[OptionName] = Option;
        }
        else
        {
            TOptionMap OptionMap;
            OptionMap[OptionName] = Option;
            m_OptionMaps[ToolName] = OptionMap;
        }
    }

}; // TPersistToolOptions

extern TPersistOptions g_ToolOptions;

#endif // #ifndef TopToolsPersistOptionsH


