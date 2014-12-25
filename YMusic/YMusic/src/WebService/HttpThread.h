#ifndef __HTTP_THREAD_H__
#define __HTTP_THREAD_H__

#pragma once

#include "../../YMusic.h"
#include "../WebService/HttpManager.h"

class CHttpThread: public CThread
{
public:
	CHttpThread(){}
	CHttpThread(CurlData* pdata, HWND hwnd, int iMessage);
	virtual ~CHttpThread();

	void SetData(CurlData* pdata){ m_pdata = pdata;}
	void SetHwnd(HWND hWnd){m_hUIWnd = hWnd;}
	void SetMsg(int msg){m_iMessage = msg;}
private:
	virtual unsigned int Run(void*);
	CurlData* m_pdata;
	HWND m_hUIWnd;
	int m_iMessage; //<By Jackie 2013-6-9> 设定是需要把请求出来的内容发送给主窗口.
};

#endif