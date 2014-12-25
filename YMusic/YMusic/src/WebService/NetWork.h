#pragma once

#ifdef NETWORK_DLL_EXPORT
#define NETWORK_API __declspec(dllexport)
#else
#define NETWORK_API __declspec(dllimport)
#pragma comment(lib,"network.lib")
#endif

typedef void* hLinker;
typedef void* hListener;

class ILinker
{
public:
	virtual void onConnected(hLinker h) = 0;
	virtual void onDisconnected(hLinker h,const int ec) = 0;
	virtual void onSendCompleted(hLinker h,const void* pbuffer,const int ec,size_t bytes) = 0;
	virtual size_t onRecvCompleted(hLinker h,const void* pbuffer,size_t bytes) = 0;
	virtual void onDestroy(hLinker h) = 0;
};

class IListener
{
public:
	virtual void onConnected(hListener,hLinker) = 0;
	virtual void onDisconnected(hListener,hLinker,const int ec) = 0;
	virtual void onSendCompleted(hListener,hLinker,const void* pbuffer,const int ec,size_t bytes) = 0;
	virtual size_t onRecvCompleted(hListener,hLinker,const void* pbuffer,size_t bytes) = 0;
	virtual void onDestroy(hListener,hLinker) = 0;
};

struct xiami_network_handler
{
	unsigned int nsize;
	unsigned int nver;

	bool (*init)(unsigned int iocnt,unsigned int threadcnt);
	void (*asynrun)();
	void (*run)();
	void (*stop)();

	void (*setNetSendSpeed)(unsigned int);
	void (*setNetRecvSpeed)(unsigned int);
    unsigned int (*getNetSendSpeed)();
    unsigned int (*getNetRecvSpeed)();

	hListener (*createListener)(IListener*,unsigned int clinetTimeoutMillsec,unsigned int nMaxLinkCnt);
	bool (*listen)(hListener,unsigned int uport,const char* ip,bool breuseaddr);
	void (*closeListener)(hListener);
	bool (*asynsendall)(hListener,const void*,unsigned int);
	void (*destroyListener)(hListener);

	hLinker (*createLinker)(ILinker*);
	bool (*closeLinker)(hLinker);
	void (*destroyLinker)(hLinker);

	bool (*connect)(hLinker,const char* ip,const char* sport,unsigned int timeoutsec);
	bool (*asynconnect)(hLinker,const char* ip,const char* sport,unsigned int timeoutsec);

	unsigned int (*send)(hLinker,const void*,unsigned int);
	bool (*asynsend)(hLinker,const void*,unsigned int);
	unsigned int (*recv)(hLinker,void*,unsigned int);
	bool (*setsendbufsize)(hLinker,int);
	bool (*setrecvbufsize)(hLinker,int);

    bool (*isOpen)(hLinker);
	bool (*waitReadable)(hLinker,unsigned int ntimeout);
	bool (*waitWriteable)(hLinker,unsigned int ntimeout);

	unsigned int (*tag)(hLinker);
	void (*setTag)(hLinker,unsigned int);

	void (*setTimeout)(hLinker,unsigned int);
	bool (*isTmimeout)(hLinker);

	void (*setSendSpeed)(hLinker,unsigned int);
	void (*setRecvSpeed)(hLinker,unsigned int);
    unsigned int (*getSendSpeed)(hLinker);
    unsigned int (*getRecvSpeed)(hLinker);
};

#ifdef   __cplusplus
extern "C" {
#endif
	NETWORK_API xiami_network_handler* get_xiami_network();

#ifdef   __cplusplus
}
#endif