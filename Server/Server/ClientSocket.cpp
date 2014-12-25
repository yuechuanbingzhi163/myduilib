// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ClientSocket.h"
#include "mainwindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::CClientSocket(CMainWindow* pDlg)
{
    m_pDlg=pDlg;
/*	m_pFile=NULL;
	m_pArchiveIn=NULL;
	m_pArchiveOut=NULL;*/

}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::Initialize()
{
/*	m_pFile=new CSocketFile(this);
	m_pArchiveIn=new CArchive(m_pFile,CArchive::load);
	m_pArchiveOut=new CArchive(m_pFile,CArchive::store);*/

}

/*void CClientSocket::SendMassage(CMsg* msg)
{
	if(m_pArchiveOut!=NULL)
	{
		msg->Serialize(*m_pArchiveOut);
		m_pArchiveOut->Flush();*
	}

}*/

/*void CClientSocket::ReceiveMessage(CMsg* msg)
{
	//	msg->Serialize(*m_pArchiveIn);
}*/

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnReceive(int nErrorCode) 
{
//	AfxMessageBox("CClientSocket::OnReceive");
//	AfxMessageBox("CClientSocket::OnReceive");
	CSocket::OnReceive(nErrorCode);
	if(nErrorCode==0)
	m_pDlg->OnReceive(this);
}


//CMsg* CClientSocket::ReadMsg(CClientSocket* pSocket)
