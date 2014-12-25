#ifndef __RZLOG_H__
#define __RZLOG_H__
#pragma once

#include <time.h>
// #include <stdio.h>
// #include <stdlib.h>
#include <string>
// #include <locale.h>
// #include <iostream>
using namespace std;
#include "../LrcMgr/RzLock.h"


class CRzLog
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
	self_type& operator = (const self_type&){};
	
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





#endif