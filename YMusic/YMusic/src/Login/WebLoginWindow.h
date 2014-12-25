#pragma once
#include "../../YMusic.h"

class CWebLoginWindow : 
	public YWindow,
	public CWebBrowserEventHandler,
	public INotifyUI,
	public SkinChangedReceiver
{
public:
	CWebLoginWindow();
	~CWebLoginWindow();
	static LPCTSTR GetWindowClsss() ;
	LPCTSTR GetWindowClassName() const;

	void GoUrl(CString& sUrl);
	void SetLoginSucceedURL(const CString& str){_loginSucceedURL = str;}
	CString GetToken();
	virtual void Notify(TNotifyUI& msg); 
protected:
	virtual BOOL Receive(SkinChangedParam param);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void CheckLoginSucceed(const CString& strUrl);
	virtual void NavigateComplete2(IDispatch *pDisp,VARIANT *&url);

	CString        _loginSucceedURL;
	CString        _sToken;
};
