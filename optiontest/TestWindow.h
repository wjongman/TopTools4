//---------------------------------------------------------------------------

#ifndef TestWindowH
#define TestWindowH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
  TButton *bnAutosave;
  TButton *bnQuit;
  TButton *bnTopTools;
  void __fastcall bnQuitClick(TObject *Sender);
  void __fastcall bnAutosaveClick(TObject *Sender);
  void __fastcall bnTopToolsClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TForm1(TComponent* Owner);
  __fastcall ~TForm1();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
