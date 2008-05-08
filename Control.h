//---------------------------------------------------------------------------

#ifndef ControlH
#define ControlH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <Buttons.hpp>

#include "Tool.h"
#include "TopToolBar.h"

//---------------------------------------------------------------------------
class TControlForm : public TToolForm
{
__published:	// IDE-managed Components
public:		// User declarations
  __fastcall TControlForm(TComponent* Owner);

  void __fastcall UpdateFocus();
//  void __fastcall TimerEvent(const TPoint& ptMouse)
//                  { m_toolbar->FlattenButtons(ptMouse); }
protected:
  virtual void __fastcall OnGetFocus();
  virtual void __fastcall OnLoseFocus();

  // This tool has no settings to be saved
  virtual void __fastcall LoadSettings() {};
  virtual void __fastcall SaveSettings() {};

  // Overridden to show up near the tray area
  virtual void __fastcall SetDefaultPosition();

private:	// User declarations
  TTopToolBar* m_toolbar;

};
//---------------------------------------------------------------------------
#endif
