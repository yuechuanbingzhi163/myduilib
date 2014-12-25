#include "stdafx.h"
#include "RzLock.h"


CRzLock::CRzLock(void)
{
	InitializeCriticalSection(&m_lock);
};
CRzLock::~CRzLock(void)
{
	DeleteCriticalSection(&m_lock);
};

void CRzLock::Lock()
{
	EnterCriticalSection(&m_lock);
};
void CRzLock::UnLock()
{
	LeaveCriticalSection(&m_lock);
};
