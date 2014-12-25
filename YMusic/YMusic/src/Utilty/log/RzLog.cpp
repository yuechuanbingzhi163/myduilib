#include "stdafx.h"
#include "RzLog.h"


/*-------------------------------*/
CRzLog::CRzLog(const char_type* filename,Log_Level lev /* = InfoLog */,Log_Type type /* = DAY */)
	:_pfile(NULL),_level(lev),_type(type)
{
	assert(filename);

	memset(_filename,0,sizeof(_filename));
	memset(_timename,0,sizeof(_timename));
	DFn_strcpy_s(_filename,512,filename);

	std::locale::global( std::locale("chs"));

	(*this)(InfoLog,_T("LogBegin"));
}
CRzLog::~CRzLog()
{
	(*this)(InfoLog,_T("LogEnd"));
	_Close();
}

bool CRzLog::_Open()
{
	if(_IsOpen())
		return true;

	std::string stmp(_timename);
	stmp += _filename;

	DFn_fopen_s(&_pfile,stmp.c_str(),_T("a"));
//	_pfile = Rzfopen(stmp.c_str(),_T("a"));

	if(!_pfile)
	{
		std::exception e("open logfile error");
		throw e;
	}


	return _IsOpen();
}

void CRzLog::_Close()
{
	if(_IsOpen())
	{	
		fclose(_pfile);		
		_pfile = NULL;
	}	
}

void CRzLog::_NewLog(tm* atm)
{
	static  char_type sformat[7][24] = {_T("%Y"),_T("%Y%m"),_T("%Y%m%d"),_T("%Y%m%d_%H"),
		_T("%Y%m%d_%H_%M"),_T("%Y%m%d_%H_%M_%S")};

	char_type buffer[24] = {0};	
	DFn_strftime(buffer,24,sformat[_type],atm);

	if(DFn_strcmp(buffer,_timename) != 0)
	{
		DFn_strcpy_s(_timename,512,buffer);
		_Close();
		_Open();
	}
}

void CRzLog::_OutTitle(Log_Level &lev)
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
		DFn_fprintf(_pfile,_T("\n[%-4d-%02d-%02d %02d:%02d:%02d][%s][%x]| "),aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec,
		pLevel[lev],GetCurrentThreadId());
#ifdef _CONSOLE
	   DFn_fprintf(stderr,_T("\n[%-4d-%02d-%02d %02d:%02d:%02d][%s][%x]| "),aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec,
		pLevel[lev],GetCurrentThreadId());
#endif
}

CRzLog& CRzLog::operator()(Log_Level lve,const char_type* str,...)
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
			DFn_vfprintf(_pfile,str,va);
			//Rzfprintf(_pfile,_T("\n"));
#ifdef _CONSOLE
			DFn_vfprintf(stderr,str,va);
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

CRzLog& CRzLog::operator()(const char_type* str,...)
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
				DFn_vfprintf(_pfile,str,va);
#ifdef  _CONSOLE
				DFn_vfprintf(stderr,str,va);
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