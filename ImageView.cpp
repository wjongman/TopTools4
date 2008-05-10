//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ImageView.h"
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
//    Image->OnMouseDown = FormMouseDown;
}

//---------------------------------------------------------------------------
__fastcall TImageViewer::~TImageViewer()
{
    if (m_ViewerMenu)
        delete m_ViewerMenu;
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
        if (menuItem->Hint == "Save")
        {
//      SaveToFile();
//      EndCapture();
        }
        else if (menuItem->Hint == "Copy")
        {
//      CopyToClipboard();
        }
        else if (menuItem->Hint == "Print")
        {
            PrintImage(Image->Picture->Bitmap);
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
    NewItem->Caption = "Close Viewer";
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

#if 0
//---------------------------------------------------------------------------
void __fastcall TImageViewer::DoSaveToFile(const String& PathName)
{
    String sFileName = PathName;
    if (DisplayIsPaletted())
    {
        // On paletted displays we only support Windows .bmp bitmaps
        sFileName = ChangeFileExt(sFileName, ".bmp");
        m_pBufferBmp->SaveToFile(sFileName);
    }
    else
    {
        String extension = ExtractFileExt(sFileName).LowerCase();
        if (extension == "")
        {
            // If no extension is present, we use
            // the most recently selected image type
            switch (m_CaptureOptions.FilterIndex)
            {
            case 1:
                extension = ".bmp";
                break;
            case 2:
                extension = ".png";
                break;
            case 3:
                extension = ".gif";
                break;
            case 4:
                extension = ".jpg";
                break;

            default:
                extension = ".bmp";
            }
            sFileName += extension;
        }

        if (extension == ".bmp")
        {
            __try
            {
                m_pBufferBmp->SaveToFile(sFileName);
            }
            catch (const Exception &E)
            {
                ShowMessage(String(E.Message));
            }
        }
        else if (extension == ".jpg")
        {
            TJPEGImage* Image = new TJPEGImage();
            Image->Assign(m_pBufferBmp);
            __try
            {
                Image->SaveToFile(sFileName);
            }
            catch (const Exception &E)
            {
                ShowMessage(String(E.Message));
            }
            delete Image;
        }
        else if (extension == ".gif")
        {
            TGIFImage* Image = new TGIFImage();
            Image->ColorReduction = rmQuantizeWindows;
            Image->Assign(m_pBufferBmp);
            __try
            {
                Image->SaveToFile(sFileName);
            }
            catch (const Exception &E)
            {
                ShowMessage(String(E.Message));
            }
            delete Image;
        }
        else if (extension == ".png")
        {
            TPNGObject* Image = new TPNGObject();
            Image->Assign(m_pBufferBmp);
            __try
            {
                Image->SaveToFile(sFileName);
            }
            catch (const Exception &E)
            {
                ShowMessage(String(E.Message));
            }
            delete Image;
        }
        else
        {
            // We have an unsupported extension here, should we change
            // the extension to reflect the filter-index settings?
            // Or should we popup a messagebox to notify the user?
            // For now we just do nothing...
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::SaveToFile()
{
    TRACE("TScreenGrabber::SaveToFile()");

    TSavePictureDialog *SavePicDlg = new TSavePictureDialog(this);
    SavePicDlg->Options << ofOverwritePrompt << ofEnableSizing;
    SavePicDlg->InitialDir = m_CaptureOptions.LastDir;

    bool haspalette = DisplayIsPaletted();
    if (haspalette)
    {
        SavePicDlg->FilterIndex = 1;
        SavePicDlg->Filter = "Windows Bitmap (*.bmp)|*.bmp";
    }
    else
    {
        SavePicDlg->FilterIndex = m_CaptureOptions.FilterIndex;
        SavePicDlg->Filter = "Windows Bitmap (*.bmp)|*.bmp|"
                             "PNG Image (*.png)|*.png|"
                             "GIF Image (*.gif)|*.gif|"
                             "JPEG Image (*.jpg)|*.jpg";
    }
    // Display the Save File Dialog
    if (SavePicDlg->Execute())
    {
        m_CaptureOptions.LastDir = ExtractFilePath(SavePicDlg->FileName);
        m_CaptureOptions.FilterIndex = SavePicDlg->FilterIndex;
        m_CaptureOptions.Save();

        DoSaveToFile(SavePicDlg->FileName);
    }

    delete SavePicDlg;
    m_pBufferBmp->Assign(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::CopyToClipboard()
{
    TRACE("TScreenGrabber::CopyToClipboard()");

    Clipboard()->Assign(m_pBufferBmp);
    m_pBufferBmp->Assign(NULL);
}
#endif
//---------------------------------------------------------------------------
void __fastcall TImageViewer::PrintImage(Graphics::TBitmap* pBitmap)
{
//    if ( PrintDialog1->Execute() )
    {
        //Graphics::TBitmap* pBitmap = Image->Picture->Bitmap;
        Printer()->BeginDoc();

        double fPrinterVert = (double) GetDeviceCaps(Printer()->Canvas->Handle, LOGPIXELSY);
        double fPrinterHorz = (double) GetDeviceCaps(Printer()->Canvas->Handle, LOGPIXELSX);
        double fScreenVert  = (double) GetDeviceCaps(Canvas->Handle, LOGPIXELSY);
        double fScreenHorz  = (double) GetDeviceCaps(Canvas->Handle, LOGPIXELSX);

        int iHeight = (int) ((double)pBitmap->Height * (fPrinterVert / fScreenVert));
        int iWidth  = (int) ((double)pBitmap->Width  * (fPrinterHorz / fScreenHorz));

//        StretchBltBitmap(Printer()->Canvas, 0, 0, iWidth, iHeight, pBitmap);
        StretchToPrintCanvas(Printer()->Canvas, 0, 0, iWidth, iHeight, pBitmap);

        Printer()->EndDoc();
    }
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::StretchToPrintCanvas(TCanvas *pPrintCanvas, int iX, int iY,
        int iWidth, int iHeight,
        Graphics::TBitmap *pBitmap)
{
    /*
    Here's the core routine:

          // Change bitmap info so we're Bottom-up (safer)
          with ABitmap.BitmapInfo.bmiHeader do
            biHeight := -ABitmap.Height;

          SetStretchBltMode(Canvas.Handle, HALFTONE);
          StretchDIBits(Canvas.Handle,
            ALeft, ATop, AWidth, AHeight,
            0, 0, ABitmap.Width, ABitmap.Height,
            ABitmap.Bits,
            ABitmap.BitmapInfo,
            DIB_RGB_COLORS,
            SRCCOPY);
    */
    unsigned int HeaderSize = 0;
    unsigned int ImageSize = 0;
    GetDIBSizes(pBitmap->Handle, HeaderSize, ImageSize);

    std::vector<unsigned char> buffer;
    buffer.reserve(HeaderSize + ImageSize);

    BITMAPINFO* pBmi = (BITMAPINFO*) & buffer[0];
    unsigned char* pImage = (unsigned char*) & buffer[HeaderSize];

    if (GetDIB(pBitmap->Handle, pBitmap->Palette, pBmi, pImage))
    {
        // Change bitmap info so we're Bottom-up (safer)
        //pBmi->bmiHeader.biHeight = -pBitmap->Height;
        //pBmi->bmiHeader.biWidth = -pBitmap->Width;
        // hmm, not sure, it prints a mirror copy..
        SetStretchBltMode(pPrintCanvas->Handle, HALFTONE);
        StretchDIBits(pPrintCanvas->Handle,
                      iX, iY, iWidth, iHeight,
                      0, 0, pBitmap->Width, pBitmap->Height,
                      pImage, pBmi, DIB_RGB_COLORS, SRCCOPY);
    }
    else
    {
        ShowMessage("Printing failed!\nError copying the DIB.");
    }
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::StretchBltBitmap(TCanvas *pCanvas, int iX, int iY,
        int iWidth, int iHeight,
        Graphics::TBitmap *pBitmap)
{
    char* Buffer = NULL;
    unsigned int HeaderSize = 0, ImageSize = 0;
    GetDIBSizes(pBitmap->Handle, HeaderSize, ImageSize);
    try
    {
        Buffer = new char[HeaderSize + ImageSize];
    }
    catch (Exception& E)
    {
        MessageDlg("Error allocating memory for DIB : " + E.Message,
                   mtError, TMsgDlgButtons() << mbOK, 0);
        return;
    }

    BITMAPINFO* pBmi = (BITMAPINFO*) Buffer;
    unsigned char* pImage = (unsigned char*) Buffer + HeaderSize;
    if (GetDIB(pBitmap->Handle, pBitmap->Palette, pBmi, pImage))
    {
        SetStretchBltMode(pCanvas->Handle, HALFTONE);
        StretchDIBits(pCanvas->Handle,
                      iX, iY, iWidth, iHeight,
                      0, 0, pBitmap->Width, pBitmap->Height,
                      pImage, pBmi, DIB_RGB_COLORS, SRCCOPY);
    }
    else
    {
        MessageDlg("Error copying the DIB.", mtError,
                   TMsgDlgButtons() << mbOK, 0);
    }
    delete[] Buffer;
}

//---------------------------------------------------------------------------

