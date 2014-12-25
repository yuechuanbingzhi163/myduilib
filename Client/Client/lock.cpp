#include "stdafx.h"
#include "lock.h"

CLock::CLock(void)
{
	InitializeCriticalSection(&m_lock);
};
CLock::~CLock(void)
{
	DeleteCriticalSection(&m_lock);
}

void CLock::Lock()
{
	EnterCriticalSection(&m_lock);
}
void CLock::UnLock()
{
	LeaveCriticalSection(&m_lock);
}