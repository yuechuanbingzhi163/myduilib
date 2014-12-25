#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#endif


#include "YTingRuntime.h"


/*******************************************************************************/
class CNullLock {
public:
	CNullLock(){}
	~CNullLock(){}
	void Lock(){}
	void Unlock(){}
};
class CLock {
public:
    CLock();
    ~CLock();
    void Lock();
    void Unlock();
protected:
#ifdef _WIN32
    CRITICAL_SECTION cs;
#else
    pthread_mutex_t m;
#endif
};


#ifdef _WIN32

inline CLock::CLock()
{
    InitializeCriticalSection(&cs);
}

inline CLock::~CLock()
{
    DeleteCriticalSection(&cs);
}

inline void CLock::Lock()
{
	EnterCriticalSection(&cs);
}

inline void CLock::Unlock()
{
	LeaveCriticalSection(&cs);
}
#else
inline CLock::CLock()
{
    pthread_mutexattr_t   mta;
    pthread_mutexattr_init(&mta);
    pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m,&mta);
}

inline CLock::~CLock()
{
	pthread_mutex_destroy(&m);
}

inline void CLock::Lock()
{
	pthread_mutex_lock(&m);
}

inline void CLock::Unlock()
{
	pthread_mutex_unlock(&m);
}
#endif

class CLockObj {
public:
    explicit CLockObj(CLock * plock):_plock(plock)
    {
        assert(_plock);
        _plock->Lock();
    }
    ~CLockObj() 
    {
        _plock->Unlock();
    }
protected:
    CLock * _plock;
};
/******************************************************************************/
