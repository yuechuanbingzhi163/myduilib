#if !defined(AFX_LISTENINGSOCKET_H__22F35FCD_9C1C_49A5_A70C_DA901A831035__INCLUDED_)
#define AFX_LISTENINGSOCKET_H__22F35FCD_9C1C_49A5_A70C_DA901A831035__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListeningSocket.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListeningSocket command target
class CMainWindow;
class CListeningSocket : public CSocket
{
//	DECHARE_DYNAMIC(CListeningSocket);
// Attributes
public:
	CMainWindow* m_pDlg;

// Operations
public:
	CListeningSocket();
	CListeningSocket(CMainWindow* pDlg);
	virtual ~CListeningSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListeningSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListeningSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENINGSOCKET_H__22F35FCD_9C1C_49A5_A70C_DA901A831035__INCLUDED_)
