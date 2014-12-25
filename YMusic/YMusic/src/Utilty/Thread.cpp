#include "StdAfx.h"
#include "Thread.h"


CThread::CThread(): m_hThread(NULL)
{

}


CThread::~CThread()
{
	if(m_hThread)
	{
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}


unsigned int CThread::ThreadProc( void* pArgs)
{

	CThread* pthread = (CThread*)pArgs;
	pthread->Run(NULL);

	//<By Jackie 2013-6-8>的线程类, 是在heap上申请的, 我们在这里delete掉.
	// 如果声明为局部变量, 那么就会出现提前析构的情况.
	delete pthread;
	return 0;
}

void CThread::Start()
{
	assert(m_hThread == NULL);
	unsigned int threadId = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, this, 0, &threadId);
	
}

unsigned int CThread::Run(void*)
{
	//<By Jackie 2013-6-8>在继承类中扩展这个函数.
	return 0;
}
