#include "stdafx.h"
#include "Exception.h"

namespace DuiLib
{
	Exception::Exception()
		: m_lineNum(0)
	{
	}

	Exception::Exception(const CDuiString& msg)
		: m_msg(msg)
		, m_lineNum(0)
	{
	}

	Exception::Exception(const CDuiString &msg, const CDuiString &filename, unsigned int lineNum)
		: m_msg(msg)
		, m_filename(filename)
		, m_lineNum(0)
	{
	}

	Exception::~Exception()
	{
	}

	const CDuiString& Exception::getMessage() const
	{
		return m_msg;
	}

	void Exception::setMessage(const CDuiString& msg)
	{
		m_msg = msg;
	}

	const CDuiString& Exception::getFilename() const
	{
		return m_filename;
	}

	unsigned int Exception::getLineNum() const
	{
		return m_lineNum;
	}
}