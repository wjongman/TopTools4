//----------------------------------------------------------------------------
#ifndef HotkeyDialogH
#define HotkeyDialogH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//----------------------------------------------------------------------------
class THotkeyDlg : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
  TComboBox *cbHotkeyAction;
  TMemo *memoExplainHotkeys;
  TLabel *lbAction;
  TLabel *lbHotkey;
  TEdit *edHotkey;
private:
public:
	virtual __fastcall THotkeyDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE THotkeyDlg *HotkeyDlg;
//----------------------------------------------------------------------------
#endif    
