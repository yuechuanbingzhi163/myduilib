#include "stdafx.h"
#include "BaseWindow.h"


/*************************************************************************************/

CBaseWindow::CBaseWindow()
{

}

CBaseWindow::~CBaseWindow()
{
	
}

LPCTSTR CBaseWindow::GetWindowClassName() const
{
	return _T("YTingWin");
}
UINT CBaseWindow::GetClassStyle() const
{
	return CS_DBLCLKS/*|CS_DROPSHADOW*/;//|CS_VREDRAW|CS_HREDRAW|CS_SAVEBITS*/;
}

LRESULT CBaseWindow::ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(WM_WEB_SERVICE==uMsg)
		return OnWebService(uMsg,wParam,lParam,bHandled);

	return __super::ProcessWindowMessage(uMsg,wParam,lParam,bHandled);
}

LRESULT CBaseWindow::OnWebService(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	void *pRequest = (void *)wParam;
	void *proto = (void *)lParam;

	ASSERT(pRequest);
//	ASSERT(proto);
	this->OnWebService(pRequest,proto);

// 	if(proto)
// 	{
// 		delete proto;
// 	}
	if(pRequest)
	{
		delete pRequest;
	}


	bHandled = true;
	return 0;
}

void CBaseWindow::OnWebService(void *pRequest ,void *pProto)
{

}

