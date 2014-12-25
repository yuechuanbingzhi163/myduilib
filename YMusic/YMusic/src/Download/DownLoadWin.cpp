#include "stdafx.h"

#include "DownLoadWin.h"

__UsingMyNameSpace

YDownLoadWin::YDownLoadWin()
{}

YDownLoadWin::~YDownLoadWin()
{}

LPCTSTR YDownLoadWin::GetWindowClass()
{
	return _T("YDownLoadWin");
}

LPCTSTR YDownLoadWin::GetWindowClassName() const
{
	return YDownLoadWin::GetWindowClass();
}

void YDownLoadWin::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage( hWnd );
}

LRESULT YDownLoadWin::OnCreate(UINT uMsg,LPARAM lparam,WPARAM wparam,BOOL& bHandled)
{
	return 0;
}

CControlUI* YDownLoadWin::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}
void YDownLoadWin::InitWindow()
{

}

BOOL YDownLoadWin::Receive(SkinChangedParam param)
{
	return TRUE;
}

void YDownLoadWin::Notify(TNotifyUI& msg)
{
	return CNotifyPump::NotifyPump( msg );
}



