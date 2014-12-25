#ifndef CHATDIALOG_HPP
#define CHATDIALOG_HPP

#include "DuiLib/skin_change_event.hpp"
#include "Msg.h"
#include "lock.h"

class ChatDialog 
	: public CWin
	, public INotifyUI
	, public IDialogBuilderCallback
	, public IMessageFilterUI
	, public SkinChangedReceiver
{
public:

	ChatDialog(const CDuiString& bgimage, DWORD bkcolor, const User& myselft_info, const User& friend_info);
	~ChatDialog();

public:

	LPCTSTR GetWindowClassName() const;	

	virtual void InitWindow();

	void OnPrepare(TNotifyUI& msg);
	
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM , bool& bHandled);

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled);

//	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual BOOL Receive(SkinChangedParam param);

	void SetTextColor(DWORD dwColor);

    void SendMsg();

	void ShowMsg(CMsg* pMsg);

	void FormatMsgToEditView(CString sName,CString sTime,CString sMsg);
protected:	

	void Notify(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
	void OnTransFile(TNotifyUI& msg);
private:
	void FontStyleChanged();

private:

	CRichEditUI* m_pMsgView;
	CRichEditUI* m_pInput;

	bool emotion_timer_start_;

	bool bold_;
	bool italic_;
	bool underline_;
	DWORD text_color_;
	DWORD font_size_;
	CDuiString font_face_name_;

	CDuiString bgimage_;
	DWORD bkcolor_;
	User myselft_;
	User friend_;
};

extern CPtrList theChatDialogList;
extern CLock    thelockChatDlgList;

//extern std::map<CString sWhoSnd,std::vector<CMsg*> > theMsgQueue;
extern std::map<CString,std::vector<CMsg*> > theMsgQueue;
extern CLock theLockMsgQue;
#endif // CHARTDIALOG_HPP