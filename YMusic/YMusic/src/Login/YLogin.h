#ifndef __YLogin_h
#define __YLogin_h

#pragma once
#include "../../YMusic.h"

class YLoginWindow
	: public YWindow
	, public INotifyUI
	, public IDialogBuilderCallback
	, public Singleton<YLoginWindow>
{
public:
	virtual ~YLoginWindow(){};
	template<typename T>
	void FindControl(T& pctrl,LPCTSTR pstrName) const;
	static YLoginWindow* Instance(HWND hWndParent);
public:
	static LPCTSTR GetWindowClsss() ;
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage( HWND hWnd );
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void Notify(TNotifyUI& msg);
	void OnClick(TNotifyUI& msg);
protected:
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	DECLARE_SINGLETON_CLASS(YLoginWindow);
	static YLoginWindow* m_hSingleton;
};


#endif//__YLogin_h