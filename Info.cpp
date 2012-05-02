//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <vcl\Clipbrd.hpp>
#include "Info.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Tool"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TInfoForm::TInfoForm(TComponent* Owner)
: TToolForm(Owner, "info")
{
  Screen->Cursors[crCrosshair] = LoadCursor((void*)HInstance, "CROSSHAIR");

  ClientHeight = XYPanel->Height + RGBPanel->Height + WebPanel->Height;
  Width = 160;
  m_bDragging = false;
}

//---------------------------------------------------------------------------
void __fastcall TInfoForm::TimerEvent(TPoint ptCurMouse)
{
  // Update the position info labels
  if (!m_bDragging)
  {
    XValue->Caption = ptCurMouse.x;
    YValue->Caption = ptCurMouse.y;
  }

  // Probe the color under the mouse
  HDC DesktopDC = GetDC(NULL);
  COLORREF Color = ::GetPixel(DesktopDC, ptCurMouse.x, ptCurMouse.y);
  ReleaseDC(NULL, DesktopDC);

  int red = GetRValue(Color);
  int green = GetGValue(Color);
  int blue = GetBValue(Color);

  // Update the color info labels
  Red->Caption = red;
  Green->Caption = green;
  Blue->Caption = blue;

  // Update webcolor info label
  char RGBtext[9];
  wsprintf(RGBtext, "# %02X %02X %02X", red, green, blue);
  RGB->Caption = RGBtext;

  // Update the webcolor string
  wsprintf(m_szWebColor, "%02X%02X%02X", red, green, blue);

  // Update HSV info
  UpdateHsv(red, green, blue);

  // Update the ColorPanel
  ColorPanel->Color = (TColor) Color;
}

//---------------------------------------------------------------------------
void __fastcall TInfoForm::UpdateHsv(int red, int green, int blue)
// In:   r, g and b in [0,255]
// Out:  h in [0,360]°, except when s == 0,
//       then h is undefined (some value not in [0,360])
//       s in [0,100]%
//       v in [0,100]%
{
  float h, s, v;

  // Map r, g and b to [0,1]
  float r = (float)red / 255;
  float g = (float)green / 255;
  float b = (float)blue / 255;
  float max = Maximum(r, g, b);
  float min = Minimum(r, g, b);

  v = max;                    // This settles the value of v


  // Calculate saturation
  if (max == 0)
    s = 0;                    // Saturation is 0 if r, g and b all are 0
  else
    s = (max - min)/max;

  if (s == 0)                 // Achromatic color, we are done
    h = -1;

  else                        // Cromatic color, calculate hue
  {
    float delta = max - min;

    if (r == max)
      h = (g - b)/delta;      // Resulting color is between yellow and magenta

    else if (g == max)
      h = 2 + (b - r)/delta;  // Resulting color is between cyan and yellow

    else if (b == max)
      h = 4 + (r - g)/delta;  // Resulting color is between magenta and cyan

    h *= 60;                  // Convert hue to degrees
    if (h < 0)
      h += 360;               // Make sure h is non-negative
  }

  char temp[5];
  wsprintf(temp, "%d°", (int)(h + 0.5));
  Hue->Caption = String(temp);
  wsprintf(temp, "%d%%", (int)((s * 100) + 0.5));
  Sat->Caption = String(temp);
  wsprintf(temp, "%d%%", (int)((v * 100) + 0.5));
  Val->Caption = String(temp);
}

//---------------------------------------------------------------------------
void __fastcall TInfoForm::miHideClick(TObject *Sender)
{
  Close();
}

//---------------------------------------------------------------------------
void __fastcall TInfoForm::miExitClick(TObject *Sender)
{
  Application->Terminate();
}

//---------------------------------------------------------------------------
void __fastcall TInfoForm::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  // Left-click and drag from the info-window displays mouse coordinates
  // relative to the window above which the mouse is hovering (instead of
  // relative to the top-left of the screen)
  if (Button == mbLeft)
  {
    ::SetCapture(Handle);
    Screen->Cursor = TCursor(crCrosshair);
    m_bDragging = true;
  }
}

//---------------------------------------------------------------------------
void __fastcall TInfoForm::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  if (m_bDragging)
  {
    // Get cursor-pos in screen coordinates
    POINT ptAbsMouse = TPoint(X, Y);
    ::ClientToScreen(Handle, &ptAbsMouse);

    // Get handle of window at that position
    HWND hTargetWnd = ::WindowFromPoint(ptAbsMouse);

    // Translate into target window coordinates
    POINT ptRelMouse = ptAbsMouse;
    ::ScreenToClient(hTargetWnd, &ptRelMouse);

    XValue->Caption = ptRelMouse.x;
    YValue->Caption = ptRelMouse.y;
  }
}

//---------------------------------------------------------------------------
void __fastcall TInfoForm::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (m_bDragging && Button == mbLeft)
  {
    m_bDragging = false;
    Screen->Cursor = TCursor(crDefault);
    ::ReleaseCapture();
  }
}
//---------------------------------------------------------------------------

