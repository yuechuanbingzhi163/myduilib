#ifndef __URL_H__
#define __URL_H__

#pragma once
#include "../../YMusic.h"

// #include <string>
// using namespace std;

namespace em_utility
{
	class UrlPraser : public Singleton<UrlPraser>
	{
	protected:
		DECLARE_SINGLETON_CLASS(UrlPraser)
	public:
		UrlPraser(){}
		UrlPraser(const char* url);
		bool Init(const char* url);
		const std::string GetScheme() const
		{
			return m_scheme;
		}
		const std::string GetHost() const
		{
			return m_host;
		}
		const std::string GetPort() const
		{
			return m_port;
		}
		const std::string GetPath() const
		{
			return m_path;
		}
		const std::string GetQuery() const
		{
			return m_query;
		}
		const std::string GetFragment() const
		{
			return m_fragment;
		}
		const std::string GetUserName() const
		{
			return m_username;
		}
		const std::string GetPassword() const
		{
			return m_password;
		}
		const std::string GetFileName() const
		{
			return m_filename;
		}
		std::string GetParameter(const std::string query,const char* param);
		std::string GetParameter(const char* param) ;
	protected:		
		bool IsSchemeChar(int c)
		{
			return (!isalpha(c) && '+' != c && '-' != c && '.' != c) ? 0 : 1;
		}
		// 版本一
		inline void split(const std::string& s, char c, std::vector<std::string>& v) {
			std::string::size_type index = 0;
			std::string::size_type position = s.find(c);

			while (position != std::string::npos) {
				v.push_back(s.substr(index, position - index));
				index = ++position;
				position = s.find(c, position);
			}
			v.push_back(s.substr(index, s.length()));
		}
		// 版本二
// 		inline vector<string> &split1(const std::string &s, char delim, vector<std::string> &elems) {
// 			std::istringstream iss(s);
// 			string item;
// 			while (getline(iss, item, delim)) {
// 				elems.push_back(item);
// 			}
// 			return elems;
// 		}

		//版本一 豪华型
		template<typename T>
		inline void split_t(const std::basic_string<T>& s, T c, std::vector< std::basic_string<T> >& v) {
			typename std::basic_string<T>::size_type index = 0;
			typename std::basic_string<T>::size_type position = s.find(c);

			while (position != std::basic_string<T>::npos) {
				v.push_back(s.substr(index, position - index));
				index = ++position;
				position = s.find(c, position);
			}
			v.push_back(s.substr(index, s.length()));
		}
	private:
		std::string m_scheme;               /* mandatory *///协议
		std::string m_host;                 /* mandatory *///主机
		std::string m_port;                 /* optional *///端口
		std::string m_path;                 /* optional *///路劲
		std::string m_query;                /* optional *///查询
		std::string m_fragment;             /* optional *///
		std::string m_username;             /* optional *///
		std::string m_password;             /* optional *///
		std::string m_filename;				/* optional *///文件名
		std::map<std::string,std::string>  m_param;
	};
}
#endif//__URL_H__