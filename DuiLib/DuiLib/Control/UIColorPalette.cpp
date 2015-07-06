
#include "UIColorPalette.h"
#include "MSkinProperty_Ex.h"
#include <../MDuiLib/Core/MSkinProperty.h>
#include <../MDuiLib/Core/UIManager.h>
#include <../MDuiLib/Core/UIRender.h>
#include <../MDuiLib/Core/MMainXml.h>
#include <../MSelect/MLogSystem/MLogSystem.h>

#include <math.h>


#define HSLMAX   255	/* H,L, and S vary over 0-HSLMAX */
#define RGBMAX   255    /* R,G, and B vary over 0-RGBMAX */
#define HSLUNDEFINED (HSLMAX*2/3)


	//*****************************************************************************************************/	// Convert hue value to RGB
	static float HueToRGB( float v1, float v2, float vH )
	{
		if ( vH < 0.0f ) vH += 1.0f;
		if ( vH > 1.0f ) vH -= 1.0f;
		if ( ( 6.0f * vH ) < 1.0f ) return ( v1 + ( v2 - v1 ) * 6.0f * vH );
		if ( ( 2.0f * vH ) < 1.0f ) return ( v2 );
		if ( ( 3.0f * vH ) < 2.0f ) return ( v1 + ( v2 - v1 ) * ( ( 2.0f / 3.0f ) - vH ) * 6.0f );
		return ( v1 );
	}


	//	* Convert color RGB to HSL	* pHue HSL hue value			[0 - 1]		* pSat HSL saturation value		[0 - 1]		* pLue HSL luminance value		[0 - 1]
	static void RGBToHSL( DWORD clr, float *pHue, float *pSat, float *pLue )
	{
		float R = (float)( GetRValue(clr) / 255.0f );  //RGB from 0 to 255
		float G = (float)( GetGValue(clr) / 255.0f );
		float B = (float)( GetBValue(clr) / 255.0f );

		float H, S, L;

		float fMin = min( R, min(G, B) );		//Min. value of RGB
		float fMax = max( R, max(G, B) );		//Max. value of RGB
		float fDelta = fMax - fMin;				//Delta RGB value

		L = ( fMax + fMin ) / 2.0f;

		if ( fDelta == 0 )                     //This is a gray, no chroma...
		{
			H = 0.0f;                          //HSL results from 0 to 1
			S = 0.0f;
		}
		else                                   //Chromatic data...
		{
			float del_R, del_G, del_B;

			if ( L < 0.5 ) S = fDelta / ( fMax + fMin );
			else           S = fDelta / ( 2.0f - fMax - fMin );

			del_R = ( ( ( fMax - R ) / 6.0f ) + ( fDelta / 2.0f ) ) / fDelta;
			del_G = ( ( ( fMax - G ) / 6.0f ) + ( fDelta / 2.0f ) ) / fDelta;
			del_B = ( ( ( fMax - B ) / 6.0f ) + ( fDelta / 2.0f ) ) / fDelta;

			if      ( R == fMax ) H = del_B - del_G;
			else if ( G == fMax ) H = ( 1.0f / 3.0f ) + del_R - del_B;
			else if ( B == fMax ) H = ( 2.0f / 3.0f ) + del_G - del_R;

			if ( H < 0.0f ) H += 1.0f;
			if ( H > 1.0f )  H -= 1.0f;
		}

		*pHue=H;
		*pSat=S;
		*pLue=L;
	}

	//	* Convert color HSL to RGB	* H HSL hue value				[0 - 1]		* S HSL saturation value		[0 - 1]		* L HSL luminance value			[0 - 1]
	static DWORD HSLToRGB( float H, float S, float L )
	{
		BYTE R, G, B;
		float var_1, var_2;

		if ( S == 0 )                       //HSL from 0 to 1
		{
			R = G = B = (BYTE)(L * 255.0f);   //RGB results from 0 to 255
		}
		else
		{
			if ( L < 0.5 ) var_2 = L * ( 1.0f + S );
			else           var_2 = ( L + S ) - ( S * L );

			var_1 = 2.0f * L - var_2;

			R = (BYTE)( 255.0f * HueToRGB( var_1, var_2, H + ( 1.0f / 3.0f ) ) );
			G = (BYTE)( 255.0f * HueToRGB( var_1, var_2, H ) );
			B = (BYTE)( 255.0f * HueToRGB( var_1, var_2, H - ( 1.0f / 3.0f ) ) );
		}

		return RGB(R,G,B);	
	}


	//	* _HSLToRGB color HSL value to RGB	* clr  RGB color value	* nHue HSL hue value			[0 - 360]	* nSat HSL saturation value		[0 - 200]	* nLue HSL luminance value		[0 - 200]
	#define _HSLToRGB(h,s,l) (0xFF << 24 | HSLToRGB((float)h / 360.0f,(float)s / 200.0f,l / 200.0f))
	//*****************************************************************************************************/
namespace DuiLib
{
	//*****************************************************************************************************/
	CColorPaletteUI::CColorPaletteUI()
		: m_uButtonState(0)
		, m_bIsInBar(false)
		, m_bIsInPallet(false)
		, m_nCurH(30)
		, m_nCurS(180)
		, m_nCurB(100)
		, m_nAlaph(250)
		, m_nPalletHeight(0)
		, m_nBarHeight(32)
	{
		SetMinHeight( 96 );
	}

	CColorPaletteUI::~CColorPaletteUI()
	{
	}

	//*************************************************/
	void	CColorPaletteUI::DeleteThis()	//删除自己
	{
		delete this;
	}
	bool	CColorPaletteUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		if( m_pManager != NULL ) m_pManager->SendNotify(this, E_NOTIFY_Click );	//_T("click")
		return true;
	}
	int		CColorPaletteUI::AttachAttribute(MCtrlProp_Base * pCtrlPropData, CControlUI* pParentUI, IExtendUIBuilder * pExtendBuilder )		//使用MCtrlProp_Base结构里的数据来设置控件属性，函数内要求pCtrlPropData强制转换为相关的类型
	{
		int nBaseReturn = CControlUI::AttachAttribute( pCtrlPropData, pParentUI, pExtendBuilder );
		if( nBaseReturn <= 0 )
		{
			return nBaseReturn;
		}

		MCtrlData_ColorPalette & propData = *(MCtrlData_ColorPalette *)(pCtrlPropData );

		if( propData.m_nAlaph.IsDataFresh() )			{	SetPaletteAlaph( propData.m_nAlaph );	}
		if( propData.m_nBarHeight.IsDataFresh() )		{	SetBarHeight( propData.m_nBarHeight );	}
		if( propData.m_stThumbImage.IsDataFresh() )		{	SetThumbImage( propData.m_stThumbImage.c_str() );	}

		return propData.GetFreshCount_Prop();
	}
	//*************************************************/
	void	CColorPaletteUI::DoInit()
	{
		m_bmpImagePallet.MakeMemoryBmp( GetManager()->GetPaintDC(), 360, 200, 0xFA000000 );
		m_bmpImageBar.MakeMemoryBmp( GetManager()->GetPaintDC(), 200, 16, 0xFA000000 );
	}
	void	CColorPaletteUI::SetPos(RECT rc)
	{
		CControlUI::SetPos(rc);

		m_nPalletHeight = m_rcItem.bottom - m_rcItem.top - m_nBarHeight;
		if( m_nPalletHeight < 64 )
		{
			m_nPalletHeight = 64;
			m_nBarHeight = m_rcItem.bottom - m_rcItem.top - m_nPalletHeight;
			if( m_nBarHeight < 10 )
			{
				m_nBarHeight = 10;
			}
		}

		m_ptLastPalletMouse.x = m_nCurH * (m_rcItem.right - m_rcItem.left) / 360 + m_rcItem.left;
		m_ptLastPalletMouse.y = (200 - m_nCurB) * m_nPalletHeight / 200 + m_rcItem.top;
	//	m_ptLastPalletMouse.x = (m_rcItem.right + m_rcItem.left)/2;
	//	m_ptLastPalletMouse.y = m_rcItem.top + m_nPalletHeight/2;

		_UpdatePalletData();
		_UpdateBarData();
	}

	void	CColorPaletteUI::DoEvent(TEventUI& event)
	{
		CControlUI::DoEvent(event);

		if( event.Type == UIEVENT_BUTTONDOWN ) 
		{	
			if (event.ptMouse.x >= m_rcItem.left && event.ptMouse.y >= m_rcItem.top && 
				event.ptMouse.x < m_rcItem.right && event.ptMouse.y < m_rcItem.top + m_nPalletHeight)
			{
				int x = (event.ptMouse.x - m_rcItem.left) * 360 / (m_rcItem.right - m_rcItem.left);
				int y = (event.ptMouse.y - m_rcItem.top) *200 / m_nPalletHeight;
				x = min(max(x, 0), 360);
				y = min(max(y, 0), 200);

				m_ptLastPalletMouse = event.ptMouse;
				if (m_ptLastPalletMouse.x < m_rcItem.left) m_ptLastPalletMouse.x = m_rcItem.left;
				if (m_ptLastPalletMouse.x > m_rcItem.right) m_ptLastPalletMouse.x = m_rcItem.right;
				if (m_ptLastPalletMouse.y < m_rcItem.top) m_ptLastPalletMouse.y = m_rcItem.top;
				if (m_ptLastPalletMouse.y > m_rcItem.top + m_nPalletHeight) m_ptLastPalletMouse.y = m_rcItem.top + m_nPalletHeight;	
				
				m_nCurH = x;
				m_nCurB = 200-y;

				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				m_bIsInPallet = true;
				m_bIsInBar = false;

				_UpdateBarData();
			}
			if (event.ptMouse.x >= m_rcItem.left && event.ptMouse.y >= m_rcItem.bottom - m_nBarHeight && 
				event.ptMouse.x < m_rcItem.right && event.ptMouse.y < m_rcItem.bottom)
			{
				m_nCurS = (m_rcItem.right - event.ptMouse.x) * 200 / (m_rcItem.right - m_rcItem.left);
				//m_nCurS = (event.ptMouse.x - m_rcItem.left) * 200 / (m_rcItem.right - m_rcItem.left);
				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				m_bIsInBar = true;
				m_bIsInPallet = false;
				_UpdatePalletData();
			}

			//Invalidate();
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP ) 
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 )
			{
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);

				m_bIsInPallet = false;
				m_bIsInBar = false;
				//Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE ) 
		{

			if (!(m_uButtonState &UISTATE_PUSHED))
			{
				m_bIsInBar = false;
				m_bIsInPallet = false;
			}
			if (m_bIsInPallet == true)
			{
				POINT pt = event.ptMouse;
				pt.x -= m_rcItem.left;
				pt.y -= m_rcItem.top;

				if (pt.x >=0 && pt.y >= 0 && pt.x <= m_rcItem.right && pt.y <= m_rcItem.top + m_nPalletHeight)
				{
					int x = pt.x * 360 / (m_rcItem.right - m_rcItem.left);
					int y = pt.y * 200 / m_nPalletHeight;
					x = min(max(x, 0), 360);
					y = min(max(y, 0), 200);

					m_ptLastPalletMouse = event.ptMouse;
					if (m_ptLastPalletMouse.x < m_rcItem.left) m_ptLastPalletMouse.x = m_rcItem.left;
					if (m_ptLastPalletMouse.x > m_rcItem.right) m_ptLastPalletMouse.x = m_rcItem.right;
					if (m_ptLastPalletMouse.y < m_rcItem.top) m_ptLastPalletMouse.y = m_rcItem.top;
					if (m_ptLastPalletMouse.y > m_rcItem.top + m_nPalletHeight) m_ptLastPalletMouse.y = m_rcItem.top + m_nPalletHeight;	


					m_nCurH = x;
					m_nCurB = 200-y;

					_UpdateBarData();
				}
			}
			if (m_bIsInBar == true)
			{
				m_nCurS = (m_rcItem.right - event.ptMouse.x) * 200 / (m_rcItem.right - m_rcItem.left);
				//m_nCurS = (event.ptMouse.x - m_rcItem.left) * 200 / (m_rcItem.right - m_rcItem.left);
				m_nCurS = min(max(m_nCurS, 0), 200);

				_UpdatePalletData();
			}

			//Invalidate();
			return;
		}

	}

	void	CColorPaletteUI::DoPaint(HDC hDC, const RECT& rcPaint)
	{
		CControlUI::DoPaint(hDC, rcPaint);
		if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;

		PaintPallet(hDC);
	}

	void	CColorPaletteUI::PaintPallet(HDC hDC)
	{
		int nSaveDC = ::SaveDC(hDC);

		//创建剪切区域
		HRGN hClip1 = ::CreateRectRgn(m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.top + m_nPalletHeight);
		HRGN hClip2 = ::CreateRectRgn(m_rcItem.left, m_rcItem.bottom - m_nBarHeight, m_rcItem.right, m_rcItem.bottom);
		::CombineRgn(hClip1, hClip1, hClip2, RGN_OR);
		::SelectClipRgn(hDC, hClip1);
		::DeleteObject(hClip1);
		::DeleteObject(hClip2);

		::SetStretchBltMode(hDC, HALFTONE);
		//拉伸模式将内存图画到控件上
		RECT rcCorner = { 2, 2, 2, 2 };

		RECT rcBar = { m_rcItem.left, m_rcItem.bottom - m_nBarHeight, m_rcItem.right, m_rcItem.bottom };
		RECT rcBmpPartBar = { 0, 0, m_bmpImageBar.GetBmpWidth(), m_bmpImageBar.GetBmpHeight() };
		//CRenderEngine::DrawImage(hDC, m_bmpImageBar, rcBar, 256 );
		bool bDrawResult = CRenderEngine::DrawImage( hDC, m_bmpImageBar, rcBar, rcBmpPartBar, rcCorner, false, m_nAlaph );

		//StretchBlt(hDC, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_nPalletHeight, m_MemDc, 0, 0, 360, 199, SRCCOPY);
		//StretchBlt(hDC, m_rcItem.left, m_rcItem.bottom - m_nBarHeight, m_rcItem.right - m_rcItem.left, m_nBarHeight, m_MemDc, 0, 210, 200, m_nBarHeight, SRCCOPY);
		RECT rcPallet = { m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.top + m_nPalletHeight };
		RECT rcBmpPartPallet = { 0, 0, m_bmpImagePallet.GetBmpWidth(), m_bmpImagePallet.GetBmpHeight() };
		bDrawResult = CRenderEngine::DrawImage( hDC, m_bmpImagePallet, rcPallet, rcBmpPartPallet, rcCorner, false, m_nAlaph );
		//CRenderEngine::DrawImage( hDC, m_bmpImagePallet.m_hBitmap, rcPallet, m_rcItem, rcBmpPart, rcCorner, false, 200 );	//这个老出错，出你妹啊！

		RECT rcCurSorPaint = {m_ptLastPalletMouse.x - 4,m_ptLastPalletMouse.y - 4,m_ptLastPalletMouse.x + 4,m_ptLastPalletMouse.y + 4};
		//CRenderEngine::DrawImageString(hDC, m_pManager, rcCurSorPaint, m_rcPaint, m_strThumbImage);
		DrawImageEx(hDC, m_strThumbImage, rcCurSorPaint, m_rcPaint);

		//rcCurSorPaint.left = m_rcItem.left + m_nCurS * (m_rcItem.right - m_rcItem.left) / 200 - 4;
		//rcCurSorPaint.right = m_rcItem.left + m_nCurS * (m_rcItem.right - m_rcItem.left) / 200 + 4;
		rcCurSorPaint.left = m_rcItem.left + (200 - m_nCurS) * (m_rcItem.right - m_rcItem.left) / 200 - 4;
		rcCurSorPaint.right = rcCurSorPaint.left + 8;
		rcCurSorPaint.top = m_rcItem.bottom - m_nBarHeight / 2 - 4;
		rcCurSorPaint.bottom = m_rcItem.bottom - m_nBarHeight / 2 + 4;
		//CRenderEngine::DrawImageString(hDC, m_pManager, rcCurSorPaint, m_rcPaint, m_strThumbImage);
		DrawImageEx(hDC, m_strThumbImage, rcCurSorPaint, m_rcPaint);
		::RestoreDC(hDC,nSaveDC);
	}
	//*****************************************************************************************************/
	void	CColorPaletteUI::_UpdatePalletData()
	{
		//int x, y;
		//BYTE *pPiexl;
		//DWORD dwColor;
		//for(y = 0; y < 200; ++y) {
		//	for(x = 0; x < 360; ++x) {
		//		pPiexl = LPBYTE(m_pBits)+((199-y)*m_bmInfo.bmWidthBytes)+((x*m_bmInfo.bmBitsPixel)/8);
		//		//*(DWORD*)pPiexl = HSLTORGB(x, m_nCurS, y);
		//		dwColor = _HSLToRGB(x, m_nCurS, y);
		//		pPiexl[0] = GetBValue(dwColor);
		//		pPiexl[1] = GetGValue(dwColor);
		//		pPiexl[2] = GetRValue(dwColor);
		//	}
		//}
		//SetBitmapBits(m_hMemBitmap, m_bmInfo.bmWidthBytes * m_bmInfo.bmHeight, m_pBits);

		DWORD * pColorData = (DWORD *)( m_bmpImagePallet.m_pVoidDest );
		for( int x = 0; x < 360; x ++ )
		{
			for( int y = 0; y < 200; y ++ )
			{
				//DWORD dwRGB = _HSLToRGB(x, 200 - m_nCurS, y);		//这句创造了一个非常漂亮的花纹图案
				DWORD dwRGB = _HSLToRGB(x, m_nCurS, y);
				memcpy( pColorData + y * 360 + x, &dwRGB, sizeof(DWORD) );
				//pColorData[ y * 360 + x ] = _HSLToRGB(x, m_nCurS, y);
			}
		}
		//DebugSaveBitmap( _M("D://c__01.bmp"), m_bmpImagePallet.m_hBitmap );
		Invalidate();
	}

	void	CColorPaletteUI::_UpdateBarData()
	{
		//int x, y;
		//BYTE *pPiexl;
		//DWORD dwColor;
		////这里画出Bar
		//for(y = 0; y < m_nBarHeight; ++y) {
		//	for(x = 0; x < 200; ++x) {
		//		pPiexl = LPBYTE(m_pBits)+((210 + y)*m_bmInfo.bmWidthBytes)+((x*m_bmInfo.bmBitsPixel)/8);
		//		//*(DWORD*)pPiexl = _HSLToRGB(m_nCurH, x , m_nCurB);
		//		dwColor = _HSLToRGB(m_nCurH, x , m_nCurB);
		//		pPiexl[0] = GetBValue(dwColor);
		//		pPiexl[1] = GetGValue(dwColor);
		//		pPiexl[2] = GetRValue(dwColor);
		//	}
		//}
		//SetBitmapBits(m_hMemBitmap, m_bmInfo.bmWidthBytes * m_bmInfo.bmHeight, m_pBits);

		DWORD * pColorData = (DWORD *)( m_bmpImageBar.m_pVoidDest );
		for( int y = 0; y < 16; y ++ )
		{
			if( 0 == y )
			{
				for( int x = 0; x < 200; x ++ )
				{
					DWORD dwRGB = _HSLToRGB(m_nCurH, x , m_nCurB);
					memcpy( pColorData + y * 200 + 200 - 1 - x, &dwRGB, sizeof(DWORD) );
				}
			}
			else
			{
				memcpy( pColorData + y * 200, pColorData, sizeof(DWORD) * 200 );

			}
		}
		//DebugSaveBitmap( _M("D://c__02.bmp"), m_bmpImageBar.m_hBitmap );
		Invalidate();
	}
	//*****************************************************************************************************/};
	DWORD	CColorPaletteUI::GetSelectColor(bool bAlaph)
	{
		DWORD dwRawColor = _HSLToRGB(m_nCurH, m_nCurS, m_nCurB);
		if( bAlaph )
		{
			dwRawColor = dwRawColor & 0xFFFFFF;
			dwRawColor += m_nAlaph * 0x1000000;
		}
		return dwRawColor ;
	}
	void	CColorPaletteUI::SetSelectColor(DWORD dwColor) 
	{
		float H = 0, S = 0, B = 0;
		COLORREF dwBkClr = RGB(GetBValue(dwColor),GetGValue(dwColor),GetRValue(dwColor));
		RGBToHSL(dwBkClr, &H, &S, &B);
		m_nCurH = (int)(H*360);
		m_nCurS = (int)(S*200);
		m_nCurB = (int)(B*200);
		Invalidate();
	}
	void	CColorPaletteUI::SetPaletteAlaph(int nAlaph)	{	m_nAlaph = nAlaph;		Invalidate();	}
	int		CColorPaletteUI::GetPaletteAlaph()	const		{	return m_nAlaph;	}
	int		CColorPaletteUI::GetPalletHeight()	const		{	return m_nPalletHeight;		}
	void	CColorPaletteUI::SetPalletHeight(int nHeight)	{	m_nPalletHeight = nHeight;	}
	int		CColorPaletteUI::GetBarHeight()		const		{	return m_nBarHeight;		}
	void	CColorPaletteUI::SetBarHeight(int nHeight)		{	m_nBarHeight = nHeight;		}
	LPCTSTR	CColorPaletteUI::GetThumbImage()	const		{	return m_strThumbImage.c_str();	}
	void	CColorPaletteUI::SetThumbImage(LPCTSTR pszImage)
	{
		if (m_strThumbImage != pszImage)
		{
			m_strThumbImage = pszImage;
			Invalidate();
		}
	}
	LPCTSTR CColorPaletteUI::GetClass() const	{	return _M("CColorPaletteUI");	}
	LPVOID	CColorPaletteUI::GetInterface(const MString & stTypeName)
	{
		if( stTypeName == MCtrlData_ColorPalette::DuiCtrlName() ) return static_cast<CColorPaletteUI*>(this);
		return CControlUI::GetInterface( stTypeName );
	}

	//*****************************************************************************************************/
};