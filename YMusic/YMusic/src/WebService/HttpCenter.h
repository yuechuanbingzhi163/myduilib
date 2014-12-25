#ifndef HttpCenter_h
#define HttpCenter_h

#pragma once
#include "HttpThread.h"

class CHttpCenter : public Singleton<CHttpCenter>
{
public:
	typedef CString          UNICODESTR;
	typedef std::string      UTF8STR;
	CHttpCenter();
	virtual ~CHttpCenter();
public:
	//µÇÂ¼
	virtual bool MakeLogin(UNICODESTR& strUserKey,UNICODESTR& strPassword);
	virtual bool MakeLogin(UNICODESTR& sToken);
	virtual bool MakeLogin(UTF8STR& strUserKey,UTF8STR& strPassword);
	virtual bool MakeLogin(UTF8STR sToken);
	//·ÖÏí
//	virtual bool MakeShare(unsigned int nSendUserId,LPCTSTR lpszCommentMsg,unsigned int nObjId)
	//ÊÕ²Ø
protected:
	DECLARE_SINGLETON_CLASS(CHttpCenter);
private:
	CHttpThread* m_pHttpThread;
};




#endif//HttpCenter_h