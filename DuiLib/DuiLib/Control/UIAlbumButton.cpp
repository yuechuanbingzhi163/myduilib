#include "stdafx.h"
#include "UIAlbumButton.h"
namespace DuiLib{

REGIST_DUICLASS(CAlbumButtonUI);

CAlbumButtonUI::CAlbumButtonUI()
{

}
LPCTSTR CAlbumButtonUI::GetClassName()
{
	return _T("AlbumButtonUI");
}

LPCTSTR CAlbumButtonUI::GetClass() const
{
	return CAlbumButtonUI::GetClassName();
}

LPVOID CAlbumButtonUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("AlbumButton")) == 0 ) 
		return static_cast<CAlbumButtonUI*>(this);
	return CFadeButtonUI::GetInterface(pstrName);
}

void CAlbumButtonUI::DoEvent(TEventUI& event)
{
	CFadeButtonUI::DoEvent( event );
}

void CAlbumButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("bkhotimage")) == 0 )
		SetHotBkImage(pstrValue);
	else 
		return __super::SetAttribute(pstrName,pstrValue);
}
void CAlbumButtonUI::PaintBkImage(HDC hDC)
{
	CFadeAnimation::PaintAnimationStatusImage(hDC,(LPCTSTR)m_sBkImage,(LPCTSTR)m_sHotBkImage,m_uButtonState);
}

void CAlbumButtonUI::SetHotBkImage(LPCTSTR pStrImage)
{
	m_sHotBkImage =pStrImage;
	Invalidate();
}
CDuiString CAlbumButtonUI::GetHotBkImage() const
{
	return m_sHotBkImage;
}

void CAlbumButtonUI::PaintStatusImage(HDC hDC)
{
	return CButtonUI::PaintStatusImage(hDC); //È¥³ýFADE Ð§¹û
}
void CAlbumButtonUI::SetAlbumImage(LPCTSTR pStrImage)
{
	if(pStrImage && lstrlen(pStrImage) != 0)
	{
		if(m_sOldNormalImage.IsEmpty())
			m_sOldNormalImage = GetNormalImage();
		SetNormalImage(pStrImage);
	}
	else
	{
		if(!m_sOldNormalImage.IsEmpty())
			SetNormalImage(m_sOldNormalImage);
	}
}

}