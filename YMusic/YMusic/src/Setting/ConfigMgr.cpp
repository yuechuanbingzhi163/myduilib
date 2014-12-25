#include "stdafx.h"
#include "ConfigMgr.h"


	CConfigMgr::CConfigMgr()
	{
	
	}
	CConfigMgr::~CConfigMgr()
	{
	}

	void CConfigMgr::Load()
	{
		GetNormalConfig()->Load();
		GetAlertConfig()->Load();
		GetHotKeyConfig()->Load();
		GetCacheConfig()->Load();
		GetSkinConfig()->Load();
		GetLrcConfig()->Load();
	}
	CConfigMgr::spNormalConfigT CConfigMgr::GetNormalConfig()
	{
		if(NULL == m_pNormal)
		{
			m_pNormal = spNormalConfigT(new CNormalConfig());		
		}

		
		return m_pNormal;
	}
	CConfigMgr::spAlertConfigT CConfigMgr::GetAlertConfig()
	{
		if(NULL == m_pAlert)
		{
			m_pAlert = spAlertConfigT(new CAlertConfig());
			
		}
		return m_pAlert;
	}
	CConfigMgr::spHotKeyConfigT CConfigMgr::GetHotKeyConfig()
	{
		if(NULL == m_pHotKey)
		{
			m_pHotKey = spHotKeyConfigT(new CHotKeyConfig());
			
		}
		return m_pHotKey;
	}
	CConfigMgr::spCacheConfigT CConfigMgr::GetCacheConfig()
	{
		if(NULL == m_pCache)
		{
			m_pCache = spCacheConfigT(new CCacheConfig());
			
		}
		return m_pCache;
	}
	CConfigMgr::spSkinConfigT CConfigMgr::GetSkinConfig()
	{
		if(NULL == m_pSkin)
		{
			m_pSkin = spSkinConfigT(new CSkinConfig());
			
		}

		return m_pSkin;
	}

	CConfigMgr::spUpdateConfigT CConfigMgr::GetUpdateConfig()
	{
		if(NULL == m_pUpdate)
		{
			m_pUpdate = spUpdateConfigT(new CUpdateConfig());
			
		}

		return m_pUpdate;
	}
	CConfigMgr::spLrcConfigT CConfigMgr::GetLrcConfig()
	{
		if (NULL == m_pLrc)
		{
			m_pLrc = spLrcConfigT(new CLrcConfig());
			
		}

		return m_pLrc;
	}

