#pragma once
#include "../../YMusic.h"

#include "../LrcMgr/lrcFileReader.h"
#include "LrcToolBar.h"
#include "LrcMask.h"


class CDesktopLrcWindow 
	: public YWindow 
	, public Singleton<CDesktopLrcWindow>
{

public:
	typedef smart_pointer::SmartPtr<CLrcFileReader>  spLrcReaderT;
	typedef smart_pointer::SmartPtr<CLrcToolBar>     spLrcToolBarT;
	typedef smart_pointer::SmartPtr<CLrcMask>        spLrcMaskT;
public:
	CDesktopLrcWindow();
	~CDesktopLrcWindow();

	static LPCTSTR GetWindowClsss() ;
	LPCTSTR GetWindowClassName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNCMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNCMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void LockLrc(bool bLock);
	bool IsLock()const;
	bool GetMouseLeaveTag();
	void SetBkTag(bool bDrawBK);//toolbar´°¿Úµ÷ÓÃ;
	spLrcToolBarT GetLrcToolBar() const;
	spLrcMaskT    GetLrcMask() const;
	void LrcChange();
	void SetStopText(CString strText){m_strOnStop = strText;}
private:
	void DrawSpecialText(HDC hdc,const CString& strText,LPRECT lpRect);

	void DrawDesktopUI();
	void OnLrcChanged(/*CNotifyCenter::NotifyT& notify*/);

private:
	CString  m_strOnStop;
	spLrcMaskT    m_spLrcMask;
	spLrcToolBarT m_spLrcToolBar;
	spLrcReaderT  m_splrcFileReader;
	bool m_bLock;
	bool m_bDrawBk;
	bool m_bMouseOn;
	bool m_bTrace;
	int m_nTimerId;

};
