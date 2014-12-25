#include "stdafx.h"

#include "UIMedia.h"
#include "../YApp.h"

namespace DuiLib
{
	CMediaUI::CMediaUI(): m_hWnd(NULL){}

	LPCTSTR CMediaUI::GetClassName()
	{
		return _T("MediaUI");
	}

	LPCTSTR CMediaUI::GetClass() const
	{
		return CMediaUI::GetClassName();
	}

    void CMediaUI::SetVisible(bool bVisible)
    {
        __super::SetVisible(bVisible);
        ::ShowWindow(m_hWnd, bVisible);
    }

    void CMediaUI::SetInternVisible(bool bVisible)
    {
        __super::SetInternVisible(bVisible);
        ::ShowWindow(m_hWnd, bVisible);
    }

    void CMediaUI::SetPos(RECT rc)
    {
        __super::SetPos(rc);
        ::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
    }

    BOOL CMediaUI::Attach(HWND hWndNew)
    {
        if (! ::IsWindow(hWndNew))
        {
            return FALSE;
        }

        m_hWnd = hWndNew;
        return TRUE;
    }

    HWND CMediaUI::Detach()
    {
        HWND hWnd = m_hWnd;
        m_hWnd = NULL;
        return hWnd;
    }

    HWND CMediaUI::GetHWND()
    {
        return m_hWnd;
    }
}