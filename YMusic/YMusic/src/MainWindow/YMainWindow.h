#ifndef __YMainWindow_h
#define __YMainWindow_h

#pragma once
#include "../../YMusic.h"
#include "../LrcView/LrcView.h"
#include "../MusicLib/MusicLib.h"
#include "../DesktopLrc/DesktopLrcWindow.h"



class YMainWindow
	: public YWindow
	, public INotifyUI
	, public IDialogBuilderCallback
{
public:
	YMainWindow();
	virtual ~YMainWindow();

public:
	static LPCTSTR GetWindowClsss() ;
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	void OnFinalMessage( HWND hWnd );
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void Notify(TNotifyUI& msg);
	void OnValueChanged(const TNotifyUI& msg);
	void OnItemActivate(const TNotifyUI& msg);
	void OnItemSelect(const TNotifyUI& msg);
	void OnSelectChanged(const TNotifyUI& msg);
	void OnClick(const TNotifyUI& msg);
	void OnMenu(const TNotifyUI& msg);
	void OnBlur(const TNotifyUI& msg);
	void OnReturn(const TNotifyUI& msg);
	void OnTimer(const TNotifyUI& msg);
	void OnPlayListMenu(const TNotifyUI& msg);
	void OnSongListMenu(const TNotifyUI& msg);
	void OnSongItemMenu(const TNotifyUI& msg);
	bool OnVolumeLayoutUIEvent(void* pParam);
	void OnShowPlayModeMenu();
	void OnShowMainMenu();
	void OnShowMiniMode();
	void OnShowSkinWindow();
	void OnShowLoginWindow();
	void OnShowShareWindow();
	void OnGoNextLayout(const TNotifyUI& msg);
	void OnGoBackLayout(const TNotifyUI& msg);
	void OnGoDeskLayout(const TNotifyUI& msg);
	void OnShowLrcViewMenu(const TNotifyUI& msg);
	void SendNotify(LPCTSTR lpControl,LPCTSTR sType);
public:
	void ShowTrayInfo(spSongInfoT spSong);
	void ShowTrayTooltip(TCHAR* szTip,TCHAR* szTitle);
	void ShowTrayMenu();
	void TryAutoLogin();
	void AddTracyIcon();
	void PlayOrPause(bool bPlay = true);
	void NextSong();
	void PrevSong();
	void Stop();
	void Play();
	void Pause();
	
	static void CallbackPlaying(void *data);
	static void CallbackPosChanged(void *data);
	static void CallbackEndReached(void *data);

	int ShowDetailAddingInfo(CFileMgr::filesArrayT files,void* param);
protected:
	void InitVariable();
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMenuCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTaskbarCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTaskbarButtonCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
	virtual	LRESULT OnTrayNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnWebService(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnPlayerPlaying(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnPlayerPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnPlayerEndReached(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnUserLogin(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnHttpfileDownload(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	static void CallbackPlayer(void *data, UINT uMsg);
/*protected:*/
public:
	void OnPlayListChanged(/*CNotifyCenter::NotifyT& notify*/);
	void OnCurPlayListChanged(/*CNotifyCenter::NotifyT& notify*/);
	void OnPlayListSongChanged(/*CNotifyCenter::NotifyT& notify*/);
	void OnNetChanged(/*CNotifyCenter::NotifyT& notify*/);
	void OnUserChanged(/*CNotifyCenter::NotifyT& notify*/);
	void OnVersionExpired(/*CNotifyCenter::NotifyT& notify*/);		
	void OnSkinChanged(/*CNotifyCenter::NotifyT& notify*/);
	void OnSongStatusChanged(bool bPlay = true);

	void UpdateDowningProgress();
	void UpdatePlayingProgress();
	void UpdateBufferingProgress();
	void ShowSongInfo(spSongInfoT song);
	void UpdateUserName();
	void UpdatePlayList();
	void UpdateSongList(int nPlayListId);
	void UpdateFavList();//收藏列表
	void CreateAddSongThread(bool bOpenFile);
	static DWORD WINAPI _ThreadProc_AddSong(void* param);
	void _AddSongProc(void* param);
	void _AddSongProc2(unsigned int u_listId,CFileMgr::filesArrayT files);

	int OnAddPlayList();//添加列表
	void OnOpenProperty(spSongInfoT spSong);//查看属性
	void OnOpenFolderPath(LPCTSTR sPath);//打开所在目录
	void ShowInfo(CString sInfo);//操作提示
	int OnMenuCommandPlayList(MenuCmdMsg* pMsg);
	int OnMenuCommandSongList(MenuCmdMsg* pMsg);
	int OnMenuCommadnPlayMode(MenuCmdMsg* pMsg);
	int OnMenuCommandSongItem(MenuCmdMsg* pMsg);
	int OnMenuCommandLrcView(MenuCmdMsg* pMsg);
	int OnMenuCommandMainMenu(MenuCmdMsg* pMsg);
	void OnRemoveSongFromList(unsigned int nPlayListID,unsigned int nSongIndex);
	void OnRemoveSongsFromList(unsigned int nPlayListID);
	void OnCollectSongFromList(unsigned int nPlayListID,unsigned int nSongIndex);
	void OnCollectSongsFromList(unsigned int nPlayListID);
	void OnShareSongFromList(unsigned int nPlayListID,unsigned int nSongIndex);
	void OnShareSongsFromList(unsigned int nPlayListID);
	void OnDownLoadSongFromList(unsigned int nPlayListID,unsigned int nSongIndex);
	void OnDownLoadSongsFromList(unsigned int nPlayListID);
	void OnAddSongtoNewList(unsigned int nSongIndex,unsigned int nPlayListID,unsigned int nNewListID,bool bMove = false);
	void OnAddSongstoNewList(unsigned int nPlayListID,unsigned int nNewListID,bool bMove = false);
	void OnRemoveDumplicate(unsigned int nPlayListID); // 去重
public:
	typedef CComPtr<ITaskbarList4>        spTaskbarListT;
public:
	spTaskbarListT GetTaskbarList() const;
	YMusicLibWnd* GetMusicLibWin() const;	
	CDesktopLrcWindow*  GetDesktopLrcWin()const;
	YMusicLibWnd* GetMusicLibWin2() const;	
	CDesktopLrcWindow*  GetDesktopLrcWin2()const;

	void ShowMusicLibWin(BOOL bShow = TRUE);
	void ShowDesktopLrcWin(BOOL bShow = TRUE);
	void ShowPanelLrcTab(BOOL bShow = TRUE);
	void ShowSelectLrcWin();
	void ShowSettingWin();
	void ShowSkinWin();
	void LoadSongLrc();
	void SetBkColor(DWORD dwCor);
	void SetBkImage(LPCTSTR szImage);
	void AddReceive(SkinChangedReceiver* win);
	void BroadCast();
private:
	DECLARE_SINGLETON_CLASS(YMainWindow);
private:
	typedef struct uiCompent
	{
		CListUI* _pPlayList;
		CListUI* _pSongList;
		CAnimationTabLayoutUI *_pTabLayout;
		CFadeCheckBoxUI* _pVolumeBtn;
		CSliderUI* _pVolumeSlider;
		CTabLayoutUI* _pVolumeTab;
		CLabelUI* _pTimeUsed;
		CLabelUI* _pTimeTotal;
		CSliderUI* _pPlayProgress;
		CLabelUI* _pSongName;
		CLabelUI* _pAlbumName;
		CFadeCheckBoxUI* _pCollect;
		CFadeButtonUI* _pShare;
		CHorizontalLayoutUI* _pInfoLayout;
		CLabelUI* _pInfoLabel;
		CLabelUI* _pCountLabel;
		CAnimationTabLayoutUI *_pLrcViewTab;
		CLrcViewUI* _pLrcView1;
	}_UICompent;
	uiCompent  _ui;
	bool _bOpenFile;

	HICON                _hOnlineIcon;
	HICON                _hOfflineIcon;
	CTrayIconController  _trayIcon; 
	spTaskbarListT       _pTaskbarList;

	YMusicLibWnd*          m_pMusicLibWnd;	
	CDesktopLrcWindow*     m_pDesktopLrcWnd;

	UINT _uTaskbarButtonCreateMsg;
	UINT _uTaskbarCreatedMsg;

	SkinChangedObserver skin_changed_observer_;
};





#endif//__YMainWindow_h