#include "StdAfx.h"
#include "ThreadHelper.h"
#include <iostream>
using namespace std;

namespace em_utility{
static unsigned __stdcall ThreadCallBackProc(void *pvArg)
{
	(static_cast<ThreadFunction *>(pvArg))->Run();
	return 0;
}

//Wiindows Event
Event::Event(void)
{
	mEvent = NULL;
}
Event::~Event(void)
{
	Destroy();
}
void Event::Create()
{
	mEvent = CreateEvent(NULL, true, 0, NULL);
}
void Event::Destroy()
{
	if (mEvent != NULL)
	{
		CloseHandle(mEvent);
		mEvent = NULL;	
	}
}
void Event::Trigger(void)
{
	SetEvent(mEvent);
}
void Event::Wait()
{
	WaitForSingleObject(mEvent, INFINITE);
	ResetEvent(mEvent);
}
/////////////////////////////////////////////////////////////////
ThreadFunction::ThreadFunction()
	: m_pThreadManager(NULL)
{}
ThreadFunction::~ThreadFunction()
{}
void ThreadFunction::operator()()
{
	assert(m_pThreadManager);
	m_pThreadManager->ProcThread();
}
void ThreadFunction::Run()
{
	assert(m_pThreadManager);
	m_pThreadManager->ProcThread();
}
///////////////////////////////////////////////////////////////
Thread::Thread()
	: m_hThread(NULL)
	, m_pThreadFunction(NULL)
{}
Thread::~Thread()
{
	Uninit();
}
int Thread::Init(ThreadFunction* pThreadFunction)
{
	if (!pThreadFunction)
		return false;

	if (m_hThread)
		return false;

	m_pThreadFunction = pThreadFunction;

	m_hThread = (HANDLE)_beginthreadex(
		NULL,			        	// SD
		0,				        	// initial stack size
		ThreadCallBackProc,			// thread function
		(void*)m_pThreadFunction, 	// thread argument
		0,				        	// creation option
		NULL						// thread identifier
		);	
	if (!m_hThread)
	{
		cout<<"error"<<endl;
		return false;
	}
	cout<<"_beginthreadex success"<<endl;
	return true;
}
void Thread::Uninit()
{
	if (m_hThread)
	{

		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);

		m_hThread = NULL;
		m_pThreadFunction = NULL;
	}
}
//////////////////////////////////////////////////////////////////////////
Request::Request(unsigned int requestID)
	: mAborted(false)
	, mRequestID(requestID)
	, mThreadEvent(NULL)
{}
Request::~Request()	
{
	if (mThreadEvent && mThreadEvent->IsAutoDestroy())
	{
		delete mThreadEvent;
		mThreadEvent = NULL;
	}
}
void Request::SetThreadEvent(ThreadEvent* pThreadEvent)	
{ 
	mThreadEvent = pThreadEvent; 
}
ThreadEvent* Request::GetThreadEvent()	
{ 
	return mThreadEvent;
}
void Request::Abort()		
{ 
	mAborted = true;	
}
bool Request::NeedAborted()	
{ 
	return mAborted;	
}
unsigned int Request::GetID()		
{ 
	return mRequestID;	
}
//////////////////////////////////////////////////////////////////////////
ThreadBase::ThreadBase()
	: mIsRunning(false)
	, mShuttingDown(false)
	, mIsSingleThread(false)
	, mRequestCount(0)
	, mTimeLimitMS(0)
	, m_pThread(NULL)
	, mMaxResquest(5)
{}
ThreadBase::~ThreadBase()
{}
void ThreadBase::Startup()
{	
	if (mIsRunning)
		return;

	mShuttingDown = false;

	mThreadFunc.SetThreadManager(this);

	m_MutexAddRequest.Create();
	m_MutexThreadEnd.Create();

	m_pThread = new Thread;
	assert(m_pThread);

	if (!m_pThread->Init(&mThreadFunc))
	{
		cout<<"Init Thread error"<<endl;
		return ;
	}

	mIsRunning = true;

	cout<<"Startup Thread"<<endl;
}
void ThreadBase::Shutdown()
{
	if (!mIsRunning)
		return;

	AbortAllRequests();

	Request* pRequest = NULL;

	{
		EE_LOCK_MUTEX(mRequestMutex);
		while (!mRequestQueue.empty())
		{
			pRequest = mRequestQueue.front();
			mRequestQueue.pop_front();	

			if (pRequest)
			{
				delete pRequest;
			}
		}
	}

	mShuttingDown = true;
	m_MutexAddRequest.Trigger();
	m_MutexThreadEnd.Wait();

	m_pThread->Uninit();
	delete m_pThread;
	m_pThread = NULL;

	m_MutexAddRequest.Destroy();
	m_MutexThreadEnd.Destroy();

	for (int i = E_MAX_COUNT-1; i >= 0; --i)
	{
		EE_LOCK_MUTEX(mResponseMutex[i]);

		if (mResponseQueue[i].empty())
			continue;

		pRequest = mResponseQueue[i].front();
		mResponseQueue[i].pop_front();	

		if (pRequest)
		{
			ThreadEvent* pThreadEvent = pRequest->GetThreadEvent();
			assert(pThreadEvent);
			if (!pThreadEvent->RespondEvent())
			{
				EE_LOCK_MUTEX(mResponseMutex[pThreadEvent->GetLevel()]);
				mResponseQueue[pThreadEvent->GetLevel()].push_front(pRequest);
			}
			else
			{
				delete pRequest;
			}
		}		
	}


	mIsRunning = false;
	cout<<"Stream thread Ended."<<endl;
}
bool ThreadBase::IsRunning()
{
	return mIsRunning;
}
unsigned int ThreadBase::AddRequest(ThreadEvent* pThreadEvent)
{
	assert(pThreadEvent);

	if (mShuttingDown)
		return 0;

	if (mIsSingleThread)
	{
		pThreadEvent->ProcessEvent();
		pThreadEvent->RespondEvent();
		return 0;
	}
// 	if(mRequestQueue.size()>mMaxResquest)
// 	{
// 		mForProcQueue.push_back(pThreadEvent);
// 		return 0;
// 	}

	mRequestCount++;

	Request* pRequest = new Request(mRequestCount);
	if ( pRequest == NULL )
	{
		return 0;
	}
	assert(pRequest);

	pRequest->SetThreadEvent(pThreadEvent);

	{
		EE_LOCK_MUTEX(mRequestMutex);
		mRequestQueue.push_back(pRequest);
	}

	m_MutexAddRequest.Trigger();

	return mRequestCount;
}
void ThreadBase::AbortAllRequests()
{
	{
		EE_LOCK_MUTEX(mRequestMutex);

		for (RequestQueue::iterator iter = mRequestQueue.begin(); iter != mRequestQueue.end(); ++iter)
		{
			(*iter)->Abort();
		}
	}

	for (int i = E_MAX_COUNT-1; i >= 0; --i)
	{
		EE_LOCK_MUTEX(mResponseMutex[i]);

		for (RequestQueue::iterator iter = mResponseQueue[i].begin(); iter != mResponseQueue[i].end(); ++iter)
		{
			(*iter)->Abort();
		}
	}
}
void ThreadBase::RespondEvents()
{
	if ( mResponseQueue[E_NORMAL].size() ||
		mResponseQueue[E_HIGHT].size() )
	{
		Request* pRequest	= NULL;
		ThreadEvent* pThreadEvent = NULL;

		for (int i=E_MAX_COUNT-1; i>=0; --i)
		{
			unsigned int t = 0;
			while(t++<3)
			{
				{
					EE_LOCK_MUTEX(mResponseMutex[i]);
					if (mResponseQueue[i].empty())
						break;

					pRequest = mResponseQueue[i].front();	
					mResponseQueue[i].pop_front();
				}

				if (pRequest)
				{
					if (!pRequest->NeedAborted())
					{
						pThreadEvent = pRequest->GetThreadEvent();
						assert(pThreadEvent);

						if(pThreadEvent->RespondEvent())
						{
							delete pRequest;
						}
						else
						{
							mResponseQueue[i].push_front(pRequest);
						}
					}
					else
					{
						delete pRequest;	
					}
				}
			}//while
		}//for
	}//if size
}
void ThreadBase::SetRespondingEventTimeLimit(unsigned int uTimeLimit)
{
	mTimeLimitMS = uTimeLimit;
}
unsigned int ThreadBase::GetRespondingEventTimeLimit()
{
	return mTimeLimitMS;
}
int ThreadBase::GetRequestCount()
{
	return (int)mRequestQueue.size();
}
int ThreadBase::GetRespondCount()
{
	int nSum = 0;
	for (int i = E_MAX_COUNT - 1; i >= 0; --i)
	{
		nSum += (int)(mResponseQueue[i].size());
	}
	return nSum;
}
bool ThreadBase::HasTask()
{
	return GetRespondCount() || GetRequestCount();
}
void ThreadBase::WaitForNextRequest()
{
	mRequestMutex.Lock();

	if (!mRequestQueue.empty())
	{
		mRequestMutex.Unlock();
		return ;
	}

	mRequestMutex.Unlock();

	m_MutexAddRequest.Wait();
}
void ThreadBase::ProcessNextRequest()
{
	Request* pRequest				= NULL;
	ThreadEvent* pThreadEvent		= NULL;

	// :::::::::::::::::处理请求队列:::::::::::::::::::
	do
	{
		EE_LOCK_MUTEX(mRequestMutex);

		if (mRequestQueue.empty())
			continue;
		//Modify
// 		if (mRequestQueue.size()<mMaxResquest&&!mForProcQueue.empty())
// 		{
// 			ThreadEvent* pEvent = mForProcQueue.front();
// 			mForProcQueue.pop_front();
// 			AddRequest(pEvent);
// 		}
		pRequest = mRequestQueue.front();
		mRequestQueue.pop_front();

		if (pRequest->NeedAborted())
		{
			delete pRequest;
			pRequest = NULL;
		}
		else
		{
			break;
		}		
	} while(0);

	if (!pRequest)
		return;

	pThreadEvent = pRequest->GetThreadEvent();
	assert(pThreadEvent);

	pThreadEvent->ProcessEvent();

	{
		EE_LOCK_MUTEX(mResponseMutex[pThreadEvent->GetLevel()]);
		if (!mShuttingDown)
			mResponseQueue[pThreadEvent->GetLevel()].push_back(pRequest);
	}
}
void ThreadBase::ProcThread()
{
	while (!mShuttingDown)
	{
		WaitForNextRequest();
		if(mShuttingDown)
			break;
		ProcessNextRequest();
	}
	m_MutexThreadEnd.Trigger();
}
void ThreadBase::EnableSingleThread( bool bEnable )
{
	mIsSingleThread = bEnable;
}
///////////////////////////////////////////////////////////////////
StreamThread* StreamThread::Instance()
{
	static StreamThread ms_pSingleton;
	return &ms_pSingleton;
}

StreamThread::StreamThread()
{
}
StreamThread::~StreamThread()
{
}

}