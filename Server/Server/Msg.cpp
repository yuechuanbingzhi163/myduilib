#include "stdafx.h"
#include "Msg.h"

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
	m_systemMsg.msgItem=m_systemMsg.type=_T("");
//	m_loadMsg.name=m_loadMsg.password=_T("");

}

/*
<MSGTYPE TYPE="SYSTEMMSG">
	<WHO NAME="ME" />
	<TOWHO NAME="YOU" />
	<MSG CONTEXT="hello,this is a test" />
	<TTIME TIME="2012-06-21 13:26:30" />
</MSGTYPE>
*/
CString CMsg::WhatType()
{
//	DUI__Trace(m_strText);
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
<MSGTYPE TYPE="NOMALMSG">
	<WHO NAME="ME" />
	<TOWHO NAME="YOU" />
	<MSG CONTEXT="hello,this is a test" />
	<TTIME TIME="2012-06-21 13:26:30" />
</MSGTYPE>
*/
void CMsg::BuildNomal()
{
	CMarkup m_xml(m_strText);
	CMarkupNode root = m_xml.GetRoot(); 
	if( !root.IsValid() ) return ;
	LPCTSTR pstrClass = NULL;
	LPCTSTR pstrName = NULL;
	LPCTSTR pstrValue = NULL;
	for(CMarkupNode node=root.GetChild();node.IsValid();node=node.GetSibling())
	{
		pstrClass = node.GetName();
		if(_tcscmp(pstrClass, _T("WHO")) == 0 )
		{
			pstrName = node.GetAttributeName(0);
			if(_tcscmp(pstrName, _T("NAME")) == 0 )
			{
				pstrValue = node.GetAttributeValue(0);
				m_nomalMsg.who = pstrValue;
			}
		}
		if(_tcscmp(pstrClass, _T("TOWHO")) == 0 )
		{
			pstrName = node.GetAttributeName(0);
			if(_tcscmp(pstrName, _T("NAME")) == 0 )
			{
				pstrValue = node.GetAttributeValue(0);
				m_nomalMsg.toWho = pstrValue;
			}
		}
		if(_tcscmp(pstrClass, _T("MSG")) == 0 )
		{
			pstrName = node.GetAttributeName(0);
			if(_tcscmp(pstrName, _T("CONTEXT")) == 0 )
			{
				pstrValue = node.GetAttributeValue(0);
				m_nomalMsg.msgItem = pstrValue;
			}
		}
		if(_tcscmp(pstrClass, _T("TTIME")) == 0 )
		{
			pstrName = node.GetAttributeName(0);
			if(_tcscmp(pstrName, _T("TIME")) == 0 )
			{
				pstrValue = node.GetAttributeValue(0);
				m_nomalMsg.sndTime = pstrValue;
			}
		}
	}
}
void CMsg::BuildLoad()
{
	int begin=0;
	int end=0;
	end=m_strText.Find(MSGTYPE::SEP);

	begin=end+1;
	end=m_strText.Find(MSGTYPE::SEP,begin);
    m_loadMsg.name=m_strText.Mid(begin,end-begin);//提取是谁发的消息

	begin=end+1;
    m_loadMsg.password=m_strText.Mid(begin);//消息体

}
/*
<MSGTYPE TYPE="SYSTEMMSG">
	<WHAT TYPE="LOAD" />
	<WHO NAME="YOU" />
	<MSG CONTEXT="hello,this is a test" />
	<TTIME TIME="2012-06-21 13:26:30" />
</MSGTYPE>
*/

void CMsg::BuileSystem()
{
	CMarkup m_xml(m_strText);
	CMarkupNode root = m_xml.GetRoot(); 
	if( !root.IsValid() ) return ;
	LPCTSTR pstrClass = NULL;
	LPCTSTR pstrName = NULL;
	LPCTSTR pstrValue = NULL;
	for(CMarkupNode node=root.GetChild();node.IsValid();node=node.GetSibling())
	{
		pstrClass = node.GetName();
		if(_tcscmp(pstrClass, _T("WHAT")) == 0 )
		{
			pstrName = node.GetAttributeName(0);
			if(_tcscmp(pstrName, _T("TYPE")) == 0 )
			{
				pstrValue = node.GetAttributeValue(0);
				m_systemMsg.type = pstrValue;
			}
		}
		if(_tcscmp(pstrClass, _T("WHO")) == 0 )
		{
			pstrName = node.GetAttributeName(0);
			if(_tcscmp(pstrName, _T("NAME")) == 0 )
			{
				pstrValue = node.GetAttributeValue(0);
				m_systemMsg.name = pstrValue;
			}
		}
		if(_tcscmp(pstrClass, _T("MSG")) == 0 )
		{
			pstrName = node.GetAttributeName(0);
			if(_tcscmp(pstrName, _T("CONTEXT")) == 0 )
			{
				pstrValue = node.GetAttributeValue(0);
				m_systemMsg.msgItem = pstrValue;
			}
		}
		if(_tcscmp(pstrClass, _T("TTIME")) == 0 )
		{
			pstrName = node.GetAttributeName(0);
			if(_tcscmp(pstrName, _T("TIME")) == 0 )
			{
				pstrValue = node.GetAttributeValue(0);
				m_systemMsg.sndTime = pstrValue;
			}
		}
	}
}


