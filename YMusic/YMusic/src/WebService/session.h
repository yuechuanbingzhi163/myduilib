#pragma once
#include <winsock2.h>
#include <windows.h>
#include <string>
#include "WebService.h"

	class CWebServiceSession
	{
	public:
		static const size_t MinBufferSize = 1024;
		static const size_t MaxBufferSize = 1024*1024;
		static const size_t MaxErrCnt     = 3;
	public:
		HWND m_hwnd;
		CWebServiceSession(HWND hwnd,WebServiceRequestTypeT RequestType,int nParam=0);
		virtual ~CWebServiceSession();
		bool SendRequest(SOCKET sock);
		int  GetResponse(SOCKET sock,char *buf,int len);
		bool GetAllResponse(SOCKET  sock,std::string &strXmlData,int nTimeOut=2*60);

		bool FixBuffer(CBuffer& buffer);
		bool ParseResponseHeader(CBuffer& buffer,int &nContentLen,bool &bGZip,bool &bChunked,int& nError);
		bool GetAllResponse(SOCKET& sock,CBuffer& buffer,int nTimeOut = 10);
		bool RecvByContentLen(SOCKET& sock,CBuffer& buffer,const int nContenLen);
		bool RecvByChunked(SOCKET& sock,CBuffer& buffer);

		void SetHeader(const std::string& strHeader);
		WebServiceRequestTypeT  GetRequestType() const;
		void SetTimeOut(bool bTimeOut);
		bool IsTimeOut();
		bool ParseResponseHeader(char *pBuf,int nLen,int &nContentLen ,int &nHeaderLen,bool &bGZip,bool &bChunked);
		bool RecvByContentLen(SOCKET  sock,char **pData,int nBufSize,int &nDataLen,int nContentLen);
		bool RecvByChunked(SOCKET  sock,char **pData,int nBufSize,int &nDataLen);

		int GetParam() const;
		static int TCPsend(SOCKET s,const char* buf,int len,int flags);

		int m_tCreate;
	private:
		std::string m_strHeader;
		WebServiceRequestTypeT m_RequestType;
		int m_nParam;
		bool m_bTimeOut;
	};

