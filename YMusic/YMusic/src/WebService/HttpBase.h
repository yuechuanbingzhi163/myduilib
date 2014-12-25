#ifndef _HttpBase_h
#define _HttpBase_h
#pragma once

#include "../../YMusic.h"

class CHttpBase : public Singleton<CHttpBase>
{
public:
	struct CurlData
	{
		char* url;					/*这个就是要使用的url了.*/
		char* data;					/*data是post的内容*/
		char *buffer;               /* buffer to store cached data*/
		size_t buffer_len;          /* currently allocated buffers length */
		size_t buffer_pos;          /* end of data in buffer*/
		bool   bshowprogress;       /*是否开启进度*/
		double dltotal;
		double dlnow;
		double ultotal;
		double ulnow;
		HWND   hWnd;
		void Init()
		{
			bshowprogress = false;
			dltotal = dlnow = ultotal = ulnow = 0;
			hWnd = NULL;

			url = NULL;

			data = NULL;
			buffer = NULL;
			buffer_len = buffer_pos = 0;
		}

		//<By Jackie 2013-6-25>这个函数会delete this, 所以必须是最后调用的函数!!!
		void Clear()
		{
			if(url) 
			{
				delete []url;
				url = NULL;
			}
			if(data)
			{
				delete []data;
				data = NULL;
			}
			if(buffer)
			{
				delete []buffer;
				buffer = NULL;
			}
//			delete this;
		}
		void InitGetMethod(const char* s_url)
		{
			assert(s_url);
			if(url)
			{
				delete []url;
				url = NULL;
			}
			url = new char[256];
			memset(url,0,256);
			_snprintf_s(url,256,256,s_url);
		}
		void InitPostMethod(const char* s_url,const char* s_data)
		{
			assert(s_url&&s_data);
			if(url) delete []url,url = NULL;
			if(data) delete []data,data = NULL;
			url = new char[256];
			memset(url,0,256);
			_snprintf_s(url,256,256,s_url);
			data = new char[256];
			memset(data,0,256);
			_snprintf_s(data,256,256,s_data);
		}
	};
public:
	CHttpBase();
	virtual ~CHttpBase();
public:
	int DoPost();
	int DoGet();
public:
	int MakeCurlData(CurlData* pdata,HWND hWndOwner);
protected:
	void Init();
	void Clear();
	static size_t WriteFunction( void* ptr, size_t size, size_t nmemb, void *usrptr );
	static int ProgressFunction(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
protected:
	CURL*     m_url;
	CurlData* m_data;
	HWND      m_hWndOwner;
};



#endif//_HttpBase_h