/********************************************************************
    FileName :  RzLock.h   
    Version  :  0.10
    Date     :	2010-1-26 18:50:16
    Author   :  小家伙
    Comment  :  常规锁

*********************************************************************/
#ifndef  __RZLOCK_H__
#define  __RZLOCK_H__
#pragma once
#include <windows.h>
class  CRzLock
{
public:
	typedef CRITICAL_SECTION LOCK;
	CRzLock(void);

	virtual ~CRzLock(void);
	
private:
	LOCK m_lock;
public:
	void Lock();

	void UnLock();
	
};

#endif

