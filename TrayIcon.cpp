// An encapsulation of the Win95 function Shell_NotifyIcon()
// It docks an icon in the system tray and sets up the notification scheme
#include <vcl.h>
#pragma hdrstop

#include "trayicon.h"

const UINT uNotify = 1600;

//---------------------------------------------------------------------------
cTrayIcon::cTrayIcon(HWND hWindow, HICON hIcon, char *szTip)
{
  // Store parent window handle
  m_hWnd = hWindow;
  // Store icon handle
  m_hIcon = hIcon;
  //
  SendTrayMessage(m_hWnd, NIM_ADD, uNotify, hIcon, szTip);
}

//---------------------------------------------------------------------------
cTrayIcon::~cTrayIcon()
{
  Remove();
}

//---------------------------------------------------------------------------
void
cTrayIcon::SetIcon(HICON hIcon, char* szTip)
{
  SendTrayMessage(m_hWnd, NIM_MODIFY, uNotify, hIcon, szTip);
}

//---------------------------------------------------------------------------
void
cTrayIcon::SetTooltipText(char* szTip)
{
  SendTrayMessage(m_hWnd, NIM_MODIFY, uNotify, NULL, szTip);
}

//---------------------------------------------------------------------------
void
cTrayIcon::Restore()
{
  Remove();
  Add();
}

//---------------------------------------------------------------------------
void
cTrayIcon::Add()
{
  SendTrayMessage(m_hWnd, NIM_ADD, uNotify, m_hIcon, NULL);
}

//---------------------------------------------------------------------------
void
cTrayIcon::Remove()
{
  SendTrayMessage(m_hWnd, NIM_DELETE, uNotify, NULL, NULL);
}

//---------------------------------------------------------------------------
void
cTrayIcon::Change()
{
  SendTrayMessage(m_hWnd, NIM_MODIFY, uNotify, NULL, NULL);
}

//---------------------------------------------------------------------------
int
cTrayIcon::SendTrayMessage(HWND hwnd, DWORD dwMessage,
                       UINT uID, HICON hIcon, char* szTip)
{
  int result;

  NOTIFYICONDATA tnd;

  tnd.cbSize            = sizeof(NOTIFYICONDATA);
  tnd.hWnd              = hwnd;
  tnd.uID               = uID;
  tnd.uFlags            = NIF_MESSAGE | NIF_ICON | NIF_TIP;
  tnd.uCallbackMessage  = TRAYICONNOTIFY;
  tnd.hIcon             = hIcon;

  if (szTip)
    lstrcpyn(tnd.szTip, szTip, sizeof(tnd.szTip));
  else
    tnd.szTip[0] = '\0';

  result = Shell_NotifyIcon(dwMessage, &tnd);

  return result;
}

//---------------------------------------------------------------------------

