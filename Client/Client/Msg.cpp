#include "stdafx.h"
#include "Msg.h"
#include "mainwindow.h"

User theUser;

CMsg::CMsg(CString& preMsg)
{
	m_strText=preMsg;
	Init();

}
CMsg::CMsg()
{
	m_strText=_T("");
	Init();
}

void CMsg::Init()
{
	m_strText=_T("");
    m_nomalMsg.who=m_nomalMsg.toWho=m_nomalMsg.msgItem=_T("");
//	m_loadMsg.name=m_loadMsg.password=_T("");

}
/*
<MSGTYPE TYPE="LOGIN">
	<MSG who="name" result="success" time="" />
</MSGTYPE>
*/
CString CMsg::WhatType()
{
	CMarkup m_xml(m_strText);

	CString preType(MSGTYPE::NULLMSG);
	CMarkupNode root = m_xml.GetRoot(); //tag = <MSGTYPE TYPE="">
	if( !root.IsValid() ) return preType;
	LPCTSTR pstrClass = NULL;
	int nAttributes = 0;
	LPCTSTR pstrName = NULL;
	LPCTSTR pstrValue = NULL;
	LPTSTR pstr = NULL;
	pstrClass = root.GetName();
	if(_tcscmp(pstrClass, _T("MSGTYPE")) != 0 )
		return preType;
	nAttributes = root.GetAttributeCount();
	if(nAttributes==0) return preType;
	pstrName = root.GetAttributeName(0);
	pstrValue = root.GetAttributeValue(0);
	if(_tcscmp(pstrName, _T("TYPE")) == 0 )
		preType = pstrValue;
	return preType;
}
/*
<MSGTYPE TYPE="LOGIN">
	<MSG who="name" result="success" time="" />
</MSGTYPE>
*/

void CMsg::BuildNomal()
{
	CMarkup m_xml(m_strText);
	CMarkupNode root = m_xml.GetRoot(); 
	if( !root.IsValid() ) return ;
	LPCTSTR pstrClass = NULL;
	LPCTSTR pstrValue = NULL;
	LPCTSTR pstrValue2 = NULL;
	LPCTSTR pstrValue3 = NULL;
	CMarkupNode node = root.GetChild();
	if(!node.IsValid()) return;
	pstrClass = node.GetName();
	int nAttributes = node.GetAttributeCount();
	if(nAttributes==0||nAttributes>3) return ;
	if (_tcscmp(pstrClass, _T("MSG")) ==0)
	{
		pstrValue = node.GetAttributeValue(0);
		pstrValue2 = node.GetAttributeValue(1);
		pstrValue3 = node.GetAttributeValue(2);
		m_nomalMsg.who = pstrValue;
		m_nomalMsg.toWho = pstrValue2;
		m_nomalMsg.msgItem = pstrValue3;
	}
}
/*
<MSGTYPE TYPE="CHATMSG">
	<MSG who="name" towho="name" text="" time="" />
</MSGTYPE>
*/
void CMsg::BuildChatMsg()
{
	CMarkup m_xml(m_strText);
	CMarkupNode root = m_xml.GetRoot(); 
	if( !root.IsValid() ) return ;
	LPCTSTR pstrClass = NULL;
	LPCTSTR pstrValue = NULL;
	LPCTSTR pstrValue2 = NULL;
	LPCTSTR pstrValue3 = NULL;
	LPCTSTR pstrValue4 = NULL;
	CMarkupNode node = root.GetChild();
	if(!node.IsValid()) return;
	pstrClass = node.GetName();
	int nAttributes = node.GetAttributeCount();
	if(nAttributes==0||nAttributes>4) return ;
	if (_tcscmp(pstrClass, _T("MSG")) ==0)
	{
		pstrValue = node.GetAttributeValue(0);
		pstrValue2 = node.GetAttributeValue(1);
		pstrValue3 = node.GetAttributeValue(2);
		pstrValue4 = node.GetAttributeValue(3);
		m_nomalMsg.who = pstrValue;
		m_nomalMsg.toWho = pstrValue2;
		m_nomalMsg.msgItem = pstrValue3;
		m_nomalMsg.sndTime = pstrValue4;
	}
}
//<MSGTYPE TYPE="SYSMSG">
//  <MSG who="name" what="load" msg="上线了" time="" />
//</MSGTYPE>
void CMsg::BuildSystem()
{
	CMarkup m_xml(m_strText);
	CMarkupNode root = m_xml.GetRoot(); 
	if( !root.IsValid() ) return ;
	LPCTSTR pstrClass = NULL;
	LPCTSTR pstrValue = NULL;
	LPCTSTR pstrValue2 = NULL;
	LPCTSTR pstrValue3 = NULL;
	LPCTSTR pstrValue4 = NULL;
	CMarkupNode node = root.GetChild();
	if(!node.IsValid()) return;
	pstrClass = node.GetName();
	int nAttributes = node.GetAttributeCount();
	if(nAttributes==0||nAttributes>3) return ;
	if (_tcscmp(pstrClass, _T("MSG")) ==0)
	{
		pstrValue = node.GetAttributeValue(0);
		pstrValue2 = node.GetAttributeValue(1);
		pstrValue3 = node.GetAttributeValue(2);
		pstrValue4 = node.GetAttributeValue(3);
		m_nomalMsg.who = pstrValue;
		m_nomalMsg.toWho = pstrValue2;
		m_nomalMsg.msgItem = pstrValue3;
		m_nomalMsg.sndTime = pstrValue4;
	}
}

//<MSGTYPE TYPE="USERLIST">
//  <USER loadname="loadname" callname="callname" ip="ip" port="port" state="online" />
//  <USER loadname="loadname" callname="callname" ip="ip" port="port" state="online" />
//</MSGTYPE>
void CMsg::BuildUserList()
{
	CMarkup m_xml(m_strText);
	CMarkupNode root = m_xml.GetRoot(); 
	if( !root.IsValid() ) return ;
	LPCTSTR pstrClass = NULL;
	LPCTSTR pstrValue = NULL;
	LPCTSTR pstrValue2 = NULL;
	LPCTSTR pstrValue3 = NULL;
	LPCTSTR pstrValue4 = NULL;
	LPCTSTR pstrValue5 = NULL;
	theApp->EmptyUserList();
	User* pMySelf = new User;

	for (CMarkupNode node=root.GetChild();node.IsValid();node=node.GetSibling())
	{
		pstrClass = node.GetName();
		if (_tcscmp(pstrClass,_T("USER"))==0)
		{
			pstrValue = node.GetAttributeValue(0);
			pstrValue2 = node.GetAttributeValue(1);
			pstrValue3 = node.GetAttributeValue(2);
			pstrValue4 = node.GetAttributeValue(3);
			pstrValue5 = node.GetAttributeValue(4);
			if (_tcscmp(pstrValue,theUser.m_loadName)==0)
			{//用户自己
				pMySelf->m_loadName = pstrValue;
				pMySelf->m_callName = pstrValue2;
				pMySelf->m_userIp = pstrValue3;
				pMySelf->m_userPort = pstrValue4;
				pMySelf->m_userState = pstrValue5;

				theUser.m_callName = pstrValue2;
				theUser.m_userIp = pstrValue3;
				theUser.m_userPort = pstrValue4;
				theUser.m_userState = pstrValue5;
			}
			else
			{
				User* pUser = new User;
				pUser->m_loadName = pstrValue;
				pUser->m_callName = pstrValue2;
				pUser->m_userIp = pstrValue3;
				pUser->m_userPort = pstrValue4;
				pUser->m_userState = pstrValue5;
				theApp->m_LockuserList.Lock();
				theApp->m_userList.AddTail(pUser);
				theApp->m_LockuserList.UnLock();
			}		
		}
	}
	theApp->m_LockuserList.Lock();
	theApp->m_userList.AddHead(pMySelf);
	theApp->m_LockuserList.UnLock();

//	theApp->CreateUDPSocket(theUser.m_userIp,theUser.m_userPort);
}
//<MSGTYPE TYPE="SHAKE">
//  <MSG who="" result="" time="" />
//</MSGTYPE>
void CMsg::BuildShake()
{
	CMarkup m_xml(m_strText);
	CMarkupNode root = m_xml.GetRoot(); 
	if( !root.IsValid() ) return ;
	LPCTSTR pstrClass = NULL;
	LPCTSTR pstrValue = NULL;
	LPCTSTR pstrValue2 = NULL;
	CMarkupNode node = root.GetChild();
	if(!node.IsValid()) return;
	pstrClass = node.GetName();
	int nAttributes = node.GetAttributeCount();
	if(nAttributes==0||nAttributes>3) return ;
	if (_tcscmp(pstrClass, _T("MSG")) ==0)
	{
		pstrValue = node.GetAttributeValue(0);
		pstrValue2 = node.GetAttributeValue(2);
		m_nomalMsg.who = pstrValue;
		m_nomalMsg.sndTime = pstrValue2;
	}
}

