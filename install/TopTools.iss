; todo: check to see if program is currently running and offer to terminate it...
[Setup]
AppName=TopTools 4 alfa-1
AppVerName=TopTools version 4.0.0.20
AppVersion=4.0.0.20
AppCopyright=Copyright � 1997-2008 Willem Jongman.
AppId=TopTools_4
DefaultDirName={pf}\TopTools 4
OutputBaseFilename=Setup_TopTools4_alfa1

DisableReadyMemo=no
DisableReadyPage=no
DisableStartupPrompt=yes
DisableProgramGroupPage=yes

DefaultGroupName=TopTools 4

SetupIconFile=toptools3.ico

WizardImageFile=logobig.bmp
WizardSmallImageFile=logosmall.bmp
WizardImageBackColor=clWhite
WizardImageStretch=false

AppPublisher=Willem Jongman
AppPublisherURL=http://toptools.org
AppMutex=TopTools_4_Mutex

;[Types]
;Name: "normal"; Description: "Normal installation";
;, creates a key in the registry where settings and preferences will be saved.";
;Name: "portable"; Description: "Portable installation";
;, extracts the program to a location of your choice and creates an .ini file for saving settings.";

;[Components]
;Name: "normal"; Description: "Normal installation"; Types: normal; Flags: fixed
;Name: "portable"; Description: "Portable installation"; Types: portable; Flags: fixed

[Files]
Source: ..\exe\TopTools4.exe; DestDir: {app};
;Source: ..\exe\TopTools4.exe; DestDir: {app}; Components: normal portable

[Icons]
;Name: "{group}\TopTools"; Filename: "{app}\TopTools.exe";
;Name: "{userdesktop}\TopTools"; Filename: "{app}\TopTools.exe";
;Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\TopTools"; Filename: "{app}\TopTools.exe";
;;Name: "{userstartmenu}\TopTools"; Filename: "{app}\TopTools.exe";
;;Name: "{userstartup}\TopTools"; Filename: "{app}\TopTools.exe";

Name: {group}\TopTools 4; Filename: {app}\TopTools4.exe; Tasks: programgroup; IconIndex: 0;
Name: {group}\Uninstall TopTools 4; Filename: {uninstallexe}; Tasks: programgroup;
Name: {userdesktop}\TopTools4; Filename: {app}\TopTools4.exe; Tasks: desktopicon;
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\TopTools; Filename: {app}\TopTools4.exe; Tasks: quicklaunchicon;
;Name: "{userstartmenu}\TopTools"; Filename: "{app}\TopTools.exe"; Tasks: startmenu
;Name: "{userstartup}\TopTools"; Filename: "{app}\TopTools.exe"; Tasks: startupicon

[Registry]
; delete "TopTools4" key upon install
;Root: HKCU; Subkey: Software\TopTools 4; Flags: deletekey
; delete "TopTools 4" key upon uninstall
Root: HKCU; Subkey: Software\TopTools 4; Flags: uninsdeletekey;

[INI]
;Filename: "TopTools4.ini"; Section: "main"; Flags: uninsdeletesection;

[Tasks]
Name: programgroup; Description: Create a &Program group in in the Startmenu; Flags: checkedonce; GroupDescription: Shortcuts and Icons:
Name: quicklaunchicon; Description: Create an icon on the &Quick Launch bar; Flags: checkedonce; GroupDescription: Shortcuts and Icons:
Name: desktopicon; Description: Create an icon on the &Desktop; Flags: unchecked; GroupDescription: Shortcuts and Icons:
;Name: regentry; Description: "Use system-registry to save preferences and settings"; GroupDescription: "Settings:"; Flags: unchecked
;Name: inifile; Description: "Use .ini-file to save preferences and settings"; GroupDescription: "Settings:"; Flags: unchecked
;Name: startupicon; Description: "Create a shortcut in the &Startup folder"; GroupDescription: "Shortcuts:"; Flags: unchecked


[Run]
Filename: {app}\TopTools4.exe; Description: Start the program; Flags: postinstall nowait skipifsilent

[_ISToolPreCompile]
Name: D:\Develop\TopTools4\install\prepare_install.bat; Parameters: ..\exe\TopTools4.exe
