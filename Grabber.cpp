//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//#include <vcl\Clipbrd.hpp>
//#include <Registry.hpp>
//#include <Filectrl.hpp>
//#include <jpeg.hpp>
/*
#include "gif\gifimage.hpp"
#include "png\pngimage.hpp"
*/
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
//    m_GrabberMode = gmOpenViewer;
    m_GrabberMode = gmShowMenu;

    OnRightButtonClick = HandleRightButtonClick;
}

//---------------------------------------------------------------------------
__fastcall TScreenGrabber::~TScreenGrabber()
{
    if (m_CaptureMenu)
        delete m_CaptureMenu;

    delete m_pBufferBmp;
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::UpdateSettings()
{
    TRACE("TScreenGrabber::UpdateSettings()");

//    m_CaptureOptions.Load();
//    m_AutoSaveOptions.LoadFromRegistry();
//    m_AutoSaveOptions.Load();
//  PopulateCaptureMenu();
}
/*
//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::MouseDown(TMouseButton Button,
        TShiftState Shift, int X, int Y)
{
    TRACE("TScreenGrabber::MouseDown()");
    if (Button == mbLeft && !m_bDrawing)
    {
        // Mark start of selection rectangle
        TPoint ptLeftTop = ClientToScreen(Point(X, Y));

        m_rcSelect.left = m_rcSelect.right = ptLeftTop.x;
        m_rcSelect.top = m_rcSelect.bottom = ptLeftTop.y;
        m_rcSelect.right = m_rcSelect.right = ptLeftTop.x;
        m_rcSelect.bottom = m_rcSelect.bottom = ptLeftTop.y;

        m_bFirstRect = true;
        m_bDrawing = true;
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::MouseMove(TShiftState Shift, int X, int Y)
{
    if (m_bDrawing)
    {
        DrawSelectRect(m_rcSelect);

        TPoint ptRightBottom = ClientToScreen(Point(X, Y));
        m_rcSelect.right = ptRightBottom.x;
        m_rcSelect.bottom = ptRightBottom.y;

        DrawSelectRect(m_rcSelect);
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::MouseUp(TMouseButton Button,
                                        TShiftState Shift, int X, int Y)
{
    TRACE("TScreenGrabber::MouseUp()");

    // Right button cancels the operation
    if (Button == mbRight)
    {
        if (m_bDrawing)
        {
            DrawSelectRect(m_rcSelect);
            m_bDrawing = false;
        }
        // Release mouse-hook
        StopTracking();
    }

    // Left button triggers capture
    else if (Button == mbLeft && m_bDrawing)
    {
        DrawSelectRect(m_rcSelect);
        m_bDrawing = false;

        // Release mouse-hook
        StopTracking();

        // Copy selected area to the bitmap
        GetDesktopArea(&m_rcSelect);

        // Determine what to do with the bitmap
        HandleCapture(X, Y);
    }
}
*/
//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::HandleRightButtonClick(TObject *Sender,
                        TMouseButton Button, TShiftState Shift, int X, int Y)
{
     GetDesktopArea();
     ViewImage(m_pBufferBmp);
     EndCapture();
// Left Click to drag or resize, Right Click to grab area.
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::HandleCapture(int X, int Y)
{
    if (g_ToolOptions.GetBool("capture\\autosave", "bypassmenu"))
    {
        AutoSaveToFile();
        if (g_ToolOptions.GetBool("capture\\autosave", "continuous"))
        {
            CaptureNext();
        }
        else
        {
            EndCapture();
        }
    }
    else if (g_ToolOptions.GetBool("capture\\autosave", "openviewer"))
    {
        EndCapture();
        ViewImage(m_pBufferBmp);
    }
    else
    {
        // Ask user what to do with this grabbed bitmap
        POINT ptAbs = ClientToScreen(Point(X, Y));
        ShowCaptureMenu(ptAbs.x, ptAbs.y);
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::ShowCaptureMenu(int X, int Y)
{
    PopulateCaptureMenu();

    m_CaptureMenu->Popup(X, Y);
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
    NewItem->Caption = "Snapshot";
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

//     // Separator ------------------------
//     NewItem = new TMenuItem(m_CaptureMenu);
//     NewItem->Caption = "-";
//     m_CaptureMenu->Items->Add(NewItem);
//
// //     NewItem = new TMenuItem(m_CaptureMenu);
// //     NewItem->OnClick = CaptureMenuClick;
// //     NewItem->Caption = "View...";
// //     NewItem->Hint = "View";
// //     m_CaptureMenu->Items->Add(NewItem);
//
//     NewItem = new TMenuItem(m_CaptureMenu);
//     NewItem->OnClick = CaptureMenuClick;
//     NewItem->Caption = "Print...";
//     NewItem->Hint = "Print";
//     m_CaptureMenu->Items->Add(NewItem);
//
    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Auto Save";
    NewItem->Hint = "AutoSave";
    NewItem->Enabled = g_ToolOptions.GetBool("capture", "autosave");//m_CaptureOptions.AutoSave;
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Auto Save && Grab More";
    NewItem->Hint = "AutoSaveOn";
    NewItem->Enabled = g_ToolOptions.GetBool("capture", "autosave");//m_CaptureOptions.AutoSave;
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
    TPersistImage image(m_pBufferBmp);

    String InitialDir = g_ToolOptions.GetString("capture", "lastdir");
    int filterindex = g_ToolOptions.GetInt("capture", "filterindex");

    image.SaveFileDialog(filterindex, InitialDir);

    g_ToolOptions.Set("capture", "lastdir", InitialDir);
    g_ToolOptions.Set("capture", "filterindex", filterindex);

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
    TRACE("TScreenGrabber::ViewImage()");

//    GetDesktopArea();

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
    TRACE("TScreenGrabber::AutoSaveToFile()");

    // First check if the target directory exists
    if (!DirectoryExists(g_ToolOptions.GetString("capture\autosave", "directory")))
    {
        // We should offer to change or create directory here..
        String sMsg = "Your autosave settings refer to a directory that doesn't exist: \n\n";
        ShowMessage(sMsg + g_ToolOptions.GetString("capture\autosave", "directory"));//m_AutoSaveOptions.GetString("directory"));
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
//    m_AutoSaveOptions.SaveToRegistry();
    m_AutoSaveOptions.Save();
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::GetDesktopArea()
{
    TRACE("TScreenGrabber::GetDesktopArea()");
    GetWindowRect(Handle, &m_rcSelect);
    Hide();  // we are transparent but under Aero we are visible
    GetDesktopArea(&m_rcSelect);
    Show();
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::GetDesktopArea(LPRECT lpRect)
{
    TRACE("TScreenGrabber::GetDesktopArea()");

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

