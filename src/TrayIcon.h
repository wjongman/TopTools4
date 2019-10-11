#ifndef TrayIconH
#define TrayIconH


#define TRAYICONNOTIFY		(WM_APP + 100)

//---------------------------------------------------------------------------
class cTrayIcon
{
public:
  cTrayIcon(HWND hWindow, HICON hIcon, char* szTip = "");
  ~cTrayIcon();

public:
  void SetIcon(HICON hIcon, char* szTip = "");
  void SetTooltipText(char* szTip);

  void Add();
  void Remove();
  void Change();
  void Restore();

protected:
  HWND m_hWnd;
  HICON m_hIcon;
  int SendTrayMessage(HWND hWnd, DWORD dwMessage,
                      UINT uID = 0, HICON hIcon = 0, char* szTip = "");

};
//---------------------------------------------------------------------------
#endif
