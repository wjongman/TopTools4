//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "BaseConv.h"
#include <Clipbrd.hpp>

#include <stdlib.h>
#include <stdio.h>
#include "PersistOptions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Tool"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TBaseConvForm::TBaseConvForm(TComponent* Owner)
: TToolForm(Owner, "baseconv")
{
  // Initialize flags
  bUpdating = false;
  bSkipDelimit = false;

  // Clear labels
  AscEdit->Clear();
  HexEdit->Clear();
  DecEdit->Clear();
  BinEdit->Clear();

  LoadSettings();
}

//---------------------------------------------------------------------------
__fastcall TBaseConvForm::~TBaseConvForm()
{
//  SaveSettings();
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::AscEditKeyPress(TObject *Sender, char &Key)
{
  if (Key > 0 && Key < 127 && isprint(Key))
  {
    iValue = Key;
    UpdateAsc(iValue);
    UpdateHex(iValue);
    UpdateDec(iValue);
    UpdateBin(iValue);
  }
  Key = '\0';
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::HexEditKeyPress(TObject *Sender, char &Key)
{
  if (Key == VK_BACK)
    return;

  // 32 bits are the limit
  if (HexEdit->Text.Length() > 7 && HexEdit->SelLength == 0)
    Key = '\0';

  if (!(isdigit(Key) || (toupper(Key) >= 'A' && toupper(Key) <= 'F')))
    Key = '\0';
  else
    Key = (char)toupper(Key);
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::DecEditKeyPress(TObject *Sender, char &Key)
{
  if (Key == VK_BACK)
    return;

  if (!isdigit(Key))
    Key = '\0';
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::BinEditKeyPress(TObject *Sender, char &Key)
{
  if (Key == VK_BACK)
  {
    bSkipDelimit = true;
    return;
  }

  if (BinEdit->Text.Length() > 40 && BinEdit->SelLength == 0)
    Key = '\0';

  else if (!(Key == '0' || Key == '1'))
    Key = '\0';
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::HexEditChange(TObject *Sender)
{
  if (bUpdating)
    return;

  iValue = 0;
  char szInBuf[40] = "";
  strcpy(szInBuf, HexEdit->Text.c_str());
  sscanf(szInBuf, "%x", &iValue);
  UpdateAsc(iValue);
  UpdateDec(iValue);
  UpdateBin(iValue);
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::DecEditChange(TObject *Sender)
{
  if (bUpdating)
    return;

  iValue = 0;
  char szInBuf[40] = "";
  strcpy(szInBuf, DecEdit->Text.c_str());
  sscanf(szInBuf, "%u", &iValue);
  UpdateAsc(iValue);
  UpdateHex(iValue);
  UpdateBin(iValue);
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::BinEditKeyDown(TObject *Sender, WORD &Key,
                                              TShiftState Shift)
{
  if (Key == VK_DELETE)
    bSkipDelimit = true;

  EditKeyDown(Sender, Key, Shift);
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::BinEditChange(TObject *Sender)
{
  if (bUpdating || bSkipDelimit)
  {
    bSkipDelimit = false;
    return;
  }

  String sBin = UnDelimitBin(BinEdit->Text);
  // convert binary to decimal
  int i = sBin.Length();
  int multiplier = 1;
  iValue = 0;
  while (i > 0)
  {
    iValue = (sBin[i] == '1' ? iValue + multiplier : iValue);
    multiplier *= 2;
    i--;
  }
  UpdateBin(iValue);
  UpdateAsc(iValue);
  UpdateHex(iValue);
  UpdateDec(iValue);
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::UpdateAsc(int iValue)
{
  bUpdating = true;
  AscEdit->Clear();
  if (iValue > 0 && iValue < 128 && isprint(iValue))
    AscEdit->Text = AnsiString((char)iValue);
  bUpdating = false;
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::UpdateHex(int iValue)
{
  bUpdating = true;
  char szOutBuf[256];
  itoa(iValue, szOutBuf, 16);
  HexEdit->Clear();
  HexEdit->Text = AnsiString(strupr(szOutBuf));
  bUpdating = false;
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::UpdateDec(int iValue)
{
  bUpdating = true;
  char szOutBuf[256];
  ultoa(iValue, szOutBuf, 10);
  DecEdit->Clear();
  DecEdit->Text = AnsiString(szOutBuf);
  bUpdating = false;
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::UpdateBin(int iValue)
{
  bUpdating = true;
  char szOutBuf[256];
  itoa(iValue, szOutBuf, 2);
  // Remember caret position
  int binlen = BinEdit->Text.Length();
  int caretpos = BinEdit->SelStart;

  BinEdit->Clear();
  BinEdit->Text = DelimitBin(String(szOutBuf));
  // Restore caret position
  int caretshift = BinEdit->Text.Length() - binlen;
  BinEdit->SelStart = caretpos + caretshift;

  bUpdating = false;
}

//---------------------------------------------------------------------------
const String __fastcall TBaseConvForm::UnDelimitBin(const String &sDelim)
{
  String sUnDelim = "";
  int len = sDelim.Length();
  int i = 1;
  while (i <= len)
  {
    if (sDelim[i] != ' ')
      sUnDelim += sDelim[i];
    i++;
  }
  return sUnDelim;
}

//---------------------------------------------------------------------------

const String __fastcall TBaseConvForm::DelimitBin(const String &sUnDelim)
{
  String sDelim = "";
  int len = sUnDelim.Length();
  int i = 1;
  while (i <= len)
  {
    if ((len-i+1)%4 == 0)
      sDelim += " ";
    sDelim += sUnDelim[i];
    i++;
  }
  return sDelim;
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::EditKeyDown(TObject *Sender, WORD &Key,
                                           TShiftState Shift)
{
  if (Key == VK_DOWN)
  {
    // Down arrow decrements value
    iValue--;
    UpdateAsc(iValue);
    UpdateHex(iValue);
    UpdateDec(iValue);
    UpdateBin(iValue);
  }
  if (Key == VK_UP)
  {
    // Up arrow increments value
    iValue++;
    UpdateAsc(iValue);
    UpdateHex(iValue);
    UpdateDec(iValue);
    UpdateBin(iValue);
  }
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::EditContextPopup(TObject *Sender, TPoint &MousePos,
                                                bool &Handled)
{
  // See which editbox has focus
  TEdit* pEditbox = reinterpret_cast<TEdit*>(Sender);
  if (!pEditbox)
  {
    // Nothing to do
    Handled = true;
    return;
  }

  if (Clipboard()->HasFormat(CF_TEXT))
  {
    // See what is on the clipboard
    String ClipText = Clipboard()->AsText;

    // Validate clipboard contents according to type of target editbox
    bool valid = false;

    if (pEditbox == BinEdit)
      valid = IsValidBinary(ClipText);

    else if (pEditbox == HexEdit)
      valid = IsValidHex(ClipText);

    else if (pEditbox == DecEdit)
      valid = IsValidDecimal(ClipText);

    else if (pEditbox == AscEdit)
      valid = IsValidAscii(ClipText);

    // We only allow validated contents to be pasted
    if (valid)
    {
      // Consolidate what we are going to paste because
      // an other application might alter the clipboard
      // contents before we do the actual paste
      m_ClipboardContent = ClipText;

      // Also remember where to paste
      m_PasteTarget = pEditbox;

      // Enable the "Paste" item in the menu
      miPaste->Enabled = true;
    }
    else
    {
      m_ClipboardContent = "";
      m_PasteTarget = NULL;
      miPaste->Enabled = false;
    }
  }

  // Only enable copy menu when there is a text selection
  if (!pEditbox->SelText.IsEmpty())
  {
    // Remember text to be copied
    m_CopyText = pEditbox->SelText;

    // Enable the "Copy" item in the menu
    miCopy->Enabled = true;
  }
  else
  {
    m_CopyText = "";
    miCopy->Enabled = false;
  }

  miBinary->Checked = plBinary->Visible;

  // Popup the contextmenu near the mousepointer
  TPoint ptMouse = ClientToScreen(MousePos);
  BaseconvMenu->Popup(ptMouse.x, ptMouse.y);

  // Tell caller we handled the menu
  Handled = true;
}

//---------------------------------------------------------------------------
bool TBaseConvForm::IsValidAscii(const String& Values)
{
  return false;
//  char Value = Values[1];
//  return (Value > 0 && Value < 127 && isprint(Value));
}

//---------------------------------------------------------------------------
bool TBaseConvForm::IsValidHex(const String& Values)
{
  for (int i = 1; i <= Values.Length(); i++)
  {
    char Value = Values[i];
    if (!(isdigit(Value) || (toupper(Value) >= 'A' && toupper(Value) <= 'F')))
      return false;
  }
  return true;
}

//---------------------------------------------------------------------------
bool TBaseConvForm::IsValidDecimal(const String& Values)
{
  for (int i = 1; i <= Values.Length(); i++)
  {
    if (!isdigit(Values[i]))
      return false;
  }
  return true;
}

//---------------------------------------------------------------------------
bool TBaseConvForm::IsValidBinary(const String& Values)
{
  for (int i = 1; i <= Values.Length(); i++)
  {
    char Value = Values[i];
    if (!(Value == '0' || Value == '1'))
      return false;
  }
  return true;
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::miHideClick(TObject *Sender)
{
  Close();
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::miCopyClick(TObject *Sender)
{
  Clipboard()->AsText = m_CopyText;
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::miPasteClick(TObject *Sender)
{
  if (m_PasteTarget)
    m_PasteTarget->Text = m_ClipboardContent.UpperCase();
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::miBinaryClick(TObject *Sender)
{
  miBinary->Checked = !miBinary->Checked;
  ShowBinaryField(miBinary->Checked);
  SaveSettings();
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::miExitClick(TObject *Sender)
{
  Application->Terminate();
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::LoadSettings()
{
  ShowBinaryField(g_ToolOptions.Get("baseconv", "showbinary", true));
}

//---------------------------------------------------------------------------
void __fastcall TBaseConvForm::SaveSettings()
{
  g_ToolOptions.Set("baseconv", "showbinary", plBinary->Visible);
}

//---------------------------------------------------------------------------
void TBaseConvForm::ShowBinaryField(bool show)
{
  if (show)
  {
    ClientHeight = plTop->Height + plBinary->Height;
    plBinary->Visible = true;
  }
  else
  {
    ClientHeight = plTop->Height;
    plBinary->Visible = false;
  }
}

