#ifndef _DOWNLOADITEM_H
#define _DOWNLOADITEM_H

#pragma once
#include "../../../YMusic.h"
#include "../DownHelper/ThreadHelper.h"
#include "../HTTPprotocol/HTTPprotocol.h"

using namespace em_utility;

class DownloadListItemUI 
	: public CListContainerElementUI
	, public ThreadEvent
{
public:
	DownloadListItemUI();
	virtual ~DownloadListItemUI();
public:
	static LPCTSTR GetClassName();
	LPCTSTR GetClass() const;

	void SetSong(spSongInfoT song);
	spSongInfoT GetSong() const;

	void InitVar();
	void Update();
	void ResumeLoad();
	void PauseLoad();
protected:
	void OnPrepare();
	void ShowOrHideBtn(bool bShow = true);
	bool OnClick(void* event);
protected:
	static unsigned __stdcall ThreadProc(void *pvArg);
	virtual void _ThreadProc_();
	virtual bool ProcessEvent() ;
	virtual bool RespondEvent() ;
	virtual void Cleanup() ;
	//进度条回调
	static int LoadProgress(void* ptr, double rDlTotal, double rDlNow, double rUlTotal, double rUlNow);

	static CDuiString getSizeString(double dload,double size);
	static CDuiString getRateString(double rate);
private:
	spSongInfoT _spSongInfo;
	bool _bIsDowning;


	CProgressUI* _pProgress;
	CLabelUI*  _pSizeLab;
	CLabelUI*  _pRateLab;
	CLabelUI*  _pTitleLab;

	CButtonUI* _pStart;
	CButtonUI* _pPause;
	CButtonUI* _pRemove;

private:
	void*  m_thread;
	CHTTPprotocol* m_pHttp;
	double m_downloadOld;
	static const int DownloadItemHeight;
};
///////////////////////////////////////////////////
class DownloadFinishedListItemUI
	: public CListContainerElementUI
{
public:
	DownloadFinishedListItemUI();
	virtual ~DownloadFinishedListItemUI();
public:
	static LPCTSTR GetClassName();
	LPCTSTR GetClass() const;

	void SetSong(spSongInfoT song);
	spSongInfoT GetSong() const;
protected:
	void Update();
	void OnPrepare();
	bool Notify(void* param);
private:
	spSongInfoT _spSongInfo;
	CCheckBoxUI * _pCheckBox;
	CLabelUI * _pTitlelab;
	CLabelUI * _pSizelab;
	CButtonUI * _pPlaybtn;
	CButtonUI * _pAddbtn;
	CButtonUI * _pRemovebtn;

	static const int DownloadFinishedItemHeight;
};







#endif//_DOWNLOADITEM_H