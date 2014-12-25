#include <winsock2.h>
#include <assert.h>
#include <time.h>
#include "session.h"
#include "WebService.h"
#include "shareTingApp.h"
#include "zlib.h"

#define   CHUNK   16384 
__MyNameSpaceBegin

int inflate_read(char *source,int len,char **dest,int gzip)
{  
	int ret;  
	unsigned have;  
	z_stream strm;  
	unsigned char out[CHUNK];  
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

CWebServiceSession::CWebServiceSession(HWND hwnd,WebServiceRequestTypeT RequestType,int nParam)
{
    m_hwnd = hwnd;
    m_RequestType = RequestType;
    m_nParam = nParam;
	m_bTimeOut = false;

	m_tCreate = clock();
}

void CWebServiceSession::SetTimeOut(bool bTimeOut)
{
	m_bTimeOut = bTimeOut;
}

bool CWebServiceSession::IsTimeOut()
{
	return m_bTimeOut;
}

CWebServiceSession::~CWebServiceSession()
{

}

bool CWebServiceSession::SendRequest(SOCKET  sock)
{
	int nLen = m_strHeader.length();
	const char *pData = m_strHeader.c_str();
	int nRet = TCPsend(sock,pData,nLen,0);
	assert(nRet == nLen);
	return (nRet==nLen?true:false);
}

int CWebServiceSession::GetResponse(SOCKET  sock,char *buf,int len)
{
    return recv(sock,buf,len,0);
}

bool CWebServiceSession::ParseResponseHeader(CBuffer& buffer,int &nContentLen,bool &bGZip,bool &bChunked,int& nError)
{
	const int pos = buffer.find((uint8*)("\r\n\r\n"),4);
	if(-1 == pos) return false;

	static const char GZIP[]           ="gzip";
	static const char CONTENT_ENCOD[]  ="Content-Encoding:";
	static const char CHUNKED[]        = "chunked";
	static const char TRANSFER_ENCOD[] ="Transfer-Encoding:";
	static const char CONTENT_LENGTH[] = "Content-Length: ";

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
		sscanf_s((char*)buffer[i+strlen(CONTENT_LENGTH)],"%d\r\n",&nContentLen);
	}

	assert(0 != nContentLen);
	buffer.rpos(pos+4);
	return true;
}
bool CWebServiceSession::RecvByContentLen(SOCKET& sock,CBuffer& buffer,const int nContenLen)
{
	assert(INVALID_SOCKET != sock);
	if(INVALID_SOCKET == sock) return false;
	assert(0 != nContenLen);

	if(nContenLen > 0)
	{
		if(buffer.cnt() >= nContenLen) return true;
	}

	const int oldLen = buffer.cnt();

	do 
	{
		if(!FixBuffer(buffer)) return false;
		int nRet = recv(sock, (char*)(buffer[buffer.wpos()]),buffer.nextwriteblocksize(), 0);
		if(nRet > 0) 
			buffer.wpos(buffer.wpos()+nRet);
		if(0 == nRet)
		{
			closesocket(sock);
			sock=INVALID_SOCKET;
		//	assert(false);
			return true;
		}
		if(nRet < 0)
		{
			closesocket(sock);
			sock=INVALID_SOCKET;
			assert(false);
			sLogError("CWebServiceSession recv error! ntype:%d nRet = %d",GetRequestType(), ::WSAGetLastError());
			return false;
		}

		if(nContenLen > 0 && buffer.cnt() >= nContenLen) return true;
	} while (true);
	assert(false);
	return false;
}
bool CWebServiceSession::RecvByChunked(SOCKET& sock,CBuffer& buffer)
{
	assert(INVALID_SOCKET != sock);
	if(INVALID_SOCKET == sock) return false;

	static const char CHUNKED[]        ="\r\n";
	static const size_t MinChunkedSize = strlen(CHUNKED);

	CBuffer chunkedBuffer;
	chunkedBuffer<<buffer;
	buffer.clear();
	
	size_t chunkCnt = 0;
	long datasize = -1;
	do 
	{
		do 
		{
			if(-1 != datasize)
			{
				if(chunkedBuffer.cnt() < datasize)
					break;
				else
				{
					size_t bytes = datasize - MinChunkedSize;
					while(bytes != 0)
					{
						size_t blocksize = std::min<size_t>(chunkedBuffer.nextreadblocksize(),bytes);
						buffer.Write(chunkedBuffer[chunkedBuffer.rpos()],blocksize);
						chunkedBuffer.rpos(chunkedBuffer.rpos()+blocksize);
						bytes -= blocksize;
					}
					chunkedBuffer.rpos(chunkedBuffer.rpos()+MinChunkedSize);
					if(2 == datasize)
						return true;
					datasize = -1;
					++chunkCnt;
				}
			}

			if(chunkedBuffer.cnt() <= MinChunkedSize) 
				break;
			const int pos = chunkedBuffer.find((const uint8*)CHUNKED,MinChunkedSize);
			if(-1 == pos) break;
			assert(pos != chunkedBuffer.rpos());
			if(pos == chunkedBuffer.rpos())
				return false;
			if(pos > chunkedBuffer.rpos())
			{
				const char* pStart = (const char*)chunkedBuffer[chunkedBuffer.rpos()];
				char* pEnd = NULL;
				datasize = strtol(pStart,&pEnd,16);

				if(0 == datasize && pEnd == pStart)
					return false;
				if(0 > datasize)
					return false;
				if(datasize > MaxBufferSize)
					return false;
				if(0 == datasize)
				{
					assert(chunkCnt > 0);
					if(0 >= chunkCnt) return false;
				}
				
				chunkedBuffer.rpos(pos+MinChunkedSize);
				datasize += MinChunkedSize;
			}else
			{
				std::string str;
				while(chunkedBuffer.rpos() != pos)
				{
					char* pc = (char*)chunkedBuffer[chunkedBuffer.rpos()];
					str += *pc;
					chunkedBuffer.rpos(chunkedBuffer.rpos()+1);
				}

				const char* pStart = (const char*)chunkedBuffer[chunkedBuffer.rpos()];
				char* pEnd = NULL;
				datasize   = strtol(pStart,&pEnd,16);
				if(0 == datasize && pEnd == pStart)
					return false;
				if(0 > datasize)
					return false;
				if(datasize > MaxBufferSize)
					return false;
				if(0 == datasize)
				{
					assert(chunkCnt > 0);
					if(0 >= chunkCnt) return false;
				}
				
				chunkedBuffer.rpos(MinChunkedSize);
				datasize += MinChunkedSize;
			}
		} while (true);


		if(!FixBuffer(chunkedBuffer)) return false;
		int nRet = recv(sock,(char*)(chunkedBuffer[chunkedBuffer.wpos()]),chunkedBuffer.nextwriteblocksize(),0);
		if(nRet > 0) 
			chunkedBuffer.wpos(chunkedBuffer.wpos()+nRet);
		
		if(0 == nRet)
		{
			closesocket(sock);
			sock=INVALID_SOCKET;
			assert(!buffer.empty());
			return true;
		}
		if(nRet < 0)
		{
			closesocket(sock);
			sock=INVALID_SOCKET;
			assert(false);
			sLogError("CWebServiceSession recv error! ntype:%d nRet = %d",GetRequestType(), ::WSAGetLastError());
			return false;
		}
	} while (1);
	return false;
}
bool CWebServiceSession::FixBuffer(CBuffer& buffer)
{
	if(buffer.empty())
		buffer.clear();

	int i = MaxErrCnt;
	while(buffer.nextwriteblocksize() < MinBufferSize)
	{
		buffer.resize();
		--i;
		if(0 >= i)
			break;
	}

	if(buffer.nextwriteblocksize() < MinBufferSize)
		return false;

	return true;
}
bool CWebServiceSession::GetAllResponse(SOCKET& sock,CBuffer& buffer,int nTimeOut)
{
	assert(buffer.empty());
	assert(INVALID_SOCKET != sock);
	if(!buffer.empty()) return false;
	if(INVALID_SOCKET == sock) return false;
	buffer.clear();

	int  nContentLen = -1;
	bool bGZip =  false;
	bool bChunked=  false;
	bool bHeaderParsed = false;
	int  nError = 200;
	do 
	{
		if(!FixBuffer(buffer)) return false;

		int nRet = recv(sock, (char*)(buffer[buffer.wpos()]),buffer.nextwriteblocksize(), 0);
		if(nRet > 0)
		{
			buffer.wpos(buffer.wpos() + nRet);
			bHeaderParsed = ParseResponseHeader(buffer,nContentLen,bGZip,bChunked,nError);
			if(bHeaderParsed)
				break;
		}
		if(nRet <= 0)
		{
			closesocket(sock);
			sock=INVALID_SOCKET;
			assert(0==nRet);
			assert(buffer.empty());
			sLogError("CWebServiceSession recv error! ntype:%d nRet = %d",GetRequestType(), ::WSAGetLastError());
			return false;
		}
	} while (!bHeaderParsed);

// 	assert(!buffer.empty());
// 	if(buffer.empty())
// 		return false;

	assert(200 == nError);
	if(200 != nError)
		return false;

	bool bOK = (bChunked ? RecvByChunked(sock,buffer) : RecvByContentLen(sock,buffer,nContentLen));
	assert(bOK);
	if(!bOK) return false;

// 
// 	if(2 == this->GetRequestType())
// 	{
// 		static int  k = 0;
// 		++k;
// 		char buf[1024] = {0};
// 		sprintf(buf,"c:/test/t2/chunk_%d.log",k);
// 
// 		FILE* fp = fopen(buf,"a");
// 		fwrite(buffer[buffer.rpos()],1,buffer.cnt(),fp);
// 		fclose(fp);
// 	}
	if(bGZip)
	{
		char* p_buf = NULL;
		int nResult = inflate_read((char*)(buffer[buffer.rpos()]), buffer.cnt(), &p_buf, 1);
		if(Z_OK != nResult)
		{
			assert(false);
			sLogError("inflate_read error(%d),pkSize:%u,ntype:%d",nResult,buffer.cnt(),GetRequestType());
			return false;
		}
		buffer.clear();
		buffer.Write((uint8*)p_buf,strlen(p_buf));	
		buffer.Write((uint8*)'\0',1);
	}

	return bOK;
}


bool CWebServiceSession::GetAllResponse(SOCKET  sock,std::string &strXmlData, int nTimeOut)
{
	int nRet = 0;
	fd_set      fdRead;
	timeval     tv = {1, 0};
	FD_ZERO(&fdRead);//初始化fd_set
	const int BUFSIZE = 16 * 1024;
	char recvBuf[BUFSIZE + 1];
	int nRecvLen = 0;
	bool bHeaderParsed = false;   //是否正在接收HTTP协议头
	int  nContentLen = -1;
	int  nHeaderLen = -1;
	bool bGZip =  false;
	bool bChunked=  false;
	int nRecvBufSize  =0;
	char *pRecvBuf = NULL;
	int nDataLen =0;


	while(!bHeaderParsed)
	{
		if(BUFSIZE <= nRecvLen)
			break;
		int nRes = recv(sock, recvBuf + nRecvLen, BUFSIZE - nRecvLen, 0);  // 从webserver获取数据 

		if(nRes <= 0)
		{
			nRet = WSAGetLastError();
			sLogError("CWebServiceSession recv error! ntype:%d nRet = %d",GetRequestType(), nRet);
			break;
		}
		nRecvLen += nRes;	// 累加接收的总字节数
	
		recvBuf[nRecvLen] = 0;
		
		bHeaderParsed = ParseResponseHeader(recvBuf,nRecvLen,nContentLen,nHeaderLen,bGZip,bChunked);
	}


	if(!bHeaderParsed)	
	{
		return false;
	}
	nDataLen =  nRecvLen - nHeaderLen;
	if(nContentLen  <= 0)
	{
		nRecvBufSize = nDataLen;	
	}
	else
	{
		nRecvBufSize = nContentLen;
	}
	
	pRecvBuf = new char[nRecvBufSize+1];
	if(pRecvBuf == NULL)
	{
		return false;
	}
	memcpy(pRecvBuf, recvBuf + nHeaderLen,nDataLen);	// 第一次接收的正文部分
	pRecvBuf[nRecvBufSize]=0;
	bool bOK =false;

	if(bChunked)
	{
		bOK = RecvByChunked(sock,&pRecvBuf,nRecvBufSize,nDataLen);
	}
	else
	{
		bOK =RecvByContentLen(sock,&pRecvBuf,nRecvBufSize,nDataLen,nContentLen);
	}
	
 	if (bOK )
	{
		if(bGZip)
		{
			char *p_buf = NULL;
			if(Z_OK != inflate_read(pRecvBuf, nContentLen, &p_buf, 1))//解压缩数据
			{
				//mainLog()->write_log(em_utility::ll_error, "XML解压失败！nContentLen = %d, pRcvBuf = %s, nRecvLen = %d, nTotalRecv = %d", nContentLen, pRecvBuf, nRecvLen, nTotalRecv);
			}
			if(p_buf != NULL)
			{
				strXmlData = p_buf;
				delete[] p_buf;
			}
		}
		else
		{
		//	printf("ws response : %s\n ",pRecvBuf);
			strXmlData = pRecvBuf;
			
		}
	}
	if(pRecvBuf != NULL)
		delete[] pRecvBuf;
	//printf("ws response : %s\n ",strXmlData.c_str());
	
	return bOK;
}

bool CWebServiceSession::RecvByContentLen(SOCKET  sock,char **pData,int nBufSize,int &nDataLen,int nContentLen)
{
// 	assert(nContentLen > 0);
// 	assert(nDataLen <= 32*1024);
// 	if(nContentLen <=0)return false;
// 	if(nDataLen > 32*1024) return false;

	const int BUFSIZE = 32 * 1024;
	char recvBuf[BUFSIZE + 1];
	int nRecvLen = 0;

	while(true)
	{
		int nRes = recv(sock, recvBuf + nRecvLen, BUFSIZE - nRecvLen, 0);  // 从webserver获取数据 
		if(nRes <= 0)
		{
			//nRet = WSAGetLastError();
			//mainLog()->write_log(em_utility::ll_error, "CWebServiceSession recv error! nRet = %d", nRet);
			break;
		}
		if(nDataLen + nRes > nBufSize)
		{
			nBufSize += 32*1024;
			assert(nBufSize >= (nDataLen + nRes));
			char * pNewData = new char[nBufSize];
			if(pNewData == NULL)
			{
				return false;
			}
			memcpy(pNewData, *pData,nDataLen);
			delete []*pData;
			*pData = pNewData;
		}
		memcpy(*pData + nDataLen, recvBuf ,nRes);
		nDataLen +=nRes;
		(*pData)[nDataLen]=0;
		if(nContentLen >0 && nDataLen >= nContentLen )
		{
			break;
		}
	}
	return true;
}

bool CWebServiceSession::RecvByChunked(SOCKET  sock,char **pData,int nBufSize,int &nDataLen)
{
	int nRecvBufSize = nDataLen + 32*1024;
	char *pRecvBuf = new char[nRecvBufSize];
	if(pRecvBuf == NULL)
		return false;
	memcpy(pRecvBuf,*pData,nDataLen);
	int nRecvLen = nDataLen;
	while(true)
	{
		while(true)
		{
			char *pCur=pRecvBuf;
			while(pCur < pRecvBuf + nRecvLen )
			{
				if(pCur[0] =='\r' && pCur[1] =='\n' ) break;
				pCur++;
			}
			if(pCur == pRecvBuf + nRecvLen) break;
			char*pDataStart = pCur+2;
			int nChunkHeaderLen = pDataStart - pRecvBuf -2;
			int nChunkLen = atoi(pRecvBuf);
			if(nChunkLen + nChunkHeaderLen + 4 > nRecvLen)
				break;

			if(nDataLen + nChunkLen > nBufSize)
			{
				nRecvBufSize += 32*1024;
		
				char * pNewData = new char[nRecvBufSize];
				if(pNewData == NULL)
				{
					return false;
				}
				memcpy(pNewData, *pData,nDataLen);
				delete []*pData;
				*pData = pNewData;
			}
			memcpy(*pData, pDataStart,nChunkLen);
			nDataLen+= nChunkLen;

			char *pTempBuf = new char[nRecvBufSize];
			if(pTempBuf == NULL)
			{
				return false;
			}
			int nChunkTotalSize =  nDataLen+nChunkHeaderLen + 4;
			memcpy(pTempBuf, pRecvBuf +nChunkTotalSize ,nRecvLen - nChunkTotalSize );
			delete [] pRecvBuf;
			pRecvBuf = pTempBuf;
			nRecvLen = nRecvLen - nChunkTotalSize;
		}

		int nRes = recv(sock, pRecvBuf + nRecvLen, nRecvBufSize - nRecvLen, 0);  // 从webserver获取数据 

		if(nRes <= 0)
		{
			//nRet = WSAGetLastError();
			//mainLog()->write_log(em_utility::ll_error, "CWebServiceSession recv error! nRet = %d", nRet);
			break;
		}
		nRecvLen += nRes;
		if(nRecvLen == nRecvBufSize)
		{
			nRecvBufSize += 32*1024;
			char * pNew = new char[nRecvBufSize];
			if(pNew == NULL)
			{
				return false;
			}
			memcpy(pNew, pRecvBuf,nDataLen);
			delete []pRecvBuf;
			pRecvBuf = pNew;
		}
	}
}

bool CWebServiceSession::ParseResponseHeader(char *pBuf,int nLen,int &nContentLen ,int &nHeaderLen,bool &bGZip,bool &bChunked)
{
	int nPos = 0;
	int nFind = 0;
	std::string strHdr;
	
	char *pData = (char *)pBuf;
	while (nPos < nLen && nFind < 4)	// 跳过报头部分，连续两个换行\r\n
	{
		if (*pData == '\r'|| *pData == '\n')
		{
			++nFind;
		}
		else
		{
			nFind = 0;
		}
		++nPos;
		++pData;
	}
	if (nFind != 4)
	{
		return false;
	}
	nHeaderLen = pData - pBuf;


	int nStartPos;
	int nEndPos;
	strHdr += pBuf;
	bGZip = false;
 	if(strHdr.find("gzip") != std::string::npos && strHdr.find("Content-Encoding:") != std::string::npos )
	{
		bGZip = true;
	}
	bChunked = false;
	if(strHdr.find("chunked") != std::string::npos && strHdr.find("Transfer-Encoding:") != std::string::npos )
	{
		bChunked = true;
	}
	nContentLen = -1;
	static const char CONTENT_LENGTH[] = "Content-Length: ";
	nStartPos = strHdr.find(CONTENT_LENGTH);

	if (nStartPos!=-1)
	{
		nStartPos += strlen(CONTENT_LENGTH);
		nEndPos = strHdr.find("\r\n", nStartPos);
		std::string strContenLen = strHdr.substr(nStartPos, nEndPos - nStartPos);

		nContentLen = atoi(strContenLen.c_str());	// 得到正文长度
	}

	return true;
}
void CWebServiceSession::SetHeader(const std::string& strHeader)
{
    m_strHeader = strHeader;
}

int  CWebServiceSession::GetParam() const
{
   return m_nParam;
}

WebServiceRequestTypeT CWebServiceSession::GetRequestType() const
{
   return m_RequestType;
}

int CWebServiceSession::TCPsend(SOCKET s,const char* buf,int len,int flags)
{

	int n=0,sendCount=0;
	int length =len;
	if(buf==NULL)
		return 0;
	while(length>0)
	{
		n=send(s,buf+sendCount,length,flags); //发送数据
		if(n<=0)//网络出现异常
		{
			ATLTRACE(_T("send(),error code:%d\n"),WSAGetLastError());
			break;

		}
		length-=n;
		sendCount+=n; 
	}

	return sendCount;
}

__MyNameSpaceEnd