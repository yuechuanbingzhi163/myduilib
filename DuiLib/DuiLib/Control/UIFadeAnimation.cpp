#include "StdAfx.h"
#include "UIFadeAnimation.h"


namespace DuiLib {
	CFadeAnimation::CFadeAnimation(CControlUI* pOwner):CUIAnimation(pOwner), m_bMouseHove( FALSE ), m_bMouseLeave( FALSE )
	{

	}

	CFadeAnimation::~CFadeAnimation()
	{
		StopAnimation();
	}

	void CFadeAnimation::DoAnimationEvent(TEventUI& event)
	{
		switch (event.Type)
		{
		case UIEVENT_MOUSEENTER: EnterFadeIn(); break;
		case UIEVENT_MOUSELEAVE: EnterFadeOut(); break;
		case UIEVENT_TIMER: OnAnimationElapse(event.wParam); break;
		default: break;
		}
	}

	BOOL CFadeAnimation::IsAnimationing() const
	{
		return (IsAnimationRunning(FADE_IN_ID) || IsAnimationRunning(FADE_OUT_ID));
	}

	BOOL CFadeAnimation::EnterFadeIn()
	{
		if(!IsAnimationRunning( FADE_IN_ID ))
		{
			StopAnimation( FADE_OUT_ID );

			m_bFadeAlpha = 0;
			m_bMouseHove = TRUE;
			StartAnimation( FADE_ELAPSE, FADE_FRAME_COUNT, FADE_IN_ID );
			UpdateAnimation();
			return TRUE;
		}
		return FALSE;
	}
	BOOL CFadeAnimation::EnterFadeOut()
	{
		if (!IsAnimationRunning( FADE_OUT_ID ))
		{
			StopAnimation(FADE_IN_ID);

			m_bFadeAlpha = 0;
			m_bMouseLeave = TRUE;
			StartAnimation(FADE_ELAPSE, FADE_FRAME_COUNT, FADE_OUT_ID);
			UpdateAnimation();
			return TRUE;
		}
		return FALSE;
	}

	void CFadeAnimation::OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID)
	{
		if(FADE_IN_ID == nAnimationID || FADE_OUT_ID == nAnimationID)
		{
			m_bFadeAlpha = (BYTE)((nCurFrame / (double)nTotalFrame) * 255);
			if(m_bFadeAlpha == 0) 
				m_bFadeAlpha = 10;
			UpdateAnimation();
		}
	}

	void CFadeAnimation::PaintAnimationStatusImage(HDC hDC,LPCTSTR lpszNormalImage,LPCTSTR lpszHotImage,UINT nState)
	{
		ASSERT(hDC);
		ASSERT(lpszNormalImage);
		if(!lpszNormalImage || !m_pControl)
			return;

		if( IsAnimationing())
		{
// 			if( m_bMouseHove )
// 			{
// 				m_bMouseHove = FALSE;
// 				//m_sLastImage = lpszHotImage;
// 				m_pControl->DrawImage(hDC, (LPCTSTR)lpszNormalImage);
// 				return;
// 			}
// 
// 			if( m_bMouseLeave )
// 			{
// 				m_bMouseLeave = FALSE;
// 				//m_sLastImage = lpszNormalImage;
// 				m_pControl->DrawImage(hDC, (LPCTSTR)lpszHotImage);
// 				return;
// 			}

			LPCTSTR pOldImage = lpszNormalImage;
			LPCTSTR pNewImage = lpszHotImage;

// 			m_sOldImage = lpszNormalImage;
// 			m_sNewImage = lpszHotImage;
			if( IsAnimationRunning(FADE_OUT_ID) )
			{
// 				m_sOldImage = lpszHotImage;
// 				m_sNewImage = lpszNormalImage;
				pOldImage = lpszHotImage;
				pNewImage = lpszNormalImage;
			}

			m_pControl->DrawImage(hDC, pOldImage, NULL, true, 255 - m_bFadeAlpha );
			m_pControl->DrawImage(hDC, pNewImage, NULL, true, m_bFadeAlpha );
		}
		else
		{
			if(0 != (nState&UISTATE_HOT))
				m_pControl->DrawImage(hDC,lpszHotImage);
			else
				m_pControl->DrawImage(hDC,lpszNormalImage);
// 			if(m_sLastImage.IsEmpty())
// 				m_sLastImage = lpszNormalImage;
// 			m_pControl->DrawImage(hDC, (LPCTSTR)m_sLastImage);
		}
	}

	void CFadeAnimation::UpdateAnimation()
	{
		ASSERT(m_pControl);
		if(m_pControl)
			m_pControl->Invalidate();
	}
}