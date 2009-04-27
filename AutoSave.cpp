//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Registry.hpp>
#include <Filectrl.hpp>
#include <shlobj.h> // Make sure NO_WIN32_LEAN_AND_MEAN is defined..

#include "AutoSave.h"
#include "PersistOptions.h"
#include "PersistImage.h"

//---------------------------------------------------------------------------
TAutoSave::TAutoSave()
{
    // Init with default values
    Directory = GetSpecialFolderPath(CSIDL_DESKTOPDIRECTORY);
    Prefix = "Snapshot";
    Digits = 2;
    NextValue = 1;
    ImageType = 1;
    ExistAction = 0;
}

//---------------------------------------------------------------------------
String TAutoSave::GetFirstFilename(int startvalue)
{
    NextValue = startvalue;
    return GetFullPathName();
}

//---------------------------------------------------------------------------
String TAutoSave::GetNextFilename()
{
    NextValue++;
    return GetFullPathName();
}

//---------------------------------------------------------------------------
String TAutoSave::GetFullPathName()
{
    return Directory + Prefix + GetSequenceString() + GetExtension(ImageType);
}

//---------------------------------------------------------------------------
String TAutoSave::GetSequenceString()
{
    String SeqNum = "";

    if (Digits != 0)
        SeqNum.sprintf("%0*d", Digits, NextValue);

    return SeqNum;
}

//---------------------------------------------------------------------------
String TAutoSave::GetExtension(int index)
{
    if (index < 0 || index > 3)
        return 0;

    char* Extension[] = { ".bmp", ".png", ".gif", ".jpg"};
    return Extension[index];
}

//---------------------------------------------------------------------------
void __fastcall TAutoSave::LoadOptions()
{
    String ToolName = "capture\\autosave";

    Directory = g_ToolOptions.Get(ToolName, "directory", Directory);
    Prefix = g_ToolOptions.Get(ToolName, "filename", Prefix);
    Digits = g_ToolOptions.Get(ToolName, "digits", Digits);
    NextValue = g_ToolOptions.Get(ToolName, "nextvalue", NextValue);
    ImageType = g_ToolOptions.Get(ToolName, "imagetype", ImageType);
    ExistAction = g_ToolOptions.Get(ToolName, "existaction", ExistAction);
}

//---------------------------------------------------------------------------
void __fastcall TAutoSave::SaveOptions()
{
    String ToolName = "capture\\autosave";

    g_ToolOptions.Set(ToolName, "directory", Directory);
    g_ToolOptions.Set(ToolName, "filename", Prefix);
    g_ToolOptions.Set(ToolName, "digits", Digits);
    g_ToolOptions.Set(ToolName, "nextvalue", NextValue);
    g_ToolOptions.Set(ToolName, "imagetype", ImageType);
    g_ToolOptions.Set(ToolName, "existaction", ExistAction);
}

//---------------------------------------------------------------------------
String TAutoSave::GetSpecialFolderPath(int FolderSpec)
// Wrapper function around the SHGetSpecialFolderLocation +
// SHGetPathFromIDList API pair, handling allocation intrinsics.
{
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
void TAutoSave::SaveBitmap(Graphics::TBitmap* pBitmap)
{
    // First check if the target directory exists
    if (!DirectoryExists(Directory))
    {
        // todo: offer to change or create directory here..
        // todo: stuff all static strings in a resource file
        String sMsg = "Your autosave settings refer to a directory that doesn't exist: \n\n";
        ShowMessage(sMsg + Directory);
        return;
    }

    // Find first available filename (might be slow on huge directories..)
    while (FileExists(GetFullPathName()))
    {
        IncrementNextValue();
    }

    TPersistImage image(pBitmap);
    image.Save(GetFullPathName());

    IncrementNextValue();
}

//---------------------------------------------------------------------------

