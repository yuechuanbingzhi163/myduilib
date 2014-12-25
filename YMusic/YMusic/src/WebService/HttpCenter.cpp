#include "stdafx.h"
#include "HttpCenter.h"

#include "../Login/LoginWindow.h"

CHttpCenter::CHttpCenter()
{
	
}
CHttpCenter::~CHttpCenter()
{
	//此处不可再delete，因为CThread类已经做了delete操作
// 	if (m_pHttpThread)
// 	{	
// 		delete m_pHttpThread;
// 		m_pHttpThread = NULL;
// 	}
}
bool CHttpCenter::MakeLogin(UNICODESTR& strUserKey,UNICODESTR& strPassword)
{
	return false;
}
bool CHttpCenter::MakeLogin(UNICODESTR& sToken)
{
	return false;
}
bool CHttpCenter::MakeLogin(UTF8STR& strUserKey,UTF8STR& strPassword)
{
	CurlData* pData = new CurlData;
	pData->SetURL("http://localhost/YMusic/login.php");
	pData->n_type = url::WSRT_LOGIN;
	std::string sparam;
	sparam = "email=";
	sparam += strUserKey;
	sparam += "&";
	sparam += "password=";
	sparam += strPassword;
	pData->SetData(sparam.c_str());

	//HttpManagerProto::Instance()->DoHttpPost(pData);
// 	CHttpThread* pthread = new CHttpThread(pData,CLoginWindow::Instance()->GetHWND(),WM_WEB_SERVICE);
// 	pthread->Start();
	CHttpThread* pHttpThread = NULL;
	pHttpThread = new CHttpThread();
	pHttpThread->SetData(pData);
	pHttpThread->SetHwnd(CLoginWindow::Instance()->GetHWND());
	pHttpThread->SetMsg(WM_WEB_SERVICE);
	pHttpThread->Start();

	return false;
}
bool CHttpCenter::MakeLogin(UTF8STR sToken)
{
	return false;
}
