#pragma once

#include <vector>
#include <Windows.h>
#include <tchar.h>

class ISerialize
{
public:
	bool Load(ISerialize* pIserial);
	bool Load(const std::string& s);
	bool Load(LPCTSTR s);
	bool LoadFromFile(LPCTSTR sFile);

	bool SaveToStream(std::wostringstream& ss);
	bool SaveToStream(std::ostringstream& ss);
	bool SaveToFile(LPCTSTR sFile);


	typedef boost::property_tree::wiptree node_type;
	typedef node_type::assoc_iterator   node_itor_type;

	virtual bool serial(node_type& node,bool isload) = 0;
	bool serial(node_type& node,bool isload,LPCTSTR nodename);

	template<typename K>
	static bool serial_range(std::vector<K>& v,node_type& node,bool isload,LPCTSTR nodename)
	{
		typedef typename std::vector<K>::iterator iter;
		if(isload)
		{
			v.clear();
			node_itor_type it = node.find(nodename);
			if(it != node.not_found())
			{
				BOOST_FOREACH(node_type::value_type& i,it->second)
				{
					node_type& item_node = i.second;
					K value;
					if(value.serial(item_node,isload))
						v.push_back(value);
				}
			}
		}
		else
		{
			node_type list_node;
			for (iter it = v.begin(),iend = v.end(); it != iend;++it)
			{
				node_type item_node;
				if(!it->serial(item_node,isload))
					return false;
				list_node.push_back(node_type::value_type(_T(""),item_node));
			}

			node.add_child(nodename,list_node);
		}

		return true;
	}

	template<typename K>
	static bool serial_range(std::vector<boost::shared_ptr<K> >& v,node_type& node,bool isload,LPCTSTR nodename)
	{
		typedef typename boost::shared_ptr<K>  value_type;
		typedef typename std::vector<boost::shared_ptr<K> >::iterator iter;
		if(isload)
		{
			v.clear();
			node_itor_type it = node.find(nodename);
			if(it != node.not_found())
			{
				BOOST_FOREACH(node_type::value_type& i,it->second)
				{
					node_type& item_node = i.second;
					value_type value(new K());
					if(value->serial(item_node,isload))
						v.push_back(value);
				}
			}
		}
		else
		{
			node_type list_node;
			for (iter it = v.begin(),iend = v.end(); it != iend;++it)
			{
				node_type item_node;
				if(!(*it)->serial(item_node,isload))
					return false;
				list_node.push_back(node_type::value_type(_T(""),item_node));
			}

			node.add_child(nodename,list_node);
		}

		return true;
	}
};
