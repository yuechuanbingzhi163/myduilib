#ifndef __UIFADEOPTION_H__ 
#define __UIFADEOPTION_H__


#include "UIFadeAnimation.h"
#pragma once

namespace DuiLib {
	class UILIB_API CFadeOptionUI : 
		public COptionUI,
		public CFadeAnimation
	{
	public:
		CFadeOptionUI();
		virtual ~CFadeOptionUI();

		// add by rz.li
		static LPCTSTR GetClassName();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void DoEvent(TEventUI& event);
		void PaintStatusImage(HDC hDC);
	protected:
		void PaintAnimationButton(HDC hDC);
	};
}
#endif//__UIFADEOPTION_H__