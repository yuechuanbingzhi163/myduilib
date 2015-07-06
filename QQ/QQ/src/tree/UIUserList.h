#ifndef UIUSERLIST_H
#define UIUSERLIST_H
#pragma once

#include "UIListCommonDefine.h"
#include "../userinfo/user.h"

namespace DuiLib
{
struct FriendListItemInfo
{
	bool folder;
	bool empty;
	CDuiString id;
	CDuiString logo;
	CDuiString nick_name;
	CDuiString description;
};
struct UserListItemInfo
{
	bool folder;
	bool empty;
	UserInfo user;
};

class CUserListUI : public CListUI
{
public:
	enum {SCROLL_TIMERID = 10};

	CUserListUI(CPaintManagerUI& paint_manager);

	~CUserListUI();

	bool Add(CControlUI* pControl);

	bool AddAt(CControlUI* pControl, int iIndex);

	bool Remove(CControlUI* pControl);

	bool RemoveAt(int iIndex);

	void RemoveAll();

	void DoEvent(TEventUI& event);

	Node* GetRoot();

	Node* AddNode(const UserListItemInfo& item, Node* parent = NULL);
	Node* AddNode(const UserInfo& user, Node* parent = NULL);
	Node* AddRoot(const UserInfo& user);
	bool RemoveNode(Node* node);

	void SetChildVisible(Node* node, bool visible);

	bool CanExpand(Node* node) const;

	bool SelectItem(int iIndex, bool bTakeFocus = false);

	void SetUserCount(Node*node,int iOnLine,int iTotal);

	UserInfo* GetUser(Node* node);
private:
	Node*	root_node_;
	LONG	delay_deltaY_;
	DWORD	delay_number_;
	DWORD	delay_left_;
	CDuiRect	text_padding_;
	int level_text_start_pos_;
	CDuiString level_expand_image_;
	CDuiString level_collapse_image_;
	CPaintManagerUI& paint_manager_;

    CDialogBuilder m_dlgBuilder;
};

} // DuiLib

#endif // UIUSERLIST_H