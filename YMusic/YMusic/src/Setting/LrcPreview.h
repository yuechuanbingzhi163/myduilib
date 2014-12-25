/***********************************************************
 *	FileName£ºLrcPreview.h
 *  Author  £ºDFn Li
 *  Timer   : 2013/3/20 18:10
 *  E-Mail  : libyyu@qq.com
 *  Comment : show lrc to program panel
 *  Version : 1.0.0.1
 ***********************************************************/
#ifndef LRCPREVIEW_H
#define LRCPREVIEW_H




class CLrcPreViewUI : public CControlUI
{

public:

public:
	CLrcPreViewUI();
	virtual ~CLrcPreViewUI();

	static LPCTSTR GetClassName();
	LPCTSTR GetClass() const;
protected:
	void DoInit();
	void DoEvent(TEventUI& event);
	void PaintText(HDC hDC);

public:
	DWORD m_dwFontColor;
	CString m_sFontName;
	int    m_iFontSize;
	int    m_iFontStyle;
private:

	CString        m_sLrcPreView;
	
};


#endif//LRCPREVIEW_H