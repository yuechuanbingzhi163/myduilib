#ifndef __UIFADECHECKBOX_H__
#define __UIFADECHECKBOX_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CFadeCheckBoxUI :
		public CFadeOptionUI
	{
	public:
		// add by rz.li
		static LPCTSTR GetClassName();

		LPCTSTR GetClass() const;

		void SetCheck(bool bCheck,bool bSendMsg = true);
		bool GetCheck() const;
	};
}

#endif // __UIFADECHECKBOX_H__
