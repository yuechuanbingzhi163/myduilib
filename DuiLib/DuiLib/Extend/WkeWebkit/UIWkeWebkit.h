#ifndef UIWkeWebkit_h__
#define UIWkeWebkit_h__

#pragma once

#include "wke.h"

namespace DuiLib
{
#ifdef _UNICODE
	typedef std::wstring wkeString;
#else
	typedef std::string wkeString;
#endif
	const char TM_TICKER = 1;

	class CWkeWebkitWnd;
	
	class UILIB_API CWkeWebkitUI : public CControlUI
	{
		friend class CWkeWebkitWnd;
	public:
		CWkeWebkitUI(void);
		~CWkeWebkitUI(void);
		
		static void WkeWebkit_Init();
		static void WkeWebkit_Shutdown();

		static LPCTSTR GetClassName();
		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);

		void	SetInternVisible(bool bVisible = true);
		void	DoInit();
		void	SetPos(RECT rc);
		void	DoEvent(TEventUI& event);	
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void	SetURL( wkeString strValue);
		void	SetFile(wkeString strValue);
		wkeString RunJS(wkeString strValue);

		void	SetClientHandler(const wkeClientHandler* handler);

		void	GoBack();
		void	GoForward();

	protected:
		CWkeWebkitWnd *m_pWindow;
	};

}

#endif//UIWkeWebkit_h__