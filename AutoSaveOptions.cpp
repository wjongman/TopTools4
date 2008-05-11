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
//  m_pOptions = new TopTools::Options("capture\\autosave");
  m_sToolName = "capture\\autosave";

  m_Options.Set(m_sToolName, "directory", GetSpecialFolderPath(CSIDL_DESKTOPDIRECTORY));
  m_Options.Set(m_sToolName, "filename", "Snapshot");
  m_Options.Set(m_sToolName, "digits", 2);
  m_Options.Set(m_sToolName, "nextvalue", 1);
  m_Options.Set(m_sToolName, "imagetype", 0);
  m_Options.Set(m_sToolName, "existaction", 0);
  m_Options.Set(m_sToolName, "bypassmenu", false);
  m_Options.Set(m_sToolName, "continuous", false);
}

//---------------------------------------------------------------------------
TAutoSaveOptions::~TAutoSaveOptions()
{
//  delete m_pOptions;
}

//---------------------------------------------------------------------------
void __fastcall TAutoSaveOptions::Load()
{
  m_Options.Load();
}

//---------------------------------------------------------------------------
void __fastcall TAutoSaveOptions::Save()
{
  m_Options.Save();
}

//---------------------------------------------------------------------------
int TAutoSaveOptions::GetInt(const String& OptionName)
{
  return m_Options.GetInt(m_sToolName, OptionName);
}

//---------------------------------------------------------------------------
String TAutoSaveOptions::GetString(const String& OptionName)
{
  return m_Options.GetString(m_sToolName, OptionName);
}

//---------------------------------------------------------------------------
bool TAutoSaveOptions::GetBool(const String& OptionName)
{
  return m_Options.GetBool(m_sToolName, OptionName);
}

//---------------------------------------------------------------------------
void TAutoSaveOptions::Set(const String& OptionName, int Option)
{
  m_Options.Set(m_sToolName, OptionName, Option);
}

//---------------------------------------------------------------------------
void TAutoSaveOptions::Set(const String& OptionName, String Option)
{
  m_Options.Set(m_sToolName, OptionName, Option);
}

//---------------------------------------------------------------------------
void TAutoSaveOptions::Set(const String& OptionName, bool Option)
{
  m_Options.Set(m_sToolName, OptionName, Option);
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
    // todo: Find a way to control where it pops-up
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

