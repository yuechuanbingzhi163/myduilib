#ifndef _Mutex_h
#define _Mutex_h


#if defined (WIN32)
	#include <Windows.h>
	#include <process.h>
	#include <assert.h>
#endif

namespace em_utility{
class Mutex 
{
public:
	Mutex()			{ InitializeCriticalSection(&mCriticalSection);	}
	~Mutex()		{ DeleteCriticalSection(&mCriticalSection);		}
	void Lock()		{ EnterCriticalSection(&mCriticalSection);		}
	void Unlock()	{ LeaveCriticalSection(&mCriticalSection);		}
private:
	CRITICAL_SECTION mCriticalSection;	
};

class MutexLock
{
public:
	MutexLock(Mutex& mutex) : mMutex(mutex) { mMutex.Lock();	}
	~MutexLock()							{ mMutex.Unlock();	}
private:
	Mutex& mMutex;
};

#define AUTO_MUTEX_NAME auto_mutex

#define EE_AUTO_MUTEX mutable Mutex AUTO_MUTEX_NAME;
#define EE_LOCK_AUTO_MUTEX MutexLock NameLock(AUTO_MUTEX_NAME);
#define EE_LOCK_MUTEX_NAMED(mutexName, lockName) MutexLock lockName(mutexName);

#define EE_MUTEX(name)	mutable Mutex name;
#define EE_STATIC_MUTEX(name)	static Mutex name;
#define EE_LOCK_MUTEX(mutexName) MutexLock _lockName(mutexName);

}

#endif//_Mutex_h