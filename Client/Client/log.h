#ifndef _LOG_H
#define _LOG_H

#pragma once

#include "Lock.h"
class CLog 
{
public:
	typedef CLock               clock;
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
public:
	CLog(const TCHAR* filename,const TCHAR* dirname,Log_Level lev = InfoLog,Log_Type type = DAY);
	~CLog();
public:
	CLog& operator() (Log_Level lve,const TCHAR* str,...);
	CLog& operator() (const TCHAR* str,...);
protected:
	CLog(const CLog& logobj);
	CLog& operator = (const CLog& logobj);
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

	TCHAR                _dirname[512];
	TCHAR                _filename[512];
	TCHAR                _timename[512];
};

extern CLog* theLog;

#define  aLog                (*theLog)
#ifdef _UNICODE
	#define  write_log(lev,fmt,...)  (aLog(lev,fmt,##__VA_ARGS__))
#else
	#define  write_log(lev,fmt,...)  (aLog(lev,"[%s][%d]"fmt,__FILE__,__LINE__,##__VA_ARGS__))
#endif
#define  sLog(fmt,...)         (write_log(CLog::InfoLog,fmt,##__VA_ARGS__))
#define  sLogError(fmt,...)    (write_log(CLog::ErrorLog,fmt,##__VA_ARGS__))
#define  sLogWarn(fmt,...)     (write_log(CLog::WarningLog,fmt,##__VA_ARGS__))

#endif//_LOG_H