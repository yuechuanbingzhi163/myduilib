#ifndef __YSongListItem_h
#define __YSongListItem_h
#pragma once
#include "../../YMusic.h"
class YSongListItemUI : public CListContainerElementUI
{
public:
	YSongListItemUI();
	virtual ~YSongListItemUI();
public:
	static LPCTSTR GetClassName();
	LPCTSTR GetClass() const;

	void SetSong(spSongInfoT song);
	spSongInfoT GetSong() const;

	void Update();
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void DoEvent(TEventUI& event);
	bool DoNotify(void* param);

	void ChangeIcon(int bPlaying = em_play);
protected:
	void InitVar();

private:
	spSongInfoT _spSongInfo;
	CLabelUI* _pNorLabelIndex;
	CLabelUI* _pNorLabelName;
	CLabelUI* _pNorLabelAritst;
	CButtonUI* _pNorButtonMenu;
	CHorizontalLayoutUI* _pNormalLayout;

	CControlUI* _pItemIco;
	CLabelUI* _pPushLabelIndex;
	CLabelUI* _pPushLabelName;
	CLabelUI* _pPushLabelAritst;
	CLabelUI* _pPushLabelAlbum;
	CButtonUI* _pPushDownLoad;
	CCheckBoxUI* _pPushCollect;
	CButtonUI* _pPushShare;
	CButtonUI* _pPushRemove;
	CHorizontalLayoutUI* _pPushLayout;
};

#define SONG_LIST _T("SongList")
typedef CListUI  CSongListUI;

#endif//__YSongListItem_h