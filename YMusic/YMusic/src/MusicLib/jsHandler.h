#pragma once

class CJsHandler : 
	public IDispatch,
	public CWebBrowserEventHandler
{
public:
//	typedef boost::function<void(CJsHandler* jsHandler,VARIANT* pRet,DISPPARAMS* pDispParams)> JsCallBackT;
	typedef void (*JsCallBackT)(CJsHandler*,VARIANT*,DISPPARAMS*);
	typedef std::vector<std::pair<CString,JsCallBackT> > JsEventMapT;
	CJsHandler(CWebBrowserUI* pWebUI);
	virtual ~CJsHandler();
public:
	void RegisterJsCallBack(LPCTSTR lpszName,JsCallBackT f);
	void UnRegisterJsCallBack(LPCTSTR lpszName);

	BOOL CallJsFun( LPOLESTR pName, VARIANT *pVarResult, VARIANT *params = NULL, int cArgs = 0);
protected:// IUnknown Methods
	STDMETHODIMP QueryInterface(REFIID iid,void**ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	// IDispatch Methods
	HRESULT _stdcall GetTypeInfoCount(unsigned int * pctinfo);
	HRESULT _stdcall GetTypeInfo(unsigned int iTInfo,LCID lcid,ITypeInfo FAR* FAR* ppTInfo);
	HRESULT _stdcall GetIDsOfNames(
		REFIID riid, 
		OLECHAR FAR* FAR* rgszNames, 
		unsigned int cNames, 
		LCID lcid, 
		DISPID FAR* rgDispId 
		);
	HRESULT _stdcall Invoke(
		DISPID dispIdMember,
		REFIID riid,
		LCID lcid,
		WORD wFlags,
		DISPPARAMS* pDispParams,
		VARIANT* pVarResult,
		EXCEPINFO* pExcepInfo,
		unsigned int* puArgErr
		);

protected:  //webBrow
	HRESULT STDMETHODCALLTYPE GetExternal(/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);;
protected:
	DWORD            _refNum;
	JsEventMapT     _jsEventMap;
	CWebBrowserUI*  _pWebUI;
};



class CSimpleJsHandler :
	public IDispatch,
	public CWebBrowserEventHandler
{
	static const TCHAR WEB_TO_APP_JSFUNNAME[];
	static const TCHAR APP_TO_WEB_JSFUNNAME[];
public:
	typedef void (*JsCallBackT)(CSimpleJsHandler*,LPCTSTR,CString&) ;
	typedef std::map<CString,JsCallBackT> JsEventMapT;

public:
	CSimpleJsHandler(CWebBrowserUI* pWebUI);
	virtual ~CSimpleJsHandler();
public:
	void RegisterJsCallBack(LPCTSTR lpszName,JsCallBackT f);

	void UnRegisterJsCallBack(LPCTSTR lpszName);

	BOOL CallJsFun(LPCTSTR lpszFunName,LPCTSTR sParams,VARIANT *pVarResult);
	BOOL InvokeWeb(LPCTSTR lpszFunName,LPCTSTR sParam);

	void NavigateUrl(IWebBrowser2* pWebBro,LPCTSTR url);
protected:// IUnknown Methods
	STDMETHODIMP QueryInterface(REFIID iid,void**ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	// IDispatch Methods
	HRESULT _stdcall GetTypeInfoCount(unsigned int * pctinfo);
	HRESULT _stdcall GetTypeInfo(unsigned int iTInfo,LCID lcid,ITypeInfo FAR* FAR* ppTInfo);
	HRESULT _stdcall GetIDsOfNames(
		REFIID riid, 
		OLECHAR FAR* FAR* rgszNames, 
		unsigned int cNames, 
		LCID lcid, 
		DISPID FAR* rgDispId 
		);
	HRESULT _stdcall Invoke(
		DISPID dispIdMember,
		REFIID riid,
		LCID lcid,
		WORD wFlags,
		DISPPARAMS* pDispParams,
		VARIANT* pVarResult,
		EXCEPINFO* pExcepInfo,
		unsigned int* puArgErr
		);

protected:  //webBrow
	virtual HRESULT STDMETHODCALLTYPE GetHostInfo( DOCHOSTUIINFO __RPC_FAR *pInfo);
	HRESULT STDMETHODCALLTYPE GetExternal(/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
	void BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel );
protected:
	DWORD            _refNum;
	JsEventMapT     _jsEventMap;
	CWebBrowserUI*  _pWebUI;
};
