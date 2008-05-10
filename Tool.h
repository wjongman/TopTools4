//---------------------------------------------------------------------------

#ifndef ToolH
#define ToolH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Registry.hpp>

#include "ToolOptions.h"
#include "Persist.h"

// Identifiers for our custom cursors
enum { crCrosshair,
       crHorUp,
       crHorDown,
       crVertLeft,
       crVertRight,
       crDragHand,
       crLastCursor };

enum TToolWindowStyle { twsTool, twsNormal };

/////////////////////////////////////////////////////////////////////////////
class TToolForm : public TForm
{
__published:

public:   // User declarations
  __fastcall TToolForm(TComponent* Owner, const String& toolname);
  __fastcall ~TToolForm();

private:  // User declarations
  void __fastcall CreateParams(TCreateParams &Params);
  void __fastcall ReadState(Classes::TReader* Reader);
  void __fastcall SetTopMost(bool topmost);
  void __fastcall SetWindowStyle(TToolWindowStyle style);

protected:
  String m_ToolName;
  TopTools::Options* m_pOptions;

  bool FSnapEdge;
  bool FDraggableForm;
  TToolWindowStyle FWindowStyle;
  TNotifyEvent FOnMinimize;

  bool __fastcall TransparencyIsSupported();
  void __fastcall SetTransparency(bool layered, int percent = 0);
  void __fastcall SetColorKey(bool layered, COLORREF colorkey);

  int __fastcall GetTaskbarRect(LPRECT lprect);


  virtual void __fastcall OnToolShow(TObject *Sender) { LoadPosition(); }
  virtual void __fastcall OnToolHide(TObject *Sender) { SavePosition(); }
  virtual void __fastcall LoadPosition();
  virtual void __fastcall SavePosition();
  virtual void __fastcall SetDefaultPosition();
  virtual void __fastcall ConstrainPosition();

  virtual void __fastcall OnGetFocus() {}
  virtual void __fastcall OnLoseFocus() {}

protected:
  virtual void __fastcall WndProc0(TMessage &Msg);
  virtual void __fastcall WndProc(TMessage &Msg);

__published:  // IDE-managed Components
  __property OnKeyDown;
  __property OnClose;
  __property bool SnapScreenEdges = { read = FSnapEdge, write = FSnapEdge };
  __property bool DraggableForm = { read = FDraggableForm, write = FDraggableForm };
  __property TToolWindowStyle WindowStyle = { read = FWindowStyle, write = SetWindowStyle };
  __property TNotifyEvent OnMinimize = { read = FOnMinimize, write = FOnMinimize };

};

//---------------------------------------------------------------------------
#endif
