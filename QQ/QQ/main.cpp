// QQ.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "QQ.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("QQ\\skin\\0"));
	CPaintManagerUI::SetResourceZip(_T("0.zip"));

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	Console::Write(_T("你好%.2f\n"), 111.011f);
	VSOutput::Write(_T("你好%.2f\n"), 100.0403f);

	theApp = new CMainWindow();
	if (theApp == NULL) return 0;
	theApp->Create(NULL, _T("QQ2013"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	theApp->CenterWindow();
	theApp->ShowWindow(true);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}

