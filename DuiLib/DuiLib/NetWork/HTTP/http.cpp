/*
 * Copyright (C) Ngwsx
 */
#include "stdafx.h"
#include "http.h"

namespace DuiLib
{
CHttpLink::CHttpLink()
    : m_state(0)
    , m_requestLen(0)
    , m_content(NULL)
    , m_contentLen(0)
    , m_httpcall(NULL)
{

}


CHttpLink::~CHttpLink()
{
    if (m_content == NULL) 
    {
        return;
    }

    free(m_content);
    m_content = NULL;
}


bool CHttpLink::SendRequest(char *host, int port, char *uri)
{
    /* TODO: setting timeout */

    m_response = m_responseBuf;
    m_responseSize = sizeof(m_responseBuf) - 1;
    m_responseLen = 0;

    m_statusCode = 0;
    m_contentLen = 0;
    m_headerDone = 0;

    if (m_content != NULL) 
    {
        free(m_content);
        m_content = NULL;
    }

	sprintf_s(m_request, 1024, "GET %s HTTP/1.1\r\n"
		"Host: %s:%d\r\n\r\n",
		uri, host, port);
    //callback this
    CSock::Init(this);

    return CSock::Open(host, port);
}


void CHttpLink::OnSocketConnect(CSock *s)
{
    OnSocketWrite(s);
}


void CHttpLink::OnSocketClose(CSock *s)
{
    Close();

    if (this->m_httpcall)
        this->m_httpcall->OnHttpClose(this);
}


void CHttpLink::OnSocketRead(CSock *s)
{
    int    size, n;
    char  *p, *last, ch;

    n = Recv(m_response + m_responseLen, m_responseSize - m_responseLen);

    if (n == -1) {
        return;
    }

    if (n == 0) {
        /* TODO */
        return;
    }

    m_responseLen += n;

    if (m_statusCode == 0) {
        p = m_response + sizeof("HTTP/1.1 ") - 1;
        ch = *(p + 3);
        *(p + 3) = '\0';

        m_statusCode = atoi(p);
        if (m_statusCode != 200) {
            /* TODO */
            return;
        }

        *(p + 3) = ch;
    }

    if (m_contentLen == 0) {
        m_response[m_responseLen] = '\0';

        p = strstr(m_response, "Content-Length: ");
        if (p == NULL) {
            return;
        }

        p += sizeof("Content-Length: ") - 1;

        last = strstr(p, "\r\n");
        if (last == NULL) {
            return;
        }

        ch = *last;
        *last = '\0';
        m_contentLen = atoi(p);
        *last = ch;
    }

    if (m_content == NULL && m_contentLen > 0) {
        m_content = (char *) malloc(m_contentLen);
        if (m_content == NULL) {
            return;
        }
    }

    if (!m_headerDone && m_content != NULL) {
        p = strstr(m_response, "\r\n\r\n");
        if (p == NULL) {
            return;
        }

        p += 4;

        size = m_responseLen - (p - m_response);
        if (size > 0) {
            memcpy(m_content, p, size);
            m_responseLen = size;
        }

        m_response = m_content;
        m_responseSize = m_contentLen;

        m_headerDone = 1;
    }

    if (m_headerDone && m_responseLen == m_responseSize)
    {
        if (this->m_httpcall)
            this->m_httpcall->OnHttpRead(this, m_content, m_contentLen);
    }
}


void CHttpLink::OnSocketWrite(CSock *s)
{
    int  n;

    if (m_requestLen == 0) {
        return;
    }

    m_state = STATE_SENDING;

    n = Send(m_request, m_requestLen);

    if (n == -1) {
        return;
    }

    if (n == m_requestLen) {
        m_state = STATE_SENT;
        m_responseLen = 0;
    }

    m_requestLen -= n;
}


void CHttpLink::OnSocketError(CSock *s, int error)
{
    if (this->m_httpcall)
        this->m_httpcall->OnHttpError(this, error);
}
}