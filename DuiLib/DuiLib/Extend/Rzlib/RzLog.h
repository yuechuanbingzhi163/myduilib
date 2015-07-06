#ifndef __RZLOG_H__
#define __RZLOG_H__
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <locale>
#include "RzType.h"
#include "RzLock.h"

using namespace std;

_RzCFunBegin
_RzStdBegin

class RZ_DLL_API CRzLog
{
public:
	typedef CRzLog                self_type;
	typedef TCHAR                 char_type;
	typedef CRzLock               clock;

public:
	enum Log_Level
	{
		InfoLog = 0,
		WarningLog = 1,
		ErrorLog = 2
	};
	enum Log_Type
	{
		YEAR = 0,
		MONTH = 1,
		DAY = 2,
		HOUR = 3,
		MIN = 4,
		SECOND = 5
	};	

	CRzLog(const char_type* filename,Log_Level lev = InfoLog,Log_Type type = DAY);
	~CRzLog();

	self_type& operator()(Log_Level lve,const char_type* str,...);
	self_type& operator()(const char_type* str,...);

protected:
	CRzLog(const self_type& ){};
	self_type& operator = (const self_type&){ return *this; };
	
private:
	bool                     _Open();
	void                     _Close();
	bool                     _IsOpen() {return NULL != _pfile;};
	void                     _OutTitle(Log_Level &lev);
	void                     _NewLog(tm* atm);
private:
	Log_Level                _level;
	Log_Type                 _type;
	FILE*                    _pfile;
	clock                    _lock;

	char_type                _filename[512];
	char_type                _timename[512];
};

/*-------------------------------*/
inline CRzLog::CRzLog(const char_type* filename,Log_Level lev /* = InfoLog */,Log_Type type /* = DAY */)
:_pfile(NULL),_level(lev),_type(type)
{
	assert(filename);
	
	memset(_filename,0,sizeof(_filename));
	memset(_timename,0,sizeof(_timename));
	Rzstrcpy(_filename,filename);

	std::locale::global( std::locale("chs"));

	(*this)(InfoLog,_T("LogBegin"));
}
inline CRzLog::~CRzLog()
{
	(*this)(InfoLog,_T("LogEnd"));
	_Close();
}

inline bool CRzLog::_Open()
{
	if(_IsOpen())
		return true;

	Rzstring stmp(_timename);
	stmp += _filename;

	_pfile = Rzfopen(stmp.c_str(),_T("a"));

	if(!_pfile)
	{
        std::exception e("open logfile error");
		throw e;
	}

	
	return _IsOpen();
}

inline void CRzLog::_Close()
{
	if(_IsOpen())
	{	
		fclose(_pfile);		
		_pfile = NULL;
	}	
}

inline void CRzLog::_NewLog(tm* atm)
{
	static  char_type sformat[7][24] = {_T("%Y"),_T("%Y%m"),_T("%Y%m%d"),_T("%Y%m%d_%H"),
		                                     _T("%Y%m%d_%H_%M"),_T("%Y%m%d_%H_%M_%S")};

 	char_type buffer[24] = {0};	
 	Rzstrftime(buffer,24,sformat[_type],atm);

	if(Rzstrcmp(buffer,_timename) != 0)
	{
		Rzstrcpy(_timename,buffer);
		_Close();
		_Open();
	}
}

inline void CRzLog::_OutTitle(Log_Level &lev)
{
	static char_type* pLevel[] ={_T("Info"),_T("Warn"),_T("Error")};

	time_t t = time(NULL);
	tm* aTm = localtime(&t);

	_NewLog(aTm);  

	//       YYYY   year
	//       MM     month (2 digits 01-12)
	//       DD     day (2 digits 01-31)
	//       HH     hour (2 digits 00-23)
	//       MM     minutes (2 digits 00-59)
	//       SS     seconds (2 digits 00-59)
	if(_IsOpen())
		Rzfprintf(_pfile,_T("\n[%-4d-%02d-%02d %02d:%02d:%02d][%s][%x]| "),aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec,
		     pLevel[lev],GetCurrentThreadId());
#ifdef _CONSOLE
	Rzfprintf(stderr,_T("\n[%-4d-%02d-%02d %02d:%02d:%02d][%s][%x]| "),aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec,
		pLevel[lev],GetCurrentThreadId());

#endif
}

inline CRzLog& CRzLog::operator()(Log_Level lve,const char_type* str,...)
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
				Rzvfprintf(_pfile,str,va);
				//Rzfprintf(_pfile,_T("\n"));
#ifdef _CONSOLE
				Rzvfprintf(stderr,str,va);
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

inline CRzLog& CRzLog::operator()(const char_type* str,...)
{
	if(!str)
		return *this;

	Log_Level lv = InfoLog;

	if(lv < _level)
		return *this;


	_lock.Lock();
	{
		try
		{
			_OutTitle(lv);

			if(_IsOpen())
			{
				va_list va;	
				va_start(va,str);
				Rzvfprintf(_pfile,str,va);
#ifdef  _CONSOLE
				Rzvfprintf(stderr,str,va);
#endif
				//Rzfprintf(_pfile,_T("\n"));
				va_end(va);
				fflush(_pfile);	
			}
		}
		catch(const std::exception& e)
		{
			Rzcout<<e.what()<<std::endl;
		}
		catch(...)
		{
			Rzcout<<::GetLastError()<<std::endl;
		}
	}
	_lock.UnLock();

	return *this;
}


_RzStdEnd
_RzCFunEnd

#endif