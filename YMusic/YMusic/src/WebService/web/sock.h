#pragma once
#include <WinSock2.h>
#include <MSWSock.h>
#include <Windows.h>

#pragma comment(lib,"ws2_32.lib")
#include "../../YMusic.h"

class CWSAStartup
{
public:
	explicit CWSAStartup(BYTE hver = 2,BYTE lver = 1);
	~CWSAStartup();
protected:
	CWSAStartup(const CWSAStartup& other){}
};

class CSockAddr : public sockaddr_in
{
public:
	CSockAddr():addlen(sizeof(sockaddr_in)){}
	CSockAddr(unsigned short uport,const char* ip = NULL);
	~CSockAddr(){}

	int addlen;
};

class CSock
{
public:
//	typedef boost::function<bool (void)> ExitWaitPred;
	typedef bool(*ExitWaitPred)(void);
	CSock():_s(INVALID_SOCKET){}
	virtual ~CSock(){Close();}
public:
	operator SOCKET() const{return _s;};

	bool Ioctl(long cmd,u_long FAR* argp);

	bool IsCreate()const {return INVALID_SOCKET != _s;}
	bool Create(int ntype = SOCK_STREAM);
	bool Close();
	bool Bind(CSockAddr* addr);

	bool Listen(int nbacklog = 5);
	SOCKET Accept(CSockAddr* addr = NULL);

	bool IsConnect() const;
	bool Connect(CSockAddr* pRemoteaddr);
	bool ConnectEx(CSockAddr* pRemoteaddr,int nTimeOut = 10,ExitWaitPred isExitWaitPred = NULL);


	bool WaitReadable(unsigned int nTimeOut = 10,ExitWaitPred isExitWaitPred = NULL);
	bool WaitWriteable(unsigned int nTimeOut = 10,ExitWaitPred isExitWaitPred = NULL);

	int Recv(char* buf,size_t cnt);
	int Send(char* buf,size_t cnt);

	int RecvFrom(char* buf,size_t cnt,CSockAddr* pRemoteaddr = NULL);
	int SendTo(char* buf,size_t cnt,CSockAddr* pRemoteaddr = NULL);
	

protected:
	SOCKET    _s;
};


typedef smart_pointer::SmartPtr<CSock> HSOCK;