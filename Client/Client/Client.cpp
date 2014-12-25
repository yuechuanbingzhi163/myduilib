// Client.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Client.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+_T("skin"));
	CPaintManagerUI::SetResourceZip(_T("Cli.dat"));

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;
	if (!AfxSocketInit())
	{
		MessageBox(NULL,_T("加载Com出错"),_T("Error"),MB_OK|MB_ICONERROR);
		return 0;
	}
	if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0))
	{
		MessageBox(NULL,_T("无法提供主线程窗口"),_T("Error"),MB_OK|MB_ICONERROR);
		return 0;
	}
	theApp = new CMainWindow();
	if( theApp == NULL ) return 0;
	theApp->Create(NULL, _T("虾聊2013"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	theApp->CenterWindow();
	theApp->ShowWindow(true);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}