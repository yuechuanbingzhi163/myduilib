#ifndef Utility_H
#define Utility_H
#pragma once

namespace DuiLib
{
	class UILIB_API Console
	{
	private:
		static bool m_bConsoleInit;
	protected:
		static void _RedirectIOToConsole( );
	public:
#ifdef _UNICODE
		static void Write(const TCHAR* format,...);
#else
		static void Write(const char* format,...);
#endif


	};

	class UILIB_API VSOutput
	{
	public:

#ifdef _UNICODE
		static void Write(const TCHAR* format,...);
#else
		static void Write(const char* format,...);
#endif
	};

	class UILIB_API Math
	{
	public:
		/*
		* 随机值： 0-1
		*/
		static float UnitRandom();
		/*
		* 随机值： fLow-fHigh
		*/
		static float RangeRandom(float fLow, float fHigh);

		/*
		* 随机值： (-1) - (1)
		*/
		static float SymmetricRandom();
	};

	template<typename T>
	inline void SafeRelease(T p)
	{
		if ( p )
		{
			p->Release();
		}
		p = NULL;
	}

	template<typename T>
	inline void SafeDelete(T p,bool isArr = false)
	{
		if (isArr)
		{
			delete []p;
			p = NULL;
		}
		else
		{
			delete p;
			p = NULL;
		}
	}

	template<typename T>
	inline void Swap(T& r, T& l)
	{
		T tmp = r;
		r = l;
		l = tmp;
	}
}


#endif