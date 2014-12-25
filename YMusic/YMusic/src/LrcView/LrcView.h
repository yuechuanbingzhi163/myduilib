
/***********************************************************
 *	FileName£ºLrcView.h
 *  Author  £ºDFn Li
 *  Timer   : 2013/3/20 18:10
 *  E-Mail  : libyyu@qq.com
 *  Comment : show lrc to program panel
 *  Version : 1.0.0.1
 ***********************************************************/
#ifndef LRCVIEW_H
#define LRCVIEW_H
#pragma once
#include "../../YMusic.h"
#include "../LrcMgr/lrcFileReader.h"
#include "../LrcSelectWnd/LrcSelectWnd.h"


const static int DELAY_TIME = 500;
const static int TOTAL_ALPHA_VALUE = 220;

class CLrcViewUI : public CControlUI
{

public:
	typedef smart_pointer::SmartPtr<CLrcFileReader>  spLrcReaderT;
public:
	CLrcViewUI();
	virtual ~CLrcViewUI();

	static LPCTSTR GetClassName();
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	CLrcSelectWnd* GetLrcSelectWnd();

	void LrcChanged(spSongInfoT song);
	spSongInfoT GetSong(){return m_pSelectLrcSong;}
protected:
	void DoInit();
	void DoEvent(TEventUI& event);
	void PaintText(HDC hDC);

private:
	spLrcReaderT    m_spLrcReader;
	CLrcSelectWnd*  m_LrcSelectWnd;

	spSongInfoT     m_pSelectLrcSong;
	int            m_nAlphaPercent;

	bool           m_bIsDrag;
	CString        m_sOnStopShow;
	
};


#endif//LRCVIEW_H