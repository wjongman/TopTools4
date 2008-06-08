//---------------------------------------------------------------------------

#ifndef AutoSaveH
#define AutoSaveH
//----------------------------------------------------------------------------
class TAutoSave
{
public:
    TAutoSave();
    virtual ~TAutoSave();

    void IncrementNextValue();
    String GetFullPathName();

private:
    String m_sToolName;

    String GetSpecialFolderPath(int FolderSpec);
    String GetFirstFilename(int startvalue);
    String GetNextFilename();
    String GetExtension(int index);
    String GetSequenceString();
};


//---------------------------------------------------------------------------
#endif

