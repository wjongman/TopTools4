//---------------------------------------------------------------------------

#ifndef CustomCopyDlgH
#define CustomCopyDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TCustomCopyDlg : public TForm
{
__published:  // IDE-managed Components
    TPanel *ButtonPanel;
    TButton *Button1;
    TButton *Button2;
    TButton *Button3;
    TButton *Button4;
    TButton *Button5;
    TButton *Button6;
    TButton *Button7;
    TButton *Button8;
    TButton *Button9;
    TButton *Button10;
    TButton *Button11;
    TButton *Button12;
    TButton *Button13;
    TButton *Button14;
    TButton *Button15;
    TPanel *HelpPanel;
    TMemo *HelpMemo;
    TPanel *EditPanel;
    TEdit *edMask;
    TEdit *edPreview;
    TLabel *Label1;
    TLabel *Label2;
    TButton *Button16;
    TPanel *OkCancelPanel;
    TButton *bnCancel;
    TButton *bnOK;
    TCheckBox *ckHelp;
    void __fastcall bnOKClick(TObject *Sender);
    void __fastcall bnCancelClick(TObject *Sender);
    void __fastcall AddPlaceholder(TObject *Sender);
    void __fastcall edMaskChange(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ckHelpClick(TObject *Sender);
private:  // User declarations
    void __fastcall RenderPreview();
public:   // User declarations
    __fastcall TCustomCopyDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCustomCopyDlg *CustomCopyDlg;
//---------------------------------------------------------------------------
#endif
