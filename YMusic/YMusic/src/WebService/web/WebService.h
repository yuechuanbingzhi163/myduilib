#pragma once
#include "UrlService.h"
#include "NetWork.h"


	class CHttpLink;
	class CWebServiceMgr :
		public ILinker
	{
	public:
		typedef url::CRequest* lpRequest;
		typedef url::CRequest::ResponderT ResponderT;
		CWebServiceMgr();
		~CWebServiceMgr();
	public:	
		void SetNotifyMsg(UINT msg){_nMsg = msg;}
		std::string BuildHttpHeader(lpRequest pRequest,bool bPost = false);
		void ClearRequest();
		bool IsOnline()const {return _isOnline;}
	public:
		void SendRequest(lpRequest pRequest);
		void SendRequest(HWND hwnd,lpRequest pRequest,DWORD dwTag);
		void SendRequest(HWND hwnd,lpRequest pRequest);
		void SendRequest(lpRequest pRequest,ResponderT responder,DWORD dwTag);
		void SendRequest(lpRequest pRequest,ResponderT responder);
		void SendRequest(lpRequest pRequest,CBuffer& buffer);

		void AsynSendRequest(lpRequest pRequest);
		void AsynSendRequest(HWND hwnd,lpRequest pRequest,DWORD dwTag);
		void AsynSendRequest(HWND hwnd,lpRequest pRequest);
		void AsynSendRequest(lpRequest pRequest,ResponderT responder,DWORD dwTag);
		void AsynSendRequest(lpRequest pRequest,ResponderT responder);
	
		void PostRequest(lpRequest pRequest);
		void PostRequest(HWND hwnd,lpRequest pRequest,DWORD dwTag);
		void PostRequest(HWND hwnd,lpRequest pRequest);
		void PostRequest(lpRequest pRequest,ResponderT responder,DWORD dwTag);
		void PostRequest(lpRequest pRequest,ResponderT responder);

		void AsynPostRequest(lpRequest pRequest);
		void AsynPostRequest(HWND hwnd,lpRequest pRequest,DWORD dwTag);
		void AsynPostRequest(HWND hwnd,lpRequest pRequest);
		void AsynPostRequest(lpRequest pRequest,ResponderT responder,DWORD dwTag);
		void AsynPostRequest(lpRequest pRequest,ResponderT responder);
	private:
		void onAsynRequest(lpRequest pRequest,bool bPost = false);
		void OnRequest(lpRequest pRequest,bool bPost = false);
		void OnResponse(lpRequest pRequest,CBuffer& buffer);
		void OnResponse(lpRequest pRequest,void* pResponse);
		

		bool GetRequestFromCache(const std::string& headerStr,CBuffer& buffer);
	protected:
		virtual void onConnected(hLinker h);
		virtual void onDisconnected(hLinker h,const int ec);
		virtual void onSendCompleted(hLinker h,const void* pbuffer,const int ec,size_t bytes);
		virtual size_t onRecvCompleted(hLinker h,const void* pbuffer,size_t bytes);
		virtual void onDestroy(hLinker h);
	private:
//		typedef boost::shared_ptr<CHttpLink> spHttpLinkT;
		typedef smart_pointer::SmartPtr<CHttpLink> spHttpLinkT;
		UINT              _nMsg;
		//spOperationQueueT _pOps;
		bool              _isOnline;
		CWebServiceCatch  _webCache;

		std::vector<std::string> _serversIp;
	};

