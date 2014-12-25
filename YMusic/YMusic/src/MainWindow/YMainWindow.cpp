#include "stdafx.h"

#include "YMainWindow.h"
#include "../YApp.h"

//#include "../AddingWindow/AddingSong.h"
#include "../LrcSelectWnd/LrcSelectWnd.h"
#include "../Share/ShareWin.h"
#include "../Login/LoginWindow.h"
#include "../DesktopLrc/LrcMask.h"
#include "../WebService/HttpBase.h"
#include "../Setting/SettingWindow.h"
#include "../TagModify/TagModifyWin.h"
#include "../Property/PropertyWin.h"
#include "../PlayMgr/audioinfo.h"
#include "../MvFolder/MediaPlayerWnd.h"
#include <ShObjIdl.h>
//-------------------------------------------------------------------
//添加歌曲时使用的线程参数
typedef struct 
{
	bool  _bOpenFile;
	CLabelUI* _pCount;
	CListUI* _pPlayList;
	CListUI* _pSongList;
	HWND     _hWnd;
	int     _nPlayListID;
}AddSongParam,*LPADDSONGPARAM;
//-------------------------------------------------------------------
YMainWindow::YMainWindow()
{
	_uTaskbarCreatedMsg = CTrayIconController::RegisterTaskbarCreatedMsg();
	_uTaskbarButtonCreateMsg = ::RegisterWindowMessage(_T("TaskbarButtonCreated"));
	_hOnlineIcon        = ::LoadIcon(CPaintManagerUI::GetInstance(),MAKEINTRESOURCE(IDI_YMUSIC));
	_hOfflineIcon       = ::LoadIcon(CPaintManagerUI::GetInstance(),MAKEINTRESOURCE(IDI_YMUSIC));
}
//-------------------------------------------------------------------
YMainWindow::~YMainWindow()
{
	::DestroyIcon(_hOnlineIcon);
	::DestroyIcon(_hOfflineIcon);
}
//-------------------------------------------------------------------
LPCTSTR YMainWindow::GetWindowClsss()  
{
	return _T("YMusic");
}
//-------------------------------------------------------------------
LPCTSTR YMainWindow::GetWindowClassName() const
{
	return YMainWindow::GetWindowClsss();
}
//-------------------------------------------------------------------
UINT YMainWindow::GetClassStyle() const
{
	return CS_DBLCLKS;
}
//-------------------------------------------------------------------
void YMainWindow::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
//-------------------------------------------------------------------
CControlUI* YMainWindow::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass,PLAY_LIST) == 0)
	{
		return new CPlayListUI;
	}
	if(_tcscmp(pstrClass,SONG_LIST) == 0)
	{
		return new CSongListUI;
	}
	if (_tcscmp(pstrClass,_T("LrcView")) == 0)
	{
		return new CLrcViewUI;
	}
	return NULL;
}
//-------------------------------------------------------------------
void YMainWindow::Notify(TNotifyUI& msg)
{
	CControlUI* pUI = msg.pSender;

	YTrace_(_T("YMainWindow::Notify class:%s,event:%s,name:%s,text:%s,tag:%d"),pUI ? pUI->GetClass():_T(""),msg.sType.GetData(),pUI?pUI->GetName().GetData():_T(""),pUI?pUI->GetText().GetData():_T(""),pUI?(int)pUI->GetTag():0);

	if (0 == msg.sType.Compare(kItemActivate))
		return OnItemActivate(msg);
	else if(0 == msg.sType.Compare(kItemSelect))
		return OnItemSelect(msg);
	else if(0 == msg.sType.Compare(kSelectChanged))
		return OnSelectChanged(msg);
	else if (0 == msg.sType.Compare(kClick))
		return OnClick(msg);
	else if(0 == msg.sType.Compare(kMenu))
		return OnMenu(msg);
	else if(0 == msg.sType.Compare(kValueChanged))
		return OnValueChanged(msg);
	else if (0 == msg.sType.Compare(kKillFocus))
		return OnBlur(msg);
	else if(0 == msg.sType.Compare(kReturn))
		return OnReturn(msg);
	else if (0 == msg.sType.Compare(kTimer))
		return OnTimer(msg);
}
//-------------------------------------------------------------------
void YMainWindow::OnItemActivate(const TNotifyUI& msg)
{
	CControlUI* pSender = msg.pSender;
	if (pSender->IsClass(YPlayListItemUI::GetClassName()))
	{
		if(_ui._pSongList->GetCount()==0) return;
		YSongListItemUI* pSongItem = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(0));

		int iIndex_List = _ui._pPlayList->GetCurSel();
		if(iIndex_List<0) return ;
		YPlayListItemUI* pListItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(iIndex_List));
		if (!thePlayListMgr->GetCurList()||pListItem->GetPlayList()->GetLocalId()!=thePlayListMgr->GetCurList()->GetLocalId())
		{
			thePlayListMgr->ClearRand();
		}
		thePlayListMgr->SetCurList(pListItem->GetPlayList());
		thePlayListMgr->SetCurSong(pSongItem->GetSong());
		thePlayer->Stop();
		PlayOrPause(true);
	}
	else if (pSender->IsClass(YSongListItemUI::GetClassName()))
	{
		int iIndex_List = _ui._pPlayList->GetCurSel();
		if(iIndex_List<0) return ;
		YPlayListItemUI* pListItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(iIndex_List));
		int iIndex_Song = _ui._pSongList->GetCurSel();
		if(iIndex_Song<0) return ;
		YSongListItemUI* pSongItem = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(iIndex_Song));
		if (!thePlayListMgr->GetCurList()||pListItem->GetPlayList()->GetLocalId()!=thePlayListMgr->GetCurList()->GetLocalId())
		{
			thePlayListMgr->ClearRand();
		}
		thePlayListMgr->SetCurList(pListItem->GetPlayList());
		thePlayListMgr->SetCurSong(pSongItem->GetSong());
		thePlayer->Stop();
		PlayOrPause(true);
	}
}
//-------------------------------------------------------------------
void YMainWindow::OnItemSelect(const TNotifyUI& msg)
{
	CControlUI* pSender = msg.pSender;
	if (pSender->IsName(varMainWindow::kPlayList))
	{
		int iIndex = _ui._pPlayList->GetCurSel();
		if(iIndex<0) return ;
		YPlayListItemUI* pListItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(iIndex));
		if (pListItem&&pListItem->GetPlayList()->GetLocalId()!=_ui._pSongList->GetTag())
		{
			_ui._pSongList->SetTag(pListItem->GetPlayList()->GetLocalId());
			UpdateSongList(pListItem->GetPlayList()->GetLocalId());
		}

		_ui._pSongList->NeedUpdate();
	}
	
}
//-------------------------------------------------------------------
void YMainWindow::OnSelectChanged(const TNotifyUI& msg)
{
	CControlUI* pSender = msg.pSender;
	if (pSender->IsName(varMainWindow::kLrc))
	{
		_ui._pTabLayout->SelectItem((UINT)pSender->GetTag());
	}
	else if (pSender->IsName(varMainWindow::kVolume))
	{
		bool bChecked = _ui._pVolumeBtn->IsSelected();

		thePlayer->Volume(bChecked?0:theConfigMgr->GetNormalConfig()->GetVolume());
		_ui._pVolumeBtn->SetToolTip((bChecked ? _T("取消静音"):_T("静音")));
		theConfigMgr->GetNormalConfig()->SetVolumeOff(bChecked);
	
		CString sInfoTip;
		bChecked ? sInfoTip.Format(_T("当前已静音")) : sInfoTip.Format(_T("当前音量%d%%"),_ui._pVolumeSlider->GetValue());
		CInfoWindow::ShowInfoWindow(theMainWin->GetHWND(),_ui._pVolumeBtn->GetPos(),sInfoTip);
	}
	else if(pSender->IsName(varMainWindow::kExpandMusicLib))
	{
		bool bChecked = static_cast<CCheckBoxUI*>(pSender)->IsSelected();
		ShowMusicLibWin(bChecked);
		pSender->SetToolTip(bChecked?_T("隐藏乐库"):_T("展开乐库"));
	}
	else if (pSender->IsName(varMainWindow::kCollect))
	{
		CInfoWindow::ShowInfoWindow(theMainWin->GetHWND(),_ui._pCollect->GetPos(),_T("暂不支持收藏"));
		_ui._pCollect->Selected(false);
	}
}
//-------------------------------------------------------------------
void YMainWindow::OnClick(const TNotifyUI& msg)
{
	CControlUI* pSender = msg.pSender;

	if (pSender->IsName(varMainWindow::kQuitApp))
	{
		BOOL isToTray = FALSE;
		isToTray = theConfigMgr->GetNormalConfig()->IsCloseToTray();
		if(!isToTray)
		{
			sLog("PostQuitMessage");
			this->Stop();
			PostMessage(WM_QUIT,WPARAM(0),LPARAM(0));
		}
		else 
		{
			SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
			ShowWindow(SW_HIDE);//隐藏主窗口 
			ShowTrayTooltip(_T("隐藏主程序到托盘,您可以通过右键菜单恢复！"),_T("逸听"));
		}
	}
	else if (pSender->IsName(varMainWindow::kChangeSkin))
		OnShowSkinWindow();
	else if (pSender->IsName(varMainWindow::kMainMenu))
		OnShowMainMenu();
	else if (pSender->IsName(varMainWindow::kMiniMode))
		OnShowMiniMode();
	else if (pSender->IsName(varMainWindow::kShowMin))
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	else if (pSender->IsName(varMainWindow::kRestoreSize))
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
	else if (pSender->IsName(varMainWindow::kShowMax))
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	else if (pSender->IsName(varMainWindow::kAddPlayList))
		OnAddPlayList();
	else if (pSender->IsName(varMainWindow::kNext))
		NextSong();
	else if (pSender->IsName(varMainWindow::kPrev))
		PrevSong();
	else if(pSender->IsName(varMainWindow::kPlay))
		PlayOrPause(true);
	else if(pSender->IsName(varMainWindow::kPause))
		PlayOrPause(false);
	else if (pSender->GetUserData().Compare(varPlayModeMenu::kUserData)==0)
		OnShowPlayModeMenu();
	else if(pSender->IsName(varMainWindow::kGoBack))
		OnGoBackLayout(msg);
	else if(pSender->IsName(varMainWindow::kGoNext))
		OnGoNextLayout(msg);
	else if(pSender->IsName(varMainWindow::kGoDesk))
		OnGoDeskLayout(msg);
	else if(pSender->IsName(list_song_item::kNorMenu))
		OnSongItemMenu(msg);
	else if(pSender->IsName(varMainWindow::kLogin)||pSender->IsName(_T("favoriteloginbtn")))
		OnShowLoginWindow();
	else if (pSender->IsName(varMainWindow::kSearchBtn))
		ShowSelectLrcWin();
	else if (pSender->IsName(varMainWindow::kShare))
		CInfoWindow::ShowInfoWindow(theMainWin->GetHWND(),pSender->GetPos(),_T("暂不支持分享"));	
}
//-------------------------------------------------------------------
void YMainWindow::OnMenu(const TNotifyUI& msg)
{
	if (msg.pSender->IsName(varMainWindow::kPlayList))
		return OnPlayListMenu(msg);
	else if (msg.pSender->IsName(varMainWindow::kSongList))
		return OnSongListMenu(msg);
	else if(msg.pSender->IsName(varMainWindow::kLrcView1))
		return OnShowLrcViewMenu(msg);
}
//-------------------------------------------------------------------
void YMainWindow::OnBlur(const TNotifyUI& msg)
{
	CControlUI* pSender = msg.pSender;
	if (pSender->IsName(play_list_item::kListEdit))
	{
		CString sName(pSender->GetText());
		sName.Trim();
		if (!sName.IsEmpty())
		{
			YPlayListItemUI* pParent = (YPlayListItemUI*)(pSender->GetTag());
			CString sNameOri = pParent->GetPlayList()->GetName();
			if(sNameOri==sName)
			{
				pParent->ReName(false);
				ShowInfo(_T("播放列表未做修改"));
				GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,2000);
				return ;
			}
			if (theDbMgr->GetUserDB()->IsPlayListExists(sName))
			{
//				static_cast<CEditUI*>(pSender)->SetFocus();
//				static_cast<CEditUI*>(pSender)->SetSelAll();
				CInfoWindow::ShowInfoWindow(m_hWnd,pSender->GetPos(),_T("不允许使用重复的列表名"));
				static_cast<CEditUI*>(pSender)->SetText(sNameOri);
				pParent->ReName(false);
				ShowInfo(_T("播放列表未做修改"));
				GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,2000);
				return ;
			}
			pParent->ReName(false);
			theDbMgr->GetUserDB()->UpdatePlayList(pParent->GetPlayList());

			ShowInfo(_T("成功更名播放列表"));
			GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,2000);
		}
	}
}
//-------------------------------------------------------------------
void YMainWindow::OnReturn(const TNotifyUI& msg)
{
	CControlUI* pSender = msg.pSender;
	if (pSender->IsName(play_list_item::kListEdit))
	{
		CString sName(pSender->GetText());
		sName.Trim();
		if (!sName.IsEmpty())
		{
			YPlayListItemUI* pParent = (YPlayListItemUI*)(pSender->GetTag());
			CString sNameOri = pParent->GetPlayList()->GetName();
			if(sNameOri==sName)
			{
				pParent->ReName(false);
				ShowInfo(_T("播放列表未做修改"));
				GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,2000);
				return ;
			}
			if (theDbMgr->GetUserDB()->IsPlayListExists(sName))
			{
				static_cast<CEditUI*>(pSender)->SetFocus();
				static_cast<CEditUI*>(pSender)->SetSelAll();
				CInfoWindow::ShowInfoWindow(m_hWnd,pSender->GetPos(),_T("不允许使用重复的列表名"));
				return ;
			}
			YPlayListItemUI* plistItem = (YPlayListItemUI*)(pSender->GetTag());
			plistItem->ReName(false);
			theDbMgr->GetUserDB()->UpdatePlayList(plistItem->GetPlayList());

			ShowInfo(_T("成功更名播放列表"));
			GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,2000);
		}
	}
}
//-------------------------------------------------------------------
void YMainWindow::OnTimer(const TNotifyUI& msg)
{
	CControlUI* pSender = msg.pSender;
	if(!pSender) return ;
	if (pSender->IsName(varMainWindow::kInfoLayout))
	{
		GetPaintMgr()->KillTimer(_ui._pInfoLayout);
		_ui._pInfoLayout->SetVisible(false);
	}
}
//-------------------------------------------------------------------
void YMainWindow::OnPlayListMenu(const TNotifyUI& msg)
{
	CControlUI* pSender = msg.pSender;
	CMenuWnd* pMenu = new CMenuWnd(GetHWND());
	POINT pt;
	GetCursorPos(&pt);
	pMenu->Init(NULL,_T("play-list-menu.xml"),0,pt);
	pMenu->SetTag((UINT_PTR)EMT_PLAYLIST);
	
	ScreenToClient(&pt);
	CControlUI* pControl = GetPaintMgr()->FindSubControlByPoint(msg.pSender,pt);
	UINT uSelCur = _ui._pPlayList->GetCurSel();
	if (!pControl||uSelCur<0)
	{
		//没有选中任何项
		LPCTSTR kItems[] =
		{
			varPlayListMenu::kPlay,
			varPlayListMenu::kRemove,
			varPlayListMenu::kRename
		};
		for(int i=0;i<sizeof(kItems)/sizeof(kItems[0]);++i)
			pMenu->EnableItem(kItems[i],FALSE);	
	}
	else if (pControl&&pControl->IsClass(kVerticalLayoutUIClassName))
	{
		//没有选中任何项
		LPCTSTR kItems[] =
		{
			varPlayListMenu::kPlay,
			varPlayListMenu::kRemove,
			varPlayListMenu::kRename
		};
		for(int i=0;i<sizeof(kItems)/sizeof(kItems[0]);++i)
			pMenu->EnableItem(kItems[i],FALSE);	
	}
	else
	{
		YPlayListItemUI* pItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(uSelCur));
		YTrace_(pItem->GetPlayList()->GetName());
		if (pItem->GetPlayList()->GetListType()!=CPlayListInfo::PlayListType::plt_user)
		{
			LPCTSTR kItems[] =
			{
				varPlayListMenu::kRemove,
				varPlayListMenu::kRename
			};
			for(int i=0;i<sizeof(kItems)/sizeof(kItems[0]);++i)
				pMenu->EnableItem(kItems[i],FALSE);	
		}
		if (_ui._pSongList->GetCount()==0)
		{
			pMenu->EnableItem(varPlayListMenu::kPlay,FALSE);
		}
		pMenu->SetUserData((UINT_PTR)pItem);
	}
}
//-------------------------------------------------------------------
void YMainWindow::OnSongListMenu(const TNotifyUI& msg)
{
	CMenuWnd* pMenu = new CMenuWnd(GetHWND());
	POINT pt;
	GetCursorPos(&pt);
	pMenu->Init(NULL,_T("song-list-menu.xml"),0,pt);
	pMenu->SetTag((UINT_PTR)EMT_SONGLIST);

	ScreenToClient(&pt);
	CControlUI* pControl = GetPaintMgr()->FindSubControlByPoint(msg.pSender,pt);
	const int nCurSelCnt = _ui._pSongList->GetSelCount();
	int nCurList = _ui._pPlayList->GetCurSel();
	if(nCurList<0) nCurList = 0;
	YPlayListItemUI* pListItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(nCurList));
	if (!pControl||nCurSelCnt<=0||(pControl&&pControl->IsClass(kVerticalLayoutUIClassName)))
	{
		//没有选中任何项
		LPCTSTR kItems[] =
		{
			varSongListMenu::kAddTo,
			varSongListMenu::kMoveTo,
			varSongListMenu::kPlay,
			varSongListMenu::kShare,
			varSongListMenu::kCollect,
			varSongListMenu::kModify,
			varSongListMenu::kRemove,
			varSongListMenu::kOpenDir,
			varSongListMenu::kProperty,
			varSongItemMenu::kDownLoad
		};
		for(int i=0;i<sizeof(kItems)/sizeof(kItems[0]);++i)
			pMenu->EnableItem(kItems[i],FALSE);	
	}
	if(nCurSelCnt > 1)
	{
		LPCTSTR kItems[] =
		{
			varSongListMenu::kPlay,
			varSongListMenu::kOpenDir,
			varSongListMenu::kProperty,
			varSongListMenu::kModify,
			varSongListMenu::kShare
		};
		for(int i=0;i<sizeof(kItems)/sizeof(kItems[0]);++i)
			pMenu->EnableItem(kItems[i],FALSE);	
	}
	if (_ui._pSongList->GetCount()==0)
	{
		LPCTSTR kItems[] =
		{
			varSongListMenu::kClear,
			varSongListMenu::kDumplicate
		};
		for(int i=0;i<sizeof(kItems)/sizeof(kItems[0]);++i)
			pMenu->EnableItem(kItems[i],FALSE);			
	}
	int nCurSong = _ui._pSongList->GetCurSel();
	if(nCurSong<0) nCurSong = 0;
	YSongListItemUI* pSongItem = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(nCurSong));

	if (pSongItem&&pSongItem->GetSong())
	{
		if(pSongItem->GetSong()->IsLocal())//本地歌曲
		{
			//Disabled：下载和分享
			pMenu->EnableItem(varSongItemMenu::kDownLoad,FALSE);
			pMenu->EnableItem(varSongListMenu::kShare,FALSE);
			pMenu->EnableItem(varSongListMenu::kCollect,FALSE);
		}
		else
		{
			LPCTSTR kItems[] =
			{
				varSongListMenu::kOpenDir
			};
			for(int i=0;i<sizeof(kItems)/sizeof(kItems[0]);++i)
				pMenu->EnableItem(kItems[i],FALSE);		
		}
	}

	if (pListItem
		&&pListItem->GetPlayList()->GetListType()==CPlayListInfo::plt_sys
		&&pListItem->GetPlayList()->GetName().Compare(_T("我的收藏"))==0)
	{
		LPCTSTR kItems[] =
		{
			varSongListMenu::kAddTo,
			varSongListMenu::kMoveTo,
			varSongListMenu::kAdd,
			varSongListMenu::kOpenDir,
			varSongListMenu::kProperty,
			varSongListMenu::kModify
		};
		for(int i=0;i<sizeof(kItems)/sizeof(kItems[0]);++i)
			pMenu->EnableItem(kItems[i],FALSE);			
	}
	
	CMenuElementUI* pAddtoItem = static_cast<CMenuElementUI*>(pMenu->GetPaintManager()->FindControl(varSongListMenu::kAddTo));
	CMenuElementUI* pMovetoItem = static_cast<CMenuElementUI*>(pMenu->GetPaintManager()->FindControl(varSongListMenu::kMoveTo));
	ASSERT(pAddtoItem);ASSERT(pMovetoItem);	
//	CPlayCenter::playListMapT sListArray = thePlayCenter->LoadAllPlayList();
	std::vector<spPlayListInfoT> sListArray;
	theDbMgr->GetUserDB()->GetAllPlayLists(sListArray);
	if (!sListArray.empty())
	{//音乐列表不空
		int iCount = 0;
//		for (CPlayCenter::playListMapT::iterator it=sListArray.begin();it!=sListArray.end();++it)
		for (std::vector<spPlayListInfoT>::iterator it=sListArray.begin();it!=sListArray.end();++it)
		{
			if (_ui._pSongList->GetTag()!=(*it)->GetLocalId()
/*				&&it->second.GetListType()!=CPlayListInfo::plt_sys*/
				&&(*it)->GetName().Compare(_T("我的收藏"))!=0)
			{//循环遍历歌曲添加到其他列表，自己除外
				CMenuElementUI* pLineItem = new CMenuElementUI;
				pLineItem->SetName(varSongListMenu::kAddTo);
				pLineItem->SetText((*it)->GetName());
				pLineItem->SetUserData(varSongListMenu::kAddTo);
				pLineItem->SetTag((UINT_PTR)EMT_TOOTHERLIST);

				CMenuElementUI* pLineItem2 = new CMenuElementUI;
				pLineItem2->SetName(varSongListMenu::kMoveTo);
				pLineItem2->SetText((*it)->GetName());
				pLineItem2->SetUserData(varSongListMenu::kMoveTo);
				pLineItem2->SetTag((UINT_PTR)EMT_TOOTHERLIST);

				pAddtoItem->Add(pLineItem);//添加到...
				pMovetoItem->Add(pLineItem2);//移动到...
				iCount ++;
			}
		}

		{
			CMenuElementUI* pLineItem = new CMenuElementUI;
			pLineItem->SetName(varSongListMenu::kAddTo);
			pLineItem->SetText(_T("新建列表"));
			pLineItem->SetUserData(varSongListMenu::kToNewList);
			pLineItem->SetTag((UINT_PTR)EMT_TONEWLIST);

			CMenuElementUI* pLineItem2 = new CMenuElementUI;
			pLineItem2->SetName(varSongListMenu::kMoveTo);
			pLineItem2->SetText(_T("新建列表"));
			pLineItem2->SetUserData(varSongListMenu::kToNewList);
			pLineItem2->SetTag((UINT_PTR)EMT_TONEWLIST);

			pAddtoItem->AddAt(pLineItem,0);
			pMovetoItem->AddAt(pLineItem2,0);
			if(iCount>0)
			{
				CMenuElementUI* pSep = new CMenuElementUI;
				CMenuElementUI* pSep2 = new CMenuElementUI;
				CStdStringPtrMap* pPtrMap = pMenu->GetPaintManager()->GetCustomStyle(_T("linestyle"));
				pSep->SetCustomStyle2(pPtrMap);		
				pSep2->SetCustomStyle2(pPtrMap);		
				pPtrMap = pMenu->GetPaintManager()->GetCustomStyle(_T("line"));
				CLabelUI* pLabel = new CLabelUI;
				CLabelUI* pLabel2 = new CLabelUI;
				pLabel->SetCustomStyle2(pPtrMap);
				pLabel2->SetCustomStyle2(pPtrMap);
				pSep->Add(pLabel);
				pSep2->Add(pLabel2);
				pAddtoItem->AddAt(pSep,1);
				pMovetoItem->AddAt(pSep2,1);
			}
		}
	}
}
//-------------------------------------------------------------------
void YMainWindow::OnSongItemMenu(const TNotifyUI& msg)
{
	CMenuWnd* pMenu = new CMenuWnd(GetHWND());
	POINT pt;
	GetCursorPos(&pt);
	pMenu->Init(NULL,_T("song-item-menu.xml"),0,pt);
	pMenu->SetTag((UINT_PTR)EMT_SONGITEM);

	int nCurList = _ui._pSongList->GetCurSel();
	if(nCurList<0) nCurList = 0;
	YSongListItemUI* pSongItem = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(nCurList));
	
	if (pSongItem&&pSongItem->GetSong()&&pSongItem->GetSong()->IsLocal())
	{
		pMenu->EnableItem(varSongItemMenu::kDownLoad,FALSE);
		pMenu->EnableItem(varSongItemMenu::kShare,FALSE);
		pMenu->EnableItem(varSongItemMenu::kCollect,FALSE);
	}
}
//-------------------------------------------------------------------
void YMainWindow::OnValueChanged(const TNotifyUI& msg)
{
	CControlUI* pSender = msg.pSender;
	if (pSender->IsName(varMainWindow::kPlayProgress))
	{
		int nVal = _ui._pPlayProgress->GetValue();
//		thePlayCenter->SetPostion(nVal+1);

		thePlayer->SeekTo(nVal+1);
	}
	else if (pSender->IsName(varMainWindow::kVolumeslider))
	{
		int nVal = _ui._pVolumeSlider->GetValue();
//		thePlayCenter->SetVolume(nVal);
		theConfigMgr->GetNormalConfig()->SetVolume(nVal);
		theConfigMgr->GetNormalConfig()->SetVolumeOff(_ui._pVolumeBtn->IsSelected());

		thePlayer->Volume(nVal);

		CString sInfoTip;
		sInfoTip.Format(_T("音量%d%%"),nVal);
		CInfoWindow::ShowInfoWindow(theMainWin->GetHWND(),_ui._pVolumeSlider->GetPos(),sInfoTip);
	}
}
//-------------------------------------------------------------------
void YMainWindow::OnShowPlayModeMenu()
{
	CMenuWnd* pModeMenu = new CMenuWnd(GetHWND());
	POINT pt;
	GetCursorPos(&pt);
	pModeMenu->Init(NULL,_T("play-mode-menu.xml"),0,pt);
	pModeMenu->SetTag((UINT_PTR)EMT_PLAYMODE);
	CControlUI* pUI = NULL;
	switch (thePlayListMgr->GetPlayMode())
	{
	case EM_MODE::EM_LIST_ORDER://列表顺序播放
		pUI = pModeMenu->GetPaintManager()->FindControl(varPlayModeMenu::kListOrder);
		break;
	case EM_MODE::EM_LIST_LOOP:
		pUI = pModeMenu->GetPaintManager()->FindControl(varPlayModeMenu::kListLoop);
		break;
// 	case EM_MODE::EM_SINGLE:
// 		pUI = pModeMenu->GetPaintManager()->FindControl(varPlayModeMenu::kSingle);
// 		break;
	case EM_MODE::EM_SINGLE_LOOP:
		pUI = pModeMenu->GetPaintManager()->FindControl(varPlayModeMenu::kSingleLoop);
		break;
	case EM_MODE::EM_RAMDON:
		pUI = pModeMenu->GetPaintManager()->FindControl(varPlayModeMenu::kRandom);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	ASSERT(pUI);
	if(pUI)
	{
		CMenuElementUI* pItem = static_cast<CMenuElementUI*>(pUI);
		COptionUI* pOption = static_cast<COptionUI*>(pItem->FindSubControl(varPlayModeMenu::kCheckBox));
		ASSERT(pOption);
		if(pOption) pOption->Selected(true,false);
	}
}
//-------------------------------------------------------------------
void YMainWindow::OnShowMainMenu()
{
	CMenuWnd* pMenu = new CMenuWnd(GetHWND());
	POINT pt;
	GetCursorPos(&pt);
	pMenu->Init(NULL,_T("main-menu.xml"),0,pt);
	pMenu->SetTag((UINT_PTR)EMT_MAIN);
	CControlUI* pUI = NULL;
	bool isTop = false;
	theDbMgr->GetGlobalDBase()->GetDictValue(ConfigEX::KEEPMOST_TOP,isTop);
	pUI = pMenu->GetPaintManager()->FindControl(varMainMenu::kKeepTop);
	ASSERT(pUI);
	if (pUI)
	{
		CMenuElementUI* pItem = static_cast<CMenuElementUI*>(pUI);
		CCheckBoxUI* pOption = static_cast<CCheckBoxUI*>(pItem->FindSubControl(_T("topcheck")));
		ASSERT(pOption);
		if(pOption) pOption->SetCheck(isTop);
		pItem->SetText(isTop?_T("取消置顶"):_T("总在最前"));
	}
	
	BOOL isLrcVisible = FALSE;
	isLrcVisible = GetDesktopLrcWin()->IsWindowVisible();
	pUI = pMenu->GetPaintManager()->FindControl(varMainMenu::kDeskLrc);
	ASSERT(pUI);
	if (pUI)
	{
		CMenuElementUI* pItem = static_cast<CMenuElementUI*>(pUI);
		pItem->SetText(isLrcVisible?_T("隐藏歌词"):_T("显示歌词"));
	}
	BOOL isLrcLock = FALSE;
	isLrcLock = GetDesktopLrcWin()->IsLock();
	pUI = pMenu->GetPaintManager()->FindControl(varMainMenu::kLockLrc);
	ASSERT(pUI);
	if (pUI)
	{
		CMenuElementUI* pItem = static_cast<CMenuElementUI*>(pUI);
		pItem->SetText(isLrcLock?_T("解锁歌词"):_T("锁定歌词"));
	}
}
//-------------------------------------------------------------------
void YMainWindow::OnShowMiniMode()
{
	
}
//-------------------------------------------------------------------
void YMainWindow::OnShowLrcViewMenu(const TNotifyUI& msg)
{
	CMenuWnd* pMenu = new CMenuWnd(GetHWND());
	POINT pt;
	GetCursorPos(&pt);
	pMenu->Init(NULL,_T("lrc-container-menu.xml"),0,pt);
	pMenu->SetTag((UINT_PTR)EMT_LRCVIEW);
	
// 	TStreamStatus ts;
// 	thePlayCenter->GetStatus(&ts);
	int status = thePlayer->GetStatus();
//	if(!ts.fPause&&!ts.fPlay)
	if(status!=em_play&&status!=em_pause)
	{
		//没有播放任何歌曲
		LPCTSTR kItems[] =
		{
			varLrcViewMenu::kSearchLrc,
			varLrcViewMenu::kLrcModify,
			varLrcViewMenu::kLrcLocal,
			varLrcViewMenu::kLrcEdit,
			varLrcViewMenu::kLrcFolder
		};
		for(int i=0;i<sizeof(kItems)/sizeof(kItems[0]);++i)
			pMenu->EnableItem(kItems[i],FALSE);	
	}		
} 
//-------------------------------------------------------------------
void YMainWindow::SendNotify(LPCTSTR lpControl,LPCTSTR sType)
{
	CControlUI* pControl = NULL;
	FindControl(pControl,lpControl);
	if(!pControl) return ;
	GetPaintMgr()->SendNotify(pControl,sType);
}
//-------------------------------------------------------------------
void YMainWindow::OnShowSkinWindow()
{
	YSkinWindow* spSkin = YSkinWindow::Instance();
	if(!spSkin->IsWindow())
		spSkin->Create(m_hWnd,_T("更改皮肤"),UI_WNDSTYLE_DIALOG,WS_EX_WINDOWEDGE | WS_EX_OVERLAPPEDWINDOW);
	spSkin->CenterWindow();
	spSkin->ShowWindow(true);
}
//-------------------------------------------------------------------
void YMainWindow::OnShowLoginWindow()
{
	sLog("YMainWindow::OnShowLoginWindow");
	CLoginWindow* spLogin = CLoginWindow::Instance();
	if(!spLogin->IsWindow())
		spLogin->Create(m_hWnd,_T("用户登录"),UI_WNDSTYLE_DIALOG,WS_EX_WINDOWEDGE | WS_EX_OVERLAPPEDWINDOW);
	spLogin->CenterWindow();
	spLogin->ShowWindow(true);

// 	CHttpBase* pHttp = HttpBase::Instance();
// 	ASSERT(pHttp);
// 	CString sUrl = _T("http://www.douban.com/j/app/login");
// 	std::string sUtf8Url = em_utility::mci::unicode_2_utf8(sUrl);
// 	CHttpBase::CurlData* pData = new CHttpBase::CurlData();
// 	ASSERT(pData);
// 	pData->Init();
// 	pData->InitPostMethod(sUtf8Url.c_str(),"password=happy2010&email=389465209@qq.com&app_name=radio_desktop_win&version=100");
// 
// 	pHttp->MakeCurlData(pData,m_hWnd);
// 	int ret = pHttp->DoPost();
// 
// 	sLog("DoPost<%d>",ret);
}
//-------------------------------------------------------------------
void YMainWindow::OnShowShareWindow()
{
	spUserContextT pContext = theRuntimeState->GetCurrentContext(NULL);
	spLogonUserInfoT theUser = pContext->GetUser();
	if(!theUser)
	{
		theMainWin->OnShowLoginWindow();
		return;
	}
	CShareWin* pShare = CShareWin::Instance();
	if(!pShare->IsWindow())
		pShare->Create(m_hWnd,_T("分享"),UI_WNDSTYLE_DIALOG,WS_EX_WINDOWEDGE | WS_EX_OVERLAPPEDWINDOW);
	pShare->CenterWindow();
	pShare->ShowWindow(true);
}
//-------------------------------------------------------------------
void YMainWindow::OnGoBackLayout(const TNotifyUI& msg)
{
	if (_ui._pLrcViewTab->GetTag()==0)
		_ui._pTabLayout->SelectItem(0);
	else
	{
		_ui._pLrcViewTab->SetTag((UINT)_ui._pLrcViewTab->GetTag()-1);
		_ui._pLrcViewTab->SelectItem(_ui._pLrcViewTab->GetTag());
		if(_ui._pLrcViewTab->GetTag()+2==_ui._pLrcViewTab->GetCount())
		{
			CButtonUI* pUI = NULL;
			FindControl(pUI,varMainWindow::kGoNext);
			pUI->SetVisible(true);
		}
	}
}
//-------------------------------------------------------------------
void YMainWindow::OnGoNextLayout(const TNotifyUI& msg)
{
	unsigned int nIndex = (UINT)_ui._pLrcViewTab->GetTag()+1;
	if(nIndex==_ui._pLrcViewTab->GetCount()-1) msg.pSender->SetVisible(false);
	_ui._pLrcViewTab->SetTag(nIndex),_ui._pLrcViewTab->SelectItem(_ui._pLrcViewTab->GetTag());
}
//-------------------------------------------------------------------
void YMainWindow::OnGoDeskLayout(const TNotifyUI& msg)
{
	_ui._pTabLayout->SelectItem(0);
	ShowDesktopLrcWin(TRUE);
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnWebService(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sLog("YMainWindow::OnCreate");
	SetIcon(_hOnlineIcon,0);
	SetIcon(_hOnlineIcon);

	ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if(!RegisterSkin(_T("main.xml"),0,this))
		return 0;

	GetPaintMgr()->AddNotifier(this);

	AddTracyIcon();

	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	InitVariable();
	//注册热键;
	sLog("RegAllHotKey");
	theConfigMgr->GetHotKeyConfig()->RegAllHotKey();
	//是否置顶
	BOOL isTop = FALSE;
	theDbMgr->GetGlobalDBase()->GetDictValue(ConfigEX::KEEPMOST_TOP,isTop);
	if(isTop) 
	{
		SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	}
	//读取背景皮肤
	CConfigMgr::spSkinConfigT pSkinCfg = theConfigMgr->GetSkinConfig();
	CString szBgImage = pSkinCfg->GetBg();
	DWORD dwCor = pSkinCfg->GetColor();
	if (!szBgImage.IsEmpty())
	{
		GetPaintMgr()->GetRoot()->SetBkImage(szBgImage);
	}
	//设置定时器
	int _nTimerId = SetTimer(1,500);
	
	ShowDesktopLrcWin(TRUE);

	//尝试自动登录
	TryAutoLogin();

	return 0;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		Sleep(1000);
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
		if( !bZoomed ) {
			
		}
		else {
			
		}
	}
	return lRes;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
/*	if(thePlayCenter->GetPlayState()==EP_STATE::EP_INIT)
		return 0;
	TStreamStatus ts;
	thePlayCenter->GetStatus(&ts);
	if(ts.fPlay)
		UpdatePlayingProgress();
	else if(!ts.fPause)
	{
		switch (thePlayCenter->GetPlayMode())
		{
		case EM_MODE::EM_LIST_ORDER:
		case EM_MODE::EM_LIST_LOOP:
		case EM_MODE::EM_SINGLE_LOOP:
		case EM_MODE::EM_RAMDON:
			{
				sLog("YMainWindow::OnTimer,playmode:%d",thePlayCenter->GetPlayMode());
				this->NextSong();
				OnSongStatusChanged();
			}
			break;
		case EM_MODE::EM_SINGLE:
			{
				spSongInfoT song;
				ShowSongInfo(song);
				thePlayCenter->SetPlayState(EP_STATE::EP_STOP);
				OnSongStatusChanged();
			}
		default:
			break;
		}
	}
*/
	return 0;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg==WM_TIMER)
		OnTimer(uMsg,wParam,lParam,bHandled);
	if (uMsg==_uTaskbarButtonCreateMsg)
		OnTaskbarButtonCreate(uMsg,wParam,lParam,bHandled);
	if (uMsg==_uTaskbarCreatedMsg)
		OnTaskbarCreate(uMsg,wParam,lParam,bHandled);
	if(uMsg==WM_TRAY_NOTIFY)
		OnTrayNotify(uMsg,wParam,lParam,bHandled);
	if(uMsg==WM_USER_PLAYING)
		OnPlayerPlaying(uMsg,wParam,lParam,bHandled);
	if(uMsg==WM_USER_POS_CHANGED)
		OnPlayerPosChanged(uMsg,wParam,lParam,bHandled);
	if(uMsg==WM_USER_END_REACHED)
		OnPlayerEndReached(uMsg,wParam,lParam,bHandled);
	if(uMsg==WM_LOGIN)
		OnUserLogin(uMsg,wParam,lParam,bHandled);
	if(uMsg==WM_HTTP_FILE_DOWNLOAD)
		OnHttpfileDownload(uMsg,wParam,lParam,bHandled);
	return __super::ProcessWindowMessage( uMsg,  wParam,  lParam,  bHandled);
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnMenuCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MenuCmdMsg* pMsg = (MenuCmdMsg*)wParam;
	if (pMsg->nMenuTag==EMT_PLAYLIST)
		OnMenuCommandPlayList(pMsg);
	else if (pMsg->nMenuTag==EMT_SONGLIST)
		OnMenuCommandSongList(pMsg);
	else if(pMsg->nMenuTag==EMT_PLAYMODE)
		OnMenuCommadnPlayMode(pMsg);
	else if(pMsg->nMenuTag==EMT_SONGITEM)
		OnMenuCommandSongItem(pMsg);
	else if (pMsg->nMenuTag==EMT_LRCVIEW)
		OnMenuCommandLrcView(pMsg);
	else if(pMsg->nMenuTag==EMT_MAIN)
		OnMenuCommandMainMenu(pMsg);
	return 0;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	YTrace_(_T("YMainWindow::OnMoving"));
	//移动主窗口时候,同时移动音乐库窗口
	YMusicLibWnd* spMusicLibWnd(GetMusicLibWin2());
//	ASSERT(spMusicLibWnd);
	if (spMusicLibWnd&&spMusicLibWnd->IsWindow())
	{
		RECT rtMusicLib;
		spMusicLibWnd->GetWindowRect(&rtMusicLib);
		LPRECT pRect = (LPRECT)lParam;
		spMusicLibWnd->MoveWindow(pRect->right,pRect->top,rtMusicLib.right-rtMusicLib.left,rtMusicLib.bottom-rtMusicLib.top,TRUE);
	}
	return 0;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	YMusicLibWnd* spMusicLibWnd(GetMusicLibWin2());
//	ASSERT(spMusicLibWnd);
	if (spMusicLibWnd&&spMusicLibWnd->IsWindow())
	{
		RECT rtMusicLib;
		spMusicLibWnd->GetWindowRect(&rtMusicLib);
		LPRECT pRect = (LPRECT)lParam;
		spMusicLibWnd->MoveWindow(pRect->right,pRect->top,rtMusicLib.right-rtMusicLib.left,rtMusicLib.bottom-rtMusicLib.top,TRUE);
	}
	return 0;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnTaskbarCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	AddTracyIcon();
	return 0;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnTaskbarButtonCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
{
	spTaskbarListT theTaskbarList(GetTaskbarList());
	if(theTaskbarList)
	{
		{
		/*
		设置进度条的类型
		TBPF_NORMAL、TBPF_INDETERMINATE、TBPF_NOPROGRESS------蓝色
		TBPF_ERROR-------红色
		TBPF_PAUSED------黄色
		*/
//		theTaskbarList->SetProgressState(GetHWND(),TBPF_NORMAL);
		/*
		设置进度条的位置，现在设为7/10
		*/
// 		theTaskbarList->SetProgressValue(GetHWND(),8,10);
// 		theTaskbarList->SetOverlayIcon(m_hWnd,_hOfflineIcon,_T("off"));
		}
		
		CString sImgPath = CPaintManagerUI::GetInstancePath() + _T("ThumbnailToolbar.bmp");
		HIMAGELIST hImglist = ImageList_LoadImage(NULL,sImgPath,16,1,RGB(255,0,255),IMAGE_BITMAP,LR_LOADFROMFILE);
		theTaskbarList->ThumbBarSetImageList(m_hWnd,hImglist);

		const BOOL isPlaying = (em_play==thePlayer->GetStatus());

		THUMBBUTTON buttons[3];
		const THUMBBUTTONMASK dwMask = THB_BITMAP|THB_TOOLTIP;
		buttons[0].iId  = 0;
		buttons[0].dwMask = dwMask;
		buttons[0].iBitmap = 0;
		lstrcpy(buttons[0].szTip,_T("上一首"));

		buttons[1].iId  = 1;
		buttons[1].dwMask = dwMask;
		buttons[1].iBitmap = (isPlaying ? 1 : 2);
		isPlaying ? lstrcpy(buttons[1].szTip,_T("暂停")) : lstrcpy(buttons[1].szTip,_T("播放"));

		buttons[2].iId  = 2;
		buttons[2].dwMask = dwMask;
		buttons[2].iBitmap = 3;
		lstrcpy(buttons[2].szTip,_T("下一首"));

		theTaskbarList->ThumbBarAddButtons(m_hWnd,sizeof(buttons)/sizeof(buttons[0]),buttons);
		ImageList_Destroy(hImglist);
	}
	return 0;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnTrayNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//wParam接收的是图标的ID， 
	//而lParam接收的是鼠标的行为 
	if(wParam!=IDI_YMUSIC) 
		return 1; 
	switch(lParam) 
	{
		case WM_RBUTTONUP://右键起来时弹出快捷菜单 
		{
			sLog("YMainWindow::OnTrayNotify : WM_RBUTTONUP");
			CMenuWnd* pMenu = new CMenuWnd(GetHWND());
			POINT pt;
			GetCursorPos(&pt);
			pMenu->Init(NULL,_T("tray-menu.xml"),NULL,pt);
			pMenu->SetTag((UINT_PTR)EMT_MAIN);
			CControlUI* pUI = NULL;
			switch (thePlayListMgr->GetPlayMode())
			{
			case EM_MODE::EM_LIST_ORDER://列表顺序播放
				pUI = pMenu->GetPaintManager()->FindControl(varPlayModeMenu::kListOrder);
				break;
			case EM_MODE::EM_LIST_LOOP:
				pUI = pMenu->GetPaintManager()->FindControl(varPlayModeMenu::kListLoop);
				break;
// 			case EM_MODE::EM_SINGLE:
// 				pUI = pMenu->GetPaintManager()->FindControl(varPlayModeMenu::kSingle);
// 				break;
			case EM_MODE::EM_SINGLE_LOOP:
				pUI = pMenu->GetPaintManager()->FindControl(varPlayModeMenu::kSingleLoop);
				break;
			case EM_MODE::EM_RAMDON:
				pUI = pMenu->GetPaintManager()->FindControl(varPlayModeMenu::kRandom);
				break;
			default:
				ASSERT(FALSE);
				break;
			}
			ASSERT(pUI);
			if(pUI)
			{
				CMenuElementUI* pItem = static_cast<CMenuElementUI*>(pUI);
				COptionUI* pOption = static_cast<COptionUI*>(pItem->FindSubControl(varPlayModeMenu::kCheckBox));
				ASSERT(pOption);
				if(pOption) pOption->Selected(true,false);
			}
			bool isTop = FALSE;
			theDbMgr->GetGlobalDBase()->GetDictValue(ConfigEX::KEEPMOST_TOP,isTop);
			pUI = pMenu->GetPaintManager()->FindControl(varMainMenu::kKeepTop);
			ASSERT(pUI);
			if (pUI)
			{
				CMenuElementUI* pItem = static_cast<CMenuElementUI*>(pUI);
				CCheckBoxUI* pOption = static_cast<CCheckBoxUI*>(pItem->FindSubControl(_T("topcheck")));
				ASSERT(pOption);
				if(pOption) pOption->SetCheck(isTop);
				pItem->SetText(isTop?_T("取消置顶"):_T("总在最前"));
			}
			BOOL isLrcVisible = false;
			isLrcVisible = GetDesktopLrcWin()->IsWindowVisible();
			pUI = pMenu->GetPaintManager()->FindControl(varMainMenu::kDeskLrc);
			ASSERT(pUI);
			if (pUI)
			{
				CMenuElementUI* pItem = static_cast<CMenuElementUI*>(pUI);
				pItem->SetText(isLrcVisible?_T("隐藏歌词"):_T("显示歌词"));
			}
			BOOL isLrcLock = false;
			isLrcLock = GetDesktopLrcWin()->IsLock();
			pUI = pMenu->GetPaintManager()->FindControl(varMainMenu::kLockLrc);
			ASSERT(pUI);
			if (pUI)
			{
				CMenuElementUI* pItem = static_cast<CMenuElementUI*>(pUI);
				pItem->SetText(isLrcLock?_T("解锁歌词"):_T("锁定歌词"));
			}
		}
		break;
		case WM_LBUTTONDBLCLK://双击左键的处理 
		{
			sLog("YMainWindow::OnTrayNotify : WM_LBUTTONDBLCLK");
			this->ShowWindow(true);// 
			SetForegroundWindow(this->m_hWnd);
			this->BringWindowToTop();
		}
		break;
	}
	return 0;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(THBN_CLICKED != (UINT)HIWORD(wParam))
	{
		bHandled = FALSE;
		return 0;
	}
	switch ((int)LOWORD(wParam))
	{
	case 0: 
		PrevSong();
		sLog("[WM_COMMAND][PrevSong]");
		break;
	case 1:
		{
			if (thePlayer->GetHWND()!=theMainWin->GetHWND())
			{
				thePlayer->SetHWND(theMainWin->GetHWND());
				if(CMediaWin::Instance()->IsWindow())
					CMediaWin::Instance()->SendMessage(WM_SYSCOMMAND,SC_CLOSE,0);

				Play();
				sLog("[WM_COMMAND][Play]");
			}
			else
			{
				bool bPlay = thePlayer->GetStatus()==em_play_status::em_play?false:true;
				PlayOrPause(bPlay); 
				sLog("[WM_COMMAND][PlayOrPause(%d)]",bPlay);
			}			
		}
		break;
	case 2: NextSong(); 
		sLog("[WM_COMMAND][NextSong]");
		break;
	default:ASSERT(FALSE);break;
	}
	return 0;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnPlayerPlaying(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sLog("Playing");
	return TRUE;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnPlayerPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
//	sLog("PlayPosChanged");
	UpdatePlayingProgress();
	return TRUE;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnPlayerEndReached(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sLog("EndReached and Ready PlayNext");
	NextSong();
	return TRUE;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnUserLogin(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	OnUserChanged();
	UpdatePlayList();

	spUserContextT pContext = theRuntimeState->GetCurrentContext(NULL);
	spLogonUserInfoT theUser(pContext->GetUser());
	if(theUser)
	{
		CString sTitle;
		sTitle.Format(_T("Hi %s"),theUser->sNickName);
		_trayIcon.ShowToolTip(_T("欢迎回来!!!"),sTitle);
	}

	return 0;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	struct HotKeyOper
	{
		DWORD dwKey;
		UINT  wFlag;
	};
	HotKeyOper hk [] ={ 
		{theConfigMgr->GetHotKeyConfig()->GetChangeModeHotKey(),0},
		{theConfigMgr->GetHotKeyConfig()->GetLrcWindowHotKey(),1},
		{theConfigMgr->GetHotKeyConfig()->GetPlayControlHotKey(),2},
		{theConfigMgr->GetHotKeyConfig()->GetPlayPreHotKey(),3},
		{theConfigMgr->GetHotKeyConfig()->GetPlayNextHotKey(),4},
		{theConfigMgr->GetHotKeyConfig()->GetUpVolHotKey(),5},
		{theConfigMgr->GetHotKeyConfig()->GetDownVolHotKey(),6}
	};
	UINT wVirtualKeyCode,wModifiers,wFlag;
	for (int i=0;i<sizeof(hk)/sizeof(hk[0]);++i)
	{
		wVirtualKeyCode = LOBYTE(LOWORD(hk[i].dwKey));
		wModifiers = CSys::HKF2SDK( HIBYTE(LOWORD(hk[i].dwKey)));
		if ((UINT)LOWORD(lParam) == wModifiers && (UINT)HIWORD(lParam) == wVirtualKeyCode)
		{
			wFlag = hk[i].wFlag;
			break;
		}
	}
	switch(wFlag)
	{
	case 0: // Ctrl + Alt + Q
		{
			if(IsIconic() || !IsWindowVisible())
			{
				ShowWindow();
				ForegroundWindow();
				sLog("[HotKey:Ctrl + Alt + Q][ShowWindow]");
			}
			else
			{
				SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
				ShowWindow(false);
				sLog("[HotKey:Ctrl + Alt + Q][SC_MINIMIZE]");
			}
		}
		break;
	case 1: // Ctrl + Alt + D
		{
// 			CCheckBoxUI* pUI = NULL;
// 			FindControl(pUI,varMainWindow::kExpandMusicLib);
// 			ASSERT(pUI);
// 			bool bIsShowed = pUI->IsSelected();
// 			pUI->Selected(!bIsShowed);
			BOOL isLrcVisible = GetDesktopLrcWin()->IsWindowVisible();
			ShowDesktopLrcWin(!isLrcVisible);
			sLog("[HotKey:Ctrl + Alt + D][ShowDesktopLrcWin(%d)]",!isLrcVisible);
		}
		break;
	case 2: // Ctrl + Alt + F5
		{
			bool bParam = (thePlayer->GetStatus()!=em_play) ;
			this->PlayOrPause(bParam);
			sLog("[HotKey:Ctrl + Alt + F5][PlayOrPause(%d)]",bParam);
		}
		break;
	case 3: // Ctrl + Alt + ←
		this->PrevSong();
		sLog("[HotKey:Ctrl + Alt + ←][PrevSong]");
		break;
	case 4: // Ctrl + Alt + → 
		this->NextSong();
		sLog("[HotKey:Ctrl + Alt + →][NextSong]");
		break;
	case 5: // Ctrl + Alt + ↑
		{
			int nCurVol = _ui._pVolumeSlider->GetValue()+10;
			if (nCurVol>100)
			{
				nCurVol = 100;
			}
			_ui._pVolumeSlider->SetValue(nCurVol);
			GetPaintMgr()->SendNotify(_ui._pVolumeSlider,kValueChanged);
			sLog("[HotKey:Ctrl + Alt + ↑][UpVolume(%d)]",nCurVol);
		}
		break;
	case 6: // Ctrl + Alt + ↓
		{
			int nCurVol = _ui._pVolumeSlider->GetValue()-10;
			if (nCurVol<0)
			{
				nCurVol = 0;
			}
			_ui._pVolumeSlider->SetValue(nCurVol);
			GetPaintMgr()->SendNotify(_ui._pVolumeSlider,kValueChanged);
			sLog("[HotKey:Ctrl + Alt + ↓][DownVolume(%d)]",nCurVol);
		}
		break;
	}
	return 0;
}
//-------------------------------------------------------------------
LRESULT YMainWindow::OnHttpfileDownload(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg != WM_HTTP_FILE_DOWNLOAD) 
		return 0;
	if(lParam == WP_NOTHING)
		return 0;
	else if (lParam == WP_LOAD_FINISH)
	{
		ShowSongInfo(thePlayListMgr->GetCurSong());
	}
	return 0;
}
//-------------------------------------------------------------------
void YMainWindow::CallbackPlayer(void *data, UINT uMsg)
{
	CAVPlayer *pAVPlayer = (CAVPlayer *) data;

	if (pAVPlayer)
	{
		HWND hWnd = pAVPlayer->GetHWND();

		if (::IsWindow(hWnd)/* && ::IsWindow(::GetParent(hWnd))*/)
		{
			::PostMessage(::IsWindow(::GetParent(hWnd)) ? ::GetParent(hWnd) : hWnd, uMsg, (WPARAM)data, 0);
		}
	}
}
//-------------------------------------------------------------------
void YMainWindow::CallbackPlaying(void *data)
{
	CallbackPlayer(data, WM_USER_PLAYING);
}
//-------------------------------------------------------------------
void YMainWindow::CallbackPosChanged(void *data)
{
	CallbackPlayer(data, WM_USER_POS_CHANGED);
}
//-------------------------------------------------------------------
void YMainWindow::CallbackEndReached(void *data)
{
	CallbackPlayer(data, WM_USER_END_REACHED);
}
//-------------------------------------------------------------------
int YMainWindow::OnMenuCommandPlayList(MenuCmdMsg* pMsg)
{
	if (pMsg->sItemName.Compare(varPlayListMenu::kPlay)==0)
	{
		if(_ui._pSongList->GetCount()==0) return 0;
		YPlayListItemUI* pListItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(_ui._pPlayList->GetCurSel()));
		if (!thePlayListMgr->GetCurList()||pListItem->GetPlayList()->GetLocalId()!=thePlayListMgr->GetCurList()->GetLocalId())
		{
			thePlayListMgr->ClearRand();
		}
		thePlayListMgr->SetCurList(pListItem->GetPlayList());
		thePlayListMgr->SetCurSong(spSongInfoT(NULL));
		thePlayer->Stop();
		PlayOrPause(true);
	}
	else if (pMsg->sItemName.Compare(varPlayListMenu::kNewList)==0)
	{
		OnAddPlayList();
	}
	else if (pMsg->sItemName.Compare(varPlayListMenu::kRemove)==0)
	{
		if (DuiLib::MsgBox(m_hWnd,_T("您确认移处该列表？\r\n点击确定\"继续\",否则点击\"取消\""),_T("警告"),MBT_WARN|MBBT_CHECKBOX|MBBT_OKCANCLE))
		{
			YPlayListItemUI* plistItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(_ui._pPlayList->GetCurSel()));
			ASSERT(plistItem);
			//当前列表正在播放
			if (thePlayListMgr->GetCurList()&&thePlayListMgr->GetCurList()->GetLocalId()==plistItem->GetPlayList()->GetLocalId())
			{
				Stop();
			}
			theDbMgr->GetUserDB()->RemovePlayList(plistItem->GetPlayList()->GetLocalId());
			thePlayListMgr->RemoveList(plistItem->GetPlayList());
			_ui._pPlayList->Remove(plistItem);

			ShowInfo(_T("成功移除播放列表"));
			GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,2000);
		}
	}
	else if (pMsg->sItemName.Compare(varPlayListMenu::kRename)==0)
	{
		YPlayListItemUI* plistItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(_ui._pPlayList->GetCurSel()));
		plistItem->ReName(true);
	}
	return 0;
}
int YMainWindow::OnMenuCommandSongList(MenuCmdMsg* pMsg)
{
	if (pMsg->sItemName.Compare(varSongListMenu::kAddFile)==0||pMsg->sItemName.Compare(varSongListMenu::kAddFolder)==0)
	{//添加
		_bOpenFile = (pMsg->sItemName.Compare(varSongListMenu::kAddFile)==0);
	
		CreateAddSongThread(_bOpenFile);
	}
	else if (pMsg->sItemName.Compare(varSongListMenu::kPlay)==0)
	{//播放
		int iIndex = _ui._pSongList->GetCurSel();
		if(iIndex<0) return 0;
		YSongListItemUI* pSongItem = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(iIndex));
		if (!thePlayListMgr->GetCurList()||_ui._pSongList->GetTag()!=thePlayListMgr->GetCurList()->GetLocalId())
		{
			thePlayListMgr->ClearRand();
		}
		thePlayListMgr->SetCurList(spPlayListInfoT(NULL));
		thePlayListMgr->SetCurSong(pSongItem->GetSong());
		this->Stop();
		PlayOrPause(true);	
	}
	else if (pMsg->sItemName.Compare(varSongListMenu::kProperty)==0)
	{//属性
		if(_ui._pSongList->GetCount()==0) return 0;
		int iIndex = _ui._pSongList->GetCurSel();
		if(iIndex<0) return 0;
		YSongListItemUI* pSongItem = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(iIndex));
		OnOpenProperty(pSongItem->GetSong());
	}
	else if (pMsg->sItemName.Compare(varSongListMenu::kOpenDir)==0)
	{//目录
		if(_ui._pSongList->GetCount()==0) return 0;
		int iIndex = _ui._pSongList->GetCurSel();
		if(iIndex<0) return 0;
		YSongListItemUI* pSongItem = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(iIndex));
		OnOpenFolderPath(pSongItem->GetSong()->GetLocalPath());
	}
	else if (pMsg->sItemName.Compare(varSongListMenu::kClear)==0)
	{//清空列表			
		if (DuiLib::MsgBox(m_hWnd,_T("您确认清空列表？\r\n点击\"<f 1><c #F27D30>确定</c></f>\"继续,否则点击\"<f 1><c #F27D30>取消</c></f>\""),_T("警告"),MBT_WARN|MBBT_CHECKBOX|MBBT_OKCANCLE))
		{			
			unsigned int nPlayListId = (unsigned int)(_ui._pSongList->GetTag());

			if (thePlayListMgr->GetCurList()&&thePlayListMgr->GetCurList()->GetLocalId()==nPlayListId)
			{
				this->Stop();
			}
			theDbMgr->GetUserDB()->RemoveAllPlayListSongs(nPlayListId);

			thePlayListMgr->RemoveAllSongFromList(nPlayListId);
			UpdateSongList(nPlayListId);
			ShowInfo(_T("成功清空列表"));
			GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,2000);
		}
	}
	else if (pMsg->sItemName.Compare(varSongListMenu::kRemove)==0)
	{//移除
		int nSelCnt = _ui._pSongList->GetSelCount();
		if(nSelCnt<=0) return 0;

		unsigned int nPlayListId = (unsigned int)(_ui._pSongList->GetTag());
	
		if (DuiLib::MsgBox(m_hWnd,_T("您准备移除<f 1><c #F27D30>")+mci::ToString(nSelCnt)+_T("</c></f>首歌曲\r\n点击\"<f 1><c #F27D30>确认</c></f>\"移除歌曲,否则请点击\"<f 1><c #F27D30>取消</c></f>\""),_T("警告"),MBT_WARN|MBBT_CHECKBOX|MBBT_OKCANCLE))
		{
			OnRemoveSongsFromList(nPlayListId);
		}	
	}
	else if (pMsg->sItemName.Compare(varSongListMenu::kAddTo)==0)
	{
		int nSelCnt = _ui._pSongList->GetSelCount();
		if(nSelCnt<=0) return 0;
		if (pMsg->nItemTag==EMT_TONEWLIST)
		{//添加到新建列表
			if(OnAddPlayList() >0)
			{
				unsigned int nPlayListId = (unsigned int)(_ui._pSongList->GetTag());
				spPlayListInfoT spList;
				CListUI* pPlayList = _ui._pPlayList;
				YPlayListItemUI* pItem = static_cast<YPlayListItemUI*>(pPlayList->GetItemAt(pPlayList->GetCount()-1));
				if(pItem==NULL)
					return 0;
				unsigned int nNewListId = pItem->GetPlayList()->GetLocalId();
				OnAddSongstoNewList(nPlayListId,nNewListId,false);
			}
		}
		else if(pMsg->nItemTag==EMT_TOOTHERLIST)
		{
			unsigned int nPlayListId = (unsigned int)(_ui._pSongList->GetTag());
			spPlayListInfoT spList;
			ASSERT(theDbMgr->GetUserDB()->GetPlayListByName(pMsg->sItemText.GetData(),spList));
			unsigned int nNewListId = spList->GetLocalId();
			OnAddSongstoNewList(nPlayListId,nNewListId,false);
		}		
	}
	else if (pMsg->sItemName.Compare(varSongListMenu::kMoveTo)==0)
	{
		int nSelCnt = _ui._pSongList->GetSelCount();
		if(nSelCnt<=0) return 0;
		if (pMsg->nItemTag==EMT_TONEWLIST)
		{//添加到新建列表
			if(OnAddPlayList() >0)
			{
				unsigned int nPlayListId = (unsigned int)(_ui._pSongList->GetTag());
				spPlayListInfoT spList;
				CListUI* pPlayList = _ui._pPlayList;
				YPlayListItemUI* pItem = static_cast<YPlayListItemUI*>(pPlayList->GetItemAt(pPlayList->GetCount()-1));
				if(pItem==NULL)
					return 0;
				unsigned int nNewListId = pItem->GetPlayList()->GetLocalId();
				OnAddSongstoNewList(nPlayListId,nNewListId,true);
			}
		}
		else if(pMsg->nItemTag==EMT_TOOTHERLIST)
		{
			unsigned int nPlayListId = (unsigned int)(_ui._pSongList->GetTag());
			spPlayListInfoT spList;
			ASSERT(theDbMgr->GetUserDB()->GetPlayListByName(pMsg->sItemText.GetData(),spList));
			unsigned int nNewListId = spList->GetLocalId();
			OnAddSongstoNewList(nPlayListId,nNewListId,true);
		}		
	}
	else if (pMsg->sItemName.Compare(varSongListMenu::kShare)==0)
	{//分享
		OnShowShareWindow();
		CShareWin* pShare = CShareWin::Instance();
		ASSERT(pShare);
		if(!pShare->IsWindow()) return 0;
		if(_ui._pSongList->GetCount()==0) return 0;
		int iIndex = _ui._pSongList->GetCurSel();
		if(iIndex<0) return 0;
		YSongListItemUI* pSongItem = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(iIndex));
		pShare->Share(pSongItem->GetSong());
	}
	else if (pMsg->sItemName.Compare(varSongListMenu::kModify)==0)
	{
		if(_ui._pSongList->GetCount()==0) return 0;
		int iIndex = _ui._pSongList->GetCurSel();
		if(iIndex<0) return 0;
		YSongListItemUI* pSongItem = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(iIndex));
		int iList = _ui._pPlayList->GetCurSel();
		if(iList<0) return 0;
		YPlayListItemUI* pListItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(iList));
		TagModifyWin::Instance()->InitWnd(pSongItem->GetSong(),pListItem->GetPlayList()->GetLocalId());
	}
	else if (pMsg->sItemName.Compare(varSongListMenu::kDumplicate)==0)
	{
		unsigned int nPlayListId = (unsigned int)(_ui._pSongList->GetTag());
		OnRemoveDumplicate(nPlayListId);
	}
	return 0;
}
int YMainWindow::OnMenuCommadnPlayMode(MenuCmdMsg* pMsg)
{
	CTabLayoutUI* pMode = NULL;
	FindControl(pMode,varMainWindow::kModeLayout);
	if (pMsg->sItemName.Compare(varPlayModeMenu::kListOrder)==0)
	{
		pMode->SelectItem(EM_MODE::EM_LIST_ORDER);

		thePlayListMgr->SetPlayMode(EM_LIST_ORDER);

		theConfigMgr->GetNormalConfig()->SetPlayMode(EM_MODE::EM_LIST_ORDER);
	}
	else if (pMsg->sItemName.Compare(varPlayModeMenu::kListLoop)==0)
	{
		pMode->SelectItem(EM_MODE::EM_LIST_LOOP);

		thePlayListMgr->SetPlayMode(EM_LIST_LOOP);

		theConfigMgr->GetNormalConfig()->SetPlayMode(EM_MODE::EM_LIST_LOOP);
	}
// 	else if (pMsg->sItemName.Compare(varPlayModeMenu::kSingle)==0)
// 	{
// 		pMode->SelectItem(EM_MODE::EM_SINGLE);
// 
// 		thePlayListMgr->SetPlayMode(EM_SINGLE);
// 
// 		theConfigMgr->GetNormalConfig()->SetPlayMode(EM_MODE::EM_SINGLE);
// 	}
	else if (pMsg->sItemName.Compare(varPlayModeMenu::kSingleLoop)==0)
	{
		pMode->SelectItem(EM_MODE::EM_SINGLE_LOOP);

		thePlayListMgr->SetPlayMode(EM_SINGLE_LOOP);

		theConfigMgr->GetNormalConfig()->SetPlayMode(EM_MODE::EM_SINGLE_LOOP);
	}
	else if (pMsg->sItemName.Compare(varPlayModeMenu::kRandom)==0)
	{
		pMode->SelectItem(EM_MODE::EM_RAMDON);

		thePlayListMgr->SetPlayMode(EM_RAMDON);

		theConfigMgr->GetNormalConfig()->SetPlayMode(EM_MODE::EM_RAMDON);
	}
	return 0;
}
int YMainWindow::OnMenuCommandSongItem(MenuCmdMsg* pMsg)
{
	if (pMsg->sItemName.Compare(varSongItemMenu::kPlay)==0)
	{
		int iIndex_Song = _ui._pSongList->GetCurSel();
		if(iIndex_Song<0) return 0;
		YSongListItemUI* pSongItem = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(iIndex_Song));
		//thePlayListMgr内部会根据当前选中的列表来设置需要播放的列表
		//所以这里可以直接设置当前列表为NULL
		thePlayListMgr->SetCurList(spPlayListInfoT(NULL));
		thePlayListMgr->SetCurSong(pSongItem->GetSong());

		thePlayer->Stop();
		PlayOrPause(true);	
	}
	else if (pMsg->sItemName.Compare(varSongItemMenu::kCollect)==0)
	{
		unsigned int nSongIndex = _ui._pSongList->GetCurSel();
		if(nSongIndex<0) return 0;
		unsigned int nPlayListId = (unsigned int)(_ui._pSongList->GetTag());
		OnCollectSongFromList(nPlayListId,nSongIndex);
	}
	else if (pMsg->sItemName.Compare(varSongItemMenu::kShare)==0)
	{
		OnShowShareWindow();

		unsigned int nSongIndex = _ui._pSongList->GetCurSel();
		if(nSongIndex<0) return 0;
		unsigned int nPlayListId = (unsigned int)(_ui._pSongList->GetTag());
		OnShareSongFromList(nPlayListId,nSongIndex);
	}
	else if (pMsg->sItemName.Compare(varSongItemMenu::kRemove)==0)
	{
		unsigned int nSongIndex = _ui._pSongList->GetCurSel();
		if(nSongIndex<0) return 0;
		unsigned int nPlayListId = (unsigned int)(_ui._pSongList->GetTag());

		if (DuiLib::MsgBox(m_hWnd,_T("您准备移除<f 1><c #F27D30>")+mci::ToString(1)+_T("</c></f>首歌曲\r\n点击\"<f 1><c #F27D30>确认</c></f>\"移除歌曲,否则请点击\"<f 1><c #F27D30>取消</c></f>\""),_T("警告"),MBT_WARN|MBBT_CHECKBOX|MBBT_OKCANCLE))
		{
			OnRemoveSongFromList(nPlayListId,nSongIndex);
		}	
	}
	else if (pMsg->sItemName.Compare(varSongItemMenu::kDownLoad)==0)
	{
		
	}
	return 0;
}
int YMainWindow::OnMenuCommandLrcView(MenuCmdMsg* pMsg)
{
	if(0 == pMsg->sItemName.Compare(varLrcViewMenu::kSearchLrc))
	{
		ShowSelectLrcWin();
	}
	else if( 0 == pMsg->sItemName.Compare(varLrcViewMenu::kLrcAdvance))
	{
		theLrcMgr->SetLrcForward();
		CDuiRect rc  = _ui._pLrcView1->GetPos();
		POINT pt = {(rc.left+rc.right)/2-50,(rc.top+rc.bottom)/2};
		CString sInfo;
		sInfo.Format(_T("歌词前进%0.1fs"),theLrcMgr->GetLrcTimeOffset() < 0 ? 0-theLrcMgr->GetLrcTimeOffset():theLrcMgr->GetLrcTimeOffset());
		CInfoWindow::ShowInfoWindow(theMainWin->GetHWND(),pt,sInfo);
	}
	else if ( 0 == pMsg->sItemName.Compare(varLrcViewMenu::kLrcBack))
	{
		theLrcMgr->SetLrcBack();
		CDuiRect rc  =  _ui._pLrcView1->GetPos();
		POINT pt = {(rc.left+rc.right)/2-50,(rc.top+rc.bottom)/2};
		CString sInfo;
		sInfo.Format(_T("歌词退后%0.1fs"),theLrcMgr->GetLrcTimeOffset() < 0 ? 0-theLrcMgr->GetLrcTimeOffset():theLrcMgr->GetLrcTimeOffset());
		CInfoWindow::ShowInfoWindow(theMainWin->GetHWND(),pt,sInfo);
	}
	else if ( 0 == pMsg->sItemName.Compare(varLrcViewMenu::kLrcReset))
	{
		theLrcMgr->ResetLrc();

		CDuiRect rc  = _ui._pLrcView1->GetPos();
		POINT pt = {(rc.left+rc.right)/2-50,(rc.top+rc.bottom)/2};
		CString sInfo;
		sInfo.Format(_T("复位时间戳"));
		CInfoWindow::ShowInfoWindow(theMainWin->GetHWND(),pt,sInfo);
	}
	else if ( 0 == pMsg->sItemName.Compare(varLrcViewMenu::kLrcEdit))
	{
		spSongInfoT spSong = thePlayListMgr->GetCurSong();
		if (spSong)
		{
			if(spSong->GetLrcLocalPath().IsEmpty())
			{
				CDuiRect rc  = _ui._pLrcView1->GetPos();
				POINT pt = {(rc.left+rc.right)/2-120,(rc.top+rc.bottom)/2};
				CString sInfo;
				sInfo.Format(_T("歌曲赞无歌词,请下载或在关联本地歌词再进行编辑"));
				CInfoWindow::ShowInfoWindow(theMainWin->GetHWND(),pt,sInfo);
				return 0;
			}
			SHELLEXECUTEINFO shexecinfo = {0};
			shexecinfo.cbSize = sizeof(SHELLEXECUTEINFO);
			shexecinfo.fMask = SEE_MASK_INVOKEIDLIST;
			shexecinfo.hwnd = theMainWin->GetHWND();
			shexecinfo.lpVerb = L"open";
			shexecinfo.lpFile = spSong->GetLrcLocalPath();
			shexecinfo.lpParameters = NULL;
			shexecinfo.lpDirectory = NULL;
			shexecinfo.nShow = SW_SHOW;
			shexecinfo.hInstApp = CPaintManagerUI::GetInstance();
			ShellExecuteEx(&shexecinfo);
		}
	}
	else if ( 0 == pMsg->sItemName.Compare(varLrcViewMenu::kLrcFolder))
	{
		spSongInfoT spSong = thePlayListMgr->GetCurSong();
		if (spSong)
		{
			CString sLrc = spSong->GetLrcLocalPath();
			OnOpenFolderPath(sLrc);
		}
	}
	else if ( 0 == pMsg->sItemName.Compare(varLrcViewMenu::kLrcLocal))
	{
		CLrcSelectWnd::Instance()->ReleatLocalLrc();
	}
	else if ( 0 == pMsg->sItemName.Compare(varLrcViewMenu::kLrcSetting))
	{
		CSettingWindow* pSettingWnd = CSettingWindow::Instance();
		assert(pSettingWnd);
		if(!pSettingWnd->IsWindow())
			pSettingWnd->Create(m_hWnd,_T("设置"),UI_WNDSTYLE_DIALOG,0);
		pSettingWnd->ShowWindow(true);
		pSettingWnd->showPage(CSettingWindow::emTabPages::LrcPanelPage);
	}
	else if ( 0 == pMsg->sItemName.Compare(varLrcViewMenu::kLrcDesk))
	{
		ShowDesktopLrcWin();
		_ui._pTabLayout->SelectItem(0);
	}
	return 0;
}
int YMainWindow::OnMenuCommandMainMenu(MenuCmdMsg* pMsg)
{
	if( 0 == pMsg->sItemName.Compare(varMainMenu::kExit))
	{
		sLog("PostQuitMessage");
		this->Stop();
		PostMessage(WM_QUIT,WPARAM(0),LPARAM(0));
	}
	else if(0 == pMsg->sItemName.Compare(varMainMenu::kSetting))
	{
		ShowSettingWin();
	}
	else if (0 == pMsg->sItemName.Compare(varMainMenu::kLogin))
	{
		OnShowLoginWindow();
	}
	else if (0 == pMsg->sItemName.Compare(varMainMenu::kLogout))
	{
		theRuntimeState->SetUserInfo(spLogonUserInfoT(NULL));
	}
	else if (0 == pMsg->sItemName.Compare(varMainMenu::kKeepTop))
	{
		bool isTop = FALSE;
		
		theDbMgr->GetGlobalDBase()->GetDictValue(ConfigEX::KEEPMOST_TOP,isTop);
		theDbMgr->GetGlobalDBase()->DeleteDict(ConfigEX::KEEPMOST_TOP);
		theDbMgr->GetGlobalDBase()->InsertDict(ConfigEX::KEEPMOST_TOP,!isTop);
		
		SetWindowPos(!isTop?HWND_TOPMOST:HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	}
	else if (0 == pMsg->sItemName.Compare(varMainMenu::kDeskLrc))
	{
		BOOL isLrcVisible = GetDesktopLrcWin()->IsWindowVisible();
		ShowDesktopLrcWin(!isLrcVisible);
	}
	else if (0 == pMsg->sItemName.Compare(varMainMenu::kLockLrc))
	{
		bool isLrcLock = GetDesktopLrcWin()->IsLock();
		GetDesktopLrcWin()->LockLrc(!isLrcLock);
		if(!isLrcLock)ShowTrayTooltip(_T("桌面歌词已锁定,您可以通过右键解锁！"),_T("逸听"));
	}
	else if (0 == pMsg->sItemName.Compare(varMainMenu::kSkinWin))
	{
		ShowSkinWin();
	}
	else if ( 0 == pMsg->sItemName.Compare(varPlayModeMenu::kListLoop)
		|| 0==pMsg->sItemName.Compare(varPlayModeMenu::kListOrder)
		|| 0==pMsg->sItemName.Compare(varPlayModeMenu::kSingleLoop)
		|| 0==pMsg->sItemName.Compare(varPlayModeMenu::kRandom))
	{
		OnMenuCommadnPlayMode(pMsg);
	}
// 	else if ( 0 == pMsg->sItemName.Compare() )
// 	{
// 	}
	return 0;
}
//-------------------------------------------------------------------

/*******************初始化********************************/
void YMainWindow::InitVariable()
{
	m_pMusicLibWnd = NULL;
	ZeroMemory(&_ui,sizeof(_ui));
	FindControl(_ui._pPlayList,varMainWindow::kPlayList);
	FindControl(_ui._pSongList,varMainWindow::kSongList);
	FindControl(_ui._pTabLayout,varMainWindow::kTabLayout);
	FindControl(_ui._pVolumeBtn,varMainWindow::kVolume);
	FindControl(_ui._pVolumeSlider,varMainWindow::kVolumeslider);
	FindControl(_ui._pVolumeTab,varMainWindow::kVolumetab);
	ASSERT(_ui._pPlayList);ASSERT(_ui._pSongList);ASSERT(_ui._pTabLayout);
	ASSERT(_ui._pVolumeBtn);ASSERT(_ui._pVolumeSlider);ASSERT(_ui._pVolumeTab);
	_ui._pVolumeBtn->OnEvent += MakeDelegate(this,&YMainWindow::OnVolumeLayoutUIEvent);
	_ui._pVolumeSlider->OnEvent += MakeDelegate(this,&YMainWindow::OnVolumeLayoutUIEvent);
	BOOL bVolumeOff = FALSE;
	bVolumeOff = theConfigMgr->GetNormalConfig()->IsVolumeOff();
	int nVol = theConfigMgr->GetNormalConfig()->GetVolume();
	if(bVolumeOff)
	{
		_ui._pVolumeBtn->SetToolTip(_T("取消静音"));
		_ui._pVolumeBtn->Selected(true);
		thePlayer->Volume(0);
	}
	else 
	{
		thePlayer->Volume(nVol);
	}
	_ui._pVolumeSlider->SetValue(nVol);
	FindControl(_ui._pTimeUsed,varMainWindow::kTimeUsed);
	FindControl(_ui._pTimeTotal,varMainWindow::kTimeTotal);
	ASSERT(_ui._pTimeUsed);ASSERT(_ui._pTimeTotal);
	FindControl(_ui._pPlayProgress,varMainWindow::kPlayProgress);
	ASSERT(_ui._pPlayProgress);
	FindControl(_ui._pSongName,varMainWindow::kSongName);
	FindControl(_ui._pAlbumName,varMainWindow::kAuthorName);
	ASSERT(_ui._pSongName);ASSERT(_ui._pAlbumName);
	FindControl(_ui._pCollect,varMainWindow::kCollect);
	FindControl(_ui._pShare,varMainWindow::kShare);
	ASSERT(_ui._pCollect&&_ui._pShare);
	FindControl(_ui._pLrcView1,varMainWindow::kLrcView1);
	ASSERT(_ui._pLrcView1);
	FindControl(_ui._pInfoLayout,varMainWindow::kInfoLayout);
	FindControl(_ui._pInfoLabel,varMainWindow::kInfoLabel);
	ASSERT(_ui._pInfoLayout&&_ui._pInfoLabel);
	FindControl(_ui._pCountLabel,varMainWindow::kCount);
	ASSERT(_ui._pCountLabel);
	CTabLayoutUI* pMode = NULL;
	FindControl(pMode,varMainWindow::kModeLayout);
	int nMode = theConfigMgr->GetNormalConfig()->GetPlayMode();
	pMode->SetTag(nMode),pMode->SelectItem(nMode);
	thePlayListMgr->SetPlayMode(nMode);
	FindControl(_ui._pLrcViewTab,varMainWindow::kLrcViewTab);
	ASSERT(_ui._pLrcViewTab);

	ShowSongInfo(thePlayListMgr->GetCurSong());
	OnSongStatusChanged(!!thePlayListMgr->GetCurSong());
	UpdatePlayList();
}
//-------------------------------------------------------------------
bool YMainWindow::OnVolumeLayoutUIEvent(void* pParam)
{
	const TEventUI* pEvent    = static_cast<TEventUI*>(pParam);
	CControlUI* pSender = pEvent->pSender;
	if(pSender == _ui._pVolumeBtn)
	{
		switch(pEvent->Type)
		{
		case UIEVENT_MOUSEENTER:pSender->SetTag(1); _ui._pVolumeTab->SelectItem(1); break;
		case UIEVENT_MOUSELEAVE:
			{
				pSender->SetTag(0); 
				if( 1 != _ui._pVolumeSlider->GetTag())
					_ui._pVolumeTab->SelectItem(0);
			}
			break;
		}
	}
	else if(pSender == _ui._pVolumeSlider)
	{
		switch(pEvent->Type)
		{
		case UIEVENT_MOUSEENTER: pSender->SetTag(1); _ui._pVolumeTab->SelectItem(1); break;
		case UIEVENT_MOUSELEAVE:
			{
				pSender->SetTag(0);
				if(1 != _ui._pVolumeBtn->GetTag())
					_ui._pVolumeTab->SelectItem(0);
			}
			break;
		}
	}
	return true;
}
//-------------------------------------------------------------------
void YMainWindow::OnPlayListChanged()
{

}
void YMainWindow::OnCurPlayListChanged()
{

}
void YMainWindow::OnPlayListSongChanged()
{

}
void YMainWindow::OnNetChanged()
{

}
void YMainWindow::OnUserChanged()
{
	CControlUI* pUserName = NULL;
	CTabLayoutUI* pSwitch = NULL;
	FindControl(pSwitch,varMainWindow::kUser_LogSwitch);
	FindControl(pUserName,varMainWindow::kUserName);
	ASSERT(pSwitch&&pUserName);
	if(pSwitch&&pUserName)
	{
		spUserContextT pContext = theRuntimeState->GetCurrentContext(NULL);
		spLogonUserInfoT theUser(pContext->GetUser());
		if(theUser)
			pUserName->SetText(theUser->sNickName),pSwitch->SelectItem(1);
		else 
			pSwitch->SelectItem(0);
	}
	this->Stop();

	CTabLayoutUI* pMode = NULL;
	FindControl(pMode,varMainWindow::kModeLayout);
	int nMode = theConfigMgr->GetNormalConfig()->GetPlayMode();
	//	thePlayCenter->SetPlayMode(nMode);
	pMode->SetTag(nMode),pMode->SelectItem(nMode);
	thePlayListMgr->SetPlayMode(nMode);
}
void YMainWindow::OnVersionExpired()
{

}
void YMainWindow::OnSongStatusChanged(bool bPlay)
{
	sLog("YMainWindow::OnSongStatusChanged(%d)",bPlay);
	CFadeButtonUI* pPlay = NULL;
	CFadeButtonUI* pPause = NULL;
	FindControl(pPlay,varMainWindow::kPlay);
	FindControl(pPause,varMainWindow::kPause);
	ASSERT(pPause&&pPlay);
	pPlay->SetVisible(!bPlay);
	pPause->SetVisible(bPlay);

	spTaskbarListT theTaskbarList(GetTaskbarList());
	if(!!theTaskbarList)
	{
		THUMBBUTTON button;
		button.iId = 1;
		button.dwMask = THB_BITMAP|THB_TOOLTIP;
		button.iBitmap = (bPlay ? 1 : 2);
		bPlay ?  lstrcpy(button.szTip,_T("暂停")) : lstrcpy(button.szTip,_T("播放"));

		theTaskbarList->ThumbBarUpdateButtons(GetHWND(),1,&button);
	}
	CDesktopLrcWindow* theWin(GetDesktopLrcWin());
	if(theWin)
		theWin->GetLrcToolBar()->SetStatePlay(bPlay);
}
void YMainWindow::OnSkinChanged()
{

}
//-------------------------------------------------------------------
void YMainWindow::UpdateDowningProgress()
{

}
void YMainWindow::UpdatePlayingProgress()
{
// 	_ui._pTimeUsed->SetText( thePlayCenter->GetTimeUsed());
// 	_ui._pPlayProgress->SetValue(thePlayCenter->GetTimeUsed2());

	CDuiString  strTime;
	struct tm   tmCurrent,tmTotal;
	time_t      timeCurrent = thePlayer->GetTime() / 1000;
	 time_t      timeTotal   = thePlayer->GetTotalTime() / 1000;
	TCHAR       szCurrent[MAX_PATH],szTotal[MAX_PATH];

	gmtime_s(&tmCurrent, &timeCurrent);
	gmtime_s(&tmTotal, &timeTotal);

	_tcsftime(szCurrent, MAX_PATH, _T("%X"), &tmCurrent);
	_tcsftime(szTotal,   MAX_PATH, _T("%X"), &tmTotal);
	strTime.Format(_T("%s"), szCurrent);
	_ui._pTimeUsed->SetText(strTime);
	strTime.Format(_T("%s"), szTotal);
	_ui._pTimeTotal->SetText(strTime);
	_ui._pPlayProgress->SetValue(thePlayer->GetPos());
	
}
void YMainWindow::UpdateBufferingProgress()
{

}
//-------------------------------------------------------------------
void YMainWindow::ShowInfo(CString sInfo)
{
	_ui._pInfoLayout->SetVisible(true);
	_ui._pInfoLabel->SetText(sInfo);
}
//-------------------------------------------------------------------
void YMainWindow::ShowSongInfo(spSongInfoT song)
{
	sLog("YMainWindow::ShowSongInfo");
	if (song)
	{
		_ui._pSongName->SetText(song->GetSongName()+_T(" - ")+song->GetArtistName());
		_ui._pAlbumName->SetText(song->GetAlbumName());
		_ui._pSongName->SetToolTip(song->GetSongName()+_T(" - ")+song->GetArtistName());
		_ui._pAlbumName->SetToolTip(song->GetAlbumName());

		_ui._pPlayProgress->SetMouseEnabled(true);
		_ui._pPlayProgress->SetEnabled(true);
		_ui._pCollect->SetVisible(true);
		_ui._pShare->SetVisible(true);
		//专辑图片解析
		CString imagePath = theCacheMgr->GetImagePath();
		CString songPath ;
		CString file_name;
		if(song->IsLocal())
		{
			songPath = song->GetLocalPath();
			file_name = mci::rip_file_name(songPath,false)+_T(".jpg");
		}
		else 
		{
			songPath = song->GetSongName() + _T(" - ") + song->GetArtistName();
			file_name = songPath + _T(".jpg");
		}
		CString szImage = imagePath + file_name;
		CAlbumButtonUI* pLogo = NULL;
		FindControl(pLogo,varMainWindow::kAlbumLogo);
		assert(pLogo);
		if(mci::file_exist(szImage))
		{//图片已经存在本地
			CString sImage;
			int right_bottom = pLogo->GetPos().bottom-pLogo->GetPos().top - 8;
			sImage.Format(_T("file='%s' dest='8,8,%d,%d' corner='5,5 5,5'"),szImage.GetString(),right_bottom,right_bottom);
			pLogo->SetAttribute(_T("normalimage"),sImage);

			sImage.Format(_T("file='%s' mask='#FFFF00FF'"),szImage.GetString());
			_ui._pLrcViewTab->SetBkImage(sImage);
		}
		else
		{
			AudioTagInfo taginfo;
			taginfo.init(mci::unicode_2_ansi(song->GetLocalPath()));
			std::string savename = mci::unicode_2_ansi(szImage);
			if (song->IsLocal()&&taginfo.save_pic2file(savename))
			{//解析文件获取专辑图片
				CString sImage;
				int right_bottom = pLogo->GetPos().bottom-pLogo->GetPos().top - 8;
				sImage.Format(_T("file='%s' dest='8,8,%d,%d' corner='5,5,5,5'"),szImage.GetString(),right_bottom,right_bottom);
				pLogo->SetAttribute(_T("normalimage"),sImage);
				 
				sImage.Format(_T("file='%s' mask='#FFFF00FF'"),szImage.GetString());
				_ui._pLrcViewTab->SetBkImage(sImage);
			}
			else if (!song->GetCoverUrl().IsEmpty())
			{
				CString songPath ;
				CString file_name;

				songPath = song->GetSongName() + _T(" - ") + song->GetArtistName();
				file_name = songPath + _T(".jpg");

				TCHAR validChar[] = _T("/\\:*?\"<>|");
				for (int i=0;i<sizeof(validChar)/sizeof(validChar[0]);++i)
				{
					file_name.Replace(validChar[i],_T(''));
				}
				CString szImage = imagePath + file_name;
				em_utility::down_http_file::down_task new_task;
				new_task.h_notify_wnd = theMainWin->GetHWND();
				new_task.s_url = song->GetCoverUrl();
				new_task.s_local_file_down = szImage;
				new_task.n_notify_msg = WM_HTTP_FILE_DOWNLOAD;
				new_task.n_wparam = WP_LRCLOAD;

				theHttpDownerMgr->add_task(new_task);
			}
			else
			{
				CString sImage;
				int right_bottom = pLogo->GetPos().bottom-pLogo->GetPos().top - 8;
				sImage.Format(_T("file='YTing/logo.png' dest='8,8,%d,%d' corner='5,5,5,5'"),right_bottom,right_bottom);
				if(pLogo) pLogo->SetAttribute(_T("normalimage"),sImage);

				sImage.Format(_T("file='YTing/Main/lyric-bg.png' source='0,0,320,200' corner='2,25,2,2' mask='#FFFF00FF'"));
				_ui._pLrcViewTab->SetBkImage(_T(""));
			}
			
		}
	}
	else 
	{
		_ui._pSongName->SetText(_T("逸听音乐,最好听的音乐"));
		_ui._pAlbumName->SetText(_T("听逸听,感受声音魅丽"));
		_ui._pSongName->SetToolTip(_T("逸听音乐,最好听的音乐"));
		_ui._pAlbumName->SetToolTip(_T("听逸听,感受声音魅丽"));
		_ui._pTimeTotal->SetText(_T("00:00"));
		_ui._pTimeUsed->SetText(_T("00:00"));
//		_ui._pPlayProgress->SetMinValue(0);
		_ui._pPlayProgress->SetValue(0);
		_ui._pPlayProgress->SetMouseEnabled(false);
		_ui._pPlayProgress->SetEnabled(false);
		_ui._pCollect->SetVisible(false);
		_ui._pShare->SetVisible(false);
		
		OnSongStatusChanged(false);
//		thePlayCenter->SetPlayIndex(0);
		_ui._pSongList->NeedUpdate();

		CAlbumButtonUI* pLogo = NULL;
		FindControl(pLogo,varMainWindow::kAlbumLogo);
		ASSERT(pLogo);
		CString sImage;
		int right_bottom = 72;
		sImage.Format(_T("file='YTing/logo.png' dest='8,8,%d,%d' corner='5,5,5,5'"),right_bottom,right_bottom);
		if(pLogo) pLogo->SetAttribute(_T("normalimage"),sImage);

		sImage.Format(_T("file='YTing/Main/lyric-bg.png' source='0,0,320,200' corner='2,25,2,2' mask='#FFFF00FF'"));
		_ui._pLrcViewTab->SetBkImage(_T(""));
	}
	// 读取歌词
	theLrcMgr->GetLrcReader(song);
	ShowTrayInfo(song);
}
//-------------------------------------------------------------------
void YMainWindow::LoadSongLrc()
{
	sLog("YMainWindow::LoadSongLrc");
//	_ui._pLrcView1->LrcChanged(thePlayCenter->GetCurrentPlayingSong());
	_ui._pLrcView1->LrcChanged(thePlayListMgr->GetCurSong());
	//桌面歌词更新
	if(m_pDesktopLrcWnd&&m_pDesktopLrcWnd->IsWindow())
		m_pDesktopLrcWnd->LrcChange();
}
//-------------------------------------------------------------------
void YMainWindow::UpdateUserName()
{

}
void YMainWindow::UpdatePlayList()
{
	sLog("YMainWindow::UpdatePlayList");
	CListUI* pPlayLists = _ui._pPlayList;
	ASSERT(pPlayLists);
	pPlayLists->RemoveAll();
	
	PlayListMgr::playListSongMapT listMap = thePlayListMgr->GetAllPlayList();
	//加载列表
	for (PlayListMgr::playListSongMapIterT it = listMap.begin(),iend = listMap.end();it != iend;++it)
	{
		spPlayListInfoT spListInfo ;
		theDbMgr->GetUserDB()->GetPlayList(spListInfo,it->first);

 		YPlayListItemUI* pListItem = new YPlayListItemUI();
 		ASSERT(pListItem);
 		pListItem->SetPlayList(spListInfo);
 		pPlayLists->Add(pListItem);
	}
	if(pPlayLists->GetCount()==0) return ;
	pPlayLists->SelectItem(0);
	YPlayListItemUI* pListItem = static_cast<YPlayListItemUI*>(pPlayLists->GetItemAt(0));
	_ui._pSongList->SetTag(pListItem->GetPlayList()->GetLocalId());
	//加载列表对应的歌曲
	UpdateSongList(_ui._pSongList->GetTag());
}
void YMainWindow::UpdateSongList(int nPlayListId)
{
	sLog("YMainWindow::UpdateSongList(%d)",nPlayListId);
	CListUI* pSongLists = _ui._pSongList;
	ASSERT(pSongLists);
	pSongLists->RemoveAll();
	
	PlayListMgr::playListSongMapT listMap = thePlayListMgr->GetAllPlayList();
	PlayListMgr::playListSongMapIterT listIt = listMap.find(nPlayListId);
	if (listIt==listMap.end())
	{
		return ;
	}
	PlayListMgr::songsArrayT& songList = listIt->second;
	for (PlayListMgr::songsArrayIterT it=songList.begin(),iend = songList.end();it!=iend;++it)
	{
		CSongInfo songInfo = *it;
		spSongInfoT spSongInfo = spSongInfoT(new CSongInfo(songInfo));
		YSongListItemUI* spSongListItem = new YSongListItemUI;
		spSongListItem->SetSong(spSongInfo);
		pSongLists->Add(spSongListItem);
	}
	
	_ui._pCountLabel->SetText(_T("共")+mci::ToString(songList.size())+_T("首歌曲"));

	if(0==static_cast<CVerticalLayoutUI*>(pSongLists->GetParent())->GetItemAt(0)->GetUserData().Compare(_T("favorite-login")))
		static_cast<CVerticalLayoutUI*>(pSongLists->GetParent())->RemoveAt(0);
	int iIndex = _ui._pPlayList->GetCurSel();
	if(iIndex<0) return ;
	YPlayListItemUI* pListItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(iIndex));
	if (pListItem&&pListItem->GetPlayList()->GetListType()==CPlayListInfo::plt_sys
		&&pListItem->GetPlayList()->GetName().Compare(_T("我的收藏"))==0
		&&pListItem->GetPlayList()->GetLocalId()==nPlayListId)
	{
		UpdateFavList();
	}
}
void YMainWindow::UpdateFavList()
{
	CListUI* pSongLists = _ui._pSongList;
	ASSERT(pSongLists);

	if(0==static_cast<CVerticalLayoutUI*>(pSongLists->GetParent())->GetItemAt(0)->GetUserData().Compare(_T("favorite-login")))
		static_cast<CVerticalLayoutUI*>(pSongLists->GetParent())->RemoveAt(0);

	if(theRuntimeState->GetLogUserInfo()==NULL)
	{//用户未登录
		CVerticalLayoutUI* pListElement = NULL;
		CDialogBuilder dlgBuilder;
		if( !dlgBuilder.GetMarkup()->IsValid() ) {
			pListElement = static_cast<CVerticalLayoutUI*>(dlgBuilder.Create(_T("favorite-login.xml"), (UINT)0, NULL, GetPaintMgr()));
		}
		else {
			pListElement = static_cast<CVerticalLayoutUI*>(dlgBuilder.Create((UINT)0,GetPaintMgr()));
		}
		if (pListElement == NULL)
			return ;
		ASSERT(pListElement);
		pListElement->SetUserData(_T("favorite-login"));
		static_cast<CVerticalLayoutUI*>(pSongLists->GetParent())->AddAt(pListElement,0);
	}
	else//取得用户收藏列表
	{

	}
}
//-------------------------------------------------------------------
void YMainWindow::CreateAddSongThread(bool bOpenFile)
{
	_bOpenFile = bOpenFile;
	DWORD dwID = 0;
	HANDLE hThread = CreateThread(NULL,0,YMainWindow::_ThreadProc_AddSong,this,0,&dwID);
	if(hThread)
		CloseHandle(hThread);
}
void YMainWindow::_AddSongProc(void* param)
{
	CFileMgr::filesArrayT files;
	if(_bOpenFile)
		CFileMgr::Instance()->OpenFile(m_hWnd,files);
	else
		CFileMgr::Instance()->OpenFolder(m_hWnd,files);
	
	unsigned int u_listId = (unsigned int)(_ui._pSongList->GetTag());
	assert(u_listId>0);
	_AddSongProc2(u_listId,files);
}
void YMainWindow::_AddSongProc2(unsigned int u_listId,CFileMgr::filesArrayT files)
{
	CListUI* pSongLists = _ui._pSongList;
	CListUI* pPlayLists = _ui._pPlayList;
	ASSERT(pSongLists);ASSERT(pPlayLists);
	pSongLists->SetMouseEnabled(false),pSongLists->SetEnabled(false),pSongLists->SetMouseChildEnabled(false);
	pPlayLists->SetMouseEnabled(false),pPlayLists->SetEnabled(false),pPlayLists->SetMouseChildEnabled(false);
	static_cast<CHorizontalLayoutUI*>(GetPaintMgr()->GetRoot())->SetMouseChildEnabled(false);
	CButtonUI* pAddList = NULL;
	FindControl(pAddList,varMainWindow::kAddPlayList);
	pAddList->SetMouseEnabled(false);
	CButtonUI* pSaveAsList = NULL;
	FindControl(pSaveAsList,varMainWindow::kSaveAsList);
	ASSERT(pSaveAsList);
	pSaveAsList->SetMouseEnabled(false);

	size_t nCount = 0;
	if((nCount=files.size())==0)
	{
		ShowInfo(_T("目录下未搜索到歌曲文件"));
		GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,2000);
		pSongLists->SetMouseEnabled(true),pSongLists->SetEnabled(true),pSongLists->SetMouseChildEnabled(true);
		pPlayLists->SetMouseEnabled(true),pPlayLists->SetEnabled(true),pPlayLists->SetMouseChildEnabled(true);
		pAddList->SetMouseEnabled(true);
		pSaveAsList->SetMouseEnabled(true);
		static_cast<CHorizontalLayoutUI*>(GetPaintMgr()->GetRoot())->SetMouseChildEnabled(true);
		return ;
	} 
	int nHaveSong = pSongLists->GetCount();
	nCount += nHaveSong;
	CFileMgr::filesArrayT theLeftFiles;
	if(nCount>nMaxSongNumber)
	{
		CString sTip; sTip.Format(_T("为了考虑速率因素,每个列表长度最大为<f 1><c #F27D30>%d</c></f>\r\n多余的部分将自动添加到新建列表中"), nMaxSongNumber);
		MsgBox(sTip, _T("逸听"), MBT_WARN | MBBT_CHECKBOX | MBBT_OK);
		CFileMgr::filesArrayT theFiles(files.begin(),files.begin()+(nMaxSongNumber-nHaveSong));
		CFileMgr::filesArrayT theTemp(files.begin()+theFiles.size(),files.end());
		theLeftFiles = theTemp;
		files = theFiles;
	}
	_ui._pInfoLayout->SetVisible(true);
	//	::SetCursor(NULL);
	int iSearched = 0;
	for (CFileMgr::filesArrayIterT iter=files.begin();iter!=files.end();++iter)
	{
		{
			iSearched++;
//			unsigned int u_listId = (unsigned int)(_ui._pSongList->GetTag());
			spSongInfoT spSongInfo = thePlayListMgr->PraseFromPath(*iter);
			//添加到数据库
			theDbMgr->GetUserDB()->AddSongsToPlayList(spSongInfo,u_listId);
			//添加到播放列表
			thePlayListMgr->AddSongToList(spSongInfo,u_listId);

			YSongListItemUI* songItem = new YSongListItemUI;
			ASSERT(songItem);
			songItem->SetSong(spSongInfo);
			_ui._pSongList->Add(songItem);
			CString sSearched;sSearched.Format(_T("已成功导入%s"),spSongInfo->GetLocalPath().GetBuffer());
			spSongInfo->GetLocalPath().ReleaseBuffer();
			_ui._pInfoLabel->SetText(sSearched);
			_ui._pCountLabel->SetText(_T("共")+mci::ToString(nHaveSong+iSearched)+_T("首歌曲"));
			::Sleep(10);
		}
	}
	//	::SetCursor();
		
	if(theLeftFiles.size()>0)
	{
		int id = this->OnAddPlayList();
		if (id >0)
		{
			_ui._pPlayList->SelectItem(_ui._pPlayList->GetCount()-1,true);
			this->_AddSongProc2(id, theLeftFiles);
			return;
		}
	}
	GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,3000);
	pSongLists->SetMouseEnabled(true),pSongLists->SetEnabled(true),pSongLists->SetMouseChildEnabled(true);
	pPlayLists->SetMouseEnabled(true),pPlayLists->SetEnabled(true),pPlayLists->SetMouseChildEnabled(true);
	pAddList->SetMouseEnabled(true);
	pSaveAsList->SetMouseEnabled(true);
	static_cast<CHorizontalLayoutUI*>(GetPaintMgr()->GetRoot())->SetMouseChildEnabled(true);
}
DWORD WINAPI YMainWindow::_ThreadProc_AddSong(void* param)
{
	YMainWindow* p_obj = (YMainWindow*)param;
	p_obj->_AddSongProc(NULL);
	return 0;
}
int YMainWindow::ShowDetailAddingInfo(CFileMgr::filesArrayT files,void* param)
{
	_ui._pInfoLayout->SetVisible(true);
	int iSearched = 0;
	for (CFileMgr::filesArrayIterT iter=files.begin();iter!=files.end();++iter)
	{
// 		TStreamFormat tf = thePlayCenter->GetFileFormat(*iter);
// 		if (tf==TStreamFormat::sfUnknown)
// 		{
// 			sLogWarn("歌曲<%s>格式暂不支持",iter->GetBuffer());
// 			iter->ReleaseBuffer();
// 		}
// 		else
		{
			iSearched++;
// 			spSongInfoT spSongInfo;
// 			theDbMgr->GetUserDB()->AddSongToPlayList(*iter,(unsigned int)(_ui._pSongList->GetTag()),spSongInfo);
// 			thePlayCenter->AddSongToList(*spSongInfo,(unsigned int)(_ui._pSongList->GetTag()));
			unsigned int u_listId = (unsigned int)(_ui._pSongList->GetTag());
			spSongInfoT spSongInfo = thePlayListMgr->PraseFromPath(*iter);
			//添加到播放列表
			thePlayListMgr->AddSongToList(spSongInfo,u_listId);
			//添加到数据库
			theDbMgr->GetUserDB()->AddSongsToPlayList(spSongInfo,u_listId);
			YSongListItemUI* songItem = new YSongListItemUI;
			ASSERT(songItem);
			songItem->SetSong(spSongInfo);
			_ui._pSongList->Add(songItem);
			CString sSearched;sSearched.Format(_T("已成功添加%d首歌曲"),iSearched);
			_ui._pInfoLabel->SetText(sSearched);
			CTextUI* pText = (CTextUI*)param;
			if(pText)
				pText->SetText(_T("正在导入:")+spSongInfo->GetLocalPath());
		}
	}
	GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,3000);
	_ui._pCountLabel->SetText(_T("共")+mci::ToString(_ui._pSongList->GetCount())+_T("首歌曲"));
	return 1;
}
//-------------------------------------------------------------------
int YMainWindow::OnAddPlayList()
{
	sLog("YMainWindow::OnAddPlayList");
	std::vector<spPlayListInfoT> UserPlayListArray;
	theDbMgr->GetUserDB()->GetAllPlayListsByType(UserPlayListArray,CPlayListInfo::plt_user);
	if (UserPlayListArray.size() >= MAX_USER_LIST)
	{
		MsgBox(_T("您的标签列表已满，请整理后再试！\n"),_T("逸听"),MBT_WARN|MBBT_OK);
		return 0;
	}
	CString sListName = _T("新建列表");
	if (theDbMgr->GetUserDB()->IsPlayListExists(sListName))
	{
		int iIndex = 1;
		sListName.Format(_T("新建列表%d"),iIndex);
		while (theDbMgr->GetUserDB()->IsPlayListExists(sListName))
		{
			iIndex ++;
			sListName.Format(_T("新建列表%d"),iIndex);
		}
	}
	//添加列表到数据库
	spPlayListInfoT spList(new CPlayListInfo(sListName,CPlayListInfo::PlayListType::plt_user));
	theDbMgr->GetUserDB()->AddPlayList(spList);
	
	CListUI* pPlayLists = _ui._pPlayList;
	ASSERT(pPlayLists);
	YPlayListItemUI* pListItem = new YPlayListItemUI();
	ASSERT(pListItem);
	pListItem->SetPlayList(spList);
	pPlayLists->Add(pListItem);

	thePlayListMgr->AddList(spList);

	ShowInfo(_T("成功添加播放列表"));
	GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,2000);	
	return spList->GetLocalId();
}
//-------------------------------------------------------------------
//托盘图标
void YMainWindow::ShowTrayTooltip(TCHAR* szTip,TCHAR* szTitle)
{
	ASSERT(szTip&&szTitle);
	_trayIcon.ShowToolTip(szTip,szTitle);
}
void YMainWindow::ShowTrayInfo(spSongInfoT spSong)
{
	if (spSong)
	{
		CString sTip;
		sTip.Format(
			_T("歌名:%s\r\n歌手:%s\r\n专辑:%s"),
			spSong->GetSongName().GetString(),
			spSong->GetArtistName().GetString(),
			spSong->GetAlbumName().GetString());
		TCHAR szTip[128] = {0};
		wcscpy(szTip,sTip);
		_trayIcon.SetTip(szTip);

	}
	else
	{
		_trayIcon.SetTip(_T("逸听,聆听最美的声音!"));
	}
}
void YMainWindow::AddTracyIcon()
{
	_trayIcon.Remove();
	_trayIcon.Setup(*this,IDI_YMUSIC,WM_TRAY_NOTIFY,_hOnlineIcon,_T("逸听,聆听最美的声音!"));
	_trayIcon.ShowToolTip(_T(" 逸听--聆听最美的音乐!"),_T("欢迎使用"));
}
//-------------------------------------------------------------------
//登录
void YMainWindow::TryAutoLogin()
{
	sLog("YMainWindow::TryAutoLogin");
	spLogonUserInfoT theLastUser = theDbMgr->GetGlobalDBase()->GetLastUserInfo();
	bool autoLogin = theConfigMgr->GetNormalConfig()->IsEnableAutoLogin();
	//if(theLastUser && theLastUser->bKeepLogged)
	if(theLastUser && autoLogin)
	{
		OnShowLoginWindow();
		//从Cookie中加载的是加密后的密码
		CLoginWindow::Instance()->Login(theLastUser->sLogonEmail,theLastUser->sPasswordMD5,true);
	}
}
//-------------------------------------------------------------------
//播放控制
void YMainWindow::Play()
{
	if (thePlayer->GetHWND()!=theMainWin->GetHWND())
	{
		thePlayer->SetHWND(theMainWin->GetHWND());
		if(CMediaWin::Instance()->IsWindow())
			CMediaWin::Instance()->SendMessage(WM_SYSCOMMAND,SC_CLOSE,0);
	}
	if (thePlayer->GetStatus()==em_pause)
	{//当前是暂停
		thePlayer->Play();
		return ;
	}
	if (_ui._pPlayList->GetCount()<=0)
		return ;

	//当前选中的列表
	int iIndex = _ui._pPlayList->GetCurSel();
	if(iIndex<0)
	{
		iIndex = 0;
		_ui._pPlayList->SelectItem(iIndex);
	}
	if(_ui._pSongList->GetCount()<=0) return;
	int nCnt = _ui._pSongList->GetSelCount();
	int iSong = _ui._pSongList->GetCurSel();
	if(nCnt==0||iSong<0)
		iSong = 0;

	YPlayListItemUI* pListItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(iIndex));
	YSongListItemUI* pSongItem = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(iSong));
	_ui._pSongList->SetTag(pListItem->GetPlayList()->GetLocalId());

	if (!thePlayListMgr->GetCurList())
	{
		thePlayListMgr->SetCurList(pListItem->GetPlayList());
	}
	if (!thePlayListMgr->GetCurSong())
	{
		thePlayListMgr->SetCurSong(pSongItem->GetSong());
	}

	spSongInfoT spSong = thePlayListMgr->GetCurSong();
	if (spSong)
	{
		if (spSong->IsLocal())
		{
			CString sLocalPath = spSong->GetLocalPath();
			std::string s_play = mci::unicode_2_utf8(sLocalPath);
			thePlayer->Play(s_play);
			sLog("Play:%s",mci::unicode_2_ansi(sLocalPath).c_str());
		}
		else
		{
			CString sURL = spSong->GetListenFileUrl();
			std::string s_play = mci::unicode_2_utf8(sURL);
			thePlayer->Play(s_play);
			sLog("Play:%s",s_play.c_str());
		}
		ShowSongInfo(thePlayListMgr->GetCurSong());
	}
}
void YMainWindow::Pause()
{
	thePlayer->Pause();
	sLog("Pause");
}
void YMainWindow::PlayOrPause(bool bPlay)
{
	bPlay ? Play() : Pause();

	OnSongStatusChanged(bPlay);
	_ui._pPlayList->NeedUpdate();
	_ui._pSongList->NeedUpdate();
}
void YMainWindow::NextSong()
{
	YTrace_(_T("NextSong"));
	if (thePlayer->GetHWND()!=theMainWin->GetHWND())
	{
		thePlayer->SetHWND(theMainWin->GetHWND());
		if(CMediaWin::Instance()->IsWindow())
			CMediaWin::Instance()->SendMessage(WM_SYSCOMMAND,SC_CLOSE,0);
	}
	this->Stop();

	if (_ui._pPlayList->GetCount()==0)
	{
		return ;
	}
	if (!thePlayListMgr->GetCurList())
	{
		int iIndex = _ui._pPlayList->GetCurSel();
		if (iIndex<0)
		{
			iIndex = 0;
		}
		YPlayListItemUI* pListItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(iIndex));
		ASSERT(pListItem);
		thePlayListMgr->SetCurList(pListItem->GetPlayList());
	}
	spSongInfoT spSong = thePlayListMgr->GetNextSongToPlay();
	if (spSong)
	{
		if(spSong->IsLocal())
		{
			thePlayer->Play(mci::unicode_2_utf8(spSong->GetLocalPath()));
			sLog("Play:%s",mci::unicode_2_ansi(spSong->GetLocalPath()).c_str());
		}
		else
		{
			thePlayer->Play(mci::unicode_2_utf8(spSong->GetListenFileUrl()));
			sLog("Play:%s",mci::unicode_2_utf8(spSong->GetListenFileUrl()).c_str());
		}
	}
	else
	{

	}
	ShowSongInfo(thePlayListMgr->GetCurSong());
	OnSongStatusChanged(thePlayer->GetStatus()==em_play);
	_ui._pPlayList->NeedUpdate();
	_ui._pSongList->NeedUpdate();
}
void YMainWindow::PrevSong()
{
	YTrace_(_T("PrevSong"));
	if (thePlayer->GetHWND()!=theMainWin->GetHWND())
	{
		thePlayer->SetHWND(theMainWin->GetHWND());
		if(CMediaWin::Instance()->IsWindow())
			CMediaWin::Instance()->SendMessage(WM_SYSCOMMAND,SC_CLOSE,0);
	}
	this->Stop();

	if (_ui._pPlayList->GetCount()==0)
	{
		return ;
	}
	if (!thePlayListMgr->GetCurList())
	{
		int iIndex = _ui._pPlayList->GetCurSel();
		if (iIndex<0)
		{
			iIndex = 0;
		}
		YPlayListItemUI* pListItem = static_cast<YPlayListItemUI*>(_ui._pPlayList->GetItemAt(iIndex));
		ASSERT(pListItem);
		thePlayListMgr->SetCurList(pListItem->GetPlayList());
	}

	spSongInfoT spSong = thePlayListMgr->GetPrevSongToPlay();
	if (spSong)
	{
		if(spSong->IsLocal()) 
		{
			thePlayer->Play(mci::unicode_2_utf8(spSong->GetLocalPath()));
			sLog("Play:%s",mci::unicode_2_ansi(spSong->GetLocalPath()).c_str());
		}
		else
		{
			thePlayer->Play(mci::unicode_2_utf8(spSong->GetListenFileUrl()));
			sLog("Play:%s",mci::unicode_2_utf8(spSong->GetListenFileUrl()).c_str());
		}
	}

// 	_ui._pSongList->NeedUpdate();
// 	ShowSongInfo(thePlayCenter->GetCurrentPlayingSong());
	ShowSongInfo(thePlayListMgr->GetCurSong());
	OnSongStatusChanged(thePlayer->GetStatus()==em_play);
	_ui._pPlayList->NeedUpdate();
	_ui._pSongList->NeedUpdate();
}
void YMainWindow::Stop()
{
	thePlayer->Stop();
	ShowSongInfo(spSongInfoT(NULL));
	OnSongStatusChanged(false);
	_ui._pPlayList->NeedUpdate();
	_ui._pSongList->NeedUpdate();
	sLog("Stop");
}
//-------------------------------------------------------------------
//歌曲属性和目录
void YMainWindow::OnOpenProperty(spSongInfoT spSong)
{
	ASSERT(spSong);
	if(!spSong)
		return ;
//	调用系统属性窗口
// 	SHELLEXECUTEINFO shexecinfo = {0};
// 	shexecinfo.cbSize = sizeof(SHELLEXECUTEINFO);
// 	shexecinfo.fMask = SEE_MASK_INVOKEIDLIST;
// 	shexecinfo.hwnd = NULL;
// 	shexecinfo.lpVerb = L"properties";
// 	shexecinfo.lpFile = spSong->GetLocalPath();
// 	shexecinfo.lpParameters = NULL;
// 	shexecinfo.lpDirectory = NULL;
// 	shexecinfo.nShow = SW_SHOW;
// 	shexecinfo.hInstApp = NULL;
// 	ShellExecuteEx(&shexecinfo);

	PropertyWin* pInfo = new PropertyWin;
	ASSERT(pInfo);
	if(!pInfo)
		return ;
	pInfo->Create(m_hWnd,_T("文件属性"),
		UI_WNDSTYLE_DIALOG,WS_EX_WINDOWEDGE | WS_EX_OVERLAPPEDWINDOW);
	pInfo->CenterWindow();
	pInfo->ShowWindow(true);
	pInfo->InitWnd(spSong);
}
void YMainWindow::OnOpenFolderPath(LPCTSTR sPath)
{
	CString cs_path = sPath;
	cs_path.Replace(_T('/'),_T('\\'));
	sLog(("查看目录[%s]"),mci::unicode_2_ansi(cs_path).c_str());
	ShellExecute(NULL,NULL,L"explorer",L"/select,"+cs_path,NULL,SW_SHOW);
}
//移除指定歌曲
void YMainWindow::OnRemoveSongFromList(unsigned int nPlayListID,unsigned int nSongIndex)
{
	int SongItemExpHeight = 40;
	YSongListItemUI* song = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(nSongIndex));
	if (song->GetFixedHeight()==SongItemExpHeight)
	{
		this->Stop();
	}
	//从数据库移除
	theDbMgr->GetUserDB()->RemovePlayListSongs(song->GetSong(),nPlayListID);
	//从播放队列中移除
	thePlayListMgr->RemoveSongFromList(song->GetSong(),nPlayListID);
	_ui._pSongList->Remove(song);

	ShowInfo(_T("成功移除歌曲"));
	GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,2000);
	_ui._pCountLabel->SetText(_T("共")+mci::ToString(_ui._pSongList->GetCount())+_T("首歌曲"));
}
void YMainWindow::OnRemoveSongsFromList(unsigned int nPlayListID)
{
	int nSelCnt = _ui._pSongList->GetSelCount();
	if (nSelCnt==0) return;
	std::deque<int> v_del;//保存需要移除的索引
	for (int rbegin=_ui._pSongList->GetCount()-1;rbegin>=0;--rbegin)
	{
		CControlUI* pControl = _ui._pSongList->GetItemAt(rbegin);
		if(pControl)
		{
			IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
			if (pListItem&&pListItem->IsSelected())
			{
//				OnRemoveSongFromList(nPlayListID,rbegin);
				v_del.push_back(rbegin);
			}
		}
	}
	while (!v_del.empty())
	{
		int rbegin = v_del.front();
		v_del.pop_front();
		OnRemoveSongFromList(nPlayListID,rbegin);
	}
}
//收藏歌曲
void YMainWindow::OnCollectSongFromList(unsigned int nPlayListID,unsigned int nSongIndex)
{

}
void YMainWindow::OnCollectSongsFromList(unsigned int nPlayListID)
{

}
//分享歌曲
void YMainWindow::OnShareSongFromList(unsigned int nPlayListID,unsigned int nSongIndex)
{
	CShareWin* pShare = CShareWin::Instance();
	ASSERT(pShare);
	if(!pShare->IsWindow())
		return ;
	YSongListItemUI* pSongUI = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(nSongIndex));
	if(!pSongUI)
		return ;
	pShare->Share(pSongUI->GetSong());
}
void YMainWindow::OnShareSongsFromList(unsigned int nPlayListID)
{

}
//下载歌曲
void YMainWindow::OnDownLoadSongFromList(unsigned int nPlayListID,unsigned int nSongIndex)
{

}
void YMainWindow::OnDownLoadSongsFromList(unsigned int nPlayListID)
{

}
//添加到，移动到
void YMainWindow::OnAddSongtoNewList(unsigned int nSongIndex,unsigned int nPlayListID,unsigned int nNewListID,bool bMove /* = false */)
{
	YSongListItemUI* song = static_cast<YSongListItemUI*>(_ui._pSongList->GetItemAt(nSongIndex));
	if (bMove)
	{
		int SongItemExpHeight = 40;
		if (song->GetFixedHeight()==SongItemExpHeight)
		{
			this->Stop();
		}
		//从数据库移除
		theDbMgr->GetUserDB()->RemovePlayListSongs(song->GetSong(),nPlayListID);
		//从播放列表移除
//		thePlayCenter->RemoveSongFromList(song->GetSong()->GetLocalId(),nPlayListID);
		thePlayListMgr->RemoveSongFromList(song->GetSong(),nPlayListID);
		//从ＵＩ中移除
		_ui._pSongList->Remove(song);
	}

	if(song)
	{		
		theDbMgr->GetUserDB()->AddSongsToPlayList(song->GetSong(),nNewListID);
//		thePlayCenter->AddSongToList(song->GetSong(),nNewListID);
		thePlayListMgr->AddSongToList(song->GetSong(),nNewListID);
		ShowInfo(_T("成功移动歌曲"));
		GetPaintMgr()->SetTimer(_ui._pInfoLayout,emTimerType::EMT_SHOWINFO_TIMERID,2000);
		_ui._pCountLabel->SetText(_T("共")+mci::ToString(_ui._pSongList->GetCount())+_T("首歌曲"));
	}
}
void YMainWindow::OnAddSongstoNewList(unsigned int nPlayListID,unsigned int nNewListID,bool bMove /* = false */)
{
	int nSelCnt = _ui._pSongList->GetSelCount();
	if (nSelCnt==0) return;
	std::deque<int> v_add;
	for (int rbegin=_ui._pSongList->GetCount()-1;rbegin>=0;--rbegin)
	{
		CControlUI* pControl = _ui._pSongList->GetItemAt(rbegin);
		if(pControl)
		{
			IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
			if (pListItem&&pListItem->IsSelected())
			{
//				OnAddSongtoNewList(rbegin,nPlayListID,nNewListID,bMove);
				v_add.push_back(rbegin);
			}
		}
	}
	while (!v_add.empty())
	{
		int rbegin = v_add.front();
		v_add.pop_front();
		OnAddSongtoNewList(rbegin,nPlayListID,nNewListID,bMove);
	}
}
//-------------------------------------------------------------------
//去重
void YMainWindow::OnRemoveDumplicate(unsigned int nPlayListID)
{
	sLog("[Begin]去除列表重复歌曲");
	std::vector<spSongInfoT> spSongList;
	theDbMgr->GetUserDB()->GetPlayListSongs(spSongList,nPlayListID);
	// 排序
	std::sort(spSongList.begin(), spSongList.end(), songOrder()); 
	// 开始去重
	std::vector<spSongInfoT>::iterator end_unique = std::unique(spSongList.begin(), spSongList.end(), checkDuplic());  
	if(end_unique !=  spSongList.end())  
	{  
		theDbMgr->GetUserDB()->RemoveAllPlayListSongs(nPlayListID);

		spSongList.erase(end_unique, spSongList.end());  

		theDbMgr->GetUserDB()->AddSongsToPlayList(spSongList,nPlayListID);	

		thePlayListMgr->LoadAllPlayList();

		UpdateSongList(nPlayListID);
	}  
	sLog("[End]去除列表重复歌曲");
	CInfoWindow::ShowInfoWindow(m_hWnd,_T("操作成功"));
}
//-------------------------------------------------------------------
//创建音乐库
YMusicLibWnd* YMainWindow::GetMusicLibWin() const
{
	ASSERT(IsMainThread());
	ASSERT(IsWindow());
	if(/*!m_pMusicLibWnd &&*/ IsWindow())
	{
		YMainWindow* pThis = const_cast<YMainWindow*>(this);
		pThis->m_pMusicLibWnd = YMusicLibWnd::Instance();
		ASSERT(m_pMusicLibWnd);
		if (m_pMusicLibWnd&&!m_pMusicLibWnd->IsWindow())
		{
			m_pMusicLibWnd->Create(*this,NULL,WS_POPUP,0);		
		}
	}
	CDuiRect rtWnd;
	GetWindowRect(&rtWnd);

	CDuiRect rt;
	m_pMusicLibWnd->GetWindowRect(&rt);
	m_pMusicLibWnd->MoveWindow(rtWnd.right,rtWnd.top,rt.GetWidth(),rt.GetHeight());
	return m_pMusicLibWnd;
}
YMusicLibWnd* YMainWindow::GetMusicLibWin2() const
{
	return m_pMusicLibWnd;
}
void YMainWindow::ShowMusicLibWin(BOOL bShow/* = TRUE*/)
{
	sLog("YMainWindow::ShowMusicLibWin");
	YMusicLibWnd* theWin(GetMusicLibWin());
	ASSERT(theWin);
	if(theWin)
		theWin->ShowWindow(!!bShow);
}
//-------------------------------------------------------------------
YMainWindow::spTaskbarListT YMainWindow::GetTaskbarList() const
{
	ASSERT(IsMainThread());
	ASSERT(IsWindow());
	if(!_pTaskbarList && IsWindow())
	{
		YMainWindow* pThis = const_cast<YMainWindow*>(this);
		HRESULT hr = pThis->_pTaskbarList.CoCreateInstance(CLSID_TaskbarList,NULL,CLSCTX_ALL);
		//VERIFY(SUCCEEDED(hr));

		//ASSERT(_pTaskbarList);
	}

	return _pTaskbarList;
}
//-------------------------------------------------------------------
//创建桌面歌词
CDesktopLrcWindow* YMainWindow::GetDesktopLrcWin() const
{
	ASSERT(IsMainThread());
	ASSERT(IsWindow());
	if(/*!m_pMusicLibWnd &&*/ IsWindow())
	{
		YMainWindow* pThis = const_cast<YMainWindow*>(this);
		pThis->m_pDesktopLrcWnd = CDesktopLrcWindow::Instance();
		ASSERT(m_pDesktopLrcWnd);
		if (m_pDesktopLrcWnd&&!m_pDesktopLrcWnd->IsWindow())
		{
			m_pDesktopLrcWnd->Create(GetMusicLibWin()->GetHWND(),NULL,WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_TOPMOST);
		}
	}
	return m_pDesktopLrcWnd;
}
CDesktopLrcWindow* YMainWindow::GetDesktopLrcWin2() const
{
	return m_pDesktopLrcWnd;
}
void YMainWindow::ShowDesktopLrcWin(BOOL bShow/* = TRUE*/)
{
	sLog("YMainWindow::ShowDesktopLrcWin");
	CDesktopLrcWindow* theWin(GetDesktopLrcWin());
	if(theWin)
		theWin->ShowWindow(!!bShow),theWin->LrcChange();
}
//-------------------------------------------------------------------
void YMainWindow::ShowSelectLrcWin()
{
	sLog("YMainWindow::ShowSelectLrcWin");
	CLrcSelectWnd* pLrcWnd = CLrcSelectWnd::Instance();
	ASSERT(pLrcWnd);
	if(!pLrcWnd->IsWindow())
		pLrcWnd->Create(m_hWnd,_T("歌词搜索"),UI_WNDSTYLE_DIALOG,0);
	pLrcWnd->ShowWindow(true);

	spSongInfoT spSong = thePlayListMgr->GetCurSong();

	pLrcWnd->InitSearchEdit(spSong);

}
//-------------------------------------------------------------------
void YMainWindow::ShowPanelLrcTab(BOOL bShow /*= TRUE*/)
{
	sLog("YMainWindow::ShowPanelLrcTab(%d)",bShow);
	if (bShow)
	{
		this->ShowWindow(bShow);// 
		SetForegroundWindow(this->m_hWnd);
		this->BringWindowToTop();

		CControlUI* pUI = NULL;
		FindControl(pUI,varMainWindow::kLrc);
		ASSERT(pUI);
		if(!pUI) return;
//		_ui._pTabLayout->SelectItem((UINT)pUI->GetTag());

		GetPaintMgr()->SendNotify(pUI,kSelectChanged,0,0,true);
	}
}
void YMainWindow::ShowSettingWin()
{
	sLog("YMainWindow::ShowSettingWin");
	CSettingWindow* spSetting = CSettingWindow::Instance();
	ASSERT(spSetting);
	if(!spSetting->IsWindow())
		spSetting->Create(m_hWnd,_T("设置"),UI_WNDSTYLE_DIALOG,0);
	spSetting->CenterWindow();
	spSetting->ShowWindow(true);
}
//-------------------------------------------------------------------
void YMainWindow::ShowSkinWin()
{
	sLog("YMainWindow::ShowSkinWin");
	OnShowSkinWindow();
}
//-------------------------------------------------------------------
void YMainWindow::SetBkColor(DWORD dwCor)
{
	GetPaintMgr()->GetRoot()->SetBkColor(dwCor);

	CConfigMgr::spSkinConfigT pSkinCfg = theConfigMgr->GetSkinConfig();
	pSkinCfg->SetBg(_T(""));
	pSkinCfg->SetColor(dwCor);

	pSkinCfg->Save();

	sLog("YMainWindow::SetBkColor");
}
void YMainWindow::SetBkImage(LPCTSTR szImage)
{
	CControlUI* background = GetPaintMgr()->GetRoot();
	if (background != NULL)
	{
		background->SetBkImage(szImage);
		background->SetBkColor(0);
		background->NeedUpdate();

		SkinChangedParam param;
		param.bkcolor = background->GetBkColor();
		param.bgimage = background->GetBkImage();

		skin_changed_observer_.Broadcast(param);

		CConfigMgr::spSkinConfigT pSkinCfg = theConfigMgr->GetSkinConfig();
		pSkinCfg->SetBg(szImage);
		pSkinCfg->SetColor(0);

		pSkinCfg->Save();

		sLog(mci::unicode_2_ansi(CString(_T("YMainWindow::SetBkImage"))+szImage).c_str());
	}
}
//-------------------------------------------------------------------
void YMainWindow::AddReceive(SkinChangedReceiver* win)
{
	sLog("YMainWindow::AddReceive");
	this->skin_changed_observer_.AddReceiver(win);
}
void YMainWindow::BroadCast()
{
	sLog("YMainWindow::BroadCast");
	CControlUI* background = GetPaintMgr()->GetRoot();
	if (background != NULL)
	{
		SkinChangedParam param;
		param.bkcolor = background->GetBkColor();
		param.bgimage = background->GetBkImage();

		skin_changed_observer_.Broadcast(param);
	}
}
//-------------------------------------------------------------------