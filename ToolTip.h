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
    TLabel *Label1;
    TLabel *Label2;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormPaint(TObject *Sender);

private:  // User declarations
    String m_TipText;

public:   // User declarations
    __fastcall TToolTipForm(TComponent* Owner);
    void __fastcall SetText(const String& sIn);
    void __fastcall SetPosition(int X, int Y);
    void __fastcall SetDimensions(int W, int H);
    void __fastcall SetDisplayPosition(int left, int top, int width = 0, int height = 0);

protected:
    virtual void __fastcall CreateParams(Controls::TCreateParams &Params);

};
//---------------------------------------------------------------------------
extern PACKAGE TToolTipForm *ToolTipForm;
//---------------------------------------------------------------------------
#endif

