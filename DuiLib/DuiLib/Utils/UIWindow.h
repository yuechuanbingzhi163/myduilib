#pragma once
#include "./dui/Core/UIBase.h"
namespace DuiLib {
	enum MsgBoxType{
		MBT_INFO = 1<<0,
		MBT_OK   = 1<<1,
		MBT_WARN = 1<<2,

		MBBT_OK = 1<<3,
		MBBT_CANCEL = 1<<4,

		MBBT_CHECKBOX = 1<<5,
	};
	enum MsgBoxRetT{
		MBRT_CANCEL = 0,
		MBRT_OK,
		MBRT_OKCHECKED,
	};
	UILIB_API int MsgBox(
		HWND hwnd,
		LPCTSTR lpszText,
		LPCTSTR lpszCaption,
		UINT nType);

/************************************************************************/
/*                                                                      */
/************************************************************************/	
	class UILIB_API CWin  
		: public CWindowWnd
	{
	public:
		CWin();
		virtual ~CWin();

		LPCTSTR GetWindowClassName() const;	

		DWORD GetStyle() const;
		DWORD GetExStyle() const;
		BOOL ModifyStyle(DWORD dwRemove,DWORD dwAdd,UINT nFlags = 0);
		BOOL ModifyStyleEx(DWORD dwRemove,DWORD dwAdd,UINT nFlags = 0);

		BOOL DestroyWindow();

		BOOL MoveWindow(
			int x,
			int y,
			int nWidth,
			int nHeight,
			BOOL bRepaint = TRUE);

		BOOL MoveWindow(
			LPCRECT lpRect,
			BOOL bRepaint = TRUE);

		BOOL MoveWindowToPoint(LPPOINT lpPoint);

		BOOL SetWindowPos(
			HWND hWndInsertAfter,
			int x,
			int y,
			int cx,
			int cy,
			UINT nFlags);

		BOOL SetWindowPos(
			HWND hWndInsertAfter,
			LPCRECT lpRect,
			UINT nFlags);

		UINT ArrangeIconicWindows();

		BOOL ForegroundWindow();
		BOOL BringWindowToTop();

		BOOL GetWindowRect( LPRECT lpRect) const;
		BOOL GetClientRect( LPRECT lpRect) const;

		BOOL ClientToScreen( LPPOINT lpPoint) const;
		BOOL ClientToScreen( LPRECT lpRect) const;
		BOOL ScreenToClient( LPPOINT lpPoint) const;
		BOOL ScreenToClient( LPRECT lpRect) const;
		int MapWindowPoints(
			HWND hWndTo,
			LPPOINT lpPoint,
			UINT nCount) const;

		int MapWindowPoints(
			HWND hWndTo,
			LPRECT lpRect) const; 

		BOOL UpdateWindow();

		void SetRedraw(BOOL bRedraw = TRUE);
		BOOL Invalidate(BOOL bErase = TRUE);
		BOOL InvalidateRect(
			LPCRECT lpRect,
			BOOL bErase = TRUE);
		BOOL ValidateRect(LPCRECT lpRect);
		void InvalidateRgn(
			HRGN hRgn,
			BOOL bErase = TRUE);

		BOOL ValidateRgn(HRGN hRgn);

		BOOL IsWindow() const;
		BOOL IsIconic() const;
		BOOL IsZoomed() const;
		BOOL IsWindowEnabled() const;
		BOOL EnableWindow(BOOL bEnable = TRUE);
		BOOL IsWindowVisible() const;
		BOOL IsWindowUnicode() const;
		BOOL IsParentDialog();

		UINT_PTR SetTimer(
			UINT_PTR nIDEvent,
			UINT nElapse,
			void (CALLBACK* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD) = NULL); 
		BOOL KillTimer(UINT_PTR nIDEvent);

		HWND SetActiveWindow();
		HWND SetCapture();
		HWND SetFocus();

		BOOL IsChild(HWND hWnd);
		HWND GetTopLevelWindow() const; 
		HWND GetTopLevelParent() const;
		HWND GetParent() const;
		HWND SetParent(HWND hWndNewParent);

		BOOL FlashWindow(BOOL bInvert);
		int MsgBox(
			LPCTSTR lpszText,
			LPCTSTR lpszCaption = _T(""),
			UINT nType = MBT_INFO|MBBT_OK|MBBT_CANCEL);

		int SetHotKey(
			WORD wVirtualKeyCode,
			WORD wModifiers);
		DWORD GetHotKey() const;

		BOOL IsMainThread() const;
		DWORD GetWindowThreadID();
		DWORD GetWindowProcessID();

		HFONT GetFont() const;
		void SetFont(
			HFONT hFont,
			BOOL bRedraw = TRUE);

		BOOL SetWindowText(LPCTSTR lpszString);
		int GetWindowText(
			LPTSTR lpszStringBuf,
			int nMaxCount);
		int GetWindowTextLength() const;

		HICON LoadIcon(UINT nid) const;
		HICON LoadIcon(LPCTSTR lpIconName) const;

		HICON SetIcon(HICON hIcon,BOOL bBigIcon = TRUE);
		HICON GetIcon(BOOL bBigIcon = TRUE) const;
	public:
		bool RegisterSkin(STRINGorID xml, 
			              LPCTSTR type = NULL,
						  IDialogBuilderCallback* pCallback = NULL,
						  CControlUI* pParent = NULL);
		void UnRegisterSkin();

		UINT_PTR GetTag() const;
		void SetTag(UINT_PTR pTag);
	protected:
		CPaintManagerUI* GetPaintMgr() const;
		
		virtual BOOL HandleMsg(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& lResult);
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0);
	private:
		CPaintManagerUI*  _paintManager;
		UINT_PTR          _pTag;
	};	
}