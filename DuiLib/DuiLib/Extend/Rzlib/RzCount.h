#ifndef  __RZCOUNT_H__
#define  __RZCOUNT_H__

#include "RzLock.h"
namespace RzStd
{
	class RZ_DLL_API CRzCount
	{
	public:
		explicit CRzCount():m_i(0){};
		~CRzCount(){};
	private:
		CRzLock m_Lock;
		unsigned int m_i;
	public:
		unsigned int  Set(const unsigned int& n)
		{
			unsigned int nResult = n;
			m_Lock.Lock();
			m_i = n;
			m_Lock.UnLock();
			return nResult;
		};
		unsigned int  Add()
		{
			unsigned int nResult = 0;
			m_Lock.Lock();
			nResult = ++m_i;
			m_Lock.UnLock();
			return nResult;
		};
		unsigned int  Dec()
		{
			unsigned int nResult = 0;
			m_Lock.Lock();
			nResult = --m_i;
			m_Lock.UnLock();
			return nResult;
		};
		unsigned int  GetCount()
		{
			unsigned int nResult = 0;
			m_Lock.Lock();
			nResult = m_i;
			m_Lock.UnLock();
			return nResult;
		};
	};
};
#endif

