#ifndef __LrcSelectWnd_h
#define __LrcSelectWnd_h

#pragma once
#include "../../YMusic.h"


class CLrcSelectWnd:
	public YWindow,
	public INotifyUI,
	public SkinChangedReceiver,
	public Singleton<CLrcSelectWnd>
{
public:
	CLrcSelectWnd();
	~CLrcSelectWnd();
	static LPCTSTR GetWindowClass();
	LPCTSTR GetWindowClassName() const;

	virtual void Notify(TNotifyUI& msg); 
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
;
	void AddItem(int nIndex,CString sArtist,CString sSongName);
	void ClearItem();
	void InitSearchEdit(spSongInfoT spSong);
	CString GetInputSongName();
	CString GetInputArtistName();
	void  ReleatLocalLrc();
protected:
	virtual BOOL Receive(SkinChangedParam param);
private:
	void OnOK();
	void OnClose();
	void OnSearchLrc();
	void OnReleatLocalLrc();
private:
	int m_nTimerID;
	int m_nSeconds;
	CString m_InPutArtist;
	CString m_InPutSong;
	spSongInfoT m_spSongForSearch;
	CSearchLrcFromWeb m_Search;
private:
	DECLARE_SINGLETON_CLASS(CLrcSelectWnd);
};

#endif//__LrcSelectWnd_h