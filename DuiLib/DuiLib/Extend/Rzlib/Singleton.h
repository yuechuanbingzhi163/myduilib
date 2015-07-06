#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#pragma once

#include "RzGuard.h"
using namespace std;



template <class T>
class RZ_DLL_API Singleton
{
public:
	static inline T* Instance();

	Singleton(void) {}
	~Singleton(void){ /*if( 0 != _instance.get() ) _instance.reset();*/ }
private:

	Singleton(const Singleton& ) {}
	Singleton& operator= (const Singleton& ) {}
protected:
	static auto_ptr<T> _instance;
	static CResGuard _rs;
};

template <class T>
auto_ptr<T> Singleton<T>::_instance;

template <class T>
CResGuard Singleton<T>::_rs;


template <class T>
inline T* Singleton<T>::Instance()
{
	if( 0 == _instance.get() )
	{
		CResGuard::CGuard gd(_rs);
		if( 0 == _instance.get())
		{
			_instance.reset ( new T);
		}
	}
	return _instance.get();
}

#define DECLARE_SINGLETON_CLASS( type ) \
	friend class auto_ptr< type >;\
	friend class Singleton< type >;


#endif