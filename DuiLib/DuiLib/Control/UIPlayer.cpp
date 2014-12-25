#include "stdafx.h"
#include "UIPlayer.h"

#pragma comment(lib,"strmiids.lib")
#pragma comment(lib,"quartz.lib")
namespace DuiLib
{
	class CPlayerWnd : public CWindowWnd
	{
	public:
		CPlayerWnd();

		void Init(CPlayerUI *owner);

		LPCTSTR GetWindowClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		LRESULT OnMouseActivate(UINT usg, WPARAM wparam, LPARAM lParam,BOOL &handled);
		LRESULT OnSetFocus(UINT msg, WPARAM wparam, LPARAM lParam, BOOL &handled);
		LRESULT OnKillFocus(UINT msg, WPARAM wparam, LPARAM lParam, BOOL &handled);
		LRESULT OnEraseBkgnd(UINT msg, WPARAM wparam, LPARAM lParam,BOOL &handled);
		LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lParam, BOOL &handled);

	protected:
		CPlayerUI *m_pOwner;
	};
	CPlayerWnd::CPlayerWnd()
		:m_pOwner(NULL)
	{

	}
	void
	CPlayerWnd::Init(CPlayerUI *owner)
	{
		m_pOwner = owner;
		CDuiRect rcPos = m_pOwner->GetPos();
		UINT uStyle = WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;

		Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
	}
	LPCTSTR
	CPlayerWnd::GetWindowClassName() const 
	{
		return _T("PlayerWnd");
	}
	void
	CPlayerWnd::OnFinalMessage(HWND hWnd)
	{
		m_pOwner->m_pWindow = NULL;

		delete this;
	}
	LRESULT
	CPlayerWnd::OnMouseActivate(UINT usg, WPARAM wparam, LPARAM lParam, BOOL &handled)
	{
		handled = FALSE;

		return 0;
	}
	LRESULT
	CPlayerWnd::OnSetFocus(UINT msg, WPARAM wparam, LPARAM lParam, BOOL &handled)
	{
		handled = FALSE;

		m_pOwner->m_bFocused = true;

		return 0;
	}
	LRESULT
	CPlayerWnd::OnKillFocus(UINT msg, WPARAM wparam, LPARAM lParam, BOOL &handled)
	{
		handled = FALSE;

		m_pOwner->m_bFocused = false;

		return 0;
	}
	LRESULT
	CPlayerWnd::OnEraseBkgnd(UINT msg, WPARAM wparam, LPARAM lParam, BOOL &handled)
	{
#if 0
		return 1;
#else
		handled = FALSE;

		return 0;
#endif
	}
	LRESULT
	CPlayerWnd::OnPaint(UINT msg, WPARAM wparam, LPARAM lParam, BOOL &handled)
	{
#if 0
		PAINTSTRUCT  ps = { 0 };

		::BeginPaint(m_hWnd, &ps);
		::EndPaint(m_hWnd, &ps);

		return 1;
#else
		PAINTSTRUCT  ps = { 0 };

		::BeginPaint(m_hWnd, &ps);
		::EndPaint(m_hWnd, &ps);

		handled = FALSE;

		return 0;
#endif
	}
	LRESULT
	CPlayerWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		BOOL     handled;
		LRESULT  rc;

		handled = TRUE;
		rc = 0;
		switch (uMsg)
		{
		case WM_PAINT:
			rc = OnPaint(uMsg, wParam, lParam, handled);
			break;
		case WM_SETFOCUS:
			rc = OnSetFocus(uMsg, wParam, lParam, handled);
			break;
		case WM_KILLFOCUS:
			rc = OnKillFocus(uMsg, wParam, lParam, handled);
			break;
		case WM_ERASEBKGND:
			rc = OnEraseBkgnd(uMsg, wParam, lParam, handled);
			break;
		case WM_MOUSEACTIVATE:
			rc = OnMouseActivate(uMsg, wParam, lParam, handled);
			break;
		case WM_MOVE:
		case WM_GRAPH_NOTIFY:
			if (m_pOwner != NULL) {
				m_pOwner->OnGraphNotify(uMsg, wParam, lParam);
			}
			handled = FALSE;
			break;

		default:
			handled = FALSE;
			break;
		}
		if (handled) {
			return rc;
		}

		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}
	/////////////////////////////////////////////////////
	CPlayerUI::CPlayerUI()
		: m_pWindow(NULL)
		, m_pGraph(NULL)
		, m_pControl(NULL)
		, m_pSeeking(NULL)
		, m_pEvent(NULL)
		, m_pEventEx(NULL)
		, m_pVideoWin(NULL)
		, m_pAudio(NULL)
		, m_pVideo(NULL)
	{

	}
	CPlayerUI::~CPlayerUI()
	{
		Stop();
	}
	LPCTSTR
	CPlayerUI::GetClassName()
	{
		return _T("PlayerUI");
	}
	LPCTSTR
	CPlayerUI::GetClass() const 
	{
		return CPlayerUI::GetClassName();
	}
// 	bool
// 	CPlayerUI::IsName(const LPCTSTR pstrName)
// 	{
// 		return (!this->GetName().IsEmpty())&&(_tcscmp(this->GetName().GetData(),pstrName) == 0);
// 	}
	LPVOID
	CPlayerUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("Player")) == 0 ) return static_cast<CPlayerUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}
	void
	CPlayerUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);

		if (m_pWindow != NULL) {
			::ShowWindow(*m_pWindow, IsVisible() ? SW_SHOW : SW_HIDE);
		}
	}
	void
	CPlayerUI::SetInternVisible(bool bVisible)
	{
		CControlUI::SetInternVisible(bVisible);

		if (m_pWindow != NULL) {
			::ShowWindow(*m_pWindow, IsVisible() ? SW_SHOW : SW_HIDE);
		}
	}
	void 
	CPlayerUI::SetPos(RECT rc)
	{
		CControlUI::SetPos(rc);

		if (m_pWindow == NULL) {
			return;
		}

		MoveWindow(*m_pWindow, m_rcItem.left, m_rcItem.top,
			m_rcItem.right - m_rcItem.left,
			m_rcItem.bottom - m_rcItem.top, TRUE);

		if (m_pVideoWin != NULL) {
			m_pVideoWin->SetWindowPosition(0, 0, rc.right - rc.left,
				rc.bottom - rc.top);
		}
	}
	void
	CPlayerUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		CControlUI::SetAttribute(pstrName, pstrValue);
	}
	void
	CPlayerUI::DoPaint(HDC hDC, const RECT& rcPaint)
	{
		CControlUI::DoPaint(hDC, rcPaint);
	}
	HWND
	CPlayerUI::GetHWND()
	{
		if (m_pWindow == NULL) {
			return NULL;
		}

		return m_pWindow->GetHWND();
	}
	void
	CPlayerUI::OnGraphNotify(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		long     ev, param1, param2;
		HRESULT  rc;

		if (msg == WM_MOVE)
		{
			if (m_pVideoWin != NULL) 
			{
				m_pVideoWin->NotifyOwnerMessage((OAHWND) m_pWindow->GetHWND(), msg,
					wparam, lparam);
			}

		}
		else if (msg == WM_GRAPH_NOTIFY) 
		{
			if (m_pEvent == NULL) 
			{
				return;
			}
			do 
			{
				rc = m_pEvent->GetEvent(&ev, &param1, &param2, 0);
				if (FAILED(rc)) 
				{
					break;
				}
				switch (ev) 
				{
				case EC_PAUSED:
					GetManager()->SendNotify(this, _T("player_paused"));
					break;
				case EC_COMPLETE:
					GetManager()->SendNotify(this, _T("player_fullscreen_lost"));
					GetManager()->SendNotify(this, _T("player_completed"), 0, 0, true);
					break;
				case EC_USERABORT:
					GetManager()->SendNotify(this, _T("player_user_aborted"));
					break;
				case EC_FULLSCREEN_LOST:
					GetManager()->SendNotify(this, _T("player_fullscreen_lost"));
					break;
				default:
					break;
				}
				m_pEvent->FreeEventParams(ev, param1, param2);
			} while (SUCCEEDED(rc));
		}
	}
	///////////////////////////////
	void
	CPlayerUI::SetUrl(const LPCTSTR url)
	{
		m_url = url;
	}
	bool 
	CPlayerUI::Play()
	{
		RECT            rect;
		HRESULT          rc;
		LPCTSTR          url;

		if (m_pWindow == NULL) 
		{
			m_pWindow = new CPlayerWnd();
			if (m_pWindow == NULL) 
				return false;

			m_pWindow->Init(this);
		} 
		else 
		{
			rc = m_pControl->Run();
			if (FAILED(rc)) 
				goto error;
			
			return true;
		}

		url = m_url;

	

		rc = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
			IID_IGraphBuilder, (void **) &m_pGraph);
		if (FAILED(rc)) {
			goto error;
		}

		rc = m_pGraph->QueryInterface(IID_IMediaControl, (void **) &m_pControl);
		rc = m_pGraph->QueryInterface(IID_IMediaSeeking, (void **) &m_pSeeking);
		rc = m_pGraph->QueryInterface(IID_IMediaEvent, (void **) &m_pEvent);
		rc = m_pGraph->QueryInterface(IID_IMediaEventEx, (void **) &m_pEventEx);
		rc = m_pGraph->QueryInterface(IID_IVideoWindow, (void **) &m_pVideoWin);
		rc = m_pGraph->QueryInterface(IID_IVideoWindow, (void **) &m_pAudio);
		rc = m_pGraph->QueryInterface(IID_IVideoWindow, (void **) &m_pVideo);

		rc = m_pGraph->RenderFile((LPCWSTR) url, NULL);
		if (FAILED(rc)) {
			goto error;
		}

		m_pVideoWin->put_Owner((OAHWND) m_pWindow->GetHWND());
		m_pVideoWin->put_MessageDrain((OAHWND) GetManager()->GetPaintWindow());
		m_pVideoWin->put_WindowStyle(WS_CHILD|WS_CLIPSIBLINGS);

		m_pEventEx->SetNotifyWindow((OAHWND) m_pWindow->GetHWND(),
			WM_GRAPH_NOTIFY, 0);

		rect = GetPos();
		m_pVideoWin->SetWindowPosition(0, 0, rect.right - rect.left,
			rect.bottom - rect.top);

		rc = m_pControl->Run();
		if (FAILED(rc)) {
			goto error;
		}

		return true;

error:

		Stop();

		return false;
	}
	bool
	CPlayerUI::Pause()
	{
		HRESULT  rc;

		if (m_pWindow == NULL) {
			return false;
		}

		rc = m_pControl->Pause();
		if (FAILED(rc)) {
			Stop();
			return false;
		}

		return true;
	}
	bool
	CPlayerUI::Stop()
	{
		HRESULT  rc;

		if (m_pWindow == NULL) {
			return false;
		}

		m_pEvent->Release();
		m_pEvent = NULL;

		m_pEventEx->SetNotifyWindow(NULL, 0, 0);
		m_pEventEx->Release();
		m_pEventEx = NULL;

		m_pSeeking->Release();
		m_pSeeking = NULL;

		rc = m_pControl->Stop();
		m_pControl->Release();
		m_pControl = NULL;

		m_pAudio->Release();
		m_pAudio = NULL;

		m_pVideo->Release();
		m_pVideo = NULL;

		m_pVideoWin->put_Visible(OAFALSE);
		m_pVideoWin->put_Owner(NULL);
		m_pVideoWin->Release();
		m_pVideoWin = NULL;

		m_pGraph->Release();
		m_pGraph = NULL;

		::DestroyWindow(*m_pWindow);
		delete m_pWindow;
		m_pWindow = NULL;

		return true;
	}
	int
	CPlayerUI::GetState()
	{
		HRESULT        rc;
		OAFilterState  state;

		if (m_pControl == NULL) {
			return PLAYER_STATE_STOPPED;
		}

		rc = m_pControl->GetState(500, &state);
		if (FAILED(rc)) {
			return -1;
		}

		switch (state) {

		case State_Stopped:
			return PLAYER_STATE_STOPPED;
		case State_Paused:
			return PLAYER_STATE_PAUSED;
		case State_Running:
			return PLAYER_STATE_RUNNING;
		default:
			return -1;
		}
	}
	LONGLONG
	CPlayerUI::GetDuration()
	{
		HRESULT   rc;
		LONGLONG  duration;

		if (m_pSeeking == NULL) {
			return -1;
		}

		rc = m_pSeeking->GetDuration(&duration);
		if (FAILED(rc)) {
			return -1;
		}

		return duration;
	}
	LONGLONG 
	CPlayerUI::GetPosition()
	{
		HRESULT   rc;
		LONGLONG  cur_pos;

		if (m_pSeeking == NULL) {
			return -1;
		}

		rc = m_pSeeking->GetCurrentPosition(&cur_pos);
		if (FAILED(rc)) {
			return -1;
		}

		return cur_pos;
	}
	bool
	CPlayerUI::SetPosition(LONGLONG pos)
	{
		HRESULT  rc;

		if (m_pSeeking == NULL) {
			return false;
		}

		rc = m_pSeeking->SetPositions(&pos, AM_SEEKING_AbsolutePositioning, NULL,
			AM_SEEKING_NoPositioning);
		if (FAILED(rc)) {
			return false;
		}

		return true;
	}
	bool 
	CPlayerUI::GetFullScreenMode()
	{
		long     full_screen;
		HRESULT  rc;

		if (m_pVideoWin == NULL) {
			return false;
		}

		rc = m_pVideoWin->get_FullScreenMode(&full_screen);
		if (FAILED(rc)) {
			return false;
		}

		return full_screen == OATRUE ? true : false;
	}
	bool
	CPlayerUI::SetFullScreenMode(bool full_screen)
	{
		HRESULT  rc;

		if (m_pVideoWin == NULL) {
			return false;
		}

		rc = m_pVideoWin->put_FullScreenMode(full_screen ? OATRUE : OAFALSE);
		if (FAILED(rc)) {
			return false;
		}

		return true;
	}
	int 
	CPlayerUI::GetVolume()
	{
		long     volume;
		HRESULT  rc;

		if (m_pAudio == NULL) {
			return -1;
		}

		rc = m_pAudio->get_Volume(&volume);
		if (FAILED(rc)) {
			return -1;
		}

#if 0
		return (volume + 10000) / 100;
#else
		return volume;
#endif
	}
	bool
	CPlayerUI::SetVolume(int volume)
	{
		HRESULT  rc;

		if (m_pAudio == NULL) {
			return false;
		}

#if 0
		volume -= 100;
		volume *= 100;
#endif

		rc = m_pAudio->put_Volume(volume);
		if (FAILED(rc)) {
			return false;
		}

		return true;
	}
}