//---------------------------------------------------------------------------
#ifndef AutoSaveH
#define AutoSaveH
//----------------------------------------------------------------------------
class TAutoSave
{
public:
  TAutoSave();

  void __fastcall LoadOptions();
  void __fastcall SaveOptions();

  String GetFullPathName();
  String GetFirstFilename(int startvalue);
  String GetNextFilename();
  String GetExtension(int index);
  String GetSequenceString();


  void IncrementNextValue() { NextValue++; }

  String Directory;
  String Prefix;
  int Digits;
  int NextValue;
  int ImageType;
  int ExistAction;
  bool Bypass;
  bool Continuous;
  bool AutoSave;
  String LastDir;

private:
  String GetSpecialFolderPath(int FolderSpec);
};

//---------------------------------------------------------------------------
#endif
