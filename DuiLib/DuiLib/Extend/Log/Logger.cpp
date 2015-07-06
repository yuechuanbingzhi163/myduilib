#include "stdafx.h"
#include "Logger.h"
#include <time.h>
#include <direct.h>

namespace DuiLib
{
	Logger theDuiLogger("./", "duilib.log", Log_Level::LogLevelAll);
    //默认构造函数
    Logger::Logger()
    {
        //初始化
		m_bEnable = true;
        memset(m_strLogPath, 0, MAX_STR_LEN);
        memset(m_strCurLogName, 0, MAX_STR_LEN);
        m_pFileStream = NULL;
        //设置默认的写日志级别
        m_nLogLevel = Log_Level::LogLevelNormal;
        //初始化临界区变量
        InitializeCriticalSection(&m_cs);
        //创建日志文件名
        GenerateLogName();
		LogBegin();
    }
     
    //构造函数
	Logger::Logger(const char * strLogPath, const char * strLogName, Log_Level nLogLevel) :m_nLogLevel(nLogLevel)
    {
        //初始化
		m_bEnable = true;
        m_pFileStream = NULL;
        strcpy(m_strLogPath, strLogPath);
		if (strLogName) strcpy(m_strCurLogName, strLogName);
        //初始化临界区变量
        InitializeCriticalSection(&m_cs);
         //创建日志目录
        CreateLogPath();
         //创建日志文件名
        GenerateLogName();
		LogBegin();
    }

    //析构函数
    Logger::~Logger()
    {
		LogEnd();
        //释放临界区
        DeleteCriticalSection(&m_cs);
        //关闭文件流
        if(m_pFileStream)
            fclose(m_pFileStream);
		m_pFileStream = NULL;
		m_bEnable = false;
    }

	void Logger::LogBegin()
	{
		time_t curTime;
		struct tm * pTimeInfo = NULL;
		time(&curTime);
		pTimeInfo = localtime(&curTime);
		char pTemp[MAX_STR_LEN] = { 0 };
		sprintf_s(pTemp, "---------------------[DuiLog Begin %4d-%02d-%02d]---------------------", pTimeInfo->tm_year+1900, pTimeInfo->tm_mon + 1, pTimeInfo->tm_mday);
		Trace(pTemp);
	}

	void Logger::LogEnd()
	{
		Trace("---------------------[DuiLog End]---------------------\n");
	}
     
    //写关键信息接口
    void Logger::TraceKeyInfo(const char * strInfo, ...)
    {
        if(!strInfo)
            return;
        char pTemp[MAX_STR_LEN] = {0};
        strcpy(pTemp, GetCurrentTime());
        strcat(pTemp, KEYINFOPREFIX);
        //获取可变形参
        va_list arg_ptr = NULL;
        va_start(arg_ptr, strInfo);
        vsprintf(pTemp + strlen(pTemp), strInfo, arg_ptr);
        va_end(arg_ptr);
        //写日志文件
        Trace(pTemp);
        arg_ptr = NULL;
     
    }

    //写错误信息
    void Logger::TraceError(const char* strInfo, ...)
    {
        //判断当前的写日志级别，若设置为不写日志则函数返回
        if(m_nLogLevel >= Log_Level::LogLevelStop)
            return;
        if(!strInfo)
            return;
        char pTemp[MAX_STR_LEN] = {0};
        strcpy(pTemp, GetCurrentTime());
        strcat(pTemp, ERRORPREFIX);
        va_list arg_ptr = NULL;
        va_start(arg_ptr, strInfo);
        vsprintf(pTemp + strlen(pTemp), strInfo, arg_ptr);
        va_end(arg_ptr);
        Trace(pTemp);
        arg_ptr = NULL;
    }
     
    //写警告信息
    void Logger::TraceWarning(const char * strInfo, ...)
    {
        //判断当前的写日志级别，若设置为只写错误信息则函数返回
        if(m_nLogLevel >= Log_Level::LogLevelNormal)
            return;
        if(!strInfo)
            return;
        char pTemp[MAX_STR_LEN] = {0};
        strcpy(pTemp, GetCurrentTime());
        strcat(pTemp, WARNINGPREFIX);
        va_list arg_ptr = NULL;
        va_start(arg_ptr, strInfo);
        vsprintf(pTemp + strlen(pTemp), strInfo, arg_ptr);
        va_end(arg_ptr);
        Trace(pTemp);
        arg_ptr = NULL;
    }
     
    //写一般信息
    void Logger::TraceInfo(const char * strInfo, ...)
    {
        //判断当前的写日志级别，若设置只写错误和警告信息则函数返回
        if(m_nLogLevel >= Log_Level::LogLevelMid)
            return;
        if(!strInfo)
            return;
        char pTemp[MAX_STR_LEN] = {0};
        strcpy(pTemp, GetCurrentTime());
        strcat(pTemp,INFOPREFIX);
        va_list arg_ptr = NULL;
        va_start(arg_ptr, strInfo);
        vsprintf(pTemp + strlen(pTemp), strInfo, arg_ptr);
        va_end(arg_ptr);
        Trace(pTemp);
        arg_ptr = NULL;
    }
     
    //获取系统当前时间
    char * Logger::GetCurrentTime()
    {
        time_t curTime;
        struct tm * pTimeInfo = NULL;
        time(&curTime);
        pTimeInfo = localtime(&curTime);
        char temp[MAX_STR_LEN] = {0};
        sprintf(temp, "[%02d:%02d:%02d]", pTimeInfo->tm_hour, pTimeInfo->tm_min, pTimeInfo->tm_sec);
        char * pTemp = temp;
        return pTemp;   
    }
    //设置写日志级别
    void Logger::SetLogLevel(Log_Level nLevel)
    {
        m_nLogLevel = nLevel;
    }

	//
	void Logger::SetLogEnable(bool bEnable /* = true */)
	{
		m_bEnable = bEnable;
	}
     
    //写文件操作
    void Logger::Trace(const char * strInfo)
    {
        if(!strInfo)
            return;
        try
        {
            //进入临界区
            EnterCriticalSection(&m_cs);
            //若文件流没有打开，则重新打开
            if(!m_pFileStream)
            {
                char temp[1024] = {0};
                strcat(temp, m_strLogPath);
                strcat(temp, m_strCurLogName);
                m_pFileStream = fopen(temp, "a+");
                if(!m_pFileStream)
                {
                    return;
                }
            }
            //写日志信息到文件流
            fprintf(m_pFileStream, "%s\n", strInfo);
            fflush(m_pFileStream);
            //离开临界区
            LeaveCriticalSection(&m_cs);
        }
        //若发生异常，则先离开临界区，防止死锁
        catch(...)
        {
            LeaveCriticalSection(&m_cs);
        }
    }
    //创建日志文件的名称
    void Logger::GenerateLogName()
    {
		if (strlen(m_strCurLogName) > 0)
		{
			if (m_pFileStream)
				fclose(m_pFileStream);
			char temp[1024] = { 0 };
			strcat(temp, m_strLogPath);
			strcat(temp, m_strCurLogName);
			//以追加的方式打开文件流
			m_pFileStream = fopen(temp, "w");
			return;
		}
        time_t curTime;
        struct tm * pTimeInfo = NULL;
        time(&curTime);
        pTimeInfo = localtime(&curTime);
        char temp[1024] = {0};
        //日志的名称如：2013-01-01.log
        sprintf(temp, "%04d-%02d-%02d.log", pTimeInfo->tm_year+1900, pTimeInfo->tm_mon + 1, pTimeInfo->tm_mday);
        if(0 != strcmp(m_strCurLogName, temp))
        {
            strcpy(m_strCurLogName,temp);
            if(m_pFileStream)
                fclose(m_pFileStream);
            char temp[1024] = {0};
            strcat(temp, m_strLogPath);
            strcat(temp, m_strCurLogName);
            //以追加的方式打开文件流
            m_pFileStream = fopen(temp, "a+");
        }    
    }
     
    //创建日志文件的路径
    void Logger::CreateLogPath()
    {
		if (strlen(m_strLogPath) == 0)
		{
			strcpy(m_strLogPath, "./");
		}
		if (m_strLogPath[strlen(m_strLogPath) - 1] != '/')
		{
			strcat(m_strLogPath, "/");
		}
		char* p = m_strLogPath;
		while (*p!='\0')
		{
			if (*p == '\\')
				*p = '/';
			++p;
		}
		WIN32_FIND_DATAA wfd;
		memset(&wfd, 0x0, sizeof(wfd));
		
		HANDLE hFind = FindFirstFileA(p, &wfd);
		if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			FindClose(hFind);
			return;
		}
		if (hFind != INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);
		}

		mkdir(m_strLogPath);
    }
	/////////////////////////////////////////////////////////////////////////////////
	////
	void LogHelper::SetDuiLogLevel(Log_Level nLogLevel)
	{
		theDuiLogger.SetLogLevel(nLogLevel);
	}

	void LogHelper::EnableDuiLog(bool bEnable /* = true */)
	{
		theDuiLogger.SetLogEnable(bEnable);
	}

	void LogHelper::WriteLog(Log_Level nLogLevel, const char* logStr)
	{

	}
}