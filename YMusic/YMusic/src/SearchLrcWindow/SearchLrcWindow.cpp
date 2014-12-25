#include "stdafx.h"
#include "SearchLrcWindow.h"

#include "../YApp.h"

YSearchLrcWindow* YSearchLrcWindow::m_Singleton = NULL;
YSearchLrcWindow::YSearchLrcWindow()
{

}

YSearchLrcWindow* YSearchLrcWindow::Instance(HWND hWndParent)
{
	if (m_Singleton==NULL)
	{
		m_Singleton = new YSearchLrcWindow();
		ASSERT(m_Singleton);
		m_Singleton->Create(hWndParent,_T("¸è´ÊËÑË÷"),UI_WNDSTYLE_FRAME,WS_EX_WINDOWEDGE | WS_EX_OVERLAPPEDWINDOW);
	}
	return m_Singleton;
}

LPCTSTR YSearchLrcWindow::GetWindowClsss()  
{
	return _T("YSearchLrc");
}

LPCTSTR YSearchLrcWindow::GetWindowClassName() const
{
	return YSearchLrcWindow::GetWindowClsss();
}

void YSearchLrcWindow::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage(hWnd);
	delete m_Singleton;
	m_Singleton = NULL;
}

CControlUI* YSearchLrcWindow::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void YSearchLrcWindow::Notify(TNotifyUI& msg)
{

}

LRESULT YSearchLrcWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sLog("YSearchLrcWindow::OnCreate");
	ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if(!RegisterSkin(_T("searchlyricwnd.xml"),0,this))
		return 0;

	GetPaintMgr()->AddNotifier(this);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);
	return 0;
}