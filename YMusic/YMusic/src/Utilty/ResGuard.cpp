
#include "stdafx.h"
#include "ResGuard.h"
// Instances of this class will be accessed by multiple threads. So,
// all members of this class (except the constructor and destructor)
// must be thread-safe.

CResGuard::CResGuard()
{
	m_lGrdCnt = 0;
	InitializeCriticalSection(&m_cs);
}
CResGuard::~CResGuard()
{
	DeleteCriticalSection(&m_cs);
}

// IsGuarded is used for debugging
BOOL CResGuard::IsGuarded() const
{
	return(m_lGrdCnt > 0);
}




void CResGuard::Guard()
{
	EnterCriticalSection(&m_cs);
	m_lGrdCnt++;
}
void CResGuard::Unguard()
{
	m_lGrdCnt--;
	LeaveCriticalSection(&m_cs);
}

// Guard/Unguard can only be accessed by the nested CGuard class.


