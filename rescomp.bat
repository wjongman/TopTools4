rem use Microsoft resource compiler to create .res file, because
rem Borland's RC.exe doesn't like Vista icons.


"C:\Program Files (x86)\Windows Kits\8.1\bin\x86\rc.exe" %1 %2 %3
