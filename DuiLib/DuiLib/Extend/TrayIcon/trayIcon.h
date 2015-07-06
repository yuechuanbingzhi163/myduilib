#ifndef _trayIcon_h
#define _trayIcon_h

#pragma once

#include <ShellAPI.h>
namespace DuiLib {
	class UILIB_API CTrayIconController
	{
	public:
		CTrayIconController();
		virtual ~CTrayIconController();
	public:
		static UINT RegisterTaskbarCreatedMsg();

		BOOL Setup(HWND hwnd, UINT uID, UINT uCBMsg, HICON hicon, TCHAR* tip);
		BOOL Remove();
		BOOL SetIcon(HICON hicon);
		BOOL SetTip(TCHAR* tip);
		BOOL ShowToolTip(LPCTSTR szMsg,LPCTSTR szTitle,DWORD dwInfoFlags = NIIF_INFO,UINT uTimeout = 1000*2);
	protected:
		NOTIFYICONDATA    _nid;
	};
}

#endif//_trayIcon_h