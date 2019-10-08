//----------------------------------------------------------------------------
#ifndef AboutH
#define AboutH
//----------------------------------------------------------------------------
#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>

// Global string to automaticly update build date
extern String g_sBuildDate;
//----------------------------------------------------------------------------
class TAboutBox : public TForm
{
__published:
  TLabel *lbVersion;
  TLabel *lbCopy;
  TLabel *lbUrl;
  TLabel *lbClose;
  TImage *Logo;
  void __fastcall UrlClick(TObject *Sender);
  void __fastcall CloseClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormPaint(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);

private:
  String __fastcall GetVersionString(void);
  String __fastcall GetCopyrightString(void);
  String __fastcall VersionInfo(const String& sQuery);

public:
  virtual __fastcall TAboutBox(TComponent* AOwner);
  virtual __fastcall ~TAboutBox();
};
//----------------------------------------------------------------------------
#endif
