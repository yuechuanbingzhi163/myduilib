#ifndef __ANIMATIONCONTROLUI_H__
#define __ANIMATIONCONTROLUI_H__
#include "UIAnimation.h"
#pragma once

namespace DuiLib
{
	class UILIB_API CAnimControlUI : 
		public CLabelUI,
		public CUIAnimation
	{
	public:
		CAnimControlUI();
		virtual ~CAnimControlUI();

		// add by rz.li
		static LPCTSTR GetClassName();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetImages(LPCTSTR pstrImagesName);
		CDuiString GetImages() const;

		void SetElapse(unsigned int uEllapse);
		unsigned int GetElapse() const;

		void SetTotalFrameCnt(unsigned int uTotalFrameCnt);
		unsigned int GetTotalFrameCnt() const;

		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintStatusImage(HDC hDC);

		//animation
		void Start();
		void Stop();
		BOOL IsRunning() const;
		void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);
	private:
		static const int ANIMATION_ID = 20;
		CDuiString     m_sImagesName;
		CDuiString     m_sImagesExt;
		unsigned int   m_uElapse;
		unsigned int   m_uTotalFrameCnt;
		unsigned int   m_uCurFrame;
	};
}
#endif//__ANIMATIONCONTROLUI_H__