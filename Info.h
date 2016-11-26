//---------------------------------------------------------------------------

#ifndef InfoH
#define InfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Tool.h"
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TInfoForm : public TToolForm
{
__published:	// IDE-managed Components
  TPanel *RGBPanel;
  TLabel *Red;
  TLabel *RLabel;
  TLabel *GLabel;
  TLabel *Green;
  TLabel *BLabel;
  TLabel *Blue;
  TPanel *XYPanel;
  TLabel *YValue;
  TLabel *YLabel;
  TLabel *XValue;
  TLabel *XLabel;
  TPanel *WebPanel;
  TPanel *ColorPanel;
  TLabel *RGB;
  TLabel *Hue;
  TLabel *Sat;
  TLabel *Val;
  TLabel *VLabel;
  TLabel *SLabel;
  TLabel *HLabel;
  TPopupMenu *InfoPopupMenu;
  TMenuItem *miHide;
  TMenuItem *miColorDisplayFormat;
  TMenuItem *miExit;
  TMenuItem *N1;
  void __fastcall miHideClick(TObject *Sender);
  void __fastcall miExitClick(TObject *Sender);
  void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
  void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);

protected:
  // This tool has no settings to be saved
//  virtual void __fastcall LoadSettings() {};
//  virtual void __fastcall SaveSettings() {};

private:
  bool m_bDragging;

public:
  __fastcall TInfoForm(TComponent* Owner);
  void __fastcall TimerEvent(TPoint ptCurMouse);

};
//---------------------------------------------------------------------------
#endif
