//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TestWindow.h"
//#include "OptionDlg.h"
//#include "AutoSaveDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// Global String for registry access
const String g_RegBaseKey("Software\\TopTools 3\\");

TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
}

//---------------------------------------------------------------------------
__fastcall TForm1::~TForm1()
{
}

//---------------------------------------------------------------------------
void __fastcall TForm1::bnAutosaveClick(TObject *Sender)
{
//  AutoSaveDialog->ShowModal();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::bnQuitClick(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::bnTopToolsClick(TObject *Sender)
{
//  
}
//---------------------------------------------------------------------------

