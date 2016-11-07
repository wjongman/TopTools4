//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("GrabberPreset.res");
USEFORM("PresetDlg.cpp", PresetDialog);
USEFORM("PresetPropsDlg.cpp", PresetPropsDlg);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TPresetDialog), &PresetDialog);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
