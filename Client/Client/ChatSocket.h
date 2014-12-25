#if !defined(AFX_CHATSOCKET_H__B31DAF3F_C5C7_4A55_8297_F162C13A4FA6__INCLUDED_)
#define AFX_CHATSOCKET_H__B31DAF3F_C5C7_4A55_8297_F162C13A4FA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatSocket.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CChatSocket command target
class CMainWindow;

class CChatSocket : public CSocket
{
//		DECHARE_DYNAMIC(CChatSocket);
// Attributes
public:
	CMainWindow* m_pDlg;
//	CSocketFile* m_pFle;
//	CArchice* CArchiveIn

// Operations
public:
	CChatSocket();
	CChatSocket(CMainWindow* pDlg);
	virtual ~CChatSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CChatSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATSOCKET_H__B31DAF3F_C5C7_4A55_8297_F162C13A4FA6__INCLUDED_)
