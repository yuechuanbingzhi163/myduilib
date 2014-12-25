#ifndef __UISEQBUTTON_H__
#define __UISEQBUTTON_H__

#include "UISeqAnimation.h"
#pragma once

namespace DuiLib {
	class UILIB_API CSeqButtonUI :
		public CButtonUI,
		public CSeqAnimation
	{
	public:
		CSeqButtonUI();
		virtual ~CSeqButtonUI();

		// add by rz.li
		static LPCTSTR GetClassName();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoEvent(TEventUI& event);
		void PaintStatusImage(HDC hDC);
	};
}
#endif//__UISEQBUTTON_H__