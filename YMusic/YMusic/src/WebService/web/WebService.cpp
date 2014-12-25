#include "stdafx.h"
#include "WebService.h"
#include "Link.h"
#include "../YApp.h"
#include "WebServiceCatch.h"
#include "ResponseParser.h"

#include <cassert>
#include "zlib.h"


int ungzip(char *source,int len,char **dest,int gzip)
{  
#define   CHUNK   16384 

	int ret;  
	unsigned have;  
	z_stream strm;  
	unsigned char out[CHUNK] = {0};  
	int totalsize = 0;  

	/* allocate inflate state */  
	strm.zalloc = Z_NULL;  
	strm.zfree = Z_NULL;  
	strm.opaque = Z_NULL;  
	strm.avail_in = 0;  
	strm.next_in = Z_NULL;  

	if(gzip)  
		ret = inflateInit2(&strm, 47);  
	else  
		ret = inflateInit(&strm);  

	if (ret != Z_OK)  
		return ret;  

	strm.avail_in = len;  
	strm.next_in = (Bytef*)source;  

	/* run inflate() on input until output buffer not full */  
	do {  
		strm.avail_out = CHUNK;  
		strm.next_out = out;  
		ret = inflate(&strm, Z_NO_FLUSH);  
		//		assert(ret != Z_STREAM_ERROR); /* state not clobbered */  
		switch (ret) 
		{  
		case Z_NEED_DICT:  
			ret = Z_DATA_ERROR; /* and fall through */  
		case Z_DATA_ERROR:  
		case Z_MEM_ERROR:  
			inflateEnd(&strm);  
			return ret;  
		}  
		have = CHUNK - strm.avail_out;  
		totalsize += have;  
		*dest = (char *)realloc(*dest,totalsize);  
		memcpy(*dest + totalsize - have,out,have);  
	} while (strm.avail_out == 0);  

	/* clean up and return */  
	(void)inflateEnd(&strm);  
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;  
}

int ungzip2(char *source,int len,int gzip,CBuffer& buffer)
{  
	static const size_t chunk_size = 4096;

	int ret;  
	unsigned have;  
	z_stream strm;  
	unsigned char out[chunk_size] = {0};  

	/* allocate inflate state */  
	strm.zalloc = Z_NULL;  
	strm.zfree = Z_NULL;  
	strm.opaque = Z_NULL;  
	strm.avail_in = 0;  
	strm.next_in = Z_NULL;  

	if(gzip)  
		ret = inflateInit2(&strm, 47);  
	else  
		ret = inflateInit(&strm);  

	if (ret != Z_OK)  
		return -1;  

	strm.avail_in = len;  
	strm.next_in = (Bytef*)source;  

	do {  
		strm.avail_out = chunk_size;  
		strm.next_out = out;  
		ret = inflate(&strm, Z_NO_FLUSH);  

		switch (ret) 
		{  
		case Z_NEED_DICT:  
			ret = Z_DATA_ERROR; /* and fall through */  
		case Z_DATA_ERROR:  
		case Z_MEM_ERROR:  
			inflateEnd(&strm);  
			return -1;  
		}  
		have = chunk_size - strm.avail_out;  
		buffer.Write(out,have);
	} while (Z_STREAM_END != ret);  

	/* clean up and return */  
	(void)inflateEnd(&strm);  
	return ret == Z_STREAM_END ? 0 : -1;
}

struct	RequestInfo {
	static const size_t MAX_RETRY_CNT = 2;
	enum{RT_GET=1,RT_POST}opt;
	url::CRequest* rq;
	bool bHeaderParsed;
	bool bGZip;
	bool bChunked;
	int  nContentLen;
	int  nError;
	int  errCnt;

	struct RequestInfo()
	{
		rq  = NULL;
		bHeaderParsed = false;
		bGZip = false;
		bChunked = false;
		nContentLen = 0;
		nError = 200;
		errCnt = 0;
		opt    = RT_GET;
	}
	bool ParseResponseHeader(CBuffer& buffer,bool &bGZip,bool &bChunked,int &nContentLen,int& nError)
	{
		const int pos = buffer.find((uint8*)("\r\n\r\n"),4);
		if(-1 == pos) return false;

		static const char HTTP_OK_FLAG[]   ="200 OK";
		static const char GZIP[]           ="gzip";
		static const char CONTENT_ENCOD[]  ="Content-Encoding:";
		static const char CHUNKED[]        ="chunked";
		static const char TRANSFER_ENCOD[] ="Transfer-Encoding:";
		static const char CONTENT_LENGTH[] ="Content-Length: ";

		if(-1 == buffer.find((uint8*)HTTP_OK_FLAG,strlen(HTTP_OK_FLAG)))
		{
			*(buffer[pos]) = '\0';
			if(0 == sscanf_s((char*)(buffer)[buffer.rpos()],"HTTP/1.1 %d",&nError))
				sscanf_s((char*)(buffer)[buffer.rpos()],"HTTP/1.0 %d",&nError);

			assert(200 == nError);
			assert(false);
			return false;
		}

		bGZip = 
			(-1 != buffer.find((uint8*)GZIP,strlen(GZIP)) &&
			-1 != buffer.find((uint8*)CONTENT_ENCOD,strlen(CONTENT_ENCOD)));

		bChunked =
			(-1 != buffer.find((uint8*)CHUNKED,strlen(CHUNKED)) &&
			-1 != buffer.find((uint8*)TRANSFER_ENCOD,strlen(TRANSFER_ENCOD)));


		nContentLen = -1;
		int i = buffer.find((uint8*)CONTENT_LENGTH,strlen(CONTENT_LENGTH));
		if(-1 != i)
		{
			*(buffer[pos]) = '\0';
			sscanf_s((char*)(buffer)[i+strlen(CONTENT_LENGTH)],"%d\r\n",&nContentLen);
		}

		//	assert(0 != nContentLen);
		buffer.rpos(pos+4);
	//	assert('{' == *(char*)buffer[buffer.rpos()] && !bChunked);
		return true;
	}
	bool parseHttpResponse(CBuffer& buffer,bool& haserr)
	{
		assert(!buffer.empty());
		if(buffer.empty())
			return false;
		
		haserr = false;
		if(!bHeaderParsed)
			bHeaderParsed = ParseResponseHeader(buffer,bGZip,bChunked,nContentLen,nError);

		if(!bHeaderParsed)
			return false;

		if(200 != nError)
			return true;

		bool isEnd = false;
		if(!bChunked)
		{
			assert(nContentLen > 0);
			isEnd = ((int)buffer.cnt() >= nContentLen);
		}
		else
		{
			static char endChunked[] = "\r\n0\r\n\r\n";
			static size_t endChunkedLen = strlen(endChunked);
			if(buffer.cnt() >= strlen(endChunked))
				isEnd = (0 == memcmp(endChunked,(char*)buffer[buffer.rpos() + buffer.cnt() - strlen(endChunked)],strlen(endChunked)));
		}

		if(isEnd)
		{
			if(!bChunked)
			{
				if(bGZip)
				{
// 					char* p_buf   = NULL;
// 					char* p_inbuf = (char*)(buffer[buffer.rpos()]);
// 					int nResult = ungzip(p_inbuf, buffer.cnt(), &p_buf, 1);
// 					assert(Z_OK == nResult);
// 					if(Z_OK == nResult)
// 					{
// 						//gzip有10个字节的头部和8个字节的尾部
// 
// 						std::string sBuf(p_buf);
// 						sBuf.erase(sBuf.rfind('}') + 1);
// 						buffer.clear();
// 						//_pBuffer->Write((uint8*)p_buf,strlen(p_buf)-18); 
// 						buffer<<sBuf;
// 						free(p_buf);
// 					}

					CBuffer outBuffer;
					if(0 == ungzip2((char*)(buffer[buffer.rpos()]),buffer.cnt(),1,outBuffer))
					{
						outBuffer.swap(buffer);
					}
					else
					{
						assert(false);
					}

				}
			}
			else
			{
				CBuffer thebuffer;
				while(!buffer.empty())
				{
					const char* pStart = (const char*)buffer[buffer.rpos()];
					char* pEnd = NULL;
					int datasize = strtol(pStart,&pEnd,16);
					if(0 == datasize && pEnd == pStart)
					{
						haserr = true;
						break;
					}
					if(0 > datasize)
					{
						haserr = true;
						break;
					}

					if(datasize > (int)buffer.cnt() -2)
					{
						haserr = true;
						break;
					}

					buffer.rpos(buffer.rpos() + (pEnd - pStart) + 2);
					if(datasize > 0)
						thebuffer.Write(buffer[buffer.rpos()],datasize);
					buffer.rpos(buffer.rpos() + datasize + 2);
					if(0 >= datasize)
						break;
				}
				assert(!haserr);
				buffer.swap(thebuffer);
			}	
		}

		return isEnd;
	}
};
/************************************************************************/
/*                                                                      */
/************************************************************************/
CWebServiceMgr::CWebServiceMgr():_nMsg(0),_isOnline(true)
{
#if defined (_DEBUG) || defined (_XIAMI_BETA)
	_serversIp.push_back(std::string("dev.xiami.com"));
//	_serversIp.push_back(std::string("www.xiami.com"));
#else
	_serversIp.push_back(std::string("www.xiami.com"));
#endif // _DEBUG

	_serversIp.push_back(std::string("122.225.11.137"));
// 	WORD wVersionRequested;
//     WSADATA wsaData;
//     memset(&wsaData,0,sizeof(wsaData));
//     wVersionRequested = MAKEWORD( 2, 1 );
//     WSAStartup( wVersionRequested, &wsaData );

	theNet->init(1,1);
	theNet->asynrun();
}

CWebServiceMgr::~CWebServiceMgr()
{
}

std::string CWebServiceMgr::BuildHttpHeader(lpRequest pRequest,bool bPost/* = false*/)
{
	assert(pRequest);
	assert(!_serversIp.empty());

	const std::string paths(pRequest->GetPaths());
	const std::string params(pRequest->GetParams());
	std::string strRequest;

	if (!bPost)
	{
		strRequest ="GET http://";
		strRequest += _serversIp[0].c_str();
	    strRequest += paths;
		if(!params.empty())
		{
			strRequest += "?";
			strRequest += params;
		}
		
		strRequest += " HTTP/1.1\r\n"
					  "Accept: */*\r\n"
					  "Accept-Language: zh-cn\r\n"
					  "Accept-Encoding: gzip, deflate\r\n"
					  "Accept-Charset: utf8\r\n"
					  "User-Agent: yting(windows) v0.0.0.1\r\n";

		spUserContextT pContext = theRuntimeState->GetCurrentContext(NULL);
		spLogonUserInfoT theUser = pContext->GetUser();
		if(theUser)
		{
			CString str;
			str.Format(_T("%s:%s"),theUser->sLogonEmail,theUser->sPasswordMD5);
			std::string ansiStr = em_utility::mci::unicode_2_ansi(str);
			if(!ansiStr.empty())
			{
				std::string base64Str = ZBase64::Encode((const unsigned char*)ansiStr.c_str(),ansiStr.size());
				if(!base64Str.empty())
				{
					strRequest += "Authorization:Basic ";
					strRequest += base64Str;
					strRequest += "\r\n";
				}
			}
		}
		strRequest +=   "Host: "
			"www.xiami.com"
			"\r\n"
			"Connection: keep-alive\r\n"
			"Pragma: no-cache\r\n"
			"Cache-Control: no-cache\r\n\r\n";
	}
	else
	{
		strRequest  = "POST ";
		strRequest += paths;
		strRequest += " HTTP/1.1\r\n"
					  "Accept: */*\r\n"
					  "Accept-Language: zh-cn\r\n"
					  //"Accept-Encoding: gzip, deflate\r\n"
					  "Accept-Charset: utf8\r\n"
					  "User-Agent: yting(windows) v0.0.0.1\r\n";

		spUserContextT pContext = theRuntimeState->GetCurrentContext(NULL);
		spLogonUserInfoT theUser = pContext->GetUser();
		if(theUser)
		{
			CString str;
			str.Format(_T("%s:%s"),theUser->sLogonEmail,theUser->sPasswordMD5);
			std::string ansiStr = em_utility::mci::unicode_2_ansi(str);
			if(!ansiStr.empty())
			{
				std::string base64Str = ZBase64::Encode((const unsigned char*)ansiStr.c_str(),ansiStr.size());
				if(!base64Str.empty())
				{
					strRequest += "Authorization:Basic ";
					strRequest += base64Str;
					strRequest += "\r\n";
				}
			}
		}


		assert(!params.empty());
		char szbuf[64] = {0};
		sprintf(szbuf,"%u",params.size());
		strRequest +=  "Content-Length:";
		strRequest +=  szbuf;
		strRequest +=  "\r\n";
		strRequest += "Content-Type: application/x-www-form-urlencoded\r\n";
		strRequest +=   "Host: "
			"dev.xiami.com"
			"\r\n"
			"Connection: keep-alive\r\n"
			"Pragma: no-cache\r\n"
			"Cache-Control: no-cache\r\n\r\n";
		strRequest += params;
	}
	
	return strRequest;
}

void CWebServiceMgr::ClearRequest()
{
	//_pOps->cancle();
}
void CWebServiceMgr::AsynSendRequest(lpRequest pRequest)
{
	assert(pRequest);
	onAsynRequest(pRequest);
}
void CWebServiceMgr::AsynSendRequest(HWND hwnd,lpRequest pRequest,DWORD dwTag)
{
	assert(pRequest);
	pRequest->SetHwnd(hwnd);
	pRequest->SetTag(dwTag);

	return AsynSendRequest(pRequest);
}
void CWebServiceMgr::AsynSendRequest(HWND hwnd,lpRequest pRequest)
{
	assert(pRequest);
	pRequest->SetHwnd(hwnd);

	return AsynSendRequest(pRequest);
}

void CWebServiceMgr::AsynSendRequest(lpRequest pRequest,ResponderT responder,DWORD dwTag)
{
	assert(pRequest);
	pRequest->SetReponder(responder);
	pRequest->SetTag(dwTag);

	return AsynSendRequest(pRequest);
}
void CWebServiceMgr::AsynSendRequest(lpRequest pRequest,ResponderT responder)
{
	assert(pRequest);
	pRequest->SetReponder(responder);

	return AsynSendRequest(pRequest);
}
void CWebServiceMgr::SendRequest(lpRequest pRequest)
{
	assert(pRequest);
	return OnRequest(pRequest);
}
void CWebServiceMgr::SendRequest(HWND hwnd,lpRequest pRequest,DWORD dwTag)
{
	assert(pRequest);
	pRequest->SetHwnd(hwnd);
	pRequest->SetTag(dwTag);

	return SendRequest(pRequest);
}
void CWebServiceMgr::SendRequest(HWND hwnd,lpRequest pRequest)
{
	assert(pRequest);
	pRequest->SetHwnd(hwnd);

	return SendRequest(pRequest);
}
void CWebServiceMgr::SendRequest(lpRequest pRequest,ResponderT responder,DWORD dwTag)
{
	assert(pRequest);
	pRequest->SetReponder(responder);
	pRequest->SetTag(dwTag);

	return SendRequest(pRequest);
}
void CWebServiceMgr::SendRequest(lpRequest pRequest,ResponderT responder)
{
	assert(pRequest);
	pRequest->SetReponder(responder);

	return SendRequest(pRequest);
}

void CWebServiceMgr::SendRequest(lpRequest pRequest,CBuffer& buffer)
{
	assert(pRequest);
	if(!pRequest) return;
	const std::string requestHeaderStr = BuildHttpHeader(pRequest);
	RequestInfo info;
	info.rq	= pRequest;
	hLinker h = theNet->createLinker(NULL);
	assert(h);
	do 
	{
		buffer.clear();
		theNet->closeLinker(h);
		if(theNet->connect(h,_serversIp[info.errCnt%_serversIp.size()].c_str(),"http",10) &&
			(requestHeaderStr.size() == theNet->send(h,requestHeaderStr.c_str(),requestHeaderStr.size())))
		{
			while(theNet->waitReadable(h,10))
			{
				if(buffer.full())
					buffer.resize();
				size_t bytes = theNet->recv(h,buffer[buffer.wpos()],buffer.nextwriteblocksize());
				if(bytes > 0)
				{
					buffer.wpos(buffer.wpos() + bytes);
					bool haserr = false;
					if(info.parseHttpResponse(buffer,haserr))
					{
						if(haserr || 200 != info.nError)
						{
							info.rq->SetTimeOut(true);
							buffer.clear();
						}
						
						theNet->destroyLinker(h);
						delete pRequest;
						pRequest = NULL;
						return;
					}
				}
			}
		}
	} while (info.errCnt++ <  RequestInfo::MAX_RETRY_CNT);

	assert(false);
	info.rq->SetTimeOut(true);
	buffer.clear();
	theNet->destroyLinker(h);
	delete pRequest;
	pRequest = NULL;
	return;
}


void CWebServiceMgr::PostRequest(lpRequest pRequest)
{
	assert(pRequest);
	OnRequest(pRequest,true);
}
void CWebServiceMgr::PostRequest(HWND hwnd,lpRequest pRequest,DWORD dwTag)
{
	assert(pRequest);
	pRequest->SetHwnd(hwnd);
	pRequest->SetTag(dwTag);

	return PostRequest(pRequest);
}		
void CWebServiceMgr::PostRequest(HWND hwnd,lpRequest pRequest)
{
	assert(pRequest);
	pRequest->SetHwnd(hwnd);

	return PostRequest(pRequest);
}
void CWebServiceMgr::PostRequest(lpRequest pRequest,ResponderT responder,DWORD dwTag)
{
	assert(pRequest);
	pRequest->SetReponder(responder);
	pRequest->SetTag(dwTag);

	return PostRequest(pRequest);
}
void CWebServiceMgr::PostRequest(lpRequest pRequest,ResponderT responder)
{
	assert(pRequest);
	pRequest->SetReponder(responder);
	
	return PostRequest(pRequest);
}

void CWebServiceMgr::AsynPostRequest(lpRequest pRequest)
{
	assert(pRequest);
	onAsynRequest(pRequest,true);
}
void CWebServiceMgr::AsynPostRequest(HWND hwnd,lpRequest pRequest,DWORD dwTag)
{
	assert(pRequest);
	pRequest->SetHwnd(hwnd);
	pRequest->SetTag(dwTag);

	return AsynPostRequest(pRequest);
}
void CWebServiceMgr::AsynPostRequest(HWND hwnd,lpRequest pRequest)
{
	assert(pRequest);
	pRequest->SetHwnd(hwnd);

	return AsynPostRequest(pRequest);
}
void CWebServiceMgr::AsynPostRequest(lpRequest pRequest,ResponderT responder,DWORD dwTag)
{
	assert(pRequest);
	pRequest->SetReponder(responder);
	pRequest->SetTag(dwTag);

	return AsynPostRequest(pRequest);
}
void CWebServiceMgr::AsynPostRequest(lpRequest pRequest,ResponderT responder)
{
	assert(pRequest);
	pRequest->SetReponder(responder);

	return AsynPostRequest(pRequest);
}

void CWebServiceMgr::onAsynRequest(lpRequest pRequest,bool bPost)
{
	assert(pRequest);
	if(pRequest)
	{
		RequestInfo* pInfo = new RequestInfo();
		pInfo->rq  = pRequest;
		pInfo->opt = (bPost ? RequestInfo::RT_POST : RequestInfo::RT_GET);
		hLinker h = theNet->createLinker(this);
		theNet->setTimeout(h,1000*15);
		theNet->setTag(h,(unsigned int)pInfo);
		theNet->asynconnect(h,_serversIp[pInfo->errCnt%_serversIp.size()].c_str(),"http",10);
	}
}

void CWebServiceMgr::OnRequest(lpRequest pRequest,bool bPost)
{
	assert(pRequest);
	if(!pRequest) return;
	const std::string requestHeaderStr = BuildHttpHeader(pRequest,bPost);
	RequestInfo info;
	info.rq	= pRequest;
	info.opt = (bPost ? RequestInfo::RT_POST : RequestInfo::RT_GET);
	hLinker h = theNet->createLinker(NULL);
	assert(h);
	CBuffer buffer;
	do 
	{
		buffer.clear();
		theNet->closeLinker(h);
		if(theNet->connect(h,_serversIp[info.errCnt%_serversIp.size()].c_str(),"http",10) &&
			(requestHeaderStr.size() == theNet->send(h,requestHeaderStr.c_str(),requestHeaderStr.size())))
		{
			while(theNet->waitReadable(h,10))
			{
				if(buffer.full())
					buffer.resize();
				size_t bytes = theNet->recv(h,buffer[buffer.wpos()],buffer.nextwriteblocksize());
				if(bytes > 0)
				{
					buffer.wpos(buffer.wpos() + bytes);
					bool haserr = false;
					if(info.parseHttpResponse(buffer,haserr))
					{
						if(haserr || 200 != info.nError)
						{
							info.rq->SetTimeOut(true);
							OnResponse(info.rq,NULL);
						}
						else
							OnResponse(info.rq,buffer);

						theNet->destroyLinker(h);
						return;
					}
				}
			}
		}
	} while (info.errCnt++ <  RequestInfo::MAX_RETRY_CNT);

	assert(false);
	info.rq->SetTimeOut(true);
	OnResponse(info.rq,NULL);
	theNet->destroyLinker(h);
	return;


	/*
	CHttpLink link;
	const int cacheType                = pRequest->GetCatchStrategyType();
	const std::string requestHeaderStr = BuildHttpHeader(pRequest->GetUrl());
	auto sendRequest = [&]()-> bool const 
	{
		static const int MAX_RETRY_CNT = 2;	
		for (int i =0;i<MAX_RETRY_CNT;++i)
		{
			if(!link.IsConnected())
			{
				CSockAddr addr(_nPort,_strServer.c_str());
				if(!link.Connect(addr))
				{
					CSockAddr addr2(_nPort,"122.225.11.137");
					if(!link.Connect(addr2))
						continue;
				}
			}

			if(link.SendHttpRequest(requestHeaderStr))
				return true;

			link.Close();
			Sleep(500);
		}
		return false;
	};


	CLink::spBuffer theBuffer(link.GetBuffer());
	if(CST_CATCH == cacheType &&  GetRequestFromCache(requestHeaderStr,*theBuffer) )
	{
		pRequest->SetCatched(true);
		return OnResponse(pRequest,*theBuffer);
	}


	if(sendRequest())
	{
		if(CST_NOCATCH != cacheType)
			_webCache.SaveCache(requestHeaderStr,*theBuffer,3600);

		if(!_isOnline)
		{
			sLogWarn("网络连接成功!!!");
			_isOnline = true;
			theNotifyCenter->sendNotifyOnMainThread(NT_NetChanged,(unsigned long)_isOnline);
		}

		return OnResponse(pRequest,*theBuffer);
	}
	else
	{
		if(_isOnline && !link.IsConnected())
		{
			sLogWarn("网络连接断开!!!");
			_isOnline = false;
			theNotifyCenter->sendNotifyOnMainThread(NT_NetChanged,(unsigned long)_isOnline);
		}

		if(CST_NONET == cacheType && GetRequestFromCache(requestHeaderStr,*theBuffer))
		{
			pRequest->SetCatched(true);
			return OnResponse(pRequest,*theBuffer);
		}

		pRequest->SetTimeOut(true);
		return OnResponse(pRequest,NULL);
	}
	*/
}
void CWebServiceMgr::OnResponse(lpRequest pRequest,CBuffer& buffer)
{
	void *pResponse = NULL;
	const bool bNeedParse = (pRequest && (pRequest->HasReponder() || pRequest->GetHwnd()) );

	if(bNeedParse)
	{
		static CResponseParser Parser;
		std::string xmlbuf;
		buffer>>xmlbuf;
		//	assert(!xmlbuf.empty());
		sLogWarn(xmlbuf.c_str());

		int t_begin = clock();
		pResponse = Parser.Parse(pRequest->GetRequestType(),xmlbuf);
		int t_time = clock() - t_begin;
	}

	return OnResponse(pRequest,pResponse);
}
void CWebServiceMgr::OnResponse(lpRequest pRequest,void* pResponse)
{
	assert(pRequest);
	if(!pRequest) return;

	sLog("request:%d time:%d",pRequest->GetRequestType(),clock() - pRequest->GetCreateTime());
	BOOL bRet = FALSE;
	//接收消息的窗口负责处理释放内存
	if(pRequest->HasReponder())
	{
		pRequest->Response(pResponse);
	}
	else if(pRequest->GetHwnd())
	{
		bRet = PostMessage(pRequest->GetHwnd(),_nMsg,(WPARAM)pRequest,(LPARAM)pResponse);
		assert(bRet);
	}
	if(!bRet)
	{
		if(pResponse)
		{
			delete pResponse;
			pResponse = NULL;
		}

		if(pRequest)
		{
			delete pRequest;
			pRequest = NULL;
		}
	}
}
bool CWebServiceMgr::GetRequestFromCache(const std::string& headerStr,CBuffer& buffer)
{
	return _webCache.LoadCache(headerStr,buffer);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

void CWebServiceMgr::onConnected(hLinker h)
{
	RequestInfo* pInfo = (RequestInfo*)(theNet->tag(h));
	assert(pInfo);
	if(pInfo)
	{
		std::string	sbuf = BuildHttpHeader(pInfo->rq,(RequestInfo::RT_POST == pInfo->opt));
		theNet->asynsend(h,sbuf.c_str(),sbuf.size());
	}
}
void CWebServiceMgr::onDisconnected(hLinker h,const int ec)
{
	RequestInfo* pInfo = (RequestInfo*)(theNet->tag(h));
	if(pInfo)
	{
		pInfo->errCnt++;
		if(pInfo->errCnt < RequestInfo::MAX_RETRY_CNT)
		{
			theNet->asynconnect(h,_serversIp[pInfo->errCnt%_serversIp.size()].c_str(),"http",10);
		}
		else
		{
			pInfo->rq->SetTimeOut(true);
			OnResponse(pInfo->rq,NULL);
			delete pInfo;
			theNet->setTag(h,0);
			theNet->destroyLinker(h);
		}
	}
	else
	{
		theNet->destroyLinker(h);
	}
}
void CWebServiceMgr::onSendCompleted(hLinker h,const void* pbuffer,const int ec,size_t bytes)
{
	assert(!ec);
}

size_t CWebServiceMgr::onRecvCompleted(hLinker h,const void* pbuffer,size_t bytes)
{
//	return bytes;
	RequestInfo* pInfo = (RequestInfo*)(theNet->tag(h));
	assert(pInfo);
	if(pInfo)
	{
		CBuffer buffer((uint8*)pbuffer,bytes);
		bool haserr = false;
		if(pInfo->parseHttpResponse(buffer,haserr))
		{
			if(haserr || pInfo->nError != 200)
			{
				pInfo->rq->SetTimeOut(true);
				OnResponse(pInfo->rq,NULL);
			}
			else
				OnResponse(pInfo->rq,buffer);

			theNet->closeLinker(h);
			buffer.clear();
			delete pInfo;
			theNet->setTag(h,0);
		}

		return bytes - buffer.cnt();
	}
	return bytes;
}
void CWebServiceMgr::onDestroy(hLinker h)
{
	
}
