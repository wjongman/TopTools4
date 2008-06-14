//---------------------------------------------------------------------------
#ifndef TopToolsPersistOptionsH
#define TopToolsPersistOptionsH

#include <inifiles.hpp>
#include <registry.hpp>
#include <map>
#include "ToolOptions.h"

// Global String for registry access
extern const String g_RegBaseKey;
/*
/////////////////////////////////////////////////////////////////////////////
enum TToolId
{
    idMain = 1,
    idRuler = 2,
    idLoupe = 4,
    idInfo = 8,
    idBaseconv = 16
};*/

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
    typedef std::map<String, TOptionMap> TOptionMaps;
    typedef std::map<String, TOptionMap>::iterator option_map_iterator;

    TOptionMaps m_OptionMaps;
    String m_RegBaseKey;
    String m_IniFilePathName;

public:
    //-------------------------------------------------------------------------
    TPersistOptions()
    {
        InitOptions();
        m_RegBaseKey = g_RegBaseKey;
        m_IniFilePathName = "";
    }

    //-------------------------------------------------------------------------
    void Load()
    {
        // See how the user wants to run the app
        TRunMode RunMode;
        if (ParamCount() > 0 && ParamStr(1) == "-p")
        {
            RunMode = rmPortable;
        }
        else if (ParamCount() > 0 && ParamStr(1) == "-i")
        {
            RunMode = rmIniFile;
            // todo: allow optional ParamStr(2) to indicate target inifile
            if (ParamCount() > 1)
            {
                String IniFileName = ParamStr(2);
                if (FileExists(IniFileName))
                {
                    m_IniFilePathName = IniFileName;
                    Load(IniFileName);
                    return;
                }
            }
        }
        else
        {
            if (RegKeyExists())
                RunMode = rmRegistry;
            else if (IniFileExists())
                RunMode = rmIniFile;
            else
                // todo: offer dialog at this point?
                RunMode = rmPortable;
        }

        Load(RunMode);
    }

    //-------------------------------------------------------------------------
    void EnumIniSections(TStringList *pSectionList)
    {
        TIniFile *Ini = new TIniFile(m_IniFilePathName);
        try
        {
	        Ini->ReadSections(pSectionList);
        }
        __finally
        {
	        delete Ini;
        }
    }

    //-------------------------------------------------------------------------
    void EnumRegKeySections(TStringList *pKeyList)
    {
        TRegistry *Reg = new TRegistry();
        Reg->RootKey = HKEY_CURRENT_USER;
        try
        {
            if (Reg->OpenKey(m_RegBaseKey, false))
            {
                Reg->GetKeyNames(pKeyList);
            }
        }
        __finally
        {
            delete Reg;
        }
    }

    //-------------------------------------------------------------------------
    void Load(const TRunMode& RunMode)
    {
        TStringList *pSectionList = new TStringList;

        switch (RunMode)
        {
        case rmIniFile:
            EnumIniSections(pSectionList);
            break;
        case rmRegistry:
            EnumRegKeySections(pSectionList);
            break;
        }
        // bug: when m_OptionMaps is empty no options are loaded and
        // RunMode degrades to rmPortable
        for (option_map_iterator iter = m_OptionMaps.begin(); iter != m_OptionMaps.end(); iter++)
        {
            (iter->second).Load(RunMode);
        }
        delete pSectionList;
    }

    //-------------------------------------------------------------------------
     void Load(const String& IniFileName)
     {
//         // Load persisted options
//         //m_OptionMaps.Load(RunMode);
//
//         // todo: we need to enumerate the sections held by the storage medium,
//         // at the moment we only find initialized sections in the map
//         for (option_map_iterator iter = m_OptionMaps.begin(); iter != m_OptionMaps.end(); iter++)
//         {
//             (iter->second).Load(IniFileName);
//         }
     }

    //-------------------------------------------------------------------------
    void Save()
    {
        for (option_map_iterator iter = m_OptionMaps.begin(); iter != m_OptionMaps.end(); iter++)
        {
            (iter->second).Save();
        }
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
    bool IniFileExists()
    {
        // See if default ini-file exists
        String IniFileName = ChangeFileExt(ParamStr(0), ".ini");
        String IniFilePath = "%APPDATA%\\TopTools 4\\" + IniFileName;
        //String IniFilePath = "%USERPROFILE%\\Local Settings\\Application Data\\TopTools 4\\" + IniFileName;
        if (FileExists(IniFileName))// || FileExists(IniFilePath))
        {
            return true;
        }
        return false;
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

public:
    //-------------------------------------------------------------------------
    int Get(const String& ToolName, const String& OptionName, int iDefault)
    {
        option_map_iterator iter = m_OptionMaps.find(ToolName);
        if (iter != m_OptionMaps.end())
        {
            return(iter->second).Get(OptionName, iDefault);
        }

        return iDefault;
    }

    //-------------------------------------------------------------------------
    String Get(const String& ToolName, const String& OptionName, const String& sDefault)
    {
        option_map_iterator iter = m_OptionMaps.find(ToolName);
        if (iter != m_OptionMaps.end())
        {
            return(iter->second).Get(OptionName, sDefault);
        }

        return sDefault;
    }

    //-------------------------------------------------------------------------
    bool Get(const String& ToolName, const String& OptionName, bool bDefault)
    {
        option_map_iterator iter = m_OptionMaps.find(ToolName);
        if (iter != m_OptionMaps.end())
        {
            return(iter->second).Get(OptionName, bDefault);
        }

        return bDefault;
    }

    //-------------------------------------------------------------------------
    int GetInt(const String& ToolName, const String& OptionName)
    {
        return Get(ToolName, OptionName, 0);
    }

    //-------------------------------------------------------------------------
    String GetString(const String& ToolName, const String& OptionName)
    {
        return Get(ToolName, OptionName, "");
    }

    //-------------------------------------------------------------------------
    bool GetBool(const String& ToolName, const String& OptionName)
    {
        return Get(ToolName, OptionName, false);
    }

    //-------------------------------------------------------------------------
    void Set(const String& ToolName, const String& OptionName, int Option)
    {
        option_map_iterator iter = m_OptionMaps.find(ToolName);
        if (iter != m_OptionMaps.end())
        {
            (iter->second).Set(OptionName, Option);
        }
        else
        {
            TOptionMap OptionMap(m_RegBaseKey, ToolName);
            OptionMap.Set(OptionName, Option);
            m_OptionMaps[ToolName] = OptionMap;
        }
    }

    //-------------------------------------------------------------------------
    void Set(const String& ToolName, const String& OptionName, String Option)
    {
        option_map_iterator iter = m_OptionMaps.find(ToolName);
        if (iter != m_OptionMaps.end())
        {
            (iter->second).Set(OptionName, Option);
        }
        else
        {
            TOptionMap OptionMap(m_RegBaseKey, ToolName);
            OptionMap.Set(OptionName, Option);
            m_OptionMaps[ToolName] = OptionMap;
        }
    }

    //-------------------------------------------------------------------------
    void Set(const String& ToolName, const String& OptionName, bool Option)
    {
        option_map_iterator iter = m_OptionMaps.find(ToolName);
        if (iter != m_OptionMaps.end())
        {
            (iter->second).Set(OptionName, Option);
        }
        else
        {
            TOptionMap OptionMap(m_RegBaseKey, ToolName);
            OptionMap.Set(OptionName, Option);
            m_OptionMaps[ToolName] = OptionMap;
        }
    }

}; // TPersistToolOptions

extern TPersistOptions g_ToolOptions;

#endif // #ifndef TopToolsPersistOptionsH


