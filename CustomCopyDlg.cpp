//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CustomCopyDlg.h"
#include "InfoFormatter.h"
#include "PersistOptions.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCustomCopyDlg *CustomCopyDlg;
//---------------------------------------------------------------------------
__fastcall TCustomCopyDlg::TCustomCopyDlg(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TCustomCopyDlg::FormShow(TObject *Sender)
{
    ckHelp->Checked = g_ToolOptions.Get("info", "showhelp", false);

    ClientHeight = ButtonPanel->Height + EditPanel->Height;
    if (ckHelp->Checked)
    {
        ClientHeight += HelpPanel->Height;
    }

    edMask->Text = g_ToolOptions.Get("info", "mask", "");
    edMask->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TCustomCopyDlg::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    g_ToolOptions.Set("info", "showhelp", ckHelp->Checked);
}

//---------------------------------------------------------------------------
void __fastcall TCustomCopyDlg::bnOKClick(TObject *Sender)
{
    g_ToolOptions.Set("info", "mask", edMask->Text);
    ModalResult = mrOk;
    Close();
}

//---------------------------------------------------------------------------
void __fastcall TCustomCopyDlg::bnCancelClick(TObject *Sender)
{
    ModalResult = mrCancel;
    Close();
}

//---------------------------------------------------------------------------
void __fastcall TCustomCopyDlg::edMaskChange(TObject *Sender)
{
    RenderPreview();
}

//---------------------------------------------------------------------------
String StripAmpersant(String unstripped)
{
    if (unstripped.Length() > 3)
        return unstripped.Delete(2, 1);

    return unstripped;
}

//---------------------------------------------------------------------------
void __fastcall TCustomCopyDlg::AddPlaceholder(TObject *Sender)
{
    TButton* button = reinterpret_cast<TButton*>(Sender);
    String insertstr = StripAmpersant(button->Caption);
    String oldstr = edMask->Text;
    int insertpos = edMask->SelStart;
    edMask->Text = oldstr.Insert(insertstr, insertpos + 1);
    edMask->SetFocus();
    edMask->SelStart = insertpos + insertstr.Length();
}

//---------------------------------------------------------------------------
void __fastcall TCustomCopyDlg::RenderPreview()
{
    String mask = edMask->Text;
    InfoFormatter fmt(mask.c_str());
    TPixelInfo pi(true);

    edPreview->Lines->Clear();
    edPreview->Lines->Add(fmt.GetFormattedString(pi).c_str());
}

//---------------------------------------------------------------------------
void __fastcall TCustomCopyDlg::ckHelpClick(TObject *Sender)
{
    ClientHeight = ButtonPanel->Height + EditPanel->Height;
    if (ckHelp->Checked)
    {
        ClientHeight += HelpPanel->Height;
    }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

