#ifndef __MainMenu_h
#define __MainMenu_h

#pragma once
#include "../Utilty/Singleton.h"

class YMainMenuWindow
	: public YWindow
	, public INotifyUI
	, public IDialogBuilderCallback
	, public Singleton<YMainMenuWindow>
{
public:
	YMainMenuWindow();
	virtual ~YMainMenuWindow(){};
	template<typename T>
	void FindControl(T& pctrl,LPCTSTR pstrName) const;
public:
	static LPCTSTR GetWindowClsss() ;
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage( HWND hWnd );
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void Notify(TNotifyUI& msg);
protected:
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	DECLARE_SINGLETON_CLASS(YMainMenuWindow);
};




#endif//__MainMenu_h