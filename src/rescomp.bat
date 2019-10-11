; use Microsoft resource compiler to create .res files, because
; Borland's RC.exe doesn't like Vista icons.

"C:\Program Files (x86)\Windows Kits\8.1\bin\x86\rc.exe" /rv Graphics.rc 
"C:\Program Files (x86)\Windows Kits\8.1\bin\x86\rc.exe" /rv AppIcon.rc 
