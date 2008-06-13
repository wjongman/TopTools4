//---------------------------------------------------------------------------
#ifndef TopToolsPersistOptionsH
#define TopToolsPersistOptionsH

#include <inifiles.hpp>
#include <registry.hpp>
#include <map>

// Global String for registry access
extern const String g_RegBaseKey;

/////////////////////////////////////////////////////////////////////////////
enum TToolId
{
    idMain = 1,
    idRuler = 2,
    idLoupe = 4,
    idInfo = 8,
    idBaseconv = 16
};

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
enum TRunMode
{
    rmPortable,
    rmIniFile,
    rmRegistry
};

/////////////////////////////////////////////////////////////////////////////
enum DataType
{
    dtInt,
    dtString
};

/////////////////////////////////////////////////////////////////////////////
class TOption
{
public:
    //-------------------------------------------------------------------------
    TOption() : IntVal(0), StringVal("")
    {
    }

    //-------------------------------------------------------------------------
    TOption(int iValue)
    {
        IntVal = iValue;
        StringVal = IntToStr(iValue);
    }

    //-------------------------------------------------------------------------
    TOption(String sValue)
    {
        IntVal = StrToIntDef(sValue, 0);
        StringVal = sValue;
    }

    //-------------------------------------------------------------------------
    int GetIntVal()
    {
        return IntVal;
    }

    //-------------------------------------------------------------------------
    String GetStringVal()
    {
        return StringVal;
    }

    //-------------------------------------------------------------------------
    bool GetBoolVal()
    {
        return IntVal != 0;
    }

    //-------------------------------------------------------------------------
    int GetValue(int iDefault)
    {
        if (GetDataType() == dtInt)
        {
            return IntVal;
        }
        return iDefault;
    }

    //-------------------------------------------------------------------------
    String GetValue(const String& sDefault)
    {
        // should we return sDefault when StringVal is empty?
        return StringVal;
    }

    //-------------------------------------------------------------------------
    DataType GetDataType()
    {
        // Ini files are agostic to data types; all options are strings.
        // However, he registry requires us to specify a data type
        // (REG_SZ, REG_DWORD etc.)
        // Unless we decide to store all our options as string, we need a
        // way to communicate the datatype when interacting with the registry
        // For now we'll infer the datatype by trying to convert to int.
        // Let Delphi's StrToIntDef() do the heavy lifting..
        int testint1 = StrToIntDef(StringVal, 1);
        int testint2 = StrToIntDef(StringVal, 2);
        if (testint1 == 1 && testint2 == 2)
        {
            return dtString;
        }
        return dtInt;
    }

private:

    int IntVal;
    String StringVal;

}; // class TOption

/////////////////////////////////////////////////////////////////////////////
//
// Encapsulation of map that holds optionname-TOption pairs
//
/////////////////////////////////////////////////////////////////////////////
class TOptionMap
{
    typedef std::map<String, TOption> OptionMap;
    typedef std::map<String, TOption>::iterator option_iterator;

private:
    String m_ToolName;
    OptionMap m_Options;
    String m_RegBaseKey;
    TRunMode m_RunMode;

public:

    //-------------------------------------------------------------------------
    TOptionMap()
    : m_RegBaseKey(""), m_ToolName(""), m_RunMode(rmPortable)
    {
    }

    //-------------------------------------------------------------------------
    TOptionMap(const String& sRegBaseKey, const String& sToolName)
    : m_RegBaseKey(sRegBaseKey), m_ToolName(sToolName), m_RunMode(rmPortable)
    {
    }

    //-------------------------------------------------------------------------
    void Set(const String& OptionName, int Value)
    {
        m_Options[OptionName] = TOption(Value);
    }

    //-------------------------------------------------------------------------
    void Set(const String& OptionName, String Value)
    {
        m_Options[OptionName] = TOption(Value);
    }

    //-------------------------------------------------------------------------
    void Set(const String& OptionName, bool Value)
    {
        m_Options[OptionName] = TOption(Value);
    }

    //-------------------------------------------------------------------------
    int Get(const String& OptionName, int iDefault)
    {
        TOption Option = m_Options[OptionName];
        return Option.GetValue(iDefault);
    }

    //-------------------------------------------------------------------------
    String Get(const String& OptionName, const String& sDefault)
    {
        TOption Option = m_Options[OptionName];
        return Option.GetValue(sDefault);
    }

    //-------------------------------------------------------------------------
    bool Get(const String& OptionName, bool bDefault)
    {
        TOption Option = m_Options[OptionName];
        return Option.GetValue(bDefault);
    }

    //-------------------------------------------------------------------------
    bool Load(TRunMode RunMode)
    {
        m_RunMode = RunMode;

        if (RunMode == rmPortable)
            return false;

        else if (RunMode == rmIniFile)
            return LoadFromIniFile();

        return LoadFromRegistry();
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
    bool __fastcall EnumIniFile()
    {
        String IniFilename = ChangeFileExt(ParamStr(0), ".ini");

        // If there is no ini file, skip this part and use default values
        if (!FileExists(IniFilename))
            return false;

        TIniFile *Ini = new TIniFile(IniFilename);
        if (!Ini)
            // todo: check why it failed, was it a privilege issue?
            return false;

        String SectionName = m_ToolName;
        TStringList *NameList = new TStringList;

        Ini->ReadSection(SectionName, NameList);
        for (int i = 0; i < NameList->Count; i++)
        {
            String OptionName = NameList->Strings[i];
            String OptionValue = Ini->ReadString(SectionName, OptionName, "");
            TOption Option(OptionValue);
            m_Options[OptionName] = Option;
        }
        delete Ini;
        delete NameList;
        return true;
    }

    //-------------------------------------------------------------------------
    bool EnumRegKeys()
    {
        // todo: If there is no reg-key, skip this part, we'll use the default values
        bool bSuccess = false;
        TStringList *NameList = new TStringList;

        TRegistry *Reg = new TRegistry();
        Reg->RootKey = HKEY_CURRENT_USER;
        try
        {
            if (Reg->OpenKey(m_RegBaseKey + m_ToolName, false))
            {
                Reg->GetValueNames(NameList);
                for (int i = 0; i < NameList->Count; i++)
                {
                    String OptionName = NameList->Strings[i];
                    TRegDataType DataType = Reg->GetDataType(OptionName);
                    switch (DataType)
                    {
                    case rdInteger:
                        {
                            TOption Option(Reg->ReadInteger(OptionName));
                            m_Options[OptionName] = Option;
                        }
                        break;

                    case rdString:
                        {
                            TOption Option(Reg->ReadString(OptionName));
                            m_Options[OptionName] = Option;
                        }
                        break;
                    }
                }

                Reg->CloseKey();
                bSuccess = true;
            }
        }
        __finally
        {
            delete Reg;
            delete NameList;
        }
        return bSuccess;
    }

    //-------------------------------------------------------------------------
    bool __fastcall SaveToIniFile()
    {
        String SectionName = m_ToolName;
        String IniFilename = ChangeFileExt(ParamStr(0), ".ini");
        TIniFile *Ini = new TIniFile(IniFilename);
        if (!Ini)
            // todo: check why it failed, was it a privilege issue?
            return false;

        for (option_iterator iter = m_Options.begin(); iter != m_Options.end(); iter++)
        {
            String OptionName = iter->first;
            TOption Option = iter->second;
            Ini->WriteString(SectionName, OptionName, Option.GetStringVal());
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
            if (Reg->OpenKey(m_RegBaseKey + m_ToolName, true))
            {
                for (option_iterator iter = m_Options.begin(); iter != m_Options.end(); iter++)
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
    bool __fastcall LoadFromIniFile()
    {
        return EnumIniFile();
    }

    //-------------------------------------------------------------------------
    bool __fastcall LoadFromRegistry()
    {
        return EnumRegKeys();
    }

}; // class TOptionMap

//} // namespace TopTools


/////////////////////////////////////////////////////////////////////////////
//
// Encapsulation of map that holds toolname-TOptionMap pairs
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
        m_RegBaseKey = g_RegBaseKey;//"Software\\TopTools 4\\";
        m_IniFilePathName = "";

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
        Set("capture\\autosave", "bypassmenu", false);
        Set("capture\\autosave", "continuous", false);
        Set("capture\\autosave", "digits", 2);
        Set("capture\\autosave", "directory", "");
        Set("capture\\autosave", "existaction", 0);
        Set("capture\\autosave", "filename", "Snapshot");
        Set("capture\\autosave", "imagetype", 0);
        Set("capture\\autosave", "nextvalue", 1);
        Set("capture\\autosave", "lastdir", "");

        Set("main", "autostart", false);
        Set("main", "doubleclick", dcoControl);
        Set("main", "istrayapp", false);
        Set("main", "rememberstate", true);
        Set("main", "savedstate", dcoControl);
        Set("main", "singleton", false);
        Set("main", "stayontop", true);

        Set("ruler", "arrownudge", true);
        Set("ruler", "horizontal", true);
        Set("ruler", "length", 1024);
        Set("ruler", "transparency", 50);
        Set("ruler", "transparent", false);

        Set("baseconv", "showbinary", true);

        Set("loupe", "centerbox", false);
        Set("loupe", "crosshair", false);
        Set("loupe", "grid", false);
        Set("loupe", "height", 200);
        Set("loupe", "refresh", 250);
        Set("loupe", "selfmagnify", false);
        Set("loupe", "width", 200);
        Set("loupe", "zoom", 4);

        Set("info", "prefix", false);
        Set("info", "quotes", false);

        Set("capture", "autosave", false);
        Set("capture", "showloupe", false);

        Set("hotkeys\\capturestart", "enabled", false);
        Set("hotkeys\\colorcopy", "enabled", false);
        Set("hotkeys\\doubleclick", "enabled", false);
        Set("hotkeys\\zoomin", "enabled", false);
        Set("hotkeys\\zoomout", "enabled", false);

        //Set("control", "top", 0);
    }

public:
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
    void Load(TRunMode RunMode)
    {
        // Load persisted options
        //m_OptionMaps.Load(RunMode);

        // todo: we need to enumerate the sections held by the storage medium,
        // at the moment we only find initialized sections in the map
        for (option_map_iterator iter = m_OptionMaps.begin(); iter != m_OptionMaps.end(); iter++)
        {
            (iter->second).Load(RunMode);
        }
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
// Global flag to hold runmode
extern TRunMode g_RunMode;

#endif // #ifndef TopToolsPersistOptionsH


