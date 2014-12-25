#ifndef __DATAPARSER_H
#define __DATAPARSER_H

#include "include/json.h"

template<typename T = Json::Value>
class CDataParser
{
public:
	typedef Json::Value          value_type;

	template<typename O>
	static void toValue(value_type& v,O& o);

	template<typename O>
	static void  getValue(value_type& v,const char* key,O& o)
	{
		assert(key);
		if(!v.isNull())
		{
			value_type theValue = v[key];
			if(!theValue.isNull())
				return toValue(theValue,o);
		}

		o = O();
	}

	template<typename O>
	static void getValue(value_type& v,const char* key,O& o,O def_o)
	{
		assert(key);
		if(!v.isNull())
		{
			value_type theValue = v[key];
			if(!theValue.isNull())
				return toValue(theValue,o);
		}

		o = def_o;
	}

	template<typename O>
	static O getValue(value_type& v,const char* key)
	{
		assert(key);
		O o = O();
		if(!v.isNull())
		{
			value_type theValue = v[key];
			if(!theValue.isNull())
				toValue(theValue,o);
		}
		return o;
	}

	// 		template<typename O>
	// 		static O getValue(value_type& v,const char* key,O def_o)
	// 		{
	// 			assert(key);
	// 			O o = def_o;
	// 			if(!v.isNull())
	// 			{
	// 				value_type theValue = v[key];
	// 				if(!theValue.isNull())
	// 					toValue(theValue,o);
	// 			}
	// 			return o;
	// 		}

	template<>
	static void toValue(value_type& v,CString& o)
	{
		o = em_utility::mci::utf8_2_CString(v.asCString());
	}

	template<>
	static void toValue(value_type& v,std::string& o)
	{
		o = v.asCString();
	}

	template<>
	static void toValue(value_type& v,int& o)
	{
		o = v.isInt() ? v.asInt() : atoi(v.asCString());
	}

	template<>
	static void toValue(value_type& v,unsigned& o)
	{
		o = v.isIntegral() ?  v.asUInt() : atoi(v.asCString());
	}

	template<>
	static void toValue(value_type& v,long& o)
	{
		o = v.isIntegral() ?  v.asInt(): atol(v.asCString());
	}

	template<>
	static void toValue(value_type& v,bool& o)
	{
		o = v.isIntegral() ? v.asBool() :  ("true" == v.asString());
	}
};

typedef CDataParser<Json::Value>   JsonParser;


#endif//__DATAPARSER_H