#ifndef COLORPICKER_HPP
#define COLORPICKER_HPP


class ChatDialog;

class CColorPicker 	
	: public CWin
	, public INotifyUI
	, public IDialogBuilderCallback
	, public IMessageFilterUI
{
public:
	CColorPicker(ChatDialog* chat_dialog, POINT ptMouse);

	LPCTSTR GetWindowClassName() const;

	CControlUI* CreateControl(LPCTSTR pstrClass);

	void Notify(TNotifyUI& msg);

	void InitWindow();

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled);

	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM , bool& bHandled);
private:
	POINT based_point_;
	ChatDialog* chat_dialog_;
};

#endif // COLORPICKER_HPP