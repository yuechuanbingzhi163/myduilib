
#pragma once

#include "../../YMusic.h"

using namespace smart_pointer;
using namespace em_utility;

class CRuntimeState;
class CUserContext;
typedef SmartPtr<CRuntimeState> spRuntimeStateT;
typedef SmartPtr<CUserContext>  spUserContextT;


/************************************************************************/
/* 控制运行时的各种变量                                                 */
/************************************************************************/

class CUserContext
{
public:
	~CUserContext();
	bool IsValid() const;
	spRuntimeStateT  GetRuntimeState() const;
	spLogonUserInfoT GetUser() const;
	CString GetUserId() const;
	bool IsUserLogined() const;
	spUserDBaseT GetUserDB() const;
	bool IsMainThread() const;
	bool IsSameContext(spUserContextT& ctx) const;
	bool IsCurrentContext() const;

	void SetTag(int tag);
	int GetTag() const;
private:
	friend class CRuntimeState;
	explicit CUserContext(spRuntimeStateT runtime,spLogonUserInfoT user,spUserDBaseT userdb,LPCTSTR sFlag = NULL);
private:
	spRuntimeStateT   _runtimeState;
	spLogonUserInfoT  _user;
	spUserDBaseT      _userDB;
	int               _tag;
};

class CRuntimeState 
{
public:
	typedef SmartPtr<CCacheMgr>                  spCacheMgrT;
	typedef SmartPtr<CDatabaseManager>           spDbMgrT;
	typedef SmartPtr<CFilePathMgr>               spFilePathMgrT;
	typedef SmartPtr<CLog>                       spLogMgrT;
//	typedef SmartPtr<CPlayCenter>                spPlayerCenterT;//以后替换为AVPlayer
	typedef SmartPtr<CAVPlayer>                  spPlayerT;
	typedef SmartPtr<PlayListMgr>                spPlayListMgrT;
//	typedef SmartPtr<CWinNotifyCenter>           spWinNotifyCenterT;
	typedef SmartPtr<CConfigMgr>                 spConfigMgrT;
	typedef SmartPtr<CWebServiceMgr>             spWebServiceMgrT;
	typedef SmartPtr<em_utility::down_http_file> spHttpDownerT;
//	typedef SmartPtr<CFavMgr>                    spFavMgrT;
	typedef SmartPtr<CLrcMgr>                    spLrcMgrT;

public:
	virtual ~CRuntimeState();
	void Init(DWORD dwMainThreadId);
	void UnInit();
public:
	bool IsMainThread() const;
	bool IsOnline() const;
	spUserContextT GetCurrentContext(LPCTSTR sflag = NULL);
	void SetUserInfo(spLogonUserInfoT user);

	spLogonUserInfoT GetLogUserInfo() const;
	spCacheMgrT GetCacheMgr() const;
	spFilePathMgrT GetFilePathMgr() const;
	spLogMgrT    GetLogMgr() const;
	spDbMgrT GetDbMgr() const;
//	spWinNotifyCenterT GetNotifyCenter() const;
	spConfigMgrT GetConfigMgr() const;
	spWebServiceMgrT GetWebServiceMgr() const;
//	spPlayerCenterT GetPlayerCenter() const;
	spPlayerT GetPlayer() const;
	spPlayListMgrT GetPlayListMgr() const;
	spHttpDownerT GetHttpDonwer() const;
//	spFavMgrT GetFavMgr()const;
	spLrcMgrT GetLrcMgr()const;
protected:
	bool                     _isOnline;

	spCacheMgrT              _spCacheMgr;
	spFilePathMgrT           _spFilePathMgr;
	spLogMgrT                _spLogMgr;
	spDbMgrT                 _spDbMgr;
//	spWinNotifyCenterT       _spWinNotifyCenter;
	spLogonUserInfoT         _spUserInfo;
	spConfigMgrT             _spConfigMgr;
	spWebServiceMgrT         _spWebServiceMgr;
//	spPlayerCenterT          _spPlayerCenter;//以后替换
	spPlayerT                _spPlayer;
	spPlayListMgrT			 _spPlayListMgr;
	spHttpDownerT            _spHttpDowner;
//	spFavMgrT                _spFavMgr;
	spLrcMgrT                _spLrcMgr;
private:
	DECLARE_SINGLETON_CLASS(CRuntimeState);
};
