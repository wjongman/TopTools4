//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Grabber.h"
#include "AutoSaveDlg.h"
#include "PersistImage.h"
#include "PresetManager.h"

//---------------------------------------------------------------------------
__fastcall TScreenGrabber::TScreenGrabber(TComponent* Owner)
  : TScreenForm(Owner),
    FOnCaptureComplete(NULL),
    m_CaptureMenu(NULL)
{
    // Have a bitmap to store the grabbed stuff
    m_pBufferBmp = new Graphics::TBitmap;

    OnRightButtonClick = HandleRightButtonClick;

    m_AutoSaver.LoadOptions();
    m_PresetList = TGrabberPresets::LoadFromIniFile("..\\presets.ini");
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
void __fastcall TScreenGrabber::WndProc(Messages::TMessage &Message)
{
    switch (Message.Msg)
    {
    case WM_KEYDOWN:
        {
            bool shift = ::GetKeyState(VK_SHIFT) & 0x8000;
            switch (Message.WParam)
            {
            case VK_RETURN:
                GetDesktopArea();
                ViewImage(m_pBufferBmp);
                EndCapture();
                break;

            case 'C':
                GetDesktopArea();
                CopyToClipboard();
                EndCapture();
                break;

            case 'S':
                GetDesktopArea();
                SaveToFile();
                if (!shift)
                    EndCapture();
                break;

            case 'P':
                GetDesktopArea();
                Print();
                if (!shift)
                    EndCapture();
                break;

            case 'A':
                GetDesktopArea();
                AutoSaveToFile();
                if (!shift)
                    EndCapture();
                break;
            }
        }

    case WM_SYSKEYDOWN:
        {
            // For the Alt keys
            switch (Message.WParam)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                // Bit 29 of lParam is state of Alt key
                if (Message.LParam & 0x20000000)
                {
                    int index = Message.WParam - '0';
                    // Set dimensions according to preset value
                    DoPreset(index);
                }
                break;
            }
        }
    }
    // Base class handles the rest
    TScreenForm::WndProc(Message);
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::HandleRightButtonClick(TObject *Sender,
                                                       TMouseButton Button,
                                                       TShiftState Shift,
                                                       int X, int Y)
{
    GetDesktopArea();
    ShowCaptureMenu(X, Y);
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::ShowCaptureMenu(int X, int Y)
{
    PopulateCaptureMenu();

    POINT ptAbs = ClientToScreen(Point(X, Y));
    m_CaptureMenu->Popup(ptAbs.x, ptAbs.y);
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::PresetMenuClick(TObject *Sender)
{
    TMenuItem* menuItem = dynamic_cast<TMenuItem*>(Sender);
    if (menuItem)
    {
        DoPreset(menuItem->Tag);
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::DoPreset(int index)
{
    if (index < 0)
    {
        // Show properties dialog
    }
    else if (index == 0)
    {
        // Add current position to presets
    }
    else if (index <= (int)m_PresetList.size())
    {
        TPreset preset = m_PresetList[index-1];
        SetBounds(preset.x, preset.y, preset.w, preset.h);
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::ManagePresets(TObject *Sender)
{
    Hide();
    TPresetManager* pm = new TPresetManager(this, m_PresetList);
    if (pm->ShowModal() == mrOk)
    {
        m_PresetList = pm->GetPresetList();
    }
    delete pm;
    Show();
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::CaptureMenuClick(TObject *Sender)
{
    TMenuItem* menuItem = dynamic_cast<TMenuItem*>(Sender);

    if (menuItem)
    {
        if (menuItem->Hint == "View")
        {
            ViewImage(m_pBufferBmp);
            EndCapture();
        }
        else if (menuItem->Hint == "Copy")
        {
            CopyToClipboard();
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
        }
        else if (menuItem->Hint == "Print")
        {
            Print();
            EndCapture();
        }
        else if (menuItem->Hint == "PrintOn")
        {
            Print();
        }
        else if (menuItem->Hint == "AutoSave")
        {
            AutoSaveToFile();
            EndCapture();
        }
        else if (menuItem->Hint == "AutoSaveOn")
        {
            AutoSaveToFile();
        }
        else if (menuItem->Hint == "AutoSaveOptions")
        {
            AutosaveOptions();
        }
        else if (menuItem->Hint == "Hide")
        {
            m_pBufferBmp->Assign(NULL);
            EndCapture();
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TScreenGrabber::EndCapture()
{
    Close();
    // We are done
    if (FOnCaptureComplete)
        FOnCaptureComplete(this);
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
    NewItem->ShortCut = ShortCut(Word(0x0D), TShiftState());
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    // Presets Submenu ------------------
    TMenuItem* PresetMenu = new TMenuItem(m_CaptureMenu);
    PresetMenu->Caption = "Presets";
    m_CaptureMenu->Items->Add(PresetMenu);

        NewItem = new TMenuItem(PresetMenu);
        NewItem->Caption = "Add Preset...";
        NewItem->ShortCut = ShortCut(Word('0'), TShiftState() << ssAlt);
        NewItem->OnClick = PresetMenuClick;
        NewItem->Tag = 0;
        PresetMenu->Add(NewItem);

        // Separator ------------------------
        NewItem = new TMenuItem(PresetMenu);
        NewItem->Caption = "-";
        PresetMenu->Add(NewItem);

        for (size_t i = 0; i < m_PresetList.size(); i++)
        {
            NewItem = new TMenuItem(PresetMenu);
            NewItem->Caption = m_PresetList[i].description;
            NewItem->OnClick = PresetMenuClick;
            NewItem->Tag = i + 1;
            if (i < 9)
            {
                NewItem->ShortCut = ShortCut(Word(49 + i), TShiftState() << ssAlt);
            }
            PresetMenu->Add(NewItem);
        }

        // Separator ------------------------
        NewItem = new TMenuItem(PresetMenu);
        NewItem->Caption = "-";
        PresetMenu->Add(NewItem);

        NewItem = new TMenuItem(PresetMenu);
        NewItem->Caption = "Manage Presets...";
        NewItem->OnClick = ManagePresets;
        PresetMenu->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Copy To Clipboard";
    NewItem->Hint = "Copy";
    NewItem->ShortCut = ShortCut(Word('C'), TShiftState());
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Save To File...";
    NewItem->Hint = "Save";
    NewItem->ShortCut = ShortCut(Word('S'), TShiftState());
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Save && Grab More...";
    NewItem->Hint = "SaveOn";
    NewItem->ShortCut = ShortCut(Word('S'), TShiftState() << ssShift);
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Print...";
    NewItem->Hint = "Print";
    NewItem->ShortCut = ShortCut(Word('P'), TShiftState());
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Print && Grab More...";
    NewItem->Hint = "PrintOn";
    NewItem->ShortCut = ShortCut(Word('P'), TShiftState() << ssShift);
    m_CaptureMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->Caption = "-";
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Auto Save";
    NewItem->Hint = "AutoSave";
    NewItem->ShortCut = ShortCut(Word('A'), TShiftState());
    //NewItem->Enabled = m_AutoSaver.Enabled;
    m_CaptureMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_CaptureMenu);
    NewItem->OnClick = CaptureMenuClick;
    NewItem->Caption = "Auto Save && Grab More";
    NewItem->Hint = "AutoSaveOn";
    NewItem->ShortCut = ShortCut(Word('A'), TShiftState() << ssShift);
    //NewItem->Enabled = m_AutoSaver.Enabled;
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
    NewItem->Hint = "Hide";
    NewItem->ShortCut = ShortCut(Word(0x1B), TShiftState());
    m_CaptureMenu->Items->Add(NewItem);

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

