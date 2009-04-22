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

    // TGrabberMode can be one of:
    // gmOpenViewer, gmShowMenu, gmBypassMenu, gmCopy, gmContinuous
    m_GrabberMode = gmShowMenu;

    OnRightButtonClick = HandleRightButtonClick;

    m_AutoSaver.LoadOptions();
}

//---------------------------------------------------------------------------
__fastcall TScreenGrabber::~TScreenGrabber()
{
    delete m_CaptureMenu;
    delete m_pBufferBmp;

    m_AutoSaver.SaveOptions();
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::UpdateSettings()
{
    m_AutoSaver.LoadOptions();
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::HandleCapture(int X, int Y)
{
    if (m_AutoSaver.Bypass)
    {
        AutoSaveToFile();
        if (m_AutoSaver.Continuous)
        {
            CaptureNext();
        }
        else
        {
            EndCapture();
        }
    }
//     else if (m_AutoSaver.Openviewer)
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
         EndCapture();
         break;

     case gmShowMenu:
         ShowCaptureMenu(X, Y);
         break;

     case gmBypassMenu:
         AutoSaveToFile();
         EndCapture();
         break;

     case gmContinuous:
         AutoSaveToFile();
         CaptureNext();
         break;

     default:
         EndCapture();
         break;
     }
//     EndCapture();
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
            if (SaveToFile())
                EndCapture();
        }
        else if (menuItem->Hint == "SaveOn")
        {
            if (SaveToFile())
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
            //CaptureNext();  // which one makes more sense?
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
    NewItem->Enabled = m_AutoSaver.Enabled;
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Auto Save && Grab More";
    NewItem->Hint = "AutoSaveOn";
    NewItem->Enabled = m_AutoSaver.Enabled;
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
bool __fastcall TScreenGrabber::SaveToFile()
{
    String InitialDir = g_ToolOptions.Get("capture", "lastsavedir", "%USERPROFILE%\\Desktop");
    int filterindex = g_ToolOptions.Get("capture", "lastimagetype", 2);

    TPersistImage image(m_pBufferBmp);
    bool result = image.SaveFileDialog(filterindex, InitialDir);
    if (result)
    {
        g_ToolOptions.Set("capture", "lastsavedir", InitialDir);
        g_ToolOptions.Set("capture", "lastimagetype", filterindex);

        m_pBufferBmp->Assign(NULL);
    }

    return result;
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
    pImageViewer->OnKeyPress = ViewerKeyPress;
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
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::ViewerKeyPress(TObject *Sender, char &Key)
{
    if (Key == VK_ESCAPE)
    {
        TImageViewer* viewer = reinterpret_cast<TImageViewer*>(Sender);
        if (viewer)
        {
            PostMessage(viewer->Handle, WM_CLOSE, 0, 0);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::AutoSaveToFile()
{
    m_AutoSaver.SaveBitmap(m_pBufferBmp);
}

//---------------------------------------------------------------------------
// Version independent way of checking if Aero is active
typedef HRESULT (CALLBACK * pfDwmIsCompositionEnabled)(BOOL *pfEnabled);
bool IsCompositionEnabled()
{
    // Detect Aero
    bool result = false;

    HMODULE library = ::LoadLibrary("dwmapi.dll");
    if (library)
    {
        pfDwmIsCompositionEnabled fIsEnabled;
        fIsEnabled = (pfDwmIsCompositionEnabled)::GetProcAddress(library, "DwmIsCompositionEnabled");
        if (fIsEnabled)
        {
            BOOL enabled = FALSE;
            result = SUCCEEDED(fIsEnabled(&enabled)) && enabled;
        }
        ::FreeLibrary(library);
    }

    return result;
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::GetDesktopArea()
{
    // todo: Cache result of IsCompositionEnabled() and
    // implement handling the 
    bool AeroActive = IsCompositionEnabled();

    if (AeroActive)
    {
        // We are transparent but under Aero we are visible so
        // we need to hide.
        // However when Aero is not active, hiding our window
        // forces a redraw of the underlying window and causes
        // GetDesktopArea() to fetch incomplete screen-grabs.
        Hide();
    }

    GetWindowRect(Handle, &m_rcSelect);
    GetDesktopArea(&m_rcSelect);

    if (AeroActive)
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

