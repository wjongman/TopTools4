//---------------------------------------------------------------------------

#ifndef ToolOptionsH
#define ToolOptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TTopToolsOptions : public TForm
{
//  enum {}
__published:  // IDE-managed Components
  TButton *bnCancel;
  TButton *bnHotkeyCaptureStart;
  TButton *bnHotkeyColorCopy;
  TButton *bnHotkeyRulerToggle;
  TButton *bnHotkeyZoomIn;
  TButton *bnHotkeyZoomOut;
  TButton *bnOk;
  TCheckBox *ckAutosave;
  TCheckBox *ckBinary;
  TCheckBox *ckHotkeyColorCopy;
  TCheckBox *ckHotkeyDoubleclick;
  TCheckBox *ckHotkeyCaptureStart;
  TCheckBox *ckHotkeyRuler;
  TCheckBox *ckHotkeyZoomIn;
  TCheckBox *ckHotkeyZoomOut;
  TCheckBox *ckOnTop;
  TCheckBox *ckOpenBaseconv;
  TCheckBox *ckOpenInfo;
  TCheckBox *ckOpenLoupe;
  TCheckBox *ckOpenRuler;
  TCheckBox *ckOpenToolbar;
  TCheckBox *ckPrefix;
  TCheckBox *ckQuotes;
  TCheckBox *ckSaveToolstate;
  TCheckBox *ckStart;
  TComboBox *cbHotkeyAction;
  TEdit *edHotkey;
  TEdit *edLength;
  TGroupBox *gbAttributes;
  TGroupBox *gbAutosave;
  TGroupBox *gbBaseConv;
  TGroupBox *gbGeneral;
  TGroupBox *gbGrab;
  TGroupBox *gbHotkeys;
  TGroupBox *gbInfo;
  TGroupBox *gbLoupe;
  TGroupBox *gbRef;
  TGroupBox *gbRuler;
  TLabel *lbBaseFilename;
  TLabel *lbDirectory;
  TLabel *lbAction;
  TLabel *lbHotkey;
  TLabel *lbLength;
  TListView *lvSelectOption;
  TMemo *memoExplainHotkeys;
  TPanel *plBaseconv;
  TPanel *plGeneral;
  TPanel *plGrabber;
  TPanel *plHotkeys;
  TPanel *plInfo;
  TPanel *plLoupe;
  TPanel *plRef;
  TPanel *plRuler;
  TUpDown *udLength;
  void __fastcall FormActivate(TObject *Sender);
  void __fastcall lvSelectOptionChange(TObject *Sender, TListItem *Item,
          TItemChange Change);
  void __fastcall bnHotkeyClick(TObject *Sender);

private:  // User declarations
  void HideAll();
  void ActivatePage(const String sActive);
  void ActivatePage(int PageIndex);
 TPanel* PanelArray[];

  String ActivePage;
  int m_CurrentItem;

public:   // User declarations
  __fastcall TTopToolsOptions(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTopToolsOptions *TopToolsOptions;
//---------------------------------------------------------------------------
#endif
