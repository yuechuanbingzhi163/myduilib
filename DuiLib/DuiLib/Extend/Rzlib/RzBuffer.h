#ifndef __RZBUFFER_H__
#define __RZBUFFER_H__
#include "RzDReadAWriteLock.h"
#include <iostream>
#include <vector>
using namespace std;
using namespace RzStd;

namespace RzStd
{
	template<class T,int Size>
	class RZ_DLL_API CRzBuffer
	{
	public:
		CRzBuffer();
		~CRzBuffer();
	public:
		bool Put(T& data);
		bool Get(T& data); 
	private:
		int  IsUse();
		bool IsFull();
		bool IsEmpty();
	private:
		int         _nSize;
		T           _elements[Size];
		int         _nFullSize;
		int         _nRead;
		int         _nWrite;
		CRzDReadAWriteLock m_Lock;
	};

	template<class T,int Size>
	CRzBuffer<T,Size>::CRzBuffer():_nSize(Size),_nRead(0),_nWrite(0)
	{
		_nFullSize = _nSize -1;
		for (int i=0;i<_nSize;++i)
		{
			_elements[i] = T();
		}
	}
	template<class T,int Size>
	CRzBuffer<T,Size>::~CRzBuffer()
	{		
	}

	template<class T,int Size>
	bool CRzBuffer<T,Size>::Put(T& data)
	{
		bool bResult = false;
		m_Lock.EnterWrite();
		{
			if (!IsFull())
			{
				_elements[_nWrite++] = data;
				cout<<"Write:"<<_nWrite<<endl;
				_nWrite %= _nSize;
				bResult = true;
			}
		}
		m_Lock.LeaveWrite();
		return bResult;
	}
	template<class T,int  Size>
	bool CRzBuffer<T,Size>::Get(T& data)
	{
		bool bResult = false;
		m_Lock.EnterWrite();
		{
			if (!IsEmpty())
			{
				data = _elements[_nRead++];
				cout<<"Read:"<<_nRead<<endl;
				_nRead %= _nSize;
				bResult = true;
			}
		}
		m_Lock.LeaveWrite();
		return bResult;
	}
	template<class T,int Size>
	int CRzBuffer<T,Size>::IsUse()
	{
		if (_nWrite > _nRead)
		{
			return _nWrite - _nRead;
		}
		else if(_nWrite < _nRead)
		{
			return _nFullSize - (_nRead - _nWrite);
		}
		else
			return 0;
	}
	template<class T,int Size>
	bool CRzBuffer<T,Size>::IsFull()
	{
		return IsUse() == _nFullSize;
	}
	template<class T,int Size>
	bool CRzBuffer<T,Size>::IsEmpty()
	{
		return 0 == IsUse();
	}
};
#endif 