#include "stdafx.h"
#include "login.h"

namespace sysbtn
{//系统栏按钮
	const TCHAR* const kMinBtn = _T("minbtn");//最小化
	const TCHAR* const kCloseBtn = _T("closebtn");//关闭
};
namespace loginpage
{
	const TCHAR* const kAccountEdt = _T("account");
	const TCHAR* const kDropDownAccountBtn = _T("account_drop");
	const TCHAR* const kPasswordEdt = _T("passwd");
	const TCHAR* const kKeyBoardBtn = _T("key_board");
	const TCHAR* const kRegAccountBtn = _T("reg_account");
	const TCHAR* const kForgerPsdBtn = _T("forget_psd");
};
CLogin::CLogin()
{}
CLogin::~CLogin()
{}
//////////////////////////
LPCTSTR CLogin::GetWindowClassName() const
{
	return _T("Login");
}
CControlUI* CLogin::CreateControl(LPCTSTR pstrClass)
{
	try
	{
		return NULL;
	}
	catch (...)
	{
		throw _T("CLogin::CreateControl");
	}
}
void CLogin::Notify(TNotifyUI& msg)
{
	DUI__Trace(msg.sType);
	if (msg.sType==_T("windowinit"))
	{
		OnInitWindow(msg);
	}
	else if (msg.sType==_T("click"))
	{
		OnClick(msg);
	}
}
void CLogin::InitAllVariable()
{
	m_pAccountEdt = NULL;
	m_pDropDownAccountBtn = NULL;
	m_pPasswdEdt = NULL;
	m_pKeyBoardBtn = NULL;
	m_pRegAccount = NULL;
	m_pForgetPsd = NULL;

	m_pLoginList = NULL;
}
void CLogin::OnInitWindow(TNotifyUI& msg)
{
	CPaintManagerUI* pPaintMgr = GetPaintMgr();
	ASSERT(pPaintMgr);
	FindControl(m_pAccountEdt,loginpage::kAccountEdt);
	ASSERT(m_pAccountEdt);
	FindControl(m_pDropDownAccountBtn,loginpage::kDropDownAccountBtn);
	ASSERT(m_pDropDownAccountBtn);
	FindControl(m_pPasswdEdt,loginpage::kPasswordEdt);
	ASSERT(m_pPasswdEdt);
	FindControl(m_pKeyBoardBtn,loginpage::kKeyBoardBtn);
	ASSERT(m_pKeyBoardBtn);
	FindControl(m_pRegAccount,loginpage::kRegAccountBtn);
	ASSERT(m_pRegAccount);
	FindControl(m_pForgetPsd,loginpage::kForgerPsdBtn);
	ASSERT(m_pForgetPsd);
	//m_pDropDownAccountBtn->OnEvent += MakeDelegate(this,&CLogin::OnMouseEvent,p);
	//m_pKeyBoardBtn->OnEvent += MakeDelegate(this,&CLogin::OnMouseEvent,p);
	//m_pRegAccount->OnEvent += MakeDelegate(this,&CLogin::OnMouseEvent,p);
	//m_pForgetPsd->OnEvent += MakeDelegate(this,&CLogin::OnMouseEvent,p);
}
void CLogin::OnClick(TNotifyUI& msg)
{
	if(msg.pSender->IsName(sysbtn::kCloseBtn))
	{
		OnExit(msg);
	}
	else if (msg.pSender->IsName(sysbtn::kMinBtn))
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
	}
	else if (msg.pSender->IsName(loginpage::kDropDownAccountBtn))
	{
		CLoginList* pLoginList = GetLoginList();
		if(pLoginList)pLoginList->ShowWindow(true);
	}
}
void CLogin::OnExit(TNotifyUI& msg)
{
	::SendMessage(m_hWnd,WM_SYSCOMMAND,SC_CLOSE,0);
}
bool CLogin::OnMouseEvent(void* param)
{
	TEventUI* pEvent = (TEventUI*)param;
	if(pEvent)
	{
		if (UIEVENT_MOUSEHOVER==pEvent->Type
			||UIEVENT_MOUSEENTER==pEvent->Type)
		{
			if(pEvent->pSender->IsName(loginpage::kDropDownAccountBtn))
				m_pAccountEdt->DoEvent(*pEvent);
			else if(pEvent->pSender->IsName(loginpage::kKeyBoardBtn))
				m_pPasswdEdt->DoEvent(*pEvent);
			else if (pEvent->pSender->IsName(loginpage::kRegAccountBtn)
				||pEvent->pSender->IsName(loginpage::kForgerPsdBtn))
			{
				CDuiString sText = pEvent->pSender->GetUserData();
				TCHAR szBuf[MAX_PATH] = {0};
#if defined(UNDER_WINCE)
				_stprintf(szBuf, _T("<u><c #0000F0>%s</c></u>"), sText.GetData());
#else
				_stprintf_s(szBuf, MAX_PATH - 1, _T("<u><c #0000F0>%s</c></u>"), sText.GetData());
#endif
				pEvent->pSender->SetText(szBuf);
			}
		}
		else if (UIEVENT_MOUSELEAVE==pEvent->Type)
		{
			if(pEvent->pSender->IsName(loginpage::kDropDownAccountBtn))
				m_pAccountEdt->DoEvent(*pEvent);
			else if(pEvent->pSender->IsName(loginpage::kKeyBoardBtn))
				m_pPasswdEdt->DoEvent(*pEvent);
			else if (pEvent->pSender->IsName(loginpage::kRegAccountBtn)
				|| pEvent->pSender->IsName(loginpage::kForgerPsdBtn))
			{
				CDuiString sText = pEvent->pSender->GetUserData();
				TCHAR szBuf[MAX_PATH] = {0};
#if defined(UNDER_WINCE)
				_stprintf(szBuf, _T("<c #0000F0>%s</c>"), sText.GetData());
#else
				_stprintf_s(szBuf, MAX_PATH - 1, _T("<c #0000F0>%s</c>"), sText.GetData());
#endif
				pEvent->pSender->SetText(szBuf);
			}
		}
	}
	return true;
}
template<typename T>
void CLogin::FindControl(T& pctrl,LPCTSTR pstrName) const
{
	ASSERT(pstrName);
	pctrl = static_cast<T>(GetPaintMgr()->FindControl(pstrName));
}
LRESULT CLogin::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	try
	{
		ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		if(!RegisterSkin(_T("login.xml"),0,this))
			return 0;

		GetPaintMgr()->AddNotifier(this);
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
			rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);
	}
	catch (...)
	{
		throw _T("CLogin::OnCreate");
	}
	return 0;
}
LRESULT CLogin::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	try
	{
		// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
		if( wParam == SC_CLOSE ) {
			Sleep(1000);
			Close(IDOK);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		
		return lRes;
	}
	catch (...)
	{
		throw _T("CLogin::OnSysCommand");
	}
}
CLoginList* CLogin::GetLoginList()
{
	if(m_pLoginList&&m_pLoginList->IsWindow())
		return m_pLoginList;
	else 
	{
		RECT pos = m_pAccountEdt->GetPos();
		POINT pt = {pos.left,pos.bottom};
		ClientToScreen(&pt);
		m_pLoginList = new CLoginList(pt,this);
		m_pLoginList->Create(m_hWnd,NULL,0,0);
		return m_pLoginList;
	}
}