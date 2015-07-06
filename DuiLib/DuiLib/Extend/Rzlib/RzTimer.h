#ifndef _RzTimer_H
#define _RzTimer_H
#pragma once
#include "RzType.h"
#include <algorithm>
#include <time.h>

_RzCFunBegin
_RzNameSpaceBegin(RzStd)

class RZ_DLL_API Timer
{
public:
	static Timer* Instance()
	{
		static Timer timer;
		return &timer;
	}
public:
	inline Timer()
#ifdef _WIN32
		: m_TimerMask(0)
#endif	
	{
		reset();
	}
	~Timer(){}
public:
	inline void reset()
	{
#ifdef _WIN32
		DWORD_PTR procMask;
		DWORD_PTR sysMask;
		GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

		// If procMask is 0, consider there is only one core available
		// (using 0 as procMask will cause an infinite loop below)
		if (procMask == 0)
			procMask = 1;

		// Find the lowest core that this process uses
		if( m_TimerMask == 0 )
		{
			m_TimerMask = 1;
			while( ( m_TimerMask & procMask ) == 0 )
			{
				m_TimerMask <<= 1;
			}
		}
		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, m_TimerMask);

		// Get the constant frequency
		QueryPerformanceFrequency(&m_Frequency);

		// Query the timer
		QueryPerformanceCounter(&m_StartTime);
		m_StartTick = GetTickCount();

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		m_LastTime = 0;
		m_ZeroClock = clock();
#else
		m_ZeroClock = clock();
		gettimeofday(&m_startTime, NULL);
#endif
	}

	inline unsigned long getMilliseconds()
	{
#ifdef _WIN32
		LARGE_INTEGER curTime;

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, m_TimerMask);

		// Query the timer
		QueryPerformanceCounter(&curTime);

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		LONGLONG newTime = curTime.QuadPart - m_StartTime.QuadPart;

		// scale by 1000 for milliseconds
		unsigned long newTicks = (unsigned long) (1000 * newTime / m_Frequency.QuadPart);

		// detect and compensate for performance counter leaps
		// (surprisingly common, see Microsoft KB: Q274323)
		unsigned long check = GetTickCount() - m_StartTick;
		signed long msecOff = (signed long)(newTicks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			// We must keep the timer running forward :)
			LONGLONG adjust = (std::min)(msecOff * m_Frequency.QuadPart / 1000, newTime - m_LastTime);
			m_StartTime.QuadPart += adjust;
			newTime -= adjust;

			// Re-calculate milliseconds
			newTicks = (unsigned long) (1000 * newTime / m_Frequency.QuadPart);
		}

		// Record last time for adjust
		m_LastTime = newTime;

		return newTicks;
#else

		struct timeval now;
		gettimeofday(&now, NULL);
		return (now.tv_sec-m_startTime.tv_sec)*1000+(now.tv_usec-m_startTime.tv_usec)/1000;
#endif
	}

	inline unsigned long getMicroseconds()
	{
#ifdef _WIN32
		LARGE_INTEGER curTime;

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, m_TimerMask);

		// Query the timer
		QueryPerformanceCounter(&curTime);

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		LONGLONG newTime = curTime.QuadPart - m_StartTime.QuadPart;

		// get milliseconds to check against GetTickCount
		unsigned long newTicks = (unsigned long) (1000 * newTime / m_Frequency.QuadPart);

		// detect and compensate for performance counter leaps
		// (surprisingly common, see Microsoft KB: Q274323)
		unsigned long check = GetTickCount() - m_StartTick;
		signed long msecOff = (signed long)(newTicks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			// We must keep the timer running forward :)
			LONGLONG adjust = (std::min)(msecOff * m_Frequency.QuadPart / 1000, newTime - m_LastTime);
			m_StartTime.QuadPart += adjust;
			newTime -= adjust;
		}

		// Record last time for adjust
		m_LastTime = newTime;

		// scale by 1000000 for microseconds
		unsigned long newMicro = (unsigned long) (1000000 * newTime / m_Frequency.QuadPart);

		return newMicro;
#else

		struct timeval now;
		gettimeofday(&now, NULL);
		return (now.tv_sec-m_startTime.tv_sec)*1000000+(now.tv_usec-m_startTime.tv_usec);
#endif
	}

	inline unsigned long getMillisecondsCPU()
	{
		clock_t newClock = clock();
		return (unsigned long)((float)(newClock-m_ZeroClock) / ((float)CLOCKS_PER_SEC/1000.0)) ;
	}

	inline unsigned long getMicrosecondsCPU()
	{
		clock_t newClock = clock();
		return (unsigned long)((float)(newClock-m_ZeroClock) / ((float)CLOCKS_PER_SEC/1000000.0)) ;
	}
protected:
	clock_t m_ZeroClock;
#ifdef _WIN32
	DWORD m_StartTick;
	LONGLONG m_LastTime;
	LARGE_INTEGER m_StartTime;
	LARGE_INTEGER m_Frequency;

	DWORD_PTR m_TimerMask;
#else
	struct timeval m_startTime;
#endif
};

_RzNameSpaceEnd
_RzCFunEnd


#endif