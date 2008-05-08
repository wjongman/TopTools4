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

private:
  String __fastcall GetVersionString(void);

public:
  virtual __fastcall TAboutBox(TComponent* AOwner);
  virtual __fastcall ~TAboutBox();
};
//----------------------------------------------------------------------------
#endif
