/**
* Copyright(c) 2013 XX-lib shaoyuan
* All rights reserved
* @file: thread.cpp
* @datetime£º2013-03-09
*/

#include "..\..\src\Util\thread.h"
using namespace XXLIB;
using namespace XXLIB::XNThread;

XCThread::XCThread():ulThread_(NULL), strThreadName_(""), target_(NULL) 
{
	priority_ = PRIORITY_NORMAL;
}

XCThread::XCThread(XCRunnable* target): target_(target), strThreadName_(""), ulThread_(NULL)
{
	priority_ = PRIORITY_NORMAL;
}

XCThread::~XCThread()
{
	destroy();
}

void XCThread::setId(unsigned long id)
{
	ulThreadId_ = id;
}

unsigned long XCThread::getId()
{
	return ulThreadId_;
}

void XCThread::setName(const std::string& strName)
{
	strThreadName_ = strName;
}

std::string XCThread::getName()
{
	return strThreadName_;
}

Priority XCThread::setPriority(Priority newPriority)
{
	Priority oldPriority = priority_;

	if (ulThread_ != NULL && ::SetThreadPriority((HANDLE)ulThread_, (int)newPriority))
		priority_ = newPriority;
	else
		priority_ = newPriority;
	return oldPriority;
}

Priority XCThread::getPriority()const
{
	return priority_;
}

bool XCThread::start(XCRunnable* target /* = NULL */, const std::string& name /* = _T */)
{
	if (target != NULL)
		target_ = target;

	if (!name.empty())
		strThreadName_ = name;
	destroy();
	try
	{
		ulThread_ = _beginthreadex(NULL, 0, (pthread_address)threadFunc, (void* )this, CREATE_SUSPENDED, (unsigned* )&ulThreadId_);
		if (ulThread_ == NULL) 
		{
			throw false;
		}
		::SetThreadPriority((HANDLE)ulThread_, priority_);
		::ResumeThread((HANDLE)ulThread_);
	}
	catch (bool)
	{
		destroy();
		return false;
	}
	return true;
}

void XCThread::destroy()
{
	if (ulThread_ != NULL)
	{
		_endthreadex(ulThread_);
	}
}

bool XCThread::isAlive() const 
{
	return (ulThread_ != NULL) ? (::WaitForSingleObject((HANDLE)ulThread_, 0) != WAIT_OBJECT_0) : false;
}

void XCThread::join(unsigned long millis, int nanos) 
{
	if (ulThread_ != NULL) 
	{
		::WaitForSingleObject((HANDLE)ulThread_, millis);
	}
}

unsigned long XCThread::run() 
{
	if (target_ != NULL) 
	{
		return target_->run();
	}
	return -1;
}

unsigned long __stdcall XCThread::threadFunc(void* pArguments) 
{
	XCThread* pthread = static_cast<XCThread* >(pArguments);
	if (pthread == NULL)
	{
		return -1;
	}
	unsigned long thread_handle = pthread->run();
	_endthreadex(thread_handle);
	return thread_handle;
}

bool XCThread::waitForThread()
{
	if(::WaitForSingleObject((HANDLE)ulThread_,INFINITE) == WAIT_OBJECT_0 )
		return true;
	return false;
}
bool XCThread::waitForThread(unsigned long time_out)
{
	if(::WaitForSingleObject((HANDLE)ulThread_,time_out) == WAIT_OBJECT_0)
		return true;
	return false;
}
