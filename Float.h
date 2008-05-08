//---------------------------------------------------------------------------
#ifndef FloatH
#define FloatH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFloatForm : public TForm
{
__published:	// IDE-managed Components
  TLabel *OffsetLabel;
  void __fastcall FormPaint(TObject *Sender);
private:	// User declarations
  String CaptionString;
public:		// User declarations
  __fastcall TFloatForm(TComponent* Owner);
  void __fastcall SetCaption(const String& newcaption);
  void __fastcall SetCaption(int offset);
  void __fastcall MoveRelative(int dx, int dy);
  void __fastcall MoveAbsolute(const TPoint& ptMouse,
                               const TPoint& ptRulerLeftTop,
                               bool isHorizontal);
};
//---------------------------------------------------------------------------
#endif
