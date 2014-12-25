#ifndef __UIPLAYER_H__
#define __UIPLAYER_H__

#pragma once
// #include <strmif.h>
// #include <control.h>
// #include <evcode.h>
// #include <uuids.h>
#include <dshow.h>

namespace DuiLib
{
	enum VideoMsg
	{
		WM_GRAPH_NOTIFY = WM_USER + 1984
	};
	enum PlayState
	{
		PLAYER_STATE_STOPPED = 1,
		PLAYER_STATE_PAUSED = 2,
		PLAYER_STATE_RUNNING = 3
	};
	class IPlayerUI
	{
	public:
		virtual void SetUrl(const LPCTSTR url) = 0;
		virtual bool Play() = 0;
		virtual bool Pause() = 0;
		virtual bool Stop() = 0;
		virtual int  GetState() = 0;
		virtual LONGLONG GetDuration() = 0;
		virtual LONGLONG GetPosition() = 0;
		virtual bool SetPosition(LONGLONG pos) = 0;
		virtual bool GetFullScreenMode() = 0;
		virtual bool SetFullScreenMode(bool full_screen) = 0;
		virtual int  GetVolume() = 0;
		virtual bool SetVolume(int volume) = 0;
	};
	class CPlayerWnd;
	class/* UILIB_API */CPlayerUI : public CControlUI,public IPlayerUI
	{
		friend class CPlayerWnd;
	public:
		CPlayerUI();
		~CPlayerUI();

		// add by rz.li
		static LPCTSTR  GetClassName();
//		bool           IsName(const LPCTSTR pstrName);
		LPCTSTR		   GetClass() const;
		LPVOID		   GetInterface(LPCTSTR pstrName);

		void SetVisible(bool bVisible);
		void SetInternVisible(bool bVisible);
		void SetPos(RECT rc);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void DoPaint(HDC hDC, const RECT& rcPaint);

		HWND GetHWND();

		void OnGraphNotify(UINT msg, WPARAM wparam, LPARAM lparam);
		//
		void SetUrl(const LPCTSTR url);
		bool Play();
		bool Pause();
		bool Stop();
		int  GetState();
		LONGLONG GetDuration();
		LONGLONG GetPosition();
		bool SetPosition(LONGLONG pos);
		bool GetFullScreenMode();
		bool SetFullScreenMode(bool full_screen);
		int  GetVolume();
		bool SetVolume(int volume);

	protected:
		CPlayerWnd      *m_pWindow;

		IGraphBuilder   *m_pGraph;
		IMediaControl   *m_pControl;
		IMediaSeeking   *m_pSeeking;
		IMediaEvent     *m_pEvent;
		IMediaEventEx   *m_pEventEx;
		IVideoWindow    *m_pVideoWin;
		IBasicAudio     *m_pAudio;
		IBasicVideo     *m_pVideo;

		CDuiString       m_url;
	};

} // namespace DuiLib

#endif // __UIPLAYER_H__