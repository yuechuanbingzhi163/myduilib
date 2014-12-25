#ifndef __SearchLrcWindow_h
#define __SearchLrcWindow_h

#pragma once
#include "../Utilty/Singleton.h"
class YSearchLrcWindow
	: public YWindow
	, public INotifyUI
	, public IDialogBuilderCallback
	, public Singleton<YSearchLrcWindow>
{
public:
	YSearchLrcWindow();
	virtual ~YSearchLrcWindow(){};
	static YSearchLrcWindow* Instance(HWND hWndParent);
public:
	static LPCTSTR GetWindowClsss() ;
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage( HWND hWnd );
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void Notify(TNotifyUI& msg);
protected:
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	DECLARE_SINGLETON_CLASS(YSearchLrcWindow);
	static YSearchLrcWindow* m_Singleton;
};



#endif//__SearchLrcWindow_h