#ifndef __LOGIN_NOTICE_H__
#define __LOGIN_NOTICE_H__

#pragma once

enum EMT_NOTICE_TYPE
{
	EMT_NORMAL = 0,
	EMT_GROUP_MSG = 1,//群聊消息
	EMT_SYS_MSG ,//系统消息
	EMT_OFFLINE_MSG ,//离线消息
	EMT_FRIEND_MSG,//私聊消息
	EMT_FRIEND_LOAD,
	EMT_FRIEND_SHAKE
};

class CNotice
	: public CWin
	, public INotifyUI
	, public IDialogBuilderCallback
	, public IMessageFilterUI
{
public:
	static const LPCTSTR CLASSNAME;
	CNotice(HWND pParent,LPCTSTR xml,LPCTSTR text,EMT_NOTICE_TYPE type);
	~CNotice();
	LPCTSTR GetWindowClassName() const;
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM , bool& bHandled);
	CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void Notify(TNotifyUI& msg);
	void TypeInit(TNotifyUI& msg);
	void TypeClick(TNotifyUI& msg);
	void HideNoNotice(bool bHide = true);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled);
	LRESULT OnNcLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	CString m_sWho;
private:
	CDuiString m_strText;
	CDuiString m_xmlFile;
	EMT_NOTICE_TYPE m_NoticeType;
	
};
extern bool bGroupMsgTip;
extern bool bFriednMsgTip;
extern bool bFriednLoadTip;
extern bool bFriednShakeTip;
#endif//__LOGIN_NOTICE_H__