#include "stdafx.h"
#include "HttpManager.h"
#include "../YApp.h"

HttpManagerProto::HttpManagerProto()
{
//	CurlGlobalInit();
}

HttpManagerProto::~HttpManagerProto()
{
//	CurlGlobalCleanup();
}

// call before you use this class.
bool HttpManagerProto::CurlGlobalInit()
{
	CURLcode res = ::curl_global_init( CURL_GLOBAL_ALL );
	if( CURLE_OK != res ) 
	{
		//log here
		sLogError("HttpManagerProto::CurlGlobalInit");
		return false;
	}

	curl_version_info_data* p_version = curl_version_info(CURLVERSION_NOW);
	if (p_version)
	{
#ifdef _CONSOLE
		fprintf(stderr,"\n[libcurl version:%s]\n[host:%s]|\n ",p_version->version,
			p_version->host);
#endif
	}
	return true;
}

void HttpManagerProto::CurlGlobalCleanup()
{
	::curl_global_cleanup();
}

CURL* HttpManagerProto::CurlInit()
{
	CURL* curl = ::curl_easy_init();
	if(NULL == curl)
	{
		//log here
		sLogError("HttpManagerProto::CurlInit");
	}
	return curl;
}

int HttpManagerProto::DoHttpPost(CurlData* pdata)
{
	assert(pdata);
	CURL* curl = CurlInit();
	if( NULL == curl )
		return down_http_file::dr_exit;
	//这里设置要不要返回头部, 自己看着办.
	::curl_easy_setopt( curl, CURLOPT_HEADER, false );
	::curl_easy_setopt( curl, CURLOPT_URL, pdata->url );
	::curl_easy_setopt( curl, CURLOPT_POST, 1 );
	::curl_easy_setopt( curl, CURLOPT_POSTFIELDS, pdata->data );
	::curl_easy_setopt( curl, CURLOPT_TIMEOUT, 10 );
	::curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, HttpManagerProto::WriteFunction); 
	::curl_easy_setopt( curl, CURLOPT_WRITEDATA, pdata); 
	::curl_easy_setopt( curl, CURLOPT_NOPROGRESS, !(pdata->bshowprogress));//是否开启进度监控
	::curl_easy_setopt( curl, CURLOPT_PROGRESSFUNCTION, HttpManagerProto::ProgressFunction);
	::curl_easy_setopt( curl, CURLOPT_PROGRESSDATA,pdata);
	if (pdata->bwritefile)
	{
		pdata->fp = fopen(pdata->filename,"wb");
		assert(pdata->fp);
		if(pdata->fp == NULL)
			return down_http_file::dr_open_file_failed;
	}
	CURLcode Code = ::curl_easy_perform( curl);
	if(CURLE_OK != Code)
	{
		sLogError("DoHttpPost Failed",pdata->url);
		return down_http_file::dr_open_url_failed;
	}
	long retcode = 0;
	int nReturn = down_http_file::dr_read_file_failed;

	Code = ::curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode); 
	if ( (Code == CURLE_OK) && retcode == 200 )
	{
		nReturn = down_http_file::dr_success;
	}
	//写入到文件
	if(pdata->bwritefile&&pdata->fp)
	{
		fseek(pdata->fp,SEEK_END,0);
		size_t return_size = fwrite(pdata->buffer, pdata->buffer_len, 1, pdata->fp); 	
	}
	::curl_easy_cleanup(curl);
	//如果打开了文件，关闭文件
	if(pdata->bwritefile&&pdata->fp)
		fclose(pdata->fp);

	return nReturn;
}

int HttpManagerProto::DoHttpGet(CurlData* pdata)
{
	assert(pdata);
	CURL* curl = CurlInit();
	if( NULL == curl )
		return down_http_file::dr_exit;
	::curl_easy_setopt( curl, CURLOPT_URL, pdata->url );
	::curl_easy_setopt( curl, CURLOPT_HTTPGET, 1 );
	::curl_easy_setopt( curl, CURLOPT_HEADER, false );
	::curl_easy_setopt( curl, CURLOPT_TIMEOUT, 10 );
	::curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, HttpManagerProto::WriteFunction); 
	::curl_easy_setopt( curl, CURLOPT_WRITEDATA, pdata); 
	::curl_easy_setopt( curl, CURLOPT_NOPROGRESS, !(pdata->bshowprogress));//是否开启进度监控
	::curl_easy_setopt( curl, CURLOPT_PROGRESSFUNCTION, HttpManagerProto::ProgressFunction);
	::curl_easy_setopt( curl, CURLOPT_PROGRESSDATA,pdata);
	//是否写入到文件
	if (pdata->bwritefile)
	{
		pdata->fp = fopen(pdata->filename,"wb");
		assert(pdata->fp);
		if(pdata->fp==NULL)
			return down_http_file::dr_open_file_failed;
	}
	CURLcode Code = ::curl_easy_perform( curl);
	if(CURLE_OK!=Code)
	{
		sLogError("DoHttpGet Failed",pdata->url);
		return down_http_file::dr_open_url_failed;
	}
	long retcode = 0;
	int nReturn = down_http_file::dr_read_file_failed;

	Code = ::curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode); 
	if ( (Code == CURLE_OK) && retcode == 200 )
	{
		nReturn = down_http_file::dr_success;
	}
	//写入到文件
	if(pdata->bwritefile&&pdata->fp)
	{
		fseek(pdata->fp,SEEK_END,0);
		size_t return_size = fwrite(pdata->buffer, pdata->buffer_len, 1, pdata->fp); 	
	}
	::curl_easy_cleanup(curl);
	//如果打开了文件，关闭文件
	if(pdata->bwritefile&&pdata->fp)
		fclose(pdata->fp);

	return nReturn;
}

int HttpManagerProto::DoHttpHeader(CurlData* pdata)
{
	assert(pdata);
	CURL* curl = CurlInit();
	::curl_easy_setopt( curl, CURLOPT_URL, pdata->url );
	::curl_easy_setopt( curl, CURLOPT_NOBODY, true );
	::curl_easy_setopt( curl, CURLOPT_HEADER, true );      //下载数据包括HTTP头部
//	::curl_easy_setopt( curl, CURLOPT_RETURNTRANSFER,)
	::curl_easy_setopt( curl, CURLOPT_AUTOREFERER, true );
	::curl_easy_setopt( curl, CURLOPT_TIMEOUT, 10 );       //设置超时
//	::curl_easy_setopt( curl, CURLOPT_NOSIGNAL, 1);        //屏蔽其它信号
//	::curl_easy_setopt( curl, CURLOPT_RANGE, "0-500");     //用于断点续传, 设置下载的分片
// 	CURLOPT_NOPROGRESS  
// 		为了使CURLOPT_PROGRESSFUNCTION被调用. CURLOPT_NOPROGRESS必须被设置为false.
// 		CURLOPT_PROGRESSFUNCTION
// 		CURLOPT_PROGRESSFUNCTION 指定的函数正常情况下每秒被libcurl调用一次.
// 		CURLOPT_PROGRESSDATA
// 		CURLOPT_PROGRESSDATA指定的参数将作为CURLOPT_PROGRESSFUNCTION指定函数的参数.

	return 0;
}

size_t HttpManagerProto::WriteFunction( void* ptr, size_t size, size_t nmemb, void *usrptr )
{
	
	CurlData* pdata = static_cast<CurlData*>(usrptr);
	
	char *newbuff;
	size_t rembuff;

	size *= nmemb;

	rembuff = pdata->buffer_len - pdata->buffer_pos; /* remaining space in buffer */

	if(size > rembuff) 
	{
		/* not enough space in buffer */
		newbuff = static_cast<char*>(::realloc(pdata->buffer,pdata->buffer_len + (size - rembuff) + 1) );
		if(newbuff == NULL) 
		{
			//log here
			sLogError("HttpManagerProto::WriteFunction relloc error");
			size = rembuff;
		}
		else 
		{
			/* realloc suceeded increase buffer size*/
			pdata->buffer_len += size - rembuff;
			pdata->buffer = newbuff;
		}
	}

	memcpy(&pdata->buffer[pdata->buffer_pos], ptr, size);
	pdata->buffer_pos += size;
	pdata->buffer[pdata->buffer_pos] = '\0';
	
	return size;
}

//---------------------------------------------------------------------------------------------------------------
// dltotal : 
// dlnow : 
// ultotal : 
int HttpManagerProto::ProgressFunction(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	CurlData* pdata = static_cast<CurlData*>(clientp);
	if(!pdata)
		return 0;
	pdata->dltotal = dltotal;
	pdata->dlnow = dlnow;
	pdata->ultotal = ultotal;
	pdata->ulnow = ulnow;

	if(pdata->h_notify_wnd&&pdata->n_notify_msg>0)
		::PostMessage(pdata->h_notify_wnd,pdata->n_notify_msg,(WPARAM)pdata,WP_LOADING);

	return 0;
}