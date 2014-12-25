#include "StdAfx.h"
#include "ThemeMgr.h"

CSkinMgr* CSkinMgr::m_hInst = NULL;

CSkinMgr::CSkinMgr()
{

}
CSkinMgr* CSkinMgr::getInstance()
{
	if (NULL==m_hInst)
	{
		m_hInst = new CSkinMgr();
	}
	return m_hInst;
}
bool CSkinMgr::LoadSkin(STRINGorID xml)
{
	m_xml.LoadFromFile(xml.m_lpstr);
	CMarkupNode root = m_xml.GetRoot();
	if( !root.IsValid() ) return false;
	ZeroMemory(&m_skin,sizeof(m_skin));
	LPCTSTR pstrClass = NULL;
	int nAttributes = 0;
	LPCTSTR pstrName = NULL;
	LPCTSTR pstrValue = NULL;
	LPTSTR pstr = NULL;
	for( CMarkupNode node = root.GetChild() ; node.IsValid(); node = node.GetSibling() )
	{	
		pstrClass = node.GetName();
		if ( _tcscmp(pstrClass, _T("Theme")) == 0 )
		{
			nAttributes = node.GetAttributeCount();
			for( int i = 0; i < nAttributes; i++ )
			{
				pstrName = node.GetAttributeName(i);
				pstrValue = node.GetAttributeValue(i);
				if( _tcscmp(pstrName, _T("name")) == 0 )
				{
					m_skin.m_themeName = CDuiString(pstrValue);
				}
				else if ( _tcscmp(pstrName, _T("folder")) == 0 )
				{
					m_skin.m_skinFolder = CDuiString(pstrValue);
				}
				else if ( _tcscmp(pstrName, _T("file")) == 0 )
				{
					m_skin.m_skinZipName = CDuiString(pstrValue);
				}
				else if ( _tcscmp(pstrName, _T("face")) == 0 )
				{
					m_skin.m_skinFace = CDuiString(pstrValue);
				}
				else if ( _tcscmp(pstrName, _T("skin")) == 0 )
				{
					m_skin.m_skinId = _wtoi(pstrValue);
				}
			}
			CMarkupNode childnode = node;
			if (childnode.HasChildren())
			{
				for( CMarkupNode node2 = childnode.GetChild() ; node2.IsValid(); node2 = node2.GetSibling() )
				{
					pstrClass = node2.GetName();
					if ( _tcscmp(pstrClass, _T("Skin")) == 0 )
					{
						nAttributes = node2.GetAttributeCount();
						for( int i = 0; i < nAttributes; i++ )
						{
							pstrName = node2.GetAttributeName(i);
							pstrValue = node2.GetAttributeValue(i);
							if( _tcscmp(pstrName, _T("id")) == 0 )
							{
								m_skin.m_skinList.push_back(CDuiString(pstrValue));
							}
						}
					}
				}
			}
		}
	}

	pstrClass = root.GetName();
	if( _tcscmp(pstrClass, _T("Application")) == 0 ) 
	{
		nAttributes = root.GetAttributeCount();
		LPCTSTR pAppVersionName = NULL;
		LPCTSTR pAppVersionValue = NULL;
		for( int i = 0; i < nAttributes; i++ )
		{
			pstrName = root.GetAttributeName(i);
			pstrValue = root.GetAttributeValue(i);
			if( _tcscmp(pstrName, _T("version")) == 0 )
			{
				pAppVersionValue = pstrValue;
				m_skin.m_appVersion = CDuiString(pAppVersionValue);
			}
		}
	}
	return true;
}
bool CSkinMgr::UseSkin(STRINGorID skin)
{
	return false;
}