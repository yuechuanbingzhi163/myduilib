#include "stdafx.h"
#include "LrcView.h"
#include "../YApp.h"

#include <GdiPlus.h>
#include <GdiPlusGraphics.h>
#include <GdiPlusPen.h>
using namespace Gdiplus;


CLrcViewUI::CLrcViewUI()
{
	m_bIsDrag         = false;
	m_sOnStopShow     = _T("libyyu@qq.com");

}

CLrcViewUI::~CLrcViewUI()
{

}

LPCTSTR CLrcViewUI::GetClassName()
{
	return _T("LrcView");
}

LPCTSTR CLrcViewUI::GetClass() const
{
	return CLrcViewUI::GetClassName();
}

LPVOID CLrcViewUI::GetInterface(LPCTSTR pstrName)
{
	try
	{
		if( _tcscmp(pstrName, _T("LrcView")) == 0 ) return static_cast<CLrcViewUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}
	catch (...)
	{
		throw "CLrcViewUI::GetInterface";
	}
}

void CLrcViewUI::DoInit()
{
	
}

void CLrcViewUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_BUTTONDOWN)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(32651)));
	}
	else if(event.Type == UIEVENT_TIMER && event.wParam == 1)
	{
		CContainerUI* pUI = (CContainerUI*)(this->GetParent()->GetParent());
		CButtonUI *pSearchLrcBtn = (CButtonUI*)pUI->FindSubControl(varMainWindow::kSearchBtn);
		assert(pSearchLrcBtn);
		if (pSearchLrcBtn)
		{
			if(!m_spLrcReader
				||(m_spLrcReader&&!m_spLrcReader->Lyrics.empty())
				||thePlayer->GetStatus()==em_stop)
				pSearchLrcBtn->SetVisible(false);
			else 
				pSearchLrcBtn->SetVisible(true);
		}
		this->Invalidate();
	}
	else if(UIEVENT_KEYDOWN == event.Type && (event.wKeyState&MK_CONTROL) && thePlayer->GetStatus() != em_stop)
	{
		switch( event.chKey )
		{
		case VK_UP: 
			{
				YTrace_(_T("LrcView VK_UP\r\n"));
				theLrcMgr->SetLrcBack();
				CDuiRect rc  = this->GetPos();
				POINT pt = {(rc.left+rc.right)/2-50,(rc.top+rc.bottom)/2};
				CString sInfo;
				sInfo.Format(_T("¸è´ÊÍËºó%0.1fs"),theLrcMgr->GetLrcTimeOffset() < 0 ? 0-theLrcMgr->GetLrcTimeOffset():theLrcMgr->GetLrcTimeOffset());
				CInfoWindow::ShowInfoWindow(theMainWin->GetHWND(),pt,sInfo);
			}
			break;
		case VK_DOWN:
			{
				YTrace_(_T("LrcView VK_DOWN\r\n"));
				theLrcMgr->SetLrcForward();
				CDuiRect rc  = this->GetPos();
				POINT pt = {(rc.left+rc.right)/2-50,(rc.top+rc.bottom)/2};
				CString sInfo;
				sInfo.Format(_T("¸è´ÊÇ°½ø%0.1fs"),theLrcMgr->GetLrcTimeOffset() < 0 ? 0-theLrcMgr->GetLrcTimeOffset():theLrcMgr->GetLrcTimeOffset());
				CInfoWindow::ShowInfoWindow(theMainWin->GetHWND(),pt,sInfo);
			}
			break;
		default:
			return __super::DoEvent(event);
		}
	}
	else
		return __super::DoEvent(event);
}

void CLrcViewUI::PaintText(HDC hDC)
{
// 	Gdiplus::Color BKColor     = theLrcMgr->GetBkColor();
// 	Gdiplus::Color CurRowColor = theLrcMgr->GetCurRowTextColor();
// 	Gdiplus::Color NorRowColor = theLrcMgr->GetNorRowTextColor();

	Gdiplus::Color PlayedColor = theConfigMgr->GetLrcConfig()->GetLrcPanelPlayedColor();
	Gdiplus::Color PlayingColor = theConfigMgr->GetLrcConfig()->GetLrcPanelPlayingColor();
	Gdiplus::Color UnPlayColor = theConfigMgr->GetLrcConfig()->GetLrcPanelUnPlayedColor();

	COLORREF upRow = PlayedColor.ToCOLORREF();
	COLORREF curRow = PlayingColor.ToCOLORREF();
	COLORREF downRow = UnPlayColor.ToCOLORREF();

// 	COLORREF curRow  = CurRowColor.ToCOLORREF();
// 	COLORREF norRow  = NorRowColor.ToCOLORREF();
// 	COLORREF bkcolor = BKColor.ToCOLORREF();

	CDuiRect rc  = this->GetPos();
	HRGN hRgn    = ::CreateRectRgnIndirect(&rc);
	HRGN hOldRgn = (HRGN)::SelectObject(hDC,hRgn);
	CDuiRect rcClient(rc.left,rc.top,rc.right,rc.bottom);

	HDC memDC = hDC;
	SetBkMode(memDC,TRANSPARENT);

	float  fTimeOffset     = theLrcMgr->GetLrcTimeOffset();
	size_t nLineHeight     = theLrcMgr->GetLineHeight();
	int    nTotalHeight    = rcClient.bottom + rcClient.top;
	size_t nTotalLines     = nTotalHeight / nLineHeight;
	int    nTimeElapse     = 0;
	int    nBeginShowIndex = 0;
	size_t nCurShowIndex   = 0;

	COLORREF dwCurTextColor = 0;

	HFONT  hFont   = ::CreateFontIndirect(&theLrcMgr->GetFont());
	HFONT  hOld    = (HFONT)::SelectObject(hDC,hFont);

	if (thePlayer->GetStatus()==em_stop
		|| !m_spLrcReader ||m_spLrcReader->Lyrics.size()==0) //¿ÕÏÐ»òÕßÄ¾ÓÐ¸è´Ê;
	{
		::SetTextColor(hDC,curRow);
		::DrawText(hDC,m_sOnStopShow,m_sOnStopShow.GetLength(),&rc, DT_CENTER | DT_VCENTER |DT_SINGLELINE);
	}
	else
	{
		if (m_spLrcReader->Lyrics.size() > 0)
		{
			if (m_spLrcReader->Lyrics[0].length == 0)
			{
				int nItemCount = m_spLrcReader->Lyrics .size();
				for (int i = 0; i < nItemCount; i++)
				{
					SIZE size;
					GetTextExtentPoint32(memDC,m_spLrcReader->Lyrics[i].sLyric,m_spLrcReader->Lyrics[i].sLyric.GetLength(),&size);
					m_spLrcReader->Lyrics[i].length=size.cx;
				}
			}
		}

		nTimeElapse = (thePlayer->GetTime()) + (int)(fTimeOffset*1000);
		while (nCurShowIndex + 1 < m_spLrcReader->Lyrics.size())
		{
			int NextNodeTime = m_spLrcReader->Lyrics[nCurShowIndex+1].StartShowTime;
			if (nTimeElapse > NextNodeTime)
				nCurShowIndex++;
			else 
				break;
		}
	
		int DisplayTime   = 0;
		int DisplayOffset = 0;

		if (nCurShowIndex + 1 < m_spLrcReader->Lyrics.size())
		{
			if (nCurShowIndex >= 0)
			{	
				DisplayTime   =	m_spLrcReader->Lyrics[nCurShowIndex+1].StartShowTime - m_spLrcReader->Lyrics[nCurShowIndex].StartShowTime;
				DisplayOffset = (int)(( float(nTimeElapse -m_spLrcReader->Lyrics[nCurShowIndex].StartShowTime)) / DisplayTime * nLineHeight);
			}
			else
			{
				DisplayTime   =	m_spLrcReader->Lyrics[nCurShowIndex+1].StartShowTime;
				DisplayOffset = (int)(( float(nTimeElapse )) / DisplayTime * nLineHeight);
			}
		}

		nBeginShowIndex = nCurShowIndex - nTotalLines - 1;

		if (nBeginShowIndex < 0)
			nBeginShowIndex = 0;

		for (unsigned int i = nBeginShowIndex; i < m_spLrcReader->Lyrics .size(); i++)
		{
			int top   =  nTotalHeight / 2 - (nCurShowIndex - i) * nLineHeight - DisplayOffset;
			RECT rect = { rcClient.left, top, rcClient.right, top + nLineHeight };
			if (top > nTotalHeight )
				break;
			if(i < nCurShowIndex)
			{
				dwCurTextColor = upRow;
				CRenderEngine::DrawText(hDC,m_pManager,rect,m_spLrcReader->Lyrics[i].sLyric,dwCurTextColor,0,DT_CENTER | DT_VCENTER |DT_SINGLELINE);
			}
			else if (i == nCurShowIndex)
			{
				dwCurTextColor = curRow;
				CRenderEngine::DrawText(hDC,m_pManager,rect,m_spLrcReader->Lyrics[i].sLyric,dwCurTextColor,7,DT_CENTER | DT_VCENTER |DT_SINGLELINE);
			}
			else
			{
				dwCurTextColor = downRow;
				CRenderEngine::DrawText(hDC,m_pManager,rect,m_spLrcReader->Lyrics[i].sLyric,dwCurTextColor,3,DT_CENTER | DT_VCENTER |DT_SINGLELINE);
			}
		}

	}
	::SelectClipRgn(hDC, hOldRgn);
	::SelectObject(hDC,hOld);
	::DeleteObject(hFont);
	::DeleteObject(hRgn);
}
void CLrcViewUI::LrcChanged(spSongInfoT song)
{
	m_pManager->SetTimer(this,1,1000);
	m_spLrcReader = theLrcMgr->GetLrcReader2();
}


