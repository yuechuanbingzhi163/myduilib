#include "stdafx.h"
#include "ProtoCommand.h"

namespace proto {
base::base():_isValid(true),_bMore(false)
{

}
base::~base()
{

}

bool base::IsValid() const
{
	return _isValid;
}
std::wstring base::ErrMsg() const
{
	return _sErrMsg;
}
bool base::HasMore() const
{
	return _bMore;
}

bool base::serial(node_type& node,bool isload) 
{
	if (isload)
	{
		_isValid = (0 == node.get(_T("status"),_T("ok")).compare(_T("ok")));
		_bMore   = (0 == node.get(_T("more"),_T("false")).compare(_T("true")));
		//_bMore   = (_T("true") == node.get(_T("more"),_T("false")));

		if(!_isValid)
			_sErrMsg = node.get(_T("err"),_T(""));
	}
	else
	{
		node.put(_T("status"),_T("ok"));
		node.put(_T("more"),(HasMore() ? _T("true") : _T("false")));
		node.put(_T("err"),_sErrMsg);
	}

	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
spLogonUserInfoT login::GetUserInfo() const
{
	return _pUserInfo;
}
bool login::serial(node_type& node,bool isload)
{
	__super::serial(node,isload);
	if (isload)
	{
		if(!_pUserInfo)
			_pUserInfo = spLogonUserInfoT(new LogonUserInfoT);

		node_itor_type it = node.find(_T("data"));
		if(node.not_found() == it)
			return false;
		node_type& user_node = it->second;
		assert(!user_node.empty());

		_pUserInfo->sLogonEmail    = user_node.get(_T("email"),_T("")).c_str();
		_pUserInfo->sPasswordMD5   = user_node.get(_T("password"),_T("")).c_str();
		_pUserInfo->nUserID        = user_node.get(_T("uid"),0);
		_pUserInfo->sNickName      = user_node.get(_T("nick_name"),_T("虾米用户")).c_str();
		_pUserInfo->sAvaterUrl     = user_node.get(_T("avatar"),_T("")).c_str();
		_pUserInfo->nFollowerCount = user_node.get(_T("fans"),0);
		_pUserInfo->nFollowCount   = user_node.get(_T("friends"),0);
	}
	else
	{
		node_type user_node;
		if(_pUserInfo)
		{
			user_node.put(_T("email"),_pUserInfo->sLogonEmail);
			user_node.put(_T("password"),_pUserInfo->sPasswordMD5);
			user_node.put(_T("uid"),_pUserInfo->nUserID);
			user_node.put(_T("nick_name"),_pUserInfo->sNickName);
			user_node.put(_T("avatar"),_pUserInfo->sAvaterUrl);
			user_node.put(_T("fans"),_pUserInfo->nFollowerCount);
			user_node.put(_T("firends"),_pUserInfo->nFollowCount);
		}

		node.add_child(_T("user"),user_node);
	}
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool songlist::serial(node_type& node,bool isload)
{
	__super::serial(node,isload);
	if(isload)
		_nListId = node.get(_T("list_id"),0);
	else
		node.put(_T("list_id"),_nListId);
	return serial_range<CSongInfo>(_songs,node,isload,_T("data"));
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

bool listsongs::serial(node_type& node,bool isload)
{
	__super::serial(node,isload);
	if (isload)
	{
		node_itor_type data = node.find(_T("data"));
		if(node.not_found() == data)
			return false;
		_bMore   =(0 == node.get(_T("more"),_T("false")).compare(_T("true")));

		/*node_itor_type songs = data->second.find(_T("songs"));
		if (data->second.not_found() == songs)
		return false;*/
		return serial_range<CSongInfo>(_songs,data->second,isload,_T("songlists"));
	}
	return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool play::serial(node_type& node,bool isload)
{
	if (isload)
		_nPlayIndex  = node.get<UINT>(_T("playindex"),0);
	else
		node.put(_T("playindex"),_nPlayIndex);

	return __super::serial(node,isload);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool playlist::serial(node_type& node,bool isload)
{
	__super::serial(node,isload);

	if (isload)
	{
		node_itor_type lists = node.find(_T("data"));
		if(node.not_found() == lists)
			return false;
		node_type& lists_node = lists->second;

		if (_ListInfoArray.size()!=0)
		{
			_ListInfoArray.clear();
		}
		serial_range<CPlayListInfo>(_ListInfoArray,node,isload,_T("data"));
	}
	else
	{
		if(!_ListInfoArray.empty())
		{
			serial_range<CPlayListInfo>(_ListInfoArray,node,isload,_T("data"));
		}
	}

	return true;
}
void  playlist::GetWebPlayListArray(std::vector<spPlayListInfoT>& ListInfoArray)
{
	ListInfoArray = _ListInfoArray;
}

//void radio::GetRadioArray(std::vector<spCRadioT>& ListInfoArray)
//{
//	ListInfoArray = _RadioArray;
//}
//
//bool radio::serial(node_type& node,bool isload)
//{
//	__super::serial(node,isload);
//
//	bool bRet = false;
//	if (isload)
//	{
//		node_itor_type radios = node.find(_T("data"));
//		if(node.not_found() == radios)
//			return false;
//		node_type& radios_node = radios->second;
//
//		if (_RadioArray.size()!=0)
//		{
//			_RadioArray.clear();
//		}
//		bRet = serial_range<CRadio>(_RadioArray,node,isload,_T("data"));
//	}
//	else
//	{
//		if(!_RadioArray.empty())
//		{
//			bRet =serial_range<CRadio>(_RadioArray,node,isload,_T("data"));
//		}
//	}
//
//	return bRet;
//}


/************************************************************************/
/*                                                                      */
/************************************************************************/
bool playlistId::serial(node_type& node,bool isload)
{
	__super::serial(node,isload);

	if (isload)
	{
		node_itor_type data = node.find(_T("data"));
		if(node.not_found() == data)
		{
			_isValid = false;
			return false;
		}
		_nWebId = data->second.get<UINT>(_T("id"),0);

	}
	return true;
}

UINT playlistId::GetWebId()
{
	return _nWebId;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool FavId::serial(node_type& node,bool isload)
{
	__super::serial(node,isload);
	if (isload)
	{
		_IdArray.clear();
		node_itor_type data = node.find(_T("data"));
		if(node.not_found() == data)
			return false;

		_bMore   =(0 == node.get(_T("more"),_T("false")).compare(_T("true")));

		node_itor_type Idlist = data->second.find(_T("lists"));
		if (data->second.not_found() == Idlist)
			return false;
		node_type& IdArray = Idlist->second;
		//assert(!IdArray.empty());
		if (IdArray.empty())
			return false;
		//wprintf(_T("%d"),IdArray.size());
		for (node_type::const_iterator it = IdArray.begin();it != IdArray.end();++it)
		{
			//wprintf(_T("first:%s second:%s"),it->first.c_str(),it->second.get_value<std::wstring>().c_str());
			UINT nId = _wtoi(it->second.get_value<std::wstring>().c_str());
			_IdArray.push_back(nId);
		}
	}
	return true;
}

bool matchsong::serial(node_type& node,bool isload)
{
	__super::serial(node,isload);
	if (isload)
	{
		_songs.clear();

		return serial_range<CSongInfo>(_songs,node,isload,_T("data"));
	}

	return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
namespace js {
bool IdType::serial(node_type& node,bool isload)
{
	//__super::serial(node,isload);
	if(isload)
	{
		_type = node.get<std::size_t>(_T("type"),ID_SONG);
	}
	else
	{
		node.put<std::size_t>(_T("type"),_type);
	}
	return true;
}
bool IdList::serial(node_type& node,bool isload)
{
	__super::serial(node,isload);
	if(isload)
	{
		_name = node.get(_T("name"),_T(""));
		_listId = node.get(_T("list_id"),0);
		node_itor_type id_iter = node.find(_T("id"));
		if (node.not_found() != id_iter)
		{
			node_type& list_node = id_iter->second;
			for (auto it = list_node.begin(),end = list_node.end(); it != end; ++it)
			{
				_IdVec.push_back(it->second.get_value<UINT>());
			}
		}
// 		node_type list_node = node.find(_T("id"));
// 		std::wstring ids = node.get(_T("id"),_T(""));
// 
// 		std::vector<std::wstring> idArray;
// 		boost::split(idArray,ids,boost::is_any_of(_T(",")));
// 		for (auto it = idArray.begin(),end = idArray.end(); it != end;++it)
// 			_IdVec.push_back(_ttoi(it->c_str()));
	}
	else
	{
		node.put(_T("name"),_name);
		node.put(_T("list_id"),_listId);
		node_type list_node;
		for (auto it = _IdVec.begin(),end = _IdVec.end(); it != end; ++it)
		{
			node_type item;
			item.put_value(*it);
			list_node.push_back(node_type::value_type(_T(""),item));
		}

		node.add_child(_T("id"),list_node);
	
// 		std::wstring ids;
// 		for (auto it = _IdVec.begin(),end = _IdVec.end(); it != end; ++it)
// 		{
// 			TCHAR szbuf[256] = {0};
// 			wsprintf(szbuf,_T("%u"),*it);
// 			ids.append(szbuf);
// 			if( it != end)
// 				ids.append(_T(","));
// 			//ids<<(*it)<<_T(",");
// 		}
// 		node.put(_T("id"),ids);
	}

	return true;
}
bool IdTokenList::serial(node_type& node,bool isload)
{
	if(!__super::serial(node,isload))
		return false;
	if(isload)
	{
		_name = node.get(_T("name"),_T(""));
	}
	else
	{
		node.put(_T("name"),_name);
	}
	return _listIdToken.serial(node,isload) &&
		serial_range<CIDToken>(_idTokenArray,node,isload,_T("ids"));
}
bool IdPaging::serial(node_type& node,bool isload)
{
	if (isload)
	{
		_limit = node.get(_T("limit"),1);
		_page  = node.get(_T("page"),1);
	}
	else
	{
		node.put(_T("limit"),_limit);
		node.put(_T("page"),_page);
	}
	return __super::serial(node,isload);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

void FavList::SetHasMore(bool bHasMore)
{
	_bMore = bHasMore;
}
bool FavList::serial(node_type& node,bool isload)
{
	__super::serial(node,isload);
	if(isload)
	{
		assert(false);
	}
	else
	{

		node_type node_data;
		for (auto it = _favs.begin(),end = _favs.end();it!=end;++it)
		{
			node_type item;
			item.put_value(*it);
			node_data.push_back(node_type::value_type(_T(""),item));
		}
		node.add_child(_T("data.id"),node_data);
		/*node_type ids_node;
		node_type local_ids_node;
		for (auto it = _favs.begin(),end = _favs.end(); it != end; ++it)
		{
		spFavInfoT theFav(*it);
		node_type item;
		switch (theFav->GetType())
		{
		case CFavInfo::ft_album:
		case CFavInfo::ft_artist:
		case CFavInfo::ft_collect:
		case CFavInfo::ft_song:
		item.put_value(theFav->GetFavId());
		ids_node.push_back(node_type::value_type(_T(""),item));
		break;
		default:
		assert(false);
		break;
		}
		}

		node.add_child(_T("data.id"),ids_node);
		node.add_child(_T("data.local_id"),local_ids_node);*/
	}
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool share::serial(node_type& node,bool isload)
{
	if(isload)
	{
		_id      = node.get(_T("id"),0);
		_type    = node.get<std::size_t>(_T("type"),32);//32 is song:
		_title    = node.get(_T("title"),_T(""));
		_comment = node.get(_T("comment"),_T(""));
	}
	else
	{
		node.put(_T("id"),_id);
		node.put(_T("type"),_type);
		node.put(_T("title"),_title);
		node.put(_T("comment"),_comment);
	}
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool musicPacketRequest::serial(node_type& node,bool isload)
{
	if (isload)
	{
		_id     = node.get(_T("id"),0);
		_type   = node.get(_T("type"),1); //1 is musicpacket
		_songlimit = node.get(_T("limit1"),0);
		_songpage  = node.get(_T("page1"),0);
		_limit     = node.get(_T("limit2"),0);
		_page      = node.get(_T("page2"),0);
	}
	else
	{
		assert(false);
	}
	return true;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool LocalMusicPacketToken::serial(node_type& node,bool isload)
{
	__super::serial(node,isload);

	return serial_range<CIDToken>(_idTokenArray,node,isload,_T("ids"));
}

bool MsgBox::serial(node_type& node,bool isload)
{
	if(isload)
	{
		_title    = node.get(_T("title"),_T("虾听"));
		_text     = node.get(_T("text"),_T(""));
		_iconType = node.get(_T("icon"),0);
		_flag     = node.get(_T("flag"),1<<3|1<<4); //参考MsgBox
	}
	else
	{
		node.put(_T("title"),_title);
		node.put(_T("text"),_text);
		node.put(_T("icon"),_iconType);
		node.put(_T("flag"),_flag);
	}

	return true;
}


bool RecCount::serial( node_type& node,bool isload )
{
	if(isload)
	{
		nCount = node.get(_T("limit"),50);
	}
	return true;
}

}//namespace js
}