#include "stdafx.h"
#include "HttpBase.h"
#include "../YApp.h"

CHttpBase::CHttpBase()
{
	Init();
}
CHttpBase::~CHttpBase()
{
	Clear();
}
void CHttpBase::Init()
{
	m_url = NULL;
	
	CURLcode res = ::curl_global_init( CURL_GLOBAL_ALL );
	if(CURLE_OK != res) sLogError("CHttpBase::Init<::curl_global_init:%d>",res),ASSERT( FALSE ) ;
	m_hWndOwner = NULL;
	m_url = ::curl_easy_init();
	if(NULL == m_url)
		sLogError("CHttpBase::Init<::curl_easy_init>"),ASSERT(FALSE);
	sLog("CHttpBase::Init");
}
void CHttpBase::Clear()
{
	if(m_url)
	{
		//ÊÍ·Å×ÊÔ´
		curl_easy_cleanup(m_url);
		m_url = NULL;
	}
	if(m_data)
	{
		m_data->Clear();
		delete m_data;
		m_data = NULL;
	}
}

int CHttpBase::MakeCurlData(CurlData* pdata,HWND hWndOwner)
{
	ASSERT(pdata);
	m_hWndOwner = hWndOwner;
	m_data = pdata;
	m_data->hWnd = hWndOwner;
	return 1;
}

int CHttpBase::DoGet()
{
	ASSERT(m_url&&m_data);
	::curl_easy_setopt( m_url, CURLOPT_URL, m_data->url );
	::curl_easy_setopt( m_url, CURLOPT_HTTPGET, 1 );
	::curl_easy_setopt( m_url, CURLOPT_HEADER, false );
	::curl_easy_setopt( m_url, CURLOPT_TIMEOUT, 10 );
	::curl_easy_setopt( m_url, CURLOPT_WRITEFUNCTION, CHttpBase::WriteFunction); 
	::curl_easy_setopt( m_url, CURLOPT_WRITEDATA, m_data); 
	if(m_data->bshowprogress)
	{
		::curl_easy_setopt(m_url,CURLOPT_NOPROGRESS,0);
		::curl_easy_setopt(m_url,CURLOPT_PROGRESSFUNCTION,CHttpBase::ProgressFunction);
		::curl_easy_setopt(m_url,CURLOPT_PROGRESSDATA,m_data);
	}
	CURLcode Code = ::curl_easy_perform( m_url);
	if(CURLE_OK!=Code)
	{
		sLogError("Failed to DoGet <errcode:%d>",Code);
		return -1;
	}
	sLog("Success run DoGet:<code:%d>",Code);
	long retcode = 0;
	Code = ::curl_easy_getinfo(m_url, CURLINFO_RESPONSE_CODE , &retcode); 
	if ( (Code == CURLE_OK) && retcode == 200 )
		return 0;
	return 1;
}

int CHttpBase::DoPost()
{
	ASSERT(m_url&&m_data);
	::curl_easy_setopt( m_url, CURLOPT_HEADER, false );
	::curl_easy_setopt( m_url, CURLOPT_URL, m_data->url );
	::curl_easy_setopt( m_url, CURLOPT_POST, 1 );
	::curl_easy_setopt( m_url, CURLOPT_POSTFIELDS, m_data->data );
	::curl_easy_setopt( m_url, CURLOPT_TIMEOUT, 10 );
	::curl_easy_setopt( m_url, CURLOPT_WRITEFUNCTION, CHttpBase::WriteFunction); 
	::curl_easy_setopt( m_url, CURLOPT_WRITEDATA, m_data); 
	if(m_data->bshowprogress)
	{
		::curl_easy_setopt(m_url,CURLOPT_NOPROGRESS,0);
		::curl_easy_setopt(m_url,CURLOPT_PROGRESSFUNCTION,CHttpBase::ProgressFunction);
		::curl_easy_setopt(m_url,CURLOPT_PROGRESSDATA,m_data);
	}
	CURLcode Code = ::curl_easy_perform( m_url);
	if(CURLE_OK != Code)
	{
		sLogError("Failed to DoPost <errcode:%d>",Code);
		return -1;
	}
	sLog("Success run DoPost:<code:%d>",Code);
	long retcode = 0;
	Code = ::curl_easy_getinfo(m_url, CURLINFO_RESPONSE_CODE , &retcode); 
	if ( (Code == CURLE_OK) && retcode == 200 )
		return 0;
	return 1;
}

size_t CHttpBase::WriteFunction( void* ptr, size_t size, size_t nmemb, void *usrptr )
{
	CurlData* pdata = static_cast<CurlData*>(usrptr);

	char *newbuff;
	size_t rembuff;

	size *= nmemb;

	rembuff=pdata->buffer_len - pdata->buffer_pos; /* remaining space in buffer */

	if(size > rembuff) 
	{
		/* not enough space in buffer */
		newbuff = static_cast<char*>(::realloc(pdata->buffer,pdata->buffer_len + (size - rembuff) + 1) );
		if(newbuff==NULL) 
		{
			//log here
			size=rembuff;
		}
		else 
		{
			/* realloc suceeded increase buffer size*/
			pdata->buffer_len+=size - rembuff;
			pdata->buffer=newbuff;
		}
	}

	memcpy(&pdata->buffer[pdata->buffer_pos], ptr, size);
	pdata->buffer_pos += size;
	pdata->buffer[pdata->buffer_pos] = '\0';

	return size;
}

int CHttpBase::ProgressFunction(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	CurlData* pdata = static_cast<CurlData*>(clientp);
	pdata->dltotal = dltotal;
	pdata->dlnow = dlnow;
	pdata->ultotal = ultotal;
	pdata->ulnow = ulnow;

	if(pdata->hWnd)
		::PostMessage(pdata->hWnd,WM_WEB_SERVICE,(WPARAM)pdata,0);
	return 0;
}