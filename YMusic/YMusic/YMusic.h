#pragma once

#include "resource.h"

#include <curl/curl.h>
#pragma comment(lib,"curllib.lib")

#define _CONSOLE
#include "./src/BaseWindow/BaseWindow.h"

#include "./src/Utilty/Convert/Convert.h"
using namespace em_utility;

namespace DuiLib
{
	typedef   CBaseWindow  YWindow;
	typedef   CDuiString   YString;

#define _count_(x)  (sizeof(x)/sizeof(*x))
	void inline _consoletrace(LPCTSTR pstrFormat,...)
	{
#if defined(_DEBUG) && defined(_CONSOLE)
		TCHAR szBuffer[300] = { 0 };
		va_list args;
		va_start(args, pstrFormat);
		::wvnsprintf(szBuffer, _count_(szBuffer) - 2, pstrFormat, args);
//		_tcscat(szBuffer, _T("\n"));
		va_end(args);

		time_t t = time(NULL);
		tm* aTm = localtime(&t);

		CString sTrDebug;
		sTrDebug.Format(_T("\n[%-4d-%02d-%02d %02d:%02d:%02d][%lX]|[%s] "),
			aTm->tm_year+1900,
			aTm->tm_mon+1,
			aTm->tm_mday,
			aTm->tm_hour,
			aTm->tm_min,
			aTm->tm_sec,
			::GetCurrentThreadId(),
			szBuffer);
		::OutputDebugString(sTrDebug);
#endif
	}
#define YTrace_ _consoletrace
};

#include "./src/Variable/Var.h"

#include "./src/Utilty/Thread.h"
#include "./src/Utilty/Singleton.h"
#include "./src/Utilty/SmartPtr.h"
#include "./src/Utilty/log/log.h"
#include "./src/Utilty/win_io/FileMgr.h"
#include "./src/Utilty/down_http_file.h"
#include "./src/Utilty/UrlPraser.h"

#include "./src/DataBase/ShareData.h"

#include "./src/Login/YLogin.h"
#include "./src/MenuWindow/MainMenu.h"
#include "./src/SkinWindow/SkinWindow.h"

#include "./src/Utilty/md5_checksum.h"


#include "./src/CacheMgr/CacheMgr.h"
#include "./src/FilePathMgr/FilePathMgr.h"
//#include "./src/PlayMgr/PlayCenter.h"

#include "./src/PlayMgr/AVPlayer.h"
#include "./src/PlayMgr/PlayListMgr.h"

#include "./src/DataBase/DatabaseManager.h"

#include "./src/PlayListItem/YPlayListItem.h"
#include "./src/SongListItem/YSongListItem.h"

#include "./src/Setting/ConfigMgr.h"
#include "./src/LrcMgr/LrcMgr.h"

#include "./src/WebService/HttpManager.h"
#include "./src/WebService/ServerManager.h"

#include "./src/Utilty/base/YTingRuntime.h"

#include "./src/WebService/WebService.h"

#include "./src/BaseWindow/InfoWindow.h"
#include "./src/BaseWindow/trayIcon.h"


