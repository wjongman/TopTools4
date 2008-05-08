//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PathEllipsisLabel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TPathEllipsisLabel *)
{
  new TPathEllipsisLabel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TPathEllipsisLabel::TPathEllipsisLabel(TComponent* Owner)
  : TLabel(Owner)
{
}
//---------------------------------------------------------------------------
namespace Pathellipsislabel
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TPathEllipsisLabel)};
     RegisterComponents("TopTools", classes, 0);
  }
}
//---------------------------------------------------------------------------
 