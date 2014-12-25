#include "StdAfx.h"
#include "tray.h"
#include "Msg.h"
#include "mainwindow.h"
#include "config.h"


const LPCTSTR CTray::CLASSNAME = _T("LoginWnd");
CTray* CTray::m_hInstance = NULL;
CTray::CTray()
	: m_pMsgList(NULL)
{
	
}
CTray::~CTray()
{

}
CTray* CTray::GetInstance(HWND hWnd)
{
	if(m_hInstance==NULL)
	{
		m_hInstance = new CTray();
		m_hInstance->Create(hWnd,_T(""),WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
	}
	m_hInstance->ShowWindow(true);
	return m_hInstance;
}
LPCTSTR CTray::GetWindowClassName() const
{
	return CTray::CLASSNAME;
}

CControlUI* CTray::CreateControl(LPCTSTR pstrClass)
{
	try
	{
		if(_tcscmp(pstrClass,_T("MsgList"))==0)
			return new  CListUI();
		return NULL;
	}
	catch (...)
	{
		return NULL;
	}
	
}
void CTray::Notify(TNotifyUI& msg)
{
	try
	{
		if( 0 == msg.sType.Compare(_T("windowinit"))){
			TypeInit(msg);
		}
		else if( 0 == msg.sType.Compare(_T("click")) ){
			TypeClick(msg);
		}		
	}
	catch (...)
	{
		throw _T("CTray::Notify");
	}
}
void CTray::TypeInit(TNotifyUI& msg)
{
	CPaintManagerUI* pPaintMgrUI = GetPaintMgr();
	ASSERT(pPaintMgrUI);
	m_pMsgList = static_cast<CListUI*>(pPaintMgrUI->FindControl(_T("msg")));
	ASSERT(m_pMsgList);
}
void CTray::UpdateMsgList()
{
	theLockMsgQue.Lock();
	typedef std::map<CString,std::vector<CMsg*> >::iterator iter;
	iter it;
	size_t sz = theMsgQueue.size();
	RECT rect;
	GetWindowRect(&rect);
	rect.top -= sz * 20;
	SetForegroundWindow(m_hWnd);
	SetWindowPos(HWND_TOPMOST,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,SWP_SHOWWINDOW);
	for (it=theMsgQueue.begin();it!=theMsgQueue.end();++it)
	{
		CListContainerElementUI* pListElement = new CListContainerElementUI;
		CHorizontalLayoutUI* pHori = new CHorizontalLayoutUI;
		RECT rect = {4,1,1,4};
		pHori->SetInset(rect);
		CButtonUI* pLogo = new CButtonUI;
		pLogo->SetFixedWidth(20);
		pLogo->SetBkImage(_T("list/man.png"));
		CLabelUI* pLabel = new CLabelUI;
		pLabel->SetText(it->first);
		pHori->Add(pLogo);
		pHori->Add(pLabel);
		pListElement->Add(pHori);
		m_pMsgList->Add(pListElement);
	}
	theLockMsgQue.UnLock();
}
void CTray::TypeClick(TNotifyUI& msg)
{
	
}
LRESULT CTray::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled)
{
	try
	{
		ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		if(!RegisterSkin(_T("tray.xml"),NULL,this))
			return 0;

		GetPaintMgr()->AddNotifier(this);
// 
// 		SIZE szInit,szNew;
// 		szInit = GetPaintMgr()->GetInitSize();
// 
// 		theLockMsgQue.Lock();
// 		int isize = theMsgQueue.size();
// 		szNew.cx = szInit.cx;
// 		szNew.cy = szInit.cy + isize* 20;
// 		theLockMsgQue.UnLock();
// 		GetPaintMgr()->SetInitSize(szNew.cx,szNew.cy);
 		RECT rcClient;
 		::GetClientRect(*this, &rcClient);
		SetForegroundWindow(m_hWnd);
//		MoveWindow(0, 0, szNew.cx, szNew.cy, FALSE);
//		SetWindowPos( HWND_TOPMOST, 0,0, szNew.cx, szNew.cy, SWP_SHOWWINDOW);
  		::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
  			rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);
	}
	catch (...)
	{
		throw _T("CTray::OnCreate");
	}

	return 0;
}

