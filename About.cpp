//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "About.h"
#include <PNGImage.hpp>

//---------------------------------------------------------------------
#pragma resource "*.dfm"

// Global string to automaticly update build date
String g_sBuildDate = String(__DATE__);

//---------------------------------------------------------------------------

const char szHomepageUrl[] = "http://toptools.org";

//---------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
: TForm(AOwner)
{
  lbUrl->Hint = szHomepageUrl;

  TPNGObject* PngImage = new TPNGObject();
  PngImage->LoadFromResourceName((int)HInstance, "PNG_LOGO");
  Logo->Picture->Assign(PngImage);
  delete PngImage;
}

//---------------------------------------------------------------------
__fastcall TAboutBox::~TAboutBox()
{
}

//---------------------------------------------------------------------
void __fastcall TAboutBox::UrlClick(TObject *Sender)
{
  ::ShellExecute(0, "open", szHomepageUrl, NULL, NULL, SW_SHOWNORMAL);
  Close();
}

//---------------------------------------------------------------------------
void __fastcall TAboutBox::CloseClick(TObject *Sender)
{
  Close();
}

//---------------------------------------------------------------------
void __fastcall TAboutBox::FormShow(TObject *Sender)
{
   lbVersion->Caption = "Version: " + GetVersionString() + "  (" + g_sBuildDate + ")";
}
//---------------------------------------------------------------------------

String __fastcall TAboutBox::GetVersionString(void)
{
   DWORD handle;
   DWORD size = GetFileVersionInfoSize(Application->ExeName.c_str(), &handle);
   if (size != 0)
   {
     void *buf = malloc(size);
     GetFileVersionInfo(Application->ExeName.c_str(), 0, size, buf);
     VS_FIXEDFILEINFO *ffip;
     unsigned ffisize;
     VerQueryValue( buf, "\\", (void**)&ffip, &ffisize);
     int Ver1 = ffip->dwFileVersionMS >> 16;
     int Ver2 = ffip->dwFileVersionMS & 0xFFFF;
     int Ver3 = ffip->dwFileVersionLS >> 16;
     int Ver4 = ffip->dwFileVersionLS & 0xFFFF;
     free(buf);
     return String(Ver1) + "." + String(Ver2) + "." + String(Ver3) + "." + String(Ver4);
   }
   else return "";
}

//---------------------------------------------------------------------------

void __fastcall TAboutBox::FormPaint(TObject *Sender)
{
  // Draw a black border around our form
  Canvas->Brush->Color = clBlack;
  Canvas->FrameRect(ClientRect);
}
//---------------------------------------------------------------------------



