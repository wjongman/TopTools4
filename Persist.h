//---------------------------------------------------------------------------
#ifndef TopToolsPersistH
#define TopToolsPersistH

#include <inifiles.hpp>
#include <registry.hpp>
#include <map>

//---------------------------------------------------------------------------
namespace TopTools
{
  enum VarType
  {
    varNone, varInt, varString, varBool, varAny
  };

/////////////////////////////////////////////////////////////////////////////
#if 0
  class TVarOption
  {
  public:
    TVarOption() : IntVal(0), StringVal(""), BoolVal(false), VarType(varNone)
    {
    }
    TVarOption(int iValue) : IntVal(iValue), VarType(varInt)
    {
      StringVal = IntToStr(iValue);
      BoolVal = iValue;
    }
    TVarOption(String sValue) : StringVal(sValue), VarType(varString)
    {
    }
    TVarOption(String sValue, bool bAnyType)
    : StringVal(sValue), VarType(varAny)
    {
      // Deduce other types from String
      IntVal = StrToIntDef(sValue, 0);
      BoolVal = IntVal;
    }
    TVarOption(bool bValue) : BoolVal(bValue), VarType(varBool)
    {
    }

    int GetValue(int iDefault)
    {
      if (VarType == varNone)
      {
        VarType = varInt;
        IntVal = iDefault;
      }
      return IntVal;
    }

    String GetValue(const String& sDefault)
    {
      if (VarType == varNone)
      {
        VarType = varString;
        StringVal = sDefault;
      }
      return StringVal;
    }

    bool GetValue(bool bDefault)
    {
      if (VarType == varNone)
      {
        VarType = varBool;
        BoolVal = bDefault;
      }
      return BoolVal;
    }

    int GetIntVal()
    {
      return IntVal;
    }
    String GetStringVal()
    {
      return StringVal;
    }
    bool GetBoolVal()
    {
      return BoolVal;
    }

    TVarType GetVarType()
    {
      return VarType;
    }

  private:
    String OptionName;

    int IntVal;
    String StringVal;
    bool BoolVal;

    TVarType VarType;
  };
#endif
/////////////////////////////////////////////////////////////////////////////
  enum DataType { dtInt, dtString };

  class TOption
  {
  public:
    TOption() : IntVal(0), StringVal("")
    {
    }

    TOption(int iValue)
    {
      IntVal = iValue;
      StringVal = IntToStr(iValue);
    }

    TOption(String sValue)
    {
      IntVal = StrToIntDef(sValue, 0);
      StringVal = sValue;
    }

    int GetIntVal()
    {
      return IntVal;
    }

    String GetStringVal()
    {
      return StringVal;
    }

    bool GetBoolVal()
    {
      return IntVal != 0;
    }

    int GetValue(int iDefault)
    {
      if (GetDataType() == dtInt)
      {
        return IntVal;
      }
      return iDefault;
    }

    String GetValue(const String& sDefault)
    {
      return StringVal;
    }

    DataType GetDataType()
    {
      // Ini files are agostic to data types, all options are strings.
      // The registry however, requires us to specify a data type
      // (REG_SZ, REG_DWORD etc.)
      // Unless we decide to store all our options as string, we need a
      // way to communicate the datatype when interacting with the registry
      // For now we'll infer the datatype by trying to convert to int.
      // Let Delphi's StrToIntDef() do the heavy lifting
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

  };

const String g_RegBaseKey = "Software\\TopTools 3\\";

/////////////////////////////////////////////////////////////////////////////
  class Options
  {
    typedef std::map<String, TOption> TOptionMap;
    typedef std::map<String, TOption>::iterator option_iterator;

  private:
    String m_ToolName;
    TOptionMap m_Options;
    String m_RegBaseKey;

  public:

    //-------------------------------------------------------------------------
//    Options()
//    : m_RegBaseKey(""), m_ToolName("")
//    {
//    }

    //-------------------------------------------------------------------------
//    Options(String RegBaseKey, String ToolName)
//    : m_RegBaseKey(RegBaseKey), m_ToolName(ToolName)
//    {
//    }

    //-------------------------------------------------------------------------
    Options(String ToolName)
    : m_RegBaseKey(g_RegBaseKey), m_ToolName(ToolName)
    {
    }

    //-------------------------------------------------------------------------
    void Init(String RegBaseKey, String ToolName)
    {
      m_RegBaseKey = RegBaseKey;
      m_ToolName = ToolName;

//      if (!EnumRegKeys())
      EnumIniFile();
    }

    //-------------------------------------------------------------------------
    void Set(const String& Name, int Value)
    {
      m_Options[Name] = TOption(Value);
    }

    //-------------------------------------------------------------------------
    void Set(const String& Name, String Value)
    {
      m_Options[Name] = TOption(Value);
    }

    //-------------------------------------------------------------------------
    void Set(const String& Name, bool Value)
    {
      m_Options[Name] = TOption(Value);
    }

//     //-------------------------------------------------------------------------
//     int GetInt(const String& OptionName)
//     {
//       TVarOption VarOption = m_Options[OptionName];
//       return VarOption.GetIntVal();
//     }
//
//     //-------------------------------------------------------------------------
//     String GetString(const String& OptionName)
//     {
//
//       TVarOption VarOption = m_Options[OptionName];
//       return VarOption.GetStringVal();
//     }
//
//     //-------------------------------------------------------------------------
//     bool GetBool(const String& OptionName)
//     {
//       TVarOption VarOption = m_Options[OptionName];
//       return VarOption.GetBoolVal();
//     }

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
    bool Save()
    {
      //    if (!SaveToRegistry())
      return SaveToIniFile();

      //    return true;
    }

    //-------------------------------------------------------------------------
    bool Load()
    {
      //    if (!LoadFromRegistry())
      return LoadFromIniFile();

      //    return true;
    }

    //-------------------------------------------------------------------------
    bool __fastcall EnumIniFile()
    {
      TStringList *NameList = new TStringList;

      String SectionName = m_ToolName;
      String IniFilename = ChangeFileExt(ParamStr(0), ".ini");
      TIniFile *Ini = new TIniFile(IniFilename);
      if (!Ini)
        // todo: check why it failed, was it a privilege issue?
        return false;

      Ini->ReadSection(SectionName, NameList);
      for (int i = 0; i < NameList->Count; i++)
      {
        String OptionName = NameList->Strings[i];
        //TRegDataType DataType = Reg->GetDataType(OptionName);
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
        if (Reg->OpenKey(m_RegBaseKey + m_ToolName, false))
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

//      String SectionName = m_ToolName;
//      String IniFilename = ChangeFileExt(ParamStr(0), ".ini");
//      TIniFile *Ini = new TIniFile(IniFilename);
//      if (!Ini)
//        // todo: check why it failed, was it a privilege issue?
//        return false;
//
//      for (option_iterator iter = m_Options.begin(); iter != m_Options.end(); iter++)
//      {
//        String OptionName = iter->first;
//        TVarOption VarValue = iter->second;
//        switch (VarValue.GetVarType())
//        {
//        case varInt:
//          m_Options[OptionName] = Ini->ReadInteger(SectionName, OptionName, VarValue.GetIntVal());
//          break;
//        case varString:
//          m_Options[OptionName] = Ini->ReadString(SectionName, OptionName, VarValue.GetStringVal());
//          break;
//        case varBool:
//          m_Options[OptionName] = Ini->ReadBool(SectionName, OptionName, VarValue.GetBoolVal());
//          break;
//        }
//      }
//      delete Ini;
//      return true;
    }

    //-------------------------------------------------------------------------
    bool __fastcall LoadFromRegistry()
    {
      return EnumRegKeys();

//      bool bSuccess = false;
//      TRegistry *Reg = new TRegistry();
//      Reg->RootKey = HKEY_CURRENT_USER;
//      try
//      {
//        if (Reg->OpenKey(m_RegBaseKey + m_ToolName, false))
//        {
//          for (option_iterator iter = m_Options.begin(); iter != m_Options.end(); iter++)
//          {
//            String OptionName = iter->first;
//            TVarOption VarValue = iter->second;
//            switch (VarValue.GetVarType())
//            {
//            case varInt:
//              if (Reg->ValueExists(OptionName))
//                m_Options[OptionName] = Reg->ReadInteger(OptionName);
//              break;
//
//            case varString:
//              if (Reg->ValueExists(OptionName))
//                m_Options[OptionName] = Reg->ReadString(OptionName);
//              break;
//
//            case varBool:
//              if (Reg->ValueExists(OptionName))
//                m_Options[OptionName] = Reg->ReadBool(OptionName);
//              break;
//            }
//            Reg->CloseKey();
//            bSuccess = true;
//          }
//        }
//      }
//      __finally
//      {
//        delete Reg;
//      }
//      return bSuccess;
    }

//    //-------------------------------------------------------------------------
//    bool __fastcall LoadFromIniFile()
//    {
//      String SectionName = m_ToolName;
//      String IniFilename = ChangeFileExt(ParamStr(0), ".ini");
//      TIniFile *Ini = new TIniFile(IniFilename);
//      if (!Ini)
//        // todo: check why it failed, was it a privilege issue?
//        return false;
//
//      for (option_iterator iter = m_Options.begin(); iter != m_Options.end(); iter++)
//      {
//        String OptionName = iter->first;
//        TVarOption VarValue = iter->second;
//        switch (VarValue.GetVarType())
//        {
//        case varInt:
//          m_Options[OptionName] = Ini->ReadInteger(SectionName, OptionName, VarValue.GetIntVal());
//          break;
//        case varString:
//          m_Options[OptionName] = Ini->ReadString(SectionName, OptionName, VarValue.GetStringVal());
//          break;
//        case varBool:
//          m_Options[OptionName] = Ini->ReadBool(SectionName, OptionName, VarValue.GetBoolVal());
//          break;
//        }
//      }
//      delete Ini;
//      return true;
//    }
//
//    //-------------------------------------------------------------------------
//    bool __fastcall LoadFromRegistry()
//    {
//      bool bSuccess = false;
//      TRegistry *Reg = new TRegistry();
//      Reg->RootKey = HKEY_CURRENT_USER;
//      try
//      {
//        if (Reg->OpenKey(m_RegBaseKey + m_ToolName, false))
//        {
//          for (option_iterator iter = m_Options.begin(); iter != m_Options.end(); iter++)
//          {
//            String OptionName = iter->first;
//            TVarOption VarValue = iter->second;
//            switch (VarValue.GetVarType())
//            {
//            case varInt:
//              if (Reg->ValueExists(OptionName))
//                m_Options[OptionName] = Reg->ReadInteger(OptionName);
//              break;
//
//            case varString:
//              if (Reg->ValueExists(OptionName))
//                m_Options[OptionName] = Reg->ReadString(OptionName);
//              break;
//
//            case varBool:
//              if (Reg->ValueExists(OptionName))
//                m_Options[OptionName] = Reg->ReadBool(OptionName);
//              break;
//            }
//            Reg->CloseKey();
//            bSuccess = true;
//          }
//        }
//      }
//      __finally
//      {
//        delete Reg;
//      }
//      return bSuccess;
//    }

//    //-------------------------------------------------------------------------
//    bool __fastcall SaveToIniFile()
//    {
//      String SectionName = m_ToolName;
//      String IniFilename = ChangeFileExt(ParamStr(0), ".ini");
//      TIniFile *Ini = new TIniFile(IniFilename);
//      if (!Ini)
//        // todo: check why it failed, was it a privilege issue?
//        return false;
//
//      for (option_iterator iter = m_Options.begin(); iter != m_Options.end(); iter++)
//      {
//        String OptionName = iter->first;
//        TOption Option = iter->second;
//        switch (VarValue.GetVarType())
//        {
//        case varInt:
//          Ini->WriteInteger(SectionName, OptionName, VarValue.GetIntVal());
//          break;
//        case varString:
//          Ini->WriteString(SectionName, OptionName, VarValue.GetStringVal());
//          break;
//        case varBool:
//          Ini->WriteBool(SectionName, OptionName, VarValue.GetBoolVal());
//          break;
//        }
//      }
//      delete Ini;
//      return true;
//    }
//
//    //-------------------------------------------------------------------------
//    bool __fastcall SaveToRegistry()
//    {
//      bool bSuccess = false;
//      TRegistry *Reg = new TRegistry();
//      Reg->RootKey = HKEY_CURRENT_USER;
//      try
//      {
//        if (Reg->OpenKey(m_RegBaseKey + m_ToolName, false))
//        {
//          for (option_iterator iter = m_Options.begin(); iter != m_Options.end(); iter++)
//          {
//            String OptionName = iter->first;
//            TVarOption VarValue = iter->second;
//            switch (VarValue.GetVarType())
//            {
//            case varInt:
//              Reg->WriteInteger(OptionName, VarValue.GetIntVal());
//              break;
//            case varString:
//              Reg->WriteString(OptionName, VarValue.GetStringVal());
//              break;
//            case varBool:
//              Reg->WriteBool(OptionName, VarValue.GetBoolVal());
//              break;
//            }
//          }
//          Reg->CloseKey();
//          bSuccess = true;
//        }
//      }
//      __finally
//      {
//        delete Reg;
//      }
//      return bSuccess;
//    }
//


  };

} // namespace TopTools
#endif

/*
// QueryKey - Enumerates the subkeys of key and its associated values.
//     hKey - Key whose subkeys and values are to be enumerated.

//#include <windows.h>
//#include <stdio.h>
//#include <tchar.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

    void QueryKey(HKEY hKey)
    {
        TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
        DWORD    cbName;                   // size of name string
        TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name
        DWORD    cchClassName = MAX_PATH;  // size of class string
        DWORD    cSubKeys = 0;             // number of subkeys
        DWORD    cbMaxSubKey;              // longest subkey size
        DWORD    cchMaxClass;              // longest class string
        DWORD    cValues;              // number of values for key
        DWORD    cchMaxValue;          // longest value name
        DWORD    cbMaxValueData;       // longest value data
        DWORD    cbSecurityDescriptor; // size of security descriptor
        FILETIME ftLastWriteTime;      // last write time

        DWORD i, retCode;

        TCHAR  achValue[MAX_VALUE_NAME];
        DWORD cchValue = MAX_VALUE_NAME;

        // Get the class name and the value count.
        retCode = RegQueryInfoKey(
                      hKey,                    // key handle
                      achClass,                // buffer for class name
                      &cchClassName,           // size of class string
                      NULL,                    // reserved
                      &cSubKeys,               // number of subkeys
                      &cbMaxSubKey,            // longest subkey size
                      &cchMaxClass,            // longest class string
                      &cValues,                // number of values for this key
                      &cchMaxValue,            // longest value name
                      &cbMaxValueData,         // longest value data
                      &cbSecurityDescriptor,   // security descriptor
                      &ftLastWriteTime);       // last write time

        // Enumerate the subkeys, until RegEnumKeyEx fails.

        if (cSubKeys)
        {
//            printf( "\nNumber of subkeys: %d\n", cSubKeys);

            for (i = 0; i < cSubKeys; i++)
            {
                cbName = MAX_KEY_LENGTH;
                retCode = RegEnumKeyEx(hKey, i,
                                       achKey,
                                       &cbName,
                                       NULL,
                                       NULL,
                                       NULL,
                                       &ftLastWriteTime);
                if (retCode == ERROR_SUCCESS)
                {
//                    _tprintf(TEXT("(%d) %s\n"), i+1, achKey);
                }
            }
        }

        // Enumerate the key values.

        if (cValues)
        {
//            printf( "\nNumber of values: %d\n", cValues);

            for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
            {
                cchValue = MAX_VALUE_NAME;
                achValue[0] = '\0';
                retCode = RegEnumValue(hKey, i,
                                       achValue,
                                       &cchValue,
                                       NULL,
                                       NULL,
                                       NULL,
                                       NULL);

                if (retCode == ERROR_SUCCESS )
                {
//                    _tprintf(TEXT("(%d) %s\n"), i+1, achValue);
                }
            }
        }
    }

//void _tmain(void)
//{
//   HKEY hTestKey;
//
//   if( RegOpenKeyEx( HKEY_CURRENT_USER,
//        TEXT("SOFTWARE\\Microsoft"),
//        0,
//        KEY_READ,
//        &hTestKey) == ERROR_SUCCESS
//      )
//   {
//      QueryKey(hTestKey);
//   }
//
//}
*/

