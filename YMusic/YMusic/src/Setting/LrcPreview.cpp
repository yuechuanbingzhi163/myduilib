#include "stdafx.h"
#include "LrcPreView.h"
#include "../YApp.h"

CLrcPreViewUI::CLrcPreViewUI()
{
	m_sLrcPreView     = _T("ÌýÒôÀÖ£¬ÓÃÒÝÌý!");
	
	CConfigMgr::spLrcConfigT  lrcConfig = theConfigMgr->GetLrcConfig();
	m_dwFontColor = lrcConfig->GetLrcDeskUnPlayedColor();
	m_sFontName = lrcConfig->GetLrcDeskFontName();
	m_iFontSize = lrcConfig->GetLrcDeskFontSize();
	m_iFontStyle = lrcConfig->GetLrcDeskFontStyle();
}

CLrcPreViewUI::~CLrcPreViewUI()
{

}

LPCTSTR CLrcPreViewUI::GetClassName()
{
	return _T("LrcPrevView");
}

LPCTSTR CLrcPreViewUI::GetClass() const
{
	return CLrcPreViewUI::GetClassName();
}

void CLrcPreViewUI::DoInit()
{
	
}

void CLrcPreViewUI::DoEvent(TEventUI& event)
{
	return __super::DoEvent(event);
}

void CLrcPreViewUI::PaintText(HDC hDC)
{
	RECT rc  = this->GetPos();

	::SetBkMode(hDC,TRANSPARENT);
	::SetTextColor(hDC, RGB(GetBValue(m_dwFontColor), GetGValue(m_dwFontColor), GetRValue(m_dwFontColor)));

	LOGFONT m_LogFont;
	memset(&m_LogFont,0,sizeof(m_LogFont));
//	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &m_LogFont);
	m_LogFont.lfWidth       = 0;
	m_LogFont.lfUnderline   = FALSE;
	m_LogFont.lfStrikeOut   = FALSE;
	m_LogFont.lfOrientation = 0;
	m_LogFont.lfEscapement  = 0;
	m_LogFont.lfCharSet     = DEFAULT_CHARSET;
	m_LogFont.lfHeight      = -m_iFontSize;
	m_LogFont.lfWeight      = FW_NORMAL | ((m_iFontStyle == 1 || m_iFontStyle ==3) ? FW_BOLD : ~FW_BOLD);
	m_LogFont.lfItalic      = (m_iFontStyle==2||m_iFontStyle==3) ? TRUE : FALSE;
	_tcscpy_s(m_LogFont.lfFaceName,m_sFontName);

// 	HFONT  hFont   = ::CreateFont(-m_iFontSize,
// 		0,
// 		0,
// 		0,
// 		FW_NORMAL | ((m_iFontStyle == 1 || m_iFontStyle ==3) ? FW_BOLD : ~FW_BOLD),
// 		(m_iFontStyle==2||m_iFontStyle==3) ? TRUE : FALSE,
// 		0,
// 		0,
// 		DEFAULT_GUI_FONT,
// 		0,
// 		0,
// 		0,
// 		0,
// 		m_sFontName);
	HFONT  hFont   = ::CreateFontIndirect(&m_LogFont);
	HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);
	::DrawText(hDC, m_sLrcPreView, -1, &rc, DT_CENTER | DT_VCENTER |DT_SINGLELINE | DT_NOPREFIX);
	::SelectObject(hDC,hOldFont);
	::DeleteObject(hFont);
}
