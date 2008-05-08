@echo off
rem StripReloc version 1.10, Copyright (C) 2000 Jordan Russell, www.jrsoftware.org
rem Strip relocation section from Win32 PE files

rem usage:     stripreloc [switches] filename.exe

rem switches:  /B  don't create .bak backup files
rem            /C  write a valid checksum in the header (instead of zero)
rem            /F  force stripping DLLs instead of skipping them. do not use!

stripreloc ..\exe\TopTools3.exe
clearexports ..\exe\TopTools3.exe

