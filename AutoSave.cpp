//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AutoSave.h"
#include "PersistOptions.h"

TAutoSave::TAutoSave()
: m_sToolName("capture\\autosave")
{
    // Init default options
  // Init with default values
  Directory = GetSpecialFolderPath(CSIDL_DESKTOPDIRECTORY);
  Prefix = "Snapshot";
  Digits = 2;
  NextValue = 1;
  ImageType = 0;
  ExistAction = 0;
  Bypass = false;
  Continuous = false;
//     g_ToolOptions.Set(m_sToolName, "directory", GetSpecialFolderPath(CSIDL_DESKTOPDIRECTORY));
//     g_ToolOptions.Set(m_sToolName, "filename", "Snapshot");
//     g_ToolOptions.Set(m_sToolName, "digits", 2);
//     g_ToolOptions.Set(m_sToolName, "nextvalue", 1);
//     g_ToolOptions.Set(m_sToolName, "imagetype", 0);
//     g_ToolOptions.Set(m_sToolName, "existaction", 0);
//     g_ToolOptions.Set(m_sToolName, "bypassmenu", false);
//     g_ToolOptions.Set(m_sToolName, "continuous", false);
}

//---------------------------------------------------------------------------
String TAutoSave::GetFirstFilename(int startvalue)
{
    g_ToolOptions.Set(m_sToolName, "nextvalue", startvalue);
    return GetFullPathName();
}

//---------------------------------------------------------------------------
String TAutoSave::GetNextFilename()
{
    IncrementNextValue();
    return GetFullPathName();
}

//---------------------------------------------------------------------------
void TAutoSave::IncrementNextValue()
{
    int nextvalue = g_ToolOptions.GetInt(m_sToolName, "nextvalue") + 1;
    g_ToolOptions.Set(m_sToolName, "nextvalue", nextvalue);
}

//---------------------------------------------------------------------------
String TAutoSave::GetFullPathName()
{
    String Directory = g_ToolOptions.GetString(m_sToolName, "directory");
    String Filename = g_ToolOptions.GetString(m_sToolName, "directory");
    int imagetype = g_ToolOptions.GetInt(m_sToolName, "imagetype");

    return Directory + Filename + GetSequenceString() + GetExtension(imagetype);
}

//---------------------------------------------------------------------------
String TAutoSave::GetSequenceString()
{
    String SeqNum = "";

    int digits = g_ToolOptions.GetInt(m_sToolName, "digits");
    int nextvalue = g_ToolOptions.GetInt(m_sToolName, "nextvalue");
    if (digits != 0)
        SeqNum.sprintf("%0*d", digits, nextvalue);

    return SeqNum;
}

//---------------------------------------------------------------------------
String TAutoSave::GetExtension(int index)
{
    if (index < 0 || index > 4)
        index = 0;

    char* Extension[] = { ".bmp", ".png", ".gif", ".jpg"};
    return Extension[index];
}

#include <shlobj.h> // Make sure NO_WIN32_LEAN_AND_MEAN is defined..
//---------------------------------------------------------------------------
String TAutoSave::GetSpecialFolderPath(int FolderSpec)
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

#pragma package(smart_init)

