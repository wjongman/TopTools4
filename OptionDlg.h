//---------------------------------------------------------------------------

#ifndef OptionDlgH
#define OptionDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#include "HotkeyInfo.h"
#include "HotkeyPanel.h"

/////////////////////////////////////////////////////////////////////////////
class TToolOptionsDialog : public TForm
{
__published:  // IDE-managed Components
    TButton *bnCancel;
    TButton *bnOk;
    TCheckBox *ckBinary;
    TCheckBox *ckOnTop;
    TCheckBox *ckPrefix;
    TCheckBox *ckQuotes;
    TCheckBox *ckRememberSettings;
    TCheckBox *ckAutoStart;
    TEdit *edLength;
    TEdit *edRefresh;
    TGroupBox *gbAttributes;
    TGroupBox *gbBaseConv;
    TGroupBox *gbGeneral;
    TGroupBox *gbGrab;
    TGroupBox *gbInfo;
    TGroupBox *gbLoupe;
    TGroupBox *gbRef;
    TGroupBox *gbRuler;
    TLabel *lbRefreshrate;
    TLabel *lbMilliseconds;
    TLabel *lbLength;
    TListView *lvOptionSelector;
    TPanel *plBaseconv;
    TPanel *plGeneral;
    TPanel *plGrabber;
    TPanel *plInfo;
    TPanel *plLoupe;
    TPanel *plRef;
    TPanel *plRuler;
    TUpDown *udLength;
    TUpDown *udRefresh;
    THotkeyPanel *hkpZoomIn;
    THotkeyPanel *hkpZoomOut;
    THotkeyPanel *hkpColorCopy;
    THotkeyPanel *hkpGrabScreen;
    TLabel *lbPixels;
    TCheckBox *ckNudgeRuler;
    TGroupBox *gbAutoSave;
    TCheckBox *ckAutosave;
    TButton *bnAutosaveOptions;
    TPanel *plMarker;
    TCheckBox *cbTransparent;
    TLabel *lbTransparency;
    TEdit *edTransparency;
    TUpDown *udTransparency;
    TLabel *lbPercent;
    TCheckBox *ckShowLoupeOnGrab;
    TCheckBox *ckTrayApp;
    TPanel *plTrayicon;
    TGroupBox *gbTrayicon;
    TGroupBox *gbDoubleClick;
    TCheckBox *ckOpenRuler;
    TCheckBox *ckOpenInfo;
    TCheckBox *ckOpenLoupe;
    TCheckBox *ckOpenBaseconv;
    TCheckBox *ckOpenToolbar;
    TCheckBox *ckOptionsDlg;
    THotkeyPanel *hkpDoubleClick;
    TCheckBox *ckSingleton;
    TRadioButton *rbInifile;
    TRadioButton *rbRegistry;
    TCheckBox *ckRememberPos;

    void __fastcall FormShow(TObject *Sender);
    void __fastcall lvOptionSelectorChange(TObject *Sender, TListItem *Item, TItemChange Change);
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnAutosaveOptionsClick(TObject *Sender);
    void __fastcall ckRememberSettingsClick(TObject *Sender);

private:  // User declarations
    void HideAll();
    void ActivatePage(const String sActive);
    void ActivatePage(int PageIndex);

    String m_sActivePage;

    void InitOptions();
    void SaveOptions();

    void InitListView();
//   void InitOptions0();
//   void SaveOptions0();

    void InitHotkeyPanels();
    void SaveHotkeyPanels();

    THotkeyInfo LoadHotkeyInfo(const String& sHotKeyName);
    void SaveHotkeyInfo(const String& sHotKeyName, const THotkeyInfo& HotkeyInfo);

public:   // User declarations
    __fastcall TToolOptionsDialog(TComponent* Owner);
    __fastcall TToolOptionsDialog(TComponent* Owner, const String& PageName);
    __fastcall ~TToolOptionsDialog();
};
//---------------------------------------------------------------------------
extern PACKAGE TToolOptionsDialog *ToolOptionsDialog;
//---------------------------------------------------------------------------
#endif
