/*********************************************************************
*  这是为duilib开发的动态调色板控件，完全模仿酷狗和QQ的调色板功能
*  使用了HSB绘制模式，结合我自己研究出的一个算法绘制界面
*  这个调色板可以大致模仿出酷狗调色板的样式，但是还有差别
*  如果有哪位朋友知道更好的算法或者有类似的代码，或者此代码有bug，请联系我
*  By:Redrain  QQ：491646717   2014.8.19
*  Modify By 笑看风云 QQ: 89702120 2014-08-19 重构
*  如果有类似的需求，可以修改这个类
*  sample:<ColorPalette name="Pallet" width="506" height="220" palletheight="200" barheight="14" padding="8,5,0,0" bkcolor="#FFFFFFFF" thumbimage="UI\skin\cursor.png" />
*
*  此模块是在我原来的代码上由网友“风之羽翼”重新编写的，修改了算法，大幅提升了动态调色板的性能，之后我又进行了一点bug，再次感谢风之羽翼的代码
*********************************************************************/
#ifndef __UIColorPalette_H__
#define __UIColorPalette_H__

#pragma once

namespace DuiLib
{
	class	UILIB_API	CColorPaletteUI : public CControlUI
	{
	public:
		CColorPaletteUI();
		virtual ~CColorPaletteUI();

		static LPCTSTR GetClassName();
		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);

		virtual	bool	Activate();
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		virtual void SetPos(RECT rc);
		virtual void DoInit();
		virtual void DoEvent(TEventUI& event);
		virtual void DoPaint(HDC hDC, const RECT& rcPaint);
		virtual void PaintPallet(HDC hDC);
	public:
		//获取最终被选择的颜色，可以直接用于设置duilib背景色
		DWORD	GetSelectColor( bool bAlaph = true);
		void	SetSelectColor(DWORD dwColor);
		//设置/获取 调色板整体透明值
		void	SetPaletteAlaph(int nAlaph);
		int		GetPaletteAlaph() const;

		//设置/获取 Pallet（调色板主界面）的高度
		void	SetPalletHeight(int nHeight);
		int		GetPalletHeight() const;

		//设置/获取 下方Bar（亮度选择栏）的高度
		void	SetBarHeight(int nHeight);
		int		GetBarHeight() const;
		//设置/获取 选择图标的路径
		void	SetThumbImage(LPCTSTR pszImage);
		CDuiString	GetThumbImage() const;

	protected:
		//更新数据
		void	_UpdatePalletData();
		void	_UpdateBarData();

	protected:
		CDuiString	m_bmpImagePallet;
		MBmpImageCach	m_bmpImageBar;
		UINT		m_uButtonState;
		bool		m_bIsInBar;
		bool		m_bIsInPallet;
		int			m_nCurH;
		int			m_nCurS;
		int			m_nCurB;
	protected:
		int			m_nAlaph;
		int			m_nPalletHeight;
		int			m_nBarHeight;
		DuiLib::CPoint		m_ptLastPalletMouse;
		CDuiString		m_strThumbImage;
	};
};

#endif//__UIColorPalette_H__