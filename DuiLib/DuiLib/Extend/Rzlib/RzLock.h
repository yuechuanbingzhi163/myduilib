/********************************************************************
    FileName :  RzLock.h   
    Version  :  0.10
    Date     :	2010-1-26 18:50:16
    Author   :  小家伙
    Comment  :  常规锁

*********************************************************************/
#ifndef  __RZLOCK_H__
#define  __RZLOCK_H__
#include "RzType.h"

_RzCFunBegin
_RzNameSpaceBegin(RzStd)
class RZ_DLL_API CRzLock
{
public:
	CRzLock(void)
	{
		InitializeCriticalSection(&m_lock);
	};
	virtual ~CRzLock(void)
	{
		DeleteCriticalSection(&m_lock);
	};
private:
	RzLock m_lock;
public:
	void Lock()
	{
		EnterCriticalSection(&m_lock);
	};
	void UnLock()
	{
		LeaveCriticalSection(&m_lock);
	};
};
_RzNameSpaceEnd
_RzCFunEnd
#endif

