//---------------------------------------------------------------------------
#ifndef AutoSaveOptionsH
#define AutoSaveOptionsH
//----------------------------------------------------------------------------
class TAutoSaveOptions
{
public:
  TAutoSaveOptions();

  void __fastcall Load();
  void __fastcall Save();
//  void __fastcall LoadFromRegistry();
//  void __fastcall SaveToRegistry();

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
