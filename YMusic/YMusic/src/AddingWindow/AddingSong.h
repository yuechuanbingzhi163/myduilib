#ifndef __ADDINGSONG_H
#define __ADDINGSONG_H
#pragma once
#include "../../YMusic.h"

class YAddingSong
	:public YWindow
	,public INotifyUI
{
public:
	YAddingSong(bool bOpenFile,CListUI* pSongList);
	virtual ~YAddingSong();
	template<typename T>
	void FindControl(T& pctrl,LPCTSTR pstrName) const;
	static LPCTSTR GetWindowClsss() ;
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage( HWND hWnd );
public:
	void Notify(TNotifyUI& msg);
	void OnClick(TNotifyUI& msg);
	void OnInit(TNotifyUI& msg);
	int ShowDetail(CFileMgr::filesArrayT files);
public:
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	CListUI* _pSongList;
	CTextUI* _pText;
	bool _bCancel;
	bool _bOpenFile;
	CFileMgr::filesArrayT _files;
};







#endif//__ADDINGSONG_H