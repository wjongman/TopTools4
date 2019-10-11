//---------------------------------------------------------------------------

#ifndef PathEllipsisLabelH
#define PathEllipsisLabelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TPathEllipsisLabel : public TLabel
{
private:
protected:
	DYNAMIC void __fastcall DoDrawText(Windows::TRect &Rect, int Flags)
  {
    Flags |= DT_PATH_ELLIPSIS;
    TLabel::DoDrawText(Rect, Flags);
  }
public:
  __fastcall TPathEllipsisLabel(TComponent* Owner);
__published:
};
//---------------------------------------------------------------------------
#endif
