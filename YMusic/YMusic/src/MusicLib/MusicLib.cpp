#include "StdAfx.h"
#include "MusicLib.h"
#include "../YApp.h"
#include "jsHandler.h"
#include "../Utilty/json/DataParser.h"
#include "DownloadItem/DownloadItem.h"
#include "DownHelper/ThreadHelper.h"
#include "../MvFolder/MediaPlayerWnd.h"

enum 
{
	USER = 0,
	WEB = 1,
};

namespace MusicLibCtl
{
	static const TCHAR* kBack = _T("back");
	static const TCHAR* kRefresh = _T("refresh");
	static const TCHAR* kMyMusicLib = _T("MyMusicLib");
	static const TCHAR* kFindMusic = _T("FindMusic");
	static const TCHAR* kClose = _T("closebtn");	
	static const TCHAR* kSwitch = _T("table");
	static const TCHAR* kDownloadTab = _T("table2");
	static const TCHAR* kDownloadingList = _T("downing_list");
	static const TCHAR* kDownloadedList = _T("downed_list");

	static const TCHAR* kPlaylist = _T("playlist");
	static const TCHAR* kIListen = _T("imylisten");
	static const TCHAR* kFavNode = _T("fav_node");
	static const TCHAR* kHistory = _T("his_node");
	static const TCHAR* kIDownload = _T("idownload");
	static const TCHAR* kIDownloading = _T("downloading");
	static const TCHAR* kIDownloaded = _T("downloaded");

	static const TCHAR* kUserIe = _T("user_ie");
	static const TCHAR* kXiamiIe = _T("xiami_ie");
	static const TCHAR* kContent = _T("content");
	static const TCHAR* kSearch = _T("search");
};

namespace webJs 
{
	static const TCHAR query_collect[] = _T("querycollect");
	static const TCHAR collect[] = _T("collect");
	static const TCHAR remove_collect[] = _T("removecollect");

	static const TCHAR play[] = _T("Play");
	static const TCHAR pause[] = _T("Pause");
	static const TCHAR playmv[] = _T("PlayMv");

	static const TCHAR share[] = _T("Share");
	static const TCHAR add[] = _T("Add");
	static const TCHAR download[] = _T("Download");

	static const TCHAR query_playlist[] = _T("playlist");
	static const TCHAR new_list[] = _T("newlist");
//	static const TCHAR add[] = _T("addsong");
	static const TCHAR remove[] =_T("remove");

	static const TCHAR load_collect_data[] = _T("loadcollectdata");
	static const TCHAR load_list_data[] = _T("loadlistdata");

	static const TCHAR JumpUrl[] = _T("geturl");

	static const TCHAR get_recommend_songs[] = _T("recommendsongs");
	static const TCHAR get_recommend_albums[] = _T("recommendalbums");

	static const TCHAR msgbox[] = _T("MsgBox");

	static const TCHAR notice[] = _T("shownotice");

};


YMusicLibWnd::YMusicLibWnd()
{
	theMainWin->AddReceive(this);
}
YMusicLibWnd::~YMusicLibWnd()
{
	RemoveObserver();
// 	if (_pUserJsHandler)
// 	{
// 		delete _pUserJsHandler;
// 	}
// 	if (_pXiaMiJsHandler)
// 	{
// 		delete _pXiaMiJsHandler;
// 	}
}

LPCTSTR YMusicLibWnd::GetWindowClass() 
{
	return _T("MusicLib");
}
LPCTSTR YMusicLibWnd::GetWindowClassName() const
{
	return YMusicLibWnd::GetWindowClass();
}

CControlUI* YMusicLibWnd::CreateControl(LPCTSTR pstrClass)
{
	try
	{
		return NULL;
	}
	catch (...)
	{
		throw _T("YMusicLibWnd::CreateControl");
	}
	
}

void YMusicLibWnd::InitWindow()
{
	try
	{
		ZeroMemory(&ui_,sizeof(ui_));
		/* 初始化控件 */
		FindControl(ui_.m_pButtonBack,MusicLibCtl::kBack);
		FindControl(ui_.m_pButtonRefresh,MusicLibCtl::kRefresh);
		FindControl(ui_.m_pOptMyMusicLib,MusicLibCtl::kMyMusicLib);
		FindControl(ui_.m_pOptFindMusic,MusicLibCtl::kFindMusic);
		FindControl(ui_.m_pButtonClose,MusicLibCtl::kClose);
		FindControl(ui_.m_pTabSwitch,MusicLibCtl::kSwitch);
		FindControl(ui_.m_pDownloadTab,MusicLibCtl::kDownloadTab);
		FindControl(ui_.m_pDownloadingList,MusicLibCtl::kDownloadingList);
		FindControl(ui_.m_pDownloadedList,MusicLibCtl::kDownloadedList);

		FindControl(ui_.m_pTree,MusicLibCtl::kPlaylist);
		FindControl(ui_.m_pTreeIListenlist,MusicLibCtl::kIListen);
		FindControl(ui_.m_pTreeIDownload,MusicLibCtl::kIDownload);
		FindControl(ui_.m_pTreeFavNode,MusicLibCtl::kFavNode);
		FindControl(ui_.m_pTreeHistory,MusicLibCtl::kHistory);
		FindControl(ui_.m_pTreeDownloading,MusicLibCtl::kIDownloading);
		FindControl(ui_.m_pTreeDownloaded,MusicLibCtl::kIDownloaded);

		FindControl(ui_.m_pEditContent,MusicLibCtl::kContent);
		FindControl(ui_.m_pButtonSearch,MusicLibCtl::kSearch);
		FindControl(ui_.m_pWebUserIe,MusicLibCtl::kUserIe);
		FindControl(ui_.m_pWebXiamiIe,MusicLibCtl::kXiamiIe);

		assert(ui_.m_pWebXiamiIe);
		_pCurBrowser = ui_.m_pWebXiamiIe;

		CWebBrowserUI* webBra  = ui_.m_pWebUserIe;
		assert(webBra);
		_pUserJsHandler = new CSimpleJsHandler(webBra);
		CWebBrowserUI* webBra2  = ui_.m_pWebXiamiIe;
		assert(webBra2);
		_pXiaMiJsHandler = new CSimpleJsHandler(webBra2);

		this->InitJsHandler(_pUserJsHandler);
		this->InitJsHandler(_pXiaMiJsHandler);
		webBra->SetWebBrowserEventHandler(_pUserJsHandler);
		webBra2->SetWebBrowserEventHandler(_pXiaMiJsHandler);

		theMainWin->BroadCast();
	}
	catch (...)
	{
		throw _T("YMusicLibWnd::InitWindow");
	}
}

void YMusicLibWnd::Notify(TNotifyUI& msg)
{
	CControlUI* pUI = msg.pSender;
	try
	{
		YTrace_(_T("YMainWindow::Notify class:%s,event:%s,name:%s,text:%s,tag:%d"),pUI ? pUI->GetClass():_T(""),msg.sType.GetData(),pUI?pUI->GetName().GetData():_T(""),pUI?pUI->GetText().GetData():_T(""),pUI?(int)pUI->GetTag():0);
		if(0 == msg.sType.Compare(kWindowInit))
			return InitWindow();
		else if (0 == msg.sType.Compare(kClick))
			return OnClick(msg);
		else if (0 == msg.sType.Compare(kSelectChanged))
			return OnSelectChange(msg);
		else if (0 == msg.sType.Compare(kItemSelect))
			return OnItemSelect(msg);
		else if(0 == msg.sType.Compare(kMenu))
			return OnMenu(msg);
		else if (0 == msg.sType.Compare(kReturn))
		{
			if (pUI->IsName(MusicLibCtl::kContent))
			{
				pUI->GetParent()->SetFocus();
				OnSearch();
			}
		}
		else 
		{
			YTrace_(_T("CNotifyPump::NotifyPump\n"));
			return CNotifyPump::NotifyPump(msg);
		}
	}
	catch (...)
	{
		throw _T("YMusicLibWnd::Notify");
		return ;
	}
}
void YMusicLibWnd::OnClick(TNotifyUI& msg)
{
	try
	{
		if(msg.pSender->IsName(MusicLibCtl::kClose))
		{
			CCheckBoxUI* pBox = NULL;
			theMainWin->FindControl(pBox,varMainWindow::kExpandMusicLib);
			if(pBox) pBox->Selected(false);
			ShowWindow(false);
		}
		else if (msg.pSender->IsName(MusicLibCtl::kBack))
			OnGoBack();
		else if (msg.pSender->IsName(MusicLibCtl::kRefresh))
			OnRefreshBrowser();
		else if (msg.pSender->IsName(MusicLibCtl::kSearch))
			OnSearch();
	}
	catch (...)
	{
		throw _T("YMusicLibWnd::TypeClick");
		return ;
	}
}
void YMusicLibWnd::OnSelectChange(TNotifyUI& msg)
{
	try
	{
		CControlUI* pSender = msg.pSender;
		unsigned int nTag = pSender->GetTag();
		ui_.m_pTabSwitch->SelectItem(nTag);
		ui_.m_pTabSwitch->SetTag(nTag);
		ui_.m_pEditContent->SetTag(nTag);
		assert(ui_.m_pButtonBack);
		if (nTag == USER)
		{		
			ui_.m_pButtonBack->SetEnabled(false);
			_pCurBrowser = ui_.m_pWebUserIe;
			if(ui_.m_pTree->GetCurSel() == -1)
			{
				ui_.m_pTree->SelectItem(1,true);
			}
		}
		else
		{
			ui_.m_pButtonBack->SetEnabled(true);
			_pCurBrowser = ui_.m_pWebXiamiIe;
		}
// 		if (pSender->IsName(MusicLibCtl::kMyMusicLib))
// 			ui_.m_pTabSwitch->SelectItem(0);
// 		else if (pSender->IsName(MusicLibCtl::kFindMusic))
// 			ui_.m_pTabSwitch->SelectItem(1);
	}
	catch (...)
	{
		throw _T("YMusicLibWnd::OnSelectChange");
		return ;
	}
}
void YMusicLibWnd::OnItemSelect(TNotifyUI& msg)
{
	try
	{
		CControlUI* pSender = msg.pSender;
		if (pSender == ui_.m_pTree)
		{
			CTreeNodeUI* pNode = static_cast<CTreeNodeUI*>(ui_.m_pTree->GetItemAt(ui_.m_pTree->GetCurSel()));
			if (pNode == ui_.m_pTreeFavNode)
				OnShowFavSongs();
			else if (pNode == ui_.m_pTreeHistory)
				OnShowHisSongs();
			else if(pNode == ui_.m_pTreeDownloading)
				OnShowDownloading();
			else if(pNode == ui_.m_pTreeDownloaded)
				OnShowDownloaded();
		}
	}
	catch(...)
	{

	}
}
void YMusicLibWnd::OnRefreshBrowser()
{
	assert(_pCurBrowser);
	BSTR Url;
	_pCurBrowser->GetWebBrowse2()->get_LocationURL(&Url);
	CString sUrl(Url);
	_pCurBrowser->Navigate2(sUrl);
	_pCurBrowser->Refresh();
}
void YMusicLibWnd::OnGoBack()
{
	if (ui_.m_pWebXiamiIe)
	{
		ui_.m_pWebXiamiIe->GoBack();
	}
// 	if (ui_.m_pWebXiamiIe->GetWebBrowse2())
// 	{
// 		ui_.m_pWebXiamiIe->GetWebBrowse2()->GoBack();
// 	}
}
void YMusicLibWnd::OnMenu(TNotifyUI& msg)
{

}
void YMusicLibWnd::OnSearch()
{
	//CString url = _T("http://box.zhangmen.baidu.com/x?op=12&count=1&title=大约在冬季$$齐秦$$$$");
	
	CString sContent = ui_.m_pEditContent->GetText();
	if (!sContent.IsEmpty())
	{
		//对搜索内容进行urlencode转码;
 		std::string sUrlencode = URL::UrlEncode(em_utility::mci::unicode_2_utf8(sContent));
 		sContent = em_utility::mci::utf8_2_CString(sUrlencode.c_str());

		CString sUrl;
		spUserContextT pContext = theRuntimeState->GetCurrentContext(NULL);
		if (!pContext->GetUser())
		{
//			sUrl.Format(_T("/Search/search.php?key=%s&unlogin=1"),sContent);
			sUrl.Format(_T("/search-song?key=%s&unlogin=1"),sContent);
		}
		else
		{
//			sUrl.Format(_T("/Search/search.php?key=%s"),sContent);
			sUrl.Format(_T("/search-song?key=%s"),sContent);
		}
//		sUrl = SERVER_URL + sUrl;
		sUrl = _T("http://www.xiami.com/app/xiating") + sUrl;
		TransToURL(ui_.m_pEditContent->GetTag(),sUrl);
		CInfoWindow::ShowInfoWindow(m_hWnd,ui_.m_pEditContent->GetPos(),_T("搜索功能暂时还未完善"));
	}
	else
	{
		CInfoWindow::ShowInfoWindow(this->m_hWnd,ui_.m_pEditContent->GetPos(),_T("搜索内容不能为空！！！"));
	}
	
}
void YMusicLibWnd::OnShowFavSongs()
{
	ui_.m_pDownloadTab->SelectItem(ui_.m_pWebUserIe->GetTag());
}
void YMusicLibWnd::OnShowHisSongs()
{
	ui_.m_pDownloadTab->SelectItem(ui_.m_pWebUserIe->GetTag());
}
void YMusicLibWnd::OnShowDownloading()
{
	ui_.m_pDownloadTab->SelectItem(ui_.m_pDownloadingList->GetTag());

// 	DownloadListItemUI* loadItem = new DownloadListItemUI;
// 	assert(loadItem);
// 
// 	ui_.m_pDownloadList->Add(loadItem);

}
void YMusicLibWnd::OnShowDownloaded()
{
	ui_.m_pDownloadTab->SelectItem(ui_.m_pDownloadedList->GetTag());
}

void YMusicLibWnd::TransToURL(int iIndex,LPCTSTR url)
{
//	ui_.m_pTabSwitch->GetCurSel() == iIndex ? (ui_.m_pWebUserIe->Navigate2(url)) : (ui_.m_pWebXiamiIe->Navigate2(url));
//	ui_.m_pWebXiamiIe->Navigate2(url);
	if (ui_.m_pTabSwitch->GetCurSel() != iIndex)
	{
		if(USER==iIndex)
			GetPaintMgr()->SendNotify(ui_.m_pOptMyMusicLib,kSelectChanged,0,0,true);
		else 
			GetPaintMgr()->SendNotify(ui_.m_pOptFindMusic,kSelectChanged,0,0,true);
	}
	if(iIndex==USER)
		ui_.m_pWebUserIe->Navigate2(url);
	else if(iIndex==WEB)
		ui_.m_pWebXiamiIe->Navigate2(url);
}
LRESULT YMusicLibWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if(!RegisterSkin(_T("musiclib.xml"),0,this))
		return 0;

	GetPaintMgr()->AddNotifier(this);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

//	theMainWin->BroadCast();
	return 0;
}
LRESULT YMusicLibWnd::OnHttpfileDownload(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg != WM_HTTP_FILE_DOWNLOAD) 
		return 0;
	if(wParam == WP_NOTHING)
		return 0;
	else if (lParam == WP_LOADING)
	{
// 		char* purl = (char *)lParam;
// 		YTrace_(_T("YMusicLibWnd::OnHttpfileDownload (%s)\r\n"),purl);
		{
			CurlData * pData = (CurlData *)wParam;
			if(pData)
				YTrace_(_T("YMusicLibWnd::OnHttpfileDownload:%s,%s,%s,%s\r\n"),
				mci::ToString(pData->dltotal),
				mci::ToString(pData->dlnow),
				mci::ToString(pData->ultotal),
				mci::ToString(pData->ulnow));
		}
	}	
	else if(lParam == WP_LOAD_FINISH)
	{
		YTrace_(_T("YMusicLibWnd::OnHttpfileDownload (finished)\r\n"));
	}
	return 0;
}
LRESULT YMusicLibWnd::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg == WM_HTTP_FILE_DOWNLOAD)
		OnHttpfileDownload(uMsg,wParam,lParam,bHandled);
	return __super::ProcessWindowMessage(uMsg,wParam,lParam,bHandled);
}
BOOL YMusicLibWnd::Receive(SkinChangedParam param)
{
	try
	{
		if(!IsWindow()) return FALSE;
		sLog("YMusicLibWnd::Receive");
		bgimage_ = param.bgimage;
		bkcolor_ = param.bkcolor;
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
		sLogError("YMusicLibWnd::Receive");
		throw _T("YMusicLibWnd::Receive");
		return FALSE;
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void YMusicLibWnd::InitJsHandler(CSimpleJsHandler* h)
{
// 	h->RegisterJsCallBack(webJs::play,boost::bind(&CMusicLibWindow::OnPlay,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::collect,boost::bind(&CMusicLibWindow::OnCollect,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::remove_collect,boost::bind(&CMusicLibWindow::OnRemoveCollect,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::query_collect,boost::bind(&CMusicLibWindow::OnQuerycollect,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::share,boost::bind(&CMusicLibWindow::OnShare,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::query_playlist,boost::bind(&CMusicLibWindow::OnQueryPlayList,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::new_list,boost::bind(&CMusicLibWindow::OnNewList,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::add,boost::bind(&CMusicLibWindow::OnAdd,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::load_collect_data,boost::bind(&CMusicLibWindow::OnLoadCollectData,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::load_list_data,boost::bind(&CMusicLibWindow::OnLoadListData,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::remove,boost::bind(&CMusicLibWindow::OnRemove,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::JumpUrl,boost::bind(&CMusicLibWindow::OnGetJumpUrl,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::get_recommend_songs,boost::bind(&CMusicLibWindow::OnGetRecommendSongs,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::get_recommend_albums,boost::bind(&CMusicLibWindow::OnGetRecommendAlbums,this,_1,_2,_3));
// 	h->RegisterJsCallBack(webJs::msgbox,boost::bind(&CMusicLibWindow::OnMsgBox,this,_1,_2,_3));
	h->RegisterJsCallBack(webJs::msgbox,(CSimpleJsHandler::JsCallBackT)&YMusicLibWnd::OnMsgBox);
	h->RegisterJsCallBack(webJs::play,(CSimpleJsHandler::JsCallBackT)&YMusicLibWnd::OnPlay);
	h->RegisterJsCallBack(webJs::share,(CSimpleJsHandler::JsCallBackT)&YMusicLibWnd::OnShare);
	h->RegisterJsCallBack(webJs::add,(CSimpleJsHandler::JsCallBackT)&YMusicLibWnd::OnAdd);
	h->RegisterJsCallBack(webJs::download,(CSimpleJsHandler::JsCallBackT)&YMusicLibWnd::OnDownload);
	h->RegisterJsCallBack(webJs::playmv,(CSimpleJsHandler::JsCallBackT)&YMusicLibWnd::OnPlayMv);
} 
void YMusicLibWnd::OnMsgBox(CSimpleJsHandler* h,LPCTSTR param,CString& sResult)
{
	::MsgBox(theMainWin->GetMusicLibWin()->GetHWND(),_T("JS调用C++函数:")+CString(param),_T("提示"),MBT_INFO|MBBT_OK|MBBT_CANCEL);
}
void YMusicLibWnd::OnPlay(CSimpleJsHandler* h,LPCTSTR param,CString& sResult)
{
	Json::Reader reader;
	Json::Value value;
	CString sParam = param;
	sParam.Replace(_T('\''),_T('\"'));
	std::string sMsgUtf8 = mci::unicode_2_ansi(sParam);
	if (!reader.parse(sMsgUtf8, value) || value.isNull()) return ;
	std::string sSongName = JsonParser::getValue<std::string>(value,"songName");
	std::string sAuthorName = JsonParser::getValue<std::string>(value,"authorName");
	std::string sAlbumName = JsonParser::getValue<std::string>(value,"albumName");
	std::string sImageUrl = JsonParser::getValue<std::string>(value,"imgUrl");
	std::string sPath = JsonParser::getValue<std::string>(value,"spath");
	int nSongId = JsonParser::getValue<int>(value,"songId");
	int nSize = JsonParser::getValue<int>(value,"size");
	CString strTip;
	strTip.Format(_T("您将播放[歌曲：%s][艺人:%s][SID:%d][spath:%s][imgUrl:%s]"),
		mci::ansi_2_CString(sSongName.c_str()),
		mci::ansi_2_CString(sAuthorName.c_str()),
		nSongId,
		mci::ansi_2_CString(sPath.c_str()),
		mci::ansi_2_CString(sImageUrl.c_str()));
	if(MBRT_OK==::MsgBox(theMainWin->GetMusicLibWin()->GetHWND(),strTip,_T("播放"),MBT_INFO|MBBT_OK|MBBT_CANCEL))
	{
		spSongInfoT spSong = spSongInfoT(new CSongInfo);
		spSong->SetSongId(nSongId);
		spSong->SetSongName(mci::ansi_2_CString(sSongName.c_str()));
		spSong->SetArtistName(mci::ansi_2_CString(sAuthorName.c_str()));
		spSong->SetAblumName(mci::ansi_2_CString(sAlbumName.c_str()));
		spSong->SetListenFileUrl(mci::ansi_2_CString(sPath.c_str()));
		spSong->SetCoverUrl(mci::ansi_2_CString(sImageUrl.c_str()));
		spSong->SetLocal(false);
		spSong->SetFileSize(nSize);

		CListUI* pList = NULL;
		theMainWin->FindControl(pList,varMainWindow::kSongList);
		assert(pList);
		int nHaveSong = pList->GetCount();

		theDbMgr->GetUserDB()->AddSongsToPlayList(spSong,pList->GetTag());
		thePlayListMgr->AddSongToList(spSong,pList->GetTag());		

		YSongListItemUI* songItem = new YSongListItemUI;
		ASSERT(songItem);
		songItem->SetSong(spSong);
		pList->Add(songItem);
		CLabelUI* pInfo = NULL;
		CLabelUI* pCount = NULL;

		theMainWin->FindControl(pInfo,varMainWindow::kInfoLabel);
		theMainWin->FindControl(pCount,varMainWindow::kCount);
		ASSERT(pInfo&&pCount);
		CString splay;
		splay.Format(_T("成功导入歌曲:%s-%s"),mci::ansi_2_CString(sSongName.c_str()),
			mci::ansi_2_CString(sAuthorName.c_str()));
		pInfo->SetText(splay);
		
		pCount->SetText(_T("共") + mci::ToString(nHaveSong+1)+_T("首歌曲"));
		thePlayListMgr->SetCurSong(spSongInfoT());
		pList->SelectItem(nHaveSong);
		theMainWin->Stop();
		theMainWin->PlayOrPause(true);
	}	
}
void YMusicLibWnd::OnPlayMv(CSimpleJsHandler* h,LPCTSTR param,CString& sResult)
{
	Json::Reader reader;
	Json::Value value;
	CString sParam = param;
	sParam.Replace(_T('\''),_T('\"'));
	std::string sMsgUtf8 = mci::unicode_2_ansi(sParam);
	if (!reader.parse(sMsgUtf8, value) || value.isNull()) return ;
	std::string sSongName = JsonParser::getValue<std::string>(value,"filmName");
	std::string sAuthorName = JsonParser::getValue<std::string>(value,"filmAuthor");
	std::string sImageUrl = JsonParser::getValue<std::string>(value,"imgUrl");
	std::string sPath = JsonParser::getValue<std::string>(value,"spath");
	int nSongId = JsonParser::getValue<int>(value,"filmId");
	CString strTip;
	strTip.Format(_T("您将播放[歌曲：%s][艺人:%s][SID:%d][spath:%s]"),
		mci::ansi_2_CString(sSongName.c_str()),
		mci::ansi_2_CString(sAuthorName.c_str()),
		nSongId,
		mci::ansi_2_CString(sPath.c_str()));
	if(MBRT_OK==::MsgBox(theMainWin->GetMusicLibWin()->GetHWND(),strTip,_T("播放Mv"),MBT_INFO|MBBT_OK|MBBT_CANCEL))
	{
		CMediaWin::GetMediaPlayer()->ReadyToPlay(mci::utf8_2_CString(sPath.c_str()));
		CMediaWin::GetMediaPlayer()->Play();
	}
}
void YMusicLibWnd::OnShare(CSimpleJsHandler* h,LPCTSTR param,CString& sResult)
{
	theMainWin->OnShowShareWindow();
	Json::Reader reader;
	Json::Value value;
	CString sParam = param;
	sParam.Replace(_T('\''),_T('\"'));
	std::string sMsgUtf8 = mci::unicode_2_ansi(sParam);
	std::string st = "{\"songName\":\"2345\"}";
	if (!reader.parse(sMsgUtf8, value) || value.isNull()) return ;
	std::string sSongName = JsonParser::getValue<std::string>(value,"songName");
	std::string sAuthorName = JsonParser::getValue<std::string>(value,"authorName");
	int nSongId = JsonParser::getValue<int>(value,"songId");
	CString strTip;
	strTip.Format(_T("您将分享[歌曲：%s][艺人:%s][SID:%d]"),
		mci::ansi_2_CString(sSongName.c_str()),
		mci::ansi_2_CString(sAuthorName.c_str()),
		nSongId);
	::MsgBox(theMainWin->GetMusicLibWin()->GetHWND(),strTip,_T("提示"),MBT_INFO|MBBT_OK|MBBT_CANCEL);
}
void YMusicLibWnd::OnAdd(CSimpleJsHandler* h,LPCTSTR param,CString& sResult)
{
	Json::Reader reader;
	Json::Value value;
	CString sParam = param;
	sParam.Replace(_T('\''),_T('\"'));
	std::string sMsgUtf8 = mci::unicode_2_ansi(sParam);
	if (!reader.parse(sMsgUtf8, value) || value.isNull()) return ;
	std::string sSongName = JsonParser::getValue<std::string>(value,"songName");
	std::string sAuthorName = JsonParser::getValue<std::string>(value,"authorName");
	std::string sAlbumName = JsonParser::getValue<std::string>(value,"albumName");
	std::string sImageUrl = JsonParser::getValue<std::string>(value,"imgUrl");
	std::string sPath = JsonParser::getValue<std::string>(value,"spath");
	int nSongId = JsonParser::getValue<int>(value,"songId");
	int nSize = JsonParser::getValue<int>(value,"size");
	CString strTip;
	strTip.Format(_T("您将添加[歌曲：%s][艺人:%s][SID:%d][spath:%s][imgUrl:%s]"),
		mci::ansi_2_CString(sSongName.c_str()),
		mci::ansi_2_CString(sAuthorName.c_str()),
		nSongId,
		mci::ansi_2_CString(sPath.c_str()),
		mci::ansi_2_CString(sImageUrl.c_str()));
	if(MBRT_OK==::MsgBox(theMainWin->GetMusicLibWin()->GetHWND(),strTip,_T("添加"),MBT_INFO|MBBT_OK|MBBT_CANCEL))
	{
		spSongInfoT spSong = spSongInfoT(new CSongInfo);
		spSong->SetSongId(nSongId);
		spSong->SetSongName(mci::ansi_2_CString(sSongName.c_str()));
		spSong->SetArtistName(mci::ansi_2_CString(sAuthorName.c_str()));
		spSong->SetAblumName(mci::ansi_2_CString(sAlbumName.c_str()));
		spSong->SetListenFileUrl(mci::ansi_2_CString(sPath.c_str()));
		spSong->SetCoverUrl(mci::ansi_2_CString(sImageUrl.c_str()));
		spSong->SetLocal(false);
		spSong->SetFileSize(nSize);

		CListUI* pList = NULL;
		theMainWin->FindControl(pList,varMainWindow::kSongList);
		assert(pList);
		int nHaveSong = pList->GetCount();

		theDbMgr->GetUserDB()->AddSongsToPlayList(spSong,pList->GetTag());
		thePlayListMgr->AddSongToList(spSong,pList->GetTag());		

		YSongListItemUI* songItem = new YSongListItemUI;
		ASSERT(songItem);
		songItem->SetSong(spSong);
		pList->Add(songItem);
		CLabelUI* pInfo = NULL;
		CLabelUI* pCount = NULL;

		theMainWin->FindControl(pInfo,varMainWindow::kInfoLabel);
		theMainWin->FindControl(pCount,varMainWindow::kCount);
		ASSERT(pInfo&&pCount);
		CString splay;
		splay.Format(_T("成功导入歌曲:%s-%s"),mci::ansi_2_CString(sSongName.c_str()),
			mci::ansi_2_CString(sAuthorName.c_str()));
		pInfo->SetText(splay);

		pCount->SetText(_T("共") + mci::ToString(nHaveSong+1)+_T("首歌曲"));
	}
}
/*
std::string UnicodeConvert(const std::wstring& strWide, UINT uCodePage)
{
	std::string strANSI;
	int iLen = ::WideCharToMultiByte(uCodePage, 0, strWide.c_str(), -1, NULL, 0, NULL, NULL);

	if (iLen > 1)
	{ 
		strANSI.resize(iLen-1);
		::WideCharToMultiByte(uCodePage, 0, strWide.c_str(), -1, &strANSI[0], iLen, NULL, NULL);
	}

	return strANSI;
}

std::string UnicodeToUTF8(const std::wstring& strWide)
{
	return UnicodeConvert(strWide, CP_UTF8);
}

std::string UnicodeToANSI(const std::wstring& strWide)
{
	return UnicodeConvert(strWide, CP_ACP);
}*/
void YMusicLibWnd::OnDownload(CSimpleJsHandler* h,LPCTSTR param,CString& sResult)
{
	Json::Reader reader;
	Json::Value value;
	CString sParam = param;
	sParam.Replace(_T('\''),_T('\"'));
	std::string sMsgUtf8 = mci::unicode_2_ansi(sParam);
	if (!reader.parse(sMsgUtf8, value) || value.isNull()) return ;
	std::string sSongName = JsonParser::getValue<std::string>(value,"songName");
	std::string sAuthorName = JsonParser::getValue<std::string>(value,"authorName");
	std::string sPath = JsonParser::getValue<std::string>(value,"spath");
	std::string sImageUrl = JsonParser::getValue<std::string>(value,"imgUrl");
	int nSongId = JsonParser::getValue<int>(value,"songId");
	std::string sAlbumName = JsonParser::getValue<std::string>(value,"albumName"); 
	std::string sType  = JsonParser::getValue<std::string>(value,"type");
	int size = JsonParser::getValue<int>(value,"size");
	CString strTip;
	strTip.Format(_T("您将下载[歌曲：%s][艺人:%s][SID:%d][spath:%s]"),
		mci::ansi_2_CString(sSongName.c_str()),
		mci::ansi_2_CString(sAuthorName.c_str()),
		nSongId,
		mci::ansi_2_CString(sPath.c_str()));



//	if(MBRT_OK==::MsgBox(theMainWin->GetMusicLibWin()->GetHWND(),strTip,_T("下载"),MBT_INFO|MBBT_OK|MBBT_CANCEL))
	{//准备下载
// 		em_utility::down_http_file::down_task new_task;
// 		new_task.h_notify_wnd = theMainWin->GetMusicLibWin()->GetHWND();
// 		new_task.n_notify_msg = WM_HTTP_FILE_DOWNLOAD;
// 	
// 		new_task.n_lparam = (LPARAM)(sPath.c_str());
// 		new_task.s_url = mci::ansi_2_CString(sPath.c_str());
// 		new_task.s_local_file_down = theCacheMgr->GetSongPath() + mci::ToString(nSongId);
// 		new_task.s_local_file_normal = theCacheMgr->GetSongPath() + mci::CheckFilePathValid(mci::ansi_2_CString(sSongName.c_str()) + _T(" - ") + mci::ansi_2_CString(sAuthorName.c_str())+_T(".mp3")); 
// 
// 		theHttpDownerMgr->add_task(new_task);
		{
			spSongInfoT spSong = spSongInfoT(new CSongInfo);
			spSong->SetListenFileUrl(mci::ansi_2_CString(sPath.c_str()));
			spSong->SetSongId(nSongId);
			spSong->SetSongName(mci::ansi_2_CString(sSongName.c_str()));
			spSong->SetArtistName(mci::ansi_2_CString(sAuthorName.c_str()));
			spSong->SetAblumName(mci::ansi_2_CString(sAlbumName.c_str()));
			spSong->SetFileExt(mci::ansi_2_CString(sType.c_str()));
			spSong->SetLocal(false);
			spSong->SetFileSize(size);
			CDuiString sSave;
			sSave.Format(_T("%d.mp3"),nSongId);
			spSong->SetLocalPath(theCacheMgr->GetSongPath() + sSave);
	//		spSong->SetLocalPath(theCacheMgr->GetSongPath() + mci::CheckFilePathValid(mci::ansi_2_CString(sSongName.c_str()) + _T(" - ") + mci::ansi_2_CString(sAuthorName.c_str())+_T(".mp3")));
		
			YMusicLibWnd::Instance()->Downloading(spSong);
		}
	}
}


void YMusicLibWnd::Downloading(spSongInfoT spSong)
{
	ASSERT(spSong);
	if(!spSong)
		return ;

	DownloadListItemUI* loadItem = new DownloadListItemUI();
	assert(loadItem);
	loadItem->InitVar();
	loadItem->SetSong(spSong);
	ui_.m_pDownloadingList->Add(loadItem);//加入到界面
	em_utility::StreamThread::Instance()->AddRequest(loadItem);//加入到下载队列
}
void YMusicLibWnd::Downloaded(spSongInfoT spSong)
{
	ASSERT(spSong);
	if(!spSong)
		return ;
	spSong->SetLocal(true);
	DownloadFinishedListItemUI* pItem = new DownloadFinishedListItemUI;
	assert(pItem);
	pItem->SetSong(spSong);
	ui_.m_pDownloadedList->Add(pItem);
}