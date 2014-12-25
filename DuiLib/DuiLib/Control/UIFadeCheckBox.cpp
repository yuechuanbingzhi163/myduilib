#include "stdafx.h"
#include "UIFadeCheckBox.h"

namespace DuiLib
{
	REGIST_DUICLASS(CFadeCheckBoxUI);

	LPCTSTR CFadeCheckBoxUI::GetClassName()
	{
		return _T("FadeCheckBoxUI");
	}
	LPCTSTR CFadeCheckBoxUI::GetClass() const
	{
		return CFadeCheckBoxUI::GetClassName();
	}

	void CFadeCheckBoxUI::SetCheck(bool bCheck,bool bSendMsg /* = true */)
	{
		Selected(bCheck,bSendMsg);
	}

	bool  CFadeCheckBoxUI::GetCheck() const
	{
		return IsSelected();
	}
}
