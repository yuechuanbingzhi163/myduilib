
#include "stdafx.h"

#include "YApp.h"

#include <GdiPlus.h>
using namespace Gdiplus;
#pragma  comment(lib,  "gdiplus.lib")


CApp::~CApp()
{
	Clear();
}
BOOL CApp::Init()
{
	ASSERT(theApp);

	_spPreCmdLineHandlerMap.insert(spCmdLineHanderMapT::value_type(_T("-agent"),&CApp::OnInitAgentClient));

	_spCmdLineHandlerMap.insert(spCmdLineHanderMapT::value_type(_T("xiating"),&CApp::OnUrl));
	_spCmdLineHandlerMap.insert(spCmdLineHanderMapT::value_type(_T("-install"),&CApp::OnInstall));

	_spCmdLineHandlerMap.insert(spCmdLineHanderMapT::value_type(_T("-play"),&CApp::OnPlay));

	m_pGdiToken = NULL;

	_spRuntimeState = spRuntimeStateT(new CRuntimeState());
	_spRuntimeState->Init(::GetCurrentThreadId());
	ASSERT(theRuntimeState);

	Gdiplus::GdiplusStartupInput GdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_pGdiToken, &GdiplusStartupInput, NULL);

	return TRUE;
}

BOOL CApp::Clear()
{
	Gdiplus::GdiplusShutdown(m_pGdiToken);

	return TRUE;
}

bool OpenP2PStream()
{
	{ 
		HANDLE theMutex = OpenMutex(READ_CONTROL, FALSE, _T("Global\\XIAMI_P2PLIB_MUTEX"));
//		assert(theMutex);
		::CloseHandle(theMutex);
		if(ERROR_ALREADY_EXISTS == ::GetLastError())
		{
			return true;
		}
	}

	if(CSys::IsAdmin() || !CSys::IsVista())
	{
		CString sPath = em_utility::mci::GetModulePath();
		CString sP2PStreamName = sPath + _T("p2pstream.exe");

		SHELLEXECUTEINFO ShellInfo = {0};
		ShellInfo.cbSize = sizeof(ShellInfo);
		ShellInfo.hwnd   = NULL;
		ShellInfo.lpVerb = _T("open");
		ShellInfo.lpFile = sP2PStreamName.GetBuffer();
		ShellInfo.nShow  = SW_SHOWNORMAL;
		ShellInfo.fMask  = SEE_MASK_NOCLOSEPROCESS;
		ShellInfo.lpParameters = _T("/install");
		sP2PStreamName.ReleaseBuffer();
		if(::ShellExecuteEx(&ShellInfo))
		{
			system("net start xiamiP2PStream");
			
			return true;
		}
//		return true;
	}

	CSys::ReStartApp(NULL,TRUE);
	return false;
}


int CApp::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{

	if(!Init())
		return 0;

// 	if(!OpenP2PStream())
// 		return 0;
	
	sLog("WinMain hInstance:%d,cmdLine:%s,cmdShow:%d",hInstance,lpCmdLine,nCmdShow);
	sLog("RunMode %s",GetRunMode().c_str());

	std::wstring cmdLine(lpCmdLine);
	std::wstring::size_type nbegin = cmdLine.find_first_of(_T('：'));
	std::wstring cmd;
	std::wstring param;
	if(nbegin  != std::wstring::npos)
	{
		cmd.swap(cmdLine.substr(0,nbegin));
		if(nbegin < cmdLine.size())
		{
			param.swap(cmdLine.substr(nbegin+1));
		}
	}
	//解析命令行
	sLog("PreHandleCmdLine(%s,%s)",cmd.c_str(),param.c_str());
	if(PreHandleCmdLine(cmd,param))
		return 1;
	//确保只能有一个进程
	HANDLE theMutex = ::CreateMutex(NULL, FALSE, _T("YMusic"));
	assert(theMutex);
	//CloseHandle(theMutex);
	if(ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		HWND hwnd = FindWindow(YMainWindow::GetWindowClsss(),NULL);
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
			::BringWindowToTop(hwnd);

			return 1;
		}
		return -1;
	}

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

//	theCrashDumper.Enable(_T("xiating"),true);
//	CWSAStartup wsa(2,1);

	

	CPaintManagerUI::SetInstance(hInstance);
//#ifndef _DEBUG
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
	CPaintManagerUI::SetResourceZip(_T("YMusic.dat"));
// #else
// 	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin/YMusic"));
// #endif
	

	//处理命令行
//	HandleCmdLine(cmd,param);
	HandleCmdLine(lpCmdLine);
	
	_spMainWin = spMainWindowT(new YMainWindow);
	ASSERT(_spMainWin != NULL);
	if(!_spMainWin) return 0;

	_spMainWin->Create(NULL, _T("逸听"),UI_WNDSTYLE_FRAME,/* WS_EX_STATICEDGE | WS_EX_APPWINDOW*/WS_EX_WINDOWEDGE | WS_EX_OVERLAPPEDWINDOW);
	_spMainWin->CenterWindow();
	_spMainWin->ShowWindow(true);

	sLog("Register Player");
	thePlayer->SetHWND(_spMainWin->GetHWND());
	thePlayer->SetCallbackPlaying(YMainWindow::CallbackPlaying);
	thePlayer->SetCallbackPosChanged(YMainWindow::CallbackPosChanged);
	thePlayer->SetCallbackEndReached(YMainWindow::CallbackEndReached);

	sLog("Start MessageLoop");
	CPaintManagerUI::MessageLoop();

//	_spRuntimeState->UnInit();
//	_spRuntimeState.reset();

	::CoUninitialize();

	return 0;
}

BOOL CApp::PreHandleCmdLine(const std::wstring& sCmd,const std::wstring& sParam)
{
	spCmdLineHanderMapT::iterator it = _spPreCmdLineHandlerMap.find(sCmd);
	if(_spPreCmdLineHandlerMap.end() != it && (it->second))
	{
		return it->second(sParam);
	}

	return FALSE;
}
BOOL CApp::HandleCmdLine(LPCTSTR lpCmdLine)
{
	std::wstring cmdLine(lpCmdLine);
	std::wstring::size_type nbegin = cmdLine.find_first_of(_T(':'));
	std::wstring cmd;
	std::wstring param;
	if(nbegin  != std::wstring::npos)
	{
		cmd.swap(cmdLine.substr(0,nbegin));
		if(nbegin < cmdLine.size())
		{
			param.swap(cmdLine.substr(nbegin+1));
		}
	}
	return HandleCmdLine(cmd,param);
}
BOOL CApp::HandleCmdLine(const std::wstring& sCmd,const std::wstring& sParam)
{
	spCmdLineHanderMapT::iterator it = _spCmdLineHandlerMap.find(sCmd);
	if(_spCmdLineHandlerMap.end() != it && it->second)
	{
		return it->second(sParam);
	}

	return FALSE;
}

CApp::spRuntimeStateT CApp::GetRuntimeState() const
{
	return _spRuntimeState;
}

CApp::spMainWindowT CApp::mainWindow() const
{
	return _spMainWin;
}



CString CApp::GetCmdLines() const
{
	return _T("");
}
std::string CApp::GetRunMode() const
{
	std::string sMode;
#ifdef _DEBUG
	sMode += std::string("Debug|");
#else 
	sMode += std::string("Release|");
#endif

#ifdef _UNICODE
	sMode += std::string("Unicode");
#else 
	sMode += std::string("MultiChar");
#endif
	return sMode;
}

////
bool CApp::OnInitAgentClient(const std::wstring& sParam)
{
	int nPort = 0;
	std::wstring ssParam = std::wstring(sParam);
	if(1 != swscanf_s(ssParam.c_str(),_T("%d"),&nPort))
		return FALSE;
	if(!nPort)
		return FALSE;

	HANDLE theMutex = ::CreateMutex(NULL, TRUE, _T("YMusic_Agent"));
	assert(theMutex);
	::CloseHandle(theMutex);
	if(ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		return TRUE;
	}

//	InitAgentClient(nPort);
	return TRUE;
}
bool CApp::OnInstall(const std::wstring& sParam)
{

	return TRUE;
}
bool CApp::OnUnInstall(const std::wstring& sParam)
{
	return FALSE;
}

bool CApp::OnPlay(const std::wstring& sParam)
{	
	return TRUE;
}
bool CApp::OnPause(const std::wstring& sParam)
{

	return TRUE;
}
bool CApp::OnStop(const std::wstring& sParam)
{

	return TRUE;
}
bool CApp::OnAddSong(const std::wstring& sParam)
{

	return TRUE;
}
bool CApp::OnUrl(const std::wstring& sParam)
{
	return FALSE;
}

/************************************************************************************************/

