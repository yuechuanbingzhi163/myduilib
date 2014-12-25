#ifndef _LOG_H
#define _LOG_H

/****************************************************************************
* @FileName : log.h
* @Email    : libyyu@qq.com
* @Author   : FnLove2010
* @Comment  : This is tool for writing log ;
*****************************************************************************/


#pragma once
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <locale>


#ifdef _WIN32
	#include <windows.h>
#else
	#include<pthread.h>
	#include <sys/syscall.h>
#endif
#include "../ResGuard.h"

namespace em_utility{

class CLog
{
public:
	typedef CLog                 self_type;
	typedef char                 char_type;
	typedef wchar_t              wchar_type;
	typedef CResGuard            lock_type;
	typedef std::string          string_type;

public:
	enum Log_Level
	{
		SysLog = 0,
		InfoLog = 1,
		WarningLog = 2,
		ErrorLog = 3
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

	CLog(const char_type* filename,Log_Level lev = InfoLog,Log_Type type = DAY,size_t maxSaveLogCnt = 3);
	virtual ~CLog();

	template<typename charT>
	self_type& operator()(const charT* fmt,...);

	template<typename charT>
	self_type& operator()(Log_Level lev,const charT* fmt,...);

	template<typename charT>
	self_type& operator()(Log_Level lev,const charT* fmt,va_list& va);

	bool isValid()const {return _IsOpen();}

protected:
	CLog(const self_type& ){}
	self_type& operator = (const self_type&){return *this;}

	template<typename charT>
	void _Vfprintf(const charT* fmt,va_list& va);
private:
	bool                     _Open();
	void                     _Close();
	bool                     _IsOpen()const {return NULL != _pfile;};
	void                     _OutTitle(Log_Level &lev);
	void                     _NewLog(tm* atm);
	unsigned long            _GetCurrentThreadID() const;
	void                     _ScanAllFiles(const string_type& rootPath,std::vector<string_type>& container,bool isRecursive = true);
	void                     _Clear();
private:
	Log_Level                _level;
	Log_Type                 _type;
	/*static*/ lock_type         _lock;
	FILE*                    _pfile;
	size_t                   _maxSaveCnt;

	string_type              _dir;
	string_type              _name;
	char_type                _timename[512];
};
};

#endif//_LOG_H