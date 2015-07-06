/********************************************************************
    FileName :  RzMultiReadSingleWriteLock.h   
    Version  :  0.10
    Date     :	2010-1-25 18:42:25
    Author   :  小家伙
    Comment  :  多读单写锁

*********************************************************************/
#ifndef __RZDREADAWRITELOCK_H__
#define __RZDREADAWRITELOCK_H__
#include "RzType.h"
namespace RzStd
{
	typedef struct _RZ_InsideLock_
	{
		int nReadCount;
		bool bWriteFlag;
		RzLock rzLock;
	}SInsideLock; //内部锁

	class RZ_DLL_API CRzDReadAWriteLock
	{
	public:
		CRzDReadAWriteLock(void)
		{
			::InitializeCriticalSection(&m_Lock.rzLock);
			m_Lock.nReadCount = 0;
			m_Lock.bWriteFlag = false;
		};
		virtual ~CRzDReadAWriteLock(void)
		{
			::EnterCriticalSection(&m_Lock.rzLock);
			::LeaveCriticalSection(&m_Lock.rzLock);
			::DeleteCriticalSection(&m_Lock.rzLock);
		};
	public:
		void EnterWrite()
		{
			while(1)
			{
				::EnterCriticalSection(&m_Lock.rzLock);
				if ( !m_Lock.bWriteFlag ) //无写时即：设置写标致，写得到控制权
				{
					m_Lock.bWriteFlag = true;
					::LeaveCriticalSection(&m_Lock.rzLock); 
					goto _Start_Write;
				}
				//	//cout<<::GetCurrentThreadId()<<"有写，写等待...\n"; 
				::LeaveCriticalSection(&m_Lock.rzLock);
				Sleep(0/*1*/);
			}
_Start_Write:
			while(GetReadCount())//等待读计数器为 0
			{
				/*//cout<<"当前读个数有："<<GetReadCount()<<endl;	*/
				Sleep(/*1*/0);
			}
		};
		void LeaveWrite()
		{
			::EnterCriticalSection(&m_Lock.rzLock);
			//	//cout<<::GetCurrentThreadId()<<"写完成\n"; 
			m_Lock.bWriteFlag = false;
			::LeaveCriticalSection(&m_Lock.rzLock);
		};	
		int EnterRead()
		{
			int n=0;
			while(1)
			{
				::EnterCriticalSection(&m_Lock.rzLock);
				if (!m_Lock.bWriteFlag) //无线程在写
				{		
					n = ++m_Lock.nReadCount;
					goto _Start_Read;
				}
				//	//cout<<::GetCurrentThreadId()<<"有写，读等待...\n"; 
				::LeaveCriticalSection(&m_Lock.rzLock);
				Sleep(0/*1*/);
			}
_Start_Read:
			//	//cout<<::GetCurrentThreadId()<<"无写,进入读\n"; 
			::LeaveCriticalSection(&m_Lock.rzLock);
			return n;
		};
		int LeaveRead()
		{
			int n = 0;
			::EnterCriticalSection(&m_Lock.rzLock);
			if ( 0 < m_Lock.nReadCount )
			{
				n = --m_Lock.nReadCount;
			}
			//	//cout<<::GetCurrentThreadId()<<"读完成\n"; 
			::LeaveCriticalSection(&m_Lock.rzLock);
			return n;
		};
		int GetReadCount()
		{
			int n =0;
			::EnterCriticalSection(&m_Lock.rzLock);
			n = m_Lock.nReadCount;
			::LeaveCriticalSection(&m_Lock.rzLock);
			return n;
		};
		bool IsWrite()
		{
			bool bVar = false;
			::EnterCriticalSection(&m_Lock.rzLock);
			bVar = m_Lock.bWriteFlag;
			::LeaveCriticalSection(&m_Lock.rzLock);
			return bVar;
		};
	private:
		SInsideLock m_Lock;
	};
};
#endif // RzMultiReadSingleWriteLock_h__

