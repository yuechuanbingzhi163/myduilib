
/*
 * Copyright (C)
 */
#include "stdafx.h"
#include "sock.h"

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

namespace DuiLib
{
	CWSAStartup::CWSAStartup(BYTE hver /* = 2 */, BYTE lver /* = 1 */)
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		memset(&wsaData, 0, sizeof(wsaData));
		wVersionRequested = MAKEWORD(hver, lver);
		WSAStartup(wVersionRequested, &wsaData);
	}

	CWSAStartup::~CWSAStartup()
	{
		WSACleanup();
	}

	class CSockWnd : public CWindowWnd
	{
	public:
		CSockWnd();
		~CSockWnd();

		LPCTSTR GetWindowClassName() const { return _T("SocketWnd"); };
		void OnFinalMessage(HWND hWnd) { delete this; };

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		static void Init();
		static bool AddSocket(CSock *socket);
		static void RemoveSocket(CSock *socket);

	private:
		static CSockWnd *m_socketWnd;
		static CStdStringPtrMap m_sockets;
	};


	CSockWnd *CSockWnd::m_socketWnd = NULL;
	CStdStringPtrMap CSockWnd::m_sockets;


	CSockWnd::CSockWnd()
	{
	}


	CSockWnd::~CSockWnd()
	{
	}


	LRESULT CSockWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WORD     ev, err;
		TCHAR    key[32];
		CSock *socket;

		if (uMsg != EVENT_SOCKET)
		{
			return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		}

		wsprintf(key, _T("%d"), (int)wParam);

		socket = (CSock *)m_sockets.Find(key);

		if (socket == NULL)
		{
			return 0;
		}

		ev = WSAGETSELECTEVENT(lParam);
		err = WSAGETSELECTERROR(lParam);

		if (err != 0)
		{
			if (socket->m_callback)
				socket->m_callback->OnSocketError(socket, (int)err);
			return 0;
		}

		switch (ev)
		{

		case FD_CONNECT:
			if (socket->m_callback)
				socket->m_callback->OnSocketConnect(socket);
			break;

		case FD_CLOSE:
			if (socket->m_callback)
				socket->m_callback->OnSocketClose(socket);
			break;

		case FD_READ:
			if (socket->m_callback)
				socket->m_callback->OnSocketRead(socket);
			break;

		case FD_WRITE:
			if (socket->m_callback)
				socket->m_callback->OnSocketWrite(socket);
			break;

		default:
			break;
		}

		return 0;
	}


	void CSockWnd::Init()
	{
		if (m_socketWnd != NULL)
		{
			return;
		}

		m_socketWnd = new CSockWnd();
		m_socketWnd->Create(NULL, _T("SocketWnd"), UI_WNDSTYLE_FRAME, 0, 0, 0, 0, 0);
		::ShowWindow(*m_socketWnd, SW_HIDE);
	}


	bool CSockWnd::AddSocket(CSock *socket)
	{
		TCHAR key[32];

		if (WSAAsyncSelect(socket->m_socket, m_socketWnd->m_hWnd, EVENT_SOCKET, FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE) == -1)
		{
			return false;
		}

		wsprintf(key, _T("%d"), (int)socket->m_socket);

		return m_sockets.Insert(key, socket);
	}


	void CSockWnd::RemoveSocket(CSock *socket)
	{
		TCHAR key[32];

		wsprintf(key, _T("%d"), (int)socket->m_socket);

		m_sockets.Remove(key);
	}

	/////////////////////////////////////////////////
	CSock::CSock()
		: m_socket(INVALID_SOCKET)
		, m_callback(NULL)
		, m_bConnect(false)
	{
		CSockWnd::Init();
	}


	CSock::~CSock()
	{
		if (m_socket != INVALID_SOCKET)
		{
			Close();
		}
	}


	bool CSock::Open(char *host, int port)
	{
		int                  rc, err;
		struct hostent      *h;
		struct sockaddr_in   sin;

		h = gethostbyname(host);
		if (h == NULL)
		{
			return false;
		}

		if (m_socket != INVALID_SOCKET)
		{
			Close();
		}

		m_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket == INVALID_SOCKET)
		{
			return false;
		}

		if (!CSockWnd::AddSocket(this))
		{
			return false;
		}

		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = *((u_long *)h->h_addr_list[0]);
		sin.sin_port = htons((unsigned short)port);

		rc = connect(m_socket, (struct sockaddr *) &sin, sizeof(struct sockaddr_in));

		if (rc == -1)
		{
			err = WSAGetLastError();

			if (err != WSAEWOULDBLOCK)
			{
				return false;
			}

			m_bConnect = true;
			return true;
		}

		/* rc == 0 */

		/* TODO: post one WM_SOCKET and FD_CONNECT message into queue */

		m_bConnect = true;
		return true;
	}


	void CSock::Close()
	{
		if (m_socket == INVALID_SOCKET)
		{
			return;
		}

		closesocket(m_socket);

		CSockWnd::RemoveSocket(this);

		m_socket = INVALID_SOCKET;
		m_bConnect = false;
	}


	int CSock::Send(void *buf, size_t size)
	{
		if (m_socket == INVALID_SOCKET)
		{
			return -1;
		}

		return send(m_socket, (char *)buf, size, 0);
	}


	int CSock::Recv(void *buf, size_t size)
	{
		if (m_socket == INVALID_SOCKET)
		{
			return -1;
		}

		return recv(m_socket, (char *)buf, size, 0);
	}

}