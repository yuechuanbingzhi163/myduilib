#include "stdafx.h"
#include "xiamiWindow.h"
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
		return _T("XiamiWin");
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

	BOOL CWin::HandleMsg(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& lResult)
	{
		switch(uMsg)
		{
		case WM_NCHITTEST:
			{
				auto onNcHitTest = [&]()-> LRESULT const
				{
					CPaintManagerUI* pPaintMgrUI = GetPaintMgr();
					if (!pPaintMgrUI)
						return HTCLIENT;

					POINT pt;
					pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
					::ScreenToClient(hwnd, &pt);

					RECT rcClient;
					::GetClientRect(hwnd,&rcClient);

					if( !::IsZoomed(hwnd) ) {
						RECT rcSizeBox = pPaintMgrUI->GetSizeBox();
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

					RECT rcCaption = pPaintMgrUI->GetCaptionRect();
					if( pt.x >= rcClient.left + rcCaption.left &&
						pt.x < rcClient.right - rcCaption.right && 
						pt.y >= rcCaption.top &&
						pt.y < rcCaption.bottom ) 
					{
						CControlUI* pControl = static_cast<CControlUI*>(pPaintMgrUI->FindControl(pt));

						if(pControl)
						{
							//TRACE(_T("class:%s,id:%s"),pControl->GetClass(),pControl->GetName());
						}
						if(pControl)
						{
							if(pControl->IsClass(CChildLayoutUI::GetClassName()) ||
								pControl->IsClass(CHorizontalLayoutUI::GetClassName())||
								pControl->IsClass(CTabLayoutUI::GetClassName()) ||
								pControl->IsClass(CTileLayoutUI::GetClassName()) ||
								pControl->IsClass(CVerticalLayoutUI::GetClassName()) ||
								pControl->IsClass(CTextUI::GetClassName()) )
								return HTCAPTION;
							return HTCLIENT;
						}
						return HTCAPTION;
					}

					return HTCLIENT;
				};

				lResult = onNcHitTest();
			}
			return TRUE;
		case WM_SIZE:
			{
				CPaintManagerUI* pPaintMgrUI = GetPaintMgr();
				if (!pPaintMgrUI)
					return FALSE;
				SIZE szRoundCorner = pPaintMgrUI->GetRoundCorner();
				if( !IsIconic() && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) 
				{
					CDuiRect rcWnd;
					::GetWindowRect(*this, &rcWnd);
					rcWnd.Offset(-rcWnd.left, -rcWnd.top);
					rcWnd.right++; rcWnd.bottom++;
					HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
					::SetWindowRgn(*this, hRgn, TRUE);
					::DeleteObject(hRgn);
				}
				lResult = 0;
			}
			return FALSE;
		case WM_ERASEBKGND:
			lResult = 1;
			return TRUE;
		case WM_NCACTIVATE:
			lResult = (0 == wParam);
			return !IsIconic();
		case WM_NCPAINT:
		case WM_NCCALCSIZE:
			lResult = 0;
			return TRUE;
		case WM_GETMINMAXINFO:
			{
				MONITORINFO oMonitor = {0};
				oMonitor.cbSize = sizeof(oMonitor);
				::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
				CDuiRect rcWork = oMonitor.rcWork;
				rcWork.Offset(/*-rcWork.left, -rcWork.top*/-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

				LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
				lpMMI->ptMaxPosition.x	= rcWork.left;
				lpMMI->ptMaxPosition.y	= rcWork.top;
				lpMMI->ptMaxSize.x		= rcWork.right;
				lpMMI->ptMaxSize.y		= rcWork.bottom;

				lResult = 0;
				return FALSE;
			}
			// 		case WM_SYSCOMMAND:
			// 			{
			// 				if (GET_SC_WPARAM(wParam) == SC_CLOSE)
			// 				{
			// 					PostMessage(WM_CLOSE);
			// 					return TRUE;
			// 				}
			// 				lResult = CWindowWnd::HandleMessage(uMsg,wParam,lParam);
			// 				return TRUE;
			// 			}
		}

		return FALSE;
	}
	LRESULT CWin::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lr;
		if(HandleMsg(m_hWnd,uMsg,wParam,lParam,lr))
			return lr;
		if(this->ProcessWindowMessage(m_hWnd,uMsg,wParam,lParam,lr))
			return lr;
		if(_paintManager && _paintManager->MessageHandler(uMsg,wParam,lParam,lr))
			return lr;
		return __super::HandleMessage(uMsg,wParam,lParam);
	}
	BOOL CWin::ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID /*= 0*/)
	{
		return FALSE;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	class CMsgBoxWin : public CWin,public INotifyUI
	{
	public:
		CMsgBoxWin(){}
		~CMsgBoxWin(){}

		int ShowMsgBox(HWND hwnd,
			LPCTSTR lpszText,
			LPCTSTR lpszCaption,
			UINT nType)
		{
			if(!IsWindow())
			{
				//	this->Create(hwnd,lpszCaption,UI_WNDSTYLE_DIALOG,UI_WNDSTYLE_EX_DIALOG);
				this->Create(hwnd,lpszCaption,WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_TOPMOST);
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

			pCancelBtn->SetVisible(isShowCancel);
			pOkBtn->SetVisible(isShowOk);
			pCheckBox->SetVisible((isShowOk && isShowCheckBox));
		
			if(!isShowCancel && isShowOk)
			{
				pCancelBtn->SetVisible(true);
				pOkBtn->SetVisible(false);
				pCancelBtn->SetName(pOkBtn->GetName()),pCancelBtn->SetText(pOkBtn->GetText());
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
			 if(pSender->IsIDName(_T("closebtn")) ||
				pSender->IsIDName(_T("cancel")))
			 {
				 Close(MBRT_CANCEL);
			 }
			 else if(pSender->IsIDName(_T("yes")) ||
				     pSender->IsIDName(_T("ok")))
			 {
				 CCheckBoxUI* pCheckBox= static_cast<CCheckBoxUI*>(GetPaintMgr()->FindControl(_T("checkbox")));
				 ASSERT(pCheckBox);
				 pCheckBox->IsSelected() ? Close(MBRT_OKCHECKED) : Close(MBRT_OK);
			 }
		 }
		 BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID /*= 0*/)
		 {
			 if (WM_CREATE == uMsg)
			 {
				 ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
				 if(!RegisterSkin(101,_T("xml")) &&  !RegisterSkin(_T("MsgBox.xml")))
					 return 0;

				 GetPaintMgr()->AddNotifier(this);
				 RECT rcClient;
				 ::GetClientRect(*this, &rcClient);
				 ::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
					 rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

				 lResult = 0;
				 return TRUE;
			 }
			 else if(WM_KEYDOWN == uMsg)
			 {
				 if(VK_ESCAPE == wParam)
				 {
					 Close(MBRT_CANCEL);
					 lResult = 0;
					 return TRUE;
				 }
				 else if(VK_RETURN == wParam)
				 {
					 if( GetPaintMgr()->FindControl(_T("ok")) || GetPaintMgr()->FindControl(_T("yes")) )
					 {
						 Close(MBRT_OK);
						 lResult = 0;
						 return TRUE;
					 }
					 else if( GetPaintMgr()->FindControl(_T("cancel")) || GetPaintMgr()->FindControl(_T("no")) )
					 {
						 Close(MBRT_CANCEL);
						 lResult = 0;
						 return TRUE;
					 }
				 }
			 }
			 return __super::ProcessWindowMessage(hWnd,uMsg,wParam,lParam,lResult,dwMsgMapID);
		 }
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