#ifndef __SkinWindow_h
#define __SkinWindow_h

#pragma once
#include "../../YMusic.h"

class CSkinJsHandler;

class YSkinWindow
	: public YWindow
	, public INotifyUI
	, public IDialogBuilderCallback
	, public SkinChangedReceiver
	, public Singleton<YSkinWindow>
{
public:
	YSkinWindow();
	virtual ~YSkinWindow();
public:
	static LPCTSTR GetWindowClsss() ;
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage( HWND hWnd );
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void Notify(TNotifyUI& msg);
	void OnClick(TNotifyUI& msg);

	void LoadSysSkin();
	void LoadMySkin();
public:
	void InitJsHandler(CSkinJsHandler* h);
protected:
	virtual BOOL Receive(SkinChangedParam param);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static void OnUseSkin(CSkinJsHandler* h,LPCTSTR param,CString& sResult);
	virtual LRESULT OnHttpfileDownload(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	DECLARE_SINGLETON_CLASS(YSkinWindow);

private:
	CListUI* _pSysSkinLayout;
	CListUI* _pMySkinLayout;
	CAnimationTabLayoutUI* _pSkinLayout;

	CWebBrowserUI* _pWebSkin;
	CSkinJsHandler* _pSkinJsHandler;
};


#endif//__SkinWindow_h