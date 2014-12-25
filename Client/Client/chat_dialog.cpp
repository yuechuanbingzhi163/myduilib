#include "stdafx.h"
#include <windows.h>
#include <shellapi.h>

#include "chat_dialog.h"
#include "ColorPicker.h"
#include "mainwindow.h"

const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");

const TCHAR* const kBackgroundControlName = _T("bg");

const TCHAR* const kLogoButtonControlName = _T("logo");
const TCHAR* const kNickNameControlName = _T("nickname");
const TCHAR* const kDescriptionControlName = _T("description");

const TCHAR* const kFontButtonControlName = _T("fontbtn");
const TCHAR* const kFontbarControlName = _T("fontbar");
const TCHAR* const kFontTypeControlName = _T("font_type");
const TCHAR* const kFontSizeControlName = _T("font_size");
const TCHAR* const kBoldButtonControlName = _T("boldbtn");
const TCHAR* const kItalicButtonControlName = _T("italicbtn");
const TCHAR* const KUnderlineButtonControlName = _T("underlinebtn");
const TCHAR* const kColorButtonControlName = _T("colorbtn");

const TCHAR* const kInputRichEditControlName = _T("input_richedit");
const TCHAR* const kViewRichEditControlName = _T("view_richedit");

const TCHAR* const kEmotionButtonControlName = _T("emotionbtn");

const TCHAR* const kSendButtonControlName = _T("sendbtn");

const int kEmotionRefreshTimerId = 1001;
const int kEmotionRefreshInterval = 150;

CPtrList theChatDialogList;
CLock    thelockChatDlgList;

std::map<CString,std::vector<CMsg*> > theMsgQueue;
CLock theLockMsgQue;

ChatDialog::ChatDialog(const CDuiString& bgimage, DWORD bkcolor, const User& myselft_info, const User& friend_info)
: bgimage_(bgimage)
, bkcolor_(bkcolor)
, myselft_(myselft_info)
, friend_(friend_info)
, emotion_timer_start_(false)
, text_color_(0xFF000000)
, bold_(false)
, italic_(false)
, underline_(false)
, font_size_(12)
, font_face_name_(_T("微软雅黑"))
{}

ChatDialog::~ChatDialog()
{
	RemoveObserver();
	return;
}

LPCTSTR ChatDialog::GetWindowClassName() const
{
	return _T("ChatDialog");
}

CControlUI* ChatDialog::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}


BOOL ChatDialog::Receive(SkinChangedParam param)
{
	bgimage_ = param.bgimage;
	bkcolor_ = param.bkcolor;
	CControlUI* background = GetPaintMgr()->FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		if (!param.bgimage.IsEmpty())
		{
			TCHAR szBuf[MAX_PATH] = {0};
#if defined(UNDER_WINCE)
			_stprintf(szBuf, _T("file='%s' corner='600,200,1,1'"), param.bgimage.c_str());
#else
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), param.bgimage);
#endif
			background->SetBkImage(szBuf);
		}
		else
			background->SetBkImage(_T(""));

		background->SetBkColor(param.bkcolor);
	}

	return TRUE;
}


LRESULT ChatDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT ChatDialog::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT ChatDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
		}
	}
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

LRESULT ChatDialog::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	try
	{
		ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		if(!RegisterSkin(_T("chatwindow.xml"),NULL,this))
			return 0;

		GetPaintMgr()->AddNotifier(this);
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
			rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);
		GetPaintMgr()->SetNextTabControl(true);
	}
	catch (...)
	{
		throw _T("ChatDialog::OnCreate");
	}

	return 0;
}

LRESULT ChatDialog::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void ChatDialog::OnTimer(TNotifyUI& msg)
{}

void ChatDialog::OnExit(TNotifyUI& msg)
{
	thelockChatDlgList.Lock();
	POSITION pos,temp;
	for (pos=theChatDialogList.GetHeadPosition();pos!=NULL;)
	{
		temp = pos;
		ChatDialog* pDlg = (ChatDialog*)theChatDialogList.GetNext(pos);
		if (pDlg&&pDlg==this)
		{
			theChatDialogList.RemoveAt(temp);
		}
	}
	thelockChatDlgList.UnLock();
	Close();
}

void ChatDialog::InitWindow()
{
	 m_pInput = static_cast<CRichEditUI*>(GetPaintMgr()->FindControl(kInputRichEditControlName));
	 m_pMsgView = static_cast<CRichEditUI*>(GetPaintMgr()->FindControl(kViewRichEditControlName));

	 if (theApp->HasMsgForUserInQueue(friend_.m_loadName))
	 {
		 theLockMsgQue.Lock();
		 typedef std::map<CString,std::vector<CMsg*> >::iterator iter;
		 iter it;
		 for (it=theMsgQueue.begin();it!=theMsgQueue.end();++it)
		 {
			 if(it->first==friend_.m_loadName)
			 {
				 std::vector<CMsg*> msg = it->second;
				 for (int i=0;i<msg.size();++i)
				 {
					 ShowMsg(msg[i]);
				 }
				theMsgQueue.erase(it);
				 break;
			 }
		 }
		 theLockMsgQue.UnLock();
	 }
}

void ChatDialog::OnPrepare(TNotifyUI& msg)
{
	TCHAR szBuf[MAX_PATH] = {0};

	CControlUI* background = GetPaintMgr()->FindControl(kBackgroundControlName);
	if (background != NULL)
	{
#if defined(UNDER_WINCE)
		_stprintf(szBuf, _T("file='%s' corner='600,200,1,1'"), bgimage_.c_str());
#else
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), bgimage_.GetData());
#endif

		//background->SetBkImage(szBuf);
		//background->SetBkColor(bkcolor_);
	}

	CButtonUI* log_button = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(kLogoButtonControlName));
// 	if (log_button != NULL)
// 	{
// #if defined(UNDER_WINCE)
// 		_stprintf(szBuf, _T("%s"), friend_.logo.c_str());
// #else
// 		_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), friend_.m_callName);
// #endif
// 		log_button->SetNormalImage(szBuf);
// 	}

	CControlUI* nick_name = GetPaintMgr()->FindControl(kNickNameControlName);
	if (nick_name != NULL)
		nick_name->SetText(friend_.m_loadName);

	CControlUI* desciption = GetPaintMgr()->FindControl(kDescriptionControlName);
	if (desciption != NULL)
		desciption->SetText(friend_.m_callName);

// 	CContainerUI* pFontbar = static_cast<CContainerUI*>(GetPaintMgr()->FindControl(kFontbarControlName));
// 	if (pFontbar != NULL)
// 		pFontbar->SetVisible(!pFontbar->IsVisible());
}

CDuiString GetCurrentTimeString()
{
	SYSTEMTIME time = {0};
	TCHAR szTime[MAX_PATH] = {0};
	::GetLocalTime( &time );
#if defined(UNDER_CE)
	_stprintf( szTime, _T("%04d-%02d-%02d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
#else
	_stprintf_s( szTime, MAX_PATH, _T("%04d-%02d-%02d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
#endif

	return szTime;
}

void ChatDialog::SendMsg()
{
 
    if( m_pInput == NULL ) return;
    m_pInput->SetFocus();
    CString sText = m_pInput->GetTextRange(0, m_pInput->GetTextLength());
    if( sText.IsEmpty() ) return;
    m_pInput->SetText(_T(""));

    if( m_pMsgView == NULL ) return;

	FormatMsgToEditView(myselft_.m_loadName,mci::format_currenttime(),sText);

    CString sMsg = sText;

//	theApp->SendMsg(sMsg);
	User user;
	if (theApp->FindUserFromList(friend_.m_loadName,user))
	{
		theApp->SendTo(user,sMsg);
	}
	
}

void ChatDialog::ShowMsg(CMsg* pMsg)
{
	if(pMsg==NULL) return ;
	CString sWho = pMsg->m_nomalMsg.who;
	CString sToWho = pMsg->m_nomalMsg.toWho;
	CString sMsg = pMsg->m_nomalMsg.msgItem;
	CString sTime = pMsg->m_nomalMsg.sndTime;

	FormatMsgToEditView(sWho,sTime,sMsg);
}

void ChatDialog::FormatMsgToEditView(CString sName,CString sTime,CString sMsg)
{
	long lSelBegin = 0, lSelEnd = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR  | CFM_UNDERLINE | CFM_UNDERLINETYPE | CFM_BOLD;
	cf.dwEffects = CFE_LINK;
	cf.bUnderlineType = CFU_UNDERLINE;
	cf.crTextColor = RGB(0, 0, 255);
	if(sName==myselft_.m_loadName)
		cf.crTextColor = RGB(255, 0, 0);
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

	m_pMsgView->EndDown();
}

void ChatDialog::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
		InitWindow();
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), _T("shockbtn"))==0)
		{
			theApp->ShakeToWho(friend_);
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("trans_file"))==0)
		{
			OnTransFile(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("empty_richedit")) == 0)
		{
			int lSelBegin,lSelEnd;
			lSelEnd = lSelBegin = m_pMsgView->GetTextLength();
			m_pMsgView->SetSel(0, lSelEnd);//选择全部	
			m_pMsgView->ReplaceSel(_T(""), false);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
		{
			OnExit(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMinButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMaxButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MAXIMIZE);
			CControlUI* pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
#else
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kRestoreButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_RESTORE);
			CControlUI* pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(GetPaintMgr()->FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
#else
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kFontButtonControlName) == 0)
		{
			CContainerUI* pFontbar = static_cast<CContainerUI*>(GetPaintMgr()->FindControl(kFontbarControlName));
			if (pFontbar != NULL)
				pFontbar->SetVisible(!pFontbar->IsVisible());
		}
		else if (_tcsicmp(msg.pSender->GetName(), kEmotionButtonControlName) == 0)
		{
			POINT pt = {0};
			CDuiRect rcEmotionBtn = msg.pSender->GetPos();
			RECT rcWindow;
			GetWindowRect( &rcWindow );

			pt.y = rcWindow.top + rcEmotionBtn.top;
			pt.x = rcWindow.left + rcEmotionBtn.left;
			//emotion_list_window_.SelectEmotion(pt);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSendButtonControlName) == 0)
        {
            SendMsg();
		}
	}
    else if( _tcsicmp(msg.sType, _T("return")) == 0 ) 
    {
        if (_tcsicmp(msg.pSender->GetName(), kInputRichEditControlName) == 0)
        {
            SendMsg();
        }
    }
	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		return OnTimer(msg);
	}
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kColorButtonControlName) == 0)
		{
// 			CContainerUI* pFontbar = static_cast<CContainerUI*>(GetPaintMgr()->FindControl(kFontbarControlName));
// 			if (pFontbar != NULL)
// 			{
// 				POINT pt = {0};
// 				CDuiRect rcFontbar = pFontbar->GetPos();
// 				CDuiRect rcColorBtn = msg.pSender->GetPos();
// 				RECT rcWindow;
// 				GetWindowRect( &rcWindow);
// 
// 				pt.y = rcWindow.top + rcFontbar.top;
// 				pt.x = rcWindow.left + rcColorBtn.left + static_cast<LONG>(rcColorBtn.right - rcColorBtn.left / 2);
// 				new CColorPicker(this, pt);
// 			}
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
		else if (_tcsicmp(msg.pSender->GetName(), kBoldButtonControlName) == 0)
		{
			COptionUI* bold_button = static_cast<COptionUI*>(msg.pSender);
			if (bold_button != NULL)
			{
				bold_ = bold_button->IsSelected();
				FontStyleChanged();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kItalicButtonControlName) == 0)
		{
			COptionUI* italic_button = static_cast<COptionUI*>(msg.pSender);
			if (italic_button != NULL)
			{
				italic_ = italic_button->IsSelected();
				FontStyleChanged();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), KUnderlineButtonControlName) == 0)
		{
			COptionUI* underline_button = static_cast<COptionUI*>(msg.pSender);
			if (underline_button != NULL)
			{
				underline_ = underline_button->IsSelected();
				FontStyleChanged();
			}
		}
	}
	else if (_tcsicmp(msg.sType, _T("itemselect")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kFontTypeControlName) == 0)
		{
			CComboUI* font_type = static_cast<CComboUI*>(msg.pSender);
			if (font_type != NULL)
			{
				font_face_name_ = font_type->GetText();
				FontStyleChanged();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kFontSizeControlName) == 0)
		{
			CComboUI* font_size = static_cast<CComboUI*>(msg.pSender);
			if (font_size != NULL)
			{
				font_size_ = _ttoi(font_size->GetText());
				FontStyleChanged();
			}
		}
	}
}

void ChatDialog::SetTextColor(DWORD dwColor)
{
	COptionUI* color_button = static_cast<COptionUI*>(GetPaintMgr()->FindControl(kColorButtonControlName));
	if (color_button != NULL)
	{
		color_button->Selected(false);
		if (dwColor != 0)
		{
			text_color_ = dwColor;
			FontStyleChanged();
		}
	}
}

LRESULT ChatDialog::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM , bool& bHandled)
{
	if (uMsg == WM_KEYDOWN)
	{
		return ResponseDefaultKeyEvent(wParam);
	}
	return FALSE;
	
}

void ChatDialog::FontStyleChanged()
{}

void ChatDialog::OnTransFile(TNotifyUI& msg)
{
	OPENFILENAME openfilename;
	ZeroMemory(&openfilename, sizeof(OPENFILENAME)); 
	TCHAR szfiletitle[MAX_PATH] = _T("\0");
	CDuiString s_title=_T("请选择要传输的文件");
	LPTSTR szFile = new TCHAR[MAX_PATH * 101 + 1]; //最多可以选中100项
	szFile[0] = 0x0;

	static TCHAR lpstr[] = _T("文件(*.*;)\0*.*\0\0");
	openfilename.lStructSize = sizeof(OPENFILENAME) ;
	openfilename.hwndOwner = this->m_hWnd ;
	openfilename.hInstance = NULL ;
	openfilename.lpstrFilter = lpstr;//lpstr;
	openfilename.lpstrCustomFilter = (LPTSTR)NULL ;
	openfilename.nMaxCustFilter = 0L ;
	openfilename.nFilterIndex = 1L ;
	openfilename.lpstrFile = szFile ;
	openfilename.nMaxFile = MAX_PATH * 101 + 1 ;
	openfilename.lpstrFileTitle = szfiletitle ;
	openfilename.nMaxFileTitle = sizeof(szfiletitle) ;
	openfilename.lpstrInitialDir = NULL ;
	openfilename.lpstrTitle = s_title;
	openfilename.nFileOffset = 0;
	openfilename.nFileExtension = 0;
	openfilename.lpstrDefExt = NULL ;
	openfilename.lCustData = 0 ;

	openfilename.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_READONLY | OFN_ALLOWMULTISELECT | OFN_EXPLORER /*|OFN_LONGNAMES*/;

//	m_FilePath.erase(m_FilePath.begin(),m_FilePath.end());
	if (GetOpenFileName(&openfilename))
	{
		LPTSTR p = szFile;
		while(*p++);
		if (*p == NULL) // 只选了一个文件
		{
			CString s_file = szFile;

//			m_FilePath.push_back(s_file);
		}
		else    ///////////////////////////////////////////////////////// 多选，循环取出文件
		{
			CString folder = szFile;
			while(*p)
			{
				CString fileName = p;
				CString file = folder + _T("\\") + fileName;
//				m_FilePath.push_back(file);

				while(*p++);
			}
		}
	}
	delete[] szFile;
}

// LRESULT ChatDialog::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
// {
//	return 0;
//}