
#pragma once

struct _EXCEPTION_POINTERS;

class CMiniDumper
{
public:
	static void Enable(LPCTSTR pszAppName, bool bShowErrors,bool b_user_mode=true); //如果是普通用户模式 为真。表示不按时间命名DMP文件

private:
	static TCHAR m_szAppName[MAX_PATH];
	
public:
	static HMODULE GetDebugHelperDll(FARPROC* ppfnMiniDumpWriteDump, bool bShowErrors);
	static LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS* pExceptionInfo);
public:
	bool mb_user_mode;
};

extern CMiniDumper theCrashDumper;
