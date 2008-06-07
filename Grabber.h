//---------------------------------------------------------------------------
#ifndef GrabberH
#define GrabberH
//---------------------------------------------------------------------------
#include <ExtDlgs.hpp>
#include "ScreenForm.h"
#include "AutoSaveOptions.h"
#include "ImageView.h"
#include <vector>


//---------------------------------------------------------------------------
class TScreenGrabber : public TScreenForm
{
public:
  __fastcall TScreenGrabber(TComponent* Owner);
  __fastcall ~TScreenGrabber();
  void __fastcall UpdateSettings();

private:
  RECT m_rcSelect;

  enum TGrabberMode { gmOpenViewer, gmShowMenu, gmBypassMenu, gmContinuous };

  TGrabberMode m_GrabberMode;
  Graphics::TBitmap* m_pBufferBmp;
  TPopupMenu* m_CaptureMenu;
  TAutoSaveOptions m_AutoSaveOptions;
  TImageViewerList m_Viewers;

  void __fastcall ViewerKeyPress(TObject *Sender, char &Key);
  void __fastcall ViewerClosed(TObject *Sender, TCloseAction &Action);

  //bool __fastcall DisplayIsPaletted();

//  void __fastcall DrawSelectRect(const TRect& rect);
  void __fastcall GetDesktopArea();
  void __fastcall GetDesktopArea(LPRECT lpRect);

  void __fastcall PopulateCaptureMenu();
  void __fastcall ShowCaptureMenu(int X, int Y);
  void __fastcall CaptureMenuClick(TObject *Sender);

  void __fastcall HandleCapture(int X, int Y);
  void __fastcall ViewImage(Graphics::TBitmap* pBufferBmp);
  void __fastcall AutosaveOptions();
  void __fastcall SaveToFile();
  void __fastcall Print();
  void __fastcall AutoSaveToFile();
//   void __fastcall DoSaveToFile(const String& PathName);
   void __fastcall CopyToClipboard();
  void __fastcall CaptureNext();
  void __fastcall EndCapture();
  void __fastcall HandleRightButtonClick(TObject *Sender, TMouseButton Button,
                                         TShiftState Shift, int X, int Y);

protected:
  TNotifyEvent FOnCaptureNext;
  TNotifyEvent FOnCaptureComplete;

__published:
  __property TNotifyEvent OnCaptureNext = { read = FOnCaptureNext, write = FOnCaptureNext };
  __property TNotifyEvent OnCaptureComplete = { read = FOnCaptureComplete, write = FOnCaptureComplete };
};

/////////////////////////////////////////////////////////////////////////////
class TNotRect
{
public:
  TNotRect(int X1, int Y1, int X2, int Y2)
  {
    bVisible = false;
    left   = X1;
    top    = Y1;
    right  = X2;
    bottom = Y2;
  }

  TNotRect(const TRect& rect)
  {
    bVisible = false;
    left   = rect.left;
    top    = rect.top;
    right  = rect.right;
    bottom = rect.bottom;
  }

  void Hide(HDC hdc)
  {
    if (hdc && bVisible)
    {
      Invert(hdc);
      bVisible = false;
    }
  }

  void Show(HDC hdc)
  {
    if (hdc && !bVisible)
    {
      Invert(hdc);
      bVisible = true;
    }
  }

  void Move(HDC hdc, int x1, int y1, int x2, int y2)
  {
    if (hdc && bVisible)
    {
      Invert(hdc);  // Remove previous rectangle
      left   = x1;
      top    = y1;
      right  = x2;
      bottom = y2;
      Invert(hdc);  // Draw new rectangle
    }
  }

  void Move(HDC hdc, const TRect& rect)
  {
    if (hdc && bVisible)
    {
      Invert(hdc);  // Remove previous rectangle
      left   = rect.left;
      top    = rect.top;
      right  = rect.right;
      bottom = rect.bottom;
      Invert(hdc);  // Draw new rectangle
    }
  }

private:
  bool bVisible;
  int left, top, right, bottom;

  void Invert(HDC hdc)
  {
    LOGBRUSH lb;
    lb.lbStyle = BS_SOLID;
    lb.lbColor = RGB(0, 0, 0);
    lb.lbHatch = 0;

    HPEN hPen = ExtCreatePen(PS_COSMETIC | PS_DOT, 1, &lb, 0, NULL);
    HPEN hPenOld = SelectObject(hdc, hPen);
    int ropOld = SetROP2(hdc, R2_NOT);

    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, right, top);
    LineTo(hdc, right, bottom);
    LineTo(hdc, left,  bottom);
    LineTo(hdc, left,  top);

    SetROP2(hdc, ropOld);
    SelectObject(hdc, hPenOld);
    DeleteObject(hPen);
  }
};

//---------------------------------------------------------------------------
#endif
