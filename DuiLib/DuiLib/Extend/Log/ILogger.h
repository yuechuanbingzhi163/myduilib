#ifndef ILogger_H
#define ILogger_H
#pragma once

namespace DuiLib
{
	//日志级别的提示信息
	static const char * KEYINFOPREFIX   = " Key: \n\t\t";
	static const char * ERRORPREFIX = " Error: \n\t\t";
	static const char * WARNINGPREFIX   = " Warning: \n\t\t";
	static const char * INFOPREFIX      = " Info: \n\t\t";

	static const int MAX_STR_LEN = 1024;
	//日志级别枚举
	typedef enum Log_Level
	{
	    LogLevelAll = 0,    //所有信息都写日志
	    LogLevelMid,        //写错误、警告信息
	    LogLevelNormal,     //只写错误信息
	    LogLevelStop        //不写日志
	};

	class UILIB_API ILogger
	{
	public:
		
		//写关键信息
		virtual void TraceKeyInfo(const char * strInfo, ...) = 0;
		//写错误信息
		virtual void TraceError(const char* strInfo, ...) = 0;
		//写警告信息
		virtual void TraceWarning(const char * strInfo, ...) = 0;
		//写一般信息
		virtual void TraceInfo(const char * strInfo, ...) = 0;
		//设置写日志级别
		virtual void SetLogLevel(Log_Level nLevel) = 0;
	public:
		
	};

}


#endif//ILogger_H