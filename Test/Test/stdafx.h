// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <objbase.h>
#include <zmouse.h>
#include <exdisp.h>
#include <comdef.h>
#include <vector>
#include <sstream>
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#ifndef _USE_DUILIB_
#define _USE_DUILIB_
#endif

#include "DuiLib\UIlib.h"
using namespace DuiLib;
// #ifdef _DEBUG
// #   ifdef _UNICODE
// #       pragma comment(lib, "DuiLib_ud.lib")
// #   else
// #       pragma comment(lib, "DuiLib_d.lib")
// #   endif
// #else
// #   ifdef _UNICODE
// #       pragma comment(lib, "..\\Lib\\DuiLib_u.lib")
// #   else
// #       pragma comment(lib, "..\\Lib\\DuiLib.lib")
// #   endif
// #endif