#include "StdAfx.h"
#include "UISeqButton.h"
#pragma warning (disable : 4355)
namespace DuiLib {

	REGIST_DUICLASS(CSeqButtonUI);

	CSeqButtonUI::CSeqButtonUI():CSeqAnimation(this)
	{

	}

	CSeqButtonUI::~CSeqButtonUI()
	{

	}

	LPCTSTR CSeqButtonUI::GetClassName()
	{
		return _T("SeqButtonUI");
	}
	LPCTSTR CSeqButtonUI::GetClass() const
	{
		return CSeqButtonUI::GetClassName();
	}
	LPVOID CSeqButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("SeqButton")) == 0 ) 
			return static_cast<CSeqButtonUI*>(this);
		return CButtonUI::GetInterface(pstrName);
	}
	void CSeqButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("seqimages")) == 0 ) SetSeqImages(pstrValue);
		else if( _tcscmp(pstrName, _T("framecnt")) == 0 ) SetTotalFrameCnt(_ttoi(pstrValue));
		else CButtonUI::SetAttribute(pstrName,pstrValue);
	}
	void CSeqButtonUI::DoEvent(TEventUI& event)
	{
		CButtonUI::DoEvent( event );
		DoAnimationEvent(event);
	}
	void CSeqButtonUI::PaintStatusImage(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if( !m_sDisabledImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
			if( !m_sPushedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) ) m_sPushedImage.Empty();
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if( !m_sFocusedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
				else return;
			}
		}

		CSeqAnimation::PaintAnimationStatusImage(hDC,(LPCTSTR)m_sNormalImage,(LPCTSTR)m_sHotImage,m_uButtonState);
	}
}