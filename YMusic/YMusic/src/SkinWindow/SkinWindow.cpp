#include "stdafx.h"
#include "SkinWindow.h"

#include "../YApp.h"
#include "SysSkinLayout.h"
#include "MySkinLayout.h"
#include "SkinJsHandler.h"
#include "../Utilty/json/DataParser.h"
namespace skinJs
{
	static const TCHAR useskin[] = _T("UseSkin");
};

YSkinWindow::YSkinWindow()
{
	_pSysSkinLayout = NULL;
	_pMySkinLayout = NULL;
	_pSkinLayout = NULL;
	_pSkinJsHandler = NULL;
	_pWebSkin = NULL;
	theMainWin->AddReceive(this);
}
YSkinWindow::~YSkinWindow()
{
	RemoveObserver();
}


LPCTSTR YSkinWindow::GetWindowClsss()  
{
	return _T("YSkin");
}

LPCTSTR YSkinWindow::GetWindowClassName() const
{
	return YSkinWindow::GetWindowClsss();
}

void YSkinWindow::OnFinalMessage( HWND hWnd )
{
	if (_pSkinJsHandler)
	{
		delete _pSkinJsHandler;
		_pSkinJsHandler = NULL;
	}
	__super::OnFinalMessage(hWnd);
}

CControlUI* YSkinWindow::CreateControl(LPCTSTR pstrClass)
{
	if(_tcscmp(pstrClass,_T("SysSkin"))==0)
		return new CListUI;
	if(_tcscmp(pstrClass,_T("MySkin"))==0)
		return new CListUI;
	return NULL;
}

void YSkinWindow::Notify(TNotifyUI& msg)
{
	CControlUI* pUI = msg.pSender;

	YTrace_(_T("YSkinWindow::Notify class:%s,event:%s,name:%s"),pUI ? pUI->GetClass():_T(""),msg.sType.GetData(),pUI?pUI->GetName().GetData():_T(""));
	if (0 == msg.sType.Compare(kClick))
		OnClick(msg);
	else if(0 == msg.sType.Compare(kSelectChanged))
	{
// 		if (pUI->IsName(_T("tab1")))
// 		{
// 			_pSkinLayout->SelectItem(0);
// 		}
// 		else if (pUI->IsName(_T("tab2")))
// 		{
// 			_pSkinLayout->SelectItem(1);
// 		}
		_pSkinLayout->SelectItem(msg.pSender->GetTag());
	}
}
void YSkinWindow::OnClick(TNotifyUI& msg)
{
	if (msg.pSender->IsName(_T("close")))
	{
		Close(0L);
	}
	else if (msg.pSender->IsClass(CAlbumButtonUI::GetClassName()))
	{
		CDuiString sImage = msg.pSender->GetUserData();
		theMainWin->SetBkImage(sImage.GetData());
	}
}

LRESULT YSkinWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	sLog("YSkinWindow::OnCreate");
	ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	if(!RegisterSkin(_T("changeskinwin.xml"),0,this))
		return 0;

	GetPaintMgr()->AddNotifier(this);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	FindControl(_pSkinLayout,_T("tabs"));
	FindControl(_pSysSkinLayout,_T("SysSkinLayout"));
	FindControl(_pMySkinLayout,_T("MySkinLayout"));

	FindControl(_pWebSkin,_T("web_skin"));

	ASSERT(_pWebSkin&&_pSysSkinLayout&&_pSkinLayout&&_pMySkinLayout);

	_pSkinJsHandler = new CSkinJsHandler(_pWebSkin);

	this->InitJsHandler(_pSkinJsHandler);

	_pWebSkin->SetWebBrowserEventHandler(_pSkinJsHandler);

	theMainWin->BroadCast();

	LoadSysSkin();
	LoadMySkin();


	return 0;
}

void YSkinWindow::InitJsHandler(CSkinJsHandler* h)
{
	h->RegisterJsCallBack(skinJs::useskin,(CSkinJsHandler::JsCallBackT)&YSkinWindow::OnUseSkin);
}
void YSkinWindow::OnUseSkin(CSkinJsHandler* h,LPCTSTR param,CString& sResult)
{
	Json::Reader reader;
	Json::Value value;
	CString sParam = param;
	sParam.Replace(_T('\''),_T('\"'));
	std::string sMsgUtf8 = mci::unicode_2_ansi(sParam);
	if (!reader.parse(sMsgUtf8, value) || value.isNull()) return ;
	std::string sSkinName = JsonParser::getValue<std::string>(value,"skinName");
	std::string sSkinUrl = JsonParser::getValue<std::string>(value,"skinUrl");
	std::string sSkinDesc = JsonParser::getValue<std::string>(value,"skinDesc");
	int nSkinId = JsonParser::getValue<int>(value,"id");
	CString s_file_normal = theCacheMgr->GetWebPath() + mci::ToString(nSkinId) +_T(".jpg"); 
	if (mci::file_exist(s_file_normal))
	{
		theMainWin->SetBkImage(s_file_normal);
		return ;
	}
	if(MBRT_OK==::MsgBox(YSkinWindow::Instance()->GetHWND(),sParam,_T("OnUseSkin"),MBT_INFO|MBBT_OK|MBBT_CANCEL))
	{
		em_utility::down_http_file::down_task new_task;
		new_task.h_notify_wnd = YSkinWindow::Instance()->GetHWND();
		new_task.n_notify_msg = WM_HTTP_FILE_DOWNLOAD;

		new_task.s_url = mci::ansi_2_CString(sSkinUrl.c_str());
		new_task.s_local_file_down = theCacheMgr->GetWebPath() + mci::ansi_2_CString(sSkinName.c_str());
		new_task.s_local_file_normal = s_file_normal; 
		new_task.n_wparam = (WPARAM)(nSkinId);
		theHttpDownerMgr->add_task(new_task);
	}

}

void YSkinWindow::LoadSysSkin()
{
	CSysSkinLayoutUI* pSysSkinItem[SysRowNum] = {0,0,0};
	for (int i=0;i<SysRowNum;++i)
	{
		pSysSkinItem[i] = new CSysSkinLayoutUI;
		_pSysSkinLayout->Add(pSysSkinItem[i]);
		CDuiString sImage[SysRowNum];
		sImage[0].Format(_T("YTing/style/sys/%d/%d.jpg"),i+1,1);
		sImage[1].Format(_T("YTing/style/sys/%d/%d.jpg"),i+1,2);
		sImage[2].Format(_T("YTing/style/sys/%d/%d.jpg"),i+1,3);
		sImage[3].Format(_T("YTing/style/sys/%d/%d.jpg"),i+1,4);
		sImage[4].Format(_T("YTing/style/sys/%d/%d.jpg"),i+1,5);
		pSysSkinItem[i]->AddImage(sImage[0].GetData());
		pSysSkinItem[i]->AddImage(sImage[1].GetData());
		pSysSkinItem[i]->AddImage(sImage[2].GetData());
		pSysSkinItem[i]->AddImage(sImage[3].GetData());
		pSysSkinItem[i]->AddImage(sImage[4].GetData());
	}
}
void YSkinWindow::LoadMySkin()
{
	{
		CMySkinLayoutUI* pMySkin[MyRowNum] = {0,0,0};
		for (int i=0;i<MyRowNum;++i)
		{
			pMySkin[i] = new CMySkinLayoutUI;
			_pMySkinLayout->Add(pMySkin[i]);
			CDuiString sImage[MyRowNum];
			sImage[0].Format(_T("YTing/style/custom/%d/%d.jpg"),i+1,1);
			sImage[1].Format(_T("YTing/style/custom/%d/%d.jpg"),i+1,2);
			sImage[2].Format(_T("YTing/style/custom/%d/%d.jpg"),i+1,3);
			sImage[3].Format(_T("YTing/style/custom/%d/%d.jpg"),i+1,4);
			sImage[4].Format(_T("YTing/style/custom/%d/%d.jpg"),i+1,5);
			pMySkin[i]->AddImage(sImage[0].GetData());
			pMySkin[i]->AddImage(sImage[1].GetData());
			pMySkin[i]->AddImage(sImage[2].GetData());
			pMySkin[i]->AddImage(sImage[3].GetData());
			pMySkin[i]->AddImage(sImage[4].GetData());
		}		
	}
}
BOOL YSkinWindow::Receive(SkinChangedParam param)
{
	try
	{
		if(!IsWindow()) return FALSE;
		sLog("YSkinWindow::Receive");
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
		sLogError("YSkinWindow::Receive");
		throw _T("YSkinWindow::Receive");
		return FALSE;
	}	
}
LRESULT YSkinWindow::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg == WM_HTTP_FILE_DOWNLOAD)
		OnHttpfileDownload(uMsg,wParam,lParam,bHandled);
	return __super::ProcessWindowMessage(uMsg,wParam,lParam,bHandled);
}
LRESULT YSkinWindow::OnHttpfileDownload(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(WM_HTTP_FILE_DOWNLOAD != uMsg)
		return 0;
	if (WP_LOAD_FINISH == lParam)
	{

		int nSkinId = (int)wParam;

		if (nSkinId)
		{
			CString sImage = theCacheMgr->GetWebPath() + mci::ToString(nSkinId) +_T(".jpg"); 
			sLog("YSkinWindow::OnHttpfileDownload:%s\r\n",mci::unicode_2_ansi(sImage).c_str());
			theMainWin->SetBkImage(sImage);
		}
	}
	return 0;
}