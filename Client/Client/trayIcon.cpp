#include "StdAfx.h"
#include "trayIcon.h"
#include <Shlwapi.h>
/*#include "../../../../../DuiLib/Utils/sys.h"*/

namespace DuiLib 
{	
	UINT CTrayIconController::RegisterTaskbarCreatedMsg()
	{
		return ::RegisterWindowMessage(TEXT("TaskbarCreated"));
	}
	CTrayIconController::CTrayIconController()
	{
		memset(&_nid,0,sizeof(_nid));
		_nid.cbSize = NOTIFYICONDATA_V1_SIZE;
		
		DWORD dwVer = CSys::GetDllVersion(_T("Shell32.dll"));  
		DWORD dwWinXP = MAKELONG(0,6);  
		DWORD dwWin2000 = MAKELONG(0,5);

		if(dwVer >= dwWin2000)
			_nid.cbSize = NOTIFYICONDATA_V2_SIZE;
	}

	CTrayIconController::~CTrayIconController()
	{
		Remove();
	}

	BOOL CTrayIconController::Setup(HWND hwnd, UINT uID, UINT uCBMsg, HICON hicon, TCHAR* tip)
	{
		assert(!_nid.hWnd);
		if(_nid.hWnd) return FALSE;

		_nid.hWnd     = hwnd;
		_nid.hIcon    = hicon;
		_nid.uFlags   = NIF_ICON | NIF_TIP | NIF_MESSAGE; 
		_nid.uID      = uID; 
		_nid.uCallbackMessage = uCBMsg;
		lstrcpyn(_nid.szTip,tip,63);
	//	wcsncpy_s(_nid.szTip,tip,63);
		//::SHChangeNotify(SHCNE_ALLEVENTS, SHCNF_FLUSH, NULL, NULL);
		return Shell_NotifyIcon(NIM_ADD, &_nid);
	}
	BOOL CTrayIconController::SetIcon(HICON hicon)
	{
//		assert(hicon);
		_nid.uFlags  = NIF_ICON;
		_nid.hIcon   = hicon;

		return Shell_NotifyIcon(NIM_MODIFY,&_nid);
	}

	BOOL CTrayIconController::SetTip(TCHAR* tip)
	{
		_nid.uFlags   =  NIF_TIP;
		lstrcpyn(_nid.szTip,tip,63);
		//wcsncpy_s(_nid.szTip,tip,63);
		return Shell_NotifyIcon(NIM_MODIFY,&_nid);
	}
	BOOL CTrayIconController::Remove()
	{
		BOOL bRet = Shell_NotifyIcon( NIM_DELETE, &_nid);
		_nid.hWnd = NULL;

		return bRet;
	}
	BOOL CTrayIconController::ShowToolTip(LPCTSTR szMsg,LPCTSTR szTitle,DWORD dwInfoFlags /* = NIIF_INFO */,UINT uTimeout /* = 1000*2 */)
	{
		_nid.uFlags         = NIF_INFO;
		_nid.dwInfoFlags    = dwInfoFlags;
		_nid.uTimeout       = uTimeout;

		lstrcpyn(_nid.szInfoTitle,szTitle,63);
		lstrcpyn(_nid.szInfo,szMsg,255);

		return Shell_NotifyIcon(NIM_MODIFY,&_nid);
	}

	void CTrayIconController::StartFlash(HWND hwnd,UINT nID,UINT uTimeElapse /* = 500 */)
	{
		SetTimer(hwnd,nID,uTimeElapse,NULL);
	}
	void CTrayIconController::StopFlash(HWND hwnd,UINT nID)
	{
		KillTimer(hwnd,nID);
		SetIcon(LoadIcon(CPaintManagerUI::GetInstance(),MAKEINTRESOURCE(IDI_MAIN_ICON)));
	}
	void CTrayIconController::DoFlash()
	{
		static bool bFlag = true;
		if (bFlag)
		{
			bFlag = false;
			SetIcon(LoadIcon(CPaintManagerUI::GetInstance(),MAKEINTRESOURCE(IDI_ICON_TRAY_2)));
		}
		else
		{
			bFlag = true;
			SetIcon(LoadIcon(CPaintManagerUI::GetInstance(),MAKEINTRESOURCE(IDI_MAIN_ICON)));
		}

	}
}