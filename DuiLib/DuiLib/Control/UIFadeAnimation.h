#ifndef __UIFADEANIMATION_H__
#define __UIFADEANIMATION_H__

#include "UIAnimation.h"
#pragma once

namespace DuiLib {
	class UILIB_API CFadeAnimation :
		public CUIAnimation
	{
	public:
		CFadeAnimation(CControlUI* pOwner);
		virtual ~CFadeAnimation();
	public:
		void DoAnimationEvent(TEventUI& event);

		BOOL IsAnimationing() const;
		//animation
		virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);

		void PaintAnimationStatusImage(HDC hDC,LPCTSTR lpszNormalImage,LPCTSTR lpszHotImage,UINT nState);

	private:
		BOOL EnterFadeIn();
		BOOL EnterFadeOut();
		void UpdateAnimation();
	protected:
		BYTE       m_bFadeAlpha;
		BOOL       m_bMouseHove;
		BOOL       m_bMouseLeave;
		enum{
			FADE_IN_ID    = 8,
			FADE_OUT_ID = 9,

			FADE_ELAPSE = 10,
			FADE_FRAME_COUNT = 30,//50
		};
	};
}
#endif //__UIFADEANIMATION_H__