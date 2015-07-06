#include "mdump.h"
#include <dbghelp.h>
#include <assert.h>
#include <ShellAPI.h>
#include <ShlObj.h>
#include <time.h>
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
										 CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
										 CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
										 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

CMiniDumper theCrashDumper;
TCHAR CMiniDumper::m_szAppName[MAX_PATH] = {0};

void CMiniDumper::Enable(LPCTSTR pszAppName, bool bShowErrors,bool b_user_mode)
{
	TCHAR s_process_name[MAX_PATH];
	LPCTSTR p_app_name = NULL;
	s_process_name[0] = 0x0;
	theCrashDumper.mb_user_mode=b_user_mode;
	//if (p_app_name == NULL)
	{
		TCHAR s_buf[MAX_PATH];
		s_buf[0] = 0x0;
		DWORD n_result = ::GetModuleFileName(NULL, s_buf, sizeof(TCHAR) * MAX_PATH);
		TCHAR	s_file_name[MAX_PATH];
		s_file_name[0] = 0x0;
		_tsplitpath(s_buf, NULL, NULL, s_file_name, NULL);
		_tcscpy_s(s_process_name, MAX_PATH, s_file_name);
		if (pszAppName == NULL || _tcslen(pszAppName) == 0)
			p_app_name = s_process_name;
		else
			p_app_name = pszAppName;
	}
	assert(p_app_name != NULL);
	assert( m_szAppName[0] == _T('\0') );
	_tcsncpy(m_szAppName, p_app_name, wcslen(p_app_name));  
     
	MINIDUMPWRITEDUMP pfnMiniDumpWriteDump = NULL;
	HMODULE hDbgHelpDll = GetDebugHelperDll((FARPROC*)&pfnMiniDumpWriteDump, bShowErrors);
	if (hDbgHelpDll)
	{
		if (pfnMiniDumpWriteDump)
			SetUnhandledExceptionFilter(TopLevelFilter);
		FreeLibrary(hDbgHelpDll);
		hDbgHelpDll = NULL;
		pfnMiniDumpWriteDump = NULL;
	}
}

HMODULE CMiniDumper::GetDebugHelperDll(FARPROC* ppfnMiniDumpWriteDump, bool bShowErrors)
{
	*ppfnMiniDumpWriteDump = NULL;
	HMODULE hDll = LoadLibrary(_T("DBGHELP.DLL"));
	if (hDll == NULL)
	{
		if (bShowErrors) {

			MessageBox(NULL, _T("DBGHELP.DLL not found. Please install a DBGHELP.DLL."), m_szAppName, MB_ICONSTOP | MB_OK);
		
		}
	}
	else
	{
		*ppfnMiniDumpWriteDump = GetProcAddress(hDll, "MiniDumpWriteDump");
		if (*ppfnMiniDumpWriteDump == NULL)
		{
			if (bShowErrors) {
				
				MessageBox(NULL, _T("DBGHELP.DLL found is too old. Please upgrade to a newer version of DBGHELP.DLL."), m_szAppName, MB_ICONSTOP | MB_OK);
			}
		}
	}
	return hDll;
}

LONG CMiniDumper::TopLevelFilter(struct _EXCEPTION_POINTERS* pExceptionInfo)
{
	   
	LONG lRetValue = EXCEPTION_CONTINUE_SEARCH;
	TCHAR szResult[_MAX_PATH + 1024] = {0};
	MINIDUMPWRITEDUMP pfnMiniDumpWriteDump = NULL;
	HMODULE hDll = GetDebugHelperDll((FARPROC*)&pfnMiniDumpWriteDump, true);
	HINSTANCE	hInstCrashReporter = NULL;	
	if (hDll)
	{
		if (pfnMiniDumpWriteDump)
		{

			{
				// Create full path for BugReport.exe
				TCHAR szSharkPath[_MAX_PATH] = {0};
				TCHAR szDumpPath_no_time[_MAX_PATH] = {0};
				GetModuleFileName(NULL, szSharkPath, wcslen(szSharkPath));
				LPTSTR pszFileName = _tcsrchr(szSharkPath, _T('\\'));
				if (pszFileName) {
					pszFileName++;
					*pszFileName = _T('\0');
				}

				TCHAR szCrashReport[MAX_PATH] = {0};
				_tcsncat(szCrashReport,szSharkPath,wcslen(szCrashReport) - 1);
				_tcsncat(szCrashReport,_T("BugReport.exe"),wcslen(szCrashReport) - 1);

				// Create full path for DUMP file
				TCHAR szDumpPath[MAX_PATH]={0};
				SHGetFolderPath(NULL, CSIDL_APPDATA,NULL,SHGFP_TYPE_CURRENT,szDumpPath);
				_tcsncat(szDumpPath,_T("\\XiaTing\\"),wcslen(szDumpPath) - 1);

				// Replace spaces and dots in file name.
				TCHAR szBaseName[_MAX_PATH] = {0};
				_tcsncat(szBaseName, m_szAppName, wcslen(szBaseName) - 1);
				LPTSTR psz = szBaseName;
				while (*psz != _T('\0')) {
					if (*psz == _T('.'))
						*psz = _T('-');
					else if (*psz == _T(' '))
						*psz = _T('_');
					psz++;
				}
				_tcsncat(szDumpPath, szBaseName, wcslen(szDumpPath) - 1);
				_tcsncat(szDumpPath_no_time, szBaseName, wcslen(szDumpPath_no_time) - 1);				
				
				if (!theCrashDumper.mb_user_mode)
				{
					time_t n_now = time(NULL);
					struct tm* p_time = localtime(&n_now);
					if (p_time != NULL)
					{
						TCHAR s_format[256];
						s_format[0] = 0x0;
						_tcsftime(s_format, 255, _T("-%Y.%m.%d - %H-%M-%S"), p_time);
						_tcsncat(szDumpPath, s_format, _tcslen(s_format) -1);
					}
				}                
				_tcsncat(szDumpPath, _T(".dmp"), wcslen(szDumpPath) - 1);
				_tcsncat(szDumpPath_no_time, _T(".dmp"), wcslen(szDumpPath_no_time) - 1);

               
				HANDLE hFile = CreateFile(szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					_MINIDUMP_EXCEPTION_INFORMATION ExInfo = {0};
					ExInfo.ThreadId = GetCurrentThreadId();
					ExInfo.ExceptionPointers = pExceptionInfo;
					ExInfo.ClientPointers = NULL;

					BOOL bOK = (*pfnMiniDumpWriteDump)(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
					if (bOK)
					{
						
						_sntprintf(szResult, wcslen(szResult), _T("Saved dump file to \"%s\".\r\n\r\nPlease send this file together with a detailed bug report to dumps@emule-project.net !\r\n\r\nThank you for helping to improve eMule."), szDumpPath);
						lRetValue = EXCEPTION_EXECUTE_HANDLER;
						
						CString cs_cmd_line;
						cs_cmd_line.Format(_T("%s%s"),_T("Cmd|"),szDumpPath_no_time);						
						
						hInstCrashReporter = ShellExecuteW(NULL, _T("open"), szCrashReport, cs_cmd_line.GetBuffer(), NULL, SW_SHOW);
						if (hInstCrashReporter <= (HINSTANCE)32)
							lRetValue = EXCEPTION_CONTINUE_SEARCH;
						
					}
					else
					{						
						_sntprintf(szResult, wcslen(szResult), _T("Failed to save dump file to \"%s\".\r\n\r\nError: %u"), szDumpPath, GetLastError());
					}
					CloseHandle(hFile);
				}
				else
				{					
					_sntprintf(szResult, wcslen(szResult), _T("Failed to create dump file \"%s\".\r\n\r\nError: %u"), szDumpPath, GetLastError());
				}
			}
		}
		FreeLibrary(hDll);
		hDll = NULL;
		pfnMiniDumpWriteDump = NULL;
	}

#ifndef _DEBUG
	if (EXCEPTION_EXECUTE_HANDLER == lRetValue)		
	{
		exit(0);
	}
	else
		return lRetValue;

#else

	return lRetValue;
#endif
}
