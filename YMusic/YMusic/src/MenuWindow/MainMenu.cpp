#include "stdafx.h"

#include "MainMenu.h"

YMainMenuWindow::YMainMenuWindow()
{

}

template<typename T>
void YMainMenuWindow::FindControl(T& pctrl,LPCTSTR pstrName) const
{
	ASSERT(pstrName);
	pctrl = static_cast<T>(GetPaintMgr()->FindControl(pstrName));
}

LPCTSTR YMainMenuWindow::GetWindowClsss()  
{
	return _T("YMainMenu");
}

LPCTSTR YMainMenuWindow::GetWindowClassName() const
{
	return YMainMenuWindow::GetWindowClsss();
}

void YMainMenuWindow::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

CControlUI* YMainMenuWindow::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void YMainMenuWindow::Notify(TNotifyUI& msg)
{

}

LRESULT YMainMenuWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if(!RegisterSkin(_T("menu_main.xml"),0,this))
		return 0;

	GetPaintMgr()->AddNotifier(this);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);
	return 0;
}

