//---------------------------------------------------------------------------
#ifndef TopToolsPersistOptionsH
#define TopToolsPersistOptionsH

#include <inifiles.hpp>
#include <registry.hpp>
#include <map>
#include "ToolOptions.h"

// Suppress W8027:
// Functions containing ‘[statement]’ are not expanded inline
#pragma warn -inl

/////////////////////////////////////////////////////////////////////////////
enum TRunMode
{
    rmPortable = 0,
    rmIniFile = 1,
    rmRegistry = 2
};

/////////////////////////////////////////////////////////////////////////////
enum TDoubleClickOpen
{
    dcoControl = 1,
    dcoRuler = 2,
    dcoLoupe = 4,
    dcoInfo = 8,
    dcoBaseconv = 16,
    dcoGrab = 32
};

/////////////////////////////////////////////////////////////////////////////
//
// A (single) global instance of this class is the one and only object
// to handle option persistence in a medium agnostic way.
// (todo: make it a singleton)
//
// Implements run-mode detection (portable/inifile/registry).
// Implements Load() and Save() functions which use either the registry
// or an ini-file (or do nothing), according to detected run-mode.
//
/////////////////////////////////////////////////////////////////////////////
class TPersistOptions
{
private:
    // TOption is a (sort of) type agnostic option-data container.
    // Each TOptionMap is a map of TOption items indexed by option-name.
    typedef std::map<String, TOption> TOptionMap;
    typedef std::map<String, TOption>::iterator option_iterator;

    // A toolname indexed map of TOptionmap items holds the complete option set.
    typedef std::map<String, TOptionMap> TOptionMaps;
    typedef std::map<String, TOptionMap>::iterator option_map_iterator;

    TOptionMaps m_OptionMaps;
    String m_RegBaseKey;
    String m_IniFilePath;
    TRunMode m_RunMode;
    bool m_QuerySave;

public:
    //-------------------------------------------------------------------------
    TPersistOptions()
    {
        m_QuerySave = false;
        m_RunMode = rmPortable;
    }

    //-------------------------------------------------------------------------
    void Load(const String& RegBaseKey)
    {
        m_QuerySave = false;
        m_RegBaseKey = RegBaseKey;
        m_IniFilePath = ChangeFileExt(ParamStr(0), ".ini");

        // See how the user wants to run the app
        if (ParamCount() > 0)
        {
            // Run-mode can be forced by commandline arguments
            if (ParamStr(1) == "-p")
            {
                m_RunMode = rmPortable;
            }
            else if (ParamStr(1) == "-r")
            {
                m_RunMode = rmRegistry;
            }
            else if (ParamStr(1) == "-i")
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
                    else
                    {
                        String IniFileDir = ExtractFileDir(IniFilePath);
                        if (IniFileDir == "")
                        {
                            IniFileDir = ExtractFilePath(ParamStr(0));
                            IniFilePath = IniFileDir + IniFilePath;
                        }

                        if (TryCreateFile(IniFilePath))
                        {
                            m_IniFilePath = IniFilePath;
                        }
                        else
                        {
                            String sMsg = "Could not create " + IniFilePath
                                + "\nSettings will not be saved.\n\n";

                            ShowMessage(sMsg);
                            m_RunMode = rmPortable;
                        }
                    }
                }
            }
        }
        else
        {
            // No commandline arguments, heuristcly determine run-mode
            if (RegKeyExists())
            {
                // If a Registry key exists, we want to load these
                // per-user settings.
                // In this case we'll run in rmRegistry mode
                m_RunMode = rmRegistry;
            }
            else if (IniFileExists())
            {
                // todo: First test if inifile is writable

                // If an Ini file exists we'll run in rmIniFile mode
                m_RunMode = rmIniFile;
            }
            else
            {
                // If neither exists we run rmPortable (and offer a dialog to
                // choose what to do with the settings before TopTools exits?)
                m_RunMode = rmPortable;
                m_QuerySave = true;
            }
        }

        if (m_RunMode == rmIniFile)
        {
            LoadFromIniFile();
        }
        else if (m_RunMode == rmRegistry)
        {
            LoadFromRegistry();
        }
        else
        {
            // Do nothing.
        }

//         switch (m_RunMode)
//         {
//         case rmIniFile:
//             LoadFromIniFile();
//             break;
//         case rmRegistry:
//             LoadFromRegistry();
//             break;
//         }
    }

    /*/-------------------------------------------------------------------------
    bool KnowsRunMode()
    {
        return !m_QuerySave;
    } */

    //-------------------------------------------------------------------------
    void SetRunMode(TRunMode runmode)
    {
        m_RunMode = runmode;
    }

    //-------------------------------------------------------------------------
    TRunMode GetRunMode()
    {
        return m_RunMode;
    }

    //-------------------------------------------------------------------------
    bool Save()  // Save using last known RunMode
    {
        if (m_RunMode == rmPortable)
        {
            return false;
        }

        else if (m_RunMode == rmIniFile)
        {
            return SaveToIniFile();
        }
        else
        {
            return SaveToRegistry();
        }
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
    void Set(const String& ToolName, const String& OptionName, const String& sOption)
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

    //-------------------------------------------------------------------------
    bool SettingExists(const String& ToolName, const String& OptionName)
    {
        bool result = false;

        option_map_iterator map_iter = m_OptionMaps.find(ToolName);
        if (map_iter != m_OptionMaps.end())
        {
            option_iterator option_iter = (map_iter->second).find(OptionName);
            if (option_iter != (map_iter->second).end())
            {
                // Found
                result = true;
            }
        }
        return result;
    }

    //-------------------------------------------------------------------------
    void ClearOptions(const String& ToolName)
    {
        // Delete all options of given tool
        option_map_iterator iter = m_OptionMaps.find(ToolName);
        if (iter != m_OptionMaps.end())
        {
            iter->second.clear();
        }
    }

private:
    //-------------------------------------------------------------------------
    TOption GetOrCreateOption(const String& ToolName, const String& OptionName, const TOption& Default)
    {
        // m_OptionMaps is a map of maps of name-value pairs ;-)
        option_map_iterator map_iter = m_OptionMaps.find(ToolName);
        if (map_iter == m_OptionMaps.end())
        {
            // No map-entry for this tool yet, add a tool of this
            // name and initialize option by this name to default.
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
                return(option_iter->second);
            }
        }
    }

    //-------------------------------------------------------------------------
    void SetOption(const String& ToolName, const String& OptionName, const TOption& Option)
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
        return EnumRegKeys(HKEY_CURRENT_USER, m_RegBaseKey, "");

    }

    //-------------------------------------------------------------------------
    bool __fastcall EnumRegKeys(HKEY RootKey, const String& ParentKeyName, const String& KeyName)
    {
        bool bSuccess = false;

        TRegistry *Reg = new TRegistry();
        Reg->RootKey = RootKey;
        try
        {
            if (Reg->OpenKey(ParentKeyName, false))
            {
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

                if (Reg->HasSubKeys())
                {
                    TStringList *KeyList = new TStringList;
                    Reg->GetKeyNames(KeyList);

                    for (int key = 0; key < KeyList->Count; key++)
                    {
                        String SubKeyName = KeyList->Strings[key];
                        // Quick hack to avoid backslashes in front of basekey
                        String NextKeyName;
                        if (KeyName == "")
                        {
                            NextKeyName = SubKeyName;
                        }
                        else
                        {
                            NextKeyName = KeyName + "\\" + SubKeyName;
                        }

                        // Recursively enumerate subkeys
                        EnumRegKeys(RootKey, ParentKeyName + SubKeyName  + "\\", NextKeyName);
                    }
                    delete KeyList;
                }
            }
            Reg->CloseKey();
            bSuccess = true;
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

            // Delete existing section and write a new one
            Ini->EraseSection(SectionName);
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

                // Delete existing section and write a new one
                Reg->DeleteKey(m_RegBaseKey + KeyName);
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
public:

    String GetIniFilePath()
    {
        return m_IniFilePath;
    }

    //-------------------------------------------------------------------------
    String GetRegPath()
    {
        return "HKEY_CURRENT_USER\\" + m_RegBaseKey;
    }

    //-------------------------------------------------------------------------
    void __fastcall ClearRegistry()
    {
        // Remove all traces from registry
        TRegistry *Reg = new TRegistry();
        Reg->RootKey = HKEY_CURRENT_USER;
        Reg->DeleteKey(m_RegBaseKey);
//        DeleteRecurseRegKey(HKEY_CURRENT_USER, m_RegBaseKey);
    }

    //-------------------------------------------------------------------------
    bool __fastcall DeleteRecurseRegKey(HKEY RootKey, const String& ParentKeyName)
    {
        bool bSuccess = false;

        TRegistry *Reg = new TRegistry();
        Reg->RootKey = RootKey;

        // First, see if we can delete the key without having to recurse.
        if (Reg->DeleteKey(ParentKeyName))
            return true;

        try
        {
            if (Reg->OpenKey(ParentKeyName, false))
            {
                if (Reg->HasSubKeys())
                {
                    TStringList *KeyList = new TStringList;
                    Reg->GetKeyNames(KeyList);

                    for (int key = 0; key < KeyList->Count; key++)
                    {
                        String SubKeyName = KeyList->Strings[key];
                        // Recursively delete subkeys
                        DeleteRecurseRegKey(RootKey, ParentKeyName + SubKeyName  + "\\");
                    }
                    delete KeyList;
                }
            }
            Reg->CloseKey();
            bSuccess = true;
        }
        __finally
        {
            delete Reg;
        }
        return bSuccess;
    }

    //-------------------------------------------------------------------------
    bool ProgramDirIsWriteable()
    {
        // See if program directory is accessable (by using the CreateFile API).
        String FilePath = ExtractFilePath(ParamStr(0));

        HANDLE hFile = 0;
        hFile = CreateFile(FilePath.c_str(),
                           GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_READ | FILE_SHARE_WRITE,     // share for reading and writing
                           NULL,                                   // no security
                           OPEN_EXISTING,                          // existing file only
                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
                           NULL);                                  // no attr. template

        if (hFile == INVALID_HANDLE_VALUE)
        {
            return false;
        }
        CloseHandle(hFile);

        return true;
    }
/*
    bool ProgramDirIsWriteable()
    {
        // See if program directory is accessable
        String sProgDir = ExtractFilePath(ParamStr(0));

        return DirectoryIsWriteable(sProgDir);
    }
*/
    bool TryCreateFile(const String& sFilePath)
    {
        if (FileExists(sFilePath))
        {
            return true;
        }

        HANDLE hTestFile = NULL;
        hTestFile = CreateFile(sFilePath.c_str(),
                               GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,     // share for reading and writing
                               NULL,                                   // no security
                               CREATE_NEW,                             // create new file, fail if file already exists
                               FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
                               NULL);                                  // no attr. template

        if (hTestFile == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        CloseHandle(hTestFile);
        //DeleteFile(sTestFilePath);

        return true;
    }

/*
//Function : CheckAccess( LPCTSTR fname, int mode)
//Returns  : 0 when the access is present, -1 when access is not available
//Desc     : This function can be used as a substitue for _access. The paramters are the same as in _access()
//fname - file/directory whose access is to be checked
//mode - mode to be checked, pass the code corresponding to the access test required.
// 0 - Test existence
// 2 - Write access
// 4 - Read access
// 6 - Read & Write access.
//
#include <io.h>

int CheckAccess(LPCTSTR fname, int mode)
{
         DWORD dwAccess;
         if (mode == 0)
             return _access(fname, mode);                //access would do when we want to check the existence alone.

         if (mode == 2)
             dwAccess = GENERIC_WRITE;
         else if (mode == 4)
             dwAccess = GENERIC_READ;
         else if (mode == 6)
             dwAccess = GENERIC_READ  | GENERIC_WRITE;

         HANDLE hFile = 0;
         hFile = CreateFile(fname,
                 dwAccess,
                 FILE_SHARE_READ|FILE_SHARE_WRITE,       // share for reading and writing
                 NULL,                                   // no security
                 OPEN_EXISTING,                          // existing file only
                 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
                 NULL);                                  // no attr. template

         if (hFile == INVALID_HANDLE_VALUE)
             { return(-1); }
         CloseHandle(hFile);
         return(0);
}
*/

}; // TPersistToolOptions

extern TPersistOptions g_ToolOptions;

#endif // #ifndef TopToolsPersistOptionsH


