#include "StdAfx.h"
#include "HorizontalLayoutExUI.h"
#include <windowsx.h>

CHorizontalLayoutExUI::CHorizontalLayoutExUI()
: _tool(NULL)
, _hand_cursor(false)
, _need_msg(false)
{

}

CHorizontalLayoutExUI::~CHorizontalLayoutExUI()
{

}

UINT CHorizontalLayoutExUI::GetControlFlags() const
{
	return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
}

void CHorizontalLayoutExUI::AddPaintTool(CPaintTool *tool)
{
	_tool = tool;
}

void CHorizontalLayoutExUI::RemovePaintTool(CPaintTool *tool)
{
	_tool = NULL;
}

void CHorizontalLayoutExUI::SetHandCursor(bool hand_cursor)
{
	_hand_cursor = hand_cursor;
}

void CHorizontalLayoutExUI::SetNeedMsg(bool need)
{
	_need_msg = need;
}

void CHorizontalLayoutExUI::SetBkImage2(LPCTSTR pBKImage)
{
	if (_bk2_image == pBKImage)
		return;

	_bk2_image = pBKImage;
	Invalidate();
}

void CHorizontalLayoutExUI::PaintBkImage(HDC hDC)
{
	__super::PaintBkImage(hDC);

	if (_bk2_image.IsEmpty()) return;
	if (!DrawImage(hDC, (LPCTSTR)_bk2_image)) _bk2_image.Empty();
}

void CHorizontalLayoutExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("needmsg")) == 0) SetNeedMsg(_tcscmp(pstrValue, _T("true")) == 0);
	else if (_tcscmp(pstrName, _T("bkimage2")) == 0) SetBkImage2(pstrValue);
	else CHorizontalLayoutUI::SetAttribute(pstrName, pstrValue);
}

void CHorizontalLayoutExUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	__super::DoPaint(hDC, rcPaint);

	if (_tool)
		_tool->DoAfterPaintByTool(hDC, rcPaint, this);
}

void CHorizontalLayoutExUI::DoEvent(TEventUI& event)
{
	if (_need_msg)
	{
		if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
		{
			if (::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled()) {

				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				Invalidate();
			}
			return;
		}
		else if (event.Type == UIEVENT_BUTTONUP)
		{
			if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
				if (::PtInRect(&m_rcItem, event.ptMouse)){
					if (Activate() && m_pManager != NULL) m_pManager->SendNotify(this, _T("click"));
				}
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
				Invalidate();
			}
			return;
		}
		else if (event.Type == UIEVENT_MOUSEENTER)
		{
			if (IsEnabled()) {
				if (Activate() && m_pManager != NULL) m_pManager->SendNotify(this, _T("hover"));

				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if (event.Type == UIEVENT_MOUSELEAVE)
		{
			if (IsEnabled()) {
				if (Activate() && m_pManager != NULL) m_pManager->SendNotify(this, _T("leave"));

				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if (event.Type == UIEVENT_SETCURSOR) {
			if (_hand_cursor)
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
	}
	
	CHorizontalLayoutUI::DoEvent(event);
}

CVerticalLayoutExUI::CVerticalLayoutExUI()
: _tool(NULL)
, _need_msg(false)
, _caption(false)
, _hand_cursor(false)
{

}

CVerticalLayoutExUI::~CVerticalLayoutExUI()
{

}

LPCTSTR CVerticalLayoutExUI::GetClass() const
{
	if (_caption && _need_msg)
		return _T("VerticalLayoutExUI");
	else
		return _T("VerticalLayoutExMsgUI");

}

UINT CVerticalLayoutExUI::GetControlFlags() const
{
	return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
}


void CVerticalLayoutExUI::AddPaintTool(CPaintTool *tool)
{
	_tool = tool;
}

void CVerticalLayoutExUI::RemovePaintTool(CPaintTool *tool)
{
	_tool = NULL;
}

void CVerticalLayoutExUI::SetForeImage(LPCTSTR pForeImage)
{
	if (_tcscmp(_fore_image, pForeImage) == 0)
		return;

	_fore_image = pForeImage;
	Invalidate();
}

void CVerticalLayoutExUI::SetCoverImage(LPCTSTR pCoverImage)
{
	_cover_image = pCoverImage;
}

void CVerticalLayoutExUI::SetNeedMsg(bool need)
{
	_need_msg = need;
}

void CVerticalLayoutExUI::SetCaption(bool caption)
{
	_caption = caption;
}

void CVerticalLayoutExUI::SetHandCursor(bool hand_cursor)
{
	_hand_cursor = hand_cursor;
}

void CVerticalLayoutExUI::SetBkImage2(LPCTSTR pBKImage)
{
	if (_bk2_image == pBKImage)
		return;

	_bk2_image = pBKImage;
	Invalidate();
}

void CVerticalLayoutExUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	__super::DoPaint(hDC, rcPaint);
	DrawForeImage(hDC);

	if (_tool)
		_tool->DoAfterPaintByTool(hDC, rcPaint, this);

	DrawCoverImage(hDC);
}

void CVerticalLayoutExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("coverimage")) == 0) SetCoverImage(pstrValue);
	else if (_tcscmp(pstrName, _T("bkimage2")) == 0) SetBkImage2(pstrValue);
	else if (_tcscmp(pstrName, _T("needmsg")) == 0) SetNeedMsg(_tcscmp(pstrValue, _T("true")) == 0);
	else if (_tcscmp(pstrName, _T("caption")) == 0) SetCaption(_tcscmp(pstrValue, _T("true")) == 0);
	else if (_tcscmp(pstrName, _T("handcursor")) == 0) SetHandCursor(_tcscmp(pstrValue, _T("true")) == 0);
	else CVerticalLayoutUI::SetAttribute(pstrName, pstrValue);
}

void CVerticalLayoutExUI::DoEvent(TEventUI& event)
{
	if (_need_msg)
	{
		if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
		{
			if (::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled()) {

				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				Invalidate();
			}
			return;
		}
		else if (event.Type == UIEVENT_BUTTONUP)
		{
			if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
				if (::PtInRect(&m_rcItem, event.ptMouse)){
					if (Activate() && m_pManager != NULL) m_pManager->SendNotify(this, _T("click"));
				}
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
				Invalidate();
			}
			return;
		}
		else if (event.Type == UIEVENT_MOUSEENTER)
		{
			if (IsEnabled()) {
				if (Activate() && m_pManager != NULL) m_pManager->SendNotify(this, _T("hover"));

				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if (event.Type == UIEVENT_MOUSELEAVE)
		{
			if (IsEnabled()) {
				if (Activate() && m_pManager != NULL) m_pManager->SendNotify(this, _T("leave"));

				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if (event.Type == UIEVENT_SETCURSOR) {
			if (_hand_cursor)
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
	}

	CVerticalLayoutUI::DoEvent(event);
}

void CVerticalLayoutExUI::DrawForeImage(HDC hDC)
{
	if (_fore_image.IsEmpty())return;
	if (!DrawImage(hDC, (LPCTSTR)_fore_image)) _fore_image.Empty();
}

void CVerticalLayoutExUI::DrawCoverImage(HDC hDC)
{
	if (!IsEnabled())
	{
		if (_cover_image.IsEmpty()) return;
		if (!DrawImage(hDC, (LPCTSTR)_cover_image)) _cover_image.Empty();
	}
}

void CVerticalLayoutExUI::PaintStatusImage(HDC hDC)
{
	if (_bk2_image.IsEmpty()) return;
	if (!DrawImage(hDC, (LPCTSTR)_bk2_image)) _bk2_image.Empty();

	__super::PaintStatusImage(hDC);
}