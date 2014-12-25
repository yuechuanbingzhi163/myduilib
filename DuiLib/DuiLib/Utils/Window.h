#pragma once

namespace DuiLib {
	enum MsgBoxType{
		MBT_INFO = 1<<0,
		MBT_OK   = 1<<1,
		MBT_WARN = 1<<2,

		MBT_SHAKE = 1<<3,

		MBBT_OK = 1<<4,
		MBBT_CANCEL = 1<<5,
		MBBT_CHECKBOX = 1<<6,
		
		MBBT_OKCANCLE = MBBT_OK|MBBT_CANCEL,
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

		virtual LPCTSTR GetWindowClassName() const;	
		virtual UINT GetClassStyle() const;
		virtual void OnFinalMessage( HWND hWnd );

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
		//新修改 2013-6-27 16:59
		//动画显示窗体
		BOOL AnimateWindow(DWORD dwTime,DWORD dwFlags = 0x00000010/*AW_CENTER*/);

		//淡入淡出
		BOOL FadeInOut(DWORD dwTime,DWORD dwFlags = 0x00080000/*AW_BLEND*/);

		//抖动窗口
		BOOL ShakeWindow(int iTimes = 3);

		template<typename T>
		inline void FindControl(T& pctrl,LPCTSTR pstrName) const
		{
			pctrl = static_cast<T>(_paintManager->FindControl(pstrName));
		}

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
		
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#if defined(WIN32) && !defined(UNDER_CE)
		virtual LRESULT OnNCMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnNCMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif
		virtual LRESULT OnEraseBkGnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnMenuCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
// 		virtual BOOL HandleMsg(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& lResult);
// 		
// 		virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0);
	private:
		CPaintManagerUI*  _paintManager;
		UINT_PTR          _pTag;
	};	
}