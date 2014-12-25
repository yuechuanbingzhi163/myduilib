#include "stdafx.h"
#include "LrcToolBar.h"
#include "../YApp.h"
#include "DesktopLrcWindow.h"


CLrcToolBar::CLrcToolBar(CDesktopLrcWindow* pDeskLrc)
{
	m_pDeskLrc = pDeskLrc;
	m_bMouseOn = false;
	m_bTrace = false;
	memset(&_ui,0,sizeof(_ui));

	theMainWin->AddReceive(this);
}
CLrcToolBar::~CLrcToolBar()
{
	RemoveObserver();
//	theNotifyCenter->removeObserver(NT_PlayStatusChanged,boost::bind(&CLrcToolBar::OnSongStatusChanged,this,_1));
}
LPCTSTR CLrcToolBar::GetWindowClsss() 
{
	return _T("LrcToolBar");
}
LPCTSTR CLrcToolBar::GetWindowClassName() const
{
	return CLrcToolBar::GetWindowClsss();
}
void CLrcToolBar::Notify(TNotifyUI& msg)
{
	YTrace_(_T("CLrcToolBar::Notify class:%s,id:%s,msg:%s"),msg.pSender->GetClass(),msg.pSender->GetName().GetData(),msg.sType.GetData());
	CControlUI* pSender = msg.pSender;
	if (0 == msg.sType.Compare(kWindowInit))
	{
		theMainWin->BroadCast();
	}
	else if (0 == msg.sType.Compare(kClick))
	{
		if (pSender == _ui._pPreSongBtn)
		{
			theMainWin->PrevSong();
		}
		else if (pSender == _ui._pPlayBtn)
		{
			theMainWin->PlayOrPause(true);
		}
		else if (pSender == _ui._pPauseBtn)
		{
			theMainWin->PlayOrPause(false);
		}
		else if (pSender ==_ui._pNextSongBtn)
		{
			theMainWin->NextSong();
		}
		else if (pSender == _ui._pZoomInFont)
		{
		}
		else if (pSender == _ui._pZoomOutFont)
		{
		}
		else if (pSender == _ui._pThemeBtn)
		{
		}
		else if (pSender == _ui._pSetting)
		{
			theMainWin->ShowSettingWin();
		}
		else if (pSender == _ui._pSearchBtn)
		{
			theMainWin->ShowSelectLrcWin();
		}
		else if (pSender == _ui._pLockBtn)
		{
			m_pDeskLrc->LockLrc(true);
			ShowWindow(false);
			theMainWin->ShowTrayTooltip(_T("桌面歌词已锁定,您可以通过右键解锁！"),_T("逸听"));
			sLog("CLrcToolBar::Notify %s,lock lrc",pSender->GetName().GetData());
		}
		else if (pSender== _ui._pPanelLrcBtn)
		{
			theMainWin->ShowPanelLrcTab(true);
			if(m_pDeskLrc)::SendMessage(m_pDeskLrc->GetHWND(),WM_SYSCOMMAND,SC_MINIMIZE,0);
			sLog("CLrcToolBar::Notify %s,ShowPanelLrcTab",pSender->GetName().GetData());
		}
		else if (pSender == _ui._pCloseSysBtn)
		{
			if(m_pDeskLrc)::SendMessage(m_pDeskLrc->GetHWND(),WM_SYSCOMMAND,SC_MINIMIZE,0);
			sLog("CLrcToolBar::Notify %s,HideDeskLrc",pSender->GetName().GetData());
		}	
	}
	else if (0 == msg.sType.Compare(kSelectChanged))
	{
		if(pSender == _ui._pKaraok)
		{
			bool bChecked = static_cast<CFadeCheckBoxUI*>(pSender)->IsSelected();
			theConfigMgr->GetNormalConfig()->SetKaraokMode(bChecked);
			sLog("CLrcToolBar::Notify %s,SetKaraokMode(%d)",pSender->GetName().GetData(),bChecked);
		}
	}
}
void CLrcToolBar::SetStatePlay(bool bPlay /* = true */)
{
	_ui._pPlayBtn->SetVisible(!bPlay);
	_ui._pPauseBtn->SetVisible(bPlay);
}
LRESULT CLrcToolBar::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::ProcessWindowMessage(uMsg,wParam,lParam,bHandled);
}
LRESULT CLrcToolBar::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sLog("CWebCLrcToolBarLoginWindow::OnCreate");

	ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if(!RegisterSkin(_T("lrc-toolbar.xml")))
		return 0;
	DuiLib::CDuiRect rcBar;
	GetWindowRect(&rcBar);
	BOOL nRet = SetWindowPos(NULL,&rcBar,SWP_NOACTIVATE|SWP_NOSENDCHANGING );
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);//
	GetPaintMgr()->AddNotifier(this);
	Init();
//	theNotifyCenter->addObserver(NT_PlayStatusChanged,boost::bind(&CLrcToolBar::OnSongStatusChanged,this,_1));
	
//	theMainWin->BroadCast();
	return 0;
}

LRESULT CLrcToolBar::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!m_bTrace)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = this->m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		m_bTrace = _TrackMouseEvent(&tme)==0 ? false : true;
	}	
	m_bMouseOn = true;
	bHandled = false;
	YTrace_(m_bMouseOn?_T("CLrcToolBar::OnMouseMove:m_bMouseOn=true"):_T("CLrcToolBar::OnMouseMove:m_bMouseOn=false"));
	return 0;
}
LRESULT CLrcToolBar::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bTrace = false;
	m_bMouseOn = false;
	YTrace_(m_bMouseOn?_T("CLrcToolBar::OnMouseLeave:m_bMouseOn=true"):_T("CLrcToolBar::OnMouseLeave:m_bMouseOn=false"));
	if (!m_pDeskLrc->GetMouseLeaveTag())
	{
		m_pDeskLrc->SetBkTag(false|m_pDeskLrc->GetLrcMask()->GetMouseLeaveTag());
	}
	bHandled = false;
	return 0;
}

bool CLrcToolBar::GetMouseLeaveTag()
{
	return m_bMouseOn;
}

void CLrcToolBar::Init()
{
	FindControl(_ui._pPreSongBtn,_T("pre"));
	FindControl(_ui._pPlayBtn,_T("play"));
	FindControl(_ui._pPauseBtn,_T("pause"));
	FindControl(_ui._pNextSongBtn,_T("next"));

	FindControl(_ui._pZoomInFont,_T("zoominfont"));
	FindControl(_ui._pZoomOutFont,_T("zoomoutfont"));
	FindControl(_ui._pKaraok,_T("karaok"));
	FindControl(_ui._pThemeBtn,_T("theme"));
	FindControl(_ui._pSetting,_T("config"));

	FindControl(_ui._pSearchBtn,_T("search"));

	FindControl(_ui._pLockBtn,_T("lock"));
	FindControl(_ui._pPanelLrcBtn,_T("panel"));
	FindControl(_ui._pCloseSysBtn,_T("close"));
}

void CLrcToolBar::PlayOrPause()
{
// 	switch(thePlayerCenter->GetPlayStatus())
// 	{
// 	case pcs_idle:    thePlayerCenter->PlayNextSong(); break;
// 	case pcs_playing: thePlayerCenter->Pause(); break;
// 	case pcs_pause:   thePlayerCenter->Continue(); break;
// 	default:          ASSERT(FALSE); break;
// 	}
}

void CLrcToolBar::OnSongStatusChanged(/*CNotifyCenter::NotifyT& notify*/)
{
// 	if(notify.nType != NT_PlayStatusChanged && !notify.lp) return;
// 	PlayCenterStatus eStatus = *(PlayCenterStatus*)(notify.lp);
// 	ASSERT(ui._pPlayCheckBox);
// 	switch (eStatus)
// 	{
// 	case pcs_idle:
// 		{
// 			ui._pPlayCheckBox->SetCheck(false,false);
// 		}
// 		break;
// 	case pcs_playing:
// 		{
// 			ui._pPlayCheckBox->SetCheck(true,false);
// 		}
// 		break;
// 	case pcs_pause:
// 		{
// 			ui._pPlayCheckBox->SetCheck(false,false);
// 		}		
// 		break;
// 	}
}

BOOL CLrcToolBar::Receive(SkinChangedParam param)
{
	try
	{
		if(!IsWindow()) return FALSE;
		sLog("CLrcToolBar::Receive");
		CDuiString bgimage_ = param.bgimage;
		DWORD bkcolor_ = param.bkcolor;
		CControlUI* background = GetPaintMgr()->GetRoot();
		if (background != NULL&&0!=bgimage_.Compare(background->GetBkImage()))
		{
			if (!param.bgimage.IsEmpty())
			{
				CDuiString sBkImage = bgimage_;

				background->SetBkImage(sBkImage);
			}
			else
				background->SetBkImage(_T(""));

			background->SetBkColor(param.bkcolor);
		}

		return TRUE;
	}
	catch (...)
	{
		sLogError("CLrcToolBar::Receive");
		throw _T("CLrcToolBar::Receive");
		return FALSE;
	}	
}
