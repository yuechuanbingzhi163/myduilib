#include "StdAfx.h"
#include "DownloadItem.h"

#include "../../YApp.h"

namespace DownloadItemName
{
	static const TCHAR kProgct[] = _T("downing_progctl");
	static const TCHAR kSizelab[] = _T("downing_sizelab");
	static const TCHAR kRatelab[] = _T("downing_ratelab");
	static const TCHAR kTitlelab[] = _T("downing_titlelab");

	static const TCHAR kStart[] = _T("downing_downstartbtn");
	static const TCHAR kPause[] = _T("downing_downpausebtn");
	static const TCHAR kRemove[] = _T("downing_downdeletbtn");
};


const int DownloadListItemUI::DownloadItemHeight = 40;
DownloadListItemUI::DownloadListItemUI()
	: _pProgress(NULL),_pSizeLab(NULL),_pRateLab(NULL),_pTitleLab(NULL)
	, _pStart(NULL),_pPause(NULL),_pRemove(NULL)
{
	this->SetFixedHeight(DownloadItemHeight);
	CDialogBuilder builder;
	builder.Create(_T("downloadunit.xml"),0,NULL,NULL,this);

	OnPrepare();

	InitVar();
}

DownloadListItemUI::~DownloadListItemUI()
{
	Cleanup();
}

LPCTSTR DownloadListItemUI::GetClassName()
{
	return _T("DownloadListItemUI");
}

LPCTSTR DownloadListItemUI::GetClass() const
{
	return DownloadListItemUI::GetClassName();
}

void DownloadListItemUI::OnPrepare()
{
	_pProgress = static_cast<CProgressUI*>(FindSubControl(DownloadItemName::kProgct));
	ASSERT(_pProgress);
	_pSizeLab = static_cast<CLabelUI*>(FindSubControl(DownloadItemName::kSizelab));
	ASSERT(_pSizeLab);
	_pRateLab = static_cast<CLabelUI*>(FindSubControl(DownloadItemName::kRatelab));
	ASSERT(_pRateLab);
	_pTitleLab = static_cast<CLabelUI*>(FindSubControl(DownloadItemName::kTitlelab));
	ASSERT(_pTitleLab);
	_pStart = static_cast<CButtonUI*>(FindSubControl(DownloadItemName::kStart));
	ASSERT(_pStart);
	_pPause = static_cast<CButtonUI*>(FindSubControl(DownloadItemName::kPause));
	ASSERT(_pPause);
	_pRemove = static_cast<CButtonUI*>(FindSubControl(DownloadItemName::kRemove));
	ASSERT(_pRemove);
	_pPause->OnNotify += MakeDelegate(this,&DownloadListItemUI::OnClick);
	_pStart->OnNotify += MakeDelegate(this,&DownloadListItemUI::OnClick);
}
bool DownloadListItemUI::OnClick(void* event)
{
	TNotifyUI* pMsg = (TNotifyUI*)event;
	if (pMsg&&pMsg->sType.Compare(kClick)==0)
	{
		if (pMsg->pSender == _pPause)
		{
			if(m_pHttp)
				m_pHttp->Pause();
			ShowOrHideBtn(true);
		}
		else if (pMsg->pSender == _pStart)
		{
			if (m_pHttp)
				m_pHttp->Resume();
			ShowOrHideBtn(false);
		}
	}
	return true;
}
void DownloadListItemUI::InitVar()
{
	_bIsDowning = false;
	m_thread = NULL;
	m_pHttp = NULL;
}

void DownloadListItemUI::SetSong(spSongInfoT song)
{
	ASSERT(song);

	_spSongInfo = song;

	Update();
}

spSongInfoT DownloadListItemUI::GetSong() const
{
	return _spSongInfo;
}

void DownloadListItemUI::Update()
{
	if (_spSongInfo)
	{
		CDuiString sTitle;
		sTitle.Format(_T("%s - %s"),_spSongInfo->GetSongName(),_spSongInfo->GetArtistName());
		_pTitleLab->SetText(sTitle);

		CDuiString sSize;
		sSize.Format(_T("%d"),_spSongInfo->GetFileSize());
		_pSizeLab->SetText(sSize);
	}
}

void DownloadListItemUI::ResumeLoad()
{
	if(_bIsDowning)
		return ;
	
	if(!m_pHttp)
		m_pHttp = new CHTTPprotocol;

	m_pHttp->CreateCurl();
	
	m_thread = (HANDLE)_beginthreadex(
		NULL,			        	// SD
		0,				        	// initial stack size
		ThreadProc,			// thread function
		this, 	// thread argument
		0,				        	// creation option
		NULL						// thread identifier
		);	

	if (!m_thread)
	{
		//log here
		return ;
	}

	_bIsDowning = true;
	ShowOrHideBtn(false);
}

void DownloadListItemUI::PauseLoad()
{
	if(!_bIsDowning)
		return ;
	_bIsDowning = false;
	ShowOrHideBtn(true);
}

void DownloadListItemUI::ShowOrHideBtn(bool bShow /* = true */)
{
	_pStart->SetVisible(bShow);
	_pPause->SetVisible(!bShow);
}

bool DownloadListItemUI::ProcessEvent()
{
	//启动下载任务
	ResumeLoad();
	return true;
}

bool DownloadListItemUI::RespondEvent()
{
	return false;
}

void DownloadListItemUI::Cleanup()
{
	if (m_thread)
	{
		CloseHandle(m_thread);
		m_thread = NULL;
	}
	if (m_pHttp)
	{
		m_pHttp->ReleaseCurl();
	}

	SafeDelete(m_pHttp);
}

unsigned __stdcall DownloadListItemUI::ThreadProc(void *pvArg)
{
	DownloadListItemUI* pThis = static_cast<DownloadListItemUI*>(pvArg);
	if (!pThis)
	{
		return -1;
	}
	pThis->_ThreadProc_();

	return 0;
}

void DownloadListItemUI::_ThreadProc_()
{
	if(m_pHttp&&_spSongInfo)
	{
//		CDuiString sUrl = _T("http://wt.codefans.net/x/7/201204/Boostkfzn_pdf.rar");
		CDuiString sUrl = _spSongInfo->GetListenFileUrl().GetString();
		CDuiString sSave;
		sSave.Format(_T("%s"),_spSongInfo->GetLocalPath());
		int ret = m_pHttp->DownloadFile(mci::unicode_2_utf8(sUrl).c_str(),mci::unicode_2_ansi(sSave).c_str(),LoadProgress,this);
		if ( 0 == ret /*&& m_pHttp->getDownloadFileLenth(sUrl.str()) == m_pHttp->getLocalFileLenth(sSave.str())*/)
		{//下载成功
			VSOutput::Write(_T("%s 下载成功\n"),mci::unicode_2_ansi(sUrl).c_str());
			theMainWin->ShowTrayTooltip(_T("1首歌曲下载完成"),_T("逸听"));
			CListUI* pParent = static_cast<CListUI*>(GetParent());
			if (pParent)
			{
				pParent->Remove(this);
			}
			YMusicLibWnd::Instance()->Downloaded(_spSongInfo);
		}
		else
		{
			VSOutput::Write(_T("%s 下载出错\n"),mci::unicode_2_ansi(sUrl).c_str());
		}
	}
}

int DownloadListItemUI::LoadProgress(void* ptr, double rDlTotal, double rDlNow, double rUlTotal, double rUlNow)
{
	DownloadListItemUI* pThis = static_cast<DownloadListItemUI*>(ptr);
	if(!pThis) return 0;
	double downloadOld = pThis->m_downloadOld;
	//	剩余时间：
	int timeLeft = (int)((rDlTotal - rDlNow) / (rDlNow - downloadOld));
	//  下载速录
	double drate = rDlNow - downloadOld;
	

	double percent = 100.0 * (rDlNow/rDlTotal); 
	VSOutput::Write(_T("rDlNow:%.2f,rDlTotal:%.2f,percent:%.2f,rUlTotal:%.2f,rUlNow:%.2f\n"),rDlNow,rDlTotal,percent,rUlTotal,rUlNow);
	if (pThis)
	{
		pThis->m_downloadOld = rDlNow;
		pThis->_pProgress->SetValue((int)percent);
		pThis->_pSizeLab->SetText(getSizeString(rDlNow,rDlTotal));
		pThis->_pRateLab->SetText(getRateString(drate));
	}
	if (percent>=100.0)
	{
		VSOutput::Write(_T("Finished!\n"));
	}
	return 0; 
}
CDuiString DownloadListItemUI::getSizeString(double dload,double size)
{
	double dtmp = size;
	
	CDuiString sReturn;
	sReturn.Format(_T("%.2fMB/%.2fMB"),dload/1024/1024,dtmp/1024/1024);
	return sReturn;
}
CDuiString DownloadListItemUI::getRateString(double rate)
{
	double dtmp = rate;
	CDuiString sReturn;
	sReturn.Format(_T("%.2fKB/s"),dtmp/1024);
	return sReturn;
}
//////////////////////////////////////////////////////////////////
namespace DownloadFinishedListItemName
{
	static const TCHAR kCheckBox[] =  _T("downed_checkbox");
	static const TCHAR kTitlelab[] =  _T("downed_titlelab");
	static const TCHAR kSizelab[] =  _T("downed_sizelab");
	static const TCHAR kPlaybtn[] =  _T("downed_playbtn");
	static const TCHAR kAddbtn[] =  _T("downed_addbtn");
	static const TCHAR kRemove[] =  _T("downed_deletbtn");
};
const int DownloadFinishedListItemUI::DownloadFinishedItemHeight = 40;
DownloadFinishedListItemUI::DownloadFinishedListItemUI()
	: _pCheckBox(NULL),_pTitlelab(NULL),_pSizelab(NULL)
	, _pPlaybtn(NULL),_pAddbtn(NULL),_pRemovebtn(NULL)
{
	this->SetFixedHeight(DownloadFinishedItemHeight);
	CDialogBuilder builder;
	builder.Create(_T("downloadfinishedunit.xml"),0,NULL,NULL ,this);
	OnPrepare();
}
DownloadFinishedListItemUI::~DownloadFinishedListItemUI()
{

}
LPCTSTR DownloadFinishedListItemUI::GetClassName()
{
	return _T("DownloadFinishedListItemUI");
}
LPCTSTR DownloadFinishedListItemUI::GetClass() const
{
	return DownloadFinishedListItemUI::GetClassName();
}

void DownloadFinishedListItemUI::SetSong(spSongInfoT song)
{
	_spSongInfo = song;
	Update();
}

spSongInfoT DownloadFinishedListItemUI::GetSong() const
{
	return _spSongInfo;
}
void DownloadFinishedListItemUI::Update()
{
	if (_spSongInfo)
	{
		CDuiString sTitle;
		sTitle.Format(_T("%s - %s"),_spSongInfo->GetSongName(),_spSongInfo->GetArtistName());
		_pTitlelab->SetText(sTitle);

		double dtemp = _spSongInfo->GetFileSize();
		CDuiString sSize;
		sSize.Format(_T("%.2fMB"),dtemp/1024/1024);
		_pSizelab->SetText(sSize);
	}
}

void DownloadFinishedListItemUI::OnPrepare()
{
	_pTitlelab = static_cast<CLabelUI*>(FindSubControl(DownloadFinishedListItemName::kTitlelab));
	ASSERT(_pTitlelab);
	_pCheckBox = static_cast<CCheckBoxUI*>(FindSubControl(DownloadFinishedListItemName::kCheckBox));
	ASSERT(_pCheckBox);
	_pSizelab = static_cast<CLabelUI*>(FindSubControl(DownloadFinishedListItemName::kSizelab));
	ASSERT(_pSizelab);
	_pPlaybtn = static_cast<CButtonUI*>(FindSubControl(DownloadFinishedListItemName::kPlaybtn));
	ASSERT(_pPlaybtn);
	_pAddbtn = static_cast<CButtonUI*>(FindSubControl(DownloadFinishedListItemName::kAddbtn));
	ASSERT(_pAddbtn);
	_pRemovebtn = static_cast<CButtonUI*>(FindSubControl(DownloadFinishedListItemName::kRemove));
	ASSERT(_pRemovebtn);

	_pPlaybtn->OnNotify += MakeDelegate(this,&DownloadFinishedListItemUI::Notify);
	_pAddbtn->OnNotify += MakeDelegate(this,&DownloadFinishedListItemUI::Notify);
	_pRemovebtn->OnNotify += MakeDelegate(this,&DownloadFinishedListItemUI::Notify);
}
bool DownloadFinishedListItemUI::Notify(void* param)
{
	return true;
}