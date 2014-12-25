#include "StdAfx.h"
#include "notice.h"
#include "Msg.h"
#include "mainwindow.h"

namespace noticeView
{
	const TCHAR* kClose = _T("closebtn");
	const TCHAR* kLogo = _T("logo");
	const TCHAR* kUser = _T("user");
	const TCHAR* kNoNotice = _T("nonotice");
};
bool bGroupMsgTip = true;
bool bFriednMsgTip = true;
bool bFriednLoadTip = true;
bool bFriednShakeTip = true;
const LPCTSTR CNotice::CLASSNAME = _T("LoginNotice");
CNotice::CNotice(HWND pParent,LPCTSTR xml,LPCTSTR text,EMT_NOTICE_TYPE type)
	: m_strText(text)
	, m_xmlFile(xml)
	, m_NoticeType(type)
{
	Create(pParent,_T("登录提醒"), WS_POPUP, WS_EX_TOOLWINDOW);
	ShowWindow(true);
}
CNotice::~CNotice()
{

}
LPCTSTR CNotice::GetWindowClassName() const
{
	return CNotice::CLASSNAME;
}

CControlUI* CNotice::CreateControl(LPCTSTR pstrClass)
{
	try
	{
		return NULL;
	}
	catch (...)
	{

	}
	
}
void CNotice::HideNoNotice(bool bHide)
{
	CControlUI* pUI = GetPaintMgr()->FindControl(noticeView::kNoNotice);
	if(pUI) pUI->SetVisible(!bHide);
}
void CNotice::Notify(TNotifyUI& msg)
{
	try
	{
		if( 0 == msg.sType.Compare(_T("windowinit"))){
			TypeInit(msg);
		}
		else if( 0 == msg.sType.Compare(_T("click")) ){
			TypeClick(msg);
		}	
		else if( 0 == msg.sType.Compare(_T("timer")) ){
			GetPaintMgr()->KillTimer(GetPaintMgr()->GetRoot(),100);
			Close(IDOK);
		}
	}
	catch (...)
	{
		throw _T("CNotice::Notify");
	}
}
void CNotice::TypeInit(TNotifyUI& msg)
{
	CPaintManagerUI* pPaintMgrUI = GetPaintMgr();
	ASSERT(pPaintMgrUI);
	
	CTextUI* pLabel = static_cast<CTextUI*>(pPaintMgrUI->FindControl(noticeView::kUser));
	if(pLabel){ 		
		pLabel->SetText(m_strText);
	}
	int iWidth = GetSystemMetrics(SM_CXSCREEN);
	int iHeight = GetSystemMetrics(SM_CYSCREEN);//计算的屏幕客户工作区域
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	RECT rcWnd;
	SIZE szInit = GetPaintMgr()->GetInitSize();

	rcWnd.right = rcWork.right-1;
	rcWnd.left = rcWnd.right-szInit.cx;
	rcWnd.bottom = rcWork.bottom -1;
	rcWnd.top = rcWnd.bottom-szInit.cy;

//	MoveWindow(&rcWnd);
	HWND hWnd = GetForegroundWindow();
	SetWindowPos(HWND_TOPMOST,&rcWnd,SWP_NOACTIVATE);
	SetForegroundWindow(hWnd);
	/* 设置初始皮肤 */
}
void CNotice::TypeClick(TNotifyUI& msg)
{
	if(msg.pSender->IsName(noticeView::kClose)){
		GetPaintMgr()->KillTimer(GetPaintMgr()->GetRoot(),100);
		Close(IDOK);
	}
	else if (msg.pSender->IsName(noticeView::kLogo))
	{
		if(EMT_GROUP_MSG == m_NoticeType||EMT_SYS_MSG==m_NoticeType)
		{	
			if(theApp->IsIconic())
				theApp->ShowWindow(true);
			theApp->SetPage(3);//转到群聊窗口
		}
		else if(EMT_FRIEND_MSG==m_NoticeType||EMT_FRIEND_LOAD==m_NoticeType||EMT_FRIEND_SHAKE==m_NoticeType)
		{
			if (theApp->FindChatBox(m_sWho))
			{
				ChatDialog* pDlg = NULL;
				if ((pDlg=theApp->FindChatBox(m_sWho))!=NULL)
				{
					pDlg->ShowWindow(true);
					pDlg->BringWindowToTop();
				}
			}
			else
			{
				User friend_;
				theApp->FindUserFromList(m_sWho,friend_);
				ChatDialog* pChatDialog = new ChatDialog(_T(""), 0, theUser, friend_);
				if( pChatDialog == NULL )
					return;
#if defined(WIN32) && !defined(UNDER_CE)
				pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP,  NULL, 0, 0, 0, 0);
#else
				pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
#endif
				pChatDialog->SetWindowText(friend_.m_loadName);
				//skin_changed_observer_.AddReceiver(pChatDialog);

				pChatDialog->CenterWindow();
				::ShowWindow(*pChatDialog, SW_SHOW);
				//装入链表
				thelockChatDlgList.Lock();
				theChatDialogList.AddTail(pChatDialog);
				thelockChatDlgList.UnLock();
			}
		}
		Close(IDOK);
	}
	else if (msg.pSender->IsName(noticeView::kNoNotice))
	{
		if(EMT_GROUP_MSG==m_NoticeType)
			bGroupMsgTip = false;
		if(EMT_FRIEND_MSG==m_NoticeType)
			bFriednMsgTip = false;
		if(EMT_FRIEND_LOAD==m_NoticeType)
			bFriednLoadTip = false;
		if(EMT_FRIEND_SHAKE==m_NoticeType)
			bFriednShakeTip = false;
		Close(IDOK);
	}
}
LRESULT CNotice::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled)
{
	try
	{
		ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		if(!RegisterSkin(m_xmlFile.GetData(),NULL,this))
			return 0;

		GetPaintMgr()->AddNotifier(this);
 		RECT rcClient;
 		::GetClientRect(*this, &rcClient);
 		::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
 			rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

		GetPaintMgr()->SetTimer(GetPaintMgr()->GetRoot(),100,4000);
	}
	catch (...)
	{
		throw _T("CNotice::OnCreate");
	}

	return 0;
}
LRESULT CNotice::OnNcLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	GetPaintMgr()->KillTimer(GetPaintMgr()->GetRoot(),100);
	bHandled = FALSE;
	return 0;
}
LRESULT CNotice::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			{
				if (wParam == VK_RETURN)
				{
					return FALSE;
				}
				else if (wParam == VK_ESCAPE)
				{
					Close(IDCANCEL);
					return TRUE;
				}
			}
		default:
			break;
		}
	}
	return FALSE;
}
LRESULT CNotice::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lResult = __super::OnNcHitTest(uMsg,wParam,lParam,bHandled);
// 	if(lResult==HTCAPTION)
// 		GetPaintMgr()->KillTimer(GetPaintMgr()->GetRoot(),100);
	return lResult;
}
LRESULT CNotice::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(WM_NCLBUTTONDOWN==uMsg)
		return OnNcLButtonDown(uMsg,wParam,lParam,bHandled);

	return 0;
}