#ifndef __LABELEX_H__
#define __LABELEX_H__
#pragma once
#include "UIAniHelper.h"

namespace DuiLib
{
	class UILIB_API CLabelUIEx : public CLabelUI, public CUIAnimHelper
	{
	public:
		void PaintBkImage(HDC hDC);

		void OnFrame();

		void PaintText(HDC hDC);
	};

	class UILIB_API CLabelIconUI : public CLabelUI
	{
	public:
		CLabelIconUI();

		~CLabelIconUI(void);

	public:
		virtual void PaintStatusImage(HDC hDC);

	public:
		void SetIcon(HICON hIcon);
	private:
		HICON m_hIcon;
	};

	class UILIB_API CLabelMutilineUI : public CLabelUI
	{
	public:
		void PaintText(HDC hDC);
	};
}

#endif// __LABELEX_H__