//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Grabber.h"
#include "AutoSaveDlg.h"
#include "PersistImage.h"

//---------------------------------------------------------------------------
__fastcall TScreenGrabber::TScreenGrabber(TComponent* Owner)
      : TScreenForm(Owner),
        FOnCaptureNext(NULL),
        FOnCaptureComplete(NULL),
        m_CaptureMenu(NULL)
{
    // Have a bitmap to store the grabbed stuff
    m_pBufferBmp = new Graphics::TBitmap;

    // enum TGrabberMode { gmOpenViewer, gmShowMenu, gmBypassMenu, gmCopy, gmContinuous };
    m_GrabberMode = gmShowMenu;

    OnRightButtonClick = HandleRightButtonClick;

    m_AutoSaveOptions.Load();
}

//---------------------------------------------------------------------------
__fastcall TScreenGrabber::~TScreenGrabber()
{
    if (m_CaptureMenu)
        delete m_CaptureMenu;

    delete m_pBufferBmp;

    m_AutoSaveOptions.Save();
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::UpdateSettings()
{
    m_AutoSaveOptions.Load();
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::HandleCapture(int X, int Y)
{
    if (m_AutoSaveOptions.Bypass)
    {
        AutoSaveToFile();
        if (m_AutoSaveOptions.Continuous)
        {
            CaptureNext();
        }
        else
        {
            EndCapture();
        }
    }
//     else if (m_AutoSaveOptions.Openviewer)
// //    else if (g_ToolOptions.GetBool("capture\\autosave", "openviewer"))
//     {
//         EndCapture();
//         ViewImage(m_pBufferBmp);
//     }
    else
    {
        // Ask user what to do with this grabbed bitmap
        POINT ptAbs = ClientToScreen(Point(X, Y));
        ShowCaptureMenu(ptAbs.x, ptAbs.y);
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::HandleRightButtonClick(TObject *Sender,
                        TMouseButton Button, TShiftState Shift, int X, int Y)
{
     GetDesktopArea();

     switch (m_GrabberMode)
     {
     case gmOpenViewer:
         ViewImage(m_pBufferBmp);
         break;
     case gmShowMenu:
         ShowCaptureMenu(X, Y);
         break;
     case gmBypassMenu:
         AutoSaveToFile();
         break;
     case gmContinuous:
         break;

     default:
         break;
     }
     EndCapture();
// Left Click to drag or resize, Right Click to grab area.
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::ShowCaptureMenu(int X, int Y)
{
    PopulateCaptureMenu();

    POINT ptAbs = ClientToScreen(Point(X, Y));
    m_CaptureMenu->Popup(ptAbs.x, ptAbs.y);
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::CaptureMenuClick(TObject *Sender)
{
    TMenuItem* menuItem = dynamic_cast<TMenuItem*>(Sender);

    if (menuItem)
    {
        if (menuItem->Hint == "Hide")
        {
            m_pBufferBmp->Assign(NULL);
            EndCapture();
        }
        else if (menuItem->Hint == "Save")
        {
            SaveToFile();
            EndCapture();
        }
        else if (menuItem->Hint == "SaveOn")
        {
            SaveToFile();
            CaptureNext();
        }
        else if (menuItem->Hint == "AutoSave")
        {
            AutoSaveToFile();
            EndCapture();
        }
        else if (menuItem->Hint == "AutoSaveOn")
        {
            AutoSaveToFile();
            CaptureNext();
        }
        else if (menuItem->Hint == "Copy")
        {
            CopyToClipboard();
            EndCapture();
        }
        else if (menuItem->Hint == "View")
        {
            ViewImage(m_pBufferBmp);
            EndCapture();
        }
        else if (menuItem->Hint == "Print")
        {
            Print();
            EndCapture();
        }
        else if (menuItem->Hint == "AutoSaveOptions")
        {
            AutosaveOptions();
            EndCapture();
            //CaptureNext();
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::CaptureNext()
{
    if (FOnCaptureNext)
        FOnCaptureNext(this);
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::EndCapture()
{
    Close();
    // We are done
    //if (FOnCaptureComplete)
    //    FOnCaptureComplete(this);
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::PopulateCaptureMenu()
{
    if (!m_CaptureMenu)
    {
        m_CaptureMenu = new TPopupMenu(this);
    }

    // Start with an empty menu
    m_CaptureMenu->Items->Clear();

    // Populate the menu
    TMenuItem *NewItem;

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Take Snapshot";
    NewItem->Default = true;
    NewItem->Hint = "View";
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Copy To Clipboard";
    NewItem->Hint = "Copy";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Save To File...";
    NewItem->Hint = "Save";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Save && Grab More...";
    NewItem->Hint = "SaveOn";
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "View...";
    NewItem->Hint = "View";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Print...";
    NewItem->Hint = "Print";
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Auto Save";
    NewItem->Hint = "AutoSave";
    NewItem->Enabled = m_AutoSaveOptions.AutoSave;
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Auto Save && Grab More";
    NewItem->Hint = "AutoSaveOn";
    NewItem->Enabled = m_AutoSaveOptions.AutoSave;
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Auto Save Options...";
    NewItem->Hint = "AutoSaveOptions";
//    NewItem->Enabled = m_CaptureOptions.AutoSave;
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Cancel";
    //NewItem->Default = true;
    NewItem->Hint = "Hide";
    m_CaptureMenu->Items->Add(NewItem);


#ifdef _DEBUG
    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

#endif
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::SaveToFile()
{
    m_AutoSaveOptions.Load();
    String InitialDir = m_AutoSaveOptions.LastDir;
    int filterindex = m_AutoSaveOptions.ImageType;

    TPersistImage image(m_pBufferBmp);
    image.SaveFileDialog(filterindex, InitialDir);

    m_AutoSaveOptions.ImageType = filterindex;
    m_AutoSaveOptions.LastDir = InitialDir;
    m_AutoSaveOptions.Save();

    m_pBufferBmp->Assign(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::Print()
{
    TPersistImage image(m_pBufferBmp);

    image.Print();

    m_pBufferBmp->Assign(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::CopyToClipboard()
{
    TPersistImage image(m_pBufferBmp);

    image.Copy();

    m_pBufferBmp->Assign(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::AutosaveOptions()
{
    TAutoSaveDialog *AutoSaveDialog = new TAutoSaveDialog(this);
    if (AutoSaveDialog)
    {
        if (AutoSaveDialog->ShowModal() == mrOk)
        {
            UpdateSettings();
        }
        delete AutoSaveDialog;
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::ViewImage(Graphics::TBitmap* pBufferBmp)
{
    TImageViewer* pImageViewer = m_Viewers.NewViewer(this, m_rcSelect);
    pImageViewer->Bitmap = pBufferBmp;
    pImageViewer->KeyPreview = true;
    //pImageViewer->OnKeyPress = ViewerKeyPress;
    pImageViewer->OnClose = ViewerClosed;

    pImageViewer->Show();
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::ViewerClosed(TObject *Sender, TCloseAction &Action)
{
    TImageViewer* viewer = reinterpret_cast<TImageViewer*>(Sender);
    if (viewer)
    {
        m_Viewers.DeleteViewer(viewer->Id);
    }
//    Action = caHide;
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::ViewerKeyPress(TObject *Sender, char &Key)
{
    // todo: Raises exception !!
    if (Key == VK_ESCAPE)
    {
        TImageViewer* viewer = reinterpret_cast<TImageViewer*>(Sender);
        if (viewer)
        {
            // Nuke OnClose handler, we don't want to unleash recursive close actions..
//            viewer->OnClose = NULL;
            viewer->Close();
//            m_Viewers.DeleteViewer(viewer->Id);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::AutoSaveToFile()
{
    m_AutoSaveOptions.Load();

    // First check if the target directory exists
    if (!DirectoryExists(m_AutoSaveOptions.Directory))
    {
        // todo: offer to change or create directory here..
        // todo: stuff all static strings in a resource file
        String sMsg = "Your autosave settings refer to a directory that doesn't exist: \n\n";
        ShowMessage(sMsg + m_AutoSaveOptions.Directory);
        return;
    }

    // Find first available filename (might be slow on huge directories..)
    while (FileExists(m_AutoSaveOptions.GetFullPathName()))
    {
        m_AutoSaveOptions.IncrementNextValue();
    }

    TPersistImage image(m_pBufferBmp);
    image.Save(m_AutoSaveOptions.GetFullPathName());

    m_AutoSaveOptions.IncrementNextValue();
    m_AutoSaveOptions.Save();
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::GetDesktopArea()
{
    GetWindowRect(Handle, &m_rcSelect);
    Hide();  // we are transparent but under Aero we are visible
    GetDesktopArea(&m_rcSelect);
    Show();
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::GetDesktopArea(LPRECT lpRect)
{
    // Make sure that left < right and top < bottom
    int left = min(lpRect->left, lpRect->right);
    int top = min(lpRect->top, lpRect->bottom);
    int right = max(lpRect->left, lpRect->right);
    int bottom = max(lpRect->top, lpRect->bottom);

    int width = right - left;
    int height = bottom - top;

    HDC dcDesktop = GetDC(NULL);
    m_pBufferBmp->Handle = CreateCompatibleBitmap(dcDesktop, width, height);

    if (GetDeviceCaps(dcDesktop, RASTERCAPS) & RC_PALETTE)
    {
        // If the screen is a paletted device, we have to
        // copy the palette info into the bitmap
        int palette_size = GetDeviceCaps(dcDesktop, SIZEPALETTE);
        if (palette_size == 256)
        {
            int size = sizeof(LOGPALETTE) + 256 * sizeof(PALETTEENTRY);
            LPLOGPALETTE lplogpal = (LPLOGPALETTE) new unsigned char[size];
            lplogpal->palVersion = 0x300;
            lplogpal->palNumEntries = 256;
            GetSystemPaletteEntries(dcDesktop, 0, 256, lplogpal->palPalEntry);
            m_pBufferBmp->Palette = CreatePalette(lplogpal);
            delete [] lplogpal;
        }
    }

    // Copy the screen to our bitmap
    BitBlt(m_pBufferBmp->Canvas->Handle,
           0, 0, m_pBufferBmp->Width, m_pBufferBmp->Height,
           dcDesktop, left, top, SRCCOPY);

    ReleaseDC(NULL, dcDesktop);
}

//---------------------------------------------------------------------------

