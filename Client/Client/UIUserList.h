
/***********************************************************
 *	FileName£ºUIUserList.hpp
 *  Author  £ºDFn Li
 *  Timer   : 2013/3/2 14:24
 *  E-Mail  : libyyu@qq.com
 *  Comment : 
 *  Version : 1.0.0.1
 ***********************************************************/

#ifndef UIUSERLIST_HPP
#define UIUSERLIST_HPP

#pragma once
struct User;

namespace DuiLib{

double CalculateDelay(double state);

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

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	bool AddNode(User UserInfo);
//	bool AddNode(std::vector<USERINFO> UserList);
private:

	LONG	delay_deltaY_;
	DWORD	delay_number_;
	DWORD	delay_left_;


    CDialogBuilder m_dlgBuilder;

public:
	CPaintManagerUI& paint_manager_;
};

};


#endif // UIUSERLIST_HPP