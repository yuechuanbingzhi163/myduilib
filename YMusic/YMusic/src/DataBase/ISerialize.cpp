#include "stdafx.h"
#include "ISerialize.h"
#include <string>
#include <codecvt>
//#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "boost/program_options/detail/convert.hpp"
#include "boost/program_options/detail/utf8_codecvt_facet.hpp"
#include "shareTingApp.h"


bool ISerialize::serial(node_type& node,bool isload,LPCTSTR nodename)
{
	assert(!node.empty());
	assert(nodename);

	if(isload)
	{
		node_itor_type it = node.find(nodename);
		if(node.not_found() == it)
			return assert(false),false;
		return serial(it->second,isload);
	}
	else
	{
		node_type new_node;
		if(!serial(new_node,isload))
			return false;

		node.add_child(nodename,new_node);
	}

	return true;
}
bool ISerialize::Load(ISerialize* pIserial)
{
	assert(pIserial);
	if(pIserial)
	{
		node_type node;
		if(pIserial->serial(node,false))
			return this->serial(node,true);
	}
	return false;
}
bool ISerialize::Load(const std::string& s)
{
	typedef std::wstring_convert<std::codecvt_utf8<wchar_t> > w2utf8CoderT;
	w2utf8CoderT coder;
	node_type node;
	try
	{
		std::wistringstream stream(coder.from_bytes(s));
		read_json(stream,node);
		return serial(node,true);
	}
	catch(std::exception& e)
	{
		sLogError("LoadFromFile error(%s)",e.what());
	}
	catch (...)
	{
	}
	assert(false);
	return false;
}
bool ISerialize::Load(LPCTSTR s)
{
	std::locale oldLocale;
	std::locale utf8Locale(oldLocale,new boost::program_options::detail::utf8_codecvt_facet());
	std::wistringstream jsonIStream;
	jsonIStream.imbue(utf8Locale);
	jsonIStream.str(s);

	try
	{
		node_type node;
		read_json(jsonIStream,node);
		return serial(node,true);
	}
	catch(std::exception& e)
	{
		sLogError("LoadFromFile error(%s)",e.what());
	}
	catch (...)
	{
	}
	assert(false);
	return false;
}
bool ISerialize::LoadFromFile(LPCTSTR sFile)
{
	assert(sFile);
	std::locale oldLocale;
	std::locale utf8Locale(oldLocale,new boost::program_options::detail::utf8_codecvt_facet());
	std::wifstream strem(sFile);
	strem.imbue(utf8Locale);

	try
	{
		node_type node;
		read_json(strem,node);
		return serial(node,true);
	}
	catch(std::exception& e)
	{
		sLogError("LoadFromFile error(%s)",e.what());
	}
	catch (...)
	{
	}
	assert(false);
	return false;
}
bool ISerialize::SaveToStream(std::wostringstream& ss)
{
	std::locale oldLocale;
	std::locale utf8Locale(oldLocale,new boost::program_options::detail::utf8_codecvt_facet());

	node_type node;
	serial(node,false);

	ss.imbue(utf8Locale);
	try
	{
		write_json(ss,node);
		return true;
	}
	catch(std::exception& e)
	{
		sLogError("LoadFromFile error(%s)",e.what());
		assert(false);
	}
	catch (...)
	{
		assert(false);
	}
	
	return false;
}
bool ISerialize::SaveToStream(std::ostringstream& ss)
{
	typedef std::wstring_convert<std::codecvt_utf8<wchar_t> > w2utf8CoderT;
	w2utf8CoderT coder;
	std::wostringstream wss;
	if(SaveToStream(wss))
	{
		ss<<(coder.to_bytes(wss.str()));
		return true;
	}
	return false;
}
bool ISerialize::SaveToFile(LPCTSTR sFile)
{
	std::locale oldLocale;
	std::locale utf8Locale(oldLocale,new boost::program_options::detail::utf8_codecvt_facet());
	std::wofstream strem(sFile);
	strem.imbue(utf8Locale);

	try
	{
		node_type node;
		serial(node,false);
		write_json(strem,node);
		return true;
	}
	catch(std::exception& e)
	{
		sLogError("LoadFromFile error(%s)",e.what());
		assert(false);
	}
	catch (...)
	{
		assert(false);
	}
	return false;
}