//#define TIXML_USE_STL
#include "xml_wrapper.h"
#include <cassert>

using namespace std;

namespace xmldata
{
xml_wrapper::xml_wrapper()
{
}

xml_wrapper::~xml_wrapper()
{
}

TiXmlNode* xml_wrapper::add_child_ele(TiXmlNode* p_parent, const char* p_name, int n_value)
{
	assert(p_parent != NULL);
	assert(p_name != NULL);
	TiXmlElement child_ele(p_name);

	char s_buf[NUM_BUF_LEN];
	memset(s_buf, 0x0, NUM_BUF_LEN);
#ifdef _WIN32
	sprintf_s(s_buf, NUM_BUF_LEN, "%d", n_value);
#else
	sprintf(s_buf, "%d", n_value);
#endif
	return _add_child_ele(p_parent, p_name, s_buf);
}

TiXmlNode* xml_wrapper::add_child_ele(TiXmlNode* p_parent, const char* p_name, const char* p_value)
{
	assert(p_parent != NULL);
	assert(p_name != NULL && p_value != NULL);
	if (strlen(p_value) == 0)
		return NULL;
	return _add_child_ele(p_parent, p_name, p_value);
}

TiXmlNode* xml_wrapper::_add_child_ele(TiXmlNode* p_parent, const char* p_name, const char* p_value)
{
	assert(p_parent != NULL);
	assert(p_name != NULL && p_value != NULL);
	TiXmlElement child_ele(p_name);
	TiXmlText text(p_value);
	child_ele.InsertEndChild(text);
	TiXmlNode* p_node = p_parent->InsertEndChild(child_ele);
	return p_node;
}

bool xml_wrapper::update_child_ele(TiXmlElement * p_parent, const char * p_name, const char * p_value)
{
	assert(p_parent != NULL);
	assert(p_name != NULL && p_value != NULL);
	TiXmlElement* p_child = p_parent->FirstChildElement(p_name);
	if (p_child == NULL)
		return false;
	p_child->SetValue(p_value);
	return true;
}

int xml_wrapper::get_child_int(TiXmlElement * p_parent, const char * p_name, int n_def)
{
	assert(p_parent != NULL && p_name != NULL);
	TiXmlElement* p_ele = p_parent->FirstChildElement(p_name);
	if (p_ele == NULL)
		return n_def;
	TiXmlNode* p_node = p_ele->FirstChild();
	if (p_node == NULL)
		return n_def;
	int n_result = atoi(p_node->Value());
	return n_result;
}

double xml_wrapper::get_child_double(TiXmlElement* p_parent, const char* p_name, double n_def)
{
	double f_result = 0;
	assert(p_parent != NULL && p_name != NULL);
	TiXmlElement* p_ele = p_parent->FirstChildElement(p_name);
	if (p_ele == NULL)
		return n_def;
	TiXmlNode* p_node = p_ele->FirstChild();
	if (p_node == NULL)
		return n_def;
	f_result = atof(p_node->Value());
	return f_result;
}

string xml_wrapper::get_child_string(TiXmlElement* p_parent, const char* p_name, const std::string& s_def)
{
	assert(p_parent != NULL && p_name != NULL);
	TiXmlElement* p_ele = p_parent->FirstChildElement(p_name);
	if (p_ele == NULL)
		return s_def;
	TiXmlNode* p_node = p_ele->FirstChild();
	if (p_node == NULL)
		return s_def;
	const char* p_value = p_node->Value();
	assert(p_value != NULL);
	string s_result = p_value;
	return s_result;
}

int xml_wrapper::get_child_cstr(TiXmlElement* p_parent, const char* p_name, char* p_buf, int n_len, const char* p_def)
{
	assert(p_parent != NULL && p_name != NULL);
	assert(p_buf != NULL && n_len > 0);
	assert(p_def != NULL);
	TiXmlElement* p_ele = p_parent->FirstChildElement(p_name);
	if (p_ele == NULL)
	{
		assert(strlen(p_def) < (unsigned int)n_len);
#ifdef _WIN32
		strcpy_s(p_buf, n_len, p_def);
#else
		strcpy(p_buf, p_def);
#endif
		return INVALID_XML_VALUE;
	}
	TiXmlNode* p_node = p_ele->FirstChild();
	if (p_node == NULL)
	{
		assert(strlen(p_def) < (unsigned int)n_len);
#ifdef _WIN32
		strcpy_s(p_buf, n_len, p_def);
#else
		strcpy(p_buf, p_def);
#endif
		return INVALID_XML_VALUE;
	}
	const char* p_value = p_node->Value();
	assert(p_value != NULL);
	unsigned int n_strlen = static_cast<unsigned int>(strlen(p_value));
	if (n_strlen >= (unsigned int)n_len)
		return BUF_NOT_ENOUGH;
#ifdef _WIN32
	strcpy_s(p_buf, n_len, p_value);
#else
	strcpy(p_buf, p_value);
#endif
	return n_strlen;
}

std::string xml_wrapper::export_2_string(TiXmlDocument* p_doc)
{
	assert(p_doc != NULL);
	string s_output;
	s_output << *p_doc;
	return s_output;		
}

int xml_wrapper::get_children_count(TiXmlNode * p_parent)
{
	int sum = 0;
	TiXmlNode *p_first_child = p_parent->FirstChild();
	while (p_first_child)
	{
		p_first_child = p_parent->IterateChildren(p_first_child);
		sum++;
	}	
	return sum;
}

bool xml_wrapper::import_from_xml(TiXmlElement& ele)
{
	return false;
}

bool xml_wrapper::import_from_string(const char* p_xml_data)
{
	bool b_result = false;
	if (p_xml_data == NULL || strlen(p_xml_data) == 0)
		return false;
	TiXmlDocument doc;
	doc.Parse(p_xml_data, NULL, TIXML_ENCODING_UTF8);
	if (doc.Error())
		return false;

	assert(name_check());
	TiXmlNode* p_node = doc.FirstChild(s_node_name.c_str());
	if (p_node == NULL)
		return false;
	TiXmlElement* p_ele = p_node->ToElement();
	if (p_ele == NULL)
		return false;
	b_result = import_from_xml(*p_ele);
	return b_result;
}

std::string xml_wrapper::export_to_string() const
{
	std::string s_output;
	TiXmlDocument doc;
	TiXmlDeclaration dec;
	dec.Parse("<?xml version=\"1.0\" encoding=\"utf-8\"?>", NULL, TIXML_ENCODING_UTF8);
	TiXmlNode* p_node = doc.InsertEndChild(dec);
	assert(p_node != NULL);
	bool b_result = export_to_xml(doc);
	if (b_result)
	{
		s_output << doc;
	}
	return s_output;
}

bool xml_wrapper::export_to_xml(TiXmlNode& parent_node) const
{
	return false; 
}

bool xml_wrapper::name_check() const
{
	return !s_node_name.empty();
}


}			//namespace bluefish
