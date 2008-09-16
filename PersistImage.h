//---------------------------------------------------------------------------

#ifndef PersistImageH
#define PersistImageH
//---------------------------------------------------------------------------
class TPersistImage
{
public:
  __fastcall TPersistImage(Graphics::TBitmap* pBitmap);
  __fastcall ~TPersistImage();
  bool __fastcall SaveFileDialog(int &filterindex, String& InitialDir);
  void __fastcall Print()                      { DoPrintImage(); }
  void __fastcall Save(const String& PathName) { DoSaveToFile(PathName); }
  void __fastcall Copy()                       { DoCopyToClipboard(); }
  void __fastcall View();

private:
  Graphics::TBitmap* m_pBitmap;

  void __fastcall DoCopyToClipboard();
  void __fastcall DoSaveToFile(const String& PathName);
  void __fastcall DoPrintImage();
//  void __fastcall DoPrintImage(TCanvas *PrinterCanvas,int pX,int pY,
//                               Graphics::TBitmap *pBitmap);
  bool __fastcall DisplayIsPaletted();
  void __fastcall StretchBltBitmap(TCanvas *pCanvas, int iX, int iY,
                                   int iWidth, int iHeight,
                                   Graphics::TBitmap *pBitmap);
  void __fastcall StretchToPrintCanvas(TCanvas *pCanvas, int iX, int iY,
                                       int iWidth, int iHeight,
                                       Graphics::TBitmap *pBitmap);

protected:

};



//---------------------------------------------------------------------------
#endif

