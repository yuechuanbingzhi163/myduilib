#if !defined(AFX_CLIENTSOCKET_H__0CB6554C_25EC_419E_A0B5_9B787056CF19__INCLUDED_)
#define AFX_CLIENTSOCKET_H__0CB6554C_25EC_419E_A0B5_9B787056CF19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//

//#include "Msg.h"

/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target
class CMainWindow;
class CClientSocket : public CSocket
{
// Attributes
//	DECHARE_DYNAMIC(CClientSocket);
public:
	CMainWindow* m_pDlg;
/*	CSocketFile* m_pFile;
	CArchive* m_pArchiveIn;
	CArchive* m_pArchiveOut;*/

// Operations
public:
	CClientSocket();
	CClientSocket(CMainWindow* pDlg);
	virtual ~CClientSocket();
	void Initialize();
//	void SendMassage(CMsg* msg);
//	void ReceiveMessage(CMsg* msg);
//	void OnReceive(CClientSocket* pSocket);
//	CMsg* ReadMsg(CClientSocket* pSocket);

// Overrides
//public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__0CB6554C_25EC_419E_A0B5_9B787056CF19__INCLUDED_)
