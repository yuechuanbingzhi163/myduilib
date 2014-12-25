#include "StdAfx.h"
#include "HttpThread.h"


CHttpThread::CHttpThread(CurlData* pdata, HWND hwnd, int iMessage)
{
	m_pdata = pdata;
	m_hUIWnd = hwnd;
	m_iMessage = iMessage;
}
CHttpThread::~CHttpThread()
{}

unsigned int CHttpThread::Run(void*)
{

	int ret = 0;
	ASSERT(m_pdata);
	if(!m_pdata)
		return 0;  
	if(m_pdata->bpost)
		ret = HttpManagerProto::Instance()->DoHttpPost(m_pdata);
	else
		ret = HttpManagerProto::Instance()->DoHttpGet(m_pdata);

	::PostMessage(m_hUIWnd, m_iMessage, WPARAM(m_pdata), (LPARAM)&ret);

	return ret;
}