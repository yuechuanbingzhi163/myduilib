#ifndef __TAGMODIFYWIN_H
#define __TAGMODIFYWIN_H

#pragma once

#include "../../YMusic.h"

class TagModifyWin 
	: public YWindow
	, public INotifyUI
	, public SkinChangedReceiver
	, public Singleton<TagModifyWin>
{
public:
	TagModifyWin();
	virtual ~TagModifyWin();
	static LPCTSTR GetWindowClass() ;
	LPCTSTR GetWindowClassName() const;

	bool   InitWnd(spSongInfoT spSong,int nListId);
protected:
	virtual void Notify(TNotifyUI& msg); 
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual BOOL Receive(SkinChangedParam param);
protected:
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnModify();
private:
	DECLARE_SINGLETON_CLASS(TagModifyWin)
private:
	CEditUI * m_pSongName;
	CEditUI * m_pArtistName;
	CEditUI * m_pAlbumName;
	spSongInfoT m_pSong;
	int       m_nListId;
};



#endif//__TAGMODIFYWIN_H