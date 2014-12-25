#include "stdafx.h"
#include "Window.h"
#include <assert.h>

namespace DuiLib {
	CWin::CWin():_paintManager(NULL),_pTag(NULL)
	{

	}

	CWin::~CWin()
	{
		UnRegisterSkin();
		if(IsWindow())
			DestroyWindow();
	}
	LPCTSTR CWin::GetWindowClassName() const
	{
		return _T("BaseWindow");
	}
	UINT CWin::GetClassStyle() const
	{
		return CS_DBLCLKS;
	}
	void CWin::OnFinalMessage( HWND hWnd )
	{
		UnRegisterSkin();
		__super::OnFinalMessage(hWnd);	
	}

	DWORD CWin::GetStyle() const
	{
		assert(::IsWindow(m_hWnd));
		return (DWORD)::GetWindowLong(m_hWnd, GWL_STYLE);
	}
	DWORD CWin::GetExStyle() const
	{
		assert(::IsWindow(m_hWnd));
		return (DWORD)::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	}
	BOOL CWin::ModifyStyle(DWORD dwRemove,DWORD dwAdd,UINT nFlags/* = 0*/)
	{
		assert(::IsWindow(m_hWnd));

		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if(dwStyle == dwNewStyle)
			return FALSE;

		::SetWindowLong(m_hWnd, GWL_STYLE, dwNewStyle);
		if(nFlags != 0)
		{
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}

		return TRUE;
	}
	BOOL CWin::ModifyStyleEx(DWORD dwRemove,DWORD dwAdd,UINT nFlags/* = 0*/)
	{
		assert(::IsWindow(m_hWnd));

		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if(dwStyle == dwNewStyle)
			return FALSE;

		::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewStyle);
		if(nFlags != 0)
		{
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}

		return TRUE;
	}

	
	BOOL CWin::DestroyWindow()
	{
		assert(::IsWindow(m_hWnd));

		if(!::DestroyWindow(m_hWnd))
			return FALSE;

		m_hWnd = NULL;
		return TRUE;
	}

	BOOL CWin::MoveWindow(
		int x,
		int y,
		int nWidth,
		int nHeight,
		BOOL bRepaint /*= TRUE*/)
	{
		assert(::IsWindow(m_hWnd));
		return ::MoveWindow(m_hWnd, x, y, nWidth, nHeight, bRepaint);
	}

	BOOL CWin::MoveWindow(
		LPCRECT lpRect,
		BOOL bRepaint/* = TRUE*/)
	{
		assert(::IsWindow(m_hWnd));
		return ::MoveWindow(m_hWnd, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint);
	}

	BOOL CWin::MoveWindowToPoint(LPPOINT lpPoint)
	{
		assert(lpPoint);
		assert(::IsWindow(m_hWnd));

		RECT rcWnd;
		GetWindowRect(&rcWnd);
		const int nWidth = rcWnd.right - rcWnd.left;
		const int nHeight = rcWnd.bottom - rcWnd.top;
		rcWnd.left = lpPoint->x;
		rcWnd.top = lpPoint->y;
		rcWnd.right = rcWnd.left + nWidth;
		rcWnd.bottom = rcWnd.top + nHeight;
		MONITORINFO oMonitor = {0};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;

		if( rcWnd.bottom > rcWork.bottom ) {
			if( nHeight >= rcWork.GetHeight() ) {
				rcWnd.top = 0;
				rcWnd.bottom = nHeight;
			}
			else {
				rcWnd.bottom = lpPoint->y;//rcWork.bottom;
				rcWnd.top = rcWnd.bottom - nHeight;
			}
		}
		if( rcWnd.right > rcWork.right ) {
			if( nWidth >= rcWork.GetWidth() ) {
				rcWnd.left = 0;
				rcWnd.right = nWidth;
			}
			else {
				rcWnd.right = lpPoint->x;//rcWork.right;
				rcWnd.left = rcWnd.right - nWidth;
			}
		}
		return SetWindowPos(NULL,&rcWnd,SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	BOOL CWin::SetWindowPos(
		HWND hWndInsertAfter,
		int x,
		int y,
		int cx,
		int cy,
		UINT nFlags)
	{
		assert(::IsWindow(m_hWnd));
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, x, y, cx, cy, nFlags);
	}

	BOOL CWin::SetWindowPos(
		HWND hWndInsertAfter,
		LPCRECT lpRect,
		UINT nFlags)
	{
		assert(::IsWindow(m_hWnd));
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, nFlags);
	}

	UINT CWin::ArrangeIconicWindows()
	{
		assert(::IsWindow(m_hWnd));
		return ::ArrangeIconicWindows(m_hWnd);
	}

	BOOL CWin::ForegroundWindow()
	{
		assert(::IsWindow(m_hWnd));
		return ::SetForegroundWindow(m_hWnd);
	}

	BOOL CWin::BringWindowToTop()
	{
		assert(::IsWindow(m_hWnd));
		return ::BringWindowToTop(m_hWnd);
	}

	BOOL CWin::GetWindowRect( LPRECT lpRect) const
	{
		assert(::IsWindow(m_hWnd));
		return ::GetWindowRect(m_hWnd, lpRect);
	}
	BOOL CWin::GetClientRect( LPRECT lpRect) const
	{
		assert(::IsWindow(m_hWnd));
		return ::GetClientRect(m_hWnd, lpRect);
	}

	BOOL CWin::ClientToScreen( LPPOINT lpPoint) const
	{
		assert(::IsWindow(m_hWnd));
		return ::ClientToScreen(m_hWnd, lpPoint);
	}
	BOOL CWin::ClientToScreen( LPRECT lpRect) const
	{
		assert(::IsWindow(m_hWnd));
		if(!::ClientToScreen(m_hWnd, (LPPOINT)lpRect))
			return FALSE;
		return ::ClientToScreen(m_hWnd, ((LPPOINT)lpRect)+1);
	}
	BOOL CWin::ScreenToClient( LPPOINT lpPoint) const
	{
		assert(::IsWindow(m_hWnd));
		return ::ScreenToClient(m_hWnd, lpPoint);
	}
	BOOL CWin::ScreenToClient( LPRECT lpRect) const
	{
		assert(::IsWindow(m_hWnd));
		if(!::ScreenToClient(m_hWnd, (LPPOINT)lpRect))
			return FALSE;
		return ::ScreenToClient(m_hWnd, ((LPPOINT)lpRect)+1);
	}
	int CWin::MapWindowPoints(
		HWND hWndTo,
		LPPOINT lpPoint,
		UINT nCount) const
	{
		assert(::IsWindow(m_hWnd));
		return ::MapWindowPoints(m_hWnd, hWndTo, lpPoint, nCount);
	}

	int CWin::MapWindowPoints(
		HWND hWndTo,
		LPRECT lpRect) const
	{
		assert(::IsWindow(m_hWnd));
		return ::MapWindowPoints(m_hWnd, hWndTo, (LPPOINT)lpRect, 2);
	}

	BOOL CWin::UpdateWindow()
	{
		assert(::IsWindow(m_hWnd));
		return ::UpdateWindow(m_hWnd);
	}

	void CWin::SetRedraw(BOOL bRedraw /*= TRUE*/)
	{
		assert(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_SETREDRAW, (WPARAM)bRedraw, 0);
	}
	BOOL CWin::Invalidate(BOOL bErase /*= TRUE*/)
	{
		assert(::IsWindow(m_hWnd));
		return ::InvalidateRect(m_hWnd, NULL, bErase);
	}
	BOOL CWin::InvalidateRect(
		LPCRECT lpRect,
		BOOL bErase/* = TRUE*/)
	{
		assert(::IsWindow(m_hWnd));
		return ::InvalidateRect(m_hWnd, lpRect, bErase);
	}
	BOOL CWin::ValidateRect(LPCRECT lpRect)
	{
		assert(::IsWindow(m_hWnd));
		return ::ValidateRect(m_hWnd, lpRect);
	}
	void CWin::InvalidateRgn(
		HRGN hRgn,
		BOOL bErase/* = TRUE*/)
	{
		assert(::IsWindow(m_hWnd));
		::InvalidateRgn(m_hWnd, hRgn, bErase);
	}

	BOOL CWin::ValidateRgn(HRGN hRgn)
	{
		assert(::IsWindow(m_hWnd));
		return ::ValidateRgn(m_hWnd, hRgn);
	}

	BOOL CWin::IsIconic() const
	{
		assert(::IsWindow(m_hWnd));
		return ::IsIconic(m_hWnd);
	}
	BOOL CWin::IsZoomed() const
	{
		assert(::IsWindow(m_hWnd));
		return ::IsZoomed(m_hWnd);
	}

	BOOL CWin::IsWindowEnabled() const
	{
		assert(::IsWindow(m_hWnd));
		return ::IsWindowEnabled(m_hWnd);
	}
	BOOL CWin::EnableWindow(BOOL bEnable/* = TRUE*/)
	{
		assert(::IsWindow(m_hWnd));
		return ::EnableWindow(m_hWnd, bEnable);
	}
	BOOL CWin::IsWindowVisible() const
	{
		assert(::IsWindow(m_hWnd));
		return ::IsWindowVisible(m_hWnd);
	}
	BOOL CWin::IsWindow() const
	{
		return ::IsWindow(m_hWnd);
	}
	BOOL CWin::IsWindowUnicode() const
	{
		assert(::IsWindow(m_hWnd));
		return ::IsWindowUnicode(m_hWnd);
	}
	BOOL CWin::IsParentDialog()
	{
		assert(::IsWindow(m_hWnd));
		TCHAR szBuf[8]; // "#32770" + NUL character
		if (GetClassName(GetParent(), szBuf, sizeof(szBuf)/sizeof(szBuf[0])) == 0)
			return FALSE;
		return lstrcmp(szBuf, _T("#32770")) == 0;
	}

	UINT_PTR CWin::SetTimer(
		UINT_PTR nIDEvent,
		UINT nElapse,
		void (CALLBACK* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD) /*= NULL*/)
	{
		assert(::IsWindow(m_hWnd));
		return ::SetTimer(m_hWnd, nIDEvent, nElapse, (TIMERPROC)lpfnTimer);
	}
	BOOL CWin::KillTimer(UINT_PTR nIDEvent)
	{
		assert(::IsWindow(m_hWnd));
		return ::KillTimer(m_hWnd, nIDEvent);
	}

	HWND CWin::SetActiveWindow()
	{
		assert(::IsWindow(m_hWnd));
		return ::SetActiveWindow(m_hWnd);
	}
	HWND CWin::SetCapture()
	{
		assert(::IsWindow(m_hWnd));
		return ::SetCapture(m_hWnd);
	}
	HWND CWin::SetFocus()
	{
		assert(::IsWindow(m_hWnd));
		return ::SetFocus(m_hWnd);
	}

	BOOL CWin::IsChild(HWND hWnd)
	{
		assert(::IsWindow(m_hWnd));
		return ::IsChild(m_hWnd, hWnd);
	}
	HWND CWin::GetTopLevelWindow() const 
	{
		assert(::IsWindow(m_hWnd));

		HWND hWndParent;
		HWND hWndTmp = m_hWnd;

		do
		{
			hWndParent = hWndTmp;
			hWndTmp = (::GetWindowLong(hWndParent, GWL_STYLE) & WS_CHILD) ? ::GetParent(hWndParent) : ::GetWindow(hWndParent, GW_OWNER);
		}
		while(hWndTmp != NULL);

		return (hWndParent);
	}
	HWND CWin::GetTopLevelParent() const
	{
		assert(::IsWindow(m_hWnd));

		HWND hWndParent = m_hWnd;
		HWND hWndTmp;
		while((hWndTmp = ::GetParent(hWndParent)) != NULL)
			hWndParent = hWndTmp;

		return (hWndParent);
	}
	HWND CWin::GetParent() const
	{
		assert(::IsWindow(m_hWnd));
		return (::GetParent(m_hWnd));
	}
	HWND CWin::SetParent(HWND hWndNewParent)
	{
		assert(::IsWindow(m_hWnd));
		return (::SetParent(m_hWnd, hWndNewParent));
	}

	BOOL CWin::FlashWindow(BOOL bInvert)
	{
		assert(::IsWindow(m_hWnd));
		return ::FlashWindow(m_hWnd, bInvert);
	}

	BOOL CWin::AnimateWindow(DWORD dwTime,DWORD dwFlags/* = 0x00000010*/)
	{
		assert(::IsWindow(m_hWnd));

		HINSTANCE hInst = LoadLibrary(_T("User32.DLL")); 
		if(hInst)			
		{
			typedef BOOL(WINAPI MYFUNC(HWND,DWORD,DWORD));
			MYFUNC* AnimateWindow = NULL;
			AnimateWindow = (MYFUNC *)::GetProcAddress(hInst,"AnimateWindow");
			if(AnimateWindow)
				AnimateWindow(m_hWnd,dwTime,dwFlags);
			FreeLibrary(hInst);
			return TRUE;
		}
		return FALSE;
	}

	BOOL CWin::FadeInOut(DWORD dwTime,DWORD dwFlags/* = 0x00080000*/)
	{
		assert(::IsWindow(m_hWnd));

		HINSTANCE hInst = LoadLibrary(_T("User32.DLL")); 
		if(hInst)
		{
			typedef BOOL(WINAPI FadeWindow(HWND,DWORD,DWORD));
			FadeWindow* AnimateWindow = NULL;
			AnimateWindow = (FadeWindow *)::GetProcAddress(hInst,"AnimateWindow");
			if(AnimateWindow)
				AnimateWindow(m_hWnd,dwTime,dwFlags);
			FreeLibrary(hInst);	
			return TRUE;
		}
		return FALSE;
	}

	BOOL CWin::ShakeWindow(int iTimes /* = 3 */)
	{
		assert(::IsWindow(m_hWnd));

		int ty = 3;
		RECT   m_rect; 
		::ShowWindow(m_hWnd,SW_SHOW);

		::SetForegroundWindow(m_hWnd );
		::GetWindowRect(m_hWnd,&m_rect);  
		int recordy = m_rect.left;
		int recordx = m_rect.top;

		for(int i=0;i<iTimes;i++)
		{
			m_rect.left = recordy;
			m_rect.top = recordx;
			m_rect.top  = m_rect.top + ty;  
			m_rect.left = m_rect.left - ty;
			::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
			Sleep(20);
			m_rect.top = m_rect.top -ty;
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
			Sleep(20);
			m_rect.top = m_rect.top -ty;
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
			Sleep(20);
			m_rect.left=m_rect.left+ty;
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
			Sleep(20);
			m_rect.left=m_rect.left+ty;
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
			Sleep(20);
			m_rect.top = m_rect.top + ty;  
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
			Sleep(20);
			m_rect.top=m_rect.top+ty;
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
			Sleep(20);
			m_rect.top=m_rect.top+ty;
			::SetWindowPos( m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
			Sleep(20);
			::SetWindowPos( m_hWnd,NULL,recordy,recordx,0,0,SWP_NOSIZE);
			Sleep(3);
		}
		return TRUE;
	}

	int CWin::MsgBox(
		LPCTSTR lpszText,
		LPCTSTR lpszCaption /*= _T("")*/,
		UINT nType/* = MB_OK*/)
	{
		return DuiLib::MsgBox(m_hWnd,lpszText,lpszCaption,nType);
	}

	int CWin::SetHotKey(
		WORD wVirtualKeyCode,
		WORD wModifiers)
	{
		assert(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, WM_SETHOTKEY, MAKEWORD(wVirtualKeyCode, wModifiers), 0);
	}
	DWORD CWin::GetHotKey() const
	{
		assert(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, WM_GETHOTKEY, 0, 0);
	}

	BOOL CWin::IsMainThread() const
	{
		assert(::IsWindow(m_hWnd));
		return (::GetCurrentThreadId() ==  ::GetWindowThreadProcessId(m_hWnd, NULL));
	}
	DWORD CWin::GetWindowThreadID()
	{
		assert(::IsWindow(m_hWnd));
		return ::GetWindowThreadProcessId(m_hWnd, NULL);
	}
	DWORD CWin::GetWindowProcessID()
	{
		assert(::IsWindow(m_hWnd));
		DWORD dwProcessID;
		::GetWindowThreadProcessId(m_hWnd, &dwProcessID);
		return dwProcessID;
	}

	HFONT CWin::GetFont() const
	{
		assert(::IsWindow(m_hWnd));
		return (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0);
	}
	void CWin::SetFont(
		HFONT hFont,
		BOOL bRedraw /*= TRUE*/)
	{
		assert(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(bRedraw, 0));
	}

	BOOL CWin::SetWindowText(LPCTSTR lpszString)
	{
		assert(::IsWindow(m_hWnd));
		return ::SetWindowText(m_hWnd, lpszString);
	}
	int CWin::GetWindowText(
		LPTSTR lpszStringBuf,
		int nMaxCount)
	{
		assert(::IsWindow(m_hWnd));
		return ::GetWindowText(m_hWnd, lpszStringBuf, nMaxCount);
	}
	int CWin::GetWindowTextLength() const
	{
		assert(::IsWindow(m_hWnd));
		return ::GetWindowTextLength(m_hWnd);
	}

	HICON CWin::LoadIcon(UINT nid) const
	{
		return LoadIcon(MAKEINTRESOURCE(nid));
	}

	HICON CWin::LoadIcon(LPCTSTR lpIconName) const
	{
		return ::LoadIcon(CPaintManagerUI::GetInstance(),lpIconName);
	}

	HICON CWin::SetIcon(HICON hIcon,BOOL bBigIcon /*= TRUE*/)
	{
		assert(::IsWindow(m_hWnd));
		return (HICON)::SendMessage(m_hWnd, WM_SETICON, bBigIcon, (LPARAM)hIcon);
	}
	HICON CWin::GetIcon(BOOL bBigIcon/* = TRUE*/) const
	{
		assert(::IsWindow(m_hWnd));
		return (HICON)::SendMessage(m_hWnd, WM_GETICON, bBigIcon, 0);
	}

	bool CWin::RegisterSkin(STRINGorID xml, LPCTSTR type /* = NULL */, IDialogBuilderCallback* pCallback /* = NULL */, CControlUI* pParent /* = NULL */)
	{
		assert(IsWindow());
		assert(IsMainThread());

		UnRegisterSkin();

		_paintManager = new CPaintManagerUI();
		_paintManager->Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(xml,type,pCallback,_paintManager,pParent);
		ASSERT(pRoot && "Failed to parse XML");
		if(pRoot)
			return _paintManager->AttachDialog(pRoot);
		return false;
	}
	void CWin::UnRegisterSkin()
	{
		if(_paintManager)
		{
			delete _paintManager;
			_paintManager = NULL;
		}
	}
	UINT_PTR CWin::GetTag() const
	{
		return _pTag;
	}
	void CWin::SetTag(UINT_PTR pTag)
	{
		_pTag = pTag;
	}
	CPaintManagerUI* CWin::GetPaintMgr() const
	{
		return _paintManager;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////
	LRESULT CWin::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWin::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

#if defined(WIN32) && !defined(UNDER_CE)
	LRESULT CWin::OnNCMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
//		bHandled = FALSE;
		return 0;
	}
	LRESULT CWin::OnNCMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
//		bHandled = FALSE;
		return 0;
	}
	LRESULT CWin::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(IsIconic()) bHandled = FALSE;
		return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT CWin::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 	LPRECT pRect=NULL;
		// 
		// 	if ( wParam == TRUE)
		// 	{
		// 		LPNCCALCSIZE_PARAMS pParam = (LPNCCALCSIZE_PARAMS)lParam;
		// 		pRect=&pParam->rgrc[0];
		// 	}
		// 	else
		// 	{
		// 		pRect=(LPRECT)lParam;
		// 	}
		// 
		// 	if ( ::IsZoomed(m_hWnd))
		// 	{	// 最大化时，计算当前显示器最适合宽高度
		// 		MONITORINFO oMonitor = {};
		// 		oMonitor.cbSize = sizeof(oMonitor);
		// 		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
		// 		CDuiRect rcWork = oMonitor.rcWork;
		// 		CDuiRect rcMonitor = oMonitor.rcMonitor;
		// 		rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);
		// 
		// 		pRect->right = pRect->left + rcWork.GetWidth();
		// 		pRect->bottom = pRect->top + rcWork.GetHeight();
		// 
		// 		return WVR_REDRAW;
		// 	}

		return 0;
	}

	LRESULT CWin::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 0;
	}

	LRESULT CWin::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(m_hWnd, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

		if( !::IsZoomed(*this) ) {
			RECT rcSizeBox = GetPaintMgr()->GetSizeBox();
			if( pt.y < rcClient.top + rcSizeBox.top ) {
				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
				return HTTOP;
			}
			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
				return HTBOTTOM;
			}
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
		}

		RECT rcCaption = GetPaintMgr()->GetCaptionRect();
		//modify by dfn.li 2013-7-12 21:09
		RECT rcBottomCaption = GetPaintMgr()->GetBottomCaptionRect();
		bool bCaption = false;
		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
			bCaption = true;
		bool bBottomCaption = false;
		if ( pt.x >= rcClient.left + rcBottomCaption.left && pt.x < rcClient.right - rcBottomCaption.right \
			&& pt.y >= rcClient.bottom - rcBottomCaption.bottom && pt.y < rcClient.bottom )
			bBottomCaption = true;

		if( bCaption||bBottomCaption ) {
				CControlUI* pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(pt));
				if (pControl)
				{
					if (pControl->IsClass(CControlUI::GetClassName()) ||
						pControl->IsClass(CChildLayoutUI::GetClassName()) ||
						pControl->IsClass(CHorizontalLayoutUI::GetClassName())||
						pControl->IsClass(CTabLayoutUI::GetClassName()) ||
						pControl->IsClass(CTileLayoutUI::GetClassName()) ||
						pControl->IsClass(CVerticalLayoutUI::GetClassName()) ||
						pControl->IsClass(CTextUI::GetClassName()) ||
						pControl->IsClass(CLabelUI::GetClassName()))
					{
						return HTCAPTION;
					}
					return HTCLIENT;
				}
				return HTCAPTION;	
		}
	
		return HTCLIENT;
	}

	LRESULT CWin::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

		LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
		lpMMI->ptMaxPosition.x	= rcWork.left;
		lpMMI->ptMaxPosition.y	= rcWork.top;
		lpMMI->ptMaxSize.x		= rcWork.right;
		lpMMI->ptMaxSize.y		= rcWork.bottom;

		bHandled = FALSE;
		return 0;
	}

	LRESULT CWin::OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWin::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
#endif

	LRESULT CWin::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SIZE szRoundCorner = GetPaintMgr()->GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
		if( !IsIconic() && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
			CDuiRect rcWnd;
			::GetWindowRect(m_hWnd, &rcWnd);
			rcWnd.Offset(-rcWnd.left, -rcWnd.top);
			rcWnd.right++; rcWnd.bottom++;
			HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
			::SetWindowRgn(m_hWnd, hRgn, TRUE);
			::DeleteObject(hRgn);
		}
#endif
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWin::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWin::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
// 		if (GET_SC_WPARAM(wParam) == SC_CLOSE)
// 		{
// 			PostMessage(WM_CLOSE);
// 			return TRUE;
// 		}
// 		CWindowWnd::HandleMessage(uMsg,wParam,lParam);
// 		return TRUE;
		if (wParam == SC_CLOSE)
		{
			bHandled = TRUE;
			PostMessage(WM_CLOSE);
			return 0;
		}
#if defined(WIN32) && !defined(UNDER_CE)
		BOOL bZoomed = IsZoomed();
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( IsZoomed()!= bZoomed )
		{
		}
#else
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
		return lRes;
	}
	LRESULT CWin::OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWin::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWin::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWin::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWin::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CWin::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT CWin::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT CWin::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT CWin::OnEraseBkGnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		return 1;
	}
	LRESULT CWin::OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT CWin::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		return 0;
	}
	LRESULT CWin::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT CWin::OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT CWin::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT CWin::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT CWin::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		PAINTSTRUCT ps = {0};
		::BeginPaint(m_hWnd,&ps);
		::EndPaint(m_hWnd,&ps);
		return 0;
	}
	LRESULT CWin::OnMenuCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT CWin::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch (uMsg)
		{
		case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
#if defined(WIN32) && !defined(UNDER_CE)
		case WM_NCMOUSEMOVE:    lRes = OnNCMouseMove(uMsg, wParam, lParam, bHandled); break;
		case WM_NCMOUSELEAVE:   lRes = OnNCMouseLeave(uMsg, wParam, lParam, bHandled); break;
		case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_MOUSEWHEEL:		lRes = OnMouseWheel(uMsg, wParam, lParam, bHandled); break;
#endif
		case WM_ERASEBKGND:     lRes = OnEraseBkGnd(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_CHAR:		    lRes = OnChar(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_KEYDOWN:		lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
		case WM_KILLFOCUS:		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
		case WM_SETFOCUS:		lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
		case WM_LBUTTONUP:		lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
		case WM_LBUTTONDOWN:	lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
		case WM_MOUSEMOVE:		lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
		case WM_MOUSELEAVE:     lRes = OnMouseLeave(uMsg, wParam, lParam, bHandled); break;
		case WM_MOUSEHOVER:	    lRes = OnMouseHover(uMsg, wParam, lParam, bHandled); break;
		case WM_HOTKEY:			lRes = OnHotKey(uMsg, wParam, lParam, bHandled); break;
//		case WM_TIMER:			lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;//OnTimer响应有小问题
		case WM_MOVING:			lRes = OnMoving(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZING:			lRes = OnSizing(uMsg, wParam, lParam, bHandled); break;
		case WM_SHOWWINDOW:		lRes = OnShowWindow(uMsg, wParam, lParam, bHandled); break;
		case WM_COMMAND:		lRes = OnCommand(uMsg, wParam, lParam, bHandled); break;
//		case WM_PAINT:			lRes = OnPaint(uMsg, wParam, lParam, bHandled); break;//OnPaint响应有问题
		case WM_MENUCOMMAND:    lRes = OnMenuCommand(uMsg, wParam, lParam, bHandled); break;
		default:				bHandled = FALSE; break;
		}
		if (bHandled) return lRes;

		lRes = ProcessWindowMessage(uMsg, wParam, lParam, bHandled);
		if (bHandled) return lRes;

		if(_paintManager && _paintManager->MessageHandler(uMsg,wParam,lParam,lRes))
			return lRes;
		return __super::HandleMessage(uMsg,wParam,lParam);
	}

	LRESULT CWin::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = 0;
		
		return lRes;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	class CMsgBoxWin : public CWin,public INotifyUI
	{
	public:
		CMsgBoxWin(){}
		~CMsgBoxWin(){}
		void OnFinalMessage( HWND hWnd )
		{
			__super::OnFinalMessage(hWnd);
//			delete this;
		}
		int ShowMsgBox(HWND hwnd,
			LPCTSTR lpszText,
			LPCTSTR lpszCaption,
			UINT nType)
		{
			if(!IsWindow())
			{
				//	this->Create(hwnd,lpszCaption,UI_WNDSTYLE_DIALOG,UI_WNDSTYLE_EX_DIALOG);
				this->Create(hwnd,lpszCaption,UI_WNDSTYLE_DIALOG,0);
			}

			CenterWindow();

			CTextUI* pTitle       = static_cast<CTextUI*>(GetPaintMgr()->FindControl(_T("title")));
			CTextUI* pMsg         = static_cast<CTextUI*>(GetPaintMgr()->FindControl(_T("msg")));
			CButtonUI* pCancelBtn = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("cancel")));
			CButtonUI* pOkBtn     = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("yes")));
			CTabLayoutUI* pTable  = static_cast<CTabLayoutUI*>(GetPaintMgr()->FindControl(_T("icon")));
			CCheckBoxUI* pCheckBox= static_cast<CCheckBoxUI*>(GetPaintMgr()->FindControl(_T("checkbox")));

			ASSERT(pTitle);
			ASSERT(pMsg);
			ASSERT(pCancelBtn);
			ASSERT(pOkBtn);
			ASSERT(pTable);
			ASSERT(pCheckBox);

			if(pTitle) pTitle->SetText((lpszCaption ? lpszCaption : _T("")));
			if(pMsg)   pMsg->SetText((lpszText ? lpszText : _T("")));
			if(pCancelBtn) pCancelBtn->SetFocus();

			if(pTable)
			{
				if(nType & MBT_INFO)
					pTable->SelectItem(0);
				else if(nType & MBT_OK)
					pTable->SelectItem(1);
				else if(nType & MBT_WARN)
					pTable->SelectItem(2);
			}

			const bool isShowCancel   = (pCancelBtn && ((nType & MBBT_CANCEL) > 0));
			const bool isShowOk       = (pOkBtn && ((nType & MBBT_OK) > 0));
			const bool isShowCheckBox = (pCheckBox &&((nType & MBBT_CHECKBOX) > 0));
			const bool isShake = ((nType & MBT_SHAKE) > 0);
			_bShakeWindow = isShake;

			pCancelBtn->SetVisible(isShowCancel);
			pOkBtn->SetVisible(isShowOk);
			pCheckBox->SetVisible((/*isShowOk && */isShowCheckBox));
		
			if(!isShowCancel && isShowOk)
			{
				pCancelBtn->SetVisible(true);
				pOkBtn->SetVisible(false);
				pCancelBtn->SetName(pOkBtn->GetName());
				pCancelBtn->SetText(pOkBtn->GetText());
				pCancelBtn->SetToolTip(pOkBtn->GetToolTip());
				pCancelBtn->SetFocus();
			}
			return ShowModal();
		}
	protected:
		 virtual void Notify(TNotifyUI& msg)
		 {
			 //TRACE(_T("class:%s,Id:%s,event:%s"),msg.pSender->GetClass(),msg.pSender->GetName().GetData(),msg.sType.GetData()); 
			 if(0 != msg.sType.Compare(_T("click")))
				 return;

			  CControlUI * pSender = msg.pSender;
			 if(_tcscmp(pSender->GetName(),_T("closebtn"))==0 ||
				_tcscmp(pSender->GetName(),_T("cancel"))==0)
			 {
				 Close(MBRT_CANCEL);
			 }
			 else if(_tcscmp(pSender->GetName(),_T("yes"))==0 ||
				 _tcscmp(pSender->GetName(),_T("ok"))==0)
			 {
				 CCheckBoxUI* pCheckBox= static_cast<CCheckBoxUI*>(GetPaintMgr()->FindControl(_T("checkbox")));
				 ASSERT(pCheckBox);
				 pCheckBox->IsSelected() ? Close(MBRT_OKCHECKED) : Close(MBRT_OK);
			 }
		 }
		 LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		 {
			 if (WM_CREATE == uMsg)
			 {
				 _bShakeWindow = false;
				 ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
				 if(!RegisterSkin(_T("MsgBox.xml"),0))
					 return 0;

				 GetPaintMgr()->AddNotifier(this);
				 RECT rcClient;
				 ::GetClientRect(*this, &rcClient);
				 ::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
					 rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

				 return TRUE;
			 }
			 else if(WM_KEYDOWN == uMsg)
			 {
				 if(VK_ESCAPE == wParam)
				 {
					 Close(MBRT_CANCEL);
					 return TRUE;
				 }
				 else if(VK_RETURN == wParam)
				 {
					 if( GetPaintMgr()->FindControl(_T("ok")) || GetPaintMgr()->FindControl(_T("yes")) )
					 {
						 Close(MBRT_OK);
						 return TRUE;
					 }
					 else if( GetPaintMgr()->FindControl(_T("cancel")) || GetPaintMgr()->FindControl(_T("no")) )
					 {
						 Close(MBRT_CANCEL);
						 return TRUE;
					 }
				 }
			 }
			 else if(WM_SHOWWINDOW==uMsg)
			 {
				 if(_bShakeWindow)
					 ShakeWindow();

				 return 0;
			 }
			 return __super::ProcessWindowMessage(uMsg,wParam,lParam,bHandled);
		 }
	private:
		bool _bShakeWindow;
	};

	UILIB_API int MsgBox(
		HWND hwnd,
		LPCTSTR lpszText,
		LPCTSTR lpszCaption,
		UINT nType)
	{
		CMsgBoxWin win;
		return win.ShowMsgBox(hwnd,lpszText,lpszCaption,nType);
	}
}