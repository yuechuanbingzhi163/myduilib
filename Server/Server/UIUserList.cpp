#include "stdafx.h"
#include "UIUserList.h"
#include "Msg.h"

const int kListItemNormalHeight = 30;
const int kListItemSelectedHeight = 50;

namespace DuiLib{

	double CalculateDelay(double state)
	{
		return pow(state, 2);
	}

CUserListUI::CUserListUI(CPaintManagerUI& paint_manager)
	
	: delay_deltaY_(0)
	, delay_number_(0)
	, delay_left_(0)
	, paint_manager_(paint_manager)
{
	SetItemShowHtml(true);
}

CUserListUI::~CUserListUI()
{

}

bool CUserListUI::Add(CControlUI* pControl)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;

	return CListUI::Add(pControl);
}

bool CUserListUI::AddAt(CControlUI* pControl, int iIndex)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;

	return CListUI::AddAt(pControl, iIndex);
}

bool CUserListUI::Remove(CControlUI* pControl)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;
	int iIndex = this->GetItemIndex(pControl);
	userList_.erase(userList_.begin()+iIndex);

	return CListUI::Remove(pControl);
}
bool CUserListUI::RemoveAt(int iIndex)
{
	CControlUI* pControl = GetItemAt(iIndex);
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;

	userList_.erase(userList_.begin()+iIndex);

	return CListUI::RemoveAt(iIndex);
}

void CUserListUI::RemoveAll()
{
	CListUI::RemoveAll();
	userList_.clear();
}

void CUserListUI::DoEvent(TEventUI& event) 
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
	{
		if (m_pParent != NULL)
			m_pParent->DoEvent(event);
		else
			CVerticalLayoutUI::DoEvent(event);
		return;
	}

	if (event.Type == UIEVENT_TIMER && event.wParam == SCROLL_TIMERID)
	{
		if (delay_left_ > 0)
		{
			--delay_left_;
			SIZE sz = GetScrollPos();
			LONG lDeltaY =  (LONG)(CalculateDelay((double)delay_left_ / delay_number_) * delay_deltaY_);
			if ((lDeltaY > 0 && sz.cy != 0)  || (lDeltaY < 0 && sz.cy != GetScrollRange().cy ))
			{
				sz.cy -= lDeltaY;
				SetScrollPos(sz);
				return;
			}
		}
		delay_deltaY_ = 0;
		delay_number_ = 0;
		delay_left_ = 0;
		m_pManager->KillTimer(this, SCROLL_TIMERID);
		return;
	}
	if (event.Type == UIEVENT_SCROLLWHEEL)
	{
		LONG lDeltaY = 0;
		if (delay_number_ > 0)
			lDeltaY =  (LONG)(CalculateDelay((double)delay_left_ / delay_number_) * delay_deltaY_);
		switch (LOWORD(event.wParam))
		{
		case SB_LINEUP:
			if (delay_deltaY_ >= 0)
				delay_deltaY_ = lDeltaY + 8;
			else
				delay_deltaY_ = lDeltaY + 12;
			break;
		case SB_LINEDOWN:
			if (delay_deltaY_ <= 0)
				delay_deltaY_ = lDeltaY - 8;
			else
				delay_deltaY_ = lDeltaY - 12;
			break;
		}
		if
			(delay_deltaY_ > 100) delay_deltaY_ = 100;
		else if
			(delay_deltaY_ < -100) delay_deltaY_ = -100;

		delay_number_ = (DWORD)sqrt((double)abs(delay_deltaY_)) * 5;
		delay_left_ = delay_number_;
		m_pManager->SetTimer(this, SCROLL_TIMERID, 50U);
		return;
	}

	CListUI::DoEvent(event);
}
void CUserListUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	return __super::SetAttribute(pstrName,pstrValue);
}
namespace userinfoitem
{
	const TCHAR* const kLogo = _T("logo");
	const TCHAR* const kName = _T("nickname");
	const TCHAR* const kDescrb = _T("description");
	const TCHAR* const kFriend = _T("friend");
	const TCHAR* const kMail = _T("mail");
	const TCHAR* const kXiaoyou = _T("xiaoyou");
	const TCHAR* const kSoso = _T("soso");
};


bool CUserListUI::AddNode(User UserInfo)
{
	CListContainerElementUI* pListElement = NULL;
	if( !m_dlgBuilder.GetMarkup()->IsValid() ) {
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("userinfo.xml"), (UINT)0, NULL, &paint_manager_));
	}
	else {
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
	}
	if (pListElement == NULL)
		return false;
	ASSERT(pListElement);
	pListElement->SetTag((UINT_PTR)this);
	pListElement->SetFixedHeight(kListItemSelectedHeight);

	CButtonUI* pLogoButton = static_cast<CButtonUI*>(pListElement->FindSubControl(userinfoitem::kLogo));
	ASSERT(pLogoButton);
	CLabelUI* pNameLabel = static_cast<CLabelUI*>(pListElement->FindSubControl(userinfoitem::kName));
	ASSERT(pNameLabel);
	CLabelUI* pDescLabel = static_cast<CLabelUI*>(pListElement->FindSubControl(userinfoitem::kDescrb));
	ASSERT(pDescLabel);
	CButtonUI* pFriendButton = static_cast<CButtonUI*>(pListElement->FindSubControl(userinfoitem::kFriend));
	ASSERT(pFriendButton);
	CButtonUI* pMailButton = static_cast<CButtonUI*>(pListElement->FindSubControl(userinfoitem::kMail));
	ASSERT(pMailButton);
	CButtonUI* pXiaoyou = static_cast<CButtonUI*>(pListElement->FindSubControl(userinfoitem::kXiaoyou));
	ASSERT(pXiaoyou);
	CButtonUI* pSosoButton = static_cast<CButtonUI*>(pListElement->FindSubControl(userinfoitem::kSoso));
	ASSERT(pSosoButton);
	pNameLabel->SetText(UserInfo.m_loadName);

	if (!CListUI::Add(pListElement))
	{
		delete pListElement;
		return false;
	}
	userList_.push_back(UserInfo);

	return true;
}
/*
bool CUserListUI::AddNode(std::vector<USERINFO> UserList)
{
	for (auto it = UserList.begin();it != UserList.end();++it)
	{
		m_lock.Lock();
		ASSERT(AddNode(*it));
		m_lock.UnLock();
	}
	return true;
}*/

User& CUserListUI::GetUser(const int iIndex)
{
	return userList_[iIndex];
}

};
