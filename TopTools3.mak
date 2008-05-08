# ---------------------------------------------------------------------------
!if !$d(BCB)
BCB = $(MAKEDIR)\..
!endif

# ---------------------------------------------------------------------------
# IDE SECTION
# ---------------------------------------------------------------------------
# The following section of the project makefile is managed by the BCB IDE.
# It is recommended to use the IDE to change any of the values in this
# section.
# ---------------------------------------------------------------------------

VERSION = BCB.05.03
# ---------------------------------------------------------------------------
PROJECT = exe\TopTools3.exe
OBJFILES = obj\TopTools3.obj obj\About.obj obj\BaseConv.obj obj\Control.obj \
    obj\Float.obj obj\Info.obj obj\Loupe.obj obj\Main.obj obj\Ruler.obj \
    obj\Tool.obj obj\Grabber.obj obj\LoupePanel.obj obj\Monitor.obj \
    obj\MouseTracker.obj obj\TopToolBar.obj obj\TrayIcon.obj \
    obj\HotkeyInfo.obj obj\OptionDlg.obj obj\HotkeyManager.obj \
    obj\ToolOptions.obj obj\AutoSaveDlg.obj obj\AutoSaveOptions.obj \
    obj\ImageView.obj obj\Capture.obj
RESFILES = obj\Graphics.res Cursors.res TopTools3.res
MAINSOURCE = TopTools3.cpp
RESDEPEN = $(RESFILES) About.dfm BaseConv.dfm Control.dfm Float.dfm Info.dfm \
    Loupe.dfm Main.dfm Ruler.dfm Tool.dfm OptionDlg.dfm AutoSaveDlg.dfm \
    ImageView.dfm Capture.dfm
LIBFILES = gif\gif.lib png\png.lib
IDLFILES = 
IDLGENFILES = 
LIBRARIES = TopToolParts.lib VCLX50.lib bcbsmp50.lib vcl50.lib
PACKAGES = vcl50.bpi vclx50.bpi vcljpg50.bpi bcbsmp50.bpi
SPARELIBS = vcl50.lib bcbsmp50.lib VCLX50.lib TopToolParts.lib
DEFFILE = 
# ---------------------------------------------------------------------------
PATHCPP = .;hotkey
PATHASM = .;
PATHPAS = .;
PATHRC = .;
DEBUGLIBPATH = $(BCB)\lib\debug
RELEASELIBPATH = $(BCB)\lib\release
USERDEFINES = NO_WIN32_LEAN_AND_MEAN;_DEBUG
SYSDEFINES = NO_STRICT;_VIS_NOLIB
INCLUDEPATH = "C:\Program Files\Borland\CBuilder5\Projects\";png;gif;hotkey;$(BCB)\include;$(BCB)\include\vcl
LIBPATH = "C:\Program Files\Borland\CBuilder5\Projects\";$(BCB)\Projects\Lib;png;gif;hotkey;$(BCB)\lib\obj;$(BCB)\lib;obj
WARNINGS= -w8092 -w8091 -w8090 -w8089 -w8087 -wprc -wuse -wucp -wstv -wstu -wsig \
    -wpin -w-par -wnod -wnak -wdef -wcln -wbbf -wasm -wamp -wamb
# ---------------------------------------------------------------------------
CFLAG1 = -Od -H=obj/pch.csm -Hc -Vx -Ve -X- -r- -a8 -b- -k -y -v -vi- -c -tW -tWM
IDLCFLAGS = -I"C:\Program Files\Borland\CBuilder5\Projects\." -Ipng -Igif -Ihotkey \
    -I$(BCB)\include -I$(BCB)\include\vcl -src_suffix cpp \
    -DNO_WIN32_LEAN_AND_MEAN -D_DEBUG -boa
PFLAGS = -N2obj -N0obj -$Y+ -$W -$O- -v -JPHNE -M
RFLAGS = 
AFLAGS = /mx /w2 /zi
LFLAGS = -Iobj -D"" -aa -Tpe -x -w -v
# ---------------------------------------------------------------------------
ALLOBJ = c0w32.obj sysinit.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) $(LIBRARIES) import32.lib cp32mt.lib
# ---------------------------------------------------------------------------
!ifdef IDEOPTIONS

[Version Info]
IncludeVerInfo=0
AutoIncBuild=0
MajorVer=1
MinorVer=0
Release=0
Build=0
Debug=0
PreRelease=0
Special=0
Private=0
DLL=0

[Version Info Keys]
CompanyName=
FileDescription=
FileVersion=1.0.0.0
InternalName=
LegalCopyright=
LegalTrademarks=
OriginalFilename=
ProductName=
ProductVersion=1.0.0.0
Comments=

[Debugging]
DebugSourceDirs=$(BCB)\source\vcl

!endif





# ---------------------------------------------------------------------------
# MAKE SECTION
# ---------------------------------------------------------------------------
# This section of the project file is not used by the BCB IDE.  It is for
# the benefit of building from the command-line using the MAKE utility.
# ---------------------------------------------------------------------------

.autodepend
# ---------------------------------------------------------------------------
!if "$(USERDEFINES)" != ""
AUSERDEFINES = -d$(USERDEFINES:;= -d)
!else
AUSERDEFINES =
!endif

!if !$d(BCC32)
BCC32 = bcc32
!endif

!if !$d(CPP32)
CPP32 = cpp32
!endif

!if !$d(DCC32)
DCC32 = dcc32
!endif

!if !$d(TASM32)
TASM32 = tasm32
!endif

!if !$d(LINKER)
LINKER = ilink32
!endif

!if !$d(BRCC32)
BRCC32 = brcc32
!endif


# ---------------------------------------------------------------------------
!if $d(PATHCPP)
.PATH.CPP = $(PATHCPP)
.PATH.C   = $(PATHCPP)
!endif

!if $d(PATHPAS)
.PATH.PAS = $(PATHPAS)
!endif

!if $d(PATHASM)
.PATH.ASM = $(PATHASM)
!endif

!if $d(PATHRC)
.PATH.RC  = $(PATHRC)
!endif
# ---------------------------------------------------------------------------
$(PROJECT): $(IDLGENFILES) $(OBJFILES) $(RESDEPEN) $(DEFFILE)
    $(BCB)\BIN\$(LINKER) @&&!
    $(LFLAGS) -L$(LIBPATH) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES)
!
# ---------------------------------------------------------------------------
.pas.hpp:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.pas.obj:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.cpp.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.cpp.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.asm.obj:
    $(BCB)\BIN\$(TASM32) $(AFLAGS) -i$(INCLUDEPATH:;= -i) $(AUSERDEFINES) -d$(SYSDEFINES:;= -d) $<, $@

.rc.res:
    $(BCB)\BIN\$(BRCC32) $(RFLAGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -fo$@ $<
# ---------------------------------------------------------------------------




