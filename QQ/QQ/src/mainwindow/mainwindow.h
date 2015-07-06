#ifndef _mainwindow_h
#define _mainwindow_h

#pragma once
struct UserInfo;
#include "../tree/UIUserList.h"

class CMainWindow 
	: public CWin
	, public INotifyUI
	, public IDialogBuilderCallback
{
public:
	CMainWindow();
	~CMainWindow();
public:
	LPCTSTR GetWindowClassName() const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void InitAllVariable();
	template<typename T>
	void FindControl(T& pctrl,LPCTSTR pstrName) const;
	void Notify(TNotifyUI& msg);
	void OnInitWindow(TNotifyUI& msg);
	void OnClick(TNotifyUI& msg);
	void OnSelectChanged(TNotifyUI& msg);
	void OnItemActive(TNotifyUI& msg);
	void OnItemClick(TNotifyUI& msg);
	void OnKillFocus(TNotifyUI& msg);
	void OnReturn(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
	void OnMenu(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
public:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	typedef CUserListUI CFriendListUI ;
	typedef CUserListUI CGroupListUI;
	typedef CUserListUI CRecentListUI;
protected:
private:
private:
	CTabLayoutUI* m_pTabSwitch;
	CFriendListUI* m_pFriendTree;
	CGroupListUI* m_pGroupTree;
	CTreeViewUI* m_pRecentFriend;

};

extern CMainWindow* theApp;
#endif//_mainwindow_h