; todo: check to see if program is currently running and offer to terminate it...
[Setup]
AppName=TopTools 3.1 beta-7
AppVerName=TopTools version 3.1.8.1
AppVersion=3.1.8.1
AppCopyright=Copyright © 1997-2007 Willem Jongman.
AppId=TopTools_3
DefaultDirName={pf}\TopTools 3
OutputBaseFilename=Setup_TopTools31-beta7

DisableReadyMemo=no
DisableReadyPage=no
DisableStartupPrompt=yes
DisableProgramGroupPage=yes

DefaultGroupName=TopTools 3

SetupIconFile=toptools3.ico
UninstallIconFile=uninstall.ico

WizardImageFile=logobig.bmp
WizardSmallImageFile=logosmall.bmp
WizardImageBackColor=clWhite
WizardImageStretch=false

AppPublisher=Willem Jongman
AppPublisherURL=http://toptools.org
AppMutex=TopTools_3_Mutex

[Files]
Source: ..\exe\TopTools3.exe; DestDir: {app}

[Icons]
;Name: "{group}\TopTools"; Filename: "{app}\TopTools.exe";
;Name: "{userdesktop}\TopTools"; Filename: "{app}\TopTools.exe";
;Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\TopTools"; Filename: "{app}\TopTools.exe";
;;Name: "{userstartmenu}\TopTools"; Filename: "{app}\TopTools.exe";
;;Name: "{userstartup}\TopTools"; Filename: "{app}\TopTools.exe";

Name: {group}\TopTools 3; Filename: {app}\TopTools3.exe; Tasks: programgroup; IconIndex: 0
Name: {group}\Uninstall TopTools 3; Filename: {uninstallexe}; Tasks: programgroup
Name: {userdesktop}\TopTools3; Filename: {app}\TopTools3.exe; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\TopTools; Filename: {app}\TopTools3.exe; Tasks: quicklaunchicon
;Name: "{userstartmenu}\TopTools"; Filename: "{app}\TopTools.exe"; Tasks: startmenu
;Name: "{userstartup}\TopTools"; Filename: "{app}\TopTools.exe"; Tasks: startupicon

[Registry]
; delete "TopTools3" key upon install
Root: HKCU; Subkey: Software\TopTools 3; Flags: deletekey
; delete "TopTools 3" key upon uninstall
Root: HKCU; Subkey: Software\TopTools 3; Flags: uninsdeletekey deletekey

;[Components]
;Name: "main"; Description: "Main Options"; Types: full compact custom; Flags: fixed

[Tasks]
Name: programgroup; Description: Create a &Program group in in the Startmenu; Flags: checkedonce; GroupDescription: Shortcuts and Icons:
Name: quicklaunchicon; Description: Create an icon on the &Quick Launch bar; Flags: checkedonce; GroupDescription: Shortcuts and Icons:
Name: desktopicon; Description: Create an icon on the &Desktop; Flags: unchecked; GroupDescription: Shortcuts and Icons:
;Name: startmenu; Description: "Create an shortcut in the Start&menu"; GroupDescription: "Shortcuts:"; Flags: unchecked
;Name: startupicon; Description: "Create a shortcut in the &Startup folder"; GroupDescription: "Shortcuts:"; Flags: unchecked


[Run]
Filename: {app}\TopTools3.exe; Description: Start the program; Flags: postinstall nowait skipifsilent

[_ISToolPreCompile]
Name: D:\Develop\TopTools3\install\prepare_install.bat; Parameters: ..\exe\TopTools3.exe
