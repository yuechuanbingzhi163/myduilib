#include "stdafx.h"

#include "YLogin.h"
#include "../YApp.h"

YLoginWindow* YLoginWindow::m_hSingleton = NULL;

template<typename T>
void YLoginWindow::FindControl(T& pctrl,LPCTSTR pstrName) const
{
	ASSERT(pstrName);
	pctrl = static_cast<T>(GetPaintMgr()->FindControl(pstrName));
}

YLoginWindow* YLoginWindow::Instance(HWND hWndParent)
{
	if (m_hSingleton==NULL)
	{
		m_hSingleton = new YLoginWindow();
		ASSERT(m_hSingleton);
		m_hSingleton->Create(hWndParent,_T("µÇÂ¼"),UI_WNDSTYLE_FRAME,WS_EX_WINDOWEDGE | WS_EX_OVERLAPPEDWINDOW);
	}
	return m_hSingleton;
}

LPCTSTR YLoginWindow::GetWindowClsss()  
{
	return _T("YLogin");
}

LPCTSTR YLoginWindow::GetWindowClassName() const
{
	return YLoginWindow::GetWindowClsss();
}

void YLoginWindow::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage(hWnd);
	delete m_hSingleton;
	m_hSingleton = NULL;
}

CControlUI* YLoginWindow::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void YLoginWindow::Notify(TNotifyUI& msg)
{
	CControlUI* pUI = msg.pSender;

	YTrace_(_T("YLoginWindow::Notify class:%s,event:%s,name:%s"),pUI ? pUI->GetClass():_T(""),msg.sType.GetData(),pUI?pUI->GetName().GetData():_T(""));

	if (0 == msg.sType.Compare(kClick))
		return OnClick(msg);
}
void YLoginWindow::OnClick(TNotifyUI& msg)
{
	CControlUI* pSender = msg.pSender;

	if (pSender->IsName(varLoginWindow::kCloseBtn))
		Close(0L);
}
LRESULT YLoginWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sLog("YLoginWindow::OnCreate");

	ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if(!RegisterSkin(_T("login.xml"),0,this))
		return 0;

	GetPaintMgr()->AddNotifier(this);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);
	return 0;
}

