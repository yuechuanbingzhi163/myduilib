#pragma once

#include "WebServiceCatch.h"

#include "../../YMusic.h"

namespace url
{
	typedef std::string UTF8STR;
	typedef UTF8STR     URLSTR;
    
enum emShareObjT
{
	SOT_SONG = 32,
	SOT_ALBUM,
	SOT_ARTIST,
	SOT_AMBUS
};

    enum emRequestT
	{
		WSRT_NULL,
		//登录
		WSRT_LOGIN,
		WSRT_WEBLOGIN,
		//收藏
		WSRT_FAVORITE,
		WSRT_ADD_FAVORITE,
		WSRT_DEL_FAVORITE,
		//分享
		WSRT_SHARE,

		//播放统计
		WSRT_PLAYLOG,

		//智能列表、我的列表
		WSRT_GET_LIST,
		WSRT_NEW_LIST,
		WSRT_DEL_LIST,
		WSRT_RENAME_LIST,
		WSRT_CHANGE_STATUS,
		WSRT_LIST_GET_SONGS,
		WSRT_LIST_ADD_SONGS,
		WSRT_LIST_DEL_SONGS,
		WSRT_DETAIL_INFO,
		WSRT_LIST_CLEAR,
		
		//漫游列表;
		WSRT_GET_ROAMING,
		//匹配歌曲;
		WSRT_MATCH_SONG,
		//标签列表;
		WSRT_GET_TAG_LIST,
		WSRT_GET_TAG_SONGS,	
		//乐库推荐;
		WSRT_LIB_RECOMMEND_SONGS,
		WSRT_LIB_RECOMMEND_ALBUMS,
		//
		WSRT_SEARCH,
		WSRT_SEARCHMARK,
		WSRT_ATTENTIONLIST,
		WSRT_FANCELIST,
		WSRT_RECOMMENDWINDOW,
		WSRT_CANCELATTENTION,
		WSRT_PRAISE,
		WSRT_ADDATTENTION,
		WSRT_SENDSONG,
		WSRT_GETALLFRIENDS,
		WSRT_REPLYCOMMENT,
		WSRT_REPLYSENDSONG,
		WSRT_FRIENDUSERINFO,
		WSRT_MESSAGEINFOCOUNT,

		WSRT_RECOMMENDSENDSONG,
		WSRT_RECOMMENDGOOD,
		WSRT_RECOMMENDSHARE,
		WSRT_RECOMMEDNUPDATE
	}; 

    class CRequest
    {
    public:
//    typedef boost::function<void (CRequest*,void*)> ResponderT;
	  typedef void(*ResponderT)(CRequest*,void*); 
      CRequest(emRequestT requestType,const URLSTR& paths,const URLSTR& params);
      CRequest(emRequestT requestType,const URLSTR& paths,const URLSTR& params,ResponderT responder);
      ~CRequest();
        
      emRequestT GetRequestType() const;
      URLSTR GetPaths() const;
	  URLSTR GetParams() const;   

      HWND GetHwnd() const;
      void SetHwnd(HWND h); 

      DWORD GetTag() const;
      void SetTag(DWORD dwTag);

      CatchStrategyType GetCatchStrategyType() const;
	  void SetCatchStrategyType(CatchStrategyType nType);

      bool IsCatched() const;
	  void SetCatched(bool bCatch);

      bool IsTimeOut() const;
      void SetTimeOut(bool b);

	  int GetCreateTime();

      bool HasReponder() const;
      void SetReponder(ResponderT responder);
	  void Response(void* pdata);
    protected:
        ResponderT              _responder;
        HWND                    _hwnd;
		int                     _tCreate;
		URLSTR                  _sPaths;
		URLSTR                  _sParams; 
		emRequestT              _requestType;
        bool                    _bIsFromCatch;
		CatchStrategyType       _nStrategyType;
        bool                    _isTimeOut;
        DWORD                   _dwTag;
    };
	
	class Requester
	{
	private:
		const static std::string              _sAPI_URL;
		const static std::string              _sAPI_KEY;
		const static std::string              _sAPI_SECRET;
	public:
		Requester();
		~Requester();
		//测试
		static std::string GenSigUrl2(const std::string& sMethod, const std::vector<std::pair<std::string,std::string> >& v);
//		static std::string GenSigUrl3(const std::string& sMethod, const std::vector<std::pair<std::string,std::string> >& v);
		//end
		static std::string GenSigUrl( const std::string& sMethod, const std::vector<std::pair<std::string,std::string> >& v);
		static std::string GenSigUrl( const CString& sMethod, const std::vector<std::pair<CString,CString> >& v);
		static std::string GenSigUrlEX( const std::string& sMethod, const std::vector<std::pair<std::string,std::string> >& v);
		static std::string GenSigUrlEX(const CString& sMethod, const std::vector<std::pair<CString,CString> >& v);
		//登录
		static CRequest* MakeLogin(const UTF8STR& sUserName,const UTF8STR& sPasswordMd5);
		static CRequest* MakeLogin(LPCTSTR pszUserName,LPCTSTR pszPasswordMd5);
		static CRequest* MakeLogin(const UTF8STR& sToken);
		static CRequest* MakeLogin(LPCTSTR pszToken);
		//测试
		static CRequest* MakeLogin2(const UTF8STR& sUserName,const UTF8STR& sPasswordMd5);
//		static CRequest* MakeLogin3(const UTF8STR& sUserName,const UTF8STR& sPasswordMd5);
		//分享
		//static CRequest* MakeShare(uint nSendUserId,const UTF8STR& sCommenMsg,uint nObjId,emShareObjT objType = SOT_SONG);
		static CRequest* MakeShare(uint nSendUserId,LPCTSTR lpszCommentMsg,uint nObjId,emShareObjT objType = SOT_SONG);

		//播放统计
		static CRequest* MakePlayLog(uint nUserId,uint nSongId);
		//我的列表
	    static CRequest* MakeGetList(uint nUserId);
		static CRequest* MakeNewList(uint nUserId,uint nListId,const CString& sName,const CString& sSongs = CString(),bool bShow = true);
		static CRequest* MakeNewList(uint nUserId,uint nListId,const CString& sName,const std::vector<spSongInfoT>& songs,bool bShow = true);
	    static CRequest* MakeDelList(uint nUserId,uint nListId,bool bAll = true);
	    static CRequest* MakeReNameList(uint nUserId,uint nListId,const CString& sName);
		static CRequest* MakeChangeListStatus(uint nUserId,uint nListId,bool bShow);
	    static CRequest* MakeGetSongsFromList(uint nUserId,uint nListId,uint nPage,uint nType);
	    static CRequest* MakeAddSongToList(uint nUserId,uint nListId,const CString& sSongs);
	    static CRequest* MakeDelSongFromList(uint nUserId,uint nListId,const CString& sSongs);
	    static CRequest* MakeClearList(uint nUserId,uint nListId);

		//漫游列表;
		static CRequest* MakeGetRoamingSongs(uint nSongId,uint nListId);

		//标签列表
		static CRequest* MakeTagList(bool bLogin,const CString& sData,uint nCount = 1);
		static CRequest* MakeTagList(bool bLogin,const std::vector<std::pair<int,int> >& DataArray,uint nCount = 1);
		static CRequest* MakeGetTagListSongs(uint nType,uint nListWebId,uint nUserId = 0);

		//匹配歌曲;
		static CRequest* MakeMatchSong(std::vector<spSongInfoT> songs);
		//收藏数据;
		static CRequest* MakeGetFavData(uint nUserId,uint nPageIndex,uint nType);
		static CRequest* MakeAddFav(uint nUserId,uint nId,CFavInfo::FavType eType);
		static CRequest* MakeDelFav(uint nUserId,uint nId,CFavInfo::FavType eType);
		//歌曲详细信息;
		static CRequest* MakeDetailInfo(const CString& sIdArray,CFavInfo::FavType eType);
		static CRequest* MakeDetailInfo(const std::vector<uint>& IdArray,CFavInfo::FavType eType);

		//音乐包
		static CRequest* MakeMusicPacketDetailInfo(uint nUserId,uint nId,uint nType,uint nSonglimit,uint nSongPage,uint nLimit,uint nPage);

		//乐库推荐数据;
		static CRequest* MakeGetUnloginRecommendSongs(const CString& sPlayRecordData,uint nRetCount);
		static CRequest* MakeGetLoginRecommendSongs(uint nUserId,uint nRetCount);

		static CRequest* MakeGetUnloginRecommendAlbums(const CString& sPlayRecordData,uint nRetCount);
		static CRequest* MakeGetLoginRecommendAlbums(uint nUserId,uint nRetCount);
	};
}
