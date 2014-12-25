#ifndef __MUSICLIB_H
#define __MUSICLIB_H

#pragma once
#include "../../YMusic.h"

/*
 *音乐库：网罗最新最好听的歌曲
 */
class CSimpleJsHandler;
class YMusicLibWnd 
	: public YWindow
	, public INotifyUI
	, public IDialogBuilderCallback
	, public SkinChangedReceiver
	, public CNotifyPump
	, public Singleton<YMusicLibWnd>
{
public:
	YMusicLibWnd();
	virtual ~YMusicLibWnd();
	static LPCTSTR GetWindowClass();
	LPCTSTR GetWindowClassName() const;
public:
	void InitJsHandler(CSimpleJsHandler* h);
public:
	virtual CControlUI*  CreateControl(LPCTSTR pstrClass);
	void         InitWindow();
	virtual void Notify(TNotifyUI& msg);
	
	void         TransToURL(int iIndex,LPCTSTR url);

	void		 Downloading(spSongInfoT spSong);
	void		 Downloaded(spSongInfoT spSong);
protected:
	virtual BOOL Receive(SkinChangedParam param);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnHttpfileDownload(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void         OnClick(TNotifyUI& msg);
	void         OnSelectChange(TNotifyUI& msg);
	void         OnItemSelect(TNotifyUI& msg);
	void         OnMenu(TNotifyUI& msg);

	void		 OnRefreshBrowser();
	void         OnGoBack();
	void         OnSearch();
	//收藏;
	void OnShowFavSongs();
	//历史
	void OnShowHisSongs();
	//下载
	void OnShowDownloading();
	void OnShowDownloaded();
protected:
	static void OnMsgBox(CSimpleJsHandler* h,LPCTSTR param,CString& sResult);
	static void OnPlay(CSimpleJsHandler* h,LPCTSTR param,CString& sResult);
	static void OnPlayMv(CSimpleJsHandler* h,LPCTSTR param,CString& sResult);
	static void OnShare(CSimpleJsHandler* h,LPCTSTR param,CString& sResult);
	static void OnAdd(CSimpleJsHandler* h,LPCTSTR param,CString& sResult);
	static void OnDownload(CSimpleJsHandler* h,LPCTSTR param,CString& sResult);
private:
	DECLARE_SINGLETON_CLASS(YMusicLibWnd);
private:
	CSimpleJsHandler*    _pUserJsHandler;
	CSimpleJsHandler*    _pXiaMiJsHandler;

	CWebBrowserUI*       _pCurBrowser;
	struct ui
	{
		CFadeButtonUI* m_pButtonBack;
		CFadeButtonUI* m_pButtonRefresh;

		COptionUI* m_pOptMyMusicLib;
		COptionUI* m_pOptFindMusic;

		CFadeButtonUI* m_pButtonClose;

		CTabLayoutUI* m_pTabSwitch;
		CTabLayoutUI* m_pDownloadTab;
		CListUI* m_pDownloadingList;
		CListUI* m_pDownloadedList;

		CTreeViewUI* m_pTree;
		CTreeNodeUI* m_pTreeIListenlist;
		CTreeNodeUI* m_pTreeFavNode;
		CTreeNodeUI* m_pTreeHistory;
		CTreeNodeUI* m_pTreeIDownload;
		CTreeNodeUI* m_pTreeDownloading;
		CTreeNodeUI* m_pTreeDownloaded;

		CWebBrowserUI* m_pWebUserIe;
		CWebBrowserUI* m_pWebXiamiIe;

		CEditUI* m_pEditContent;
		CFadeButtonUI* m_pButtonSearch;
	};
	struct ui  ui_;
	CDuiString bgimage_;
	DWORD bkcolor_;
public:
	ui getUI() const
	{
		return ui_;
	}
};






#endif//__MUSICLIB_H