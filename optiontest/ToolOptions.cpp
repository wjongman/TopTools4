//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ToolOptions.h"
#include "HotkeyDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTopToolsOptions *TopToolsOptions;
//---------------------------------------------------------------------------
__fastcall TTopToolsOptions::TTopToolsOptions(TComponent* Owner)
  : TForm(Owner)
{
  ClientWidth = 400;
  ClientHeight = 240;
  plRef->Visible = false;
  ActivatePage("General");
}


//---------------------------------------------------------------------------/*
void TTopToolsOptions::ActivatePage(const String sActive)
{
  TPanel* ActivePanel = NULL;

  if (sActive == "General")
    ActivePanel = plGeneral;
  else if (sActive == "Ruler")
    ActivePanel = plRuler;
  else if (sActive == "Loupe")
    ActivePanel = plLoupe;
  else if (sActive == "Info")
    ActivePanel = plInfo;
  else if (sActive == "Base Converter")
    ActivePanel = plBaseconv;
  else if (sActive == "Screen Grabber")
    ActivePanel = plGrabber;
  else if (sActive == "Hotkeys")
    ActivePanel = plHotkeys;

  if (ActivePanel)
  {
    ActivePage = sActive;
    HideAll();
    ActivePanel->Left = plRef->Left;
    ActivePanel->Top = plRef->Top;
    ActivePanel->Visible = true;
  }
}

//---------------------------------------------------------------------------
void TTopToolsOptions::HideAll()
{
  plGeneral->Visible = false;
  plGeneral->Left = plRef->Left;
  plGeneral->Top = plRef->Top;

  plRuler->Visible = false;
  plRuler->Left = plRef->Left;
  plRuler->Top = plRef->Top;

  plLoupe->Visible = false;
  plLoupe->Left = plRef->Left;
  plLoupe->Top = plRef->Top;

  plInfo->Visible = false;
  plInfo->Left = plRef->Left;
  plInfo->Top = plRef->Top;

  plBaseconv->Visible = false;
  plBaseconv->Left = plRef->Left;
  plBaseconv->Top = plRef->Top;

  plGrabber->Visible = false;
  plGrabber->Left = plRef->Left;
  plGrabber->Top = plRef->Top;

  plHotkeys->Visible = false;
  plHotkeys->Left = plRef->Left;
  plHotkeys->Top = plRef->Top;
}

//---------------------------------------------------------------------------
void __fastcall TTopToolsOptions::FormActivate(TObject *Sender)
{
  // Set focus to currently visible item
  if (lvSelectOption->Items->Count > 0)
  {
    TListItem *item = lvSelectOption->Items->Item[m_CurrentItem];
    item->Selected = true;
    item->Focused = true;
  }

}

//---------------------------------------------------------------------------
void __fastcall TTopToolsOptions::lvSelectOptionChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
  if (lvSelectOption->ItemFocused)
    ActivatePage(lvSelectOption->ItemFocused->Caption);
}

//---------------------------------------------------------------------------

void __fastcall TTopToolsOptions::bnHotkeyClick(TObject *Sender)
{
  TButton* PressedButton = reinterpret_cast<TButton*>(Sender);
  if (PressedButton)
  {
//    ActivatePage("Hotkeys");
    THotkeyDlg* HkDialog = new THotkeyDlg(this);
    HkDialog->Left = PressedButton->Left;
    HkDialog->Top = PressedButton->Top;
    HkDialog->cbHotkeyAction->ItemIndex = PressedButton->Tag;
    HkDialog->ShowModal();
    delete HkDialog;
  }
}
//---------------------------------------------------------------------------

