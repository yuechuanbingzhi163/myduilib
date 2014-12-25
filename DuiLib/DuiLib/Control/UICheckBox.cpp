#include "stdafx.h"
#include "UICheckBox.h"

namespace DuiLib
{
	REGIST_DUICLASS(CCheckBoxUI);

	LPCTSTR CCheckBoxUI::GetClassName()
	{
		return _T("CheckBoxUI");
	}
	LPCTSTR CCheckBoxUI::GetClass() const
	{
		return _T("CheckBoxUI");
	}

	void CCheckBoxUI::SetCheck(bool bCheck)
	{
		Selected(bCheck);
	}

	bool  CCheckBoxUI::GetCheck() const
	{
		return IsSelected();
	}
}
