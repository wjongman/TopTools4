//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ImageView.h"
#include "PersistOptions.h"
#include "PersistImage.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TImageViewer *ImageViewer;
//---------------------------------------------------------------------------
__fastcall TImageViewer::TImageViewer(TComponent* Owner, int id, const TRect& rcGrab)
        : TForm(Owner), FId(id), m_rcGrab(rcGrab)
{
    m_ViewerMenu = NULL;
    OnShow = FormShow;
    //OnClose = FormClose;
    KeyPreview = true;
}

//---------------------------------------------------------------------------
__fastcall TImageViewer::~TImageViewer()
{
    if (m_ViewerMenu)
    {
        delete m_ViewerMenu;
    }
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::SetBitmap(Graphics::TBitmap* pBitmap)
{
    if (pBitmap && pBitmap->Width > 0)
    {
        Image->Picture->Assign(pBitmap);
    }
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::ShowViewerMenu(int X, int Y)
{
    if (!m_ViewerMenu)
    {
        m_ViewerMenu = new TPopupMenu(this);
    }

    PopulateViewerMenu();

    POINT ptAbs = ClientToScreen(Point(X, Y));
    m_ViewerMenu->Popup(ptAbs.x, ptAbs.y);
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::ViewerMenuClick(TObject *Sender)
{
    TMenuItem* menuItem = dynamic_cast<TMenuItem*>(Sender);

    if (menuItem)
    {
        TPersistImage image(Image->Picture->Bitmap);

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
        else if (menuItem->Hint == "Close")
        {
            Close();
        }
    }
}

//-------------------------------------------------------------
void __fastcall TImageViewer::FormKeyPress(TObject *Sender, char &Key)
{
    if (Key == VK_ESCAPE)
    {
        // todo: find out why this crashes the app
        //Close();
    }
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::PopulateViewerMenu()
{
    // Start with an empty menu
    m_ViewerMenu->Items->Clear();

    // Populate the menu
    TMenuItem *NewItem;

    NewItem = new TMenuItem(m_ViewerMenu);
    NewItem->OnClick = ViewerMenuClick;
    NewItem->Caption = "Copy To Clipboard";
    NewItem->Hint = "Copy";
    m_ViewerMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_ViewerMenu);
    NewItem->OnClick = ViewerMenuClick;
    NewItem->Caption = "Save To File...";
    NewItem->Hint = "Save";
    m_ViewerMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_ViewerMenu);
    NewItem->OnClick = ViewerMenuClick;
    NewItem->Caption = "Print...";
    NewItem->Hint = "Print";
    m_ViewerMenu->Items->Add(NewItem);

    // Separator ------------------------
    NewItem = new TMenuItem(m_ViewerMenu);
    NewItem->Caption = "-";
    m_ViewerMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_ViewerMenu);
    NewItem->OnClick = ViewerMenuClick;
    NewItem->Caption = "Close Snapshot";
    NewItem->Hint = "Close";
    m_ViewerMenu->Items->Add(NewItem);
    /*#ifdef _DEBUG
      // Separator ------------------------
      NewItem = new TMenuItem(m_ViewerMenu);
      NewItem->Caption = "-";
      m_ViewerMenu->Items->Add(NewItem);

      NewItem = new TMenuItem(m_ViewerMenu);
      NewItem->OnClick = ViewerMenuClick;
      NewItem->Caption = "Auto Save Options...";
      NewItem->Hint = "AutoSaveOptions";
      NewItem->Enabled = m_CaptureOptions.AutoSave;
      m_ViewerMenu->Items->Add(NewItem);
    #endif*/
}


/*
void __fastcall TImageViewer::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if (FOnClose)
    FOnClose(this);
}
*/
//---------------------------------------------------------------------------
void __fastcall TImageViewer::ImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  // Start a drag-operation
  {
    m_MouseOldX = X;
    m_MouseOldY = Y;
  }
  else if (Button == mbRight)// && FOnRightButtonClick)
  // Signal right-button event
  {
    ShowViewerMenu(X, Y);
//    FOnRightButtonClick(this, Button, Shift, X, Y);
  }
}

//---------------------------------------------------------------------------

void __fastcall TImageViewer::ImageMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  if (Shift.Contains(ssLeft))
  // We are dragging, move the form
  {
    Left += X - m_MouseOldX;
    Top  += Y - m_MouseOldY;
  }
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::FormShow(TObject *Sender)
{
    Graphics::TBitmap* pBitmap = Image->Picture->Bitmap;
    if (pBitmap && pBitmap->Width > 0)
    {
        // Calculate position needed to show exactly above the
        // selected area, as if just a border was drawn.

        ClientWidth = pBitmap->Width;
        ClientHeight = pBitmap->Height;

        Left += m_rcGrab.left - ClientOrigin.x;
        Top += m_rcGrab.top - ClientOrigin.y;
    }
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::SaveToFile()
{
    TPersistImage image(Image->Picture->Bitmap);

    String InitialDir = g_ToolOptions.Get("capture", "lastsavedir", "%USERPROFILE%\\Desktop");
    int filterindex = g_ToolOptions.Get("capture", "lastimagetype", 2);

    image.SaveFileDialog(filterindex, InitialDir);

    g_ToolOptions.Set("capture", "lastsavedir", InitialDir);
    g_ToolOptions.Set("capture", "lastimagetype", filterindex);
}

//---------------------------------------------------------------------------

