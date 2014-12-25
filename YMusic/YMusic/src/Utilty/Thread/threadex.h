/**
* Copyright(c) 2013 XX-lib shaoyuan
* All rights reserved
* @file: thread.h
* @datetime：2013-03-09
*/

/*******************************************************************************
* @brief:线程类
* @author：shaoyuan
* @datetime：2013-03-09
* @version：verson-1.0
*******************************************************************************/
#pragma once
#ifndef _THREAD_H_
#define _THREAD_H_
#pragma warning(disable:4251)
#include "defines.h"
/*
	NOTE:由于导出类中使用了std::string变量，因此会产生C4251的警告
	为了消除此警告，可以采用下面的方式将std::string导出即可,但是由于日志库已经将
	basic_string导出，因此会引起重复定义的错误，所以这里忽略4251的警告
*/
//template class XXLIB_DLLPORT std::allocator<char>; 
//template class XXLIB_DLLPORT std::basic_string<char>;
namespace XXLIB
{
	namespace XNThread
	{
		enum Priority
		{
			PRIORITY_HIGHEST 		= THREAD_PRIORITY_HIGHEST,			
			PRIORITY_ABOVE_NORMAL 	= THREAD_PRIORITY_ABOVE_NORMAL,		
			PRIORITY_NORMAL 		= THREAD_PRIORITY_NORMAL,			
			PRIORITY_BELOW_NORMAL 	= THREAD_PRIORITY_BELOW_NORMAL,		
			PRIORITY_LOWEST 		= THREAD_PRIORITY_LOWEST,			
		};

		//最终线程函数运行的实体
		class  XCRunnable
		{
		public:
			virtual unsigned long run() = 0;
			virtual ~XCRunnable(){}
		};

		class  XCThread : public XCRunnable
		{
			typedef	unsigned (__stdcall *pthread_address)(void* );
		public:

		public:
			XCThread();
			XCThread(XCRunnable* target);
			virtual ~XCThread();
			void setId(unsigned long id);
			unsigned long getId();
			void setName(const std::string& strName);
			std::string getName();
			Priority setPriority(Priority newPriority);
			Priority getPriority() const;
			bool start(XCRunnable* target = NULL, const std::string& name = "");
			virtual void destroy();
			bool isAlive()const;
			void join(unsigned long millis = INFINITE, int nanos = 0);
			virtual unsigned long run();
			bool waitForThread();
			bool waitForThread(unsigned long time_out);
		private:
			static unsigned long __stdcall threadFunc(void* pArguments) ;
		private:
			unsigned long ulThread_;
			XXLIB::XNThread::XCRunnable* target_;
			unsigned long ulThreadId_;
			std::string strThreadName_;
			Priority priority_;
		};
	}
}
#endif