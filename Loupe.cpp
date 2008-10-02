//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Loupe.h"
#include "PersistImage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Tool"
#pragma resource "*.dfm"

/////////////////////////////////////////////////////////////////////////////
__fastcall TLoupeForm::TLoupeForm(TComponent* Owner)
: TToolForm(Owner, "loupe"),
m_pLoupe(NULL),
m_pToolbarImageList(NULL)
{
    const int WindowSize = 200;
    ClientWidth = WindowSize;
    ClientHeight = WindowSize;

    m_pLoupe = new TLoupePanel(this);
    m_pLoupe->Parent = this;
    m_pLoupe->Align = alClient;

    Screen->Cursors[crDragHand] = LoadCursor((void*)HInstance, "DRAGHAND");

    LoadGraphics();

    SetSelfMagnify(m_bMagnifySelf);
}

//---------------------------------------------------------------------------
__fastcall TLoupeForm::~TLoupeForm()
{
    delete m_pLoupe;
    delete m_pToolbarImageList;
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormCreate(TObject *Sender)
{
    LoadSettings();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormDestroy(TObject *Sender)
{
    SaveSettings();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::LoadGraphics()
{
    // For some reason CBuilder (and Delphi for that matter) stores
    // images in the .dfm with the same color-depth as that of
    // the machine the dfm was last saved on.
    // To work around this we load all our bitmaps dynamically from resource.
    m_pToolbarImageList = new TImageList(this);
    Graphics::TBitmap* IconsBmp = new Graphics::TBitmap();
    IconsBmp->LoadFromResourceName((int)HInstance, "LOUPEICONS");
    m_pToolbarImageList->AddMasked(IconsBmp, clFuchsia);
    delete IconsBmp;

    LoupeToolBar->Images = m_pToolbarImageList;
    LoupeMenu->Images = m_pToolbarImageList;
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::WndProc(Messages::TMessage &Message)
{
    TToolForm::WndProc(Message);
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::SetSelfMagnify(bool magnify)
{
    // On Windows 9X systems, the only way to get rid of self-magnification
    // is to black-out the loupe-view when the mouse is above it.
    // Windows 2000 and up support layered (so called 'transparent') windows
    // and we can (ab)use that feature as a means to "hide from ourselves",
    // just by sitting on a layer with 0% transparancy.

    m_bMagnifySelf = magnify;

    if (m_bMagnifySelf)
    {
        // Tell the loupeview to magnify no matter where the mouse is
        m_pLoupe->MagnifySelf = true;
        // On Windows 2000 we also remove the layered attribute
        SetTransparency(false);
    }
    else
    {
        if (TransparencyIsSupported())
        {
            // On Windows 2000 and up we make the window layered
            SetTransparency(true);
            // So we want the loupeview to not block magnification
            m_pLoupe->MagnifySelf = true;
        }
        else
        {
            // On Windows 9X systems we tell the loupeview to
            // black out the diplay when the mouse is above it
            m_pLoupe->MagnifySelf = false;
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::Freeze()
{
//  m_pLoupe->ToggleFrozen();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::ToggleLock()
{
    m_pLoupe->ToggleLocked();

    if (m_pLoupe->Locked)
        m_pLoupe->Cursor = (TCursor) crDragHand;
    else
        m_pLoupe->Cursor = crDefault;
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::ToggleMagnifySelf()
{
    SetSelfMagnify(!m_bMagnifySelf);
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::UnLock()
{
    if (m_pLoupe->Locked)
    {
        bnLockPos->Down = false;
        m_pLoupe->ToggleLocked();
        m_pLoupe->Cursor = crDefault;
    }
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::UpdateUI()
{
    // Update the state of all UI elements
    Caption = "Loupe " + String(m_pLoupe->Zoom) + "x";

    miCrosshair->Checked = m_pLoupe->CrosshairVisible;
    bnCrosshair->Down = m_pLoupe->CrosshairVisible;
    miGrid->Checked = m_pLoupe->GridVisible;
    bnGrid->Down = m_pLoupe->GridVisible;
    miCenterbox->Checked = m_pLoupe->CenterboxVisible;
    bnCenterbox->Down = m_pLoupe->CenterboxVisible;
    miLockPos->Checked = m_pLoupe->Locked;
    bnLockPos->Down = m_pLoupe->Locked;
    miMagnifySelf->Checked = m_bMagnifySelf;

    // we need both 'lock position" and "freeze view"
//  miFreezeView->Caption = m_pLoupe->Frozen ? "Unfreeze View" : "Freeze View";

    bnZoomOut->Enabled = (m_pLoupe->Zoom > 1);
    miZoomOut->Enabled = (m_pLoupe->Zoom > 1);
    bnZoomIn->Enabled = (m_pLoupe->Zoom < 20);
    miZoomIn->Enabled = (m_pLoupe->Zoom < 20);

    // We don't want a grid at low magnification
//  bnGrid->Enabled = (m_pLoupe->Zoom > 2);
//  miGrid->Enabled = (m_pLoupe->Zoom > 2);
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::LoupeMenuPopup(TObject *Sender)
{
//    PopulatePersistMenu(miSaveView);
    UpdateUI();
}
/*
//---------------------------------------------------------------------------
void __fastcall TLoupeForm::PopulatePersistMenu(TMenuItem* PersistMenu)
{
    // Start with an empty menu
    PersistMenu->Items->Clear();

    // Populate the menu
    TMenuItem *NewItem;

    NewItem = new TMenuItem(PersistMenu);
    NewItem->OnClick = PersistMenuClick;
    NewItem->Caption = "Copy To Clipboard";
    NewItem->Hint = "Copy";
    PersistMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(PersistMenu);
    NewItem->OnClick = PersistMenuClick;
    NewItem->Caption = "Save To File...";
    NewItem->Hint = "Save";
    PersistMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(PersistMenu);
    NewItem->OnClick = PersistMenuClick;
    NewItem->Caption = "Print...";
    NewItem->Hint = "Print";
    PersistMenu->Items->Add(NewItem);
  }*/

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miSaveViewClick(TObject *Sender)
{
    TMenuItem* menuItem = dynamic_cast<TMenuItem*>(Sender);

    if (menuItem)
    {
        TPersistImage image(m_pLoupe->Bitmap);

        if (menuItem->Hint == "Save")
        {
            SaveToFile();
        }
        else if (menuItem->Hint == "Copy")
        {
            image.Copy();
        }
        else if (menuItem->Hint == "Print")
        {
            image.Print();
        }
    }

}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::SaveToFile()
{
    TPersistImage image(m_pLoupe->Bitmap);

    String InitialDir = g_ToolOptions.Get("loupe", "lastsavedir", "%USERPROFILE%\\Desktop");
    int filterindex = g_ToolOptions.Get("loupe", "lastimagetype", 2);

    image.SaveFileDialog(filterindex, InitialDir);

    g_ToolOptions.Set("loupe", "lastsavedir", InitialDir);
    g_ToolOptions.Set("loupe", "lastimagetype", filterindex);
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miZoomInClick(TObject *Sender)
{
    m_pLoupe->ZoomIn();
    UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miZoomOutClick(TObject *Sender)
{
    m_pLoupe->ZoomOut();
    UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miHideClick(TObject *Sender)
{
    Close();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miCrosshairClick(TObject *Sender)
{
    m_pLoupe->ToggleCrosshair();
    UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miGridClick(TObject *Sender)
{
    m_pLoupe->ToggleGrid();
    UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miCenterboxClick(TObject *Sender)
{
    m_pLoupe->ToggleCenterbox();
    UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miFreezeViewClick(TObject *Sender)
{
    Freeze();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miLockPosClick(TObject *Sender)
{
    ToggleLock();
    UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miMagnifySelfClick(TObject *Sender)
{
    ToggleMagnifySelf();
    UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miExitClick(TObject *Sender)
{
    Application->Terminate();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormMouseWheelUp(TObject *Sender,
                                             TShiftState Shift, TPoint &MousePos, bool &Handled)
{
    ZoomOut();
    Handled = true;
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormMouseWheelDown(TObject *Sender,
                                               TShiftState Shift, TPoint &MousePos, bool &Handled)
{
    ZoomIn();
    Handled = true;
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::LoadSettings()
{
    m_pLoupe->Zoom = g_ToolOptions.Get(m_ToolName, "zoom", 4);
    m_pLoupe->CrosshairVisible = g_ToolOptions.Get(m_ToolName, "crosshair", false);
    m_pLoupe->CenterboxVisible = g_ToolOptions.Get(m_ToolName, "centerbox", false);
    m_pLoupe->GridVisible = g_ToolOptions.Get(m_ToolName, "grid", false);
    m_bMagnifySelf = g_ToolOptions.Get(m_ToolName, "selfmagnify", false);
    m_pLoupe->RefreshRate = g_ToolOptions.Get(m_ToolName, "refresh", 250);
    Width = g_ToolOptions.Get(m_ToolName, "width", 200);
    Height = g_ToolOptions.Get(m_ToolName, "height", 200);

    UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::SaveSettings()
{
    g_ToolOptions.Set(m_ToolName, "zoom", m_pLoupe->Zoom);
    g_ToolOptions.Set(m_ToolName, "centerbox", m_pLoupe->CenterboxVisible);
    g_ToolOptions.Set(m_ToolName, "crosshair", m_pLoupe->CrosshairVisible);
    g_ToolOptions.Set(m_ToolName, "grid", m_pLoupe->GridVisible);
    g_ToolOptions.Set(m_ToolName, "refresh", m_pLoupe->RefreshRate);
    g_ToolOptions.Set(m_ToolName, "selfmagnify", m_bMagnifySelf);
    g_ToolOptions.Set(m_ToolName, "width", Width);
    g_ToolOptions.Set(m_ToolName, "height", Height);
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::UpdateSettings()
{
    m_pLoupe->RefreshRate = g_ToolOptions.Get(m_ToolName, "refresh", 250);
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormKeyDown(TObject *Sender, WORD &Key,
                                        TShiftState Shift)
{
    // The arrow-keys can be used to move the loupe focus,
    // if the loupe is locked we move the focuspoint,
    // else we move the mouse-cursor
    POINT ptMouse;
    GetCursorPos(&ptMouse);

    // Default change increment is 1
    int delta = 1;
    // Increment is 10 when shift is pressed
    if (Shift.Contains(ssShift))
        delta = 10;

    switch (Key)
    {
    case VK_LEFT:
        if (m_pLoupe->Locked)
            m_pLoupe->MoveLockPos(-delta, 0);
        else
            SetCursorPos(ptMouse.x - delta, ptMouse.y);
        break;
    case VK_RIGHT:
        if (m_pLoupe->Locked)
            m_pLoupe->MoveLockPos(delta, 0);
        else
            SetCursorPos(ptMouse.x + delta, ptMouse.y);
        break;
    case VK_UP:
        if (m_pLoupe->Locked)
            m_pLoupe->MoveLockPos(0, -delta);
        else
            SetCursorPos(ptMouse.x, ptMouse.y - delta);
        break;
    case VK_DOWN:
        if (m_pLoupe->Locked)
            m_pLoupe->MoveLockPos(0, delta);
        else
            SetCursorPos(ptMouse.x, ptMouse.y + delta);
        break;
    }
}

//---------------------------------------------------------------------------

