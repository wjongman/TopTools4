//---------------------------------------------------------------------------
#ifndef AutoSaveOptionsH
#define AutoSaveOptionsH

#include "Persist.h"

//----------------------------------------------------------------------------
class TAutoSaveOptions
{
public:
  TAutoSaveOptions();
  virtual ~TAutoSaveOptions();

  void __fastcall Load();
  void __fastcall Save();

  int GetInt(const String& OptionName);
  String GetString(const String& OptionName);
  bool GetBool(const String& OptionName);

  void Set(const String& OptionName, int Option);
  void Set(const String& OptionName, String Option);
  void Set(const String& OptionName, bool Option);

  void IncrementNextValue() { Set("nextvalue", GetInt("nextvalue") + 1); }
  String GetFullPathName();

private:
  TopTools::Options* m_pOptions;

  String GetSpecialFolderPath(int FolderSpec);
  String GetFirstFilename(int startvalue);
  String GetNextFilename();
  String GetExtension(int index);
  String GetSequenceString();
};

//---------------------------------------------------------------------------
#endif
