//---------------------------------------------------------------------------
#ifndef ToolOptionsH
#define ToolOptionsH

#include "PersistOptions.h"
#include <map>

/////////////////////////////////////////////////////////////////////////////
enum TToolId
{
  idMain = 1,
  idRuler = 2,
  idLoupe = 4,
  idInfo = 8,
  idBaseconv = 16
};

enum TDoubleClickOpen
{
  dcoControl = 1,
  dcoRuler = 2,
  dcoLoupe = 4,
  dcoInfo = 8,
  dcoBaseconv = 16
};

enum TRunMode
{
  rmPortable,
  rmIniFile,
  rmRegistry
};

// Global String for registry access
extern const String g_RegBaseKey;
// Global flag to hold runmode
extern TRunMode g_RunMode;

/////////////////////////////////////////////////////////////////////////////
class TToolOptionBase
{
public:
  TToolOptionBase()  { }
  ~TToolOptionBase() { }

  virtual void __fastcall Load() = 0;
  virtual void __fastcall Save() = 0;

protected:
  virtual void __fastcall Init() = 0;

  String m_ToolName;
};

/////////////////////////////////////////////////////////////////////////////
class TRulerOptions : public TToolOptionBase
{
public:
  TRulerOptions()  { Init(); }
  ~TRulerOptions() { /*Save();*/ }

  bool Horizontal;
  int Length;
  bool ArrowNudge;
  bool Transparent;
  int Transparency;

  virtual void __fastcall Load();
  virtual void __fastcall Save();

protected:
  virtual void __fastcall Init();
};

/////////////////////////////////////////////////////////////////////////////
class TLoupeOptions : public TToolOptionBase
{
public:
  TLoupeOptions()  { Init(); }
  ~TLoupeOptions() { /*Save();*/ }

  int RefreshRate;
  int Zoom;
  bool CrosshairVisible;
  bool CenterboxVisible;
  bool GridVisible;
  bool MagnifySelf;
  int Width;
  int Height;

  virtual void __fastcall Load();
  virtual void __fastcall Save();

protected:
  virtual void __fastcall Init();
};

/////////////////////////////////////////////////////////////////////////////
class TMainOptions : public TToolOptionBase
{
public:
  TMainOptions()  { Init(); }
  ~TMainOptions() { /*Save();*/ }

  int DoubleClick;
  int SavedState;
  bool RememberState;
  bool StayOnTop;
  bool AutoStart;
  bool IsSingleton;
  bool IsTrayApp;

  void __fastcall SetAutoRun();
  bool __fastcall GetAutoRun();

  virtual void __fastcall Load();
  virtual void __fastcall Save();

protected:
  virtual void __fastcall Init();
};

/////////////////////////////////////////////////////////////////////////////
class TBaseconvOptions : public TToolOptionBase
{
public:
  TBaseconvOptions()  { Init(); }
  ~TBaseconvOptions() { /*Save();*/ }

  bool ShowBinary;

  virtual void __fastcall Load();
  virtual void __fastcall Save();

protected:
  virtual void __fastcall Init();
};

/////////////////////////////////////////////////////////////////////////////
class TColorCopyOptions : public TToolOptionBase
{
public:
  TColorCopyOptions()  { Init(); }
  ~TColorCopyOptions() { /*Save();*/ }

  bool Prefix;
  bool Quotes;
  String GetFormatString();

  virtual void __fastcall Load();
  virtual void __fastcall Save();

protected:
  virtual void __fastcall Init();
};

/////////////////////////////////////////////////////////////////////////////
class TCaptureOptions : public TToolOptionBase
{
public:
  TCaptureOptions()  { Init(); }
  ~TCaptureOptions() { /*Save();*/ }

  bool AutoSave;
  bool ShowLoupe;
  int FilterIndex;
  String LastDir;

  virtual void __fastcall Load();
  virtual void __fastcall Save();

protected:
  virtual void __fastcall Init();
};

/////////////////////////////////////////////////////////////////////////////
class TTopToolOptions
{
public:
  TTopToolOptions();

  void __fastcall LoadFromRegistry();
  void __fastcall SaveToRegistry();

  TMainOptions MainOptions;
  TCaptureOptions CaptureOptions;
  TRulerOptions RulerOptions;
  TBaseconvOptions BaseconvOptions;
  TColorCopyOptions ColorCopyOptions;
  TLoupeOptions LoupeOptions;

private:
  bool m_bIsInstalled;
  bool IsInstalled()
  {
    return false;
  }
};

//---------------------------------------------------------------------------
#endif

