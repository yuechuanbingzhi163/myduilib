#include "stdafx.h"
#include "responseParser.h"
#include "../YApp.h"
#include "WebService.h"
#include "zlib.h"

struct protoFactory
{
	template<typename T,bool>
	struct creater;

	template<typename T>
	struct creater<T,true>
	{
		static T* create()
		{
			return (new T());
		}
	};

	template<typename T>
	static void* CreateInstance()
	{
		return NULL;
	}
};
/************************************************************************/
/*                                                                      */
/************************************************************************/
	CResponseParser::CResponseParser()
	{
/*		m_ParseFuncMap.insert(std::make_pair(url::WSRT_LOGIN,protoFactory::CreateInstance<proto::login>));
		m_ParseFuncMap.insert(std::make_pair(url::WSRT_WEBLOGIN,protoFactory::CreateInstance<proto::login>));

		m_ParseFuncMap.insert(std::make_pair(url::WSRT_SEARCH,protoFactory::CreateInstance<proto::songlist>));

		m_ParseFuncMap.insert(std::make_pair(url::WSRT_FAVORITE,protoFactory::CreateInstance<proto::FavId>));
		//m_ParseFuncMap.insert(std::make_pair(url::WSRT_OPERATE_FAVORITESONGS,protoFactory::CreateInstance<proto::base>));

		m_ParseFuncMap.insert(std::make_pair(url::WSRT_PLAYLOG,protoFactory::CreateInstance<proto::base>));

		m_ParseFuncMap.insert(std::make_pair(url::WSRT_SHARE,protoFactory::CreateInstance<proto::base>));

		m_ParseFuncMap.insert(std::make_pair(url::WSRT_GET_LIST,protoFactory::CreateInstance<proto::playlist>));
		m_ParseFuncMap.insert(std::make_pair(url::WSRT_NEW_LIST,protoFactory::CreateInstance<proto::playlistId>));
		m_ParseFuncMap.insert(std::make_pair(url::WSRT_RENAME_LIST,protoFactory::CreateInstance<proto::base>));
		m_ParseFuncMap.insert(std::make_pair(url::WSRT_CHANGE_STATUS,protoFactory::CreateInstance<proto::base>));
		m_ParseFuncMap.insert(std::make_pair(url::WSRT_DEL_LIST,protoFactory::CreateInstance<proto::base>));
		m_ParseFuncMap.insert(std::make_pair(url::WSRT_GET_ROAMING,protoFactory::CreateInstance<proto::songlist>));
		m_ParseFuncMap.insert(std::make_pair(url::WSRT_MATCH_SONG,protoFactory::CreateInstance<proto::matchsong>));
		m_ParseFuncMap.insert(std::make_pair(url::WSRT_DETAIL_INFO,protoFactory::CreateInstance<proto::songlist>));
		m_ParseFuncMap.insert(std::make_pair(url::WSRT_GET_TAG_LIST,protoFactory::CreateInstance<proto::playlist>));
		m_ParseFuncMap.insert(std::make_pair(url::WSRT_GET_TAG_SONGS,protoFactory::CreateInstance<proto::songlist>));
		m_ParseFuncMap.insert(std::make_pair(url::WSRT_LIST_GET_SONGS,protoFactory::CreateInstance<proto::listsongs>));
*/
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_SEARCHMARK,ParseSearchMark));
// 		
// 		
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_ATTENTIONLIST,ParseAttentionList));
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_FANCELIST,ParseFanceList));
// 
// 
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_PRAISE,ParsePraise));
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_SENDSONG,ParseSendSongCommen));
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_SENDSHARE,ParseSendShareCommen));
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_REPLYCOMMENT,ParseSongCommen));
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_REPLYSENDSONG,ParseSendSong));
// 
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_CANCELATTENTION,ParseCancelAttention));
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_ADDATTENTION,ParseAddAttention));
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_GETALLFRIENDS,ParseGetAllFriends));
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_RECOMMENDWINDOW,ParseRecommendInfo));
// 
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_RECOMMENDSENDSONG,ParseRecommendInfo));
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_RECOMMENDGOOD,ParseRecommendInfo));
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_RECOMMENDSHARE,ParseRecommendInfo));
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_RECOMMEDNUPDATE,ParseRecommendInfo));
// 		
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_FRIENDUSERINFO,ParseGetFriendUserInfo));
// 		m_ParseFuncMap.insert(std::make_pair(WSRT_MESSAGEINFOCOUNT, ParseGetMessageInfoCount));
	}

	void* CResponseParser::Parse(unsigned int RequestType, std::string &sResponse)
	{
// 		ParseFuncMapIterT iter = m_ParseFuncMap.find(RequestType);
// 		assert(m_ParseFuncMap.end() != iter);
// 		if(iter != m_ParseFuncMap.end())
// 		{
// 			protoCreaterT threCreater = iter->second;
// 			if(threCreater)
// 			{
// 				void* theProto(threCreater());
// 				theProto->Load(sResponse);
// 				return theProto;
// 			}
// 		}
		return NULL;
	}

