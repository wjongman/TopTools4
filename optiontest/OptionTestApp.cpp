//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("OptionTestApp.res");
USEFORM("TestWindow.cpp", Form1);
USEFORM("ToolOptions.cpp", TopToolsOptions);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->CreateForm(__classid(TTopToolsOptions), &TopToolsOptions);
         Application->CreateForm(__classid(TForm1), &Form1);
         Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------
