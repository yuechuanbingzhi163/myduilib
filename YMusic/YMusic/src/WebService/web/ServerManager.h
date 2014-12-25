#ifndef __SERVER_MANAGER_H__
#define __SERVER_MANAGER_H__

#pragma once
#include "../../YMusic.h"
//<By Jackie 2013-9-11> 所有业务层的http请求都从这里出.
class CServerManager
{
public:
	CServerManager();
	~CServerManager();
public:
//	void GetFlowList(vector<Flow>& flowList);
	int Login(CString userName, CString passwordMD5);

};

typedef Singleton<CServerManager> ServerFactory;

#endif