#ifndef __YPlayListItem_h
#define __YPlayListItem_h

#pragma once
#include "../../YMusic.h"

class YPlayListItemUI : public CListContainerElementUI
{
public:
	YPlayListItemUI();
	virtual ~YPlayListItemUI();
public:
	static LPCTSTR GetClassName();

	LPCTSTR GetClass() const;

	void SetPlayList(spPlayListInfoT spList);
	spPlayListInfoT GetPlayList() const;
	void ReName(bool bReName = true);

	void Update();

	void DoPaint(HDC hDC, const RECT& rcPaint);

	void ChangeIcon(int bPlaying = em_play);
protected:
	void InitVar();
private:
	spPlayListInfoT _spList;
	CLabelUI* _spListName;
	CEditUI*  _spRenameEdit;
	CControlUI* _spPlayIco;
	bool _bReName;
};

#define  PLAY_LIST  _T("PlayList")
typedef CListUI CPlayListUI;

#endif//__YPlayListItem_h