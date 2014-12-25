#ifndef __LOGIN_H_
#define __LOGIN_H_

#pragma once


class CLoginWnd 
	: public CWin
	, public INotifyUI
	, public IDialogBuilderCallback
	, public IMessageFilterUI
{
public:
	static const LPCTSTR CLASSNAME;
	CLoginWnd();
	~CLoginWnd();
	LPCTSTR GetWindowClassName() const;
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM , bool& bHandled);
	CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void Notify(TNotifyUI& msg);
	void TypeInit(TNotifyUI& msg);
	void TypeClick(TNotifyUI& msg);
	bool LoginNow();
	bool LogIn();
	bool RegisterIn();
	void SetLoginTip(CString sTip);
	void SetRegTip(CString sTip);
	void SetPage(int iIndex);
	int  GetPage();
	void SetTitle(CString sTitle);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled);
private:
	CEditUI *m_pEditEmail,*m_pEditPwd;
	CEditUI *m_pEditAcount,*m_pEditPassword;
	CAnimationTabLayoutUI* m_pTabSwitch;
};
	


#endif//__LOGIN_H_