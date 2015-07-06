
/*---------------------------------------------------------------------
//    file_name   :    HORIZONTALLAYOUTEXUI.H
//    author      :    Chao.Zhang@mojichina.com
//    create_time :    2014/9/17 13:41
//    details     :    
//-------------------------------------------------------------------*/
#ifndef __HORIZONTAL_LAYOUT_EX_UI_H__
#define __HORIZONTAL_LAYOUT_EX_UI_H__

#include "UIlib.h"
class CPaintTool
{
public:
	virtual void DoBeginPaintByTool(HDC hDC, const RECT& rc, CControlUI* control){}
	virtual void DoAfterPaintByTool(HDC hDC, const RECT& rcPaint, CControlUI* control){}
};

class CHorizontalLayoutExUI : public DuiLib::CHorizontalLayoutUI
{
public:
	CHorizontalLayoutExUI();
	virtual ~CHorizontalLayoutExUI();

	LPCTSTR GetClass() const{ return _T("HorizontalLayoutExUI"); }
	virtual UINT GetControlFlags() const override;

	void AddPaintTool(CPaintTool *tool);
	void RemovePaintTool(CPaintTool *tool);
	void SetHandCursor(bool hand_cursor);
	void SetNeedMsg(bool need);

	void SetBkImage2(LPCTSTR pBKImage);
	CDuiString GetBKImage2() const{ return _bk2_image; }

	virtual void PaintBkImage(HDC hDC) override;

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	virtual void DoPaint(HDC hDC, const RECT& rcPaint);
	virtual void DoEvent(TEventUI& event);

private:
	bool _need_msg;
	bool _hand_cursor;
	CDuiString _bk2_image;
	CPaintTool *_tool;
};

class CVerticalLayoutExUI : public DuiLib::CVerticalLayoutUI
{
public:
	CVerticalLayoutExUI();
	virtual ~CVerticalLayoutExUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;

	void AddPaintTool(CPaintTool *tool);
	void RemovePaintTool(CPaintTool *tool);

	void SetForeImage(LPCTSTR pForeImage);
	CDuiString GetForeImage() const{ return _fore_image; }

	void SetCoverImage(LPCTSTR pCoverImage);
	CDuiString GetCoverImage() const{ return _cover_image; }
	void SetNeedMsg(bool need);
	bool IsMsgNeeded() const { return _need_msg; }

	void SetCaption(bool caption);
	bool IsCaption() const { return _caption; }

	void SetHandCursor(bool hand_cursor);
	bool IsHandCursor() const { return _hand_cursor; }

	void SetBkImage2(LPCTSTR pBKImage);
	CDuiString GetBKImage2() const{ return _bk2_image; }

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	virtual void DoPaint(HDC hDC, const RECT& rcPaint);
	virtual void DoEvent(TEventUI& event);
	virtual void DrawForeImage(HDC hDC);
	virtual void DrawCoverImage(HDC hDC);
	virtual void PaintStatusImage(HDC hDC);

protected:
	bool _need_msg;
	bool _caption;
	bool _hand_cursor;

	CDuiString _bk2_image;
	CDuiString _cover_image;
	CDuiString _fore_image;
	CPaintTool *_tool;
};

#endif /* __HORIZONTAL_LAYOUT_EX_UI_H__ */