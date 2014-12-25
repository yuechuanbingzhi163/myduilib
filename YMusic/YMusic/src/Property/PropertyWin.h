#ifndef __PROPERTYWIN_H
#define __PROPERTYWIN_H

#pragma once

#include "../../YMusic.h"

class PropertyWin 
	: public YWindow
	, public INotifyUI
{
public:
	PropertyWin();
	virtual ~PropertyWin();
	static LPCTSTR GetWindowClsss() ;
	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage( HWND hWnd );

	void InitWnd(spSongInfoT spSong);
protected:
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
protected:
	virtual void OnClick(TNotifyUI& msg);
private:
	CLabelUI* m_pFileName;
	CLabelUI* m_pSongName;
	CLabelUI* m_pArtistName;
	CLabelUI* m_pAlbumName;
	CControlUI* m_pFilePos;
};






#endif//__PROPERTYWIN_H