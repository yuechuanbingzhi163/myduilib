
/*
 * Copyright (C) 
 */


#ifndef _SOCKET_H_INCLUDED_
#define _SOCKET_H_INCLUDED_

#pragma once


namespace DuiLib
{
	class UILIB_API CWSAStartup
	{
	public:
		explicit CWSAStartup(BYTE hver = 2, BYTE lver = 1);
		~CWSAStartup();
	protected:
		CWSAStartup(const CWSAStartup& other){}
	};


	class CSock;
	typedef UINT_PTR SOCKET;

	class UILIB_API ISock
	{
	public:
		virtual void OnSocketConnect(CSock *s) = 0;
		virtual void OnSocketClose(CSock *s) = 0;
		virtual void OnSocketRead(CSock *s) = 0;
		virtual void OnSocketWrite(CSock *s) = 0;
		virtual void OnSocketError(CSock *s, int error) = 0;
	};


	class UILIB_API CSock
	{
		friend class CSockWnd;

	public:
		CSock();
		virtual ~CSock();

		void Init(ISock *callback) { m_callback = callback; }
		operator SOCKET() const{ return m_socket; };

		bool IsConnect() { return m_bConnect; }
		bool Open(char *host, int port);
		void Close();
		int Send(void *buf, size_t size);
		int Recv(void *buf, size_t size);

	protected:
		ISock   *m_callback;
		SOCKET   m_socket;
		bool     m_bConnect;
	};
}

#endif
