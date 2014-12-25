#pragma once
#include <string>

#include "sock.h"
#include "../../YMusic.h"

#ifndef CanAssert
#define YTingAssert  assert
#endif

class CLink
{
protected:
	static const size_t MinBufferSize = 1024;
	static const size_t MaxBufferSize = 1024*1024;
	static const size_t MaxErrCnt     = 2;
	static const size_t TimeOutSec    = 8;
public:
	CLink();
	virtual ~CLink();
public:
	typedef smart_pointer::SmartPtr<CBuffer> spBuffer;
	spBuffer GetBuffer();

	void SetTimeOut(size_t nTimeOut);
	size_t GetTimeOut()const{return _nTimeOut;};

	bool IsConnected()const{return _isConnected;};
	bool Connect(CSockAddr& addr);
	bool Close();
	bool Send(const void* buf,size_t nlen);
	bool Recv(size_t minSize = 0);
protected:
	bool FixBuffer(CBuffer* pbuffer);
	bool _Recv(CBuffer* pbuffer,size_t minSize = 0);
protected:
	size_t          _nTimeOut;
	spBuffer        _pBuffer;
	bool            _isConnected;
	CSock           _s;
};

class CHttpLink : public CLink
{
public:
	CHttpLink(){}
	virtual ~CHttpLink(){}

public:
	bool SendHttpRequest(const std::string& s);
protected:
	bool GetAllResponse();
	bool ParseResponseHeader(bool &bGZip,bool &bChunked,int &nContentLen,int& nError);
	bool RecvByContentLen(const int nContenLen);
	bool RecvByChunked();
};
