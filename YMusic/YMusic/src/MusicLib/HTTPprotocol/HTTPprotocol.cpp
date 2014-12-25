/************************************************************************/
/*                                                                      */
/************************************************************************/
#include "stdafx.h"
#include "httpprotocol.h"


CHTTPprotocol::CHTTPprotocol()
	:m_pLog(NULL)
	,m_pCurl(NULL)
	,m_totalSize(0)
{
	m_strBaseUrl.clear();
}

CHTTPprotocol::~CHTTPprotocol() {

	if (m_pLog) {
		fclose(m_pLog);
	}
	m_pLog = NULL;
}

bool CHTTPprotocol::Initialize(const char * AszUrlBase) {
	if (!AszUrlBase) return false;

	m_strBaseUrl = AszUrlBase;
	return true;
}

void CHTTPprotocol::SpaceTransformation( String& str )
{
	const char* _sReplace = "%20";
	const char* _sFind = " "; 
	int len = strlen(_sFind);
	while(true)
	{
		int pos = str.find(_sFind);
		if (pos == -1)
		{
			return;
		}
		else
		{
			str.replace(pos,len,_sReplace);
		}
	}
}

bool CHTTPprotocol::CreateCurl( void )
{
//	curl_global_init(CURL_GLOBAL_DEFAULT);
	m_pCurl = curl_easy_init();

	return (m_pCurl != NULL);
}

void CHTTPprotocol::ReleaseCurl( void )
{
	if (m_pCurl)
	{
		// 清空;
		curl_easy_cleanup(m_pCurl);
//		curl_global_cleanup();
	}
}

static size_t WriteData2File(void *ptr, size_t size, size_t nmemb, void *stream) {
	return fwrite( ptr, size, size * nmemb, (FILE*)stream );
}

// static int LoadProgress(void* ptr, double rDlTotal, double rDlNow, double rUlTotal, double rUlNow)
// {
// 
// 	double percent = 100.0 * (rDlNow/rDlTotal); 
// 	std::cout<<(const char*)ptr<<" Loaded:"<<(percent)<<"%"<<std::endl;
// 	if (percent >= 100.0)
// 	{
// 		std::cout<<(const char*)ptr<<" Finished"<<std::endl;
// 	}
// 	return 0; 
// }

//成功返回0，失败返回1
int CHTTPprotocol::DownloadFile(const char * AszFileName, const char * AszTempFilePath,ProgressCallback progessProc,void* usrdata) {

	if (!AszTempFilePath || !AszFileName)
		return 0;

	String strUrlFile = AszFileName;
	String strTempFile = AszTempFilePath;
	SpaceTransformation(strUrlFile);

	FILE	* kpWriteFile = NULL;
	if((kpWriteFile = fopen(strTempFile.c_str(),"wb+")) == NULL)//※二进制
	{
		if((kpWriteFile=fopen(strTempFile.c_str(),"wb+")) == NULL)
		{
			return 1;
		}
	}

	do 
	{
		CURLcode urlCode ;
		curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrlFile.c_str());
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, kpWriteFile);
		curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, &WriteData2File );
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, false);
		curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, 60);
		if(progessProc)
		{
			//curl的进度条声明 
			curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, FALSE); 
			//回调进度条函数 
			curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSFUNCTION, progessProc); 
		}
		if(usrdata)
		{
			//设置进度条用户数据 
			curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSDATA, usrdata); 
		}
		// "a-b"
//		curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM_LARGE, getLocalFileLenth(strTempFile)); //断点续传
		urlCode = curl_easy_perform( m_pCurl );
		if ( urlCode != CURLE_OK )
		{
			fclose(kpWriteFile);
			fflush(kpWriteFile);
			kpWriteFile = NULL;
			return 1;
		}

		long retcode = 0;
		urlCode = ::curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE , &retcode); 
		if ( (urlCode != CURLE_OK) || retcode != 200 )
		{
			fclose(kpWriteFile);
			fflush(kpWriteFile);
			kpWriteFile = NULL;
			return 1;
		}
	} while (0);

	fclose(kpWriteFile);
	fflush(kpWriteFile);
	kpWriteFile = NULL;
	return 0;
}


bool CHTTPprotocol::sslpost( String ulr,const char* _Content )
{
	if (!_Content)
	{
		return false;
	}
	curl_easy_setopt(m_pCurl, CURLOPT_URL, ulr.c_str());
	curl_easy_setopt(m_pCurl, CURLOPT_POST, true);
	curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, _Content);
	curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, strlen(_Content));
	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 30);
	curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, 60);
	curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, true);
	CURLcode urlCode = curl_easy_perform(m_pCurl);

	if (urlCode != CURLE_OK)
	{
		return false;
	}

	long httpCode = 0;
	urlCode = curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &httpCode);
	if(CURLE_OK != urlCode || httpCode != 200)
	{
		return false;
	}
	else
	{
		//执行后续操作
	}
	return true;
}

long CHTTPprotocol::getDownloadFileLenth(String &url)
{
	long long downSize = 0;
	CURLcode urlCode;
	CURL* handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_TIMEOUT, 100);
	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(handle, CURLOPT_HEADER, 1);    //只需要header头
	curl_easy_setopt(handle, CURLOPT_NOBODY, 1);    //不需要body
	curl_easy_setopt(handle, CURLOPT_ENCODING,"gzip");
	urlCode = curl_easy_perform(handle);
	long retcode = 0;
	urlCode = ::curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE , &retcode); 
	if ( (urlCode != CURLE_OK) || retcode != 200 ){
		downSize = -1;
	}
	else{
		curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downSize);
	}
	curl_easy_cleanup(handle);

	return downSize;
}
long CHTTPprotocol::getLocalFileLenth(String &localfile)
{
	long localFileLenth = 0;
	FILE * fp = fopen(localfile.c_str(),"r"); 
	if( NULL == fp )
		return localFileLenth;
	fseek(fp,0,SEEK_END);
	localFileLenth = ftell(fp);
	fclose(fp);
	return localFileLenth;
}
long CHTTPprotocol::getDownloadFileLenth()
{
	return m_totalSize;
}

void CHTTPprotocol::Pause()
{
	if(m_pCurl)
	{
		curl_easy_pause(m_pCurl,CURLPAUSE_RECV);
	}
}
void CHTTPprotocol::Resume()
{
	if (m_pCurl)
	{
		curl_easy_pause(m_pCurl,CURLPAUSE_RECV_CONT);
	}
}