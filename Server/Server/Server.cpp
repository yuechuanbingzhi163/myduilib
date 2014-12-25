// Server.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Server.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

	HANDLE theMutex = CreateMutex(NULL,FALSE,_T("srv"));
	ASSERT(theMutex);
	if(ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		HWND hwnd = FindWindow(CMainWindow::CLASSNAME,NULL);
		ASSERT(hwnd);
		if(hwnd)
		{
			COPYDATASTRUCT cpd;
			cpd.dwData = 0;
			cpd.cbData = (wcslen(lpCmdLine) + 1)*sizeof(TCHAR);
			cpd.lpData = (void*)lpCmdLine;

			BOOL bret = ::SendMessageTimeout(hwnd,WM_COPYDATA,(WPARAM)hInstance,(LPARAM)&cpd,SMTO_BLOCK,1000*10,NULL);
			ASSERT(bret);
			::ShowWindow(hwnd,SW_SHOWNORMAL);
			::SetActiveWindow(hwnd);
			return 1;
		}
		return -1;
	}

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+_T("skin"));
	CPaintManagerUI::SetResourceZip(_T("Srv.dat"));

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
	theApp->Create(NULL, _T("服务器"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	theApp->CenterWindow();
	theApp->ShowWindow(true);
//	theApp->AnimateWindow(2000);
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();

	return 0;
}
