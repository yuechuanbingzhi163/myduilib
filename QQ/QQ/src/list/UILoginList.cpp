#include "stdafx.h"
#include "UILoginList.h"

#include "../userinfo/user.h"

const int kListItemNormalHeight = 30;
const int kListItemSelectedHeight = 50;
namespace loginlistitem
{
	const TCHAR* const kLogoSmall = _T("logo_small");
	const TCHAR* const kLogoBig = _T("logo_big");
	const TCHAR* const kAccountSmall = _T("account_small");
	const TCHAR* const kAccountBig = _T("account_big");
	const TCHAR* const kNickName = _T("nickname");
	const TCHAR* const kRemove = _T("remove");
	const TCHAR* const kTab = _T("switch");
};
namespace DuiLib{

CLoginListItemUI::CLoginListItemUI()
{
// 	this->SetFixedHeight(kListItemNormalHeight);
// 	CDialogBuilder builder;
// 	builder.Create(_T("login_list_item.xml"),0,NULL,NULL,this);

}
CLoginListItemUI::~CLoginListItemUI()
{
//	delete this;
}
LPCTSTR CLoginListItemUI::GetClassName()
{
	return _T("ListContainerElementUI");
}
LPCTSTR CLoginListItemUI::GetClass() const
{
	return CLoginListItemUI::GetClassName();
}
void CLoginListItemUI::SetPage(int iIndex)
{
	CTabLayoutUI* pSwitch = static_cast<CTabLayoutUI*>(FindSubControl(loginlistitem::kTab));
	if (pSwitch)
	{
		pSwitch->SelectItem(iIndex);
	}
}
int CLoginListItemUI::GetPage()
{
	CTabLayoutUI* pSwitch = static_cast<CTabLayoutUI*>(FindSubControl(loginlistitem::kTab));
	if (pSwitch)
	{
		return pSwitch->GetCurSel();
	}
	return 0;
}
void CLoginListItemUI::DoEvent(TEventUI& event)
{
	DUI__Trace(DUI__TraceMsg(event.Type));
// 	if (event.Type==UIEVENT_MOUSEENTER
// 		||event.Type==UIEVENT_MOUSEHOVER)
// 	{
// 		CTabLayoutUI* pSwitch = static_cast<CTabLayoutUI*>(FindSubControl(loginlistitem::kTab));
// 		if (pSwitch&&pSwitch->GetCurSel()==0)
// 		{
// 			SetFixedHeight(kListItemSelectedHeight);
// 			pSwitch->SelectItem(1);
// 		}
// 	}
// 	else if (event.Type==UIEVENT_MOUSELEAVE)
// 	{
// 		CTabLayoutUI* pSwitch = static_cast<CTabLayoutUI*>(FindSubControl(loginlistitem::kTab));
// 		if (pSwitch&&pSwitch->GetCurSel()==1)
// 		{
// 			SetFixedHeight(kListItemNormalHeight);
// 			pSwitch->SelectItem(0);
// 		}
// 	}
	this->Invalidate();
	CListContainerElementUI::DoEvent(event);
}
/////////////////////////////////////////////////////////////////////////////
CLoginListUI::CLoginListUI(CPaintManagerUI& paint_manager)
	
	: delay_deltaY_(0)
	, delay_number_(0)
	, delay_left_(0)
	, paint_manager_(paint_manager)
{
	SetItemShowHtml(true);
}

CLoginListUI::~CLoginListUI()
{

}

bool CLoginListUI::Add(CControlUI* pControl)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), CLoginListItemUI::GetClassName()) != 0)
		return false;

	return CListUI::Add(pControl);
}

bool CLoginListUI::AddAt(CControlUI* pControl, int iIndex)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), CLoginListItemUI::GetClassName()) != 0)
		return false;

	return CListUI::AddAt(pControl, iIndex);
}

bool CLoginListUI::Remove(CControlUI* pControl)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), CLoginListItemUI::GetClassName()) != 0)
		return false;
	int iIndex = this->GetItemIndex(pControl);
	userList_.erase(userList_.begin()+iIndex);

	return CListUI::Remove(pControl);
}
bool CLoginListUI::RemoveAt(int iIndex)
{
	CControlUI* pControl = GetItemAt(iIndex);
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), CLoginListItemUI::GetClassName()) != 0)
		return false;

	userList_.erase(userList_.begin()+iIndex);

	return CListUI::RemoveAt(iIndex);
}

void CLoginListUI::RemoveAll()
{
	CListUI::RemoveAll();
	userList_.clear();
}

void CLoginListUI::DoEvent(TEventUI& event) 
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
void CLoginListUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	return CListUI::SetAttribute(pstrName,pstrValue);
}


static bool OnTabEvent(void* event) {

	CControlUI* pButton = ((TEventUI*)event)->pSender;
	if( pButton != NULL ) {
		CLoginListItemUI* pListElement = (CLoginListItemUI*)(pButton->GetTag());
		if( pListElement != NULL ){
			if (((TEventUI*)event)->Type==UIEVENT_MOUSEHOVER
				||((TEventUI*)event)->Type==UIEVENT_MOUSEMOVE
				||((TEventUI*)event)->Type==UIEVENT_MOUSEENTER)
			{
				if(pListElement->GetPage()==0)
				{
					pListElement->SetPage(1);
					pListElement->SetFixedHeight(kListItemSelectedHeight);
					CLoginListUI* pList = (CLoginListUI*)(pListElement->GetTag());
					if (pList)
					{
						pList->SelectItem(pList->GetItemIndex(pListElement));
//						if(pList->GetItemIndex(pListElement)==pList->GetCount()-1)
							pList->Scroll(pListElement->GetScrollPos().cx,pListElement->GetScrollPos().cy);
					}
				}
			}
			pListElement->DoEvent(*(TEventUI*)event);

		}
	}
	return true;
}
static bool OnTab2Event(void* event) {

	CControlUI* pButton = ((TEventUI*)event)->pSender;
	if( pButton != NULL ) {
		CLoginListItemUI* pListElement = (CLoginListItemUI*)(pButton->GetTag());
		if( pListElement != NULL ){
			if (((TEventUI*)event)->Type==UIEVENT_MOUSELEAVE)
			{
				if(pListElement->GetPage()==1)
				{
					pListElement->SetPage(0);
					pListElement->SetFixedHeight(kListItemNormalHeight);
					CLoginListUI* pList = (CLoginListUI*)(pListElement->GetTag());
					if (pList)
					{
						pList->SelectItem(-1);
					}
				}
			}
			pListElement->DoEvent(*(TEventUI*)event);

		}
	}
	return true;
}
bool CLoginListUI::AddNode(UserInfo user)
{
	CLoginListItemUI* pListElement = NULL;
	if( !m_dlgBuilder.GetMarkup()->IsValid() ) {
		pListElement = static_cast<CLoginListItemUI*>(m_dlgBuilder.Create(_T("login_list_item.xml"), (UINT)0, NULL, &paint_manager_));
	}
	else {
		pListElement = static_cast<CLoginListItemUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
	}
	if (pListElement == NULL)
		return false;
	ASSERT(pListElement);
	pListElement->SetTag((UINT_PTR)this);
	pListElement->SetFixedHeight(kListItemNormalHeight);
	CControlUI* pUI = paint_manager_.FindSubControlByName(pListElement,loginlistitem::kLogoSmall);
	ASSERT(pUI);
	pUI->SetTag((UINT_PTR)pListElement);
	pUI->OnEvent += MakeDelegate(&OnTabEvent);
	pUI = paint_manager_.FindSubControlByName(pListElement,_T("small_info"));
	ASSERT(pUI);
	pUI->SetTag((UINT_PTR)pListElement);
	pUI->OnEvent += MakeDelegate(&OnTabEvent);
	pUI = paint_manager_.FindSubControlByName(pListElement,loginlistitem::kAccountSmall);
	ASSERT(pUI);
	pUI->SetTag((UINT_PTR)pListElement);
	pUI->OnEvent += MakeDelegate(&OnTabEvent);

	pUI = paint_manager_.FindSubControlByName(pListElement,_T("big_info"));
	ASSERT(pUI);
	pUI->SetTag((UINT_PTR)pListElement);
	pUI->OnEvent += MakeDelegate(&OnTab2Event);
	pUI = paint_manager_.FindSubControlByName(pListElement,_T("info_2"));
	ASSERT(pUI);
	pUI->SetTag((UINT_PTR)pListElement);
	pUI->OnEvent += MakeDelegate(&OnTab2Event);
	pUI = paint_manager_.FindSubControlByName(pListElement,_T("info_3"));
	ASSERT(pUI);
	pUI->SetTag((UINT_PTR)pListElement);
	pUI->OnEvent += MakeDelegate(&OnTab2Event);
	pUI = paint_manager_.FindSubControlByName(pListElement,loginlistitem::kAccountBig);
	ASSERT(pUI);
	pUI->SetTag((UINT_PTR)pListElement);
	pUI->OnEvent += MakeDelegate(&OnTab2Event);
	pUI = paint_manager_.FindSubControlByName(pListElement,loginlistitem::kLogoBig);
	ASSERT(pUI);
	pUI->SetTag((UINT_PTR)pListElement);
	pUI->OnEvent += MakeDelegate(&OnTab2Event);
	pUI = paint_manager_.FindSubControlByName(pListElement,loginlistitem::kNickName);
	ASSERT(pUI);
	pUI->SetTag((UINT_PTR)pListElement);
	pUI->OnEvent += MakeDelegate(&OnTab2Event);
	pUI = paint_manager_.FindSubControlByName(pListElement,loginlistitem::kRemove);
	ASSERT(pUI);
	pUI->SetTag((UINT_PTR)pListElement);
	pUI->OnEvent += MakeDelegate(&OnTab2Event);
	for (int i=0;i<6;++i)
	{
		CDuiString sName;
		sName.Format(_T("big_%d"),i);
		pUI = paint_manager_.FindSubControlByName(pListElement,sName);
		ASSERT(pUI);
		pUI->SetTag((UINT_PTR)pListElement);
		pUI->OnEvent += MakeDelegate(&OnTab2Event);
	}
	
	if (!CListUI::Add(pListElement))
	{
		delete pListElement;
		return false;
	}
	userList_.push_back(user);
	return true;
}

UserInfo& CLoginListUI::GetUser(const int iIndex)
{
	return userList_[iIndex];
}

};
