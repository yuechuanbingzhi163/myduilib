#include "stdafx.h"
#include "log.h"


CLog* theLog = NULL;
CLog::CLog(const TCHAR* filename,const TCHAR* dirname,Log_Level lev /* = InfoLog */,Log_Type type /* = DAY */)
	:_pfile(NULL),_level(lev),_type(type)
{

	memset(_filename,0,sizeof(_filename));
	memset(_timename,0,sizeof(_timename));
	memset(_dirname,0,sizeof(_dirname));
	wcscpy_s(_filename,512,filename);
	wcscpy_s(_dirname,512,dirname);

	setlocale(LC_ALL,"chs");

	(*this)(InfoLog,_T("LogBegin"));
}
CLog::~CLog()
{
	(*this)(InfoLog,_T("LogEnd"));
	_Close();
}
bool CLog::_Open()
{
	if(_IsOpen())
		return true;

	std::wstring stmp(_timename);
	stmp += _filename;
	std::wstring sfile(_dirname);

	sfile += stmp;
	int er;
	errno_t err = _wfopen_s(&_pfile,sfile.c_str(),_T("a"));
	if (_pfile == 0)
	{
		er = GetLastError();
	}
	if(!_pfile)
	{
		std::exception e("open logfile error");
		throw e;
	}

	return _IsOpen();
}
void CLog::_Close()
{
	if(_IsOpen())
	{	
		fclose(_pfile);		
		_pfile = NULL;
	}	
}
void CLog::_NewLog(tm* atm)
{
	static  TCHAR sformat[7][24] = {_T("%Y"),_T("%Y%m"),_T("%Y%m%d"),_T("%Y%m%d_%H"),
		_T("%Y%m%d_%H_%M"),_T("%Y%m%d_%H_%M_%S")};

	TCHAR buffer[24] = {0};	
	wcsftime(buffer,24,sformat[_type],atm);

	if(wcscmp(buffer,_timename) != 0)
	{
		wcscpy_s(_timename,512,buffer);
		_Close();
		_Open();
	}
}
void CLog::_OutTitle(Log_Level &lev)
{
	static TCHAR* pLevel[] ={_T("Info"),_T("Warn"),_T("Error")};

	time_t t = time(NULL);
//	tm* aTm = localtime(&t);
	tm* aTm  = new tm;
	localtime_s(aTm,&t);
	_NewLog(aTm);  

	//       YYYY   year
	//       MM     month (2 digits 01-12)
	//       DD     day (2 digits 01-31)
	//       HH     hour (2 digits 00-23)
	//       MM     minutes (2 digits 00-59)
	//       SS     seconds (2 digits 00-59)
	if(_IsOpen())
		fwprintf(_pfile,_T("\n[%-4d-%02d-%02d %02d:%02d:%02d][%s][%x]| "),aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec,
		pLevel[lev],GetCurrentThreadId());
#ifdef _CONSOLE
	fwprintf(stderr,_T("\n[%-4d-%02d-%02d %02d:%02d:%02d][%s][%x]| "),aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec,
		pLevel[lev],GetCurrentThreadId());
#endif
}
CLog& CLog::operator ()(Log_Level lve,const TCHAR* str,...)
{
	if(!str)
		return *this;

	if(lve < _level)
		return *this;


	_lock.Lock();
	{
		// 		try
		// 		{
		_OutTitle(lve);

		if(_IsOpen())
		{
			va_list va;	
			va_start(va,str);
			vfwprintf(_pfile,str,va);
			//Rzfprintf(_pfile,_T("\n"));
#ifdef _CONSOLE
			vfwprintf(stderr,str,va);
#endif
			va_end(va);
			fflush(_pfile);	
		}
		// 		}
		// 		catch(const std::exception& e)
		// 		{
		// 			Rzcout<<e.what()<<std::endl;
		// 		}
		// 		catch(...)
		// 		{
		// 			Rzcout<<::GetLastError()<<std::endl;
		// 		}
	}
	_lock.UnLock();

	return *this;
}
CLog& CLog::operator ()(const TCHAR* str,...)
{
	if(!str)
		return *this;

	Log_Level lv = InfoLog;

	if(lv < _level)
		return *this;


	_lock.Lock();
	{
		// 		try
		// 		{
		_OutTitle(lv);

		if(_IsOpen())
		{
			va_list va;	
			va_start(va,str);
			vfwprintf(_pfile,str,va);
#ifdef  _CONSOLE
			vfwprintf(stderr,str,va);
#endif
			//Rzfprintf(_pfile,_T("\n"));
			va_end(va);
			fflush(_pfile);	
		}
		//		}
		// 		catch(const std::exception& e)
		// 		{
		// //			cout<<e.what()<<std::endl;
		// 			std::exception e("write log exception");
		// 			throw e;
		// 		}
		// 		catch(...)
		// 		{
		// //			cout<<::GetLastError()<<std::endl;
		// 			std::exception e("write log exception");
		// 			throw e;
		// 		}
	}
	_lock.UnLock();

	return *this;
}