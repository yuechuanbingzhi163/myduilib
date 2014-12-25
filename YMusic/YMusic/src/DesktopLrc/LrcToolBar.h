#pragma once
#include "../../YMusic.h"

class CDesktopLrcWindow;

class CLrcToolBar :
	public YWindow,
	public INotifyUI,
	public SkinChangedReceiver
{
public:
	CLrcToolBar(CDesktopLrcWindow* pDeskLrc);
	~CLrcToolBar();
	static LPCTSTR GetWindowClsss() ;
	LPCTSTR GetWindowClassName() const;

	virtual void Notify(TNotifyUI& msg); 
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	bool GetMouseLeaveTag();
	void SetStatePlay(bool bPlay = true);
private:
	void Init();
	void PlayOrPause();
	void OnSongStatusChanged(/*CNotifyCenter::NotifyT& notify*/);
	virtual BOOL Receive(SkinChangedParam param);
private:
	bool m_bMouseOn;
	bool m_bTrace;
	CDesktopLrcWindow * m_pDeskLrc;
	typedef struct
	{
		CFadeButtonUI*       _pPreSongBtn;
		CFadeButtonUI*       _pPlayBtn;
		CFadeButtonUI*       _pPauseBtn;
		CFadeButtonUI*       _pNextSongBtn;

		CFadeButtonUI*		 _pZoomInFont;
		CFadeButtonUI*		 _pZoomOutFont;
		CFadeCheckBoxUI*     _pKaraok;
		CFadeButtonUI*		 _pThemeBtn;
		CFadeButtonUI*		 _pSetting;

		CFadeButtonUI*	     _pSearchBtn;

		CFadeButtonUI*       _pLockBtn;
		CFadeButtonUI*       _pPanelLrcBtn;
		CFadeButtonUI*       _pCloseSysBtn;
	}ui;
	ui _ui;
};
