//---------------------------------------------------------------------------

#ifndef LoupePanelH
#define LoupePanelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

const MAXZOOM = 20;

//---------------------------------------------------------------------------
class PACKAGE TLoupePanel : public TCustomPanel
{
public:
  __fastcall TLoupePanel(TComponent* Owner);
  __fastcall ~TLoupePanel();

  void __fastcall ZoomIn();
  void __fastcall ZoomOut();

  void __fastcall ToggleCrosshair();
  void __fastcall ToggleCenterbox();
  void __fastcall ToggleGrid();
  void __fastcall ToggleFrozen();
  void __fastcall ToggleLocked();

  void __fastcall UpdateView();
  void __fastcall UpdateView(const TPoint& ptMouse);
  void __fastcall MoveLockPos(int dx, int dy);
  void __fastcall UpdateView2(const TPoint& ptMouse);

protected:
  void __fastcall RenderMask(Graphics::TBitmap* TargetBmp);
  void __fastcall DrawCrosshair(Graphics::TBitmap* TargetBmp);
  void __fastcall DrawCenterbox(Graphics::TBitmap* TargetBmp);
  void __fastcall DrawGrid(Graphics::TBitmap* TargetBmp);
  void __fastcall DrawTickMarks(Graphics::TBitmap* TargetBmp);
  void __fastcall DrawSizeGrip(Graphics::TBitmap* TargetBmp);

protected:
  DYNAMIC void __fastcall Resize(void);
  virtual void __fastcall Paint(void);
  virtual void __fastcall Paint2(void);
  virtual void __fastcall Paint3(void);

  MESSAGE void OnNCHitTest(TWMNCHitTest &Message);
protected:
BEGIN_MESSAGE_MAP
  VCL_MESSAGE_HANDLER(WM_NCHITTEST, TWMNCHitTest, OnNCHitTest)
END_MESSAGE_MAP(TCustomPanel)

private:
  Graphics::TBitmap* m_BufferBmp;
  Graphics::TBitmap* m_MaskBmp;
  Graphics::TBitmap* m_Snapshot;

//  HDC m_DesktopDC;
  TTimer* m_Timer;
  void __fastcall TimerTick(TObject *Sender);
  int m_RefreshRate;

  TRect m_rcSource;

  int m_zoom;
  bool m_bShowCrosshair;
  bool m_bShowGrid;
  bool m_bShowCenterbox;
  bool m_bIsFrozen;
  bool m_bIsLocked;
  bool m_bMagnifySelf;
  bool m_bDragging;

  TPoint m_ptLockPos;
  TPoint m_ptViewCenter;
  TPoint m_ptDragStart;

  void __fastcall SetZoom(int zoom);
  void __fastcall SetRefreshRate(int rate);
  void __fastcall SetCrosshair(bool show) { m_bShowCrosshair = show; Invalidate(); }
  void __fastcall SetGrid(bool show)      { m_bShowGrid      = show; Invalidate(); }
  void __fastcall SetCenterbox(bool show) { m_bShowCenterbox = show; Invalidate(); }
  Graphics::TBitmap* __fastcall GetBitmap() { /*if (m_bIsLocked)*/ return m_BufferBmp; }

  DYNAMIC void __fastcall MouseDown(TMouseButton Button, TShiftState Shift, int X, int Y);
  DYNAMIC void __fastcall MouseMove(TShiftState Shift, int X, int Y);
  DYNAMIC void __fastcall MouseUp(TMouseButton Button, TShiftState Shift, int X, int Y);

__published:
  __property int Zoom = { read = m_zoom, write = SetZoom };
  __property int RefreshRate = { read = m_RefreshRate, write = SetRefreshRate };
  __property bool GridVisible = { read = m_bShowGrid, write = SetGrid };
  __property bool CrosshairVisible = { read = m_bShowCrosshair, write = SetCrosshair };
  __property bool CenterboxVisible = { read = m_bShowCenterbox, write = SetCenterbox };
  __property bool Locked = { read = m_bIsLocked };
  __property bool Frozen = { read = m_bIsFrozen };
  __property bool MagnifySelf = { read = m_bMagnifySelf, write = m_bMagnifySelf };
  __property Graphics::TBitmap* Bitmap = { read = GetBitmap };
};
//---------------------------------------------------------------------------
#endif

