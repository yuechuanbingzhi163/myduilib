#pragma once

#include <ShellAPI.h>
namespace DuiLib {
	class /*UILIB_API*/ CTrayIconController
	{
	public:
		CTrayIconController();
		~CTrayIconController();
	public:
		static UINT RegisterTaskbarCreatedMsg();

		BOOL Setup(HWND hwnd, UINT uID, UINT uCBMsg, HICON hicon, TCHAR* tip);
		BOOL Remove();
		BOOL SetIcon(HICON hicon);
		BOOL SetTip(TCHAR* tip);
		BOOL ShowToolTip(LPCTSTR szMsg,LPCTSTR szTitle,DWORD dwInfoFlags = NIIF_INFO,UINT uTimeout = 1000*3);

		void StartFlash(HWND hwnd,UINT nID,UINT uTimeElapse = 500);
		void StopFlash(HWND hwnd,UINT nID);
		void DoFlash();
	protected:
		NOTIFYICONDATA    _nid;
	};
}