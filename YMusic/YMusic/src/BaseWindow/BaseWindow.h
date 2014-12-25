#ifndef __BaseWindow_h
#define __BaseWindow_h

#pragma once
#include "DuiLib/UIlib.h"

#include "DuiLib/UIMenu.h"
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif

using namespace DuiLib;

#include "DuiLib/skin_change_event.hpp"

enum USER_MSG
{
	WM_WEB_SERVICE = WM_USER + 1024,
	WM_HTTP_FILE_DOWNLOAD,
	WM_INSERT_EXPRESSION,
	WM_MENU_NOTIFY,
	WM_TRAY_NOTIFY,
	WM_ENDING_SCROLL,
	WM_LRC_FINISH,
	WM_LRC_LODING,
	WM_CMD,
	WM_LOGIN,
	WM_USER_PLAYING,                      // 开始播放文件
	WM_USER_POS_CHANGED,                  // 文件播放位置改变
	WM_USER_END_REACHED,                  // 播放完毕
};
enum MSG_WPARAM
{
	WP_NOTHING = 1,
	WP_LRCLOAD ,
	WP_LOADING ,
	WP_LOAD_FINISH,
};
/************************************************************************/
/*                                                                      */
/************************************************************************/
class CBaseWindow :
	public DuiLib::CWin
{
	
public:
	
	CBaseWindow();
	~CBaseWindow();
	
public:
//	template<typename T>
//	inline void FindControl(T& pctrl,LPCTSTR pstrName) const
//	{
//		ASSERT(GetPaintMgr()&&pstrName);
//		pctrl = static_cast<T>(GetPaintMgr()->FindControl(pstrName));
//		ASSERT(pctrl != 0);
//		if (0 == pctrl)
//		{
////			sLogError("FindControl %s Error",pstrName);
//		}
//	}

protected:
	LPCTSTR GetWindowClassName() const;
	virtual UINT GetClassStyle() const;
protected:
	virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnWebService(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void OnWebService(void *pRequest ,void *pProto);
};



#endif//__BaseWindow_h