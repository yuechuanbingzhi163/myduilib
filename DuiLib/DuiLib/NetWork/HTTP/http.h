#ifndef _HTTP_H
#define _HTTP_H
#pragma once

#include "../SOCKET/sock.h"

namespace DuiLib
{
class CHttpLink;

class UILIB_API IHttpLink
{
public:
    virtual void OnHttpClose(CHttpLink *http) = 0;
    virtual void OnHttpRead(CHttpLink *http, char *content, int len) = 0;
    virtual void OnHttpError(CHttpLink *http, int error) = 0;
};

class UILIB_API CHttpLink : public CSock, public ISock
{
public:

	enum HTTP_STATE
	{
		STATE_SENDING = 1,
		STATE_SENT = 2,
	};

    CHttpLink();
    ~CHttpLink();

    bool SendRequest(char *host, int port, char *uri);

    void Init(IHttpLink *callback) { m_httpcall = callback; }
protected:
    void OnSocketConnect(CSock *s);
    void OnSocketClose(CSock *s);
    void OnSocketRead(CSock *s);
    void OnSocketWrite(CSock *s);
    void OnSocketError(CSock *s, int error);

protected:
	IHttpLink* m_httpcall;
    int m_state;

    char m_request[1024];
    int m_requestLen;

    char m_responseBuf[4096];
    char *m_response;
    int m_responseSize;
    int m_responseLen;

    int m_statusCode;
    int m_contentLen;
    int m_headerDone;

    char *m_content;
};
}
#endif//_HTTP_H