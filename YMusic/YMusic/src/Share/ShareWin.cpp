#include "stdafx.h"
#include "ShareWin.h"
#include "../YApp.h"

CShareWin::CShareWin()
{
	theMainWin->AddReceive(this);
}
CShareWin::~CShareWin()
{
	RemoveObserver();
}
LPCTSTR CShareWin::GetWindowClass()
{
	return _T("ShareWin");
}
LPCTSTR CShareWin::GetWindowClassName() const
{
	return CShareWin::GetWindowClass();
}

LRESULT CShareWin::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if(!RegisterSkin(_T("share-window.xml")))
		return 0;
	GetPaintMgr()->AddNotifier(this);

	FindControl(ui._title,_T("title"));
	FindControl(ui._edit,_T("input"));
	FindControl(ui._textlen,_T("textlen"));
	FindControl(ui._sharebtn,_T("sharebtn"));
	theMainWin->BroadCast();

	_nTimerId = SetTimer(1,1000);
	return 0;
}

void CShareWin::Share(spSongInfoT song)
{
	ASSERT(song);
	CString stitle;
	stitle.Format(_T("%s-%s"),song->GetSongName(),song->GetArtistName());
	ui._title->SetText(stitle);
	ui._title->SetToolTip(stitle);

	Share(url::SOT_SONG,song->GetSongId(),stitle,NULL);
}
void CShareWin::Share(url::emShareObjT type,std::size_t id,LPCTSTR title,LPCTSTR content)
{
// 	spUserContextT pContext = theRuntimeState->GetCurrentContext(NULL);
// 	spLogonUserInfoT theUser = pContext->GetUser();
// 	if(!theUser)
// 	{
// 		theMainWin->OnShowLoginWindow();
// 		return;
// 	}

	ASSERT(title);
//	if(!(type == _type && id == _id))
	{
//		_type = type;
		_id   = id;

		ui._title->SetText(title);
		ui._title->SetToolTip(title);
		ui._edit->SetText(content);
		ui._sharebtn->SetText(_T("·ÖÏí"));
	}

	if(!IsWindowVisible())
	{
		CenterWindow();
		ShowWindow();
	}
}

void CShareWin::Notify(TNotifyUI& msg)
{
	YTrace_(_T("class:%s,id:%s,msg:%s"),msg.pSender->GetClass(),msg.pSender->GetName().GetData(),msg.sType.GetData());
	if(0 == msg.sType.Compare(kClick))
		return OnClick(msg);
}
void CShareWin::OnClick(const TNotifyUI& msg)
{
	if(msg.pSender->IsName(_T("closebtn")))
		Close(0L);
	else if(msg.pSender->IsName(_T("sharebtn")))
	{
		spUserContextT pContext = theRuntimeState->GetCurrentContext(NULL);
		CString str(ui._edit->GetText());
	//	theWebServiceMgr->AsynPostRequest(url::Requester::MakeShare(pContext->GetUserId(),(str.IsEmpty() ? _T(" ") : str),_id,_type));
		Close(1L);
	}
	else if(msg.pSender->IsName(_T("sharesetting")))
	{
		CSys::OpenUrl(_T("http://www.xiami.com/share"));
	}
}
LRESULT CShareWin::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
//	if(nIDEvent == _nTimerId)
	{
		CString str;
		str.Format(_T("%d"),ui._edit->GetLimitText() - ui._edit->GetTextLength());
		ui._textlen->SetText(str.GetBuffer());
		str.ReleaseBuffer();
		return 0;
	}
	
//	SetMsgHandled(FALSE);
}
LRESULT CShareWin::OnWebService(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sLog("CShareWin::OnWebService");
	return __super::OnWebService(uMsg,wParam,lParam,bHandled);
}

void CShareWin::OnWebService(void *pRequest ,void *pProto)
{

}

LRESULT CShareWin::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg==WM_TIMER)
		return OnTimer(uMsg,wParam,lParam,bHandled);
	return __super::ProcessWindowMessage(uMsg,wParam,lParam,bHandled);
}
BOOL CShareWin::Receive(SkinChangedParam param)
{
	try
	{
		if(!IsWindow()) return FALSE;
		sLog("CShareWin::Receive");
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
		sLogError("CShareWin::Receive");
		throw _T("CShareWin::Receive");
		return FALSE;
	}	
}
