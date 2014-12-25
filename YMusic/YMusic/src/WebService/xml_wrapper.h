#pragma once

#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif

#include <string>
#include <vector>
#ifdef _WIN32
#include "../Utility/tinyxml/tinyxml.h"
#else
#include "../tinyxml/tinyxml.h"
#endif

namespace xmldata
{
#ifdef _WIN32
	using namespace em_utility;
#else
#endif

struct xml_wrapper 
{
	static const int NUM_BUF_LEN = 128;						//整数转换缓冲区长度
	static const int INVALID_XML_VALUE = -10000;			//无效的值
	static const int BUF_NOT_ENOUGH = -10001;				//buf缓冲区不够
	static const unsigned short XML_NAME_LEN = 128;	//最大名称长度
	xml_wrapper();
	virtual ~xml_wrapper();
	//加入一个整数类型子节点
	static TiXmlNode* add_child_ele(TiXmlNode* p_parent, const char* p_name, int n_value);
	//加入一个子节点
	static TiXmlNode* add_child_ele(TiXmlNode* p_parent, const char* p_name, const char* p_value);
	//更新一个子节点
	static bool update_child_ele(TiXmlElement* p_parent, const char* p_name, const char* p_value);
	//取得一个整数子节点值
	static int get_child_int(TiXmlElement* p_parent, const char* p_name, int n_def = 0);
	static double get_child_double(TiXmlElement* p_parent, const char* p_name, double n_def = 0);
	//取得一个string格式的子节点值
	static std::string get_child_string(TiXmlElement* p_parent, const char* p_name, const std::string& s_def = "");
	//取得一个固定长度的char子节点值
	static int get_child_cstr(TiXmlElement* p_parent, const char* p_name, char* p_buf, int n_len, const char* p_def = "");
	static std::string export_2_string(TiXmlDocument* p_doc);

	// add by wangshuai start
	static int get_children_count(TiXmlNode * p_parent);
	// add by wangshuai end
public:
	std::string export_to_string() const;
	virtual bool export_to_xml(TiXmlNode& parent_node) const;
	bool import_from_string(const char* p_xml_data);
	virtual bool import_from_xml(TiXmlElement& ele);
	bool name_check() const;

	template<class T>
	static bool import_array_p(std::vector<T*>& items, const char* p_list_name, const char* p_item_name, TiXmlElement& parent_ele)
	{
		bool b_result = true;
		items.clear();
		assert(p_list_name != NULL);
		assert(strlen(p_list_name) > 0);
		assert(p_item_name != NULL);
		assert(strlen(p_item_name) > 0);
		TiXmlNode* p_list_node = parent_ele.FirstChild(p_list_name);
		if (p_list_node != NULL)
		{
			TiXmlElement* p_list_ele = p_list_node->ToElement();
			assert(p_list_ele != NULL);
			TiXmlNode* p_child_node = p_list_ele->FirstChild(p_item_name);
			TiXmlElement* p_child_ele = NULL;
			if (p_child_node != NULL)
			{
				p_child_ele = p_child_node->ToElement();
				assert(p_child_ele != NULL);
				T* p_item = new T();
				b_result = p_item->import_from_xml(*p_child_ele);
				assert(b_result);
				items.push_back(p_item);
				while ((p_child_ele = p_child_ele->NextSiblingElement()) != NULL)
				{
					T* p_next = new T();
					b_result = p_next->import_from_xml(*p_child_ele);
					assert(b_result);
					items.push_back(p_next);
				}
			}
		}
		return b_result;
	}

	template<class T>
	static bool export_array_p(const std::vector<T*>& items, const char* p_name, TiXmlElement& parent_ele)
	{
		bool b_result = true;
		assert(p_name != NULL);
		assert(strlen(p_name) > 0);
		if (!items.empty())
		{
			TiXmlElement list_ele(p_name);
			TiXmlNode* p_list_node = parent_ele.InsertEndChild(list_ele);
			assert(p_list_node != NULL);
			typedef typename std::vector<T*>::const_iterator c_item_iter;
			for (c_item_iter i = items.begin(); i != items.end(); ++i)
			{
				const T* p_item = *i;
				b_result = p_item->export_to_xml(*p_list_node);
			}
		}
		return b_result;
	}

	template<class T>
	static bool import_array(std::vector<T>& items, const char* p_list_name, const char* p_item_name, TiXmlElement& parent_ele)
	{
		bool b_result = true;
		items.clear();
		assert(p_list_name != NULL);
		assert(strlen(p_list_name) > 0);
		assert(p_item_name != NULL);
		assert(strlen(p_item_name) > 0);
		TiXmlNode* p_list_node = parent_ele.FirstChild(p_list_name);
		if (p_list_node != NULL)
		{
			TiXmlElement* p_list_ele = p_list_node->ToElement();
			assert(p_list_ele != NULL);
			TiXmlNode* p_child_node = p_list_ele->FirstChild(p_item_name);
			TiXmlElement* p_child_ele = NULL;
			if (p_child_node != NULL)
			{
				p_child_ele = p_child_node->ToElement();
				assert(p_child_ele != NULL);
				T item;
				b_result = item.import_from_xml(*p_child_ele);
				assert(b_result);
				items.push_back(item);
				while ((p_child_ele = p_child_ele->NextSiblingElement()) != NULL)
				{
					T next;
					b_result = next.import_from_xml(*p_child_ele);
					assert(b_result);
					items.push_back(next);
				}
			}
		}
		return b_result;
	}

	template<class T>
	static bool export_array(const std::vector<T>& items, const char* p_name, TiXmlElement& parent_ele)
	{
		bool b_result = true;
		assert(p_name != NULL);
		assert(strlen(p_name) > 0);
		if (!items.empty())
		{
			TiXmlElement list_ele(p_name);
			TiXmlNode* p_list_node = parent_ele.InsertEndChild(list_ele);
			assert(p_list_node != NULL);
			typedef typename std::vector<T>::const_iterator c_item_iter;
			for (c_item_iter i = items.begin(); i != items.end(); ++i)
			{
				const T* p_item = &*i;
				b_result = p_item->export_to_xml(*p_list_node);
			}
		}
		return b_result;
	}
private:
	static TiXmlNode* _add_child_ele(TiXmlNode* p_parent, const char* p_name, const char* p_value);
protected:
	std::string s_node_name;
};
}
