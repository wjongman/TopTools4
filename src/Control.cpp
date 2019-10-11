//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Control.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "Tool"
#pragma resource "*.dfm"

//const ButtonCount = 6;

//---------------------------------------------------------------------------
__fastcall TControlForm::TControlForm(TComponent* Owner)
: TToolForm(Owner, "control")
{
    // Pull in our fancy toolbar
    m_toolbar = new TTopToolBar(this);
    m_toolbar->Parent = this;
    m_toolbar->Align = alClient;

    // Adjust dimensions
    ClientHeight = 30;
    ClientWidth = 30 * m_toolbar->GetButtonCount();

}

//---------------------------------------------------------------------------
__fastcall TControlForm::~TControlForm()
{
    delete m_toolbar;
}

//---------------------------------------------------------------------------
void __fastcall TControlForm::OnGetFocus()
{
    m_toolbar->Focussed = true;
}

//---------------------------------------------------------------------------
void __fastcall TControlForm::OnLoseFocus()
{
    m_toolbar->Focussed = false;
}

//---------------------------------------------------------------------------
void __fastcall TControlForm::UpdateFocus()
{
    m_toolbar->Refresh();
}

//---------------------------------------------------------------------------


