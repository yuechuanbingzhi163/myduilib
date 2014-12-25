#include "StdAfx.h"


#include "mainwindow.h"

#include "ChatSocket.h"
#include "ListenSocket.h"
#include "Msg.h"
#include "tray.h"

#include "log.h"
#include "notice.h"
#include "config.h"

namespace sysbtn
{
	const TCHAR* const kMinBtn = _T("minbtn");
	const TCHAR* const kMaxBtn = _T("maxbtn");
	const TCHAR* const kCloseBtn = _T("closebtn");
	const TCHAR* const kRestoreBtn = _T("restorebtn");
};
namespace search
{
	const TCHAR* const kSearchEdit = _T("search_edit");
	const TCHAR* const kSearchBtn = _T("searchbtn");
};
namespace option
{
	const TCHAR* const kFriendOpt = _T("friendopt");
	const TCHAR* const kGroupOpt = _T("groupopt");
	const TCHAR* const kMicroBlogOpt = _T("microblogopt");
	const TCHAR* const kGroupChatOpt = _T("groupchatopt");
};
namespace tab
{
	const TCHAR* const kFriendList = _T("friend_list");
	const TCHAR* const kGroupList = _T("group_list");
	const TCHAR* const kBlogList = _T("blog_list");
	const TCHAR* const kTabLayout = _T("tabs");
};
namespace bottom
{
	const TCHAR* const kOpenMusicPlayer = _T("music");
	const TCHAR* const kOpenChatRoom = _T("chatroom");
};
namespace chatview
{
	const TCHAR* kLogoBtn = _T("logo");
	const TCHAR* kNameTxt = _T("nickname");
	const TCHAR* kDescEdt = _T("description");
	const TCHAR* kMinBtn = _T("minbtn");
	const TCHAR* kMaxBtn = _T("maxbtn");
	const TCHAR* kResBtn = _T("restorebtn");
	const TCHAR* kCloseBtn = _T("closebtn");

	const TCHAR* kHomeBtn = _T("home");
	const TCHAR* kHideBtn = _T("hide");
	const TCHAR* kViewEdt = _T("view_richedit");

	const TCHAR* kFontHor = _T("fontbar");
	const TCHAR* kFontTypeCom = _T("font_type");
	const TCHAR* kFontSizeCom = _T("font_size");
	const TCHAR* kBoldOpt = _T("boldbtn");
	const TCHAR* kItalicOpt = _T("italicbtn");
	const TCHAR* kUnderlineOpt = _T("underlinebtn");
	const TCHAR* kColorOpt = _T("colorbtn");

	const TCHAR* kFontOpt = _T("fontbtn");
	const TCHAR* kEmotionOpt = _T("emotionbtn");
	const TCHAR* kImageOpt = _T("imagebtn");
	const TCHAR* kShakeOpt = _T("shockbtn");
	const TCHAR* kScreenShortOpt = _T("screenshotsbtn");
	const TCHAR* kEmptyMsgViewOpt = _T("empty_richedit");

	const TCHAR* kInputEdt = _T("input_richedit");
	const TCHAR* kSendBtn = _T("sendbtn");
};

#define DBFILE   _T("\\users.db")
#define ID_TRAY   1024
#define WM_TRAY  WM_USER + 1
bool theConning = false;
CMainWindow*  theApp = NULL;
const LPCTSTR CMainWindow::CLASSNAME = _T("Client");

CMainWindow::CMainWindow()
	: m_pSocket(NULL)
	, m_pChatMsg(NULL)
	, m_pChatSocket(NULL)
	, m_IpAddress(_T("localhost"))
	, emotion_timer_start_(false)
	, text_color_(0xFF000000)
	, bold_(false)
	, italic_(false)
	, underline_(false)
	, font_size_(12)
	, font_face_name_(_T("微软雅黑"))
	, m_pMsgRcv(NULL)
	, _parent_node_online_friends(NULL)
	, _parent_node_offline_friends(NULL)
	, _parent_node_all_firends(NULL)
	, m_pLogin(NULL)
	, m_pInput(NULL)
	, m_pMsgView(NULL)
{
	ASSERT(BuildEnvironment());
	ZeroMemory(&_ui,sizeof(IControls));
}
CMainWindow::~CMainWindow()
{

}
LPCTSTR CMainWindow::GetWindowClassName() const
{
	return CLASSNAME;
}
CControlUI* CMainWindow::CreateControl(LPCTSTR pstrClass)
{
	try
	{
		if(0 == _tcscmp(pstrClass,_T("GroupList")))
			return new CUserListUI(*(GetPaintMgr()));
		if(0 == _tcscmp(pstrClass,_T("MicroBlog")))
			return new CUserListUI(*(GetPaintMgr()));
		if(0 == _tcscmp(pstrClass,_T("FriendList")))
			return new CFriendsUI(*(GetPaintMgr()));
		return NULL;
	}
	catch (...)
	{
		throw _T("CMainWindow::CreateControl");
	}
}
void CMainWindow::Notify( TNotifyUI &msg )
{
	try
	{
		if ( 0 == _tcscmp(msg.sType,_T("windowinit")) )
			TypeInitWindow(msg);
		else if( 0 == _tcscmp(msg.sType,_T("click")))
			TypeClick(msg);
		else if( 0 == _tcscmp(msg.sType,_T("selectchanged")))
			TypeSelectedChange(msg);
		else if( 0 == _tcscmp(msg.sType,_T("itemclick")))
			TypeItemClick(msg);
		else if( 0 == _tcscmp(msg.sType,_T("itemactivate")))
			TypeItemActive(msg);
		else if( _tcsicmp(msg.sType, _T("return")) == 0 ) 
		{
			if (_tcsicmp(msg.pSender->GetName(), chatview::kInputEdt) == 0)
			{
				OnSendMsg(msg);
			}
		}
	}
	catch (...)
	{
		throw _T("CMainWindow::Notify");
	}
}
void CMainWindow::TypeClick(TNotifyUI& msg)
{
	if(msg.pSender->IsName(sysbtn::kCloseBtn))
	{
		OnExit(msg);
	}
	else if (msg.pSender->IsName(sysbtn::kMaxBtn))
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return;
	}
	else if (msg.pSender->IsName(sysbtn::kRestoreBtn))
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; 
	}
	else if (msg.pSender->IsName(sysbtn::kMinBtn))
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
	}
	else if(msg.pSender->IsName(chatview::kSendBtn))
	{
		OnSendMsg(msg);
	}
	else if (msg.pSender->IsName(chatview::kEmptyMsgViewOpt))
	{
		m_lockEditView.Lock();
		int lSelBegin,lSelEnd;
		lSelEnd = lSelBegin = m_pMsgView->GetTextLength();
		m_pMsgView->SetSel(0, lSelEnd);//选择全部	
		m_pMsgView->ReplaceSel(_T(""), false);
		m_lockEditView.UnLock();
	}
	else if (msg.pSender->IsName(chatview::kShakeOpt))
	{
		CString shake = BuildShakeMsg();
		SendMsg(shake);
		FormatMsgToEditView(theUser.m_loadName,mci::format_currenttime(),_T("您发了一个抖动窗口"));
	}
}
void CMainWindow::OnExit(TNotifyUI& msg)
{
	CString sExit = BulidCancelMsg();
	SendMsg(sExit);
	m_tray.Remove();
	CString sPath = CPaintManagerUI::GetInstancePath();
	sPath += CLASSNAME;
	sPath += _T("\\config\\config.ini");
	CString sIP = Config::ReadIP(sPath);
	CString sPort = Config::ReadPort(sPath);
	Config::WriteIP(sIP,sPath);
	Config::WritePort(sPort,sPath);
	ClearAllVar();
	AnimateWindow(2000,0x00000010|0x00010000);
	::SendMessage(m_hWnd,WM_SYSCOMMAND,SC_CLOSE,0);
}
void CMainWindow::TypeInitWindow(TNotifyUI& msg)
{
	InitAllControls();
	SetIcon(LoadIcon(IDI_MAIN_ICON));
	m_pMsgRcv = new CMsg;
	m_pChatMsg = new CMsg;
	m_pLogin = new CLoginWnd;
	if (m_pLogin==NULL)
	{
		PostQuitMessage(0L);
	}
	m_tray.Remove();
	m_tray.Setup(this->m_hWnd,IDI_MAIN_ICON,WM_TRAY,::LoadIcon(GetPaintMgr()->GetInstance(),MAKEINTRESOURCE(IDI_MAIN_ICON)),_T("虾聊2013"));
	
	m_pLogin->Create(m_hWnd,_T("登录"),UI_WNDSTYLE_DIALOG,WS_EX_WINDOWEDGE);
	m_pLogin->CenterWindow();
	m_pLogin->ShowModal();

// 	if (!ConnectToSrv())
// 	{
// 		PostQuitMessage(0L);
// 	}
	
// 	CString s = BulidLoadMsg();
// 	SendMsg(s);
}
void CMainWindow::TypeSelectedChange(TNotifyUI& msg)
{
	if (msg.pSender->IsName(option::kFriendOpt))
	{
		SetPage(0);
	}
	else if (msg.pSender->IsName(option::kGroupOpt))
	{
		SetPage(1);
	}
	else if (msg.pSender->IsName(option::kMicroBlogOpt))
	{
		SetPage(2);
	}
	else if (msg.pSender->IsName(option::kGroupChatOpt))
	{
		SetPage(3);
	}
	else if (msg.pSender->IsName(chatview::kFontOpt))
	{
		CControlUI* pUI = NULL;
		pUI = GetPaintMgr()->FindControl(chatview::kFontHor);
		if(pUI) pUI->SetVisible(static_cast<COptionUI*>(msg.pSender)->IsSelected());
	}
	else if (msg.pSender->IsName(chatview::kColorOpt))
	{
		static CHOOSECOLOR cc;
		static COLORREF crCustCoLors[16];
		cc.lStructSize=sizeof(CHOOSECOLOR);
		cc.hwndOwner= this->m_hWnd;
		cc.hInstance=NULL;
		cc.rgbResult = RGB(0,0,0);
		cc.lpCustColors=crCustCoLors;
		cc.Flags=CC_RGBINIT|CC_FULLOPEN | CC_ANYCOLOR; 
		cc.lCustData=0;
		cc.lpfnHook=NULL;
		cc.lpTemplateName=NULL;
		if(TRUE == ChooseColor(&cc))
		{ 
			TCHAR szBuf[MAX_PATH] = {0};
#if defined(UNDER_CE)
			_stprintf(szBuf, _T("FF%02X%02X%02X"), GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult));
#else
			_stprintf_s(szBuf, MAX_PATH - 1, _T("FF%02X%02X%02X"), GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult));
#endif
			LPTSTR pstr = NULL;
			text_color_ = _tcstoul(szBuf, &pstr, 16);
		}
	}
	else if (msg.pSender->IsName(bottom::kOpenMusicPlayer))
	{
//		bool bSelected = static_cast<COptionUI*>(msg.pSender)->IsSelected();
		HWND hWndPlayer = FindWindow(_T("xiating"),NULL);
		if(hWndPlayer)
		{
			::ShowWindow(hWndPlayer,SW_SHOWNORMAL);
			::SetActiveWindow(hWndPlayer);
			::BringWindowToTop(hWndPlayer);
			return ;
		}
		CString sPlayer = CPaintManagerUI::GetInstancePath();
		if(sPlayer.Right(1)!=_T("\\"))
			sPlayer += _T("\\");
		sPlayer += _T("xiating.exe");
		CSys::OpenUrl(sPlayer);
	}
}
void CMainWindow::TypeItemActive(TNotifyUI& msg)
{
	if (_ui._pUserTab != NULL)
	{
		if (_ui._pUserTab->GetCurSel() == 0)
		{
			if ((_ui._pFriendTree != NULL) &&  _ui._pFriendTree->GetItemIndex(msg.pSender) != -1)
			{
				if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
				{
					Node* node = (Node*)msg.pSender->GetTag();

					CControlUI* background = GetPaintMgr()->FindControl(_T("bg"));
					if (!_ui._pFriendTree->CanExpand(node) && (background != NULL))
					{
/*						FriendListItemInfo friend_info;

						for (std::vector<FriendListItemInfo>::const_iterator citer = friends_.begin(); citer != friends_.end(); ++citer)
						{
							if (_tcsicmp(citer->id, node->data().value) == 0)
							{
								friend_info = *citer;
								break;
							}
						}
						TCHAR szBuf[MAX_PATH] = {0};
						if (_tcslen(background->GetBkImage()) > 0)
						{
#if defined(UNDER_WINCE)
							_stprintf(szBuf, _T("bg%d.png"), bk_image_index_);
#else
							_stprintf_s(szBuf, MAX_PATH - 1, _T("bg%d.png"), bk_image_index_);
#endif
						}
						*/
						int iIndex = _ui._pFriendTree->GetItemIndex(msg.pSender);
						CControlUI* pControl = _ui._pFriendTree->GetItemAt(iIndex);
						User friend_;
						if( pControl != NULL) {
							IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
							if( pListItem != NULL )
							{
								CListContainerElementUI* pFriendListItem = static_cast<CListContainerElementUI*>(pControl);
								Node* node = (Node*)pControl->GetTag();
								if ((pFriendListItem != NULL) && (node != NULL) && !node->folder())
								{
									friend_ = _ui._pFriendTree->GetUser(node);
								}
							}
						}
						//if(friend_.m_userState==OFFLINE) return ;
						ChatDialog* pChatDlg = NULL;
						if((pChatDlg=FindChatBox(friend_.m_loadName))!=NULL){  pChatDlg->ShowWindow(true);pChatDlg->BringWindowToTop(); return ;}
						ChatDialog* pChatDialog = new ChatDialog(_T(""), background->GetBkColor(), theUser, friend_);
						if( pChatDialog == NULL )
							return;
#if defined(WIN32) && !defined(UNDER_CE)
						pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP,  NULL, 0, 0, 0, 0);
#else
						pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
#endif
						pChatDialog->SetWindowText(friend_.m_loadName);
//						skin_changed_observer_.AddReceiver(pChatDialog);

						pChatDialog->CenterWindow();
						::ShowWindow(*pChatDialog, SW_SHOW);
						//装入链表
						thelockChatDlgList.Lock();
						theChatDialogList.AddTail(pChatDialog);
						thelockChatDlgList.UnLock();
					}
				}
			}
		}
	}
}
void CMainWindow::TypeItemClick(TNotifyUI& msg)
{
	if (_ui._pUserTab != NULL)
	{
		if (_ui._pUserTab->GetCurSel() == 0)
		{
			if ((_ui._pFriendTree != NULL) &&  _ui._pFriendTree ->GetItemIndex(msg.pSender) != -1)
			{
				if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
				{
					Node* node = (Node*)msg.pSender->GetTag();

					if (_ui._pFriendTree ->CanExpand(node))
					{
						_ui._pFriendTree ->SetChildVisible(node, !node->data().child_visible_);
					}
				}
			}
		}
	}
}

void CMainWindow::SetPage(int iIndex)
{
	if(_ui._pUserTab) { 
		_ui._pUserTab->SelectItem(iIndex);
		if(iIndex==0) _ui._pFriendOpt->Selected(true);
		else if(iIndex==1) _ui._pGroupOpt->Selected(true);
		else if(iIndex==2) _ui._pMicroBlogOpt->Selected(true);
		else _ui._pGroupChatOpt->Selected(true);
	}
}
template<typename T>
void CMainWindow::FindControl(T& pctrl,LPCTSTR pstrName) const
{
	pctrl = static_cast<T>(GetPaintMgr()->FindControl(pstrName));
}
LRESULT CMainWindow::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(WM_TIMER==uMsg)
		OnTimer();
	if(WM_TRAY==uMsg)
		OnTray(uMsg,wParam,lParam,bHandled);
	return 0;
}
LRESULT CMainWindow::OnTray(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//wParam接收的是图标的ID， 
	//而lParam接收的是鼠标的行为 
//  	if(wParam!=IDI_MAIN_ICON||wParam!=IDI_ICON_TRAY_2) 
//  		return 1; 
	switch (lParam)
	{
	case WM_MOUSEMOVE:
	case WM_MOUSEHOVER:
		{
// 			theLockMsgQue.Lock();
// 			if(theMsgQueue.size()>0)
// 			{
// 				CTray* pTray = CTray::GetInstance(m_hWnd);
// 				pTray->UpdateMsgList();
// 			}
// 			theLockMsgQue.UnLock();			
			DUI__Trace(_T("TRACE test"));
		}
		break;
	case WM_LBUTTONDOWN:
		{
			theLockMsgQue.Lock();
			typedef std::map<CString,std::vector<CMsg*> >::reverse_iterator iter;
			iter it = theMsgQueue.rbegin();
			if(it!=theMsgQueue.rend())
			{
				ChatDialog* pChatDialog = NULL;
				if ((pChatDialog=FindChatBox(it->first))==NULL)
				{
					User friend_;
					FindUserFromList(it->first,friend_);
					ChatDialog* pChatDialog = new ChatDialog(_T(""),0, theUser, friend_);
					if( pChatDialog == NULL )
						return 0;
#if defined(WIN32) && !defined(UNDER_CE)
					pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP,  NULL, 0, 0, 0, 0);
#else
					pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
#endif
					pChatDialog->SetWindowText(friend_.m_loadName);
					//skin_changed_observer_.AddReceiver(pChatDialog);

					pChatDialog->CenterWindow();
					::ShowWindow(*pChatDialog, SW_SHOW);
					//装入链表
					thelockChatDlgList.Lock();
					theChatDialogList.AddTail(pChatDialog);
					thelockChatDlgList.UnLock();

				}
			}
			theLockMsgQue.UnLock();
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			theLockMsgQue.Lock();
			typedef std::map<CString,std::vector<CMsg*> >::iterator iter;
			for (iter it=theMsgQueue.begin();it!=theMsgQueue.end();++it)
			{
				ChatDialog* pChatDialog = NULL;
				if ((pChatDialog=FindChatBox(it->first))==NULL)
				{
					User friend_;
					FindUserFromList(it->first,friend_);
					ChatDialog* pChatDialog = new ChatDialog(_T(""),0, theUser, friend_);
					if( pChatDialog == NULL )
						return 0;
#if defined(WIN32) && !defined(UNDER_CE)
					pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP,  NULL, 0, 0, 0, 0);
#else
					pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
#endif
					pChatDialog->SetWindowText(friend_.m_loadName);
					//skin_changed_observer_.AddReceiver(pChatDialog);

					pChatDialog->CenterWindow();
					::ShowWindow(*pChatDialog, SW_SHOW);
					//装入链表
					thelockChatDlgList.Lock();
					theChatDialogList.AddTail(pChatDialog);
					thelockChatDlgList.UnLock();
					
				}
			}
			theLockMsgQue.UnLock();
		}
		break;
	}
	return 0;
}
LRESULT CMainWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	try
	{
		ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		if(!RegisterSkin(_T("mainwindow.xml"),0,this))
			return 0;

		GetPaintMgr()->AddNotifier(this);
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
			rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);
	}
	catch (...)
	{
		throw _T("CMainWindow::OnCreate");
	}
	return 0;
}
LRESULT CMainWindow::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		::PostQuitMessage(0L);
		m_tray.Remove();
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
		if( !bZoomed ) {
			CControlUI* pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(sysbtn::kMaxBtn));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(sysbtn::kRestoreBtn));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(sysbtn::kMaxBtn));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(sysbtn::kRestoreBtn));
			if( pControl ) pControl->SetVisible(false);
		}
	}
	return lRes;
}
LRESULT CMainWindow::OnTimer()
{
	theLockMsgQue.Lock();
	if(theMsgQueue.size()<=0)
		m_tray.StopFlash(m_hWnd,1234);
	else 
		m_tray.DoFlash();
	theLockMsgQue.UnLock();

	return 0;
}
bool CMainWindow::BuildEnvironment()
{
	CDuiString sPath = CPaintManagerUI::GetInstancePath();

	sPath += CLASSNAME ;	
	ASSERT(mci::make_dir(sPath.GetData()));

	CDuiString sLogPath = sPath + _T("\\log");
	ASSERT(mci::make_dir(sLogPath.GetData()));
// 	CDuiString sLogDir = sLogPath + _T("\\");
// 	TCHAR sDir[MAX_PATH]={0};
// 	_stprintf_s(sDir, MAX_PATH - 1, _T("%s"), sLogDir.GetData());
// 	theLog = new CLog(_T("client_log.log"),sDir);
// 	if(theLog==NULL)
// 		ASSERT(FALSE&&"无法创建日志对象");

	CDuiString sConfig = sPath + _T("\\config");
	ASSERT(mci::make_dir(sConfig.GetData()));

	CDuiString sDBPath = sPath + _T("\\db");
	ASSERT(mci::make_dir(sDBPath.GetData()));

//	CDuiString sDBFile = sDBPath + DBFILE;

//	sLog(_T("配置环境信息..."));
	return TRUE;
}
void CMainWindow::InitAllControls()
{
//	sLog(_T("绑定控件..."));
	FindControl(_ui._pMinBtn,sysbtn::kMinBtn);
	FindControl(_ui._pMaxBtn,sysbtn::kMaxBtn);
	FindControl(_ui._pResBtn,sysbtn::kRestoreBtn);
	FindControl(_ui._pCloseBtn,sysbtn::kCloseBtn);
	ASSERT(_ui._pMinBtn);ASSERT(_ui._pMaxBtn);
	ASSERT(_ui._pResBtn);ASSERT(_ui._pCloseBtn);

	FindControl(_ui._pSearchRichEdit,search::kSearchEdit);
	FindControl(_ui._pSearchBtn,search::kSearchBtn);
	ASSERT(_ui._pSearchRichEdit);ASSERT(_ui._pSearchBtn);

	FindControl(_ui._pFriendOpt,option::kFriendOpt);
	FindControl(_ui._pGroupOpt,option::kGroupOpt);
	FindControl(_ui._pMicroBlogOpt,option::kMicroBlogOpt);
	FindControl(_ui._pGroupChatOpt,option::kGroupChatOpt);
	ASSERT(_ui._pFriendOpt);ASSERT(_ui._pGroupOpt);
	ASSERT(_ui._pMicroBlogOpt);ASSERT(_ui._pGroupChatOpt);

	FindControl(_ui._pFriendTree,tab::kFriendList);
	FindControl(_ui._pGroupList,tab::kGroupList);
	FindControl(_ui._pBlogList,tab::kBlogList);
	FindControl(_ui._pUserTab,tab::kTabLayout);
	ASSERT(_ui._pFriendTree);ASSERT(_ui._pGroupList);
	ASSERT(_ui._pBlogList);ASSERT(_ui._pUserTab);

	FindControl(m_pMsgView,chatview::kViewEdt);
	FindControl(m_pInput,chatview::kInputEdt);
	ASSERT(m_pInput); ASSERT(m_pMsgView);
	//更新列表
	UpdateUserInfo();
}
void CMainWindow::ShowMsg(CMsg* pMsg)
{
	CString sMsg = pMsg->m_nomalMsg.msgItem;
	CString sTime = pMsg->m_nomalMsg.sndTime;
	CString sWho = pMsg->m_nomalMsg.who;

	FormatMsgToEditView(sWho.IsEmpty()?_T("(系统消息)"):sWho,sTime,sMsg);
	if(theApp->IsIconic())
		theApp->FlashWindow(TRUE);
}
bool CMainWindow::ConnectToSrv(LPCTSTR lpIP,UINT nPort)
{
	CString sPath = CPaintManagerUI::GetInstancePath();
	sPath += CLASSNAME;
	sPath += _T("\\config\\config.ini");
	CString sIP = Config::ReadIP(sPath);
	CString sPort = Config::ReadPort(sPath);
	CString ssIP;
	if(isalpha(sIP.GetAt(0)))
	{//是域名
		LPHOSTENT     lphostent; 
		lphostent = ::gethostbyname(mci::unicode_2_ansi(sIP).c_str());
		if(lphostent)
		{
			///连接
			struct in_addr ip_addr;
			memcpy(&ip_addr,lphostent->h_addr_list[0],4);///h_addr_list[0]里4个字节,每个字节8位
			ssIP.Format(_T("%d.%d.%d.%d"),
				ip_addr.S_un.S_un_b.s_b1,
				ip_addr.S_un.S_un_b.s_b2,
				ip_addr.S_un.S_un_b.s_b3,
				ip_addr.S_un.S_un_b.s_b4);
		}
		else ssIP = _T("127.0.0.1");
	}
	m_pSocket = new CChatSocket(this);
	if(!m_pSocket->Create())
	{
		delete m_pSocket;
		m_pSocket = NULL;
//		sLogError(_T("创建套接字失败..."));
		MessageBox(m_hWnd,_T("创建套接字失败!"),_T("Error"),MB_ICONERROR|MB_OK);
		return false;
	}
	while(!m_pSocket->Connect(ssIP,mci::ToUInt(sPort)))
	{
//		sLog(_T("尝试连接到服务器..."));
		if(MessageBox(m_hWnd,_T("无法连接服务器!\r\n重新连接?"),_T("连接"),MB_YESNO)==IDNO)
		{
			delete m_pSocket;
			m_pSocket = NULL;
			return false;
//			sLogError(_T("无法连接服务器..."));
		}
	}
	theConning = true;
	return true;
}
void CMainWindow::SendMsg(CString& strText)
{

	int nLen = 0;
	int nSent = 0;
	nLen = strText.GetLength();
	std::string ss = mci::unicode_2_ansi(strText);
	const char* snd = ss.c_str();
	if(m_pSocket)nSent=m_pSocket->Send(snd,strlen(snd));
	if(nSent!=SOCKET_ERROR)
	{
		
	}
	else
	{
//		sLogError(_T("发送消息给服务器失败..."));
		int iErrCode = WSAGetLastError();
		if (WSAECONNABORTED == iErrCode)
		{
// 			ConnectToSrv();
// 			SendMsg(strText);
			MessageBox(NULL,_T("与服务器连接断开!"),_T("Error"),MB_ICONERROR|MB_OK);
		}
//		MessageBox(NULL,_T("消息发送错误!"),_T("Error"),MB_ICONERROR|MB_OK);
	}
//	sLog(_T("发送消息给服务器..."));
}
#define MAX_RCV_BUFLEN 1024*2
#define MAX_SND_BUFLEN 1024
void CMainWindow::OnSendMsg(TNotifyUI& msg)
{
	if( m_pInput == NULL ) return;
	m_pInput->SetFocus();
	CString sText = m_pInput->GetTextRange(0, m_pInput->GetTextLength());
	if( sText.IsEmpty() ) return;
	m_pInput->SetText(_T(""));

	if( m_pMsgView == NULL ) return;

 	CString sName = theUser.m_loadName;
 	CString sTime = mci::format_currenttime();

 
 	FormatMsgToEditView(sName,sTime,sText);
	//新修改，2013-6-24 9:45
	std::string ss = mci::unicode_2_ansi(sText);
	const char* snd = ss.c_str();
	int iTotal = strlen(snd);
	int iPart = 0;
	while (iTotal>MAX_SND_BUFLEN)
	{
		char buf[MAX_SND_BUFLEN]={0};
		memcpy(buf,snd+iPart,MAX_SND_BUFLEN);
		iPart += MAX_SND_BUFLEN;
		iTotal -= iPart;

		CString sToWho = _T("");
		CString sPart = mci::ansi_2_CString(buf);
		CString sMsg = BuildNomalMsg(sPart,sToWho);
		DUI__Trace(sPart);
		SendMsg(sMsg);
		Sleep(10);
	}
	if(iTotal>0)
	{
		char buf[MAX_SND_BUFLEN]={0};
		memcpy(buf,snd+iPart,iTotal);
		CString sToWho = _T("");
		CString sPart = mci::ansi_2_CString(buf);
		CString sMsg = BuildNomalMsg(sPart,sToWho);
		SendMsg(sMsg);
	}
// 	CString sToWho = _T("");
// 	CString sMsg = BuildNomalMsg(sText,sToWho);
// 	SendMsg(sMsg);
}
void CMainWindow::FormatMsgToEditView(CString sName,CString sTime,CString sMsg)
{
	long lSelBegin = 0, lSelEnd = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR  | CFM_UNDERLINE | CFM_UNDERLINETYPE | CFM_BOLD;
	cf.dwEffects = CFE_LINK ;
	cf.bUnderlineType = CFU_UNDERLINE;
	cf.crTextColor = RGB(0, 0, 255);
	if(sName==theUser.m_loadName)
		cf.crTextColor = RGB(255, 0, 0);
	m_lockEditView.Lock();

	lSelEnd = lSelBegin = m_pMsgView->GetTextLength();
	m_pMsgView->SetSel(lSelEnd, lSelEnd);//移到末尾
	sName = _T("[ ")+sName;
	sName += _T(" ");
	sName += sTime;
	sName += _T(" ]\n");
	m_pMsgView->ReplaceSel(sName, false);

	lSelEnd = m_pMsgView->GetTextLength();
	m_pMsgView->SetSel(lSelBegin, lSelEnd);
	m_pMsgView->SetSelectionCharFormat(cf);

	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0, 0, 0);
	cf.dwEffects = 0;

	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	m_pMsgView->SetParaFormat(pf);

	lSelEnd = lSelBegin = m_pMsgView->GetTextLength();

	m_pMsgView->SetSel(-1, -1);
	m_pMsgView->ReplaceSel(sMsg, false);

	m_pMsgView->SetSel(-1, -1);
	m_pMsgView->ReplaceSel(_T("\n"), false);

	cf.crTextColor = RGB(0, 0, 0);
	lSelEnd = m_pMsgView->GetTextLength();
	m_pMsgView->SetSel(lSelBegin, lSelEnd);
	m_pMsgView->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 220;
	m_pMsgView->SetParaFormat(pf);

	m_pMsgView->NeedUpdate();
	m_pMsgView->EndDown();
	

	m_lockEditView.UnLock();
}

void CMainWindow::OnReceive(CChatSocket* pSocket)
{
	char *pBuf = new char[MAX_RCV_BUFLEN+1];
	int nBufSize = MAX_RCV_BUFLEN;
	int nReceived;
	CString name;
	CString strReceived;
	
	nReceived=pSocket->Receive(pBuf,nBufSize);

	if(nReceived!=SOCKET_ERROR)
	{
		pBuf[nReceived] = 0;
		strReceived = (mci::ansi_2_CString(pBuf));
		m_pMsgRcv->m_strText = strReceived;
		DUI__Trace(strReceived);

		CString preType = m_pMsgRcv->WhatType();
		
		if(preType==MSGTYPE::SERVERDOWN)
		{
			theConning = false;
//			sLog(_T("与服务器断开连接..."));
			DisAllConnect();			
			MessageBox(m_hWnd,_T("与服务器断开连接"),_T("提醒"),MB_OK|MB_ICONWARNING);
		}
		else if(preType==_T("FORCEOFFLINE"))
		{
			theConning = false;
//			sLog(_T("您被强制下线了..."));
			DisAllConnect();			
			MessageBox(m_hWnd,_T("很抱歉,您被强制下线了"),_T("提醒"),MB_OK|MB_ICONWARNING);
		}
		else if(preType==_T("FORCEDELETE"))
		{
			theConning = false;
//			sLog(_T("您被强制从数据库中删除..."));
			EmptyUserList();
			UpdateUserInfo();
			MessageBox(m_hWnd,_T("很抱歉,您被强制从数据库中删除"),_T("提醒"),MB_OK|MB_ICONWARNING);
		}
		else if (preType==_T("SHAKE"))
		{
//			sLog(_T("收到收到抖动消息..."));
			m_pMsgRcv->BuildShake();
			ShakeWindow(m_hWnd);
			if(m_pMsgRcv->m_nomalMsg.who.IsEmpty())
			{
				CString sText = _T("{p}{c #0000FF}{b}{u}");
				sText += theUser.m_loadName;
				sText +=_T("{/u}{/b}{/c}您好,{c #FF0000}{b}{u}");
				sText += _T("虾聊工作人员") ;
				sText += _T("{/u}{/b}{/c}给您发了一个震动消息{/p}");

				CNotice* pNotice = new CNotice(*theApp,_T("msg.xml"),sText,EMT_SYS_MSG);
				pNotice->m_sWho = m_pMsgRcv->m_nomalMsg.who;
				FormatMsgToEditView(_T("(系统消息)"),m_pMsgRcv->m_nomalMsg.sndTime,_T("虾听工作人员给您发了抖动消息"));
			}
			else if(m_pMsgRcv->m_nomalMsg.who!=theUser.m_loadName)
			{
				if(bFriednShakeTip)
				{
					CString sText = _T("{p}{c #0000FF}{b}{u}");
					sText += theUser.m_loadName;
					sText +=_T("{/u}{/b}{/c}您好,用户{c #00FF00}{b}{u}");
					sText += m_pMsgRcv->m_nomalMsg.who ;
					sText += _T("{/u}{/b}{/c}给您发了一个震动消息{/p}");

					CNotice* pNotice = new CNotice(*theApp,_T("msg.xml"),sText,EMT_FRIEND_SHAKE);
					pNotice->m_sWho = m_pMsgRcv->m_nomalMsg.who;
					FormatMsgToEditView(_T("(系统消息)"),m_pMsgRcv->m_nomalMsg.sndTime,m_pMsgRcv->m_nomalMsg.who+_T(" 给您发了抖动消息"));
				}
			}
			
		}
		else if (preType==MSGTYPE::USERLIST)
		{
//			sLog(_T("更新用户列表..."));
			m_pMsgRcv->BuildUserList();
			this->UpdateUserInfo();
		}
		else if (preType==_T("SYSMSG"))
		{
//			sLog(_T("收到系统消息..."));
			bool bResult = IsSystemMsg(strReceived);
		}
		else if (preType==_T("CHATMSG"))
		{
//			sLog(_T("收到群聊天消息..."));
			m_pMsgRcv->BuildChatMsg();
			//聊天信息
			bool bResult = IsMeSnd(strReceived);
			if (bResult)
			{//我发送出去的就不用显示
			}
			else if (IsToMe(strReceived))
			{//发送给我的
				MsgComming(m_pMsgRcv);
			}
			else 
			{
				MsgComming(m_pMsgRcv);
				ShowMsg(m_pMsgRcv);
			}
			
		}
		else
		{
			m_pMsgRcv->BuildNomal();
			if (preType==MSGTYPE::LOAD)
			{//服务端返回登录结果
//				sLog(_T("收到登录结果..."));
				bool bResult = IsUserLoad(strReceived);
			}
			else if(preType==MSGTYPE::REGI)
			{//服务端返回注册结果
//				sLog(_T("收到注册结果..."));
				bool bResult = IsUserRegit(strReceived);
			}			
			else
			{

			}
		}
		
	}
	else
	{
//		sLogError(_T("信息接收错误..."));
		MessageBox(NULL,_T("信息接收错误!"),_T("Error"),MB_OK|MB_ICONERROR);
	}
	delete pBuf;
}
/*
<MSGTYPE TYPE="NOMALMSG">
	<WHO NAME="ME" />
	<TOWHO NAME="YOU" />
	<MSG CONTEXT="hello,this is a test" />
	<TTIME TIME="2012-06-21 13:26:30" />
</MSGTYPE>
*/
CString CMainWindow::BuildNomalMsg(CString& preString,CString& toWho)
{
	CString sf = _T("<MSGTYPE TYPE=\"NOMALMSG\">");
	CString a = _T("<WHO  NAME=\"");
	a += theUser.m_loadName + _T("\" />");
	sf += a;
	a = _T("<TOWHO  NAME=\"");
	a += toWho + _T("\" />");
	sf += a;
	a = _T("<MSG  CONTEXT=\"");
	a += preString + _T("\" />");
	sf += a;
	a = _T("<TTIME  TIME=\"");
	a += mci::format_currenttime() + _T("\" />");
	sf += a;
	sf += _T("</MSGTYPE>");
	return sf;
}
/*
<MSGTYPE TYPE="SYSTEMMSG">
	<WHAT TYPE="SHAKE" />
	<WHO NAME="YOU" />
	<MSG CONTEXT="password" />
	<TTIME TIME="2012-06-21 13:26:30" />
</MSGTYPE>
*/
CString CMainWindow::BuildShakeMsg()
{
	CString sf = _T("<MSGTYPE TYPE=\"SYSTEMMSG\">");
	CString a = _T("<WHAT  TYPE=\"");
	a += _T("SHAKE");
	a += _T("\" />");
	sf += a;
	a = _T("<WHO  NAME=\"");
	a += theUser.m_loadName + _T("\" />");
	sf += a;
	a = _T("<MSG  CONTEXT=\"");
	a += theUser.m_password + _T("\" />");
	sf += a;
	a = _T("<TTIME  TIME=\"");
	a += mci::format_currenttime() + _T("\" />");
	sf += a;
	sf += _T("</MSGTYPE>");
	return sf;
}
/*
<MSGTYPE TYPE="SYSTEMMSG">
	<WHAT TYPE="REGI" />
	<WHO NAME="YOU" />
	<MSG CONTEXT="password" />
	<TTIME TIME="2012-06-21 13:26:30" />
</MSGTYPE>
*/
CString CMainWindow::BuildRegMsg()
{
	CString sf = _T("<MSGTYPE TYPE=\"SYSTEMMSG\">");
	CString a = _T("<WHAT  TYPE=\"");
	a += _T("REGI");
	a += _T("\" />");
	sf += a;
	a = _T("<WHO  NAME=\"");
	a += theUser.m_loadName + _T("\" />");
	sf += a;
	a = _T("<MSG  CONTEXT=\"");
	a += theUser.m_password + _T("\" />");
	sf += a;
	a = _T("<TTIME  TIME=\"");
	a += mci::format_currenttime() + _T("\" />");
	sf += a;
	sf += _T("</MSGTYPE>");
	return sf;
}
/*
<MSGTYPE TYPE="SYSTEMMSG">
	<WHAT TYPE="LOAD" />
	<WHO NAME="YOU" />
	<MSG CONTEXT="password" />
	<TTIME TIME="2012-06-21 13:26:30" />
</MSGTYPE>
*/
CString CMainWindow::BulidLoadMsg()
{
	CString sf = _T("<MSGTYPE TYPE=\"SYSTEMMSG\">");
	CString a = _T("<WHAT  TYPE=\"");
	a += _T("LOAD");
	a += _T("\" />");
	sf += a;
	a = _T("<WHO  NAME=\"");
	a += theUser.m_loadName + _T("\" />");
	sf += a;
	a = _T("<MSG  CONTEXT=\"");
	a += theUser.m_password + _T("\" />");
	sf += a;
	a = _T("<TTIME  TIME=\"");
	a += mci::format_currenttime() + _T("\" />");
	sf += a;
	sf += _T("</MSGTYPE>");
	return sf;
}
/*
<MSGTYPE TYPE="SYSTEMMSG">
	<WHAT TYPE="CHANGE" />
	<WHO NAME="YOU" />
	<MSG CONTEXT="password" />
	<TTIME TIME="2012-06-21 13:26:30" />
</MSGTYPE>
*/
CString CMainWindow::BulidChangeMsg()
{
	CString sf = _T("<MSGTYPE TYPE=\"SYSTEMMSG\">");
	CString a = _T("<WHAT  TYPE=\"");
	a += _T("CHANGE");
	a += _T("\" />");
	sf += a;
	a = _T("<WHO  NAME=\"");
	a += m_nameCopy + _T("\" />");
	sf += a;
	a = _T("<MSG  CONTEXT=\"");
	a += theUser.m_loadName + _T("\" />");
	sf += a;
	a = _T("<TTIME  TIME=\"");
	a += mci::format_currenttime() + _T("\" />");
	sf += a;
	sf += _T("</MSGTYPE>");
	return sf;
}
/*
<MSGTYPE TYPE="SYSTEMMSG">
	<WHAT TYPE="CLOSE" />
	<WHO NAME="YOU" />
	<MSG CONTEXT="password" />
	<TTIME TIME="2012-06-21 13:26:30" />
</MSGTYPE>
*/
CString CMainWindow::BulidCancelMsg()
{
	CString sf = _T("<MSGTYPE TYPE=\"SYSTEMMSG\">");
	CString a = _T("<WHAT  TYPE=\"");
	a += _T("CLOSE");
	a += _T("\" />");
	sf += a;
	a = _T("<WHO  NAME=\"");
	a += theUser.m_loadName + _T("\" />");
	sf += a;
	a = _T("<MSG  CONTEXT=\"");
	a += _T(" ");
	a += _T("\" />");
	sf += a;
	a = _T("<TTIME  TIME=\"");
	a += mci::format_currenttime() + _T("\" />");
	sf += a;
	sf += _T("</MSGTYPE>");
	return sf;
}
void CMainWindow::LogOut()
{
//	sLog(_T("取消登录..."));
	CString  a= BulidCancelMsg();

	SendMsg(a);
}

//解析服务端发来数据
//<MSGTYPE TYPE="LOAD">
//  <MSG who="name" result="success" time="" />
//</MSGTYPE>
bool CMainWindow::IsUserLoad(CString& textStr)
{//用户是否登陆
	CMarkup m_xml(textStr);
	CMarkupNode root = m_xml.GetRoot();
	if(!root.IsValid()) return false;
	CMarkupNode node = root.GetChild();
	LPCTSTR lpClass = node.GetName();

	LPCTSTR lpName = node.GetAttributeName(1);
	LPCTSTR lpResult = node.GetAttributeValue(1);
	if (_tcscmp(lpName,_T("result"))==0)
	{
		if(_tcscmp(lpResult,_T("loged"))==0)
		{
//			sLogWarn(_T("您重登录..."));

			if(m_pLogin) m_pLogin->SetLoginTip(_T("您已经登录了,无需再次登录"));		
			return false;
		}
		else if(_tcscmp(lpResult,_T("success"))==0)
		{
//			sLog(_T("登录成功..."));

			if(m_pLogin) m_pLogin->Close(IDOK);//关闭登录窗口
			theApp->ShowWindow(true);//显示主窗口
			SetTitle(theUser.m_loadName);
			m_tray.ShowToolTip(_T(" 虾聊--尽情释放想要说的话!!!"),_T("欢迎使用"));
			TCHAR buf[MAX_PATH] = {0};
			_stprintf_s( buf, MAX_PATH,_T("%s"),theUser.m_loadName.GetString());
			m_tray.SetTip(buf);
			SetWindowText(buf);

			SOCKADDR_IN   sa; 
			int  len  = sizeof(sa);
			m_pSocket->GetSockName((SOCKADDR*)&sa,&len);
			CString sIP;
			UINT uPort;
			sIP.Format(_T("%d.%d.%d.%d"),
				sa.sin_addr.S_un.S_un_b.s_b1,
				sa.sin_addr.S_un.S_un_b.s_b2,
				sa.sin_addr.S_un.S_un_b.s_b3,
				sa.sin_addr.S_un.S_un_b.s_b4);
			uPort = sa.sin_port;
			if(m_pChatSocket)
			{
				delete m_pChatSocket;
				m_pChatSocket = NULL;
			}
			m_pChatSocket = new CListenSocket(this);
			ASSERT(m_pChatSocket);
			int iErrCode = 0;
			//创建套接字并绑定到主机
//			if(m_pChatSocket->Create(uPort,SOCK_DGRAM,sIP)==FALSE)
			//新修改 2013-6-25 8:13
			if(m_pChatSocket->Create(uPort,SOCK_DGRAM)==FALSE)
			{
//				sLogError(_T("创建UDP套接字失败..."));
				iErrCode = WSAGetLastError();
				return true;
			}
//			sLog(_T("创建UDP套接字成功..."));
			return true;
		}
		else if (_tcscmp(lpResult,_T("fail")) ==0)
		{
//			sLogWarn(_T("登录失败..."));
			if(m_pLogin) m_pLogin->SetLoginTip(_T("该账号可能没有注册,或者您输入的账号或密码有误\n请重新输入"));
			return false;
		}
	}

	return false;
}
//<MSGTYPE TYPE="CHATMSG">
//  <MSG who="name" towho="name" text="" time="" />
//</MSGTYPE>
bool CMainWindow::IsToMe(CString& text)
{//发给我的
	CMarkup m_xml(text);
	CMarkupNode root = m_xml.GetRoot();
	if(!root.IsValid()) return false;
	CMarkupNode node = root.GetChild();
	LPCTSTR lpName = node.GetAttributeName(1);
	LPCTSTR lpResult = node.GetAttributeValue(1);
	if (_tcscmp(lpName,_T("towho"))==0)
	{
		if(_tcscmp(lpResult,theUser.m_loadName)==0)
		{
			return true;
		}
	}
	return false;
}
//<MSGTYPE TYPE="CHATMSG">
//  <MSG who="name" towho="name" text="" time="" />
//</MSGTYPE>
bool CMainWindow::IsMeSnd(CString& text)
{//是我发送的
	CMarkup m_xml(text);
	CMarkupNode root = m_xml.GetRoot();
	if(!root.IsValid()) return false;
	CMarkupNode node = root.GetChild();
	LPCTSTR lpName = node.GetAttributeName(0);
	LPCTSTR lpResult = node.GetAttributeValue(0);
	if (_tcscmp(lpName,_T("who"))==0)
	{
		if(_tcscmp(lpResult,theUser.m_loadName)==0)
		{
			return true;
		}
	}
	return false;
}
//<MSGTYPE TYPE="REGI">
//  <MSG who="name" result="success" time="" />
//</MSGTYPE>
bool CMainWindow::IsUserRegit(CString& textStr)
{
	CMarkup m_xml(textStr);
	CMarkupNode root = m_xml.GetRoot();
	if(!root.IsValid()) return false;
	CMarkupNode node = root.GetChild();
	LPCTSTR lpClass = node.GetName();
	LPCTSTR lpName = node.GetAttributeValue(0);
	LPCTSTR lpWhat = node.GetAttributeName(1);
	LPCTSTR lpResult = node.GetAttributeValue(1);
	if (_tcscmp(lpWhat,_T("result"))==0)
	{
		if(_tcscmp(lpResult,_T("fail"))==0)
		{
//			sLogWarn(_T("注册失败..."));
			if(m_pLogin) m_pLogin->SetRegTip(_T("注册失败!!!"));
			return false;
		}
		else if(_tcscmp(lpResult,_T("success"))==0)
		{
//			sLog(_T("注册成功"));
			if(m_pLogin) m_pLogin->SetRegTip(_T("恭喜你,注册成功!!!"));
			if(m_pLogin) m_pLogin->SetPage(2);
			if (_tcscmp(lpName,theUser.m_loadName)==0 )
			{
				CString sText = _T("{p}用户{c #0000FF}{b}{u}");
				sText += lpName ;
				sText += _T("{/u}{/b}{/c}您好,欢迎注册成为{b}{c #00FF00}虾聊2013{/c}{/b}用户{/p}");
				CNotice* pNotice = new CNotice(*theApp,_T("user_regiester.xml"),sText,EMT_NOTICE_TYPE::EMT_SYS_MSG);		
			}
			//新修改 2013-6-24 15:54
			//取消注册成功立即登录功能
//			CString sLoad = BulidLoadMsg();
//			SendMsg(sLoad);
			
			return true;
		}
	}

	return false;
}
//<MSGTYPE TYPE="SYSMSG">
//  <MSG who="name" what="load" msg="上线了" time="" />
//</MSGTYPE>
bool CMainWindow::IsSystemMsg(CString& textStr)
{
	CMarkup m_xml(textStr);
	CMarkupNode root = m_xml.GetRoot();
	if(!root.IsValid()) return false;
	CMarkupNode node = root.GetChild();
	LPCTSTR lpClass = node.GetName();
	CString sMsg = _T("(系统消息S) ");
	LPCTSTR lpName = NULL;	
	LPCTSTR lpResult = NULL;	
	LPCTSTR lpWhat = NULL;	
	LPCTSTR lpMsg = NULL;
	lpName = node.GetAttributeValue(0);
	lpWhat = node.GetAttributeName(1);
	lpResult = node.GetAttributeValue(1);
	lpMsg = node.GetAttributeValue(2);
	if (_tcscmp(lpName,theUser.m_loadName)!=0 && _tcscmp(lpWhat,_T("what"))==0 )
	{
		if(_tcscmp(lpResult,_T("LOAD"))==0 )
		{
			if(bFriednLoadTip)
			{
				CString sText = _T("{p}用户{c #0000FF}{b}{u}");
				sText += lpName ;
				sText += _T("{/u}{/b}{/c}上线了{/p}");
				CNotice* pNotice = new  CNotice(*theApp,_T("friend_login.xml"),sText,EMT_NOTICE_TYPE::EMT_FRIEND_LOAD);
				//点击可以打开私聊窗口
				pNotice->m_sWho = lpName;
			}
			//播放上线提示音
			CDuiString sSoundPath = CPaintManagerUI::GetInstancePath();
			if(sSoundPath.Right(1)!=_T("\\"))
				sSoundPath += _T("\\");
			sSoundPath += _T("sound\\Login.wav");
			::PlaySound(sSoundPath.GetData(),GetModuleHandle(NULL),SND_NODEFAULT | SND_NOWAIT|SND_FILENAME|SND_SYNC);
		}
	}
	//显示系统消息

	return true;
}
CString CMainWindow::AddTimeMsg(CString& text)
{
	CTime a=CTime::GetCurrentTime();
	CString b=a.Format(_T("(%H:%M:%S)"));
	b=b+text;
	return b;
}
void CMainWindow::AddRootToFriendList()
{
// 	_ui._pFriendTree->RemoveNode(_parent_node_online_friends);
// 	_ui._pFriendTree->RemoveNode(_parent_node_offline_friends);
// 	_ui._pFriendTree->RemoveNode(_parent_node_all_firends);
	_ui._pFriendTree->RemoveAll();

	FriendListItemInfo item;

	item.id = _T("0");
	item.folder = true;
	item.empty = false;
	item.nick_name = _T("在线好友");

	_parent_node_online_friends = _ui._pFriendTree->AddNode(item, NULL);

	item.id = _T("1");
	item.folder = true;
	item.empty = false;
	item.nick_name = _T("离线好友");
	_parent_node_offline_friends =  _ui._pFriendTree->AddNode(item, NULL);

	item.id = _T("2");
	item.folder = true;
	item.empty = false;
	item.nick_name = _T("所有好友");
	_parent_node_all_firends  = _ui._pFriendTree->AddNode(item, NULL);
}
void CMainWindow::UpdateUserInfo()
{
	_ui._pFriendTree->SetEnabled(false);
	_ui._pFriendTree->SetMouseChildEnabled(false);
	POSITION pos;
	m_LockuserList.Lock();
	AddRootToFriendList();
	int iAllCount = 0,iOnLine = 0;
	for (pos = m_userList.GetHeadPosition();pos != NULL;)
	{
		User* pUser = (User*)(m_userList.GetNext(pos));
		if(pUser)
		{
			_ui._pFriendTree->AddNode(*pUser,_parent_node_all_firends);
			iAllCount ++;
			if(pUser->m_userState==ONLINE)
			{	_ui._pFriendTree->AddNode(*pUser,_parent_node_online_friends); iOnLine ++;}
			else 
				_ui._pFriendTree->AddNode(*pUser,_parent_node_offline_friends);
		}
	}
	_ui._pFriendTree->SetUserCount(_parent_node_all_firends,iAllCount);
	_ui._pFriendTree->SetUserCount(_parent_node_online_friends,iOnLine);
	_ui._pFriendTree->SetUserCount(_parent_node_offline_friends,iAllCount-iOnLine);

	m_LockuserList.UnLock();
	_ui._pFriendTree->SetEnabled(true);
	_ui._pFriendTree->SetMouseChildEnabled(true);
}
void CMainWindow::DisAllConnect()
{
	POSITION pos;
	m_LockuserList.Lock();
	int iAllCount = 0,iOnLine = 0;
	for (pos = m_userList.GetHeadPosition();pos != NULL;)
	{
		User* pUser = (User*)(m_userList.GetNext(pos));
		iAllCount ++;
		if(pUser->m_userState==ONLINE)
		{ pUser->m_userState = OFFLINE;}
	}
	m_LockuserList.UnLock();

	UpdateUserInfo();
}
void CMainWindow::EmptyUserList()
{
	m_LockuserList.Lock();
	POSITION pos,temp;
	for (pos=m_userList.GetHeadPosition();NULL!=pos;)
	{
		temp = pos;
		
		User* user = (User*)m_userList.GetNext(pos);
		m_userList.RemoveAt(temp);
		delete user;
		user = NULL;
	}
	m_LockuserList.UnLock();	
}
bool CMainWindow::CheckId(CString& preIp)
{
	int begin=0;
	int end=0;
	CString seg1;
	CString seg2;
	CString seg3;
	CString seg4;

	end=preIp.Find(_T("."),begin);
	seg1=preIp.Mid(begin,end-begin);
	begin=end+1;

	end=preIp.Find(_T("."),begin);
	seg2=preIp.Mid(begin,end-begin);
	begin=end+1;

	end=preIp.Find(_T("."),begin);
	seg3=preIp.Mid(begin,end-begin);
	begin=end+1;

	seg4=preIp.Mid(begin);


	if((!seg1.IsEmpty())&&
		(!seg2.IsEmpty())&&
		(!seg3.IsEmpty())&&
		(!seg4.IsEmpty()))
	{

		if(IsNumberOfIp(seg1)&&
			IsNumberOfIp(seg2)&&
			IsNumberOfIp(seg3)&&
			IsNumberOfIp(seg4)
			)
		{
			return true;
		}


	}
	return false;
}
bool CMainWindow::IsNumberOfIp(CString ipSeg)
{
	int size = ipSeg.GetLength();
	char *segChar = NULL;
	if(size>0&&size<=3)
	{
		segChar = new char[size];
		for(int i=0;i<size;i++)
		{
			segChar[i] = ipSeg[i];
		}
		int number = atoi(segChar);
		if(!(number>=0&&number<=255))
		{
			return false;
		}
		delete segChar;
	}
	
	return true;
}
void CMainWindow::SetTitle(CString sName)
{
	CLabelUI* pTitle = NULL;
	FindControl(pTitle,_T("title"));
	if(pTitle) pTitle->SetText(sName);
}
void CMainWindow::MsgComming(CMsg* pMsg)
{
	if(bGroupMsgTip)
	{
		CString sText = _T("{p}{c #0000FF}{b}{u}");
		sText += theUser.m_loadName;
		sText +=_T("{/u}{/b}{/c}您好,用户{c #00FF00}{b}{u}");
		sText += pMsg->m_nomalMsg.who.IsEmpty() ? _T("虾聊工作人员") : pMsg->m_nomalMsg.who;
		sText += _T("{/u}{/b}{/c}发来消息: ");
		sText += pMsg->m_nomalMsg.msgItem;
		sText += _T("{/p}");
		CNotice* pNotice = new CNotice(*theApp,_T("msg.xml"),sText,EMT_GROUP_MSG);
//		pNotice->m_sWho = pMsg->m_nomalMsg.who;
	}
	//播放上线提示音
	CDuiString sSoundPath = CPaintManagerUI::GetInstancePath();
	if(sSoundPath.Right(1)!=_T("\\"))
		sSoundPath += _T("\\");
	sSoundPath += _T("sound\\msg.wav");
	::PlaySound(sSoundPath.GetData(),GetModuleHandle(NULL),SND_NODEFAULT | SND_NOWAIT|SND_FILENAME|SND_SYNC);
}
bool CMainWindow::CreateUDPSocket(CString sIP,CString sPort)
{
// 	SOCKADDR_IN   sa; 
// 	int  len  = sizeof(sa);
// 	m_pSocket->GetSockName((SOCKADDR*)&sa,&len);
// 	CString sIP;
// 	UINT uPort;
// 	sIP.Format(_T("%d.%d.%d.%d"),
// 		sa.sin_addr.S_un.S_un_b.s_b1,
// 		sa.sin_addr.S_un.S_un_b.s_b2,
// 		sa.sin_addr.S_un.S_un_b.s_b3,
// 		sa.sin_addr.S_un.S_un_b.s_b4);
// 	uPort = sa.sin_port;
	if(m_pChatSocket)
	{
		delete m_pChatSocket;
		m_pChatSocket = NULL;
	}
	m_pChatSocket = new CListenSocket(this);
	ASSERT(m_pChatSocket);
	int iErrCode = 0;
	//创建套接字并绑定到主机
	if(m_pChatSocket->Create(mci::ToUInt(sPort),SOCK_DGRAM,sIP)==FALSE)
	{
		//sLogError(_T("创建UDP套接字失败..."));
		iErrCode = WSAGetLastError();
		return true;
	}
	//sLog(_T("创建UDP套接字成功..."));
	return false;
}
//参数说明
//peerIP : 远程主机IP
//peerPort ：远程主机端口
//msg : 将要发送到远程主机的消息
void CMainWindow::SendTo(CString peerIP,CString peerPort,CString msg)
{
	//创建单聊套接字
// 	if(m_pChatSocket)
// 	{
// 		delete m_pChatSocket;
// 		m_pChatSocket = NULL;
// 	}
// 	m_pChatSocket = new CListenSocket(this);
// 	ASSERT(m_pChatSocket);
// 	int iErrCode = 0;
// 	//创建套接字并绑定到主机
// 	if(m_pChatSocket->Create(mci::ToUInt(theUser.m_userPort),SOCK_DGRAM,theUser.m_userIp)==FALSE)
// 	{
// 		iErrCode = WSAGetLastError();
// 		return ;
// 	}
	if(m_pChatSocket==NULL)
		return ;
	std::string ss = mci::unicode_2_ansi(msg);
	const char* snd = ss.c_str();
	int nSend = 0;
	nSend = m_pChatSocket->SendTo(snd,strlen(snd),mci::ToUInt(peerPort),peerIP);
}
//发送给某个用户的信息
void CMainWindow::SendTo(User user,CString msg)
{

	if (FindUserFromList(user.m_loadName,user))
	{
		//新修改，2013-6-24 9:45
		std::string ss = mci::unicode_2_ansi(msg);
		const char* snd = ss.c_str();
		int iTotal = strlen(snd);
		int iPart = 0;
		while (iTotal>MAX_SND_BUFLEN)
		{
			char buf[MAX_SND_BUFLEN]={0};
			memcpy(buf,snd+iPart,MAX_SND_BUFLEN);
			iPart += MAX_SND_BUFLEN;
			iTotal -= iPart;

			CString sPart = mci::ansi_2_CString(buf);
			DUI__Trace(sPart);
			CString sMsg = BuildChatMsg(sPart,user.m_loadName);
			DUI__Trace(sMsg);
			SendTo(user.m_userIp,user.m_userPort,sMsg);
			Sleep(50);
		}
		if(iTotal>0)
		{
			char buf[MAX_SND_BUFLEN]={0};
			memcpy(buf,snd+iPart,iTotal);
			CString sPart = mci::ansi_2_CString(buf);
			DUI__Trace(sPart);
			CString sMsg = BuildChatMsg(sPart,user.m_loadName);
			DUI__Trace(sMsg);
			SendTo(user.m_userIp,user.m_userPort,sMsg);
		}
// 		CString sMsg = BuildChatMsg(msg,user.m_loadName);
// 		SendTo(user.m_userIp,user.m_userPort,sMsg);
	}
}
//私聊时,首先判断用户是否在线
//如果不在线,就发离线消息到服务器
//等接收方下次登录时,服务器转发此消息
bool CMainWindow::bIsUserOnLine(CString name)
{
	POSITION pos;
	bool bIsOnline = false;
	m_LockuserList.Lock();
	for (pos=m_userList.GetHeadPosition();pos!=NULL;)
	{
		User* pUser = (User*)m_userList.GetNext(pos);
		if (pUser)
		{
			if (pUser->m_loadName==name&&pUser->m_userState==ONLINE)
			{
				bIsOnline = true;
				break;
			}
			else if (pUser->m_loadName==name&&pUser->m_userState==OFFLINE)
			{
				bIsOnline = false;
				break;
			}
		}
	}
	m_LockuserList.UnLock();
	return bIsOnline;
}
//在用户列表中查找指定名称的用户是否存在
bool CMainWindow::FindUserFromList(CString name,User& user)
{
	POSITION pos;
	bool bFind = false;
	m_LockuserList.Lock();
	for (pos=m_userList.GetHeadPosition();pos!=NULL;)
	{
		User* pUser = (User*)m_userList.GetNext(pos);
		if (pUser)
		{
			if (pUser->m_loadName==name)
			{
				user.m_loadName = pUser->m_loadName;
				user.m_callName = pUser->m_callName;
				user.m_password = pUser->m_password;
				user.m_userIp = pUser->m_userIp;
				user.m_userPort = pUser->m_userPort;
				user.m_userState = pUser->m_userState;
				bFind = true;
				break;
			}
		}
	}
	m_LockuserList.UnLock();
	return bFind;
}
void CMainWindow::ShakeToWho(User user)
{
	if (FindUserFromList(user.m_loadName,user))
	{
		CString sMsg = BuildShakeMsg(user.m_loadName);
		SendTo(user.m_userIp,user.m_userPort,sMsg);
	}
}
/*
<MSGTYPE TYPE="NORMALMSG">
	<MSG who="name" towho="name" text="" time="" />
</MSGTYPE>
*/
CString CMainWindow::BuildChatMsg(CString sMsg,CString sToWho)
{
	CString sf = _T("<MSGTYPE TYPE=\"NORMALMSG\">");
	CString a = _T("<MSG ");
	a += _T("who=\"");
	a += theUser.m_loadName + _T("\" ");

	a += _T("towho=\"");
	a += sToWho + _T("\" ");
	a += _T("text=\"");
	a += sMsg + _T("\" ");

	a += _T("time=\"");
	a += mci::format_currenttime() + _T("\" ");
	a += _T(" />");
	sf += a;
	sf += _T("</MSGTYPE>");
	return sf;
}
/*
<MSGTYPE TYPE="SHAKE">
	<MSG who="name" towho="name" text="" time="" />
</MSGTYPE>
*/
CString CMainWindow::BuildShakeMsg(CString sToWho)
{
	CString sf = _T("<MSGTYPE TYPE=\"SHAKE\">");
	CString a = _T("<MSG ");
	a += _T("who=\"");
	a += theUser.m_loadName + _T("\" ");

	a += _T("towho=\"");
	a += sToWho + _T("\" ");
	a += _T("text=\"");
	a += _T("\" ");

	a += _T("time=\"");
	a += mci::format_currenttime() + _T("\" ");
	a += _T(" />");
	sf += a;
	sf += _T("</MSGTYPE>");
	return sf;
}
/*
 *与其他用户点对点UDP通信
 */
void CMainWindow::RecvFrom(CListenSocket* pSocket)
{
	char *pBuf = new char[MAX_RCV_BUFLEN+1];
	int nBufSize = MAX_RCV_BUFLEN;
	int nReceived;
	CString name;
	CString strReceived;
	UINT nPort = mci::ToUInt(pSocket->m_strPeerPort);
	nReceived = pSocket->ReceiveFrom(pBuf,nBufSize,pSocket->m_strPeerIP,nPort);
	if (nReceived!=SOCKET_ERROR)
	{
		pBuf[nReceived] = 0;
		strReceived = mci::ansi_2_CString(pBuf);
		m_pChatMsg->m_strText = strReceived;
		CString preType = m_pChatMsg->WhatType();
		if(preType==_T("NORMALMSG"))
		{
			m_pChatMsg->BuildChatMsg();
			if(bFriednMsgTip)
			{
				CString sText = _T("{p}{c #0000FF}{b}{u}");
				sText += theUser.m_loadName;
				sText +=_T("{/u}{/b}{/c}您好,用户{c #00FF00}{b}{u}");
				sText += m_pChatMsg->m_nomalMsg.who ;
				sText += _T("{/u}{/b}{/c}发来消息: ");
				sText += m_pChatMsg->m_nomalMsg.msgItem;
				sText += _T("{/p}");
				CNotice* pNotice = new CNotice(*theApp,_T("msg.xml"),sText,EMT_FRIEND_MSG);
				pNotice->m_sWho = m_pChatMsg->m_nomalMsg.who;
			}
			
			//播放提示音
			//Sleep(1000);
			CDuiString sSoundPath = CPaintManagerUI::GetInstancePath();
			if(sSoundPath.Right(1)!=_T("\\"))
				sSoundPath += _T("\\");
			sSoundPath += _T("sound\\msg.wav");
			::PlaySound(sSoundPath.GetData(),GetModuleHandle(NULL),SND_NODEFAULT | SND_NOWAIT|SND_FILENAME|SND_SYNC);
			ShowChatBox(m_pChatMsg->m_nomalMsg.who,m_pChatMsg);
			theLockMsgQue.Lock();
			if(theMsgQueue.size()>0)
				m_tray.StartFlash(m_hWnd,1234);
			else 
				m_tray.StopFlash(m_hWnd,1234);
			theLockMsgQue.UnLock();

			if (IsChatBoxOpen(m_pChatMsg->m_nomalMsg.who))
			{
				ChatDialog* pDlg = NULL;
				if ((pDlg=FindChatBox(m_pChatMsg->m_nomalMsg.who))!=NULL)
				{
					if(pDlg->IsIconic())
						pDlg->FlashWindow(TRUE);
				}
			}
		}
		else if(preType==_T("SHAKE"))
		{
			m_pChatMsg->BuildChatMsg();
			if(bFriednShakeTip)
			{
				CString sText = _T("{p}{c #0000FF}{b}{u}");
				sText += theUser.m_loadName;
				sText +=_T("{/u}{/b}{/c}您好,用户{c #00FF00}{b}{u}");
				sText += m_pChatMsg->m_nomalMsg.who ;
				sText += _T("{/u}{/b}{/c}给您发了一个震动消息{/p}");

				CNotice* pNotice = new CNotice(*theApp,_T("msg.xml"),sText,EMT_FRIEND_SHAKE);
				pNotice->m_sWho = m_pChatMsg->m_nomalMsg.who;
			}
			if (IsChatBoxOpen(m_pChatMsg->m_nomalMsg.who))
			{
				ChatDialog* pDlg = NULL;
				if ((pDlg=FindChatBox(m_pChatMsg->m_nomalMsg.who))!=NULL)
				{
					pDlg->ShowWindow(true);
					pDlg->BringWindowToTop();
					ShakeWindow(*pDlg);
				}
			}
			else
			{
				User friend_;
				FindUserFromList(m_pChatMsg->m_nomalMsg.who,friend_);
				ChatDialog* pChatDialog = new ChatDialog(_T(""), 0, theUser, friend_);
				if( pChatDialog == NULL )
					return;
#if defined(WIN32) && !defined(UNDER_CE)
				pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP,  NULL, 0, 0, 0, 0);
#else
				pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
#endif
				pChatDialog->SetWindowText(friend_.m_loadName);
				//skin_changed_observer_.AddReceiver(pChatDialog);

				pChatDialog->CenterWindow();
				::ShowWindow(*pChatDialog, SW_SHOW);
				pChatDialog->BringWindowToTop();
				Sleep(100);
				ShakeWindow(*pChatDialog);
				//装入链表
				thelockChatDlgList.Lock();
				theChatDialogList.AddTail(pChatDialog);
				thelockChatDlgList.UnLock();
			}
		}
	}
	delete []pBuf;
}
void CMainWindow::ShowChatBox(CString sWho,CMsg* pMsg)
{
	User friend_;
	if (FindUserFromList(sWho,friend_))
	{
		ChatDialog* pDlg = NULL;
		if ((pDlg=FindChatBox(sWho))!=NULL)
		{
			if(pDlg->IsIconic())
			{
				pDlg->FlashWindow(TRUE);
			}
			pDlg->ShowMsg(pMsg);
			return ;
		}
		theLockMsgQue.Lock();
		typedef std::map<CString,std::vector<CMsg*> >::iterator iter;
		iter it;
		if ((it=theMsgQueue.find(sWho))!=theMsgQueue.end())
		{
			it->second.push_back(pMsg);
		}
		else
		{
			std::vector<CMsg*> msg;
			msg.push_back(pMsg);
			theMsgQueue[sWho] = msg;
//			theMsgQueue.insert(make_pair(sWho,msg));
		}
		theLockMsgQue.UnLock();
	}
}
bool CMainWindow::IsChatBoxOpen(CString sName)
{
	thelockChatDlgList.Lock();
	POSITION pos;
	for (pos=theChatDialogList.GetHeadPosition();pos!=NULL;)
	{
		ChatDialog* pDlg = (ChatDialog*)theChatDialogList.GetNext(pos);
		if (pDlg)
		{	
			TCHAR strName[MAX_PATH] = {0};
			pDlg->GetWindowText(strName,MAX_PATH);
			CString Name = strName;
			if(Name==sName)
			{
				thelockChatDlgList.UnLock();
				return true;
			}
		}
	}
	thelockChatDlgList.UnLock();
	return false;
}
ChatDialog* CMainWindow::FindChatBox(CString sName)
{
	thelockChatDlgList.Lock();
	POSITION pos;
	for (pos=theChatDialogList.GetHeadPosition();pos!=NULL;)
	{
		ChatDialog* pDlg = (ChatDialog*)theChatDialogList.GetNext(pos);
		if (pDlg)
		{	
			TCHAR strName[MAX_PATH] = {0};
			pDlg->GetWindowText(strName,MAX_PATH);
			CString Name = strName;
			if(Name==sName)
			{
				thelockChatDlgList.UnLock();
				return pDlg;
			}
		}
	}
	thelockChatDlgList.UnLock();
	return NULL;
}
bool CMainWindow::HasMsgForUserInQueue(CString sName)
{
	theLockMsgQue.Lock();
	typedef std::map<CString,std::vector<CMsg*> >::iterator iter;
	iter it;
	for (it=theMsgQueue.begin();it!=theMsgQueue.end();++it)
	{
		if(it->first==sName)
		{
			theLockMsgQue.UnLock();
			return true;
		}
	}
	theLockMsgQue.UnLock();
	return false;
}
void CMainWindow::ClearAllVar()
{
	if(m_pSocket)
	{
		delete m_pSocket;
		m_pSocket = NULL;
	}
	if(m_pChatSocket)
	{
		delete m_pChatSocket;
		m_pChatSocket = NULL;
	}
	if(m_pChatMsg)
	{
		delete m_pChatMsg;
		m_pChatMsg = NULL;
	}
//..................
}
void CMainWindow::ShakeWindow(HWND hWnd)
{
	int ty = 3;
	RECT   m_rect; 
	::ShowWindow(hWnd,SW_SHOW);

	::SetForegroundWindow(hWnd );
	::GetWindowRect(hWnd,&m_rect);  
	int recordy = m_rect.left;
	int recordx = m_rect.top;
	CString sPath = CPaintManagerUI::GetInstancePath();
	if(sPath.Right(1)!=_T("\\")) sPath += _T("\\");
	sPath += _T("sound\\shake.wav");
	PlaySound(sPath,GetModuleHandle(NULL),SND_NODEFAULT | SND_NOWAIT | SND_FILENAME | SND_SYNC); 
	for(int i=0;i<5;i++)
	{
		m_rect.left = recordy;
		m_rect.top = recordx;
		m_rect.top  = m_rect.top + ty;  
		m_rect.left = m_rect.left - ty;
		::SetWindowPos(hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
		Sleep(20);
		m_rect.top = m_rect.top -ty;
		::SetWindowPos( hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
		Sleep(20);
		m_rect.top = m_rect.top -ty;
		::SetWindowPos( hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
		Sleep(20);
		m_rect.left=m_rect.left+ty;
		::SetWindowPos( hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
		Sleep(20);
		m_rect.left=m_rect.left+ty;
		::SetWindowPos( hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
		Sleep(20);
		m_rect.top = m_rect.top + ty;  
		::SetWindowPos( hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
		Sleep(20);
		m_rect.top=m_rect.top+ty;
		::SetWindowPos( hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
		Sleep(20);
		m_rect.top=m_rect.top+ty;
		::SetWindowPos( hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE);
		Sleep(20);
		::SetWindowPos( hWnd,NULL,recordy,recordx,0,0,SWP_NOSIZE);
		Sleep(3);
	}
}