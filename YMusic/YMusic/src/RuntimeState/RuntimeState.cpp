#include <stdafx.h>
#include "RunTimeState.h"
#include "../MusicLib/DownHelper/ThreadHelper.h"

#include "../YApp.h"
CUserContext::CUserContext(spRuntimeStateT runtime,spLogonUserInfoT user,spUserDBaseT userdb,LPCTSTR sFlag /* = NULL */)
	:_runtimeState(runtime),_user(user),_userDB(userdb),_tag(0)
{
	ASSERT(IsValid());
}
CUserContext::~CUserContext()
{
	
}
bool CUserContext::IsValid() const
{
	return (_runtimeState && _userDB);
}
spRuntimeStateT  CUserContext::GetRuntimeState() const
{
	return _runtimeState;
}
spLogonUserInfoT CUserContext::GetUser() const
{
	return _user;
}
CString CUserContext::GetUserId() const
{
	spLogonUserInfoT theUserInfo = GetUser();
	return (theUserInfo ? theUserInfo->sUserID : _T(""));
}
bool CUserContext::IsUserLogined() const
{
	return !!_user;
}
spUserDBaseT CUserContext::GetUserDB() const
{
	return _userDB;
}
bool CUserContext::IsMainThread() const
{
	return GetRuntimeState()->IsMainThread();
}

bool CUserContext::IsSameContext(spUserContextT& ctx) const
{
	ASSERT(ctx);
	if(!ctx)
		return false;
	return 
		(ctx->GetRuntimeState() == this->GetRuntimeState()) &&
		(ctx->GetUser() == this->GetUser()) &&
		(ctx->GetUserDB() == this->GetUserDB());
}
bool CUserContext::IsCurrentContext() const
{
	return IsSameContext(this->GetRuntimeState()->GetCurrentContext(NULL));
}
void CUserContext::SetTag(int tag)
{
	_tag = tag;
}
int CUserContext::GetTag() const
{
	return _tag;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

CRuntimeState::~CRuntimeState()
{
	UnInit();
}
void CRuntimeState::Init(DWORD dwMainThreadId)
{
	//初始未登录
	_isOnline = false;
	//文件管理器
	_spFilePathMgr = spFilePathMgrT(new CFilePathMgr());
	//缓存管理器
	_spCacheMgr = spCacheMgrT(new CCacheMgr());
	_spCacheMgr->Init(_spFilePathMgr->GetDefaultCachePath());
	//日志管理器
	CString str= _spFilePathMgr->GetLogPath();
	str += _T("_YMusic.log");
	std::string	 strLogFileName = em_utility::mci::unicode_2_ansi(str.GetBuffer());
	_spLogMgr = spLogMgrT(new CLog(strLogFileName.c_str()));
	assert(_spLogMgr->isValid());
	if(!_spLogMgr->isValid())
		assert(false);
	str.ReleaseBuffer();
	//数据库管理器
	_spDbMgr = spDbMgrT(new CDatabaseManager());
	assert(_spDbMgr);
	_spDbMgr->SetDatabasePath(_spFilePathMgr->GetDatabasePath());
	_spDbMgr->GetGlobalDBase();	
	_spDbMgr->SetCurrentUser(_spUserInfo);
	_spDbMgr->GetUserDB();
	
	//网络管理类
	_spWebServiceMgr = spWebServiceMgrT(new CWebServiceMgr());
	_spWebServiceMgr->SetNotifyMsg(WM_WEB_SERVICE);
	//播放器类
	_spPlayer = spPlayerT(new CAVPlayer());
	assert(_spPlayer);
	//播放列表管理器
	_spPlayListMgr = spPlayListMgrT(new PlayListMgr());
	assert(_spPlayListMgr);
	_spPlayListMgr->LoadAllPlayList();
	//配置管理器
	_spConfigMgr = spConfigMgrT(new CConfigMgr);
	assert(_spConfigMgr);
	_spConfigMgr->Load();
	//歌词管理器
	_spLrcMgr = spLrcMgrT(new CLrcMgr);
	assert(_spLrcMgr);
	//文件下载管理容器
	_spHttpDowner = spHttpDownerT(new em_utility::down_http_file);
	assert(_spHttpDowner);
	_spHttpDowner->start_thread();
	//初始化libcurl环境
	HttpManagerProto::Instance()->CurlGlobalInit();
	StreamThread::Instance()->Startup();
}
void CRuntimeState::UnInit()
{
	_spHttpDowner->stop_thread();
	HttpManagerProto::Instance()->CurlGlobalCleanup();
//	StreamThread::Instance()->Shutdown();
}
bool CRuntimeState::IsMainThread() const
{
	return true;
}
bool CRuntimeState::IsOnline() const
{
	return _isOnline;
}

CRuntimeState::spCacheMgrT CRuntimeState::GetCacheMgr() const
{
	return _spCacheMgr;
}

CRuntimeState::spFilePathMgrT CRuntimeState::GetFilePathMgr() const
{
	return _spFilePathMgr;
}

CRuntimeState::spLogMgrT CRuntimeState::GetLogMgr() const
{
	return _spLogMgr;
}
CRuntimeState::spPlayerT CRuntimeState::GetPlayer() const
{
	return _spPlayer;
}
CRuntimeState::spPlayListMgrT CRuntimeState::GetPlayListMgr() const
{
	return _spPlayListMgr;
}

CRuntimeState::spDbMgrT CRuntimeState::GetDbMgr() const
{
	return _spDbMgr;
}
spUserContextT CRuntimeState::GetCurrentContext(LPCTSTR sflag/* = NULL*/)
{
	spLogonUserInfoT user(_spUserInfo);
	spUserDBaseT userdb(GetDbMgr()->GetUserDB());

	return spUserContextT(new CUserContext(theRuntimeState,user,userdb));
}
spLogonUserInfoT CRuntimeState::GetLogUserInfo() const
{
	return _spUserInfo;
}
void CRuntimeState::SetUserInfo(spLogonUserInfoT user)
{
	if(_spUserInfo != user)
	{
		if(_spUserInfo) //注销
		{
			//_spUserInfo->bKeepLogged = false;
			sLog("注销");
			sLog("theDbMgr->GetGlobalDBase():: %x",theDbMgr->GetGlobalDBase());
			ASSERT(theDbMgr->GetGlobalDBase());
			ASSERT(theDbMgr->GetGlobalDBase()->UpdateUserInfo(_spUserInfo));
		}

		theDbMgr->SetCurrentUser(user);
		_spUserInfo = user;
		theDbMgr->GetUserDBase();
		if(_spUserInfo)
			ASSERT(theDbMgr->GetGlobalDBase()->UpdateLastUserInfo(_spUserInfo));
		assert(_spPlayListMgr);
		_spPlayListMgr->LoadAllPlayList();
		_spConfigMgr->Load();
		PostMessage(theMainWin->GetHWND(),WM_LOGIN,(unsigned long)&user,0);
	}
}

CRuntimeState::spConfigMgrT CRuntimeState::GetConfigMgr() const
{
	return _spConfigMgr;
}

CRuntimeState::spLrcMgrT CRuntimeState::GetLrcMgr() const
{
	return _spLrcMgr;
}

CRuntimeState::spWebServiceMgrT CRuntimeState::GetWebServiceMgr() const
{
	return _spWebServiceMgr;
}

CRuntimeState::spHttpDownerT CRuntimeState::GetHttpDonwer() const
{
	return _spHttpDowner;
}