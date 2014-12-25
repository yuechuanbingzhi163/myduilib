#include "stdafx.h"
#include "LrcSelectWnd.h"
#include "../YApp.h"
#include <commdlg.h>
CLrcSelectWnd::CLrcSelectWnd()
{
	theMainWin->AddReceive(this);
}
CLrcSelectWnd::~CLrcSelectWnd()
{
	RemoveObserver();
}
LPCTSTR CLrcSelectWnd::GetWindowClass()
{
	return _T("LrcSelectWnd");
}
LPCTSTR CLrcSelectWnd::GetWindowClassName() const
{
	return CLrcSelectWnd::GetWindowClass();
}

void CLrcSelectWnd::Notify(TNotifyUI& msg)
{
	YTrace_(_T("class:%s,id:%s,msg:%s"),msg.pSender->GetClass(),msg.pSender->GetName().GetData(),msg.sType.GetData());
	CControlUI* pSender = msg.pSender;

	if (0 == msg.sType.Compare(kClick))
	{
		if (pSender->IsName(_T("ok")))
			OnOK();
		else if (pSender->IsName(_T("cancle")) || pSender->IsName(_T("close")))
			OnClose();
		else if (pSender->IsName(_T("SearchLrc")))
			OnSearchLrc();
		else if (pSender->IsName(_T("releate")))
			OnReleatLocalLrc();
	}
}

LRESULT CLrcSelectWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if(!RegisterSkin(_T("select-lrcwnd.xml")))
		return 0;

	GetPaintMgr()->AddNotifier(this);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	CenterWindow();
	theMainWin->BroadCast();
	return 0;
}
void CLrcSelectWnd::OnReleatLocalLrc()
{	
	OPENFILENAME openfilename;
	ZeroMemory(&openfilename, sizeof(OPENFILENAME)); 
	TCHAR szfiletitle[MAX_PATH] = _T("\0");
	CDuiString s_title=_T("关联本地歌词");
	LPTSTR szFile = new TCHAR[MAX_PATH ]; //
	szFile[0] = 0x0;

	static TCHAR lpstr[] = _T("歌词文件(*.lrc)\0*.lrc\0\0");
	openfilename.lStructSize = sizeof(OPENFILENAME) ;
	openfilename.hwndOwner = IsWindow() ? m_hWnd : theMainWin->GetHWND();
	openfilename.hInstance = CPaintManagerUI::GetInstance() ;
	openfilename.lpstrFilter = lpstr;//lpstr;
	openfilename.lpstrCustomFilter = (LPTSTR)NULL ;
	openfilename.nMaxCustFilter = 0L ;
	openfilename.nFilterIndex = 1L ;
	openfilename.lpstrFile = szFile ;
	openfilename.nMaxFile = MAX_PATH  ;
	openfilename.lpstrFileTitle = szfiletitle ;
	openfilename.nMaxFileTitle = sizeof(szfiletitle) ;
	openfilename.lpstrInitialDir = NULL ;
	openfilename.lpstrTitle = s_title;
	openfilename.nFileOffset = 0;
	openfilename.nFileExtension = 0;
	openfilename.lpstrDefExt = _T("*.lrc") ;
	openfilename.lCustData = 0 ;

	openfilename.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_READONLY  | OFN_EXPLORER /*|OFN_LONGNAMES*/;
	bool isOk = false;
	if (GetOpenFileName(&openfilename))
	{

		CString s_file = szFile;

//		spSongInfoT spSong = thePlayCenter->GetCurrentPlayingSong();
		spSongInfoT spSong = thePlayListMgr->GetCurSong();

		if(spSong)spSong->SetLrcLocalPath(s_file);
//		thePlayCenter->UpdateCurrentPlayingSong(spSong);
		thePlayListMgr->UpdateSong(spSong,thePlayListMgr->GetCurList()->GetLocalId());
		thePlayListMgr->SetCurSong(spSong);

		theMainWin->ShowSongInfo(spSong);
		isOk = true;
	}
	delete[] szFile;
	if(isOk)
		Close(1L);
}
void CLrcSelectWnd::OnOK()
{
	CControlUI *p = GetPaintMgr()->FindControl(_T("lrclist"));
	assert(p);
	CListUI *pLrcList = static_cast<CListUI*>(p);
	if(!p) 
	{
		Close();
		return ;
	}

	int nIndex = pLrcList->GetCurSel();
	if(nIndex >= 0)
	{
		Close(IDOK);
		//发送歌词选择消息
		theLrcMgr->SetSelectLrcDownload(nIndex,m_spSongForSearch);
	}

}

void CLrcSelectWnd::OnClose()
{
	Close(0L);
}

void CLrcSelectWnd::OnSearchLrc()
{
	CControlUI *pUISong   = GetPaintMgr()->FindControl(_T("ESongName"));
	CControlUI *pUIArtist = GetPaintMgr()->FindControl(_T("EArtistName"));
	assert(pUISong);
	assert(pUIArtist);
	if(pUIArtist && pUISong)
	{
		CEditUI *pEditSongName   = (CEditUI*)pUISong;
		CEditUI *pEditArtistName = (CEditUI*)pUIArtist;

		m_InPutArtist = pEditArtistName->GetText();
		m_InPutSong   = pEditSongName->GetText();
	}
	//开始搜索歌词
	{
		CAnimationTabLayoutUI* pSearchTab = NULL;
		CControlUI* pLayout = GetPaintMgr()->FindControl(_T("tab"));
		assert(pLayout);
		if(pLayout)
			pSearchTab = static_cast<CAnimationTabLayoutUI*>(pLayout);
		this->ClearItem();
		theLrcMgr->DownloadLrcFromOther(m_InPutSong,m_InPutArtist);
		SongInfoVT LrcList =theLrcMgr->GetSearchResultList();
		if (LrcList.size()<=0)
		{
			pSearchTab->SelectItem(1);
			return ;
		}
		pSearchTab->SelectItem(0);
		int index = 0;
		for (auto it = LrcList.begin();it!=LrcList.end();++it)
		{
			index ++;
			this->AddItem(index,it->strArtist,it->strTitle);
		}
	}
}

void CLrcSelectWnd::ReleatLocalLrc()
{
	OnReleatLocalLrc();
}
void CLrcSelectWnd::AddItem(int nIndex,CString sArtist,CString sSongName)
{
	CControlUI *p = GetPaintMgr()->FindControl(_T("lrclist"));
	assert(p);
	CListUI *pLrcList = static_cast<CListUI*>(p);
	if(!pLrcList) return ;

	CListTextElementUI *pListElem = new CListTextElementUI;
	assert(pListElem);
	if(!pListElem) return;

	pListElem->SetOwner(pLrcList);
	pListElem->SetText(0,mci::ToString(nIndex));
	pListElem->SetText(1,sSongName);
	pListElem->SetText(2,sArtist);

	pLrcList->Add(pListElem);	
}

void CLrcSelectWnd::ClearItem()
{
	CControlUI *p = GetPaintMgr()->FindControl(_T("lrclist"));
	assert(p);
	CListUI *pLrcList = static_cast<CListUI*>(p);
	if(!pLrcList) return ;

	pLrcList->RemoveAll();
}

void CLrcSelectWnd::InitSearchEdit(spSongInfoT spSong)
{
	CControlUI *pUISong   = GetPaintMgr()->FindControl(_T("ESongName"));
	CControlUI *pUIArtist = GetPaintMgr()->FindControl(_T("EArtistName"));
	assert(pUISong);
	assert(pUIArtist);
//	assert(spSong);
	if(pUIArtist && pUISong && spSong)
	{
		m_InPutSong = spSong->GetSongName();
		m_InPutArtist = spSong->GetArtistName();		
	}
	m_spSongForSearch = spSong;
	CEditUI *pEditSongName   = (CEditUI*)pUISong;
	CEditUI *pEditArtistName = (CEditUI*)pUIArtist;
	pEditArtistName->SetText(m_InPutArtist);
	pEditSongName->SetText(m_InPutSong);
	m_InPutSong.Empty();
	m_InPutArtist.Empty();

	CButtonUI* pSearch = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("SearchLrc")));
	CButtonUI* pOk = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("ok")));
	CButtonUI* pRel = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("releate")));
	if (m_spSongForSearch)
	{
		OnSearchLrc();
		if (pSearch && pOk && pRel)
		{
			pSearch->SetEnabled(true);
			pOk->SetEnabled(true);
			pRel->SetEnabled(true);
		}	
	}
	else
	{	
		if (pSearch && pOk && pRel)
		{
			pSearch->SetEnabled(false);
			pOk->SetEnabled(false);
			pRel->SetEnabled(false);
		}
	}
}

CString CLrcSelectWnd::GetInputSongName()
{
	CControlUI *pUISong   = GetPaintMgr()->FindControl(_T("ESongName"));
	assert(pUISong);
	if(pUISong)
	{
		CEditUI *pEditSongName   = (CEditUI*)pUISong;
		return CString(pEditSongName->GetText());
	}
	return _T("");
}

CString CLrcSelectWnd::GetInputArtistName()
{
	CControlUI *pUIArtist = GetPaintMgr()->FindControl(_T("EArtistName"));
	assert(pUIArtist);
	if(pUIArtist)
	{
		CEditUI *pEditArtistName = (CEditUI*)pUIArtist;
		return CString(pEditArtistName->GetText());
	}
	return _T("");
}

BOOL CLrcSelectWnd::Receive(SkinChangedParam param)
{
	try
	{
		if(!IsWindow()) return FALSE;
		sLog("CLrcSelectWnd::Receive");
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
		sLogError("CLrcSelectWnd::Receive");
		throw _T("CLrcSelectWnd::Receive");
		return FALSE;
	}	
}
