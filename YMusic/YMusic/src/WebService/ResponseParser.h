#pragma once

#include <map>
#include "../Utilty/lock/RzLock.h"
// class proto::base;
 typedef void* (*protoCreaterT)();

	class CResponseParser
	{
	public:
		CResponseParser();
		void* Parse(unsigned int RequestType, std::string &sResponse);
	protected:
		typedef std::map<unsigned int,protoCreaterT> ParseFuncMapT;
		typedef ParseFuncMapT::iterator ParseFuncMapIterT;
		typedef CRzLock lockT;

		ParseFuncMapT m_ParseFuncMap;
		lockT         m_lock;
	};

