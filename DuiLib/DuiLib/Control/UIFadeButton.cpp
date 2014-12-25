#include "StdAfx.h"
#include "UIFadeButton.h"
#pragma warning (disable : 4355)
namespace DuiLib {

	REGIST_DUICLASS(CFadeButtonUI);

	CFadeButtonUI::CFadeButtonUI() : CFadeAnimation( this )
	{
	}

	CFadeButtonUI::~CFadeButtonUI()
	{
		
	}

	LPCTSTR CFadeButtonUI::GetClassName()
	{
		return _T("FadeButtonUI");
	}
	LPCTSTR CFadeButtonUI::GetClass() const
	{
		return CFadeButtonUI::GetClassName();
	}

	LPVOID CFadeButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("FadeButton")) == 0 ) 
			return static_cast<CFadeButtonUI*>(this);
		return CButtonUI::GetInterface(pstrName);
	}

// 	void CFadeButtonUI::SetNormalImage(LPCTSTR pStrImage)
// 	{
// 		m_sNormalImage = pStrImage;
// 		m_sLastImage = m_sNormalImage;
// 	}

	void CFadeButtonUI::DoEvent(TEventUI& event)
	{
		CButtonUI::DoEvent( event );
		DoAnimationEvent(event);
	}

	void CFadeButtonUI::PaintStatusImage(HDC hDC)
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

		CFadeAnimation::PaintAnimationStatusImage(hDC,(LPCTSTR)m_sNormalImage,(LPCTSTR)m_sHotImage,m_uButtonState);
	}

// 	void CFadeButtonUI::OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID)
// 	{
// 		m_bFadeAlpha = (BYTE)((nCurFrame / (double)nTotalFrame) * 255);
// 		m_bFadeAlpha = m_bFadeAlpha == 0?10:m_bFadeAlpha;
// 		Invalidate();
// 	}


} // namespace DuiLib