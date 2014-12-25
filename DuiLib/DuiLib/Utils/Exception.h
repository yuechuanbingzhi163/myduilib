#ifndef __DUILIB_EXCEPTION_H__
#define __DUILIB_EXCEPTION_H__

#pragma once

namespace DuiLib
{
	class UILIB_API Exception
	{
	public:
		Exception();
		Exception(const CDuiString& msg);
		Exception(const CDuiString& msg, const CDuiString& filename, unsigned int lineNum);
		virtual ~Exception();

		//Methods
	public:
		virtual const CDuiString&	getMessage() const;
		virtual void			    setMessage(const CDuiString &msg);

		virtual const CDuiString&	getFilename() const;
		virtual unsigned int		getLineNum() const;

	protected:
		CDuiString		m_msg;
		CDuiString		m_filename;
		unsigned int	m_lineNum;
	};

	inline void __DuiThrowException(const CDuiString& msg, const CDuiString& filename, unsigned int lineNum)
	{
		throw Exception(msg, filename, lineNum);
	};

#define DuiException(formats, ...) __DuiThrowException(DuiLib::CDuiString::FormatString(formats, ##__VA_ARGS__), __FILE__, __LINE__)
}

#endif