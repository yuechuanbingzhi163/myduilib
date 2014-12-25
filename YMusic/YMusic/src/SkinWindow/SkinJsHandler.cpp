#include "stdafx.h"
#include "SkinjsHandler.h"
#include "../YApp.h"
#include "../Utilty/UrlPraser.h"


/************************************************************************/
/*                                                                      */
/************************************************************************/
const TCHAR CSkinJsHandler::WEB_TO_APP_JSFUNNAME[] = _T("InvokeApp");
const TCHAR CSkinJsHandler::APP_TO_WEB_JSFUNNAME[] = _T("InvokeWeb");
CSkinJsHandler::CSkinJsHandler(CWebBrowserUI* pWebUI):_refNum(1),_pWebUI(pWebUI)
{

}
CSkinJsHandler::~CSkinJsHandler()
{

}
void CSkinJsHandler::RegisterJsCallBack(LPCTSTR lpszName,JsCallBackT f)
{
	ASSERT(lpszName);
	ASSERT(f);
	if(!lpszName || !f)
		return;

	_jsEventMap.insert(JsEventMapT::value_type(lpszName,f));
}

void CSkinJsHandler::UnRegisterJsCallBack(LPCTSTR lpszName)
{
	ASSERT(lpszName);
	if(!lpszName)
		return;

	_jsEventMap.erase(lpszName);
}

BOOL CSkinJsHandler::CallJsFun(LPCTSTR lpszFunName,LPCTSTR sParams,VARIANT *pVarResult)
{
	ASSERT(_pWebUI);
// 	if(_pWebUI)
// 		return _pWebUI->CallJsFun((LPOLESTR)lpszFunName,pVarResult,params,cArgs);
	return FALSE;
}
BOOL CSkinJsHandler::InvokeWeb(LPCTSTR lpszFunName,LPCTSTR sParam)
{
	ASSERT(_pWebUI);
	if(_pWebUI)
	{
		VARIANT vars[2] = {0};
		vars[1].vt	    = VT_BSTR;
		vars[1].bstrVal =  SysAllocString(lpszFunName);
		vars[0].vt	    = VT_BSTR;
		vars[0].bstrVal =  SysAllocString(sParam);
		BOOL bRet = _pWebUI->CallJsFun((LPOLESTR)APP_TO_WEB_JSFUNNAME,NULL,&vars[0],2);
		ASSERT(bRet);
		VariantClear(&vars[0]);
		VariantClear(&vars[1]);
		return bRet;
	}
	return FALSE;
}

// IUnknown Methods
STDMETHODIMP CSkinJsHandler::QueryInterface(REFIID iid,void**ppvObject)
{
	*ppvObject = NULL;
	if (iid == IID_IUnknown)	*ppvObject = this;
	else if (iid == IID_IDispatch)	*ppvObject = (IDispatch*)this;
	if(*ppvObject)
	{
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CSkinJsHandler::AddRef()
{
	return ::InterlockedIncrement(&_refNum);
}

STDMETHODIMP_(ULONG) CSkinJsHandler::Release()
{
	::InterlockedDecrement(&_refNum);
	if(_refNum == 0)
	{
		//delete this;
	}
	return _refNum;
}

// IDispatch Methods
HRESULT _stdcall CSkinJsHandler::GetTypeInfoCount(
	unsigned int * pctinfo) 
{
	return E_NOTIMPL;
}

HRESULT _stdcall CSkinJsHandler::GetTypeInfo(
	unsigned int iTInfo,
	LCID lcid,
	ITypeInfo FAR* FAR* ppTInfo) 
{
	return E_NOTIMPL;
}

HRESULT _stdcall CSkinJsHandler::GetIDsOfNames(
	REFIID riid, 
	OLECHAR FAR* FAR* rgszNames, 
	unsigned int cNames, 
	LCID lcid, 
	DISPID FAR* rgDispId 
	)
{
	ASSERT(rgszNames[0]);
	CString sTrace = _T("CSkinJsHandler::GetIDsOfNames :(");
	for (int i=0;i<cNames-1;++i)
	{
		sTrace += rgszNames[i];
		sTrace += _T(",");
	}
	sTrace += rgszNames[cNames-1];
	sTrace += _T(")");
	sLog(mci::unicode_2_ansi(sTrace).c_str());
	*rgDispId = ((0 ==  _tcsicmp(rgszNames[0],WEB_TO_APP_JSFUNNAME)) ? 1 : 0);
//	*rgDispId = ((0 ==  _tcsicmp(rgszNames[0],_T("callkwmusic"))) ? 1 : 0);
//	ASSERT(*rgDispId);
	return S_OK;
}
BSTR CSkinJsHandler::Variant2BSTR( _variant_t vString )
{
	vString.ChangeType(VT_BSTR);
	return vString.bstrVal;
}
HRESULT _stdcall CSkinJsHandler::Invoke(
	DISPID dispIdMember,
	REFIID riid,
	LCID lcid,
	WORD wFlags,
	DISPPARAMS* pDispParams,
	VARIANT* pVarResult,
	EXCEPINFO* pExcepInfo,
	unsigned int* puArgErr
	)
{
	HRESULT hrReturn = E_NOTIMPL;
	if(1 == dispIdMember && pDispParams && pDispParams->cArgs>0/*&& (1== pDispParams->cArgs || 2 == pDispParams->cArgs)*/)
	{
		CString firstParam;
		CString secondParam;
// 		if(1 == pDispParams->cArgs)
// 			firstParam = pDispParams->rgvarg[0];
// 		else
// 		{
// 			firstParam = pDispParams->rgvarg[1];
// 			secondParam = pDispParams->rgvarg[0];
// 		}
		CString sTrace = _T("CSkinJsHandler::Invoke :(");
		for (int i=0;i<pDispParams->cArgs-1;++i)
		{
			sTrace += pDispParams->rgvarg[i];
			sTrace += _T(",");
		}
		sTrace += pDispParams->rgvarg[pDispParams->cArgs-1];
		sTrace += _T(")");
		sLog(mci::unicode_2_ansi(sTrace).c_str());

		firstParam = pDispParams->rgvarg[pDispParams->cArgs-1];
		if(pDispParams->cArgs >= 2) secondParam = pDispParams->rgvarg[0];
		JsEventMapT::iterator it = _jsEventMap.find(firstParam);
//		ASSERT(it != _jsEventMap.end());
		if(_jsEventMap.end() != it && it->second)
		{
			CString sResult;
			it->second(const_cast<CSkinJsHandler*>(this),secondParam.GetBuffer(),sResult);
			secondParam.ReleaseBuffer();
			if(!sResult.IsEmpty())
			{
				ASSERT(pVarResult);
				pVarResult->vt = VT_BSTR;
				pVarResult->bstrVal = SysAllocString(sResult.GetBuffer());
				sResult.ReleaseBuffer();
			}
			hrReturn = S_OK;
		}
	}

//	ASSERT(FALSE);
	return hrReturn;
}

//webBrow
HRESULT STDMETHODCALLTYPE CSkinJsHandler::GetHostInfo( DOCHOSTUIINFO __RPC_FAR *pInfo)
{
	if (pInfo != NULL)
	{
		pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO | /* DOCHOSTUIFLAG_FLAT_SCROLLBAR  | */ DOCHOSTUIFLAG_NO3DBORDER;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSkinJsHandler::GetExternal(/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
	*ppDispatch = this;
	return S_OK;
}

void CSkinJsHandler::BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel )
{
	CString surl = url->bstrVal;
	sLog("BeforeNavigate2:%s",mci::unicode_2_ansi(surl).c_str());
	if(surl.Left(7) == _T("http://"))
	{
		spLogonUserInfoT theUser = theRuntimeState->GetCurrentContext()->GetUser();
		if(theUser && !Headers->bstrVal)
		{
			CString surl = url->bstrVal;
			IWebBrowser2* pWebBro = _pWebUI->GetWebBrowse2();
			this->NavigateUrl(pWebBro,surl);
			*Cancel = VARIANT_TRUE;
			return;
		}
	}
	

	*Cancel = VARIANT_FALSE;
	//*Cancel = ( (_delegate.OnWillNavigate(this,surl)) ?  VARIANT_FALSE : VARIANT_TRUE);
}

void CSkinJsHandler::NavigateUrl(IWebBrowser2* pWebBro,LPCTSTR url)
{
	ASSERT(pWebBro);
	ASSERT(url);
	if(!pWebBro || !url)
		return;

	std::string requestHeader;
	spLogonUserInfoT theUser = theRuntimeState->GetCurrentContext()->GetUser();
	if(theUser)
	{
		CString str;
		str.Format(_T("%s:%s"),theUser->sLogonEmail,theUser->sPasswordMD5);
		std::string ansiStr = em_utility::mci::unicode_2_ansi(str);
		if(!ansiStr.empty())
		{
			std::string base64Str = ZBase64::Encode((const unsigned char*)ansiStr.c_str(),ansiStr.size());
			if(!base64Str.empty())
			{
				requestHeader += "Authorization:Basic ";
				requestHeader += base64Str;
				requestHeader += "\r\n";
			}
		}
	}

	CString headerStr = em_utility::mci::utf8_2_CString(requestHeader.c_str());

	VARIANT vHeaders;
	VARIANT vNull;

	VariantInit(&vHeaders);
	VariantInit(&vNull);

	vHeaders.vt      = VT_BSTR;
	vHeaders.bstrVal = SysAllocString(headerStr.GetBuffer());

	_variant_t varStr(url);
	pWebBro->Navigate2(&varStr,&vNull,&vNull,&vNull,&vHeaders);
	VariantClear(&vHeaders);
	VariantClear(&vNull);
}
