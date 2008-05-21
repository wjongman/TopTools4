//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <printers.hpp>
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
    if ( PrintDialog1->Execute() )
    {
        Printer()->BeginDoc();

        double fPrinterVert = (double) GetDeviceCaps(Printer()->Canvas->Handle, LOGPIXELSY);
        double fPrinterHorz = (double) GetDeviceCaps(Printer()->Canvas->Handle, LOGPIXELSX);
        double fScreenVert  = (double) GetDeviceCaps(Canvas->Handle, LOGPIXELSY);
        double fScreenHorz  = (double) GetDeviceCaps(Canvas->Handle, LOGPIXELSX);

        int iHeight = (int) ((double)pBitmap->Height * (fPrinterVert / fScreenVert));
        int iWidth  = (int) ((double)pBitmap->Width  * (fPrinterHorz / fScreenHorz));

        //DoPrintImage(Printer()->Canvas, 0, 0int pX,int pY, Graphics::TBitmap *pBitmap)
        StretchToPrintCanvas(Printer()->Canvas, 0, 0, iWidth, iHeight, pBitmap);

        Printer()->EndDoc();
    }
}

//---------------------------------------------------------------------------
void __fastcall TImageViewer::StretchToPrintCanvas(TCanvas *pPrintCanvas, int iX, int iY,
        int iWidth, int iHeight,
        Graphics::TBitmap *pBitmap)
{
    unsigned int HeaderSize = 0;
    unsigned int ImageSize = 0;
    GetDIBSizes(pBitmap->Handle, HeaderSize, ImageSize);

    std::vector<unsigned char> buffer;
    buffer.reserve(HeaderSize + ImageSize);

    BITMAPINFO* pBmi = (BITMAPINFO*) & buffer[0];
    unsigned char* pImage = (unsigned char*) & buffer[HeaderSize];

    if (GetDIB(pBitmap->Handle, pBitmap->Palette, pBmi, pImage))
    {
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
#if 1

/********************************************************************************************
*  NAME:
*           PrintImage
*  PURPOSE:
*           Print a bitmap with  StretchDIBits
*  NOTES:
*           The image must contain a Bitmap. This particular function will only
*            work if it is a Bitmap, icons and cursors will not work.
*
*  PARAMS:   PrinterCanvas : printer canvas (TPrinter->Canvas)
*          pX : bitmap X position on printer page
*          pY : bitmap Y position on printer page
*          pBitmap: TBitmap to be printed
*  RETURNS:
*               void
**********************************************************************************************/
//void __fastcall TImageViewer::PrintImage(Graphics::TBitmap* pBitmap)
void __fastcall TImageViewer::DoPrintImage(TCanvas *PrinterCanvas,int pX,int pY,
                                           Graphics::TBitmap *pBitmap)
{

    // create a memory dc for the image
    HDC h_dc = pBitmap->Canvas->Handle;
    int bmp_w = pBitmap->Width, bmp_h = pBitmap->Height;

    HDC h_mem_dc = ::CreateCompatibleDC (h_dc);
    HBITMAP h_mem_bmp = ::CreateCompatibleBitmap (h_dc, bmp_w, bmp_h);
    HBITMAP h_old_bmp = ::SelectObject (h_mem_dc, h_mem_bmp);

    // fix up bad video drivers
    bool is_pal_dev = false;
    LOGPALETTE *pal;
    HPALETTE h_pal, h_old_pal;

    if ( ::GetDeviceCaps (pBitmap->Canvas->Handle, RASTERCAPS) & RC_PALETTE)
    {
        pal = static_cast<LOGPALETTE*>(malloc (sizeof (LOGPALETTE) + (sizeof (PALETTEENTRY) * 256)));
        memset (pal, 0, sizeof (LOGPALETTE) + (sizeof (PALETTEENTRY) * 256));
        pal->palVersion = 0x300;
        pal->palNumEntries = ::GetSystemPaletteEntries(pBitmap->Canvas->Handle, 0, 256, pal->palPalEntry);
        if (pal->palNumEntries != 0)
        {
            h_pal = ::CreatePalette (pal);
            h_old_pal = ::SelectPalette (h_mem_dc, h_pal, false);
            is_pal_dev = true;
        }
        else
        {
            free (pal);
        }
    }

    // copy the image on to the memory dc
    ::BitBlt (h_mem_dc, 0, 0, bmp_w, bmp_h, h_dc, 0, 0, SRCCOPY);

    if (is_pal_dev)
    {
        ::SelectPalette (h_mem_dc, h_old_pal, false);
        ::DeleteObject (h_pal);
    }

    // delete the mem dc
    ::SelectObject (h_mem_dc, h_old_bmp);
    ::DeleteDC (h_mem_dc);

    // get memory for a BITMAPIFO Structure
    HANDLE h_bmp_info = ::GlobalAlloc (GHND, sizeof (BITMAPINFO) + (sizeof (RGBQUAD) * 256));
    BITMAPINFO* bmp_info = static_cast<BITMAPINFO*>(::GlobalLock (h_bmp_info));
    //Set up the structure

    memset (bmp_info, NULL, sizeof (BITMAPINFO) + (sizeof (RGBQUAD) * 255));
    bmp_info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp_info->bmiHeader.biPlanes = 1;
    bmp_info->bmiHeader.biBitCount = 16;//or 8
    bmp_info->bmiHeader.biWidth = bmp_w;
    bmp_info->bmiHeader.biHeight = bmp_h;
    bmp_info->bmiHeader.biCompression = BI_RGB;

    // find out how much memory for the bits
    ::GetDIBits (h_dc, h_mem_bmp, 0, bmp_h, NULL, bmp_info, DIB_RGB_COLORS);

    // Allocate memory for the bits
    HANDLE h_bits = GlobalAlloc (GHND, bmp_info->bmiHeader.biSizeImage);
    void *bits = ::GlobalLock (h_bits);

    // this time get the bits
    ::GetDIBits (h_dc, h_mem_bmp, 0, bmp_h, bits, bmp_info, DIB_RGB_COLORS);

    // fix up for bad video driver
    if (is_pal_dev)
    {
        for (int i = 0; i <pal->palNumEntries; i++)
        {
            bmp_info->bmiColors[i].rgbRed = pal->palPalEntry[i].peRed;
            bmp_info->bmiColors[i].rgbGreen = pal->palPalEntry[i].peGreen;
            bmp_info->bmiColors[i].rgbBlue = pal->palPalEntry[i].peBlue;
        }
        free (pal);
    }


    // fix up for print with palette
    is_pal_dev = false;
    if ( ::GetDeviceCaps (h_dc, RASTERCAPS) & RC_PALETTE)
    {
        pal = static_cast<LOGPALETTE*>(malloc (sizeof (LOGPALETTE) + (sizeof (PALETTEENTRY) * 256)));
        memset (pal, 0, sizeof (LOGPALETTE) + (sizeof (PALETTEENTRY) * 256));
        pal->palVersion = 0x300;
        pal->palNumEntries = 256;
        for (int i = 0; pal->palNumEntries; i++)
        {
            pal->palPalEntry[i].peRed = bmp_info->bmiColors[i].rgbRed;
            pal->palPalEntry[i].peGreen = bmp_info->bmiColors[i].rgbGreen;
            pal->palPalEntry[i].peBlue = bmp_info->bmiColors[i].rgbBlue;
        }
        h_pal = CreatePalette(pal);
        free (pal);
        h_old_pal = SelectPalette(PrinterCanvas->Handle, h_pal, false);
        is_pal_dev = true;
    }


    //  adjust bitmap dimensions for the printer device:
    int PrnXRes= GetDeviceCaps(PrinterCanvas->Handle, LOGPIXELSX); //Get dpi of printer along width
    int PrnYRes= GetDeviceCaps(PrinterCanvas->Handle, LOGPIXELSY); //Get dpi of printer along height
    bmp_w= floor(((double)pBitmap->Width / (double)Screen->PixelsPerInch) * (double)PrnXRes);
    bmp_h= floor(((double)pBitmap->Height / (double)Screen->PixelsPerInch) * (double)PrnYRes);

    // send the bits to the printer
    StretchDIBits(PrinterCanvas->Handle, pX,pY, bmp_w, bmp_h,
        0, 0, pBitmap->Width, pBitmap->Height, bits, bmp_info, DIB_RGB_COLORS, SRCCOPY);

    // clean up
    ::DeleteObject (h_mem_bmp);
    if (is_pal_dev)
    {
        ::SelectObject (PrinterCanvas->Handle, h_old_pal);
        ::DeleteObject (h_pal);
    }
    ::GlobalUnlock (bits);
    ::GlobalFree (h_bits);
    ::GlobalUnlock (bmp_info);
    ::GlobalFree (h_bmp_info);
}

#endif
