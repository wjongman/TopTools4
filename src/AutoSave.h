//---------------------------------------------------------------------------
#ifndef AutoSaveH
#define AutoSaveH
//----------------------------------------------------------------------------
class TAutoSave
{
public:
    TAutoSave();
    ~TAutoSave();

    void __fastcall LoadOptions();
    void __fastcall SaveOptions();
    void SaveBitmap(Graphics::TBitmap* pBitmap);
    String GetFullPathName();

    String Directory;
    String Prefix;
    int Digits;
    int NextValue;
    int ImageType;
    int ExistAction;

private:
    String GetExtension(int index);
    String GetSequenceString();
    String GetSpecialFolderPath(int FolderSpec);
    String GetDesktopPath();
};

//---------------------------------------------------------------------------
#endif
