#include "StdAfx.h"
#include "UIFadeOption.h"
#pragma warning (disable : 4355)
namespace DuiLib {

	REGIST_DUICLASS(CFadeOptionUI);

	CFadeOptionUI::CFadeOptionUI():CFadeAnimation(this),COptionUI()
	{

	}

	CFadeOptionUI::~CFadeOptionUI()
	{

	}

	LPCTSTR CFadeOptionUI::GetClassName()
	{
		return _T("FadeOptionUI");
	}

	LPCTSTR CFadeOptionUI::GetClass() const
	{
		return CFadeOptionUI::GetClassName();
	}
	LPVOID CFadeOptionUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("FadeOption")) == 0 ) 
			return static_cast<CFadeOptionUI*>(this);
		return COptionUI::GetInterface(pstrName);
	}

	void CFadeOptionUI::DoEvent(TEventUI& event)
	{
		COptionUI::DoEvent( event );
		DoAnimationEvent(event);
	}
	void CFadeOptionUI::PaintStatusImage(HDC hDC)
	{
		{
			if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
			else m_uButtonState &= ~ UISTATE_FOCUSED;
			if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
			else m_uButtonState &= ~ UISTATE_DISABLED;

			if ((m_uButtonState & UISTATE_SELECTED) != 0 )
			{
				if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
					if( !m_sDisabledImage.IsEmpty() ) {
						if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
						else goto Label_ForeImage;
					}
				}
				else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
					if( !m_sSelectedPushedImage.IsEmpty() ) {
						if( !DrawImage(hDC, (LPCTSTR)m_sSelectedPushedImage) ) m_sSelectedPushedImage.Empty();
						else goto Label_ForeImage;
					}
				}
				else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
					if( !m_sSelectedHotImage.IsEmpty() ) {
						if( !DrawImage(hDC, (LPCTSTR)m_sSelectedHotImage) ) m_sSelectedHotImage.Empty();
						else goto Label_ForeImage;
					}
				}

				if( !m_sSelectedImage.IsEmpty() ) {
					CFadeAnimation::PaintAnimationStatusImage(hDC,(LPCTSTR)m_sSelectedImage,(LPCTSTR)m_sSelectedHotImage,m_uButtonState);
				}
			}
			else
			{
				if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
					if( !m_sDisabledImage.IsEmpty() ) {
						if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
						else goto Label_ForeImage;
					}
				}
				else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
					if( !m_sPushedImage.IsEmpty() ) {
						if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) ) m_sPushedImage.Empty();
						else goto Label_ForeImage;
					}
				}
				else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
					if( !m_sHotImage.IsEmpty() ) {
						if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) m_sHotImage.Empty();
						else goto Label_ForeImage;
					}
				}
				else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
					if( !m_sFocusedImage.IsEmpty() ) {
						if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
						else goto Label_ForeImage;
					}
				}

				if( !m_sNormalImage.IsEmpty() ) {
					CFadeAnimation::PaintAnimationStatusImage(hDC,(LPCTSTR)m_sNormalImage,(LPCTSTR)m_sHotImage,m_uButtonState);
				}
			}		
		}
	

Label_ForeImage:
		if( !m_sForeImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sForeImage) ) m_sForeImage.Empty();
		}
	}
	void CFadeOptionUI::PaintAnimationButton(HDC hDC)
	{
// 		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
// 		else m_uButtonState &= ~ UISTATE_FOCUSED;
// 		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
// 		else m_uButtonState &= ~ UISTATE_DISABLED;
// 
// 		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
// 			if( !m_sDisabledImage.IsEmpty() ) {
// 				if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
// 				else return;
// 			}
// 		}
// 		else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
// 			if( !m_sPushedImage.IsEmpty() ) {
// 				if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) ) m_sPushedImage.Empty();
// 				else return;
// 			}
// 		}
// 		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
// 			if( !m_sFocusedImage.IsEmpty() ) {
// 				if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
// 				else return;
// 			}
// 		}
// 
// 		if (!IsSelected())
// 		{
// 			CFadeAnimation::PaintAnimationStatusImage(hDC,(LPCTSTR)m_sNormalImage,(LPCTSTR)m_sHotImage,m_uButtonState);
// 		}
// 		else
// 		{
// 			CFadeAnimation::PaintAnimationStatusImage(hDC,(LPCTSTR)m_sSelectedImage,(LPCTSTR)m_sSelectedHotImage,m_uButtonState);
// 		}
			
	}
}