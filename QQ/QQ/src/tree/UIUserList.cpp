#include "stdafx.h"
#include "UIUserList.h"


namespace DuiLib
{
	const int kFriendListItemNormalHeight = 32;
	const int kFriendListItemSelectedHeight = 50;
CUserListUI::CUserListUI(CPaintManagerUI& paint_manager)
	: root_node_(NULL)
	, delay_deltaY_(0)
	, delay_number_(0)
	, delay_left_(0)
	, level_expand_image_(_T("<i list/list_icon_b.png>"))
	, level_collapse_image_(_T("<i list/list_icon_a.png>"))
	, level_text_start_pos_(10)
	, text_padding_(10, 0, 0, 0)
	, paint_manager_(paint_manager)
{
	SetItemShowHtml(true);

	root_node_ = new Node;
	root_node_->data().level_ = -1;
	root_node_->data().child_visible_ = true;
	root_node_->data().has_child_ = true;
	root_node_->data().list_elment_ = NULL;
}

CUserListUI::~CUserListUI()
{
	if (root_node_)
		delete root_node_;

	root_node_ = NULL;
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

	if (reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElement")))->GetTag()) == NULL)
		return CListUI::Remove(pControl);
	else
		return RemoveNode(reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElement")))->GetTag()));
}

bool CUserListUI::RemoveAt(int iIndex)
{
	CControlUI* pControl = GetItemAt(iIndex);
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;

	if (reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElement")))->GetTag()) == NULL)
		return CListUI::RemoveAt(iIndex);
	else
		return RemoveNode(reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElement")))->GetTag()));
}

void CUserListUI::RemoveAll()
{
	CListUI::RemoveAll();
	for (int i = 0; i < root_node_->num_children(); ++i)
	{
		Node* child = root_node_->child(i);
		RemoveNode(child);
	}
	delete root_node_;

	root_node_ = new Node;
	root_node_->data().level_ = -1;
	root_node_->data().child_visible_ = true;
	root_node_->data().has_child_ = true;
	root_node_->data().list_elment_ = NULL;
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

Node* CUserListUI::GetRoot()
{
	return root_node_;
}

const TCHAR* const kLogoButtonControlName = _T("logo");
const TCHAR* const kLogoContainerControlName = _T("logo_container");
const TCHAR* const kNickNameControlName = _T("nickname");
const TCHAR* const kDescriptionControlName = _T("description");
const TCHAR* const kResveredControlName = _T("reserved");
const TCHAR* const kOperatorPannelControlName = _T("operation");

static bool OnLogoButtonEvent(void* event) {
    if( ((TEventUI*)event)->Type == UIEVENT_BUTTONDOWN ) {
        CControlUI* pButton = ((TEventUI*)event)->pSender;
        if( pButton != NULL ) {
            CListContainerElementUI* pListElement = (CListContainerElementUI*)(pButton->GetTag());
            if( pListElement != NULL ) pListElement->DoEvent(*(TEventUI*)event);
        }
    }
    return true;
}

Node* CUserListUI::AddNode(const UserListItemInfo& item, Node* parent)
{
	Node* tempNode = parent;
	if (!parent)
		parent = root_node_;

	TCHAR szBuf[MAX_PATH] = {0};

    CListContainerElementUI* pListElement = NULL;
	bool bIsValid = m_dlgBuilder.GetMarkup()->IsValid();
    if( ! bIsValid ) {
        pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("node.xml"), (UINT)0, NULL, &paint_manager_));
    }
    else {
        pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
    }
	ASSERT(pListElement);
    if (pListElement == NULL)
        return NULL;

	Node* node = new Node;

	node->data().level_ = parent->data().level_ + 1;
	if (item.folder)
		node->data().has_child_ = !item.empty;
	else
		node->data().has_child_ = false;

	node->data().folder_ = item.folder;

	node->data().child_visible_ = (node->data().level_ == 0);
	node->data().child_visible_ = false;

// 	node->data().text_ = item.nick_name;
// 	node->data().value = item.id;
	node->data().user_ = item.user;
	node->data().list_elment_ = pListElement;

	if (!parent->data().child_visible_)
		pListElement->SetVisible(false);

	if (parent != root_node_ && !parent->data().list_elment_->IsVisible())
		pListElement->SetVisible(false);

	CDuiRect rcPadding = text_padding_;
	for (int i = 0; i < node->data().level_; ++i)
	{
		rcPadding.left += level_text_start_pos_;		
	}
	pListElement->SetPadding(rcPadding);

	CDuiString s_result = _T("");
	CButtonUI* log_button = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(pListElement, kLogoButtonControlName));
	if (log_button != NULL)
	{
		if (!item.folder && item.user._logo!=NULL&&strlen(item.user._logo)>0)
		{
			CA2W input(item.user._logo, CP_ACP);
			s_result = input;
			log_button->SetBkImage(s_result);
		}
		else
		{
			if(NULL==tempNode)
			{
				CContainerUI* logo_container = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pListElement, kLogoContainerControlName));
				if (logo_container != NULL)
					logo_container->SetVisible(false);
			}
		}
        log_button->SetTag((UINT_PTR)pListElement);
        log_button->OnEvent += MakeDelegate(&OnLogoButtonEvent);
	}

	CDuiString html_text = _T("");
	if (node->data().has_child_)
	{
		if (node->data().child_visible_)
			html_text += level_expand_image_;
		else
			html_text += level_collapse_image_;

#if defined(UNDER_WINCE)
		_stprintf(szBuf, _T("<x %d>"), level_text_start_pos_);
#else
		_stprintf_s(szBuf, MAX_PATH - 1, _T("<x %d>"), level_text_start_pos_);
#endif
		html_text += szBuf;
	}
	s_result = _T("");
	if (item.folder)
	{
		if (node->data().user_._nickname != NULL && strlen(node->data().user_._nickname) > 0)
		{
			CA2W input(node->data().user_._nickname, CP_ACP);
			s_result = input;
		}

		html_text += s_result;
	}
	else
	{
		if (item.user._nickname != NULL && strlen(item.user._nickname) > 0)
		{
			CA2W input(node->data().user_._nickname, CP_ACP);
			s_result = input;
		}
		html_text += s_result;
	}

	CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kNickNameControlName));
	if (nick_name != NULL)
	{
		nick_name->SetShowHtml(true);
		TFontInfo* pFont = paint_manager_.GetFontInfo(nick_name->GetFont());
		unsigned int uWidth = pFont->tm.tmAveCharWidth;
		std::string sNickName("");
		if( wcslen(s_result) > 0)
		{
			CW2A input(s_result);
			sNickName = input;
		}

		int iLen = strlen(sNickName.c_str());
		uWidth *= (1 + iLen);
		if (item.folder)
			nick_name->SetFixedWidth(uWidth+10+level_text_start_pos_);
		else 
			nick_name->SetFixedWidth(uWidth);
		nick_name->SetText(html_text.GetData());
	}

	if (!item.folder && strlen(item.user._signature)>0)
	{
		CLabelUI* description = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kDescriptionControlName));
		if (description != NULL)
		{
			s_result = _T("");
			if (item.user._signature != NULL && strlen(item.user._signature) > 0)
			{
				CA2W input(node->data().user_._signature, CP_ACP);
				s_result = input;
			}
#if defined(UNDER_WINCE)
			_stprintf(szBuf, _T("<x 5><c #808080>%s</c>"), s_result.GetData());
#else
			_stprintf_s(szBuf, MAX_PATH - 1, _T("<x 5><c #808080>%s</c>"), s_result.GetData());
#endif
			description->SetShowHtml(true);
			TFontInfo* pFont = paint_manager_.GetFontInfo(description->GetFont());
			unsigned int uWidth = pFont->tm.tmAveCharWidth;
			std::string sDescription("");
			if( wcslen(s_result) > 0)
			{
				CW2A input(s_result);
				sDescription = input;
			}

			int iLen = strlen(sDescription.c_str());
			uWidth *= ( iLen);
			uWidth += pFont->tm.tmMaxCharWidth;

			description->SetFixedWidth(uWidth);
			description->SetText(szBuf);
		}
	}
	CLabelUI* pResverd = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement,kResveredControlName));
	if (pResverd != NULL)
	{
		pResverd->SetShowHtml(true);
		if(item.folder)
			pResverd->SetVisible(false);
	}
	if(tempNode==NULL) pListElement->SetFixedHeight(kFriendListItemNormalHeight);
	else pListElement->SetFixedHeight(kFriendListItemSelectedHeight);
	CContainerUI* pOperatorPannel = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pListElement, kOperatorPannelControlName));
	if (tempNode==NULL&&pOperatorPannel != NULL)
	{
		pOperatorPannel->SetVisible(false);
	}
	pListElement->SetTag((UINT_PTR)node);
	int index = 0;
	if (parent->has_children())
	{
		Node* prev = parent->get_last_child();
		index = prev->data().list_elment_->GetIndex() + 1;
	}
	else 
	{
		if (parent == root_node_)
			index = 0;
		else
			index = parent->data().list_elment_->GetIndex() + 1;
	}
	if (!CListUI::AddAt(pListElement, index))
	{
		delete pListElement;
		delete node;
		node = NULL;
	}

	parent->add_child(node);

	return node;
}

Node* CUserListUI::AddNode(const UserInfo& user, Node* parent )
{
	UserListItemInfo item;
	item.folder = false;
	item.empty = false;
	item.user = user;
	Node* node = AddNode(item,parent);
	
	if(parent)SetUserCount(parent,0,parent->num_children());
	return node;
}
Node* CUserListUI::AddRoot(const UserInfo& user)
{
	UserListItemInfo item;
	item.folder = true;
	item.empty = false;
	item.user = user;
	return AddNode(item);
}

UserInfo* CUserListUI::GetUser(Node* node)
{
	if(node == NULL)
		return NULL;
	return &(node->data().user_);
}

bool CUserListUI::RemoveNode(Node* node)
{
	if (!node || node == root_node_) return false;

	for (int i = 0; i < node->num_children(); ++i)
	{
		Node* child = node->child(i);
		RemoveNode(child);
	}

	CListUI::Remove(node->data().list_elment_);
	node->parent()->remove_child(node);
	delete node;
	node = NULL;
	return true;
}

void CUserListUI::SetChildVisible(Node* node, bool visible)
{
	if (!node || node == root_node_)
		return;

	if (node->data().child_visible_ == visible)
		return;

	node->data().child_visible_ = visible;

	TCHAR szBuf[MAX_PATH] = {0};
	CDuiString html_text;
	if (node->data().has_child_)
	{
		if (node->data().child_visible_)
			html_text += level_expand_image_;
		else
			html_text += level_collapse_image_;

#if defined(UNDER_WINCE)
		_stprintf(szBuf, _T("<x %d>"), level_text_start_pos_);
#else
		_stprintf_s(szBuf, MAX_PATH - 1, _T("<x %d>"), level_text_start_pos_);
#endif
		html_text += szBuf;

		CDuiString s_result = _T("");
		if (node->data().user_._nickname != NULL && strlen(node->data().user_._nickname) > 0)
		{
			CA2W input(node->data().user_._nickname, CP_ACP);
			s_result = input;
		}
		html_text += s_result;

		CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kNickNameControlName));
		if (nick_name != NULL)
		{
			nick_name->SetShowHtml(true);
			TFontInfo* pFont = paint_manager_.GetFontInfo(nick_name->GetFont());
			unsigned int uWidth = pFont->tm.tmAveCharWidth;
			std::string sNickName("");
			if( wcslen(s_result) > 0)
			{
				CW2A input(s_result);
				sNickName = input;
			}

			int iLen = strlen(sNickName.c_str());
			uWidth *= (1 + iLen);
			if (node->data().folder_)
				nick_name->SetFixedWidth(uWidth+10+level_text_start_pos_);
			else 
				nick_name->SetFixedWidth(uWidth);
			nick_name->SetText(html_text.GetData());
		}
	}

	if (!node->data().list_elment_->IsVisible())
		return;

	if (!node->has_children())
		return;

	Node* begin = node->child(0);
	Node* end = node->get_last_child();
	for (int i = begin->data().list_elment_->GetIndex(); i <= end->data().list_elment_->GetIndex(); ++i)
	{
		CControlUI* control = GetItemAt(i);
		if (_tcsicmp(control->GetClass(), _T("ListContainerElementUI")) == 0)
		{
			if (visible) 
			{
				Node* local_parent = ((Node*)control->GetTag())->parent();
				if (local_parent->data().child_visible_ && local_parent->data().list_elment_->IsVisible())
				{
					control->SetVisible(true);
				}
			}
			else
			{
				control->SetVisible(false);
			}
		}
	}
}

bool CUserListUI::CanExpand(Node* node) const
{
	if (!node || node == root_node_)
		return false;

	return node->data().has_child_;
}

bool CUserListUI::SelectItem(int iIndex, bool bTakeFocus)
{
    if( iIndex == m_iCurSel ) return true;

    // We should first unselect the currently selected item
    if( m_iCurSel >= 0 ) {
        CControlUI* pControl = GetItemAt(m_iCurSel);
        if( pControl != NULL) {
            IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
            if( pListItem != NULL )
			{
				CListContainerElementUI* pFriendListItem = static_cast<CListContainerElementUI*>(pControl);
				Node* node = (Node*)pControl->GetTag();
				if ((pFriendListItem != NULL) && (node != NULL) && !node->folder())
				{
//					pFriendListItem->SetFixedHeight(kFriendListItemNormalHeight);
// 					CContainerUI* pOperatorPannel = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pFriendListItem, kOperatorPannelControlName));
// 					if (pOperatorPannel != NULL)
// 					{
// 						pOperatorPannel->SetVisible(false);
// 					}
				}
				pListItem->Select(false);
			}
        }

        m_iCurSel = -1;
    }

    if( iIndex < 0 )
		return false;

	if (!__super::SelectItem(iIndex, bTakeFocus))
		return false;


	CControlUI* pControl = GetItemAt(m_iCurSel);
	if( pControl != NULL) {
		CListContainerElementUI* pFriendListItem = static_cast<CListContainerElementUI*>(pControl);
		Node* node = (Node*)pControl->GetTag();
		if ((pFriendListItem != NULL) && (node != NULL) && !node->folder())
		{
// 			pFriendListItem->SetFixedHeight(kFriendListItemSelectedHeight);
// 			CContainerUI* pOperatorPannel = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pFriendListItem, kOperatorPannelControlName));
// 			if (pOperatorPannel != NULL)
// 			{
// 				pOperatorPannel->SetVisible(true);
// 			}
		}
	}
	return true;
}

void CUserListUI::SetUserCount(Node* node,int iOnLine,int iTotal)
{
	if (!node || node == root_node_) return ;

	CLabelUI* description = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kDescriptionControlName));

	if (description != NULL)
	{
		TCHAR szCount[MAX_PATH] = {9};
#if defined(UNDER_WINCE)
		_stprintf(szCount, _T("[%d/%d]"), iOnLine,iTotal);
#else
		_stprintf_s(szCount, MAX_PATH - 1, _T("[%d/%d]"), iOnLine,iTotal);
#endif
		description->SetText(szCount);
	}
}
} // namespace DuiLib