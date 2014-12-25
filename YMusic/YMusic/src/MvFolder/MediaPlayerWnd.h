#ifndef __MediaPlayerWnd_H
#define __MediaPlayerWnd_H

#pragma once
#include "../../YMusic.h"
#include "UIMedia.h"

class CMediaWin :
	public YWindow,
	public INotifyUI,
	public SkinChangedReceiver,
	public IDialogBuilderCallback,
	public IMessageFilterUI,
	public Singleton<CMediaWin>
{
public:

	CMediaWin();
	~CMediaWin();
	static LPCTSTR GetWindowClass();
	LPCTSTR GetWindowClassName() const;
	static CMediaWin* GetMediaPlayer() ;

	void ReadyToPlay(LPCTSTR pstrName);

	bool Play();
	void Pause();
	void Resume();
	void Stop();
	void FullScreen(bool bFull = true);
	void SetVolume(int nVal);
	void SwitchVol(bool bOn = true);
	void SetTimeText(LPCTSTR szText);
	void SetProgress(int nVal,bool bShowLayout = true);
	void ChangeBtnStatus(bool bPlaying = true);
protected:
	virtual void OnFinalMessage( HWND hWnd );
	virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void Notify(TNotifyUI& msg); 
	
	void OnClick(const TNotifyUI& msg);

	bool    OnPosChanged(void* param);                              // 进度改变，用户主动改变进度
	bool    OnVolumeChanged(void* param);                           // 音量改变

protected:
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual BOOL Receive(SkinChangedParam param);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnWebService(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void    OnWebService(void *pRequest ,void *pProto);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	LRESULT OnPlayerPlaying(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPlayerPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPlayerEndReached(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
protected:
	void UpdatePlayingProgress();

private:
	struct ui{
	CLabelUI* m_pTitle;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pResBtn;
	CMediaUI* m_pMediaPlayer;
	CHorizontalLayoutUI* m_pSliLayout;
	CSliderUI* m_pProgressSli;
	CButtonUI* m_pBackWard;
	CButtonUI* m_pForWard;
	CLabelUI* m_pTimeUse;
//	CLabelUI* m_pTimeAll;
	CButtonUI* m_pPlayBtn;
	CButtonUI* m_pPauseBtn;
	CButtonUI* m_pPrevBtn;
	CButtonUI* m_pNextBtn;
	CButtonUI* m_pStopBtn;
	CButtonUI* m_pVolumeOffBtn;
	CButtonUI* m_pVolumeOnBtn;
	CSliderUI* m_pVolumeSli;
	CButtonUI* m_pFullScreenBtn;
	CButtonUI* m_pNormalBtn;
	};
	ui _ui;

	int m_nVol;
	bool m_bVolOn;
	std::string m_strCurPlay;
	bool m_bFullScreenMode;
	WINDOWPLACEMENT m_OldWndPlacement;  // 保存窗口原来的位置
private:
	DECLARE_SINGLETON_CLASS(CMediaWin);
};


#endif//__MediaPlayerWnd_H