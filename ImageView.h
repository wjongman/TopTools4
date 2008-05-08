//---------------------------------------------------------------------------

#ifndef ImageViewH
#define ImageViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <vector>
#include <list>

//---------------------------------------------------------------------------
class TImageViewer : public TForm
{
__published:  // IDE-managed Components
  TImage *Image;
  void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall FormShow(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
//  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:  // User declarations
  void __fastcall SetBitmap(Graphics::TBitmap* pBitmap);

  void __fastcall SaveToFile();
  void __fastcall DoSaveToFile(const String& PathName);
  void __fastcall CopyToClipboard();
  void __fastcall PopulateViewerMenu();
  void __fastcall ShowViewerMenu(int X, int Y);
  void __fastcall ViewerMenuClick(TObject *Sender);
  void __fastcall PrintImage(Graphics::TBitmap* pBitmap);
  void __fastcall StretchBltBitmap(TCanvas *pCanvas, int iX, int iY,
                                   int iWidth, int iHeight,
                                   Graphics::TBitmap *pBitmap);
  void __fastcall StretchToPrintCanvas(TCanvas *pCanvas, int iX, int iY,
                                       int iWidth, int iHeight,
                                       Graphics::TBitmap *pBitmap);

  int FId;
  RECT m_rcGrab;

  //TNotifyEvent FOnClose;
  TPopupMenu* m_ViewerMenu;

public:
  __fastcall TImageViewer(TComponent* Owner, int id, const TRect& rcGrab);
  __fastcall ~TImageViewer();

  __property int Id = { read = FId };
  __property Graphics::TBitmap* Bitmap = { write = SetBitmap };
//  __property OnClose;
//  __property TNotifyEvent OnClose = { read = FOnClose, write = FOnClose };
};
//---------------------------------------------------------------------------
extern PACKAGE TImageViewer *ImageViewer;
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
class TImageViewerList //: TObject
{
private:
  int m_lastid;
//  std::vector<TImageViewer*> m_Viewers;
  std::list<TImageViewer*> m_Viewers;
  typedef std::list<TImageViewer*>::iterator view_iterator;

public:

  //-------------------------------------------------------------------------
  TImageViewerList()
  {
    m_lastid = 0;
  }

  //-------------------------------------------------------------------------
  virtual __fastcall ~TImageViewerList()
  {
    // Close and delete all open viewer windows
    for (view_iterator iter = m_Viewers.begin(); iter != m_Viewers.end(); iter++)
    {
      TImageViewer* viewer = *iter;

      // Nuke OnClose handler, we don't want to unleash recursive close actions..
      viewer->OnClose = NULL;
      viewer->Close();
      delete viewer;
    }
  }

  //-------------------------------------------------------------------------
  TImageViewer* NewViewer(TComponent* Owner, const TRect& rcGrab)
  {
    TImageViewer* viewer = new TImageViewer(Owner, m_lastid++, rcGrab);
    viewer->OnClose = ViewerClosed;
    m_Viewers.push_back(viewer);
    return viewer;
  }

  //-------------------------------------------------------------------------
  void DeleteViewer(int id)
  {
    view_iterator itViewer = FindViewer(id);
    if (itViewer != m_Viewers.end())
    {
      delete *itViewer;
      m_Viewers.erase(itViewer);
    }
  }

private:

  //---------------------------------------------------------------------------
  void __fastcall ViewerClosed(TObject *Sender, TCloseAction &Action)
  {
    TImageViewer* viewer = reinterpret_cast<TImageViewer*>(Sender);
    DeleteViewer(viewer->Id);
  }

  //-------------------------------------------------------------------------
  view_iterator FindViewer(int id)
  {
    view_iterator itViewer = m_Viewers.begin();
    for (; itViewer != m_Viewers.end(); ++itViewer)
    {
      TImageViewer* viewer = *itViewer;
      if (viewer->Id == id)
        break;
    }
    return itViewer;
  }

};

#endif
