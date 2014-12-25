#include "stdafx.h"

#include "YSongListItem.h"
#include "../YApp.h"

namespace SongListItemName
{
	static const TCHAR kNorLabelIndex[] = _T("norindexlab");
	static const TCHAR kNorLabelArtist[] = _T("noraritst");
	static const TCHAR kNorLabelName[] = _T("norname");
	static const TCHAR kNorLayout[] = _T("songitemnormallayout");
	static const TCHAR kNorBtnMenu[] = _T("normenu");

	static const TCHAR kItemIcon[] = _T("song_ico");
	static const TCHAR kPushLabelIndex[] = _T("pusindexlab");
	static const TCHAR kPushLabelName[] = _T("pusname");
	static const TCHAR kPushLabelArtist[] = _T("pusaritst");
	static const TCHAR kPushLabelAlbum[] = _T("pusalbum");
	static const TCHAR kPushBtnDown[] = _T("pusdownbtn");
	static const TCHAR kPushCheckCollect[] = _T("pusheartbtn");
	static const TCHAR kPushBtnShare[] = _T("pussharebtn");
	static const TCHAR kPushBtnRemove[] = _T("pusremovebtn");
	static const TCHAR kPushLayout[] = _T("songitemexplayout");
};

const int SongItemHeight = 25;
const int SongItemExpHeight = 40;
YSongListItemUI::YSongListItemUI()
{
	this->SetFixedHeight(SongItemHeight);
	CDialogBuilder builder;
	builder.Create(_T("song-list-item.xml"),0,NULL,NULL,this);

	InitVar();
}

YSongListItemUI::~YSongListItemUI()
{

}

LPCTSTR YSongListItemUI::GetClassName()
{
	return _T("SongListItemUI");
}

LPCTSTR YSongListItemUI::GetClass() const
{
	return YSongListItemUI::GetClassName();
}

void YSongListItemUI::SetSong(spSongInfoT song)
{
	ASSERT(song);
	_spSongInfo = song;

	Update();
}
spSongInfoT YSongListItemUI::GetSong() const
{
	return _spSongInfo;
}

static bool OnNorMenuButtonEvent(void* event) {
	if( ((TEventUI*)event)->Type == UIEVENT_BUTTONDOWN ) {
		CControlUI* pButton = ((TEventUI*)event)->pSender;
		if( pButton != NULL ) {
			YSongListItemUI* pListElement = (YSongListItemUI*)(pButton->GetTag());
			if( pListElement != NULL ) pListElement->DoEvent(*(TEventUI*)event);
		}
	}
	return true;
}

void YSongListItemUI::InitVar()
{
	_pNorLabelIndex = static_cast<CLabelUI*>(FindSubControl(SongListItemName::kNorLabelIndex));
	_pNorLabelName = static_cast<CLabelUI*>(FindSubControl(SongListItemName::kNorLabelName));
	_pNorLabelAritst = static_cast<CLabelUI*>(FindSubControl(SongListItemName::kNorLabelArtist));
	_pNormalLayout = static_cast<CHorizontalLayoutUI*>(FindSubControl(SongListItemName::kNorLayout));
	_pNorButtonMenu = static_cast<CButtonUI*>(FindSubControl(SongListItemName::kNorBtnMenu));
	ASSERT(_pNorLabelIndex);
	ASSERT(_pNorLabelName);
	ASSERT(_pNorLabelAritst);
	ASSERT(_pNormalLayout);
	ASSERT(_pNorButtonMenu);
	_pNorLabelIndex->SetTag((UINT_PTR)this);
	_pNorLabelName->SetTag((UINT_PTR)this);
	_pNorLabelAritst->SetTag((UINT_PTR)this);
	_pNormalLayout->SetTag((UINT_PTR)this);
	_pNorButtonMenu->SetTag((UINT_PTR)this);
	_pNorButtonMenu->OnEvent += MakeDelegate(&OnNorMenuButtonEvent);

	_pItemIco = FindSubControl(SongListItemName::kItemIcon);
	_pPushLabelIndex = static_cast<CLabelUI*>(FindSubControl(SongListItemName::kPushLabelIndex));
	_pPushLabelName = static_cast<CLabelUI*>(FindSubControl(SongListItemName::kPushLabelName));
	_pPushLabelAritst = static_cast<CLabelUI*>(FindSubControl(SongListItemName::kPushLabelArtist));
	_pPushLabelAlbum = static_cast<CLabelUI*>(FindSubControl(SongListItemName::kPushLabelAlbum));
	_pPushDownLoad = static_cast<CButtonUI*>(FindSubControl(SongListItemName::kPushBtnDown));
	_pPushCollect = static_cast<CCheckBoxUI*>(FindSubControl(SongListItemName::kPushCheckCollect));
	_pPushShare = static_cast<CButtonUI*>(FindSubControl(SongListItemName::kPushBtnShare));
	_pPushRemove = static_cast<CButtonUI*>(FindSubControl(SongListItemName::kPushBtnRemove));
	_pPushLayout = static_cast<CHorizontalLayoutUI*>(FindSubControl(SongListItemName::kPushLayout));
	ASSERT(_pItemIco);
	ASSERT(_pPushLabelIndex);
	ASSERT(_pPushLabelName);
	ASSERT(_pPushLabelAritst);
	ASSERT(_pPushLabelAlbum);
	ASSERT(_pPushDownLoad);
	ASSERT(_pPushCollect);
	ASSERT(_pPushShare);
	ASSERT(_pPushRemove);
	ASSERT(_pPushLayout);

	_pItemIco->SetTag((UINT_PTR)this);
	_pPushLabelIndex->SetTag((UINT_PTR)this);
	_pPushLabelName->SetTag((UINT_PTR)this);
	_pPushLabelAritst->SetTag((UINT_PTR)this);
	_pPushLabelAlbum->SetTag((UINT_PTR)this);
	_pPushDownLoad->SetTag((UINT_PTR)this);
	_pPushCollect->SetTag((UINT_PTR)this);
	_pPushShare->SetTag((UINT_PTR)this);
	_pPushRemove->SetTag((UINT_PTR)this);
	_pPushLayout->SetTag((UINT_PTR)this);

	_pPushLayout->SetVisible(false);

	_pPushRemove->OnNotify += MakeDelegate(this,&YSongListItemUI::DoNotify);
	_pPushCollect->OnNotify += MakeDelegate(this,&YSongListItemUI::DoNotify);
	_pPushShare->OnNotify += MakeDelegate(this,&YSongListItemUI::DoNotify);
	_pPushDownLoad->OnNotify += MakeDelegate(this,&YSongListItemUI::DoNotify);
}
void YSongListItemUI::Update()
{
	if (_spSongInfo)
	{
		_pNorLabelName->SetText(_spSongInfo->GetSongName());
		_pNorLabelAritst->SetText(_spSongInfo->GetArtistName());
		_pNorLabelName->SetToolTip(_spSongInfo->GetSongName());
		_pNorLabelAritst->SetToolTip(_spSongInfo->GetArtistName());
		
		_pPushLabelName->SetText(_spSongInfo->GetSongName());
		_pPushLabelAritst->SetText(_spSongInfo->GetArtistName());
		_pPushLabelAlbum->SetText(_spSongInfo->GetAlbumName());
		_pPushLabelName->SetToolTip(_spSongInfo->GetSongName());
		_pPushLabelAritst->SetToolTip(_spSongInfo->GetArtistName());
		_pPushLabelAlbum->SetToolTip(_spSongInfo->GetAlbumName());
//		_pPushCollect->Selected()
	}
	else
	{
		_pNorLabelName->SetText(UnKnown);
		_pNorLabelAritst->SetText(UnKnown);
		_pNorLabelName->SetToolTip(UnKnown);
		_pNorLabelAritst->SetToolTip(UnKnown);

		_pPushLabelName->SetText(UnKnown);
		_pPushLabelAritst->SetText(UnKnown);
		_pPushLabelAlbum->SetText(UnKnown);
		_pPushLabelName->SetToolTip(UnKnown);
		_pPushLabelAritst->SetToolTip(UnKnown);
		_pPushLabelAlbum->SetToolTip(UnKnown);
	}
}
void YSongListItemUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
	ASSERT(GetOwner());

// 	spSongInfoT spSong = thePlayCenter->GetCurrentPlayingSong();
// 	int nCurPlayListLocalId = thePlayCenter->GetListIndex();
	spSongInfoT spSong = thePlayListMgr->GetCurSong();
	spPlayListInfoT spList = thePlayListMgr->GetCurList();
	if (spSong&&*spSong==*_spSongInfo&&spList&&spList->GetLocalId()==((CListUI*)GetOwner())->GetTag())
	{
		if (SongItemExpHeight != GetFixedHeight())
		{
			SetFixedHeight(SongItemExpHeight);
			_pNormalLayout->SetVisible(false);
			_pPushLayout->SetVisible(true);
			if (_spSongInfo)
			{
				_pPushDownLoad->SetVisible(!_spSongInfo->IsLocal());
				_pPushShare->SetVisible(!_spSongInfo->IsLocal());
			}
		}
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		if(pInfo)
		{
			DWORD dwSelBkColor = ((CListUI*)GetOwner())->GetSelectedItemBkColor();

			SetBkColor(dwSelBkColor);
		}
		ChangeIcon(thePlayer->GetStatus());
	}
	else
	{
		if (IsSelected() && ((CListUI*)GetOwner())->GetSelCount() == 1)
		{
			_pPushLayout->SetVisible(false);
			_pNormalLayout->SetVisible(true);
//			_pNorButtonMenu->SetVisible(true);

		}else
		{
			_pPushLayout->SetVisible(false);
			_pNormalLayout->SetVisible(true);
//			_pNorButtonMenu->SetVisible(false);
		}
		if(SongItemHeight != GetFixedHeight())
			SetFixedHeight(SongItemHeight);
		
		ChangeIcon(em_stop);
	}
	int nIndex = ((CListUI*)GetOwner())->GetItemIndex(this);
	_pNorLabelIndex->SetText(mci::ToString(nIndex+1));
	_pPushLabelIndex->SetText(mci::ToString(nIndex+1));

	

	__super::DoPaint(hDC,rcPaint);
}

void YSongListItemUI::ChangeIcon(int bPlaying /* = em_play */)
{
	static CDuiString sPlayImg = _pItemIco->GetBkImage();
	static CDuiString sPauseImg = _pItemIco->GetUserData();
	if(bPlaying==em_stop)
	{
		_pItemIco->SetVisible(false);
		_pPushLabelIndex->SetVisible(true);
	}
	else if (em_play==bPlaying)
	{
		_pItemIco->SetVisible(true);
		_pItemIco->SetBkImage(sPlayImg);
		_pItemIco->SetUserData(sPauseImg);
		_pPushLabelIndex->SetVisible(false);
	}
	else if(em_pause==bPlaying)
	{
		_pItemIco->SetVisible(true);
		_pItemIco->SetBkImage(sPauseImg);
		_pItemIco->SetUserData(sPlayImg);
		_pPushLabelIndex->SetVisible(false);
	}

	YTrace_(_T("bPlaying=%d\n"),bPlaying);
}


void YSongListItemUI::DoEvent(TEventUI& event)
{
	YTrace_(_T("doEvent:[type%d][name:%s]\n"),event.Type,event.pSender?event.pSender->GetName().GetData():_T(""));

	if(event.Type==UIEVENT_MOUSELEAVE)
	{
		_pNorButtonMenu->SetVisible(false);
	}
	else 
	{
		_pNorButtonMenu->SetVisible(true);
	}
	__super::DoEvent(event);

}

bool YSongListItemUI::DoNotify(void* param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;
	if (pMsg&&pMsg->sType.Compare(kClick)==0)
	{
		CControlUI* pSender = pMsg->pSender;
		if (pSender==_pPushDownLoad)
		{
			MenuCmdMsg msg;
			msg.sItemName = varSongItemMenu::kDownLoad;
			theMainWin->OnMenuCommandSongItem(&msg);
		}
		else if (pSender==_pPushCollect)
		{
			MenuCmdMsg msg;
			msg.sItemName = varSongItemMenu::kCollect;
			theMainWin->OnMenuCommandSongItem(&msg);
		}
		else if (pSender==_pPushShare)
		{
			MenuCmdMsg msg;
			msg.sItemName = varSongItemMenu::kShare;
			theMainWin->OnMenuCommandSongItem(&msg);
		}
		else if (pSender==_pPushRemove)
		{
			MenuCmdMsg msg;
			msg.sItemName = varSongItemMenu::kRemove;
			theMainWin->OnMenuCommandSongItem(&msg);		
		}
	}
	return true;
}