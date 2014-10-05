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
    m_bStayOnTop = true;
    m_bAnnotate = false;
    m_ViewerMenu = NULL;
    m_pOriginalBitmap = NULL;
    OnShow = FormShow;
    //OnClose = FormClose;
    KeyPreview = true;
}

//---------------------------------------------------------------------------
__fastcall TImageViewer::~TImageViewer()
{
    delete m_ViewerMenu;
    delete m_pOriginalBitmap;
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::SetBitmap(Graphics::TBitmap* pBitmap)
{
    if (pBitmap && pBitmap->Width > 0)
    {
        if (!m_pOriginalBitmap)
        {
            m_pOriginalBitmap = new Graphics::TBitmap();
        }
        m_pOriginalBitmap->Assign(pBitmap);
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
        if (menuItem->Hint == "Save")
        {
            SaveToFile();
        }
        else if (menuItem->Hint == "Copy")
        {
            TPersistImage image(Image->Picture->Bitmap);
            image.Copy();
        }
        else if (menuItem->Hint == "Print")
        {
            TPersistImage image(Image->Picture->Bitmap);
            image.Print();
        }
        else if (menuItem->Hint == "Close")
        {
            Close();
        }
        else if (menuItem->Hint == "OnTop")
        {
            m_bStayOnTop = !m_bStayOnTop;
            SetTopMost(m_bStayOnTop);
            menuItem->Checked = m_bStayOnTop;
        }
        else if (menuItem->Hint == "Annotate")
        {
            m_bAnnotate = !m_bAnnotate;
            menuItem->Checked = m_bAnnotate;
            Image->Cursor = m_bAnnotate ? crCross : crSizeAll;
            Image->Picture->Assign(m_pOriginalBitmap);
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
    NewItem->Caption = "Annotate";
    NewItem->Hint = "Annotate";
    NewItem->Checked = m_bAnnotate;
    m_ViewerMenu->Items->Add(NewItem);

    NewItem = new TMenuItem(m_ViewerMenu);
    NewItem->OnClick = ViewerMenuClick;
    NewItem->Caption = "Stay on top";
    NewItem->Hint = "OnTop";
    NewItem->Checked = m_bStayOnTop;
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
void __fastcall TImageViewer::SetTopMost(bool ontop)
{
    if (ontop)
        SetWindowPos(Handle, HWND_TOPMOST, 0, 0, 0, 0,
                     SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
    else
        SetWindowPos(Handle, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

    m_bStayOnTop = ontop;
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::ImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  // Remember start position
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
  {
    if (m_bAnnotate)
    // We are drawing
    {
      Image->Picture->Bitmap->Canvas->Pen->Color = clRed;
      Image->Picture->Bitmap->Canvas->Pen->Width = 2;
      Image->Picture->Bitmap->Canvas->MoveTo(m_MouseOldX, m_MouseOldY);
      Image->Picture->Bitmap->Canvas->LineTo(X, Y);

      m_MouseOldX = X;
      m_MouseOldY = Y;
    }
    else
    // We are dragging, move the form
    {
      Left += X - m_MouseOldX;
      Top  += Y - m_MouseOldY;
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::FormShow(TObject *Sender)
{
    // Choose initial cursor shape
    Image->Cursor = m_bAnnotate ? crCross : crSizeAll;

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

