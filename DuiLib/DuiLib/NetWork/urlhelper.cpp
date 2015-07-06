#include "stdafx.h"
#include "urlhelper.h"

namespace DuiLib
{
	URLHelper::URLHelper(const char* url)
	{
		assert(Init(url));
	}

	bool URLHelper::Init(const char* url)
	{
		m_url.assign(url);
		const char *tmpstr;
		const char *curstr;
		int len;
		int i;
		int userpass_flag;
		int bracket_flag;

		curstr = url;

		/*
		* <scheme>:<scheme-specific-part>
		* <scheme> := [a-z\+\-\.]+
		*             upper case = lower case for resiliency
		*/
		/* Read scheme */
		tmpstr = strchr(curstr, ':');
		if ( NULL == tmpstr ) {
			/* Not found the character */
			return false;
		}
		/* Get the scheme length */
		len = tmpstr - curstr;
		/* Check restrictions */
		for ( i = 0; i < len; i++ ) {
			if ( !IsSchemeChar(curstr[i]) ) {
				/* Invalid format */
				return false;
			}
		}
		/* Copy the scheme to the storage */
		m_scheme.assign(curstr, len);

		/* Make the character to lower if it is upper case. */
		for ( i = 0; i < len; i++ ) {
			m_scheme[i] = tolower(m_scheme[i]);
		}

		/* Skip ':' */
		tmpstr++;
		curstr = tmpstr;

		/*
		* //<user>:<password>@<host>:<port>/<UrlPraser-path>
		* Any ":", "@" and "/" must be encoded.
		*/
		/* Eat "//" */
		for ( i = 0; i < 2; i++ ) {
			if ( '/' != *curstr ) {
				return false;
			}
			curstr++;
		}

		/* Check if the user (and password) are specified. */
		userpass_flag = 0;
		tmpstr = curstr;
		while ( '\0' != *tmpstr ) {
			if ( '@' == *tmpstr ) {
				/* Username and password are specified */
				userpass_flag = 1;
				break;
			} else if ( '/' == *tmpstr ) {
				/* End of <host>:<port> specification */
				userpass_flag = 0;
				break;
			}
			tmpstr++;
		}

		/* User and password specification */
		tmpstr = curstr;
		if ( userpass_flag ) {
			/* Read username */
			while ( '\0' != *tmpstr && ':' != *tmpstr && '@' != *tmpstr ) {
				tmpstr++;
			}
			len = tmpstr - curstr;
			m_username.assign(curstr, len);

			/* Proceed current pointer */
			curstr = tmpstr;
			if ( ':' == *curstr ) {
				/* Skip ':' */
				curstr++;
				/* Read password */
				tmpstr = curstr;
				while ( '\0' != *tmpstr && '@' != *tmpstr ) {
					tmpstr++;
				}
				len = tmpstr - curstr;
				m_password.assign(curstr, len);
				curstr = tmpstr;
			}
			/* Skip '@' */
			if ( '@' != *curstr ) {
				return false;
			}
			curstr++;
		}

		if ( '[' == *curstr ) {
			bracket_flag = 1;
		} else {
			bracket_flag = 0;
		}
		/* Proceed on by delimiters with reading host */
		tmpstr = curstr;
		while ( '\0' != *tmpstr ) {
			if ( bracket_flag && ']' == *tmpstr ) {
				/* End of IPv6 address. */
				tmpstr++;
				break;
			} else if ( !bracket_flag && (':' == *tmpstr || '/' == *tmpstr) ) {
				/* Port number is specified. */
				break;
			}
			tmpstr++;
		}
		len = tmpstr - curstr;
		m_host.assign(curstr, len);
		curstr = tmpstr;

		/* Is port number specified? */
		if ( ':' == *curstr ) {
			curstr++;
			/* Read port number */
			tmpstr = curstr;
			while ( '\0' != *tmpstr && '/' != *tmpstr ) {
				tmpstr++;
			}
			len = tmpstr - curstr;
			m_port.assign(curstr, len);
			curstr = tmpstr;
		}

		/* End of the string */
		if ( '\0' == *curstr ) {
			return true;
		}

		/* Skip '/' */
		//if ( '/' != *curstr ) {
		//return false;
		//}
		//curstr++;

		/* Parse path */
		tmpstr = curstr;
		while ( '\0' != *tmpstr && '#' != *tmpstr  && '?' != *tmpstr ) {
			tmpstr++;
		}
		len = tmpstr - curstr;

		m_path.assign(curstr, len);
		curstr = tmpstr;

		/* Is query specified? */
		if ( '?' == *curstr ) {
			/* Skip '?' */
			curstr++;
			/* Read query */
			tmpstr = curstr;
			while ( '\0' != *tmpstr && '#' != *tmpstr ) {
				tmpstr++;
			}
			len = tmpstr - curstr;
			m_query.assign(curstr, len);
			curstr = tmpstr;
		}
		/* Is Paramter specified? */
		std::vector<std::string> v;
		this->split(m_query, '&', v);
		for (size_t i=0;i<v.size();++i)
		{
			std::vector<std::string> m;
			this->split(v[i],'=',m);
			if(m.size()==2)
				m_param[ m[0] ] = m[1];
		}
		/* Is fragment specified? */
		if ( '#' == *curstr ) {
			/* Skip '#' */
			curstr++;
			/* Read fragment */
			tmpstr = curstr;
			while ( '\0' != *tmpstr ) {
				tmpstr++;
			}
			len = tmpstr - curstr;
			m_fragment.assign(curstr, len);
			curstr = tmpstr;
		}


		return true;
	}

	std::string URLHelper::parameter(const std::string query,const char* param)
	{
		assert(param);
		if(!param) return "";
		if (query.length()==0||strlen(param)==0)
		{
			return "";
		}
		std::map<std::string,std::string>  v_param;
		std::vector<std::string> v;
		this->split(query, '&', v);
		for (size_t i=0;i<v.size();++i)
		{
			std::vector<std::string> m;
			this->split(v[i],'=',m);
			if(m.size()==2)
				v_param[ m[0] ] = m[1];
		}
		std::map<std::string,std::string>::iterator it = v_param.find(param);
		if (it != v_param.end())
		{
			return it->second;
		}
		return "";
	}
	//username=wustfeng&password=199010&useCookie=1
	std::string URLHelper::parameter(const char* param) 
	{
		assert(param);
		if(!param) return "";
		if (m_query.length()==0||strlen(param)==0)
		{
			return "";
		}
		
		std::map<std::string,std::string>::iterator it = m_param.find(param);
		if (it != m_param.end())
		{
			return it->second;
		}
		return "";
	}
}