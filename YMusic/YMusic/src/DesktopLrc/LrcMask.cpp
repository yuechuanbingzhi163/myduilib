#include "stdafx.h"
#include "LrcMask.h"
#include "../YApp.h"
#include "DesktopLrcWindow.h"

const static int OVERRECTOFFSET = 3;

CLrcMask::CLrcMask(CDesktopLrcWindow* pDeskLrc)
{
	m_pDeskLrc = pDeskLrc;
	m_bMouseOn = false;
	m_bTrace = false;
	m_nTimerId = 0;
}
CLrcMask::~CLrcMask()
{

}
LPCTSTR CLrcMask::GetWindowClsss() 
{
	return _T("LrcMask");
}
LPCTSTR CLrcMask::GetWindowClassName() const
{
	return CLrcMask::GetWindowClsss();
}
LRESULT CLrcMask::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if(!RegisterSkin(_T("lrc.xml")))
		return 0;
	DuiLib::CDuiRect rc;
	DuiLib::CDuiRect rect;
	//GetSystemMetrics(SM_CYFULLSCREEN)计算的屏幕客户工作区域不正确;by daodao 2012-11-27;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	rc.left =100;
	rc.right = rect.GetWidth() - rc.left;
	rc.bottom = rect.GetHeight();
	rc.top = rc.bottom - 80;

	DuiLib::CDuiRect rcBar;
	GetWindowRect(&rcBar);
	BOOL nRet = ::SetWindowPos(*this,HWND_TOPMOST,rc.left,rc.top,rc.GetWidth(),rc.GetHeight(),SWP_NOACTIVATE|SWP_NOSENDCHANGING );
	ModifyStyleEx(WS_EX_APPWINDOW,/*WS_EX_TRANSPARENT|*/WS_EX_LAYERED|WS_EX_TOOLWINDOW);//加入WS_EX_TRANSPARENT扩展属性 

	GetPaintMgr()->AddNotifier(this);
	DrawBkMask();
	m_nTimerId = SetTimer(135,500);
	return 0;
}
void CLrcMask::Notify(TNotifyUI& msg)
{

} 
LRESULT CLrcMask::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnPaint(uMsg,wParam,lParam,bHandled);
	DrawBkMask();
	return 0;
}
LRESULT CLrcMask::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_nTimerId!= (UINT_PTR)wParam)
	{
		bHandled = FALSE;
		return 0;
	}
	if (!this->IsWindowVisible())
		return 0;
	if ((UINT_PTR)wParam==m_nTimerId)
	{
		RedrawWindow(m_hWnd, NULL,NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
	}
	return 0;
}

LRESULT CLrcMask::OnNCMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!m_bTrace)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = this->m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_NONCLIENT;
		tme.dwHoverTime = 10;
		m_bTrace = _TrackMouseEvent(&tme)==0 ? false : true;
	}	
	DuiLib::CDuiRect rect;
	GetWindowRect(&rect);
	DuiLib::CPoint point = DuiLib::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	RECT sizebox;
	sizebox = GetPaintMgr()->GetSizeBox();
	rect.left+= sizebox.left;
	rect.top+=sizebox.top;
	rect.right-=sizebox.right;
	rect.bottom-=sizebox.bottom;
	if (::PtInRect(&rect,point))
	{
		SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(32651)));
	}
	
	m_bMouseOn = true;
//	bHandled = false;
	
	YTrace_(m_bMouseOn?_T("CLrcMask::OnMouseMove:m_bMouseOn=true"):_T("CLrcMask::OnMouseMove:m_bMouseOn=false"));
	return 0;
}
LRESULT CLrcMask::OnNCMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bTrace = false;
	m_bMouseOn = false;
	YTrace_(m_bMouseOn?_T("CLrcMask::OnMouseLeave:m_bMouseOn=true"):_T("CLrcMask::OnMouseLeave:m_bMouseOn=false"));
	if (m_pDeskLrc&&!m_pDeskLrc->GetMouseLeaveTag())
	{
		m_pDeskLrc->SetBkTag(false|m_pDeskLrc->GetLrcToolBar()->GetMouseLeaveTag());
	}
	SetCursor(LoadCursor(NULL,IDC_ARROW));
//	bHandled = false;
	return 0;
}

bool CLrcMask::GetMouseLeaveTag()
{
	return m_bMouseOn;
}
LRESULT CLrcMask::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	YTrace_(_T("CLrcMask::OnMoving"));
	bHandled = FALSE;
	LPRECT pRect = (LPRECT)lParam;
	int nWidth = pRect->right - pRect->left;
	int nHeight = pRect->bottom - pRect->top;
//	m_pDeskLrc->SendMessage(WM_MOVING,0,lParam);
	if(m_pDeskLrc)m_pDeskLrc->SetBkTag(true);
	DuiLib::CDuiRect rc,rcBar,rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	GetWindowRect(&rc);
	if(m_pDeskLrc)m_pDeskLrc->GetLrcToolBar()->GetWindowRect(&rcBar);
	int nBarHeight = rcBar.GetHeight();
	int nBarWidth = rcBar.GetWidth();
	if (pRect->left < 0 )
	{
		pRect->left = 0;
		pRect->right = nWidth;
	}
	else if (pRect->left > rect.GetWidth() - nWidth)
	{
		pRect->right = rect.GetWidth();
		pRect->left = rect.GetWidth() - nWidth;
	}

	if (pRect->top < nBarHeight-OVERRECTOFFSET)
	{
		pRect->top = nBarHeight-OVERRECTOFFSET;
		pRect->bottom = pRect->top + nHeight;
	}
	else if (pRect->bottom > rect.GetHeight())
	{
		pRect->bottom =rect.GetHeight();
		pRect->top = pRect->bottom - nHeight;
	}
	if(m_pDeskLrc)m_pDeskLrc->MoveWindow(pRect);
	if(m_pDeskLrc)m_pDeskLrc->GetLrcToolBar()->MoveWindow(pRect->left + (nWidth-nBarWidth)/2,
		pRect->top - nBarHeight+OVERRECTOFFSET,nBarWidth,nBarHeight);
	

	SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(32651)));

	DrawBkMask();
	return 0;
}
LRESULT CLrcMask::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg==WM_TIMER)
		return OnTimer(uMsg,wParam,lParam,bHandled);
	if(uMsg==WM_PAINT)
		return OnPaint(uMsg,wParam,lParam,bHandled);
	return __super::ProcessWindowMessage(uMsg,wParam,lParam,bHandled);
}
void CLrcMask::DrawBkMask()
{
	HDC hTempDc = GetDC(m_hWnd);
	HDC hMenDc = CreateCompatibleDC(hTempDc);

	DuiLib::CDuiRect rc;
	GetClientRect(&rc);

	HBITMAP hbmp;
	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biHeight = rc.GetHeight();
	bitmapinfo.bmiHeader.biWidth = rc.GetWidth();
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biCompression=BI_RGB;
	bitmapinfo.bmiHeader.biXPelsPerMeter=0;
	bitmapinfo.bmiHeader.biYPelsPerMeter=0;
	bitmapinfo.bmiHeader.biClrUsed=0;
	bitmapinfo.bmiHeader.biClrImportant=0;
	bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight * bitmapinfo.bmiHeader.biBitCount / 8;

	hbmp = CreateDIBSection(hMenDc,&bitmapinfo,0,NULL,0,0);
	SelectBitmap(hMenDc,hbmp);

	Graphics graphics(hMenDc);

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	SolidBrush brush(Color(50,200,200,200));
	Pen pen1(Color(155,223,223,223));
	Pen pen2(Color(55,223,223,223));
	LPRECT lpRect = &rc;
	RectF rect((Gdiplus::REAL)lpRect->left,(Gdiplus::REAL)lpRect->top,(Gdiplus::REAL)(lpRect->right-lpRect->left),(Gdiplus::REAL)(lpRect->bottom-lpRect->top));

	graphics.FillRectangle(&brush,rect);
	graphics.DrawRectangle(&pen1,rect.GetLeft()+2,rect.GetTop()+2,rect.Width - 4,rect.Height -4);
	graphics.DrawRectangle(&pen2,rect.GetLeft()+1,rect.GetTop()+1,rect.Width - 2,rect.Height -2);

	DuiLib::CDuiRect rtWnd;
	GetWindowRect(&rtWnd);

	DuiLib::CPoint dstPt(0,0);
	DuiLib::CPoint winPt(rtWnd.left,rtWnd.top);
	DuiLib::CSize size(rc.GetWidth(),rc.GetHeight());
	BLENDFUNCTION blend = {AC_SRC_OVER,0,255,AC_SRC_ALPHA};
	BOOL bret = ::UpdateLayeredWindow(m_hWnd,hTempDc,&winPt,&size,hMenDc,&dstPt,0,&blend,ULW_ALPHA);
	DeleteDC(hMenDc);
	ReleaseDC(m_hWnd,hTempDc);	

//	assert(bret);
}