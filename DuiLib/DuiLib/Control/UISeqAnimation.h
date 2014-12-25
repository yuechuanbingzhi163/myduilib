#ifndef __UISEQANIMATION_H__
#define __UISEQANIMATION_H__

#include "UIAnimation.h"
#pragma once

namespace DuiLib {
	class UILIB_API CSeqAnimation :
		public CUIAnimation
	{
	public:
		CSeqAnimation(CControlUI* pOwner);
		virtual ~CSeqAnimation();
	public:
		void SetSeqImages(LPCTSTR lpszImage);
		CDuiString GetSeqImages() const;
		void SetTotalFrameCnt(unsigned int n);
		unsigned int GetTotalFrameCnt() const;

		void DoAnimationEvent(TEventUI& event);

		BOOL IsAnimationing() const;
		//animation
		virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);

		void PaintAnimationStatusImage(HDC hDC,LPCTSTR lpszNormalImage,LPCTSTR lpszHotImage,UINT nState);
	private:
		BOOL EnterSeqIn();
		BOOL EnterSeqOut();
		void UpdateAnimation();
	private:
		CDuiString     m_sImagesName;
		unsigned int   m_nTotalFrameCnt;
		unsigned int   m_nCurFrame;

		enum{
			SEQ_IN_ID    = 10,
			SEQ_OUT_ID = 11,

			SEQ_ELAPSE = 10,
			//FADE_FRAME_COUNT = 30,//50
		};
	};
}
#endif//__UISEQANIMATION_H__