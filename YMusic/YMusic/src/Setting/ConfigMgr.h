#pragma once
#include "ConfigBase.h"
#include "../../YMusic.h"

class CConfigMgr
{
public:
	typedef smart_pointer::SmartPtr<CNormalConfig>  spNormalConfigT;
	typedef smart_pointer::SmartPtr<CAlertConfig>   spAlertConfigT;
	typedef smart_pointer::SmartPtr<CHotKeyConfig>  spHotKeyConfigT;
	typedef smart_pointer::SmartPtr<CCacheConfig>   spCacheConfigT;
	typedef smart_pointer::SmartPtr<CSkinConfig>    spSkinConfigT;
	typedef smart_pointer::SmartPtr<CUpdateConfig>  spUpdateConfigT;
	typedef smart_pointer::SmartPtr<CLrcConfig>     spLrcConfigT;
public:
	CConfigMgr();
	~CConfigMgr();
	void  Load();
	spNormalConfigT  GetNormalConfig();
	spAlertConfigT   GetAlertConfig();
	spHotKeyConfigT  GetHotKeyConfig();
	spCacheConfigT   GetCacheConfig();
	spSkinConfigT    GetSkinConfig();
	spUpdateConfigT  GetUpdateConfig();
	spLrcConfigT     GetLrcConfig();
private:
	spNormalConfigT   m_pNormal;
	spAlertConfigT    m_pAlert;
	spHotKeyConfigT   m_pHotKey;
	spCacheConfigT    m_pCache;
	spSkinConfigT     m_pSkin;
	spUpdateConfigT   m_pUpdate;
	spLrcConfigT      m_pLrc;
};
