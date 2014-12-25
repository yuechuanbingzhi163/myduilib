#ifndef __UIOPTION_H__
#define __UIOPTION_H__

#pragma once

namespace DuiLib
{
	class UILIB_API COptionUI : public CButtonUI
	{
	public:
		COptionUI();
		~COptionUI();

		// add by rz.li
		static LPCTSTR GetClassName();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

		bool Activate();
		void SetEnabled(bool bEnable = true);

		LPCTSTR GetSelectedImage();
		void SetSelectedImage(LPCTSTR pStrImage);

		void SetSelectedTextColor(DWORD dwTextColor);
		DWORD GetSelectedTextColor();

		LPCTSTR GetForeImage();
		void SetForeImage(LPCTSTR pStrImage);

		LPCTSTR GetSelectedHotImage();
		void SetSelectedHotImage(LPCTSTR pStrImage);

		LPCTSTR GetSelectedPushedImage();
		void SetSelectedPushedImage(LPCTSTR pStrImage);

		LPCTSTR GetGroup() const;
		void SetGroup(LPCTSTR pStrGroupName = NULL);
		bool IsSelected() const;
		virtual void Selected(bool bSelected,bool bSendMsg = true);

		SIZE EstimateSize(SIZE szAvailable);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);

		//add by daodao 2013.01.08
		void SetNotFilterSelected(bool bNotFilterSelected);
		bool IsNotFilterSelected()const;

		//add by dfn.li 2014.01.05
		void SetSelectedToolTip(LPCTSTR szTooltip);
		LPCTSTR GetSelectedToolTip() const;

		void SetNormalTooltip(LPCTSTR szTooltip);
		LPCTSTR GetNormalTooltip() const;

	protected:
		bool m_bSelected;
		CDuiString m_sGroupName;

		DWORD m_dwSelectedTextColor;

		CDuiString m_sSelectedImage;
		CDuiString m_sForeImage;

		//扩展图片;

		CDuiString m_sSelectedHotImage;
		CDuiString m_sSelectedPushedImage;

		//提示扩展
		CDuiString m_sSelectedTooltip;
		CDuiString m_sNormalTooltip;

		//add by daodao 2013.01.08
		bool m_bNotFilterSelected;

	};

} // namespace DuiLib

#endif // __UIOPTION_H__