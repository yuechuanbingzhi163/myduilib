#pragma once

#include "./RuntimeState/RuntimeState.h"
#include "./MainWindow/YMainWindow.h"

class CApp : public Singleton<CApp>
{
public:

	typedef bool(*CmdLineHandler)(const std::wstring&);
	typedef CmdLineHandler spCmdLineHandlerT;
	typedef SmartPtr<YMainWindow> spMainWindowT;
	typedef SmartPtr<CRuntimeState> spRuntimeStateT;
public:
	~CApp();

public:
	int Run(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPTSTR    lpCmdLine,
		int       nCmdShow);
	BOOL HandleCmdLine(LPCTSTR lpCmdLine);
	BOOL HandleCmdLine(const std::wstring& sCmd,const std::wstring& sParam);
protected:
	BOOL Init();
	BOOL Clear();
	BOOL PreHandleCmdLine(const std::wstring& sCmd,const std::wstring& sParam);
protected:
	static bool OnInitAgentClient(const std::wstring& sParam);
	static bool OnInstall(const std::wstring& sParam);
	bool OnUnInstall(const std::wstring& sParam);
	static bool OnPlay(const std::wstring& sParam);
	bool OnPause(const std::wstring& sParam);
	bool OnStop(const std::wstring& sParam);
	bool OnAddSong(const std::wstring& sParam);
	static bool OnUrl(const ::wstring& sParam);
public:
	spRuntimeStateT GetRuntimeState() const;
	spMainWindowT mainWindow() const;
public:
	CString       GetCmdLines() const;
	std::string   GetRunMode() const;
private:
	spRuntimeStateT            _spRuntimeState;
	spMainWindowT              _spMainWin;//Ö÷´°¿Ú
	
	
	typedef std::map<std::wstring,spCmdLineHandlerT> spCmdLineHanderMapT;
	spCmdLineHanderMapT        _spPreCmdLineHandlerMap;
	spCmdLineHanderMapT        _spCmdLineHandlerMap;

	ULONG_PTR m_pGdiToken;

	DECLARE_SINGLETON_CLASS(CApp);
};


#define  theApp             (CApp::Instance())

#define  theRuntimeState    (theApp->GetRuntimeState())
#define  theMainWin         (theApp->mainWindow())
#define  theFilePathMgr     (theRuntimeState->GetFilePathMgr())
#define  theCacheMgr        (theRuntimeState->GetCacheMgr())
//#define  thePlayCenter      (theRuntimeState->GetPlayerCenter())
#define  thePlayer          (theRuntimeState->GetPlayer())
#define  thePlayListMgr     (theRuntimeState->GetPlayListMgr())
#define  theDbMgr           (theRuntimeState->GetDbMgr())
#define  theConfigMgr       (theRuntimeState->GetConfigMgr())
#define  theLrcMgr          (theRuntimeState->GetLrcMgr())
#define  theWebServiceMgr   (theRuntimeState->GetWebServiceMgr())
#define  theHttpDownerMgr (theRuntimeState->GetHttpDonwer())

#define  theNet             (get_xiami_network())
/*

#define  theNet             (get_xiami_network())




#define  theCacheMgr        (theRuntimeState->GetCacheMgr())
#define  theDbMgr           (theRuntimeState->GetDbMgr())
#define  theNotifyCenter    (theRuntimeState->GetNotifyCenter())

#define  theWebServiceMgr   (theRuntimeState->GetWebServiceMgr())
#define  thePlayerCenter    (theRuntimeState->GetPlayerCenter())
#define  theFavMgr          (theRuntimeState->GetFavMgr())

*/




#define  theLog                (*(theRuntimeState->GetLogMgr()))
#ifdef _UNICODE
#define  sLogout(lev,fmt,...)  (theLog(lev,fmt,##__VA_ARGS__))
#else
#define  sLogout(lev,fmt,...)  (theLog(lev,"[file:%s][line:%d]"fmt,__FILE__,__LINE__,##__VA_ARGS__))
#endif

#define  sLog(fmt,...)         (sLogout(em_utility::CLog::InfoLog,fmt,##__VA_ARGS__))
#define  sLogError(fmt,...)    (sLogout(em_utility::CLog::ErrorLog,fmt,##__VA_ARGS__))
#define  sLogWarn(fmt,...)     (sLogout(em_utility::CLog::WarningLog,fmt,##__VA_ARGS__))

