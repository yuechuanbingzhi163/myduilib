#include "stdafx.h"
#include "WebLoginWindow.h"
#include "../YApp.h"



CWebLoginWindow::CWebLoginWindow()
{
	theMainWin->AddReceive(this);
}
CWebLoginWindow::~CWebLoginWindow()
{
	RemoveObserver();
}
LPCTSTR CWebLoginWindow::GetWindowClsss() 
{
	return _T("WebLoginWindow");
}
LPCTSTR CWebLoginWindow::GetWindowClassName() const
{
	return CWebLoginWindow::GetWindowClsss();
}
LRESULT CWebLoginWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sLog("CWebLoginWindow::OnCreate");

	ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if(!RegisterSkin(_T("web-login.xml")))
		return 0;
	GetPaintMgr()->AddNotifier(this);

	theMainWin->BroadCast();

	CControlUI *pWebControl = GetPaintMgr()->FindControl(_T("ie"));
	if (pWebControl)
	{
		CWebBrowserUI *pWebBrowser = static_cast<CWebBrowserUI*>(pWebControl);
		pWebBrowser->SetWebBrowserEventHandler(this);
	}
	return 0;
}

void CWebLoginWindow::GoUrl(CString& sUrl)
{
	sLog("CWebLoginWindow::GoUrl<%s>",sUrl.GetBuffer());
	sUrl.ReleaseBuffer();
	assert(this->IsWindow());

	CControlUI *pWebControl = GetPaintMgr()->FindControl(_T("ie"));
	if (pWebControl)
	{
		CWebBrowserUI *pWebBrowser = static_cast<CWebBrowserUI*>(pWebControl);
		pWebBrowser->SetHomePage(sUrl);
		pWebBrowser->Navigate2(sUrl);
	}
	
}

void CWebLoginWindow::NavigateComplete2(IDispatch *pDisp,VARIANT *&url)
{
	CString strUrl = *url;
	CheckLoginSucceed(strUrl);
}

void CWebLoginWindow::CheckLoginSucceed(const CString& strUrl)
{
	if(strUrl.IsEmpty() || _loginSucceedURL.IsEmpty()) 
		return;
	int i = strUrl.Find(_loginSucceedURL);
	if(0 != i) 
		return;

	_sToken = strUrl.Right(strUrl.GetLength() - _loginSucceedURL.GetLength());
	_sToken.IsEmpty() ? CWebLoginWindow::Close(0) : CWebLoginWindow::Close(1);
}

CString CWebLoginWindow::GetToken()
{
	return _sToken;
}

void CWebLoginWindow::Notify(TNotifyUI& msg)
{
	YTrace_(_T("CWebLoginWindow::Notify class:%s,id:%s,msg:%s"),msg.pSender->GetClass(),msg.pSender->GetName().GetData(),msg.sType.GetData());
	CControlUI* pSender = msg.pSender;
	if (0 == msg.sType.Compare(kClick))
	{
		if (pSender->IsName(_T("close")))
			CWebLoginWindow::Close(0);
	}
}
BOOL CWebLoginWindow::Receive(SkinChangedParam param)
{
	try
	{
		if(!IsWindow()) return FALSE;
		sLog("CWebLoginWindow::Receive");
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
		sLogError("CWebLoginWindow::Receive");
		throw _T("CWebLoginWindow::Receive");
		return FALSE;
	}	
}

