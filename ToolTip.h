//---------------------------------------------------------------------------

#ifndef ToolTipH
#define ToolTipH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TToolTipForm : public TForm
{
__published:  // IDE-managed Components
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormPaint(TObject *Sender);

private:  // User declarations
    String m_TipText;

public:   // User declarations
    __fastcall TToolTipForm(TComponent* Owner);
    void __fastcall SetText(const String& sIn);

protected:
    virtual void __fastcall CreateParams(Controls::TCreateParams &Params);

};
//---------------------------------------------------------------------------
extern PACKAGE TToolTipForm *ToolTipForm;
//---------------------------------------------------------------------------
#endif

