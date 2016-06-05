; todo: check to see if program is currently running and offer to terminate it...
[Setup]
AppName=TopTools 4
AppVerName=TopTools version 4.0.2.78 beta
AppVersion=4.0.2.78
AppCopyright=Copyright © 1997-2016 Willem Jongman.
AppId=TopTools_4
DefaultDirName={pf}\TopTools 4
OutputBaseFilename=Setup_TopTools_402b

DisableReadyMemo=no
DisableReadyPage=no
DisableStartupPrompt=yes
DisableProgramGroupPage=yes

DefaultGroupName=TopTools 4

SetupIconFile=..\graphics\TopTools4-256.ico

WizardImageFile=logobig.bmp
WizardSmallImageFile=logosmall.bmp
WizardImageBackColor=clWhite
WizardImageStretch=false

AppPublisher=Willem Jongman
AppPublisherURL=http://toptools.org
AppMutex=TopTools_4_Mutex

PrivilegesRequired=poweruser

;[Types]
;Name: "normal"; Description: "Normal installation";
;, creates a key in the registry where settings and preferences will be saved.";
;Name: "portable"; Description: "Portable installation";
;, extracts the program to a location of your choice and creates an .ini file for saving settings.";

;[Components]
;Name: "normal"; Description: "Normal installation"; Types: normal; Flags: fixed
;Name: "portable"; Description: "Portable installation"; Types: portable; Flags: fixed

[Files]
Source: TopTools4.exe; DestDir: {app};
;Source: ..\exe\TopTools4.exe; DestDir: {app}; Components: normal portable

[Icons]
;Name: "{group}\TopTools"; Filename: "{app}\TopTools4.exe";
;Name: "{userdesktop}\TopTools"; Filename: "{app}\TopTools4.exe";
;Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\TopTools"; Filename: "{app}\TopTools4.exe";
;;Name: "{userstartmenu}\TopTools"; Filename: "{app}\TopTools.exe";
;;Name: "{userstartup}\TopTools"; Filename: "{app}\TopTools.exe";

Name: {group}\TopTools 4; Filename: {app}\TopTools4.exe; Tasks: programgroup; IconIndex: 0;
;Name: {group}\Uninstall TopTools 4; Filename: {uninstallexe}; Tasks: programgroup;
Name: {commondesktop}\TopTools 4; Filename: {app}\TopTools4.exe; Tasks: desktopicon;
;Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\TopTools; Filename: {app}\TopTools4.exe; Tasks: quicklaunchicon;
;Name: "{userstartmenu}\TopTools"; Filename: "{app}\TopTools.exe"; Tasks: startmenu
;Name: "{userstartup}\TopTools"; Filename: "{app}\TopTools.exe"; Tasks: startupicon

[Registry]
;> Under installation certain reg-keys are made (current-user).
;> Limited user then logs in (but since that's another account) HKCU reg-keys
;> are missing..
;
;Per MS's latest recommendations, installers should not create any keys in
;HKCU. Nor should applications expect any HKCU keys/values to exist when they
;are run; applications should gracefully fall back to the default settings
;when any HKCU keys/values are missing.
;
;--
;Jordan Russell

; delete "TopTools4" key upon install
;Root: HKCU; Subkey: Software\TopTools 4; Flags: deletekey
; delete "TopTools 4" key upon uninstall
Root: HKCU; Subkey: "Software\TopTools 4"; Flags: uninsdeletekey;
;Root: HKCU; Subkey: "Software\TopTools 4\main"; ValueType: dword; ValueName: "singleton"; ValueData: "0"; Flags: createvalueifdoesntexist
;Root: HKLM; Subkey: "Software\TopTools 4"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"
[INI]
;Filename: "TopTools4.ini"; Section: "main"; Flags: uninsdeletesection;

[Tasks]
Name: programgroup; Description: Create a &Program group in the Startmenu; Flags: checkedonce; GroupDescription: Shortcuts and Icons:
;Name: quicklaunchicon; Description: Create an icon on the &Quick Launch bar; Flags: checkedonce; GroupDescription: Shortcuts and Icons:
Name: desktopicon; Description: Create an icon on the &Desktop; Flags: unchecked; GroupDescription: Shortcuts and Icons:
;Name: portable; Description: "Use system-registry to save preferences and settings"; GroupDescription: "Settings:"; Flags: unchecked
;Name: inifile; Description: "Use .ini-file to save preferences and settings"; GroupDescription: "Settings:"; Flags: unchecked
;Name: startupicon; Description: "Create a shortcut in the &Startup folder"; GroupDescription: "Shortcuts:"; Flags: unchecked


[Run]
;Filename: {app}\TopTools4.exe; Description: Start the program; Flags: postinstall nowait skipifsilent

[_ISToolPreCompile]
Name: prepare_install.bat; Parameters: ..\exe\TopTools4.exe

