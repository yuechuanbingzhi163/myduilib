#ifndef mt_helper_h
#define mt_helper_h


#pragma once
#include <tchar.h>
#ifdef WIN32
	#include <cassert>
	#include <windows.h>
#endif // WIN32

namespace em_utility
{
	class RZ_DLL_API i_lock
	{
	public:
		i_lock() {}
		virtual ~i_lock() {}
		virtual bool lock() = 0;
		virtual bool unlock() = 0;
		virtual int lock_ex() { return 0; }
		virtual int unlock_ex() { return 0; }
	};
	
	class RZ_DLL_API lock_wrapper
	{
	public:
		lock_wrapper(i_lock* p_lock);
		~lock_wrapper();
		void unlock();
	private:
		i_lock* mp_lock;
	};
	////
	lock_wrapper::lock_wrapper(i_lock* p_lock)
	{
		mp_lock = p_lock;
		if (mp_lock != NULL)
			mp_lock->lock();
	}

	lock_wrapper::~lock_wrapper()
	{
		unlock();
	}

	void lock_wrapper::unlock()
	{
		if (mp_lock != NULL)
		{
			mp_lock->unlock();
			mp_lock = NULL;
		}
	}
	////

	//轻量级锁，不能用于waitforXXXobject，不能跨进程，非内核对象
	class RZ_DLL_API critical_session : public i_lock
	{
	public:
		critical_session();
		virtual ~critical_session();
		virtual bool lock();
		virtual bool unlock();
		bool try_lock();
		int get_ref() const;
		virtual int lock_ex();
		virtual int unlock_ex();
	private:
		struct critical_impl;
		critical_impl* mp_impl;
		volatile int mn_ref;
	};
	////
	struct critical_session::critical_impl
	{
		CRITICAL_SECTION* mp_cs;
	};

	critical_session::critical_session()
	{
		mp_impl = new critical_impl;
		mp_impl->mp_cs = new CRITICAL_SECTION();
		assert(mp_impl->mp_cs != NULL);
		::InitializeCriticalSection(mp_impl->mp_cs);
		mn_ref = 0;
	}

	critical_session::~critical_session()
	{
		::DeleteCriticalSection(mp_impl->mp_cs);
		delete mp_impl->mp_cs;
		mp_impl->mp_cs = NULL;
		delete mp_impl;
		mp_impl = NULL;
	}

	int critical_session::get_ref() const
	{
		//assert(mn_ref >= 0);
		return mn_ref;
	}

	int critical_session::lock_ex()
	{
		int n_result = 1000;
		if (mp_impl != NULL && mp_impl->mp_cs != NULL)
		{
			::EnterCriticalSection(mp_impl->mp_cs);
			//assert(mn_ref >= 0);
			++mn_ref;
			n_result = mn_ref;
			n_result = mp_impl->mp_cs->LockCount;
		}
		//write log here....

		return n_result;	
	}

	int critical_session::unlock_ex()
	{
		int n_result = 1000;
		if (mp_impl != NULL && mp_impl->mp_cs != NULL)
		{
			int n_temp = mp_impl->mp_cs->LockCount;
			::LeaveCriticalSection(mp_impl->mp_cs);
			//assert(mn_ref > 0);
			--mn_ref;
			n_result = mn_ref;
			n_result = mp_impl->mp_cs->LockCount;
		}
		// log here...

		return n_result;
	}

	bool critical_session::lock()
	{
		return lock_ex() != 1000;
	}

	bool critical_session::try_lock()
	{
		assert(false);
		//BOOL b_result = ::TryEnterCriticalSection(mp_impl->mp_cs);
		//return b_result == TRUE;
		return false;
	}

	bool critical_session::unlock()
	{
		return unlock_ex() != 1000;
	}

	////
	//互斥锁
	class RZ_DLL_API mutex_lock : public i_lock
	{
	public:
		mutex_lock();
		virtual ~mutex_lock();
		virtual bool lock();
		virtual bool unlock();
		//返回true为等待到信号，false为超时
		bool wait(unsigned int n_seconds);
		virtual int lock_ex();
		virtual int unlock_ex();
	private:
		struct mutex_impl;
		mutex_impl* mp_impl;
	};
	////
	//////////////////////////////////////////////////////////////////////////
	struct RZ_DLL_API mutex_lock::mutex_impl
	{
		HANDLE mh_mutex;
	};

	mutex_lock::mutex_lock()
	{
		mp_impl = new mutex_impl;
		mp_impl->mh_mutex = ::CreateMutex(NULL, TRUE, NULL);
		assert(mp_impl->mh_mutex != NULL);
	}

	mutex_lock::~mutex_lock()
	{
		::CloseHandle(mp_impl->mh_mutex);
		delete mp_impl;
	}

	bool mutex_lock::lock()
	{
		::WaitForSingleObject(mp_impl->mh_mutex, INFINITE);
		return true;
	}

	bool mutex_lock::unlock()
	{
		::ReleaseMutex(mp_impl->mh_mutex);
		return true;
	}

	int mutex_lock::lock_ex()
	{
		return lock() ? 0 : -1;
	}

	int mutex_lock::unlock_ex()
	{
		return unlock() ? 0 : -1;
	}

	bool mutex_lock::wait(unsigned int n_seconds)
	{
		DWORD n_result = ::WaitForSingleObject(mp_impl->mh_mutex, n_seconds * 1000);	
		assert(n_result != WAIT_ABANDONED);
		return n_result == WAIT_OBJECT_0;
	}
	////
	//信号事件锁
	class RZ_DLL_API signal_event
	{
	public:
		signal_event(bool b_signal = false, const TCHAR* p_name = NULL);
		~signal_event();
		void set_signal();
		void reset_signal();
		//返回true为发信号，返回false为超时
		bool wait(unsigned int n_mill_seconds);
		void* get_handle();
	private:
		struct event_impl;
		event_impl* mp_impl;
	};
	//////////////////////////////////////////////////////////////////////////
	struct RZ_DLL_API signal_event::event_impl
	{
		HANDLE mh_event;
	};

	signal_event::signal_event(bool b_signal, const TCHAR* p_name)
	{
		mp_impl = new event_impl();
		mp_impl->	mh_event = ::CreateEvent(NULL, TRUE, b_signal ? TRUE : FALSE, p_name	); 
		assert(mp_impl->mh_event != NULL);
		assert(::GetLastError() != ERROR_ALREADY_EXISTS);
	}

	signal_event::~signal_event()
	{
		BOOL b_result = ::CloseHandle(mp_impl->mh_event);
		assert(b_result == TRUE);
		delete mp_impl;
	}

	void signal_event::set_signal()
	{
		BOOL b_result = FALSE;
		if (mp_impl != NULL && mp_impl->mh_event != INVALID_HANDLE_VALUE)
			b_result = ::SetEvent(mp_impl->mh_event);
		assert(b_result == TRUE);
		return;
	}

	void signal_event::reset_signal()
	{
		BOOL b_result = ::ResetEvent(mp_impl->mh_event);
		assert(b_result == TRUE);
		return;
	}

	bool signal_event::wait(unsigned int n_mill_seconds)
	{
		DWORD n_result = ::WaitForSingleObject(mp_impl->mh_event, n_mill_seconds);
		assert(n_result != WAIT_ABANDONED);
		return n_result == WAIT_OBJECT_0;
	}

	void* signal_event::get_handle()
	{
		return mp_impl->mh_event;
	}

///////////////////////////////////////////////////////////

}

#endif