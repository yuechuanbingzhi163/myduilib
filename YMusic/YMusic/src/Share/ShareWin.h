#ifndef __ShareWin_h
#define __ShareWin_h

#include "../../YMusic.h"

class CShareWin :
	public YWindow,
	public INotifyUI,
	public SkinChangedReceiver,
	public Singleton<CShareWin>
{
public:

	CShareWin();
	~CShareWin();
	static LPCTSTR GetWindowClass();
	LPCTSTR GetWindowClassName() const;
	void Share(spSongInfoT song);
	void Share(url::emShareObjT type,std::size_t id,LPCTSTR title,LPCTSTR content);
protected:
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void Notify(TNotifyUI& msg); 

	void OnClick(const TNotifyUI& msg);

protected:
	virtual BOOL Receive(SkinChangedParam param);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnWebService(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void    OnWebService(void *pRequest ,void *pProto);
	virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
 //   url::emShareObjT    _type;
	std::size_t  _id;
	UINT_PTR     _nTimerId;

	struct {
		CLabelUI*        _title;
		CLabelUI*        _textlen;
		CFadeButtonUI*   _sharebtn;
		CRichEditUI*     _edit;
	}ui;
private:
	DECLARE_SINGLETON_CLASS(CShareWin);
};


#endif//__ShareWin_h