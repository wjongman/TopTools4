//----------------------------------------------------------------------------
#ifndef AutoSaveDlgH
#define AutoSaveDlgH
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
#include <ComCtrls.hpp>
#include "PathEllipsisLabel.h"

#include "AutoSaveOptions.h"
//----------------------------------------------------------------------------
class TAutoSaveDialog : public TForm
{
__published:
  TButton *bnBrowseDir;
  TButton *bnCancel;
  TButton *bnOk;
  TComboBox *cbType;
  TEdit *edDigits;
  TEdit *edDirectory;
  TEdit *edPrefix;
  TGroupBox *GroupBox;
  TLabel *lbPreview;
  TLabel *lbDigits;
  TLabel *lbDirectory;
  TLabel *lbPrefix;
  TLabel *lbType;
  TPanel *plPreview;
  TPathEllipsisLabel *elbPreview;
  TUpDown *udDigits;
  TLabel *lbOffset;
  TEdit *edOffset;
  TUpDown *udOffset;
  TGroupBox *gbWhenExists;
  TRadioButton *rbPrompt;
  TRadioButton *rbOverwrite;
  TRadioButton *rbRename;
  void __fastcall bnBrowseDirClick(TObject *Sender);
  void __fastcall InputChange(TObject *Sender);
  void __fastcall edDigitsChange(TObject *Sender);
  void __fastcall ckByPassClick(TObject *Sender);
  void __fastcall bnOkClick(TObject *Sender);
private:
  TAutoSave autosaver;
  bool Initialized;

  void LoadOptions();
  void RefreshOptions();
  void SaveOptions();

  static int BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

public:
  virtual __fastcall TAutoSaveDialog(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TAutoSaveDialog *AutoSaveDialog;
//----------------------------------------------------------------------------
#endif
