//---------------------------------------------------------------------------
#ifndef TopToolsToolOptionsH
#define TopToolsToolOptionsH

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
    TOption(const String& sValue)
    {
        IntVal = StrToIntDef(sValue, 0);
        StringVal = sValue;
    }

    //-------------------------------------------------------------------------
    TOption(bool bValue)
    {
        IntVal = bValue ? 1 : 0;
        StringVal = IntToStr(IntVal);
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
        // However, the registry requires us to specify a data type
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


#endif // #ifndef TopToolsToolOptionsH


