//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "About.h"
#include <PNGImage.hpp>

//---------------------------------------------------------------------
#pragma resource "*.dfm"

// Global string to automaticly update build date
//extern String g_sBuildDate;

//---------------------------------------------------------------------------

const char szHomepageUrl[] = "https://toptools.org";

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

//---------------------------------------------------------------------------
void __fastcall TAboutBox::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if (Key == VK_ESCAPE)
        Close();
}

//---------------------------------------------------------------------
void __fastcall TAboutBox::FormShow(TObject *Sender)
{
//   lbVersion->Caption = "Version: " + GetVersionString() + "  (" + g_sBuildDate + ")";
   lbCopy->Caption = GetCopyrightString();
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
String __fastcall TAboutBox::GetCopyrightString(void)
{
  return VersionInfo("LegalCopyright");
  //return "© 1998-2019 Willem Jongman";
}

//---------------------------------------------------------------------------
void __fastcall TAboutBox::FormPaint(TObject *Sender)
{
  // Draw a black border around our form
  Canvas->Brush->Color = clBlack;
  Canvas->FrameRect(ClientRect);
}

//---------------------------------------------------------------------------
String __fastcall TAboutBox::VersionInfo(const String& sQuery)
{
    DWORD c;
    DWORD dw = 0;
    UINT ui;
    char *p;
    LPVOID ptr;
    String sOut = ParamStr(0); ///ParamStr(0) holds exe name

    c = GetFileVersionInfoSize(ParamStr(0).c_str(), &dw);
    p = new char[c + 1]; //file://create the space
    GetFileVersionInfo(ParamStr(0).c_str(), 0, c, p);//get the version info data

/// Extract the language/translation information...
    VerQueryValue(p, TEXT("\\VarFileInfo\\Translation"), &ptr, &ui);

/// ptr comes back as a ptr to two (16-bit) words containing the two halves of
/// the translation number required for StringFileInfo
    WORD *id = (WORD*)ptr;
    String sBase = String( "\\StringFileInfo\\") +
                   IntToHex(id[0], 4) +
                   IntToHex(id[1], 4) +
                   "\\";
    String qs = sBase + sQuery ; // query string
    VerQueryValue(p, qs.c_str(), &ptr, &ui);
    sOut = (char*)ptr;
    delete [] p;
    return sOut;
}


