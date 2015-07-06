// App.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>

#define			Block_Num			4

inline HBITMAP CreateMyBitmap(HDC hDC, int cx, int cy, COLORREF** pBits)
{
	LPBITMAPINFO lpbiSrc = NULL;
	lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
	if (lpbiSrc == NULL) return NULL;

	lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbiSrc->bmiHeader.biWidth = cx;
	lpbiSrc->bmiHeader.biHeight = cy;
	lpbiSrc->bmiHeader.biPlanes = 1;
	lpbiSrc->bmiHeader.biBitCount = 32;
	lpbiSrc->bmiHeader.biCompression = BI_RGB;
	lpbiSrc->bmiHeader.biSizeImage = cx * cy;
	lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
	lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
	lpbiSrc->bmiHeader.biClrUsed = 0;
	lpbiSrc->bmiHeader.biClrImportant = 0;

	HBITMAP hBitmap = CreateDIBSection(hDC, lpbiSrc, DIB_RGB_COLORS, (void **)pBits, NULL, NULL);
	delete[] lpbiSrc;
	return hBitmap;
}

class CLoginFrameWnd : public CWindowWnd, public IMessageFilterUI
{
public:
	CLoginFrameWnd() { m_timer_count = NULL; m_timer = NULL; m_cutting = true; m_cur_index = 0; m_cut_size = 0; };
    LPCTSTR GetWindowClassName() const { return _T("UILoginFrame"); };
    UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };

    void OnFinalMessage(HWND /*hWnd*/) 
    { 
        m_pm.RemovePreMessageFilter(this);
        delete this; 
    };

	void PaintArrow(HDC hdc, const RECT& rc)
	{
		if (m_cur_index >= Block_Num)
		{
			int s = m_timer_count;
			return;
		}

		RECT copy_rc = rc;
		if (m_cutting)
		{
			RECT rc_paint;
			CDuiString arrow_img; //140*140
			RECT pos = blocks[m_cur_index]->GetPos();
			switch (m_cur_index)
			{
			case 0:
				rc_paint.top = pos.top - 70;
				rc_paint.bottom = pos.top + 70 + m_cut_size;
				rc_paint.left = pos.right - 70;
				rc_paint.right = pos.right + 70;

				copy_rc.top = pos.top;
				arrow_img.Format(_T("file='s%d.png' corner='0,0,0,100'"), m_timer_count % 30 + 1);
				m_cut_size += 2;
				if (rc_paint.bottom - rc_paint.top > 602 + 115)
				{
					m_cut_size = 0;
					m_cutting = false;
				}
				break;
			case 1:
				rc_paint.top = pos.bottom - 70;
				rc_paint.bottom = pos.bottom + 70;
				rc_paint.left = pos.left - 70;
				rc_paint.right = pos.left + 70 + m_cut_size;

				copy_rc.left = pos.left;
				arrow_img.Format(_T("file='h%d.png' corner='0,0,100,0'"), m_timer_count % 30 + 1);
				m_cut_size += 2;
				if (rc_paint.right - rc_paint.left > 618 + 115)
				{
					m_cut_size = 0;
					m_cutting = false;
				}
				break;
			case 2:
				rc_paint.top = pos.top - 70;
				rc_paint.bottom = pos.top + 70 + m_cut_size;
				rc_paint.left = pos.left - 70;
				rc_paint.right = pos.left + 70;

				copy_rc.top = pos.top;
				arrow_img.Format(_T("file='s%d.png' corner='0,0,0,100'"), m_timer_count % 30 + 1);
				m_cut_size += 2;
				if (rc_paint.bottom - rc.top > 562 + 115)
				{
					m_cut_size = 0;
					m_cutting = false;
				}
				break;

			case 3:
				rc_paint.top = pos.top - 70;
				rc_paint.bottom = pos.top + 70;
				rc_paint.left = pos.left - 70;
				rc_paint.right = pos.left + 70 + m_cut_size;

				copy_rc.left = pos.left;
				arrow_img.Format(_T("file='h%d.png' corner='0,0,100,0'"), m_timer_count % 30 + 1);
				m_cut_size += 2;
				if (rc_paint.right - rc_paint.left > 348 + 115)
				{
					m_cut_size = 0;
					m_cutting = false;
				}
				break;
			default:
				break;
			}

			CRenderClip clip;
			CRenderClip::GenerateClip(hdc, copy_rc, clip);
			CRenderEngine::DrawImageString(hdc, &m_pm, rc_paint, copy_rc, arrow_img);

			CRenderClip::UseOldClipBegin(hdc, clip);
		}
	}

    void Init() 
	{
		m_transparent_rc.left = 290;
		m_transparent_rc.right = 290;
		m_transparent_rc.top = 50;
		m_transparent_rc.bottom = 100;

		for (int i = 0; i < Block_Num; ++i)
		{
			CDuiString block_name;
			block_name.Format(_T("control_%d"), i);
			blocks[i] = m_pm.FindControl(block_name);
			ASSERT(blocks[i]);
		}

		m_center_img = m_pm.FindControl(_T("center_img"));
		ASSERT(m_center_img);

		m_progress_text = static_cast<CTextUI*>(m_pm.FindControl(_T("progress_text")));
		ASSERT(m_progress_text);

		m_timer = SetTimer(m_hWnd, 0, 20, NULL);
		CenterWindow();
    }

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
        styleValue &= ~WS_CAPTION;
        ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

        m_pm.Init(m_hWnd);
        m_pm.AddPreMessageFilter(this);
        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(_T("login.xml"), (UINT)0, NULL, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);

        Init();
        return 0;
    }

    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if( ::IsIconic(*this) ) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
    }

    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

	void DoPaint()
	{
		RECT rcClient = { 0 };
		::GetClientRect(m_hWnd, &rcClient);
		DWORD dwWidth = rcClient.right - rcClient.left;
		DWORD dwHeight = rcClient.bottom - rcClient.top;

		HDC hDcPaint = ::GetDC(m_hWnd);
		HDC hDcBackground = ::CreateCompatibleDC(hDcPaint);
		COLORREF* pBackgroundBits;
		HBITMAP hbmpBackground = CreateMyBitmap(hDcPaint, dwWidth, dwHeight, &pBackgroundBits);
		::ZeroMemory(pBackgroundBits, dwWidth * dwHeight * 4);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hDcBackground, hbmpBackground);
		SetBkMode(hDcBackground, OPAQUE);
		m_pm.GetRoot()->SetPos(rcClient);
		m_pm.GetRoot()->DoPaint(hDcBackground, rcClient);
		
		PaintArrow(hDcBackground, rcClient);
		ResetAlpha((BYTE*)pBackgroundBits, dwWidth, dwHeight);

		RECT rcWnd = { 0 };
		::GetWindowRect(m_hWnd, &rcWnd);

		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		POINT ptPos = { rcWnd.left, rcWnd.top };
		SIZE sizeWnd = { dwWidth, dwHeight };
		POINT ptSrc = { 0, 0 };
		UpdateLayeredWindow(m_hWnd, hDcPaint, &ptPos, &sizeWnd, hDcBackground, &ptSrc, 0, &bf, ULW_ALPHA);

		::SelectObject(hDcBackground, hOldBitmap);
		if (hDcBackground != NULL) ::DeleteDC(hDcBackground);
		if (hbmpBackground != NULL) ::DeleteObject(hbmpBackground);
		::ReleaseDC(m_hWnd, hDcPaint);
	}

	void ShowWindow(bool bShow  = true, bool bTakeFocus  = true)
	{
		DoPaint();
		__super::ShowWindow(bShow, bTakeFocus);
	}

    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);

        RECT rcClient;
        ::GetClientRect(*this, &rcClient);

        RECT rcCaption = m_pm.GetCaptionRect();
        if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
                if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 )
                    return HTCAPTION;
        }

        return HTCLIENT;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }

        bHandled = FALSE;
        return 0;
    }

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_timer_count++;
		float progress = (float)m_timer_count / 1070 * 100;
		if (progress <= 100)
		{
			CDuiString progress_str;
			progress_str.Format(_T("%.1f%%"), progress);
			m_progress_text->SetText(progress_str);
		}
		
		if (m_cutting)
		{
			RECT rcClient = { 0 };
			::GetClientRect(m_hWnd, &rcClient);
			::InvalidateRect(m_hWnd, &rcClient, false);
		}
		else
		{
			CDuiString img_str;
			switch (m_cur_index)
			{
			case 0:
				m_cut_size += 2;
				img_str.Format(_T("file='Big.jpg' source='0,0,284,602' dest='0,%d,284,%d' fade='%d'"), m_cut_size, 602 + m_cut_size, 255 * (20 - m_cut_size) / 20);
				break;
			case 1:
				m_cut_size += 2;
				img_str.Format(_T("file='Big.jpg' source='284,0,902,%d' dest='0,%d,618,40' fade='%d'"), 40 - m_cut_size, m_cut_size, 255 * (20 - m_cut_size) / 20);
				break;
			case 2:
				m_cut_size += 2;
				img_str.Format(_T("file='Big.jpg' source='632,40,902,602' dest='0,%d,270,%d' fade='%d'"), m_cut_size, 562 + m_cut_size, 255 * (20 - m_cut_size) / 20);
				break;
			case 3:
				m_cut_size += 2;
				img_str.Format(_T("file='Big.jpg' source='284,%d,632,602' dest='0,0,348,%d' fade='%d'"), 572 + m_cut_size, 30 - m_cut_size, 255 * (20 - m_cut_size) / 20);
				break;
			default:
				break;
			}

			blocks[m_cur_index]->SetBkImage(img_str);
			if (m_cut_size >= 20)
			{
				m_cutting = true;
				m_cut_size = 0;
				m_cur_index++;

				CDuiString bkimage;
				bkimage.Format(_T("wording%d.png"), m_cur_index + 1);
				m_center_img->SetBkImage(bkimage);
			}
		}
		return 0;
	}

	void SetAlpha(BYTE* pImage, int width, int height, RECT& rc, BYTE alpha)
	{
		ASSERT(rc.right < width && rc.bottom < height && rc.left >= 0 && rc.top >= 0);

		DWORD dwAlpha = alpha << 24;
		for (int i = rc.top; i < rc.bottom; i++)
		{
			unsigned int* pSrc = (unsigned int*)pImage + (height - i) * width;
			for (int j = rc.left; j < rc.right; j++)
			{
				pSrc[j] |= dwAlpha;
			}
		}
	}

	void ResetAlpha(BYTE* pImage, int width, int height)
	{
		if (m_transparent_rc.left == -1)
			return;

		RECT rc = m_transparent_rc;
		rc.right = width - m_transparent_rc.right;
		rc.bottom = height - m_transparent_rc.bottom;
		SetAlpha(pImage, width, height, rc, 0xFF);
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		DoPaint();

		RECT rcClient = { 0 };
		::GetClientRect(m_hWnd, &rcClient);
		::ValidateRect(m_hWnd, &rcClient);

		bHandled = FALSE;
		return S_OK;
	}

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch( uMsg ) {
        case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
        case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
        case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
        case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
        case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
        case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_TIMER:         lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
        default:
            bHandled = FALSE;
        }
        if( bHandled ) return lRes;
        if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }

    LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
    {
       if (uMsg == WM_PAINT)
		{
			BOOL handled = false;
			OnPaint(uMsg, wParam, lParam, handled);
			handled = true;
			return true;
		}  
        return false;
    }

public:
    CPaintManagerUI m_pm;

	CControlUI *blocks[Block_Num];
	CControlUI *m_center_img;
	CTextUI* m_progress_text;

	RECT m_transparent_rc;
	UINT_PTR m_timer;

	UINT m_timer_count;
	UINT m_cur_index;
	UINT m_cut_size;
	bool m_cutting;
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
    CPaintManagerUI::SetResourceZip(_T("GameRes.zip"));

    HRESULT Hr = ::CoInitialize(NULL);
    if( FAILED(Hr) ) return 0;

	CLoginFrameWnd* pFrame = new CLoginFrameWnd();
    if( pFrame == NULL ) return 0;
	HWND s = pFrame->Create(NULL, _T(""), WS_POPUP, WS_EX_LAYERED, 0, 0, 902, 602);
  //  pFrame->CenterWindow();
    pFrame->ShowWindow(SW_SHOW);

    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
    return 0;
}
