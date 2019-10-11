//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//#include <vcl\Clipbrd.hpp>
#include "Info.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Tool"
#pragma resource "*.dfm"

#include "InfoFormatter.h"

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
    TPixelInfo pi(ptCurMouse.x, ptCurMouse.y);

    // Update the color info labels
    Red->Caption = pi.r;
    Green->Caption = pi.g;
    Blue->Caption = pi.b;

    // Update HSV info
    Hue->Caption = IntToStr(pi.h) + "°";
    Sat->Caption = IntToStr(pi.s) + "%";
    Val->Caption = IntToStr(pi.v) + "%";

    // Update webcolor info label
    InfoFormatter inf("# [R] [G] [B]");
    RGB->Caption = inf.GetFormattedString(pi).c_str();

    // Update the ColorPanel
    ColorPanel->Color = (TColor) pi.color;
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
void __fastcall TInfoForm::FormMouseDown(TObject *Sender, TMouseButton Button,
                                         TShiftState Shift, int X, int Y)
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
void __fastcall TInfoForm::FormMouseMove(TObject *Sender, TShiftState Shift,
                                         int X, int Y)
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
void __fastcall TInfoForm::FormMouseUp(TObject *Sender, TMouseButton Button,
                                       TShiftState Shift, int X, int Y)
{
    if (m_bDragging && Button == mbLeft)
    {
        m_bDragging = false;
        Screen->Cursor = TCursor(crDefault);
        ::ReleaseCapture();
    }
}

//---------------------------------------------------------------------------
void __fastcall TInfoForm::miOptionsClick(TObject *Sender)
{
    if (FOnOptions)
        FOnOptions(this);
}

//---------------------------------------------------------------------------

