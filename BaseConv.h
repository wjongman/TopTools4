//---------------------------------------------------------------------------
#ifndef BaseConvH
#define BaseConvH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include "Tool.h"
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TBaseConvForm : public TToolForm
{
__published:  // IDE-managed Components
  TMenuItem *miBinary;
  TMenuItem *miCopy;
  TMenuItem *miExit;
  TMenuItem *miHide;
  TMenuItem *miPaste;
  TMenuItem *N1;
  TMenuItem *N2;
  TMenuItem *N3;
  TPopupMenu *BaseconvMenu;
  TPanel *plTop;
  TLabel *Label1;
  TEdit *AscEdit;
  TLabel *Label2;
  TEdit *HexEdit;
  TLabel *Label3;
  TEdit *DecEdit;
  TPanel *plBinary;
  TLabel *Label4;
  TEdit *BinEdit;
    TMenuItem *miOptions;

  void __fastcall AscEditKeyPress(TObject *Sender, char &Key);
  void __fastcall HexEditKeyPress(TObject *Sender, char &Key);
  void __fastcall DecEditKeyPress(TObject *Sender, char &Key);
  void __fastcall BinEditKeyPress(TObject *Sender, char &Key);
  void __fastcall HexEditChange(TObject *Sender);
  void __fastcall DecEditChange(TObject *Sender);
  void __fastcall BinEditChange(TObject *Sender);
  void __fastcall EditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall BinEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall EditContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);

  void __fastcall miExitClick(TObject *Sender);
  void __fastcall miCopyClick(TObject *Sender);
  void __fastcall miPasteClick(TObject *Sender);
  void __fastcall miBinaryClick(TObject *Sender);
  void __fastcall miHideClick(TObject *Sender);
    void __fastcall miOptionsClick(TObject *Sender);

protected:
  virtual void __fastcall LoadSettings();
  virtual void __fastcall SaveSettings();
  void ShowBinaryField(bool show);

private:
  void __fastcall UpdateAsc(int value);
  void __fastcall UpdateHex(int value);
  void __fastcall UpdateDec(int value);
  void __fastcall UpdateBin(int value);

  const String __fastcall UnDelimitBin(const String &sDelim);
  const String __fastcall DelimitBin(const String &sUnDelim);

  bool bUpdating;
  bool bSkipDelimit;
  unsigned int  iValue;

  String m_ClipboardContent;
  TEdit* m_PasteTarget;
  TEdit* m_CopyTarget;
  String m_CopyText;

  bool IsValidAscii(const String& Values);
  bool IsValidHex(const String& Values);
  bool IsValidDecimal(const String& Values);
  bool IsValidBinary(const String& Values);

public:
  __fastcall TBaseConvForm(TComponent* Owner);
  __fastcall ~TBaseConvForm();

  void __fastcall UpdateSettings() { LoadSettings(); }
};
//---------------------------------------------------------------------------
#endif
