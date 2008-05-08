//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AutoSaveOptions.h"

// Global String for registry access
extern const String g_RegBaseKey;


//---------------------------------------------------------------------------
TAutoSaveOptions::TAutoSaveOptions()
{
  // Init default options
  m_pOptions = new TopTools::Options("capture\\autosave");

  m_pOptions->Set("directory", GetSpecialFolderPath(CSIDL_DESKTOPDIRECTORY));
  m_pOptions->Set("filename", "Snapshot");
  m_pOptions->Set("digits", 2);
  m_pOptions->Set("nextvalue", 1);
  m_pOptions->Set("imagetype", 0);
  m_pOptions->Set("existaction", 0);
  m_pOptions->Set("bypassmenu", false);
  m_pOptions->Set("continuous", false);
}

//---------------------------------------------------------------------------
TAutoSaveOptions::~TAutoSaveOptions()
{
  delete m_pOptions;
}

//---------------------------------------------------------------------------
void __fastcall TAutoSaveOptions::Load()
{
  m_pOptions->Load();
}

//---------------------------------------------------------------------------
void __fastcall TAutoSaveOptions::Save()
{
  m_pOptions->Save();
}

//---------------------------------------------------------------------------
int TAutoSaveOptions::GetInt(const String& OptionName)
{
  return m_pOptions->Get(OptionName, 1);
}

//---------------------------------------------------------------------------
String TAutoSaveOptions::GetString(const String& OptionName)
{
  return m_pOptions->Get(OptionName, "");
}

//---------------------------------------------------------------------------
bool TAutoSaveOptions::GetBool(const String& OptionName)
{
  return m_pOptions->Get(OptionName, false);
}

//---------------------------------------------------------------------------
void TAutoSaveOptions::Set(const String& OptionName, int Option)
{
  m_pOptions->Set(OptionName, Option);
}

//---------------------------------------------------------------------------
void TAutoSaveOptions::Set(const String& OptionName, String Option)
{
  m_pOptions->Set(OptionName, Option);
}

//---------------------------------------------------------------------------
void TAutoSaveOptions::Set(const String& OptionName, bool Option)
{
  m_pOptions->Set(OptionName, Option);
}

//---------------------------------------------------------------------------
String TAutoSaveOptions::GetFirstFilename(int startvalue)
{
  Set("nextvalue", startvalue);
  return GetFullPathName();
}

//---------------------------------------------------------------------------
String TAutoSaveOptions::GetNextFilename()
{
  IncrementNextValue();
  return GetFullPathName();
}

//---------------------------------------------------------------------------
String TAutoSaveOptions::GetFullPathName()
{
  return GetString("directory") + GetString("filename") + GetSequenceString()
    + GetExtension(GetInt("imagetype"));
}

//---------------------------------------------------------------------------
String TAutoSaveOptions::GetSequenceString()
{
  String SeqNum = "";

  if (GetInt("digits") != 0)
    SeqNum.sprintf("%0*d", GetInt("digits"), GetInt("nextvalue"));

  return SeqNum;
}

//---------------------------------------------------------------------------
String TAutoSaveOptions::GetExtension(int index)
{
  if (index < 0 || index > 4)
    index = 0;

  char* Extension[] = { ".bmp", ".png", ".gif", ".jpg"};
  return Extension[index];
}

#include <shlobj.h> // Make sure NO_WIN32_LEAN_AND_MEAN is defined..
//---------------------------------------------------------------------------
String TAutoSaveOptions::GetSpecialFolderPath(int FolderSpec)
{
  // Wrapper function around the SHGetSpecialFolderLocation +
  // SHGetPathFromIDList API pair, handling allocation intrinsics.
  String sResult = "";

  // Get the shell's IMalloc interface, we must use this
  // interface to free the memory that is allocated by
  // the shell when it passes us an ITEMIDLIST in *pidl
  LPMALLOC Allocator;
  if (::SHGetMalloc(&Allocator) == NOERROR)
  {
    ITEMIDLIST *pidl;
    HWND hwndOwner = NULL;
    if (::SHGetSpecialFolderLocation(hwndOwner, FolderSpec, &pidl) == NOERROR)
    {
      char folderpath[MAX_PATH];
      if (::SHGetPathFromIDList(pidl, folderpath))
      {
        sResult = folderpath;
        sResult += "\\";
      }
      Allocator->Free(pidl);
    }
    Allocator->Release();
  }
  return sResult;
}

//---------------------------------------------------------------------------

