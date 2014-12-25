#include "StdAfx.h"
#include "ServerManager.h"
#include "HttpManager.h"
#include "HttpThread.h"
#include "../YApp.h"
CServerManager::CServerManager(){}
CServerManager::~CServerManager(){}


// void CServerManager::GetFlowList(vector<Flow>& flowList)
// {
// 
// }

int CServerManager::Login(CString userName, CString password)
{
	std::string sUtf8UserName = em_utility::mci::unicode_2_utf8(userName);
	std::string sUtf8PassWord = em_utility::mci::unicode_2_utf8(password); 
	CString web_url = _T("http://www.douban.com/j/app/login?email=%s&password=%s&app_name=radio_desktop_win&version=100");
	CString sUrl; sUrl.Format(web_url,userName,password);
	std::string sUtf8Url = em_utility::mci::unicode_2_utf8(sUrl);

	CurlData* pdata = new CurlData();
	pdata->SetURL(sUtf8Url.c_str());
//	CHttpThread* thread = new CHttpThread;
//	CHttpThread *thread = new CHttpThread(pdata,NULL,WM_WEB_SERVICE);
//	thread->Start();
 	HttpManagerProto* Manager = new HttpManagerProto;
 	ASSERT(Manager);
 	Manager->CurlGlobalInit();
 	int ret = Manager->DoHttpGet(pdata);

	{
//		char* ret = pdata->buffer;
// 	if(ret[0] == '1')
// 	{
// 		return 1; //登录成功.
// 	}
// 	if(ret[0] == '0')
// 	{
// 		return 0; //登录验证失败.
// 	}
	}
	return -1; //登录过程异常.

}