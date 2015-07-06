#ifndef COLORPICKER_HPP
#define COLORPICKER_HPP


class CMainWindow;

class CColorPicker 	
	: public CWin
	, public INotifyUI
	, public IDialogBuilderCallback
	, public IMessageFilterUI
{
public:
	CColorPicker(CMainWindow* chat_dialog, POINT ptMouse);
	virtual LPCTSTR GetClass() const
	{
		return _T("CColorPicker");
	}
	LPCTSTR GetWindowClassName() const;

	CControlUI* CreateControl(LPCTSTR pstrClass);

	void Notify(TNotifyUI& msg);

	void InitWindow();

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled);

	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM , bool& bHandled);
private:
	POINT based_point_;
	CMainWindow* chat_dialog_;
};

#endif // COLORPICKER_HPP