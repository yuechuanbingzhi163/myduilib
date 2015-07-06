#include "stdafx.h"
#include "login_list.h"
#include "../userinfo/user.h"
#include "../login/login.h"
CLoginList::CLoginList(POINT point,CLogin* pParent)
	: m_pLoginList(NULL)
	, m_ptBasedPoint(point)
	, m_pLoginWnd(pParent)
{

}
CLoginList::~CLoginList()
{

}
LPCTSTR CLoginList::GetWindowClassName() const
{
	return _T("LoginList");
}
CControlUI* CLoginList::CreateControl(LPCTSTR pstrClass)
{
	try
	{
		if(0 == _tcscmp(pstrClass,_T("LoginList")))
			return new CLoginListUI(*(GetPaintMgr()));
		return NULL;
	}
	catch (...)
	{
		throw _T("CLoginList::CreateControl");
	}
}
template<typename T>
void CLoginList::FindControl(T& pctrl,LPCTSTR pstrName) const
{
	ASSERT(pstrName);
	pctrl = static_cast<T>(GetPaintMgr()->FindControl(pstrName));
}
void CLoginList::Notify(TNotifyUI& msg)
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
	else if (msg.sType==_T("itemclick"))
	{
		OnItemClick(msg);
	}
}
void CLoginList::OnInitWindow(TNotifyUI& msg)
{
	CPaintManagerUI* pPaintMgr = GetPaintMgr();
	ASSERT(pPaintMgr);
	FindControl(m_pLoginList,_T("login_list"));
	ASSERT(m_pLoginList);
	UserInfo user;
	for (int i=0;i<10;++i)
	{
		m_pLoginList->AddNode(user);
	}
// 		
	user.NickName("À¶ÒÝÐù");
 	m_pLoginList->AddNode(user);
}
void CLoginList::OnClick(TNotifyUI& msg)
{
	if (msg.pSender->IsName(_T("remove")))
	{
		int iIndex = m_pLoginList->GetItemIndex((CLoginListItemUI*)(msg.pSender->GetTag()));
		m_pLoginList->RemoveAt(iIndex);
	}
}
void CLoginList::OnItemClick(TNotifyUI& msg)
{
	int iIndex = m_pLoginList->GetCurSel();

	if (m_pLoginWnd)
	{
		UserInfo user = m_pLoginList->GetUser(iIndex);
	}
}
LRESULT CLoginList::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	try
	{
		ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		if(!RegisterSkin(_T("login_list.xml"),0,this))
			return 0;

		GetPaintMgr()->AddNotifier(this);
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
			rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);
		MoveWindow(m_ptBasedPoint.x,m_ptBasedPoint.y,rcClient.right - rcClient.left, \
			rcClient.bottom - rcClient.top);
	}
	catch (...)
	{
		throw _T("CLogin::OnCreate");
	}
	return 0;
}
LRESULT CLoginList::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Close(IDOK);
	return 0;
}