#ifndef _HTTPprotocol_h
#define _HTTPprotocol_h
#pragma once
#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
typedef std::string String;
typedef int(*ProgressCallback)(void* ptr, double rDlTotal, double rDlNow, double rUlTotal, double rUlNow);

class CHTTPprotocol
{
public:
	CHTTPprotocol();
	virtual ~CHTTPprotocol();

	bool Initialize(const char * AszUrlBase);

	bool CreateCurl(void);
	void ReleaseCurl(void);

	int DownloadFile(const char * AszFileName, const char * AszTempFilePath,ProgressCallback progessProc = NULL,void* usrdata = NULL);

	bool sslpost(String ulr,const char* _Content);

	void setBaseUrl(String &str){ m_strBaseUrl = str; }
	String getBaseUrl(void){ return m_strBaseUrl; }
	long getDownloadFileLenth(String &url);
	long getLocalFileLenth(String &localfile);
	long getDownloadFileLenth();
	void Pause();
	void Resume();
protected:
	void SpaceTransformation(String& str);//将字符串中的空格转换为HTTP可识别字符
	//static size_t WriteData2File(void *ptr, size_t size, size_t nmemb, void *stream);
private:
	String		m_strBaseUrl;
	FILE		* m_pLog;
	CURL * m_pCurl;
	long  m_totalSize;
};

#endif//_HTTPprotocol_h