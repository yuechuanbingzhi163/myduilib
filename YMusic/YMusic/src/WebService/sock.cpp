#include "stdafx.h"
#include "sock.h"

CWSAStartup::CWSAStartup(BYTE hver /* = 2 */,BYTE lver /* = 1 */)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	memset(&wsaData,0,sizeof(wsaData));
	wVersionRequested = MAKEWORD( hver, lver );
	WSAStartup( wVersionRequested, &wsaData );
}

CWSAStartup::~CWSAStartup()
{
	WSACleanup();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CSockAddr::CSockAddr(unsigned short uport,const char* ip /* = NULL */)
	:addlen(sizeof(sockaddr_in))
{
	unsigned long  nIp = ::htonl(INADDR_ANY);

	if(ip)
	{
		if(isalpha(ip[0]))
		{
			LPHOSTENT     lphostent; 
			lphostent=::gethostbyname(ip);
			if(lphostent)
			{
				nIp = inet_addr(inet_ntoa(*((LPIN_ADDR)*lphostent->h_addr_list)));
			}	
		}
		else
		{
			nIp = inet_addr(ip);
		}
	}
	

	assert(uport);
	this->sin_family = AF_INET;   
	this->sin_port = htons(uport);    
	this->sin_addr.S_un.S_addr = nIp;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CSock::Ioctl(long cmd,u_long FAR* argp)
{
	assert(IsCreate());
	if(!IsCreate()) return false;
	return SOCKET_ERROR != ::ioctlsocket(_s,cmd,argp);
}

bool CSock::Create(int nType)
{
	if(IsCreate()) return true;

	_s = ::socket(AF_INET,nType,0);

	if(nType == SOCK_DGRAM)
	{
		BOOL bNewBehavior = FALSE;
		DWORD dwBytesReturned = 0;
		WSAIoctl(
			_s,
			SIO_UDP_CONNRESET,
			&bNewBehavior,
			sizeof(bNewBehavior),
			NULL,
			0,
			&dwBytesReturned,
			NULL,
			NULL); //mssock bug
	}


	assert(IsCreate());
	return IsCreate();
}

bool CSock::Close()
{
	if(!IsCreate()) return true;
	::closesocket(_s);
	_s = INVALID_SOCKET;
	return !IsCreate();
}

bool CSock::Bind(CSockAddr* addr)
{
	assert(IsCreate());
	if(!IsCreate())
		return false;
	assert(addr);
	return SOCKET_ERROR != ::bind(_s,(sockaddr*)addr,(addr ? addr->addlen : NULL));
}

bool CSock::Listen(int nbacklog /* = 5 */)
{
	assert(IsCreate());
	if(!IsCreate())
		return false;

	return SOCKET_ERROR != ::listen(_s,nbacklog);
}

SOCKET CSock::Accept(CSockAddr* addr/* = NULL*/)
{
	assert(IsCreate());
	if(!IsCreate())
		return false;

	return ::accept(_s,(sockaddr*)addr,(addr ? &addr->addlen : NULL));
}

bool CSock::IsConnect() const
{
	assert(IsCreate());
	if(!IsCreate())
		return false;

	int optval, optlen = sizeof(int);
	if(SOCKET_ERROR != getsockopt(_s, SOL_SOCKET, SO_ERROR,(char*) &optval, &optlen))
	{
		switch(optval)
		{
		case 0: return true;    
		case ECONNREFUSED:break;
		}
	}
	
	return false;
}

bool CSock::Connect(CSockAddr* pRemoteaddr)
{
	assert(IsCreate());
	if(!IsCreate())
		return false;

	assert(pRemoteaddr);
	return	SOCKET_ERROR != ::connect(_s,(sockaddr*)pRemoteaddr,(pRemoteaddr ? pRemoteaddr->addlen : NULL));
}
bool CSock::ConnectEx(CSockAddr* pRemoteaddr,int nTimeOut /* = 10 */,ExitWaitPred isExitWaitPred/* = NULL*/)
{
	assert(IsCreate());
	if(!IsCreate())
		return false;

	assert(pRemoteaddr);
	unsigned long ul = 1; 
	this->Ioctl(FIONBIO,&ul);//设置阻塞模式<0为阻塞，1非阻塞>  
	int nRet = ::connect(_s,(sockaddr*)pRemoteaddr,(pRemoteaddr ? pRemoteaddr->addlen : NULL)); 
	if(SOCKET_ERROR == nRet)
	{
		int nError = ::WSAGetLastError();
		if(WSAEWOULDBLOCK != nError && 
			WSAEALREADY != nError &&
			WSAEISCONN != nError)
		{ 
			return false;
		}
		if(WSAEISCONN != nError)
		{
			if(!this->WaitWriteable(nTimeOut,isExitWaitPred))
				return false;
		}
	}

	ul = 0;
	this->Ioctl(FIONBIO,&ul);
	return true;
}


bool CSock::WaitReadable(unsigned int nTimeOut /* = 10 */,ExitWaitPred isExitWaitPred /* = NULL */)
{
	assert(IsCreate());
	if(!IsCreate())
		return false;

	if(0 >= nTimeOut)
		nTimeOut = 1;
	while(nTimeOut-- && !(isExitWaitPred ? isExitWaitPred() : false))
	{
		fd_set      fdRead;
		timeval     tv;
		FD_ZERO(&fdRead);
		FD_SET(_s,&fdRead);
		tv.tv_sec  = 1;
		tv.tv_usec = 0;

		int nRet = ::select(0,&fdRead,NULL,NULL,&tv);
		if(0 < nRet)
			return true;
		if(0 > nRet)
			break;
	}
	return false;
}

bool CSock::WaitWriteable(unsigned int nTimeOut /* = 10 */,ExitWaitPred isExitWaitPred /* = NULL */)
{
	assert(IsCreate());
	if(!IsCreate())
		return false;
	if(0 >= nTimeOut)
		nTimeOut = 1;
	while(nTimeOut-- && !(isExitWaitPred ? isExitWaitPred() : false))
	{
		fd_set      fdWrite;
		timeval     tv;
		FD_ZERO(&fdWrite);
		FD_SET(_s,&fdWrite);
		tv.tv_sec  = 1;
		tv.tv_usec = 0;

		int nRet = ::select(0,NULL,&fdWrite,NULL,&tv);
		if(0 < nRet)
			return true;
		if(0 > nRet)
			break;
	}

	return false;
}

int CSock::Recv(char* buf,size_t cnt)
{
	assert(IsCreate());
	return ::recv(_s,buf,cnt,0);
}

int CSock::Send(char* buf,size_t cnt)
{
	assert(IsCreate());

	size_t nSendLen      = 0;
	while(nSendLen != cnt)
	{
		int nRet = ::send(_s,(char*)buf+nSendLen,cnt-nSendLen,0);
		if(nRet <= 0)
			break;

		nSendLen += nRet;
	}

	return nSendLen;
}

int CSock::RecvFrom(char* buf,size_t cnt,CSockAddr* pRemoteaddr)
{
	assert(IsCreate());
	assert(pRemoteaddr);   
	return ::recvfrom(_s,buf,cnt,0,(sockaddr*)pRemoteaddr,(pRemoteaddr ? &pRemoteaddr->addlen : NULL));
}

int CSock::SendTo(char* buf,size_t cnt,CSockAddr* pRemoteaddr)
{
	assert(IsCreate());
	assert(pRemoteaddr);

	return ::sendto(_s,buf,cnt,0,(sockaddr*)pRemoteaddr,(pRemoteaddr ? pRemoteaddr->addlen : NULL));
}