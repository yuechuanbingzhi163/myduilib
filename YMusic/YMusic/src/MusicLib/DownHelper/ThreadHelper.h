#ifndef _Thread_h
#define _Thread_h

#include "Mutex.h"

#include <set>
#include <deque>
using namespace std;
namespace em_utility{
enum EVENT_SCHEDULER
{
	ES_LOW		= 3,
	ES_NORMAL	= 6,
	ES_HIGHT	= 0x0fffffff
};
enum EVENT_LEVEL
{
	E_NORMAL	= 1,
	E_HIGHT		= 2,

	E_MAX_COUNT	= 3
};

class Event
{
	HANDLE mEvent;

public:
	Event(void);
	virtual ~Event(void);
	void Create();
	void Destroy();
	void Trigger(void);
	void Wait();
};
//////////////////////////////////////////////////////////////////////////
class StreamThread;
class ThreadBase;

class ThreadFunction
{
public:
	ThreadFunction();
	virtual ~ThreadFunction();

	void operator()();

	void Run();

public:
	void SetThreadManager(ThreadBase* pThreadManager)	{ m_pThreadManager = pThreadManager;	}
	ThreadBase* GetThreadManager()					{ return m_pThreadManager;				}

protected:
	ThreadBase* m_pThreadManager;
};
// EEThread
class Thread 
{
public:
	Thread();
	virtual ~Thread();

	int Init(ThreadFunction* pThreadFunction);

	void Uninit();

protected:
	void*				m_hThread;
	ThreadFunction*		m_pThreadFunction;
};
//////////////////////////////////////////////////////////////////////////
class ThreadEvent 
{
public:
	ThreadEvent() {}
	virtual ~ThreadEvent(){}

	virtual bool GetRepeatRespondIfFail()	{ return false; }

	virtual bool IsAutoDestroy()			{ return false; }

	//virtual bool IsEqual(ThreadEvent* pThreadEvent) { return false; }

	// 返回该事件的优先级别
	virtual EVENT_LEVEL GetLevel()			{ return E_NORMAL;}
public:
	virtual bool ProcessEvent() = 0;

	virtual bool RespondEvent() = 0;
};

class Request
{
public:
	Request(unsigned int requestID);
	virtual ~Request();

	virtual void SetThreadEvent(ThreadEvent* pThreadEvent);

	virtual ThreadEvent* GetThreadEvent();

	virtual void Abort();

	virtual bool NeedAborted();

	virtual unsigned int GetID();

protected:
	bool				mAborted;
	unsigned int		mRequestID;
	ThreadEvent*		mThreadEvent;
};
//////////////////////////////////////////////////////////////////////////
class ThreadBase
{
public:
	ThreadBase();
	virtual ~ThreadBase();
	virtual void ProcThread();

public:
	virtual void Startup();

	virtual void Shutdown();

	virtual bool IsRunning();

	virtual unsigned int AddRequest(ThreadEvent* pThreadEvent);

	virtual void AbortAllRequests();

	virtual void RespondEvents(); 

	virtual void SetRespondingEventTimeLimit(unsigned int uTimeLimit);

	virtual unsigned int GetRespondingEventTimeLimit();

	virtual int GetRequestCount();

	//virtual int GetProcessCount();

	virtual int GetRespondCount();

	virtual void EnableSingleThread(bool bEnable);
protected:
	void WaitForNextRequest();

	virtual void ProcessNextRequest();

	bool HasTask();

protected:
	//typedef list<Thread*>::type				ThreadList;
	typedef std::deque<Request*>			RequestQueue;

	ThreadFunction		mThreadFunc;
	bool				mIsRunning;
	bool				mShuttingDown;
	bool				mIsSingleThread;
	unsigned int		mRequestCount;
	unsigned int		mTimeLimitMS;

	unsigned int		mMaxResquest;   //同时进行的最大任务数
	std::deque<ThreadEvent*>		mForProcQueue;	//等待处理的队列
	//bool				mHasTask;
	/*int					mInitSchedulingCount;
	int					mRequestSchedulingCount;
	int					mRespondSchedulingCount;*/

	Thread*				m_pThread;
	RequestQueue		mRequestQueue;
	//RequestQueue		mProcessQueue;
	RequestQueue		mResponseQueue[E_MAX_COUNT];

	Event m_MutexAddRequest;
	Event m_MutexThreadEnd;

	EE_MUTEX(mRequestMutex);
	//EE_MUTEX(mProcessMutex);
	EE_MUTEX(mResponseMutex[E_MAX_COUNT]);
};

class StreamThread : public ThreadBase
{
public:
	static StreamThread* Instance();

public:
	StreamThread();
	virtual ~StreamThread();
};

}

#endif//_Thread_h