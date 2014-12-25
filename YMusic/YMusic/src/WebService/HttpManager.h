#ifndef __HTTP_MANAGER_H__
#define __HTTP_MANAGER_H__

#pragma once
#include "../../YMusic.h"

//下面这个结构体, 主要是用来把url传递给线程, 让线程执行, 然后
class CurlData
{
public:
	char* url;					/* 这个就是要使用的url了.*/
	char* data;					/* data是post的内容,在post下面使用*/
	bool  bpost;				/* bpost是否以Post方式请求*/
	char *buffer;               /* buffer to store cached data*/
	size_t buffer_len;          /* currently allocated buffers length */
	size_t buffer_pos;          /* end of data in buffer*/
	bool   bshowprogress;       /*是否开启进度*/
	double dltotal;
	double dlnow;
	double ultotal;
	double ulnow;
	UINT n_type;

	HWND h_notify_wnd;
	UINT n_notify_msg;
	WPARAM n_wparam;
	LPARAM n_lparam;

	bool  bwritefile;
	char* filename;
	FILE* fp;
public:
	CurlData()
	{
		Init();
	}
	~CurlData()
	{
		Clear();
	}
public:
	void SetURL(const char* s_url)
	{
		assert(s_url);
		unsigned int len = strlen(s_url);
		url = new char[len+1];
		memset(url,0,len);
		memcpy(url,s_url,len);
		//_snprintf_s(url,1,len,s_url);
		url[len] = '\0';
	}

	void SetData(const char* s_data)
	{
		assert(s_data);
		unsigned int len = strlen(s_data);
		data = new char[len+1];
		memset(data,0,len);
		memcpy(data,s_data,len);
		data[len] = '\0';
		bpost = true;
	}

	void SetFileName(const char* s_data)
	{
		assert(s_data);
		if(strlen(s_data)==0) return;
		bwritefile = true;
		unsigned int len = strlen(s_data);
		filename = new char[len+1];
		memset(filename,0,len);
		memcpy(filename,s_data,len);
		filename[len] = '\0';
	}
protected:
	void Init()
	{
		url = NULL;		
		data = NULL;
		bpost = false;
		buffer = NULL;
		buffer_len = buffer_pos = 0;
		bwritefile = false;
		filename = NULL;
		fp = NULL;
		bshowprogress = false;
		dltotal = dlnow = ultotal = ulnow = 0;
		n_type = 0;

		h_notify_wnd = NULL;
		n_notify_msg = 0;
		n_wparam = 0;
		n_lparam = 0;
	}

	//这个函数会delete this, 所以必须是最后调用的函数!!!
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
		if (filename)
		{
			delete []filename;
			filename = NULL;
		}
		if (fp)
		{
			fclose(fp);
			fp = NULL;
		}
		delete this;
	}

};

typedef smart_pointer::SmartPtr<CurlData> spCUrlDataT;

class HttpManagerProto : public Singleton<HttpManagerProto>
{
public:
	HttpManagerProto();
	~HttpManagerProto();

public:
	int DoHttpPost(CurlData* pdata);
	int DoHttpGet(CurlData* pdata);
	int DoHttpHeader(CurlData* pdata);

	bool CurlGlobalInit();
	void CurlGlobalCleanup();
private:
	CURL* CurlInit();

	static int ProgressFunction(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	
	static size_t WriteFunction( void* ptr, size_t size, size_t nmemb, void *usrptr );


	DECLARE_SINGLETON_CLASS(HttpManagerProto);

};


#endif//__HTTP_MANAGER_H__