#include "stdafx.h"
#include "InfoWindow.h"
#include "../YApp.h"

HRGN CreateTransColorRgn(HDC dc,HWND hwnd,COLORREF TransColor)
{
	COLORREF   cl; 
	CDuiRect   cRect;
	HRGN   wndRgn; 
	::GetWindowRect(hwnd,&cRect);
	wndRgn = ::CreateRectRgn(0,0,cRect.GetWidth(),cRect.GetHeight());

	for(int x = 0,len = cRect.GetWidth(); x <= len; x++)
	{  
		int y1,y2;
		int height = cRect.GetHeight();
		for(y1 = 0; y1 <=height; y1++)
		{
			cl = ::GetPixel(dc,x,y1);
			if(cl == TransColor)
			{
				HRGN rgnTemp = ::CreateRectRgn(x,   y1,   x+1,   y1+1);
				::CombineRgn(wndRgn,wndRgn,rgnTemp,RGN_XOR);
				::DeleteObject(rgnTemp);
			} 
			else
			{
				break;
			}
		} 

		for(y2 = height-1; y2 >= y1; y2--)
		{
			cl = ::GetPixel(dc,x, y2); 
			if(cl == TransColor)
			{
				HRGN rgnTemp = ::CreateRectRgn(x,   y2,   x+1,   y2+1);
				::CombineRgn(wndRgn,wndRgn,rgnTemp, RGN_XOR); 
				::DeleteObject(rgnTemp);   
			} 
			else
			{
				break;
			}
		} 
	} 
	return wndRgn;
}
void CInfoWindow::ShowInfoWindow(HWND hparent,POINT pt,LPCTSTR pszText)
{
	ASSERT(hparent);
	if(!::IsWindow(hparent)) return;
	static	CInfoWindow info;
	if(!info.IsWindow())
	{
		info.Create(NULL,NULL,WS_VISIBLE|WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_TOPMOST);
	}
	info.ShowInfo(hparent,pt,pszText);
}
void CInfoWindow::ShowInfoWindow(HWND hparent,RECT rt,LPCTSTR pszText)
{
	POINT pt = {rt.left + 10,rt.top};
	return CInfoWindow::ShowInfoWindow(hparent,pt,pszText);
}
void CInfoWindow::ShowInfoWindow(HWND hparent,LPCTSTR pszText)
{
	assert(hparent);
	if(!::IsWindow(hparent)) return;
	CDuiRect rt;
	::GetClientRect(hparent,&rt);
	POINT pt = {rt.left + 10,rt.top};

	return CInfoWindow::ShowInfoWindow(hparent,pt,pszText);
}
void CInfoWindow::ShowInfo(HWND hparent,
	POINT pt,
	LPCTSTR pszText)
{
	assert(IsMainThread());
	this->KillTimer(_nTimerId);
	ShowWindow(SW_HIDE);
	SetText(pszText);

//	SetParent(hparent);
	AdjustWindow(hparent,pt);
	ShowWindow(SW_SHOWNORMAL);
	this->SetTimer(_nTimerId,1500);
}
void CInfoWindow::AdjustWindow(HWND hparent,POINT pt)
{
	if(hparent)
		::ClientToScreen(hparent,&pt);	

	CDuiRect rtWnd;
	GetWindowRect(&rtWnd);
	const int nWidth  = rtWnd.GetWidth();
	const int nHeight = rtWnd.GetHeight();

	rtWnd.left        = pt.x;
	rtWnd.right       = rtWnd.left + nWidth;
	rtWnd.bottom      = pt.y;// - _ptOffsize.cy;
	rtWnd.top         = rtWnd.bottom - nHeight;

	SetWindowPos(HWND_TOPMOST,&rtWnd,0);
// 	{
// 		CDuiRect rect;
// 		GetWindowRect(&rect);
// 		HDC dc = ::GetDC(m_hWnd);
// 		HDC memdc = ::CreateCompatibleDC(dc);
// 		COLORREF clr = RGB(255,0,255);
// 		COLORREF clrOld = ::SetBkColor(memdc, clr);
// 		ASSERT(clrOld != CLR_INVALID);
// 		if(clrOld != CLR_INVALID)
// 		{
// 			::ExtTextOut(memdc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
// 			::SetBkColor(memdc, clrOld);
// 		}
// 		VERIFY(CRenderEngine::DrawImageString(memdc,GetPaintMgr(),rect,rect,GetPaintMgr()->GetRoot()->GetBkImage()));
// 		HRGN hrgn = CreateTransColorRgn(memdc,m_hWnd,clr);
// 		ASSERT(hrgn);
// 		::SetWindowRgn(m_hWnd,hrgn, true);
// 		::DeleteObject(hrgn);
// 		::DeleteDC(memdc);
// 		::ReleaseDC(m_hWnd,dc);
// 	}
	Invalidate(FALSE);
}

void CInfoWindow::SetText(LPCTSTR pszText)
{
	CLabelUI* ui = NULL;
	FindControl(ui,_T("msg"));
	ASSERT(ui);
	if(!ui)
		return;


	HDC dc = ::GetDC(m_hWnd);
	RECT rt = {0};
	::DrawText(dc,pszText,wcslen(pszText),&rt,DT_CALCRECT);
	::ReleaseDC(m_hWnd,dc);

	ui->SetText(pszText);
	
	CDuiRect rtWnd;
	GetWindowRect(&rtWnd);
	rtWnd.right = rtWnd.left + std::min<LONG>(rt.right,MaxWidth) + 10;
	rtWnd.bottom = rtWnd.top + std::min<LONG>(rt.bottom,MaxHeight) + 4;

	MoveWindow(&rtWnd,FALSE);
}

LRESULT CInfoWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	this->RegisterSkin(_T("InfoWnd.xml"),0);
	return 1;
}
LRESULT CInfoWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(_nTimerId != (UINT_PTR)wParam)
		return 0;

	if(IsWindow() && IsWindowVisible())
		ShowWindow(SW_HIDE);
	this->KillTimer((UINT_PTR)wParam);
	return 0;
}
LRESULT CInfoWindow::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(WM_TIMER==uMsg)
		return OnTimer(uMsg,wParam,lParam,bHandled);
	return __super::ProcessWindowMessage(uMsg,wParam,lParam,bHandled);
}
