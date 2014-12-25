#include "stdafx.h"
#include "LoginWindow.h"
#include "../YApp.h"
#include "WebLoginWindow.h"
#include "../WebService/HttpThread.h"

#include "../WebService/ResponseParser2.h"
#include "../WebService/HttpCenter.h"

CLoginWindow::CLoginWindow(void)//:m_isKeepLogin(FALSE)
{
	theMainWin->AddReceive(this);
}

CLoginWindow::~CLoginWindow(void)
{
	RemoveObserver();
}
LPCTSTR CLoginWindow::GetWindowClsss() 
{
	return _T("LoginWindow");
}
LPCTSTR CLoginWindow::GetWindowClassName() const
{
	return CLoginWindow::GetWindowClsss();
}
LRESULT CLoginWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sLog("CLoginWindow::OnCreate");

	ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if(!RegisterSkin(_T("login.xml")))
		return 0;
	GetPaintMgr()->AddNotifier(this);
	
	
	theMainWin->BroadCast();

	typedef struct _tagKeyValue_
	{
		const TCHAR*   key;
		CString&       svalue;
	}KeyValue;
	KeyValue keyValues[] = 
	{
		{_T("regURL"),m_regURL},
		{_T("forGetPasswordURL"),m_forgetURL},
		{_T("sinaLoginURL"),m_sinURL},
		{_T("qqLoginURL"),m_qqURL},
		{_T("webLoginSucceedURL"),m_webLoginSucceedURL}
	};
	for (size_t i = 0;i<sizeof(keyValues)/sizeof(keyValues[0]);++i)
		theDbMgr->GetGlobalDBase()->GetDictValue(keyValues[i].key,keyValues[i].svalue);

	if(m_regURL.IsEmpty())
		m_regURL.Format(_T("%s/member/register"),XIAMI_HOST);
	if(m_forgetURL.IsEmpty())
		m_forgetURL.Format(_T("%s/member/getpassword"),XIAMI_HOST);
	if(m_sinURL.IsEmpty())
		m_sinURL.Format(_T("%s/app/xiating/weibo"),XIAMI_HOST);
	if(m_qqURL.IsEmpty())
		m_qqURL.Format(_T("%s/app/xiating/qzone"),XIAMI_HOST);
	if(m_webLoginSucceedURL.IsEmpty())
		m_webLoginSucceedURL.Format(_T("%s/login/succeed?t="),XIAMI_HOST);
	theDbMgr->GetGlobalDBase()->BeginTransaction();
	for (size_t i = 0;i<sizeof(keyValues)/sizeof(keyValues[0]);++i)
	{
		theDbMgr->GetGlobalDBase()->DeleteDict(keyValues[i].key);
		theDbMgr->GetGlobalDBase()->InsertDict(keyValues[i].key,keyValues[i].svalue);
	}
	theDbMgr->GetGlobalDBase()->Commit();

	CControlUI *pCheckBox   = GetPaintMgr()->FindControl(_T("KeepLogin"));
	CCheckBoxUI *pKeepLogin = static_cast<CCheckBoxUI*>(pCheckBox);
	bool autoLogin = theConfigMgr->GetNormalConfig()->IsEnableAutoLogin();
	pKeepLogin->SetCheck(autoLogin);
	//用于提示
// 	CString strs[] = 
// 	{_T("@hotmail.com"),
// 	_T("@gamil.com"),
// 	_T("@weibo.com"),
// 	_T("@qq.com"),
// 	_T("@sina.com"),
// 	_T("@xiami.com"),
// 	_T("@163.com"),
// 	_T("@126.com"),
// 	_T("@yahoo.com"),
// 	_T("@yahoo.cn"),
// 	_T("@139.com")};

	return 0;
}


 void CLoginWindow::Notify(TNotifyUI& msg)
 {
	 YTrace_(_T("CLoginWindow::Notify class:%s,id:%s,msg:%s"),msg.pSender->GetClass(),msg.pSender->GetName().GetData(),msg.sType.GetData());
	 CControlUI* pSender = msg.pSender;

	 if (0 == msg.sType.Compare(kClick))
	 {
		 if (pSender->IsName(_T("close")))
			 Close(0);
		 else if (pSender->IsName(_T("login")))
			 OnLogin();
		 else if (pSender->IsName(_T("sina")))
			 OnSinaLogin();
		 else if (pSender->IsName(_T("QQ")))
			 OnQQLogin();
		 else if (pSender->IsName(_T("ForgetCode")))
			 OnForgetPassword();
		 else if (pSender->IsName(_T("register")))
			 OnReg();
	 }
	 else if(0 == msg.sType.Compare(kReturn))
	 {
		 OnLogin();
	 }
 }

 void CLoginWindow::Login(const CString& email,const CString& pwd,bool isMD5,bool isKeepLogin)
 {
	 spUserContextT pContext = theRuntimeState->GetCurrentContext(NULL);
	 spLogonUserInfoT theUser = pContext->GetUser();
	 if(theUser && 0 == theUser->sLogonEmail.CompareNoCase(email))
	 {
		 CControlUI *pCtl = GetPaintMgr()->FindControl(_T("login"));
		 CButtonUI  *pLoginBtn = static_cast<CButtonUI*>(pCtl);
		 assert(pLoginBtn);
		 if(pLoginBtn)
			CInfoWindow::ShowInfoWindow(this->m_hWnd,pLoginBtn->GetPos(),_T("用户已经登录!!!"));
		 return;
	 }

	 CControlUI *pEmailEdit = GetPaintMgr()->FindControl(_T("email"));
	 if(pEmailEdit)
	 {
		 pEmailEdit->SetText(email);
	 }

	 std::string sUtf8UserName = em_utility::mci::unicode_2_utf8(email);
	 std::string sUtf8Password = em_utility::mci::unicode_2_utf8(pwd);
	 std::string sUtf8PasswordMD5 = (isMD5 ? sUtf8Password : em_utility::md5_checksum::get_md5((const unsigned char*)sUtf8Password.c_str(),(unsigned int)sUtf8Password.length()));

	 m_sUserName    = email;
	 m_sPasswordMd5 = em_utility::mci::utf8_2_CString(sUtf8PasswordMD5.c_str());
	 //m_isKeepLogin  = isKeepLogin;
	 
	 EnableWindow(FALSE);
//	 theWebServiceMgr->AsynSendRequest(m_hWnd,url::Requester::MakeLogin2(sUtf8UserName,sUtf8PasswordMD5));
//  	 CurlData* pData = new CurlData;
//  	 pData->SetURL("http://localhost/YMusic/login.php");
// 	 pData->n_type = url::WSRT_LOGIN;
// 	std::string sparam;
// 	sparam = "email=";
// 	sparam += sUtf8UserName;
// 	sparam += "&";
// 	sparam += "password=";
// 	sparam += sUtf8PasswordMD5;
// 	pData->SetData(sparam.c_str());

//	HttpManagerProto::Instance()->DoHttpPost(pData);
// 	CHttpThread* pthread = new CHttpThread(pData,m_hWnd,WM_WEB_SERVICE);
// 	pthread->Start();

	 CHttpCenter::Instance()->MakeLogin(sUtf8UserName,sUtf8PasswordMD5);

 }
 void CLoginWindow::LoginWithToken(const CString& token)
 {
	 CString sToken = token;
	 assert(!token.IsEmpty());
	 sLog("loginWithToken:%s",sToken.GetBuffer());
	 sToken.ReleaseBuffer();
	 theWebServiceMgr->AsynSendRequest(m_hWnd,url::Requester::MakeLogin(token));
	
//	 EnableWindow(FALSE);
 }

 void CLoginWindow::OnSinaLogin()
 {
	 this->OpenWebWindow(m_sinURL);
 }

 void CLoginWindow::OnQQLogin()
 {
	 this->OpenWebWindow(m_qqURL);
 }

 void CLoginWindow::OnLogin()
 {
	 CString strUserName,strUserPassword;

	 CControlUI *pEmailEdit,*pPasswordEdit;
	 pEmailEdit    = GetPaintMgr()->FindControl(_T("email"));
	 pPasswordEdit = GetPaintMgr()->FindControl(_T("password"));

	 if(!pEmailEdit || !pPasswordEdit)
		 return;

	 strUserName     = pEmailEdit->GetText();
	 strUserPassword = pPasswordEdit->GetText();

	 if(strUserName.IsEmpty())
	 {
		 CInfoWindow::ShowInfoWindow(this->m_hWnd,pEmailEdit->GetPos(),_T("用户名不能为空!!!"));
		 pEmailEdit->SetFocus();
		 return;
	 }

	 if(strUserPassword.IsEmpty())
	 {
		 CInfoWindow::ShowInfoWindow(this->m_hWnd,pPasswordEdit->GetPos(),_T("密码不能为空!!!"));
		 pPasswordEdit->SetFocus();
		 return;
	 }

	 bool isChecked = false;
	 CControlUI *pCheckBox   = GetPaintMgr()->FindControl(_T("KeepLogin"));
	 CCheckBoxUI *pKeepLogin = static_cast<CCheckBoxUI*>(pCheckBox);

	 if(pCheckBox)
	 {
		 if (pKeepLogin)
		 {
			  isChecked = pKeepLogin->IsSelected();
		 }
	 }

	 strUserPassword = em_utility::mci::Htmlspecialchars(strUserPassword);
	 this->Login(strUserName,strUserPassword,false,isChecked);
 }

 void CLoginWindow::OnReg()
 {
	 BOOL bResult = CSys::OpenUrl(m_regURL.GetBuffer());
	 assert(bResult);
 }

 void CLoginWindow::OnForgetPassword()
 {
	 BOOL bResult = CSys::OpenUrl(m_forgetURL.GetBuffer());
	 assert(bResult);
 }

 BOOL CLoginWindow::Receive(SkinChangedParam param)
 {
	 try
	 {
		 if(!IsWindow()) return FALSE;
		 sLog("CLoginWindow::Receive");
		 CDuiString bgimage_ = param.bgimage;
		 DWORD bkcolor_ = param.bkcolor;
		 CControlUI* background = GetPaintMgr()->GetRoot();
		 if (background != NULL&&0!=bgimage_.Compare(background->GetBkImage()))
		 {
			 if (!param.bgimage.IsEmpty())
			 {
				 CDuiString sBkImage = bgimage_;

				 background->SetBkImage(sBkImage);
			 }
			 else
				 background->SetBkImage(_T(""));

			 background->SetBkColor(param.bkcolor);
		 }

		 return TRUE;
	 }
	 catch (...)
	 {
		 sLogError("CLoginWindow::Receive");
		 throw _T("CLoginWindow::Receive");
		 return FALSE;
	 }	
 }

 void CLoginWindow::OpenWebWindow(CString& sUrl)
 {
	 assert(!sUrl.IsEmpty());
	 if(sUrl.IsEmpty())
		 return;

	 CWebLoginWindow WebLoginWnd;
	 WebLoginWnd.SetLoginSucceedURL(m_webLoginSucceedURL);

	 if(!WebLoginWnd.IsWindow())
		 WebLoginWnd.Create(*this,NULL,0,0);

	 WebLoginWnd.CenterWindow();
	 WebLoginWnd.GoUrl(sUrl);

	 if(1 == WebLoginWnd.ShowModal())
		LoginWithToken(WebLoginWnd.GetToken());
 }
LRESULT CLoginWindow::OnWebService(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sLog("CLoginWindow::OnWebService");
	return __super::OnWebService(uMsg,wParam,lParam,bHandled);
}
/*
void CLoginWindow::OnWebService(void *pRequest ,void *pProto)
{
	url::CRequest *theRequest = (url::CRequest *)pRequest;
	LogonUserInfoT * theUser = static_cast<LogonUserInfoT*>(pProto);
	if(url::WSRT_LOGIN != theRequest->GetRequestType() &&
		url::WSRT_WEBLOGIN != theRequest->GetRequestType())
 		return;
	CControlUI *pCtl = GetPaintMgr()->FindControl(_T("login"));
	CButtonUI  *pLoginBtn = static_cast<CButtonUI*>(pCtl);
 	if(theRequest->IsTimeOut())
	{
		if(pLoginBtn)
			CInfoWindow::ShowInfoWindow(this->m_hWnd,pLoginBtn->GetPos(),_T("登录超时，请重试。"));
		EnableWindow(TRUE);
		return;
	}
	spLogonUserInfoT  spLogonUserInfo(new LogonUserInfoT(*theUser));
	
	if (!spLogonUserInfo)
	{	
		if(pLoginBtn)
			CInfoWindow::ShowInfoWindow(this->m_hWnd,pLoginBtn->GetPos(),_T("      登录失败，用户名或密码错误!      "));
		EnableWindow(TRUE);
		return ;
	}
 	{	
		if(url::WSRT_WEBLOGIN != theRequest->GetRequestType())
		{
			spLogonUserInfo->sLogonEmail = m_sUserName;
			spLogonUserInfo->sPasswordMD5 = m_sPasswordMd5;
			//spLogonUserInfo->bKeepLogged = !!m_isKeepLogin;
		}else 
		{
			assert(!spLogonUserInfo->sLogonEmail.IsEmpty());
			assert(!spLogonUserInfo->sPasswordMD5.IsEmpty());
			//spLogonUserInfo->bKeepLogged = true;
		}

		CControlUI *pCheckBox   = GetPaintMgr()->FindControl(_T("KeepLogin"));
		CCheckBoxUI *pKeepLogin = static_cast<CCheckBoxUI*>(pCheckBox);
		bool autoLogin = pKeepLogin->GetCheck();
// 		if (url::WSRT_WEBLOGIN == pRequest->GetRequestType())
// 			autoLogin = true;
	    theConfigMgr->GetNormalConfig()->EnableAutoLogin(autoLogin);
		theConfigMgr->GetNormalConfig()->Save();


		if(theDbMgr->GetGlobalDBase()->GetUserInfo(mci::unicode_2_utf8(spLogonUserInfo->sUserID).c_str()))
			theDbMgr->GetGlobalDBase()->UpdateUserInfo(spLogonUserInfo);
		else
			theDbMgr->GetGlobalDBase()->AddUserInfo(spLogonUserInfo);

		theRuntimeState->SetUserInfo(spLogonUserInfo);	

		RECT rtWin;
		::GetWindowRect(theMainWin->GetHWND(),&rtWin);
		POINT rtPos = {rtWin.left+25,rtWin.top+35};
		::ScreenToClient(theMainWin->GetHWND(),&rtPos);

		CInfoWindow::ShowInfoWindow(theMainWin->GetHWND(),rtPos,_T("                           登录成功                           "));

		Close(IDOK);
	}

//	CurlData* pdata = (CurlData*)pRequest;
//	int* nRet = (int*)pProto;
}
*/
void CLoginWindow::OnWebService(void *pRequest ,void *pProto)
{
	CControlUI *pCtl = GetPaintMgr()->FindControl(_T("login"));
	CButtonUI  *pLoginBtn = static_cast<CButtonUI*>(pCtl);
	CurlData* pdata = (CurlData*)pRequest;
	int* nRet = (int*)pProto;
	if (!pdata)
	{
		CInfoWindow::ShowInfoWindow(this->m_hWnd,pLoginBtn->GetPos(),_T("      登录失败，错误类型[未知]!      "));
		EnableWindow(TRUE);
		return;
	}
	if(url::WSRT_LOGIN != pdata->n_type &&
		url::WSRT_WEBLOGIN != pdata->n_type)
	{
		CInfoWindow::ShowInfoWindow(this->m_hWnd,pLoginBtn->GetPos(),_T("      登录失败，错误类型[非WSRT_LOGIN]!      "));
		EnableWindow(TRUE);
		return;
	}

	static CResponseParser2 Parser;
	std::string xmlbuf;
	xmlbuf.assign(pdata->buffer);
	//	assert(!xmlbuf.empty());
	std::string bufLog = mci::utf8_2_ansi(xmlbuf.c_str());
	sLogWarn(bufLog.c_str());
	//modify by dfn.li 2014-01-17
	if(bufLog[0] == '?')
	{
		bufLog = bufLog.substr(1);
	}
	xmlbuf = mci::ansi_2_utf8(bufLog.c_str());

	int t_begin = clock();
	void *pResponse = NULL;
	pResponse = Parser.Parse(pdata->n_type,xmlbuf);
	int t_time = clock() - t_begin;

	LogonUserInfoT * theUser = static_cast<LogonUserInfoT*>(pResponse);
	if (!theUser)
	{
		if(pLoginBtn)
			CInfoWindow::ShowInfoWindow(this->m_hWnd,pLoginBtn->GetPos(),_T("      登录失败，用户名或密码错误!      "));
		EnableWindow(TRUE);
		return ;
	}
// 	if(theRequest->IsTimeOut())
// 	{
// 		if(pLoginBtn)
// 			CInfoWindow::ShowInfoWindow(this->m_hWnd,pLoginBtn->GetPos(),_T("登录超时，请重试。"));
// 		EnableWindow(TRUE);
// 		return;
// 	}
	spLogonUserInfoT  spLogonUserInfo(new LogonUserInfoT(*theUser));

	if (!spLogonUserInfo)
	{	
		if(pLoginBtn)
			CInfoWindow::ShowInfoWindow(this->m_hWnd,pLoginBtn->GetPos(),_T("      登录失败，用户名或密码错误!      "));
		EnableWindow(TRUE);
		return ;
	}
	{	
		if(url::WSRT_WEBLOGIN != pdata->n_type)
		{
			spLogonUserInfo->sLogonEmail = m_sUserName;
			spLogonUserInfo->sPasswordMD5 = m_sPasswordMd5;
			//spLogonUserInfo->bKeepLogged = !!m_isKeepLogin;
		}else 
		{
			assert(!spLogonUserInfo->sLogonEmail.IsEmpty());
			assert(!spLogonUserInfo->sPasswordMD5.IsEmpty());
			//spLogonUserInfo->bKeepLogged = true;
		}

		CControlUI *pCheckBox   = GetPaintMgr()->FindControl(_T("KeepLogin"));
		CCheckBoxUI *pKeepLogin = static_cast<CCheckBoxUI*>(pCheckBox);
		bool autoLogin = pKeepLogin->GetCheck();
		// 		if (url::WSRT_WEBLOGIN == pRequest->GetRequestType())
		// 			autoLogin = true;
		theConfigMgr->GetNormalConfig()->EnableAutoLogin(autoLogin);
		theConfigMgr->GetNormalConfig()->Save();


		if(theDbMgr->GetGlobalDBase()->GetUserInfo(mci::unicode_2_utf8(spLogonUserInfo->sUserID).c_str()))
			theDbMgr->GetGlobalDBase()->UpdateUserInfo(spLogonUserInfo);
		else
			theDbMgr->GetGlobalDBase()->AddUserInfo(spLogonUserInfo);

		theRuntimeState->SetUserInfo(spLogonUserInfo);	

		RECT rtWin;
		::GetWindowRect(theMainWin->GetHWND(),&rtWin);
		POINT rtPos = {rtWin.left+25,rtWin.top+35};
		::ScreenToClient(theMainWin->GetHWND(),&rtPos);

		CInfoWindow::ShowInfoWindow(theMainWin->GetHWND(),rtPos,_T("                           登录成功                           "));
		EnableWindow(TRUE);
		Close(IDOK);
	}

}