#ifndef _login_list_h
#define _login_list_h

#pragma  once
#include "../list/UILoginList.h"
class CLogin;

class CLoginList
	: public CWin
	, public INotifyUI
	, public IDialogBuilderCallback
{
public:
	CLoginList(POINT point,CLogin* pParent);
	~CLoginList();
public:
	LPCTSTR GetWindowClassName() const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void Notify(TNotifyUI& msg);
public:
	template<typename T>
	void FindControl(T& pctrl,LPCTSTR pstrName) const;
	void OnInitWindow(TNotifyUI& msg);
	void OnClick(TNotifyUI& msg);
	void OnItemClick(TNotifyUI& msg);
public:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
protected:
private:
private:
	CLoginListUI* m_pLoginList;
	POINT m_ptBasedPoint;
	CLogin* m_pLoginWnd;
};


#endif//_login_list_h