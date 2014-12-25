#include "stdafx.h"
#include "ResponseParser2.h"
#include "WebService.h"

#include "../Utilty/json/DataParser.h"

#include "../YApp.h"
using namespace url;


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	CResponseParser2::CResponseParser2()
	{
		m_ParseFuncMap.insert(std::make_pair(WSRT_LOGIN,ParseLogin));
		m_ParseFuncMap.insert(std::make_pair(WSRT_WEBLOGIN,ParseLogin));
		m_ParseFuncMap.insert(std::make_pair(WSRT_SEARCH,ParseSongList));
		m_ParseFuncMap.insert(std::make_pair(WSRT_SEARCHMARK,ParseSearchMark));
//		m_ParseFuncMap.insert(std::make_pair(WSRT_FAVORITES,ParseSongList));
//		m_ParseFuncMap.insert(std::make_pair(WSRT_OPERATEFAVORITES,ParseOperateFavorites));
		m_ParseFuncMap.insert(std::make_pair(WSRT_ATTENTIONLIST,ParseAttentionList));
		m_ParseFuncMap.insert(std::make_pair(WSRT_FANCELIST,ParseFanceList));


		m_ParseFuncMap.insert(std::make_pair(WSRT_PRAISE,ParsePraise));
		m_ParseFuncMap.insert(std::make_pair(WSRT_SENDSONG,ParseSendSongCommen));
//		m_ParseFuncMap.insert(std::make_pair(WSRT_SENDSHARE,ParseSendShareCommen));
		m_ParseFuncMap.insert(std::make_pair(WSRT_REPLYCOMMENT,ParseSongCommen));
		m_ParseFuncMap.insert(std::make_pair(WSRT_REPLYSENDSONG,ParseSendSong));

		m_ParseFuncMap.insert(std::make_pair(WSRT_CANCELATTENTION,ParseCancelAttention));
		m_ParseFuncMap.insert(std::make_pair(WSRT_ADDATTENTION,ParseAddAttention));
		m_ParseFuncMap.insert(std::make_pair(WSRT_GETALLFRIENDS,ParseGetAllFriends));
		m_ParseFuncMap.insert(std::make_pair(WSRT_RECOMMENDWINDOW,ParseRecommendInfo));

		m_ParseFuncMap.insert(std::make_pair(WSRT_RECOMMENDSENDSONG,ParseRecommendInfo));
		m_ParseFuncMap.insert(std::make_pair(WSRT_RECOMMENDGOOD,ParseRecommendInfo));
		m_ParseFuncMap.insert(std::make_pair(WSRT_RECOMMENDSHARE,ParseRecommendInfo));
		m_ParseFuncMap.insert(std::make_pair(WSRT_RECOMMEDNUPDATE,ParseRecommendInfo));
		
		m_ParseFuncMap.insert(std::make_pair(WSRT_FRIENDUSERINFO,ParseGetFriendUserInfo));
		m_ParseFuncMap.insert(std::make_pair(WSRT_MESSAGEINFOCOUNT, ParseGetMessageInfoCount));
	}

	void *CResponseParser2::Parse(unsigned int RequestType, std::string &sResponse)
	{
		ParseFuncMapIterT iter = m_ParseFuncMap.find(RequestType);
		if(iter != m_ParseFuncMap.end())
		{
			ParseFunc func = iter->second;
			if(func)
			{
				return func(sResponse);
			}
		}
		return NULL;
	}

	void *CResponseParser2::ParseLogin(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;

		if (!reader.parse(sResponse, value) || value.isNull()) return NULL;
		
		//登录不成功
		if("success" != JsonParser::getValue<std::string>(value,"status"))
		{
		//	std::string sMsgUtf8 = value["msg"].asString();
		//	CString sMsg = em_utility::mci::utf8_2_CString(sMsgUtf8.c_str());
			return NULL;
		}

		Json::Value userValue = value["user"];
		if(userValue.isNull()) return NULL;

		Json::Value user;
		Json::Value defaultV;
		if (userValue.isArray())
		{
			Json::UInt nIndex = 0;
			user = userValue[nIndex];	
		}
		if(user.isNull())
			return NULL;

		LogonUserInfoT *pUserInfo = new LogonUserInfoT();
		if(!pUserInfo) return NULL;

		int nSex = 0;
		JsonParser::getValue(user,"email",pUserInfo->sLogonEmail);
		JsonParser::getValue(user,"password",pUserInfo->sPasswordMD5);
		JsonParser::getValue(user,"uid",pUserInfo->sUserID);
		JsonParser::getValue(user,"nickname",pUserInfo->sNickName);
		JsonParser::getValue(user,"level",pUserInfo->sLevel);
		JsonParser::getValue(user,"regtime",pUserInfo->sRegTime);
		JsonParser::getValue(user,"sex",nSex);
		if (nSex==0)
		{
			pUserInfo->sSex = _T("男");
		}
		else
		{
			pUserInfo->sSex = _T("女");
		}

		return pUserInfo;
	}


	void *CResponseParser2::ParseSearchMark(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;

		if (!reader.parse(sResponse, value) || value.isNull()) return NULL;

		if("ok" != JsonParser::getValue<std::string>(value,"status"))
			return NULL;

		std::vector<CString> *pMarkResult(new std::vector<CString>);
		CString sName,sArtistName,sContent;

		const Json::Value songs = value["song_list"];
		if(!songs.isNull())
		{
			for (Json::ValueIterator it = songs.begin();it != songs.end();++it)
			{
				JsonParser::getValue(*it,"name",sName);
				JsonParser::getValue(*it,"artist_name",sArtistName);
			}
		}
		const Json::Value albums = value["album_list"];
		if(!albums.isNull())
		{
			for (Json::ValueIterator it = albums.begin();it != albums.end();++it)
			{
				JsonParser::getValue(*it,"title",sName);
				JsonParser::getValue(*it,"artist_name",sArtistName);
				sContent.Format(_T("%s - %s"),sName,sArtistName);
				pMarkResult->push_back(sContent);
			}
		}
		const Json::Value artists = value["artist_list"];
		if(!artists.isNull())
		{
			for (Json::ValueIterator it = artists.begin();it != artists.end();++it)
			{
				JsonParser::getValue(*it,"name",sContent);
				pMarkResult->push_back(sContent);
			}
		}
		return pMarkResult;
	}

	void *CResponseParser2::ParseAttentionList(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;

		if (!reader.parse(sResponse, value) || value.isNull()) return NULL;

		if("ok" != JsonParser::getValue<std::string>(value,"status"))
			return NULL;

		const Json::Value Data = value["data"];
		if(Data.isNull())
			return NULL;
		std::vector<spUserInfoT>* spUserInfoList = new std::vector<spUserInfoT>();
		if(!spUserInfoList) return NULL;

		for (Json::ValueIterator it = Data.begin();it != Data.end();++it)
		{
			spUserInfoT theUser(new UserInfoT());
			if(!theUser) break;

			JsonParser::getValue(*it,"user_id",theUser->sUserID);
			JsonParser::getValue(*it,"nick_name",theUser->sNickName);
			JsonParser::getValue(*it,"avatar",theUser->sAvaterUrl);
			JsonParser::getValue(*it,"city",theUser->sArea);
			JsonParser::getValue(*it,"friends",theUser->nFollowCount);
			JsonParser::getValue(*it,"fans",theUser->nFollowerCount);
			JsonParser::getValue(*it,"friendship",theUser->nFlag);

			spUserInfoList->push_back(theUser);
		}
		return spUserInfoList;
	}

	void *CResponseParser2::ParseGetAllFriends(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;

		if (!reader.parse(sResponse, value) || value.isNull()) return NULL;

		if("ok" != JsonParser::getValue<std::string>(value,"status"))
			return NULL;
		const Json::Value Data = value["data"];
		if(Data.isNull()) return NULL;

		UserInfoListResult* pResult(new UserInfoListResult);
		if(!pResult) return NULL;

		pResult->bNext = JsonParser::getValue<bool>(value,"more");
		for (Json::ValueIterator it = Data.begin();it != Data.end();++it)
		{
			spUserInfoT theUser(new UserInfoT());
			if(!theUser) break;

			JsonParser::getValue(*it,"user_id",theUser->sUserID);
			JsonParser::getValue(*it,"nick_name",theUser->sNickName);
			JsonParser::getValue(*it,"avatar",theUser->sAvaterUrl);
			JsonParser::getValue(*it,"city",theUser->sArea);
			JsonParser::getValue(*it,"friends",theUser->nFollowCount);
			JsonParser::getValue(*it,"fans",theUser->nFollowerCount);
			JsonParser::getValue(*it,"friendship",theUser->nFlag);

			pResult->userlist.push_back(theUser);
		}

		return pResult;
	}

	void *CResponseParser2::ParseGetMessageInfoCount(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;

		if (!reader.parse(sResponse, value) || value.isNull()) return NULL;

		if("ok" != JsonParser::getValue<std::string>(value,"status"))
			return NULL;

		std::vector<spMessageInfoCountT>* spMessageInfoCount = new std::vector<spMessageInfoCountT>();
		spMessageInfoCountT spMessageInfo(new MessageInfoCountT());

		JsonParser::getValue(value,"pmcount",spMessageInfo->nPmcount);
		JsonParser::getValue(value,"msgcount",spMessageInfo->nMsgcount);
		JsonParser::getValue(value,"fanscount",spMessageInfo->nFanscount);
		JsonParser::getValue(value,"songcount",spMessageInfo->nSongCount);
		JsonParser::getValue(value,"favcount",spMessageInfo->nFavCount);
		JsonParser::getValue(value,"sharecount",spMessageInfo->nShareCount);
		JsonParser::getValue(value,"t",spMessageInfo->nTime);

		spMessageInfoCount->push_back(spMessageInfo);
		return spMessageInfoCount;
	}

	//取推荐信息
	void *CResponseParser2::ParseRecommendInfo(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;

		if (!reader.parse(sResponse, value) || value.isNull()) return NULL;

		if("ok" != JsonParser::getValue<std::string>(value,"status"))
			return NULL;
		
		Json::Value Data = value["data"];
		if(Data.isNull()) return NULL;

		std::vector<spSongInfoT>* spSongInfoList = new std::vector<spSongInfoT>();
// 		for (Json::ValueIterator it = Data.begin();it != Data.end();++it)
// 		{
// 			spSongInfoT spSong(new CSongInfo());
// 			JsonParser::getValue(*it,"user_id",spSong->nRecommendID);
// 			JsonParser::getValue(*it,"id",spSong->sCommentId);
// 			JsonParser::getValue(*it,"type",spSong->nRecommendType);
// 			JsonParser::getValue(*it,"nick_name",spSong->sRecommendName);
// 			JsonParser::getValue(*it,"message",spSong->sRecommendInfo);
// 			JsonParser::getValue(*it,"city",spSong->sRecommendPlace);
// 			JsonParser::getValue(*it,"gmt_create",spSong->nAddTimeGMT);
// 			JsonParser::getValue(*it,"from_user_id",spSong->nGooderID);
// 			JsonParser::getValue(*it,"from_nick_name",spSong->sGooderName);
// 
// 			Json::Value songValue = (*it)["song"];
// 			if(!songValue.isNull())
// 			{
// 				JsonParser::getValue(songValue,"song_id",spSong->nSongID);
// 				JsonParser::getValue(songValue,"default_resource_id",spSong->nResourceID);
// 				JsonParser::getValue(songValue,"album_id",spSong->nAlbumID);
// 				JsonParser::getValue(songValue,"artist_id",spSong->nArtistID);
// 				JsonParser::getValue(songValue,"grade",spSong->nGrade);
// 				JsonParser::getValue(songValue,"play_seconds",spSong->nAudioLength);
// 				JsonParser::getValue(songValue,"title",spSong->sAlbumName);
// 				JsonParser::getValue(songValue,"name",spSong->sSongName);
// 				JsonParser::getValue(songValue,"artist_name",spSong->sArtistName);
// 				JsonParser::getValue(songValue,"singers",spSong->sRealArtistName);
// 				JsonParser::getValue(songValue,"album_logo",spSong->sCoverUrl);
// 				JsonParser::getValue(songValue,"lyric_file",spSong->sLrcUrl);
// 				JsonParser::getValue(songValue,"listen_file",spSong->sListenFileUrl);
// 			}
// 
// 			spSongInfoList->push_back(spSong);
// 		}

		return spSongInfoList;
	}

	void *CResponseParser2::ParseGetFriendUserInfo(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;

		if (reader.parse(sResponse, value))
		{
			std::string sStatus = value["status"].asString();
			if(sStatus == "ok")
			{		
				const Json::Value Data = value["count"];
				if(Data.isNull())
					return NULL;
				UserInfoT *spUserInfo =new UserInfoT();

				if(!Data["nick_name"].isNull())
				{
					spUserInfo->sNickName=em_utility::mci::utf8_2_CString(Data["nick_name"].asCString());
				}
				if(!Data["avatar"].isNull())
				{
					spUserInfo->sAvaterUrl=em_utility::mci::utf8_2_CString(Data["avatar"].asCString());
				}
				if(!Data["level"].isNull())
				{
					spUserInfo->sLevel = em_utility::mci::utf8_2_CString(Data["level"].asCString());
				}
				if(!Data["signature"].isNull())
				{
					spUserInfo->sSignature = em_utility::mci::utf8_2_CString(Data["signature"].asCString());
				}
				if(!Data["gender"].isNull())
				{
					spUserInfo->sSex = em_utility::mci::utf8_2_CString(Data["gender"].asCString());
				}
				if(!Data["city"].isNull())
				{
					spUserInfo->sArea = em_utility::mci::utf8_2_CString(Data["city"].asCString());
				}
				if(!Data["year"].isNull())
				{
					spUserInfo->sYear = em_utility::mci::utf8_2_CString(Data["year"].asCString());
				}
				std::string sTemp;
				if(!Data["friends"].isNull())
				{
					sTemp = Data["friends"].asString();
					spUserInfo->nFollowCount = atol(sTemp.c_str());
				}
				if(!Data["fans"].isNull())
				{
					sTemp = Data["fans"].asString();
					spUserInfo->nFollowerCount = atol(sTemp.c_str());
				}
				if(!Data["user_id"].isNull())
				{
					sTemp = Data["user_id"].asString();
//					spUserInfo->nUserID = atol(sTemp.c_str());
					spUserInfo->sUserID = mci::utf8_2_CString(sTemp.c_str());
				}
				if(!Data["collect"].isNull())
				{
					sTemp = Data["collect"].asString();
					spUserInfo->nCollect = atol(sTemp.c_str());
				}				
				if(!Data["friendship"].isNull())
				{
					sTemp = Data["friendship"].asString();
					spUserInfo->nFlag = atol(sTemp.c_str());
				}

				return spUserInfo;
			}		
		}
		return NULL;
	}

	void *CResponseParser2::ParseCancelAttention(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(sResponse,value))
		{
			Json::Value vStatus = value["status"];
			Json::Value vMsg    = value["msg"];

			WebServiceResult* pResult(new WebServiceResult);
			pResult->isError  = (vStatus.isNull() ||  (vStatus.asString() != "ok"));
			if(!vMsg.isNull()) 
				pResult->ErrorMsg = vMsg.asString();

			return pResult;
		}

		return NULL;
	}

	void *CResponseParser2::ParseAddAttention(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(sResponse,value))
		{
			Json::Value vStatus = value["status"];
			Json::Value vMsg    = value["msg"];

			WebServiceResult* pResult(new WebServiceResult);
			pResult->isError  = (vStatus.isNull() ||  (vStatus.asString() != "ok"));
			if(!vMsg.isNull()) 
				pResult->ErrorMsg = vMsg.asString();

			return pResult;
		}

		return NULL;
	}

	void *CResponseParser2::ParseFanceList(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(sResponse, value))
		{
			std::string sStatus = value["status"].asString();
			if(sStatus == "ok")
			{				
				const Json::Value Data = value["data"];
				if(Data.isNull())
					return NULL;
				std::vector<spUserInfoT>* spUserInfoList = new std::vector<spUserInfoT>();
				for(int i=0; i<Data.size(); i++)
				{
					spUserInfoT spFance(new UserInfoT());
					std::string sTemp;
					if(!Data[i]["user_id"].isNull())
					{
						sTemp=Data[i]["user_id"].asString();
//						spFance->nUserID=atol(sTemp.c_str());
						spFance->sUserID = mci::utf8_2_CString(sTemp.c_str());
					}
	
					if(!Data[i]["nick_name"].isNull())
						spFance->sNickName=em_utility::mci::utf8_2_CString(Data[i]["nick_name"].asCString());
					if(!Data[i]["avatar"].isNull())
						spFance->sAvaterUrl=em_utility::mci::utf8_2_CString(Data[i]["avatar"].asCString());
					if(!Data[i]["city"].isNull())
						spFance->sArea=em_utility::mci::utf8_2_CString(Data[i]["city"].asCString());

					if(!Data[i]["friends"].isNull())
					{
						sTemp = Data[i]["friends"].asString();
						spFance->nFollowCount = atol(sTemp.c_str());
					}
					if(!Data[i]["fans"].isNull())
					{
						sTemp = Data[i]["fans"].asString();
						spFance->nFollowerCount = atol(sTemp.c_str());
					}
					if(!Data[i]["friendship"].isNull())
					{
						sTemp = Data[i]["friendship"].asString();
						spFance->nFlag = atol(sTemp.c_str());
					}			
					spUserInfoList->push_back(spFance);
				}
				return spUserInfoList;
			}
		}
		return NULL;

	}

	void* CResponseParser2::ParseSongList(std::string& sResponse)
	{
		assert(!sResponse.empty());
		if(sResponse.empty()) return NULL;

		Json::Reader reader;
		Json::Value value;
		if(!reader.parse(sResponse, value) || value.isNull()) return NULL;

		Json::Value statusValue = value["status"];
		Json::Value songsValue  = value["songs"];

		if("ok" != statusValue.asString()) return NULL;
		if(!songsValue.isArray()) return NULL;

		SearchResult *pResult = new SearchResult();
		if(!pResult) return NULL;

		pResult->bNext = CDataParser<>::getValue<bool>(value,"more");
	
// 		for(Json::Value::iterator it = songsValue.begin();it != songsValue.end();++it)
// 		{
// 			spSongInfoT theSong(new CSongInfo());
// 			assert(theSong);
// 			if(!theSong) continue;
// 
// 			CDataParser<>::getValue(*it,"song_id",theSong->nSongID);
// 			CDataParser<>::getValue(*it,"default_resource_id",theSong->nResourceID);
// 			CDataParser<>::getValue(*it,"album_id",theSong->nAlbumID);
// 			CDataParser<>::getValue(*it,"artist_id",theSong->nArtistID);
// 			CDataParser<>::getValue(*it,"grade",theSong->nGrade,-1);
// 			CDataParser<>::getValue(*it,"play_seconds",theSong->nAudioLength);
// 			CDataParser<>::getValue(*it,"title",theSong->sAlbumName);
// 			CDataParser<>::getValue(*it,"name",theSong->sSongName);
// 			CDataParser<>::getValue(*it,"artist_name",theSong->sArtistName);
// 			CDataParser<>::getValue(*it,"singers",theSong->sRealArtistName);
// 			CDataParser<>::getValue(*it,"album_logo",theSong->sCoverUrl);
// 			CDataParser<>::getValue(*it,"lyric_file",theSong->sLrcUrl);
// 			CDataParser<>::getValue(*it,"listen_file",theSong->sListenFileUrl);
		
			/*


			theSong->nSongID          = atol((*it)["song_id"].asCString());
			theSong->nResourceID      = atol((*it)["default_resource_id"].asCString());
			theSong->nAlbumID         = atol((*it)["album_id"].asCString());
			theSong->nArtistID        = atol((*it)["artist_id"].asCString());
			theSong->nGrade           = atol((*it)["grade"].asCString());
			theSong->nAudioLength     = atol((*it)["play_seconds"].asCString());
			theSong->sAlbumName       = ToCString((*it)["title"].asCString());
			theSong->sSongName        = ToCString((*it)["name"].asCString());
			theSong->sArtistName      = ToCString((*it)["artist_name"].asCString());
			theSong->sRealArtistName  = ToCString((*it)["singers"].asCString());
			theSong->sCoverUrl        = ToCString((*it)["album_logo"].asCString());
			theSong->sLrcUrl          = ToCString((*it)["lyric_file"].asCString());
		//	theSong->sListenFileUrl   = ToCString((*it)["listen_file"].asCString());
			CDataParser<>::getValue(*it,"listen_file",theSong->sListenFileUrl);
			*/
// 			assert(theSong->nSongID);
// 			assert(!theSong->sListenFileUrl.IsEmpty());
// 			if(theSong->nSongID && !theSong->sListenFileUrl.IsEmpty())
// 				pResult->SearchList.push_back(theSong);
// 		}
		return pResult;
	}

	void *CResponseParser2::ParseFavorites(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(sResponse, value))
		{
			std::string sStatus = value["status"].asString();
		
			if(sStatus == "ok")
			{	
				const Json::Value Songs = value["songs"];
				if(Songs.isNull())
					return NULL;
				SearchResult *psr = new SearchResult();

				Json::Value moreValue = value["more"];
				if(moreValue.isNull())
					psr->bNext = false;
				else
					psr->bNext = moreValue.asBool();

				for(int i=0;i<Songs.size();i++)
				{
					spSongInfoT spFavoriteSongs(new CSongInfo());
					std::string sTemp;
// 					if(!Songs[i]["song_id"].isNull())
// 					{
// 						sTemp=Songs[i]["song_id"].asString();
// 						spFavoriteSongs->nSongID=atol(sTemp.c_str());
// 					}
// 					if(!Songs[i]["default_resource_id"].isNull())
// 					{
// 						sTemp=Songs[i]["default_resource_id"].asString();
// 						spFavoriteSongs->nResourceID=atol(sTemp.c_str());
// 					}
// 					if(!Songs[i]["album_id"].isNull())
// 					{
// 						sTemp=Songs[i]["album_id"].asString();
// 						spFavoriteSongs->nAlbumID=atol(sTemp.c_str());
// 					}
// 					if(!Songs[i]["artist_id"].isNull())
// 					{
// 						sTemp=Songs[i]["artist_id"].asString();
// 						spFavoriteSongs->nArtistID=atol(sTemp.c_str());
// 					}
// 					if(!Songs[i]["grade"].isNull())
// 					{
// 						sTemp=Songs[i]["grade"].asString();
// 						spFavoriteSongs->nGrade=atol(sTemp.c_str());
// 					}
// 					if(!Songs[i]["name"].isNull())
// 					{
// 						spFavoriteSongs->sSongName=em_utility::mci::utf8_2_CString(Songs[i]["name"].asCString());
// 					}
// 					if(!Songs[i]["artist_name"].isNull())
// 					{
// 						spFavoriteSongs->sArtistName=em_utility::mci::utf8_2_CString(Songs[i]["artist_name"].asCString());
// 					}
// 					if(!Songs[i]["singers"].isNull())
// 					{
// 						spFavoriteSongs->sRealArtistName=em_utility::mci::utf8_2_CString(Songs[i]["singers"].asCString());
// 					}
// 					if(!Songs[i]["album_logo"].isNull())
// 					{
// 						spFavoriteSongs->sCoverUrl=em_utility::mci::utf8_2_CString(Songs[i]["album_logo"].asCString());
// 					}
// 					if(!Songs[i]["lyric_file"].isNull())
// 					{
// 						spFavoriteSongs->sLrcUrl=em_utility::mci::utf8_2_CString(Songs[i]["lyric_file"].asCString());
// 					}
// 					if(!Songs[i]["listen_file"].isNull())
// 					{
// 						spFavoriteSongs->sListenFileUrl=em_utility::mci::utf8_2_CString(Songs[i]["listen_file"].asCString());
// 					}
// 					
// 					if(Songs[i]["play_seconds"].isNull())
// 					{
// 						spFavoriteSongs->nAudioLength=0;
// 					}
// 					else
// 					{
// 						sTemp=Songs[i]["play_seconds"].asString();
// 						spFavoriteSongs->nAudioLength=atol(sTemp.c_str());
// 					}
					psr->SearchList.push_back(spFavoriteSongs);
				}
				
				return psr;
			}
		}
		return NULL;
	}

	void *CResponseParser2::ParseOperateFavorites(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(sResponse,value))
		{
			Json::Value vStatus = value["status"];

			WebServiceResult* pResult(new WebServiceResult);
			pResult->isError  = (vStatus.isNull() ||  (vStatus.asString() != "ok"));
			return pResult;
		}

		return NULL;
	}

	void *CResponseParser2::ParseSongCommen(std::string &sResponse)
	{
		//sResponse = {"status":"ok"}
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(sResponse,value))
		{
			Json::Value vStatus = value["status"];
			Json::Value vMsg    = value["msg"];

			WebServiceResult* pResult(new WebServiceResult);
			assert(pResult);
			pResult->isError  = (vStatus.isNull() ||  (vStatus.asString() != "ok"));
			if(!vMsg.isNull()) 
				pResult->ErrorMsg = vMsg.asString();

			return pResult;
		}
		return NULL;
	}

	void *CResponseParser2::ParseSendSong(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(sResponse,value))
		{
			Json::Value vStatus = value["status"];
			Json::Value vMsg    = value["msg"];

			WebServiceResult* pResult(new WebServiceResult);
			assert(pResult);
			pResult->isError  = (vStatus.isNull() ||  (vStatus.asString() != "ok"));
			if(!vMsg.isNull()) 
				pResult->ErrorMsg = vMsg.asString();

			return pResult;
		}
		return NULL;
	}

	void *CResponseParser2::ParseSendSongCommen(std::string &sResponse)
	{
		//sResponse = {"status":"ok"}
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(sResponse,value))
		{
			Json::Value vStatus = value["status"];
			Json::Value vMsg    = value["msg"];

			WebServiceResult* pResult(new WebServiceResult);
			assert(pResult);
			pResult->isError  = (vStatus.isNull() ||  (vStatus.asString() != "ok"));
			if(!vMsg.isNull()) 
				pResult->ErrorMsg = vMsg.asString();

			return pResult;
		}
		sLog("send song reply: %s",sResponse.c_str());
		return NULL;
	}

	void *CResponseParser2::ParseSendShareCommen(std::string &sResponse)
	{
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(sResponse,value))
		{
			Json::Value vStatus = value["status"];
			Json::Value vMsg    = value["msg"];
		
			WebServiceResult* pResult(new WebServiceResult);
			assert(pResult);
			pResult->isError  = (vStatus.isNull() ||  (vStatus.asString() != "ok"));
			if(!vMsg.isNull()) 
				pResult->ErrorMsg = vMsg.asString();
		
			return pResult;
		}

		return NULL;
	}

	//解析赞
	void *CResponseParser2::ParsePraise(std::string &sResponse)
	{

		//sResponse = {"status":"ok","msg":"\u5df2\u7ecf\u8d5e\u8fc7"};
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(sResponse,value))
		{
			Json::Value vStatus = value["status"];
			Json::Value vMsg    = value["msg"];

			WebServiceResult* pResult(new WebServiceResult);
			pResult->isError  = (vStatus.isNull() ||  (vStatus.asString() != "ok"));
			if(!vMsg.isNull()) 
				pResult->ErrorMsg = vMsg.asString();

			return pResult;
		}

		return NULL;
	}


