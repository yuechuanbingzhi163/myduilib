#ifndef __UIFADEBUTTON_H__
#define __UIFADEBUTTON_H__

#include "UIFadeAnimation.h"
#pragma once

namespace DuiLib {
	
	class UILIB_API CFadeButtonUI : public CButtonUI, public CFadeAnimation
	{
	public:
		CFadeButtonUI();
		virtual ~CFadeButtonUI();

		// add by rz.li
		static LPCTSTR GetClassName();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void DoEvent(TEventUI& event);
		void PaintStatusImage(HDC hDC);
	};

} // namespace UiLib

#endif // __UIFADEBUTTON_H__