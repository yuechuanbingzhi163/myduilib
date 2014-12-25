#include "../Database/ISerialize.h"
#include "../Database/ShareData.h"
#include <vector>

namespace proto {
class base : 
	public ISerialize
{
public:
	base();
	~base();

	bool IsValid() const;
	std::wstring ErrMsg() const;
	bool HasMore() const;

protected:
	bool serial(node_type& node,bool isload);
protected:
	bool           _isValid;
	std::wstring   _sErrMsg;
	bool           _bMore;
};
/************************************************************************/
/*                                                                      */
/************************************************************************/
class login : 
	public base
{
public:
	spLogonUserInfoT GetUserInfo() const;
protected:
	bool serial(node_type& node,bool isload);
protected:
	spLogonUserInfoT  _pUserInfo;
};
/************************************************************************/
/*                                                                      */
/************************************************************************/
class songlist :
	public base
{
public:
	typedef std::vector<spSongInfoT> spSongsArrayT;
	spSongsArrayT _songs;
	int           _nListId;
protected:
	bool serial(node_type& node,bool isload);

};

/************************************************************************/
/*                                                                      */
/************************************************************************/
class listsongs :
	public base
{
public:
	typedef std::vector<spSongInfoT> spSongsArrayT;
	spSongsArrayT _songs;
protected:
	bool serial(node_type& node,bool isload);

};

/************************************************************************/
/*                                                                      */
/************************************************************************/
class play :
	public songlist
{
protected:
	bool serial(node_type& node,bool isload);
public:
	int    _nPlayIndex;
};
/************************************************************************/
/*                    获取网络列表                                      */
/************************************************************************/
class playlist:
	public base
{
protected:
	bool serial(node_type& node,bool isload);
public:
	std::vector<spPlayListInfoT> _ListInfoArray;
public:
	void  GetWebPlayListArray(std::vector<spPlayListInfoT>& ListInfoArray);
	
};

//class radio:
//	public base
//{
//protected:
//	bool serial(node_type& node,bool isload);
//public:
//	std::vector<spCRadioT> _RadioArray;
//public:
//	void GetRadioArray(std::vector<spCRadioT>& ListInfoArray);
//};
/************************************************************************/
/*         获取新建列表的网络id                                         */
/************************************************************************/

class playlistId:
	public base
{
private:
	UINT _nWebId;
protected:
	bool serial(node_type& node,bool isload);
public:
	UINT GetWebId();
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
class FavId:
	public base
{
public:
	std::vector<UINT>  _IdArray;

protected:
	bool serial(node_type& node,bool isload);
};

class matchsong:public base
{
public:
	typedef std::vector<spSongInfoT> spSongsArrayT;
	spSongsArrayT _songs;
public:
	bool serial(node_type& node,bool isload);
};

/************************************************************************/
/*                                                                      */
/************************************************************************/ 
namespace js{
	class IdType : 
		public base
	{
	public:
		enum{ID_ALBUM=1,ID_COLLECT,ID_ARTIST,ID_SONG,ID_LIST};
		std::size_t  _type;

		bool serial(node_type& node,bool isload);
	};
	class IdList :
		public IdType
	{
	public:
		std::vector<UINT>   _IdVec;
		std::size_t         _listId;
		std::wstring        _name;
	public:
		bool serial(node_type& node,bool isload);
	};
	class IdTokenList :
		public IdType
	{
	public:
		std::vector<CIDToken>  _idTokenArray;
		CIDToken               _listIdToken;
		std::wstring           _name;

		bool serial(node_type& node,bool isload);
	};

	class IdPaging :
		public IdType
	{
	public:
		std::size_t       _limit;
		std::size_t       _page;

		bool serial(node_type& node,bool isload);
	};
	/************************************************************************/
	/*                              收藏                                    */
	/************************************************************************/
	class FavList : 
		public base
	{
	public:
		std::vector<UINT> _favs;
		void SetHasMore(bool bHasMore);
		bool serial(node_type& node,bool isload);
	};
	/************************************************************************/
	/*                               分享                                   */
	/************************************************************************/
	class share :
		public base
	{
	public:
		std::size_t    _id;
		std::size_t    _type;
		std::wstring   _title;
		std::wstring   _comment;

		bool serial(node_type& node,bool isload);
	};
	/************************************************************************/
	/*                            音乐包相关                                */
	/************************************************************************/
	class musicPacketRequest :
		public base
	{
	public:
		std::size_t    _id;
		std::size_t    _type;
		std::size_t    _songlimit;
		std::size_t    _songpage;
		std::size_t    _limit;
		std::size_t    _page;

		bool serial(node_type& node,bool isload);
	};

	class LocalMusicPacketToken : public base
	{
	public:
		std::vector<CIDToken>  _idTokenArray;
		bool serial(node_type& node,bool isload);
	};
	/************************************************************************/
	/*                        弹窗                                          */
	/************************************************************************/

	class MsgBox : public base
	{
	public:
		std::wstring    _title;
		std::wstring    _text;
		int             _iconType;
		int             _flag;

		bool serial(node_type& node,bool isload);
	};

	/************************************************************************/
	/*                   推荐数据量限制                                     */
	/************************************************************************/
	class RecCount:public base
	{
	public:
		int nCount;
		bool serial(node_type& node,bool isload);
	};
}
}