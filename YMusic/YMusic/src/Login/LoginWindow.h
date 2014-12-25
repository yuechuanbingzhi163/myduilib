#pragma once
#include "../../YMusic.h"

class CLoginWindow :
	public YWindow,
	public INotifyUI,
	public SkinChangedReceiver,
	public Singleton<CLoginWindow>
{
public:
	CLoginWindow(void);
	~CLoginWindow(void);
	static LPCTSTR GetWindowClsss() ;
	LPCTSTR GetWindowClassName() const;

	virtual void Notify(TNotifyUI& msg); 
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//	virtual void OnWebService(url::CRequest *pRequest ,proto::base *pProto);
	virtual LRESULT OnWebService(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void OnWebService(void *pRequest ,void *pProto);

	void Login(const CString& email,const CString& pwd,bool isMD5 = false,bool isKeepLogin = true);
	void LoginWithToken(const CString& token);
	
protected:
	void OnSinaLogin();
	void OnQQLogin();
	void OnLogin();
	void OnReg();
	void OnForgetPassword();
	
	void OpenWebWindow(CString& sUrl);
	virtual BOOL Receive(SkinChangedParam param);
private:
	CString m_sUserName;
	CString m_sPasswordMd5;
	//BOOL    m_isKeepLogin;

	CString m_forgetURL;
	CString m_regURL;
	CString m_sinURL;
	CString m_qqURL;
	CString m_webLoginSucceedURL;
	
private:
	DECLARE_SINGLETON_CLASS(CLoginWindow);
};
