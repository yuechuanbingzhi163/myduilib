#ifndef _login_h
#define _login_h

#pragma once
#include "login_list.h"

class CLogin 
	: public CWin
	, public INotifyUI
	, public IDialogBuilderCallback
{
public:
	CLogin();
	~CLogin();
public:
	LPCTSTR GetWindowClassName() const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void Notify(TNotifyUI& msg);
public:
	template<typename T>
	void FindControl(T& pctrl,LPCTSTR pstrName) const;
public:
	void InitAllVariable();
	void OnInitWindow(TNotifyUI& msg);
	void OnClick(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	bool OnMouseEvent(void* param);
public:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	CLoginList* GetLoginList();
protected:
protected:
private:
private:
	CEditUI*   m_pAccountEdt;
	CButtonUI* m_pDropDownAccountBtn;
	CEditUI*   m_pPasswdEdt;
	CButtonUI* m_pKeyBoardBtn;
	CButtonUI* m_pRegAccount;
	CButtonUI* m_pForgetPsd;

	CLoginList* m_pLoginList;
};


#endif//_login_h