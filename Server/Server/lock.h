/********************************************************************
    FileName :  Lock.h   
    Version  :  0.10
    Date     :	2010-1-26 18:50:16
    Author   :  小家伙
    Comment  :  常规锁

*********************************************************************/
#ifndef  __LOCK_H__
#define  __LOCK_H__
#pragma once
//#include <windows.h>
class  CLock
{
public:
	typedef CRITICAL_SECTION    LOCK;
	CLock(void);
	
	virtual ~CLock(void);
	
private:
	LOCK m_lock;
public:
	void Lock();
	
	void UnLock();

};

#endif//__LOCK_H__

