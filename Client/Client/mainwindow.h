#ifndef __MAINWND_H_
#define __MAINWND_H_

#pragma once
#include "UIUserList.h"
#include "UIFriends.h"
#include "Login.h"

class CChatSocket;
class CListenSocket;
class CMsg;
#include "lock.h"
#include "trayIcon.h"
#include "chat_dialog.h"

class CMainWindow 
	: public CWin
	, public INotifyUI
	, public IDialogBuilderCallback
{
public:
	CMainWindow();
	~CMainWindow();
	static const LPCTSTR CLASSNAME;
public:
	LPCTSTR GetWindowClassName() const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void Notify( TNotifyUI &msg );
	void TypeClick(TNotifyUI& msg);
	void TypeInitWindow(TNotifyUI& msg);
	void TypeSelectedChange(TNotifyUI& msg);
	void TypeItemActive(TNotifyUI& msg);
	void TypeItemClick(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTray(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer();
	virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	template<typename T>
	void FindControl(T& pctrl,LPCTSTR pstrName) const;
	bool BuildEnvironment();
	void InitAllControls();
	void OnSendMsg(TNotifyUI& msg);
	void FormatMsgToEditView(CString sName,CString sTime,CString sMsg);
	void UpdateUserInfo(); 
	void EmptyUserList();
	void AddRootToFriendList();
	void DisAllConnect();
	void ShowMsg(CMsg* pMsg);
	void SetTitle(CString sName);
	void MsgComming(CMsg* pMsg);
	void SetPage(int iIndex);
public:
	bool ConnectToSrv(LPCTSTR lpIP = _T("127.0.0.1"),UINT nPort = SERVERPORT);
public:
	void SendMsg(CString& strText);
	void OnReceive(CChatSocket* pSocket);
	CString BuildNomalMsg(CString& preString,CString& toWho);//聊天消息
	CString BuildRegMsg();//注册
	CString BulidLoadMsg();//登录
	CString BulidChangeMsg();//改名
	CString BulidCancelMsg();//取消登录
	CString BuildShakeMsg();
//	CString BuildChatMsg(CString& preString);
	bool IsUserLoad(CString& textStr);
	bool IsUserRegit(CString& textStr);
	bool IsSystemMsg(CString& textStr);
	CString AddTimeMsg(CString& text);
	
	void LogOut();
	bool IsToMe(CString& text);
	bool IsMeSnd(CString& text);
	bool CheckId(CString& preIp);
private:
	bool IsNumberOfIp(CString ipSeg);
public:
	bool CreateUDPSocket(CString sIP,CString sPort);
	void RecvFrom(CListenSocket* pSocket);
	void SendTo(CString peerIP,CString peerPort,CString msg);
	void SendTo(User user,CString msg);
	bool bIsUserOnLine(CString name);
	bool FindUserFromList(CString name,User& user);
	CString BuildChatMsg(CString sMsg,CString sToWho);
	CString BuildShakeMsg(CString sToWho);
	void ShowChatBox(CString sWho,CMsg* pMsg);
	bool IsChatBoxOpen(CString sName);
	ChatDialog* FindChatBox(CString sName);
	bool HasMsgForUserInQueue(CString sName);
	void ClearAllVar();
	void ShakeWindow(HWND hWnd);
	void ShakeToWho(User user);
public:
	struct IControls
	{
		CFadeButtonUI* _pMinBtn;
		CFadeButtonUI* _pMaxBtn;
		CFadeButtonUI* _pResBtn;
		CFadeButtonUI* _pCloseBtn;

		CRichEditUI *_pSearchRichEdit;
		CButtonUI   *_pSearchBtn;

		COptionUI   *_pFriendOpt;
		COptionUI   *_pGroupOpt;
		COptionUI   *_pMicroBlogOpt;
		COptionUI   *_pGroupChatOpt;

		CAnimationTabLayoutUI *_pUserTab;

		CFriendsUI* _pFriendTree;
		CUserListUI* _pGroupList;
		CUserListUI* _pBlogList;

	};
public:
	CLock    m_LockuserList;
	CPtrList m_userList;
private:
	CListenSocket* m_pChatSocket;//UDP通信
	CMsg*   m_pChatMsg;//UDP通信消息处理
	CChatSocket* m_pSocket;//与服务端通信
	CString m_nameCopy;
	CString m_IpAddress;
	CMsg*   m_pMsgRcv;
	CLoginWnd* m_pLogin;
private:
	Node* _parent_node_online_friends;
	Node* _parent_node_offline_friends;
	Node* _parent_node_all_firends;

	CLock   m_lockEditView;
	CRichEditUI* m_pMsgView;
	CRichEditUI* m_pInput;

	bool emotion_timer_start_;

	bool bold_;
	bool italic_;
	bool underline_;
	DWORD text_color_;
	DWORD font_size_;
	CDuiString font_face_name_;
	IControls  _ui;
	CTrayIconController m_tray;
};

extern CMainWindow* theApp;
extern bool theConning;
#endif//__MAINWND_H_