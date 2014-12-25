#ifndef CMSG1_H
#define CMSG1_H

#pragma once
namespace MSGTYPE
{
	static const TCHAR* SEP = _T("_");

	static const TCHAR* NULLMSG = _T("NULLMSG");
	static const TCHAR* NOMALMSG = _T("NOMALMSG");//一般聊天消息
	static const TCHAR* SYSTEMMSG = _T("SYSTEMMSG");//系统消息
	static const TCHAR* LOADMSG = _T("LOADMSG");//请求登录消息
	static const TCHAR* ERRORMSG = _T("ERRORMSG");//错误消息

	static const TCHAR* SERVERDOWN = _T("SERVERDOWN");//服务端关闭
	static const TCHAR* FAIL = _T("FAIL");//失败
	static const TCHAR* SUCCESS = _T("SUCCESS");//成功
	static const TCHAR* NOID = _T("NOID");//未注册
	static const TCHAR* REGI = _T("REGI");//注册
	static const TCHAR* OFLINE = _T("OFFLINE");//未登录
	static const TCHAR* LOAD = _T("LOAD");//登录
	static const TCHAR* CLOSE = _T("CLOSE");//客户端退出
	static const TCHAR* CHANGE = _T("CHANGE");//客户端改名
	static const TCHAR* USERLIST = _T("USERLIST");//用户列表
	static const TCHAR* CHAT = _T("CHAT");//聊天信息
};
#define  ONLINE   _T("ONLINE")
#define  OFFLINE  _T("OFFLINE")
struct User
{
	CString m_loadName;
	CString m_password;
	CString m_callName;
	CString m_userState;
	CString m_userIp;
	CString m_userPort;
	User()
	{
		m_loadName = m_callName = m_password = m_userIp = m_userPort = _T("");
		m_userState = OFFLINE;
	}
};
class CMsg : public  CObject
{
    public:
		CString m_strText;
	    CString m_msgType;
		
		struct NomalMsg
		{
			CString who;
			CString toWho;
			CString msgItem;
			CString sndTime;
		} m_nomalMsg;
		struct SystemMsg
		{
			CString type;
			CString name;
			CString msgItem;
			CString sndTime;
		} m_systemMsg;

		struct LOADMSG
		{
			CString name;
			CString password;
		} m_loadMsg;
		
	public:
		CMsg(CString& preMsg);
		CMsg();
	    void Init();
 	    CString WhatType();
// 	//	void SetMsg();
// //	private:
 		void BuildNomal();
 		void BuildLoad();
 		void BuileSystem();

};
#endif