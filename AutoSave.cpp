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
    LoadOptions();
}

//---------------------------------------------------------------------------
TAutoSave::~TAutoSave()
{
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

    char* Extension[] = { ".bmp", ".png", ".gif", ".jpg" };
    return Extension[index];
}

//---------------------------------------------------------------------------
void __fastcall TAutoSave::LoadOptions()
{
    String ToolName = "capture\\autosave";

    Directory = g_ToolOptions.Get(ToolName, "directory", GetDesktopPath());
    Prefix = g_ToolOptions.Get(ToolName, "filename", "Snapshot");
    Digits = g_ToolOptions.Get(ToolName, "digits", 2);
    NextValue = g_ToolOptions.Get(ToolName, "nextvalue", 1);
    ImageType = g_ToolOptions.Get(ToolName, "imagetype", 1);
    ExistAction = g_ToolOptions.Get(ToolName, "existaction", 0);
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
String TAutoSave::GetDesktopPath()
{
    return GetSpecialFolderPath(CSIDL_DESKTOPDIRECTORY);
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

    String SavePath = GetFullPathName();
    if (FileExists(SavePath))
    {
        switch (ExistAction)
        {
        case 0: // Prompt
            {
                String Msg = "Overwrite existing file?";
                TMsgDlgButtons Buttons = TMsgDlgButtons() << mbOK << mbCancel;
                int ret = MessageDlg(Msg, mtConfirmation, Buttons, 0);
                if (ret == mrCancel)
                {
                    return;
                }
            }
            break;

        case 1: // Overwrite
            break;

        case 2: // Auto rename
            while (FileExists(GetFullPathName()))
            {
                // Find first available filename (might be slow on huge directories..)
                NextValue++;
            }
            SavePath = GetFullPathName();
            break;
        }
    }

    TPersistImage image(pBitmap);
    image.Save(SavePath);

    NextValue++;
}

//---------------------------------------------------------------------------

