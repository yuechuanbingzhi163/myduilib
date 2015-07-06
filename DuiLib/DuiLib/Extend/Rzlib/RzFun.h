#ifndef __RZFUN_H__
#define __RZFUN_H__
#include <time.h>
#include <sstream>
#include <iostream>
#include "RzType.h"
using namespace std;


_RzStdBegin
namespace RzStd
{
	tm* RzGetNowTime()
	{
		time_t tt;
		time(&tt);
		return localtime(&tt);		
	};
};

/*
template <class Out_type,class In_type>
Out_type RzConvert(In_type &t)
{
	Rzstringstream str;
	
	str<<t;
	Out_type out_result;
	str>>out_result;
	return out_result;
}*/


void RzGetModulePath(TCHAR* pszText,ULONG nlen)
{
	TCHAR szFileName[MAX_PATH];
	ULONG n = GetModuleFileName(GetModuleHandle(NULL),szFileName,nlen);
	TCHAR* p = Rzstrrchr(szFileName,_T('\\'));
	if(p)
		Rzstrncpy(pszText,szFileName,n - Rzstrlen(p));
}

template<typename T>
inline void SafeDelete(T &p)
{
	// 不用判断是否是NULL，delete自己知道该干什么，
	// 这样写的话，顶多浪费一个赋值操作“=”。
	delete p;
	p = NULL;
}

_RzStdEnd

#endif
