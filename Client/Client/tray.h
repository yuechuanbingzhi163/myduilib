#ifndef _TRAY_H
#define _TRAY_H

#pragma once

#include "UIUserList.h"
class CTray 
	: public CWin
	, public INotifyUI
	, public IDialogBuilderCallback
{
private:
	CTray();
public:
	static const LPCTSTR CLASSNAME;
	
	~CTray();
	LPCTSTR GetWindowClassName() const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void Notify(TNotifyUI& msg);
	void TypeInit(TNotifyUI& msg);
	void TypeClick(TNotifyUI& msg);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled);
	static CTray* GetInstance(HWND hWnd);
	void UpdateMsgList();
private:
	static CTray* m_hInstance;
	CListUI* m_pMsgList;
};
	


#endif//_TRAY_H