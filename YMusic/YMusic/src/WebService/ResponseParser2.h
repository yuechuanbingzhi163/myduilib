#pragma once
#include "../Utilty/mt_helper.h"
#include <map>

	typedef void * (*ParseFunc)(std::string &sResponse);
	class CResponseParser2
	{
	public:
		CResponseParser2();
	void *Parse(unsigned int RequestType, std::string &sResponse);

	static void *ParseLogin(std::string &sResponse);
	static void *ParseFavorites(std::string &sResponse);
	static void *ParseOperateFavorites(std::string &sResponse);
	static void *ParseAttentionList(std::string &sResponse);
	static void *ParseFanceList(std::string &sResponse);
	static void *ParseRecommendInfo(std::string &sResponse);
	static void *ParseCancelAttention(std::string &sResponse);

	static void *CResponseParser2::ParseSongList(std::string& sResponse);

	static void *ParseSongCommen(std::string &sResponse);
	static void *ParseSendSong(std::string &sResponse);
	static void *ParseSendShareCommen(std::string &sResponse);
	static void *ParseSendSongCommen(std::string &sResponse);
	static void *ParsePraise(std::string &sResponse);
	static void *ParseAddAttention(std::string &sResponse);
	static void *ParseGetAllFriends(std::string &sResponse);
	static void *ParseGetFriendUserInfo(std::string &sResponse);

	static void *ParseGetMessageInfoCount(std::string &sResponse);
	static void *ParseSearchMark(std::string &sResponse);


	typedef std::map<unsigned int,ParseFunc> ParseFuncMapT;
	typedef ParseFuncMapT::iterator ParseFuncMapIterT;
	ParseFuncMapT m_ParseFuncMap;
	private:
		em_utility::critical_session m_lock;
	};

