#include "stdafx.h"
#include "DesktopLrcWindow.h"
#include "../YApp.h"
//#include <WinGDI.h>

#include <GdiPlus.h>
using namespace Gdiplus;
#pragma  comment(lib,  "gdiplus.lib")

const static int OVERRECTOFFSET = 3;
CDesktopLrcWindow::CDesktopLrcWindow()
{
	m_bLock = false;
	m_bDrawBk =false;
	m_bMouseOn = false;
	m_bTrace = false;
	m_strOnStop = _T("逸听音乐");
}

CDesktopLrcWindow::~CDesktopLrcWindow()
{

}
LPCTSTR CDesktopLrcWindow::GetWindowClsss() 
{
	return _T("DesktopLrcWindow");
}
LPCTSTR CDesktopLrcWindow::GetWindowClassName() const
{
	return CDesktopLrcWindow::GetWindowClsss();
}
LRESULT CDesktopLrcWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	YTrace_(DUI__TraceMsg(uMsg));
	return __super::HandleMessage(uMsg,wParam,lParam);
}
LRESULT CDesktopLrcWindow::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg==WM_TIMER)
		return OnTimer(uMsg,wParam,lParam,bHandled);
	else if(uMsg==WM_PAINT)
		return OnPaint(uMsg,wParam,lParam,bHandled);
// 	else if(uMsg==WM_NCMOUSELEAVE)
// 		return OnNCMouseLeave(uMsg,wParam,lParam,bHandled);
// 	else if(uMsg==WM_SHOWWINDOW)
// 		return OnShowWindow(uMsg,wParam,lParam,bHandled);
// 	else if(uMsg==WM_MOVING)
// 		return OnMoving(uMsg,wParam,lParam,bHandled);
	else
		return __super::ProcessWindowMessage(uMsg,wParam,lParam,bHandled);
}
LRESULT CDesktopLrcWindow::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	YTrace_(_T("CDesktopLrcWindow::OnNcHitTest"));
//	PostMessage(WM_MOVING,)
	return HTCAPTION;
}
LRESULT CDesktopLrcWindow::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam==SC_MINIMIZE)
	{
		ShowWindow(false);
		m_spLrcToolBar->ShowWindow(false);
		m_spLrcMask->ShowWindow(false);
	}
	bHandled = TRUE;
	return 0;
}
LRESULT CDesktopLrcWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sLog("CDesktopLrcWindow::OnCreate");

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


 	BOOL nRet = ::SetWindowPos(*this,HWND_TOPMOST,rc.left,rc.top,rc.GetWidth(),rc.GetHeight(),SWP_NOACTIVATE|SWP_NOSENDCHANGING );
	ModifyStyleEx(WS_EX_APPWINDOW,/*WS_EX_TRANSPARENT|*/WS_EX_LAYERED|WS_EX_TOOLWINDOW);//加入WS_EX_TRANSPARENT扩展属性 

	assert(nRet!=0);

	//歌词工具条;
	m_spLrcToolBar = spLrcToolBarT(new CLrcToolBar(this));
	m_spLrcToolBar->Create(this->m_hWnd,NULL,WS_POPUP,0);
	DuiLib::CDuiRect rcBar;
	m_spLrcToolBar->GetWindowRect(&rcBar);
	int nXBarPos = rc.left + (rc.GetWidth()-rcBar.GetWidth())/2;
	int nYBarPos = rc.top - rcBar.GetHeight()+OVERRECTOFFSET;
	m_spLrcToolBar->MoveWindow(nXBarPos,nYBarPos,rcBar.GetWidth(),rcBar.GetHeight());
	m_spLrcToolBar->ShowWindow(false);
	//歌词背景层
	m_spLrcMask = spLrcMaskT(new CLrcMask(this));
	m_spLrcMask->Create(this->m_hWnd,NULL,UI_WNDSTYLE_FRAME|WS_POPUP,0);
	m_spLrcMask->MoveWindow(&rc);
	m_spLrcMask->ShowWindow(false);

	m_nTimerId = SetTimer(135,500);
	return nRet;
}
LRESULT CDesktopLrcWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PAINTSTRUCT ps = {0};
	::BeginPaint(this->m_hWnd,&ps);
	::EndPaint(this->m_hWnd,&ps);
	DrawDesktopUI();
	return 0;
}
LRESULT CDesktopLrcWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		//int nStatus = (int)thePlayerCenter->GetPlayStatus();
		//switch (nStatus)
		//{
		//case pcs_idle://空闲;
		//case pcs_playing://正在播放;
		//	/*RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);*/
		//	{
		//		RedrawWindow(m_hWnd, NULL,NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE);
		//	}				
		//	break;
		//case pcs_pause://暂停;
		//	break;			
		//}
	}
	return 0;
}

LRESULT CDesktopLrcWindow::OnNCMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!m_bTrace)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = this->m_hWnd;
		tme.dwFlags = TME_LEAVE |TME_NONCLIENT;
		tme.dwHoverTime = 1;
		m_bTrace = _TrackMouseEvent(&tme)==0 ? false : true;
	}	
	//SetCursor(IDC_HAND);
	SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(32651)));
	m_bMouseOn = true;
	m_bDrawBk = true;
	YTrace_(m_bMouseOn?_T("CDesktopLrcWindow::OnNCMouseMove:m_bMouseOn=true"):_T("CDesktopLrcWindow::OnNCMouseMove:m_bMouseOn=false"));
	return 0;
}

LRESULT CDesktopLrcWindow::OnNCMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bTrace = false;
	m_bMouseOn = false;

	bool bBarTag = m_spLrcToolBar->GetMouseLeaveTag();
	bool bMaskTag = m_spLrcMask->GetMouseLeaveTag();
	m_bDrawBk = m_bMouseOn|bBarTag|bMaskTag ;
	YTrace_(m_bMouseOn?_T("CDesktopLrcWindow::OnNCMouseLeave:m_bMouseOn=true"):_T("CDesktopLrcWindow::OnNCMouseLeave:m_bMouseOn=false"));
	YTrace_(m_bDrawBk?_T("CDesktopLrcWindow::OnNCMouseLeave:m_bDrawBk=true"):_T("CDesktopLrcWindow::OnNCMouseLeave:m_bDrawBk=false"));

	SetCursor(LoadCursor(NULL,IDC_ARROW));
	return 0;
}
LRESULT CDesktopLrcWindow::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	
	return 0;
}

LRESULT CDesktopLrcWindow::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	YTrace_(_T("CDesktopLrcWindow::OnMoving"));
	LPRECT pRect = (LPRECT)lParam;
	int nWidth = pRect->right - pRect->left;
	int nHeight = pRect->bottom - pRect->top;

	DuiLib::CDuiRect rc,rcBar,rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	GetWindowRect(&rc);
	m_spLrcToolBar->GetWindowRect(&rcBar);
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
	assert(m_spLrcToolBar);
	m_spLrcToolBar->MoveWindow(pRect->left + (nWidth-nBarWidth)/2,
		pRect->top - nBarHeight+OVERRECTOFFSET,nBarWidth,nBarHeight);
//	SetCursor(LoadCursor(NULL,IDC_HAND));
	return 0;
}
LRESULT CDesktopLrcWindow::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	//判断是否需要画背景;
	if(!m_spLrcToolBar||!m_spLrcToolBar->IsWindow()
		||!m_spLrcMask||!m_spLrcMask->IsWindow())
		return 0;
	if (m_bDrawBk&&!m_bLock)
	{
		m_spLrcToolBar->ShowWindow(true);
		m_spLrcMask->ShowWindow(true);
	}
	else
	{
		if (m_spLrcToolBar->IsWindowVisible())
			m_spLrcToolBar->ShowWindow(false);
		if (m_spLrcMask->IsWindowVisible())
			m_spLrcMask->ShowWindow(false);
	}
	return 0;
}
void CDesktopLrcWindow::LockLrc(bool bLock)
{
	if (m_bLock!=bLock)
	{
		m_bLock=bLock;
		if (bLock)
			ModifyStyleEx(0,WS_EX_TRANSPARENT);
		else
			ModifyStyleEx(WS_EX_TRANSPARENT,0);
	}
}

bool CDesktopLrcWindow::IsLock()const
{
	return m_bLock;
}

bool CDesktopLrcWindow::GetMouseLeaveTag()
{
	return m_bMouseOn;
}

void CDesktopLrcWindow::SetBkTag(bool bDrawBK)
{
	m_bDrawBk = bDrawBK;
}

void CDesktopLrcWindow::DrawSpecialText(HDC hdc,const CString& strText,LPRECT lpRect)
{
	Graphics graphics(hdc);

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	
	StringFormat strformat;
	strformat.SetAlignment(StringAlignmentCenter);
	strformat.SetLineAlignment(StringAlignmentCenter);
	strformat.SetFormatFlags(StringFormatFlagsLineLimit);

	RectF rect((Gdiplus::REAL)lpRect->left,(Gdiplus::REAL)lpRect->top,(Gdiplus::REAL)(lpRect->right-lpRect->left),(Gdiplus::REAL)(lpRect->bottom-lpRect->top));

	GraphicsPath path;

	LPCTSTR fontnames[] = {_T("微软雅黑"),_T("宋体")};

	for (int i =0;i<sizeof(fontnames)/sizeof(fontnames[0]);++i)
	{
		FontFamily fontFamily(fontnames[i]);
		if(fontFamily.IsAvailable())
		{
			path.AddString(strText,strText.GetLength(),&fontFamily,
				FontStyleBold,40,rect,&strformat);
			break;
		}
	}

	Pen pen(Color(240,252,178,142),3);

	graphics.DrawPath(&pen,&path);


	LinearGradientBrush linGrBrush(
		Point(lpRect->left,lpRect->top),Point(lpRect->left,lpRect->bottom),
		Color(255,252,178,142),
		Color(240,255,127,80));

	LinearGradientBrush linGrBrushW(
		Point(lpRect->left,lpRect->top),Point(lpRect->left,lpRect->bottom),
		Color(255,252,178,142),
		Color(240,255,99,71));

	for(int i=1; i<9; i+=1)
	{
		Pen pen(Color(62, 0, 2, 2), (Gdiplus::REAL)i);//字体边框阴影;
		pen.SetLineJoin(LineJoinRound);
		graphics.DrawPath(&pen, &path);
	} 

	//判断是否需要画背景;
	if (m_bDrawBk&&!m_bLock)
	{
// 		SolidBrush brush(Color(50,200,200,200));
// 		Pen pen1(Color(155,223,223,223));
// 		Pen pen2(Color(55,223,223,223));
// 
// 		graphics.FillRectangle(&brush,rect);
// 		graphics.DrawRectangle(&pen1,rect.GetLeft()+2,rect.GetTop()+2,rect.Width - 4,rect.Height -4);
// 		graphics.DrawRectangle(&pen2,rect.GetLeft()+1,rect.GetTop()+1,rect.Width - 2,rect.Height -2);
				
		m_spLrcToolBar->ShowWindow(true);
		m_spLrcMask->ShowWindow(true);
	}
	else
	{
		if (m_spLrcToolBar->IsWindowVisible())
		{
			m_spLrcToolBar->ShowWindow(false);
		}
		if (m_spLrcMask->IsWindowVisible())
		{
			m_spLrcMask->ShowWindow(false);
		}
	}

	graphics.FillPath(&linGrBrush,&path);
	graphics.FillPath(&linGrBrushW,&path);

	return;
}

void CDesktopLrcWindow::DrawDesktopUI()
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

	if (/*thePlayCenter->GetPlayState()==EP_STOP*/
		/*|| thePlayCenter->GetPlayState()==EP_INIT*/
		thePlayer->GetStatus()==em_stop
		|| !m_splrcFileReader||m_splrcFileReader->Lyrics.size()==0) //空闲或者木有歌词;
	{
		DrawSpecialText(hMenDc,m_strOnStop,&rc);
	}
	else
	{		
		if (m_splrcFileReader->Lyrics.size() > 0)
		{
			if (m_splrcFileReader->Lyrics[0].length == 0)
			{
				int nItemCount = m_splrcFileReader->Lyrics .size();
				for (int i = 0; i < nItemCount; i++)
				{
					SIZE size;
					GetTextExtentPoint32(hMenDc,m_splrcFileReader->Lyrics[i].sLyric,
						m_splrcFileReader->Lyrics[i].sLyric.GetLength(),
						&size);
					m_splrcFileReader->Lyrics[i].length=size.cx;
				}
			}
		}
		float  fTimeOffset  = theLrcMgr->GetLrcTimeOffset();
//		int nCurPlayTime = thePlayCenter->GetPosition()*1000 + (int)(fTimeOffset*1000)/* NowTime- m_nStartTime*/;
		int nCurPlayTime = thePlayer->GetTime()  + (int)(fTimeOffset*1000);
		UINT nCurShowIndex = 0;
		int nSubNodeTime =0;
		int nOffsetTime = 0;
		while (nCurShowIndex + 1 < m_splrcFileReader->Lyrics.size())
		{
			int CurNodeTime = m_splrcFileReader->Lyrics[nCurShowIndex].StartShowTime;
			int NextNodeTime= m_splrcFileReader->Lyrics[nCurShowIndex+1].StartShowTime;			 
			if (nCurPlayTime >NextNodeTime)
			{
				nCurShowIndex++;
			}
			else
			{
				nSubNodeTime = NextNodeTime - CurNodeTime;
				nOffsetTime =nCurPlayTime - CurNodeTime;
				break;
			}
		}
		CString sText = m_splrcFileReader->Lyrics[nCurShowIndex].sLyric;
		if (sText.IsEmpty())
		{
			sText=_T("●●●●●●");
		}
		DrawSpecialText(hMenDc,sText,&rc);
	}

	DuiLib::CDuiRect rtWnd;
	GetWindowRect(&rtWnd);

	DuiLib::CPoint dstPt(0,0);
	DuiLib::CPoint winPt(rtWnd.left,rtWnd.top);
	DuiLib::CSize size(rc.GetWidth(),rc.GetHeight());
	BLENDFUNCTION blend = {AC_SRC_OVER,0,255,AC_SRC_ALPHA};
	BOOL bret=::UpdateLayeredWindow(m_hWnd,hTempDc,&winPt,&size,hMenDc,&dstPt,0,&blend,ULW_ALPHA);
	DeleteDC(hMenDc);
	ReleaseDC(m_hWnd,hTempDc);	
	//assert(bret)有Bug
//	assert(bret);
}

CDesktopLrcWindow::spLrcToolBarT CDesktopLrcWindow::GetLrcToolBar() const
{
	return m_spLrcToolBar;
}
CDesktopLrcWindow::spLrcMaskT CDesktopLrcWindow::GetLrcMask() const
{
	return m_spLrcMask;
}
void CDesktopLrcWindow::OnLrcChanged(/*CNotifyCenter::NotifyT& notify*/)
{
	m_splrcFileReader = theLrcMgr->GetLrcReader2();
	spSongInfoT spSong = thePlayListMgr->GetCurSong();
	if (spSong&&m_splrcFileReader->Lyrics.size()==0)
	{
		SetStopText(spSong->GetSongName());
	}
}
void CDesktopLrcWindow::LrcChange()
{
	OnLrcChanged();
}