#include "stdafx.h"
#include "log.h"
#include <algorithm>
#include <cassert>


/*-------------------------------*/
namespace em_utility{

//CLog::lock_type CLog::_lock;


CLog::CLog(const char_type* filename,Log_Level lev /* = InfoLog */,Log_Type type /* = DAY */,size_t maxSaveLogCnt /* = 3 */)
	:_pfile(NULL),_level(lev),_type(type),_maxSaveCnt(maxSaveLogCnt)
{
	assert(filename);
	string_type s(filename);
	std::replace(s.begin(),s.end(),'\\','/');
	string_type::size_type index = s.find_last_of('/');
	if(std::string::npos != index)
	{
		_dir  = s.substr(0,index+1);
		_name = s.substr(index+1);

		assert(!_dir.empty());
		assert(!_name.empty());
	}

	
	if(!em_utility::mci::is_dir_exist(_dir.c_str()))
	{
		if(!em_utility::mci::confirm_dir(_dir.c_str(),'/'))
		{
//			assert(false);
		}
	}


	memset(_timename,0,sizeof(_timename));

	std::locale::global( std::locale("chs"));

	(*this)(SysLog,"LogBegin");
}


CLog::~CLog()
{
	(*this)(SysLog,"LogEnd");
	_Close();
}

bool CLog::_Open()
{
	if(_IsOpen())
		return true;

	this->_Clear();

	string_type filename(_dir);
	filename += _timename;
	filename += _name;

	_pfile = fopen(filename.c_str(),"a");
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
	const static  char_type sformat[7][24] = 
	{"%Y","%Y%m","%Y%m%d","%Y%m%d_%H","%Y%m%d_%H_%M","%Y%m%d_%H_%M_%S"};

	char_type buffer[24] = {0};	
	strftime(buffer,24,sformat[_type],atm);

	if(strcmp(buffer,_timename) != 0)
	{
		strcpy(_timename,buffer);
		_Close();
		_Open();
	}
}

 unsigned long CLog::_GetCurrentThreadID() const
{
#ifdef _WIN32
	return ::GetCurrentThreadId();
#else
	return (unsigned long)syscall(SYS_thread_selfid);
#endif
}

 void CLog::_ScanAllFiles(const string_type& rootPath,std::vector<string_type>& container,bool isRecursive /* = true */)
{

}
 void CLog::_Clear()
{

}
 void CLog::_OutTitle(Log_Level &lev)
{
	const static char_type* pLevel[] ={"Sys","Info","Warn","Error"};

	time_t t = time(NULL);
	tm* aTm = localtime(&t);

	_NewLog(aTm);  

	//       YYYY   year
	//       MM     month (2 digits 01-12)
	//       DD     day (2 digits 01-31)
	//       HH     hour (2 digits 00-23)
	//       MM     minutes (2 digits 00-59)
	//       SS     seconds (2 digits 00-59)
	if(!_IsOpen()) return;

	fprintf(_pfile,"\n[%-4d-%02d-%02d %02d:%02d:%02d][%s][%lX]| ",
		aTm->tm_year+1900,
		aTm->tm_mon+1,
		aTm->tm_mday,
		aTm->tm_hour,
		aTm->tm_min,
		aTm->tm_sec,
		pLevel[lev],
		_GetCurrentThreadID());
#ifdef _CONSOLE
	fprintf(stderr,"\n[%-4d-%02d-%02d %02d:%02d:%02d][%s][%lX]| ",
		aTm->tm_year+1900,
		aTm->tm_mon+1,
		aTm->tm_mday,
		aTm->tm_hour,
		aTm->tm_min,
		aTm->tm_sec,
		pLevel[lev],
		_GetCurrentThreadID());
#endif

#ifdef _VSOUTPUT
	char buff[1024] = {0};
	sprintf_s(buff,"\n[%-4d-%02d-%02d %02d:%02d:%02d][%s][%lX]| ",
		aTm->tm_year+1900,
		aTm->tm_mon+1,
		aTm->tm_mday,
		aTm->tm_hour,
		aTm->tm_min,
		aTm->tm_sec,
		pLevel[lev],
		_GetCurrentThreadID())
	::OutputDebugStringA(buff);
#endif
}


template<>
void CLog::_Vfprintf(const char_type* fmt,va_list& va)
{
#ifdef _CONSOLE
#ifdef _WIN32
	vfprintf(stderr,fmt,va); //only win
#else
	va_list newva;
	va_copy(newva,va);
	vfprintf(stderr,fmt,newva);
	va_end(newva);
#endif
#endif

	vfprintf(_pfile,fmt,va);
	fflush(_pfile);
}

template<>
void CLog::_Vfprintf(const wchar_type* fmt,va_list& va)
{
#ifdef _CONSOLE
#ifdef _WIN32
	vfwprintf(stderr,fmt,va); //only win
#else
	va_list newva;
	va_copy(newva,va);
	vfwprintf(stderr,fmt,newva);
	va_end(newva);
#endif
#endif

	vfwprintf(_pfile,fmt,va);
	fflush(_pfile);
}


template<typename charT>
CLog& CLog::operator()(const charT* fmt,...)
{
	va_list va;
	va_start(va,fmt);
	(*this)(InfoLog,fmt,va);
	va_end(va);

	return *this;
}

template<typename charT>
CLog& CLog::operator()(Log_Level lev,const charT* fmt,...)
{
	va_list va;
	va_start(va,fmt);
	(*this)(lev,fmt,va);
	va_end(va);

	return *this;
}

template<typename charT>
CLog& CLog::operator()(Log_Level lev,const charT* fmt,va_list& va)
{
	assert(fmt);
	if(!fmt)
		return *this;

	if(lev < _level && SysLog != lev)
		return *this;

	CResGuard::CGuard gd(_lock);
	{
		_OutTitle(lev);
		if(_IsOpen())
			this->_Vfprintf<charT>(fmt,va);
	}

	return *this;
}
};
