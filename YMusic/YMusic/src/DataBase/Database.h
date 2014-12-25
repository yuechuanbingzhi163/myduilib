#pragma once

#include "../../YMusic.h"
#include "CppSQLite3.h"
#pragma comment(lib,"sqlite3cpp.lib")
#include <vector>


	class  CDatabase
	{
	public:
		static const int  SQLERROR = -1;
		static const int  DBVersion = 0;
		static const int  BackupDBRate = 60*60*24;
		static const char DicTableName[]; 
		static const CString DBVersionKey;
		static const CString LastBackupKey;
		typedef CppSQLite3Query     SQLResult;
		typedef CppSQLite3Buffer    SQLBuffer;

		CDatabase():_pDB(NULL){}
		virtual ~CDatabase(){ CloseDB(); }

		bool OpenDB(CString sFileName);
		bool CloseDB();
		bool BackupDB(CString sFileName);
		bool IsOpen() const {return !!_pDB;}
		int GetDBVersion();

		bool GetDictValue(CString sKey, CString &sValue); //if sKey is exsited return true,if not exsit return false
		bool GetDictValue(CString sKey,int& nValue);
		bool GetDictValue(CString sKey,DWORD& dwValue);
		bool GetDictValue(CString sKey,bool& nValue);
		bool UpdateDictValue(CString sKey, CString sValue); //if Key is exsited update it and  return true,if not exsit return false
		bool UpdateDictValue(CString sKey,int nValue);
		bool UpdateDictValue(CString sKey,DWORD dwValue);
		bool UpdateDictValue(CString sKey,bool nValue);
		bool InsertDict(CString sKey, CString sValue);//if Key is exsited return false ,if not exsit insert new one and return true
        bool InsertDict(CString sKey,int nValue);  
		bool InsertDict(CString sKey,DWORD dwValue);
        bool InsertDict(CString sKey, bool nValue);
        bool DeleteDict(CString sKey);

	protected:
		int ExecScalar(const char* fmt,...);
		int ExecSql(const char* fmt,...);
		SQLResult QuerySql(const char* fmt,...);
	public:
		bool IsExistTable(const char* tablename);
		bool DeleteTable(const char* tablename);
		bool BeginTransaction();
		bool Commit();
		bool RollBack();
	protected:
		virtual bool OnCreateTables() = 0;
		virtual bool OnUpdateTables(int oldVer,int newVer) = 0;
	private:
		CppSQLite3DB * _pDB;
	};
	typedef smart_pointer::SmartPtr<CDatabase> spCDatabaseT;


	class CGlobalDBase : public CDatabase
	{
	public:
		static const char UserInfoTableName[];
		static const CString LastLogonUserKey;
	public:
		//得到所有本机登录过的用户
		bool GetAllUserInfo(std::vector<spLogonUserInfoT> &UserInfoList);
		//得到指定ID用户
		spLogonUserInfoT GetUserInfo( const char* sUserID);
		//添加用户
		bool AddUserInfo(spLogonUserInfoT spUserInfo);
		//更新用户
		bool UpdateUserInfo(spLogonUserInfoT spUserInfo);
		//删除用户
		bool RemoveUserInfo(const char* sUserID);
		bool RemoveUserInfo(unsigned int nUserID);
		//最后一个登录用户
		spLogonUserInfoT GetLastUserInfo();
		bool UpdateLastUserInfo(spLogonUserInfoT spUserInfo);

		bool GetAllPlayLists(std::vector<spPlayListInfoT> &PlayList);
		bool AddPlayList(spPlayListInfoT spListInfo);
		bool UpdatePlayList(spPlayListInfoT spListInfo);
		bool RemovePlayList(unsigned int nPlayListID);
	public:

	protected:
		bool CreateUserTable();
	protected:
		virtual bool OnCreateTables();
		virtual bool OnUpdateTables(int oldVer,int newVer);
	};
	typedef smart_pointer::SmartPtr<CGlobalDBase> spGlobalDBaseT;

	class CUserDBase:public CDatabase
	{
	public:
		static const char PlayListTableName[];//列表                  --存储用户本地所有列表
		static const char SongListTableName[];//歌曲表                --每个列表都对应一个歌曲表
		static const char FavoritesTableName[];//收藏				  --用户收藏表，需网络同步
		static const char PlayRecordTableName[];//播放记录			  --用户本地播放记录
		static const char DetailPlayRecordTableName[];//详细播放记录
		static const char LocalMusicPacketTableName[];//本地音乐包
		static const char OperationTableName[];//操作				  --用户操作历史
	public:	
		// --获取指定Id的列表信息
		bool GetPlayList(spPlayListInfoT& PlayListInfo,unsigned int nPlayListID);
		// --获取所有列表信息
		bool GetAllPlayLists(std::vector<spPlayListInfoT> &PlayList);
		// --获取指定类型的列表
		bool GetAllPlayListsByType(std::vector<spPlayListInfoT> &PlayList,unsigned int nType);
		// --添加用户列表
		bool AddPlayList(spPlayListInfoT spListInfo);
		// --更新列表信息
		bool UpdatePlayList(spPlayListInfoT spListInfo);
		// --移除指定列表
		bool RemovePlayList(unsigned int nPlayListID);
		// --重命名指定列表
		bool RenamePlayList(unsigned int nPlayListID,CString newName);
		// --检查某个名称的列表是否存在
		bool IsPlayListExists(CString sPlayListName);
		// --根据列表名称获取列表信息
		bool GetPlayListByName(CString sPlayListName,spPlayListInfoT& PlayListInfo);

		// --获取指定列表Id的歌曲列表
		bool GetPlayListSongs(std::vector<spSongInfoT> &SongList,unsigned int nPlayListID = 0  );
		// --添加歌曲到指定列表
		bool AddSongsToPlayList(spSongInfoT spSongInfo, unsigned int nPlayListID = 0);
		bool AddSongToPlayList(CString songPath,unsigned int nPlayListID,spSongInfoT& spSongInfo);
		bool AddSongsToPlayList(std::vector<spSongInfoT> &SongList, unsigned int nPlayListID = 0);
		// --更新指定列表Id的歌曲信息
		bool UpdatePlayListSongInfo(spSongInfoT spSongInfo, unsigned int nPlayListID = 0);
		bool UpdatePlayListSongInfo(std::vector<spSongInfoT> SongArray,unsigned int nPlayListID = 0);
		bool UpdatePlayListSongPath(spSongInfoT spSongInfo, unsigned int nPlayListID = 0);
		// --删除指定列表Id的歌曲
		bool RemovePlayListSongs(spSongInfoT spSongInfo, unsigned int nPlayListID = 0);
		bool RemovePlayListSongs(std::vector<spSongInfoT> &SongList, unsigned int nPlayListID = 0);
		// --清空指定列表Id的歌曲
		bool RemoveAllPlayListSongs( unsigned int nPlayListID = 0);
		// --删除指定列表Id的歌曲列表
		bool DeletePlayList( unsigned int nPlayListID = 0);

		// --我的收藏
		bool CreateFavTable();
		bool AddFavorites(spFavInfoT spFavInfo);
		bool AddFavorites(std::vector<spFavInfoT> &FavList);
		bool RemoveFavorites(unsigned int nId,unsigned int nType);
		bool RemoveFavorites(std::vector<spFavInfoT> &FavList);
		bool RemoveAllFavorites();
		bool GetAllFavorites(std::vector<spFavInfoT> &FavList);
	
		// --我的播放记录
		bool AddPlayDetailRecord(unsigned int nSongId,unsigned int nSecond,bool bSkip);
		bool GetRecentFavRecord(std::vector<spFavInfoT> &FavList);
		bool GetPlayRecord(std::vector<std::pair<int,int> >& DataArray,bool bSkipRecord);
	
		// --本地音乐包接口;
		bool GetLocalMusicPacket(unsigned int nPlayListLocalId,std::vector<std::pair<UINT,UINT> >& PacketArray,UINT nStartPos,UINT nLimit,bool bSong);
		bool AddMusicPacket(unsigned int nPlayListLocalId,const std::pair<UINT,UINT>& Packet);
		bool AddMusicPacket(unsigned int nPlayListLocalId,const std::vector<std::pair<UINT,UINT> >& PacketArray);
		bool RemoveMusicPacket(unsigned int nPlayListLocalId,const std::pair<UINT,UINT>& Packet);
		UINT GetMusicPacketCount(unsigned int nPlayListLocalId,bool bSong);
		
		// --Operation表操作;
		bool CreateOperationTable();
		bool GetAllOperation(std::vector<spSynOperationT>& OperationArray);
		bool AddOperation(spSynOperationT spSynOperation);
		bool DelOperation(spSynOperationT spSynOperation);
	protected:
		// --创建默认播放列表[试听列表](系统属性)
		bool AddDefaultPlayList();
		// --创建我的收藏列表[我的收藏](系统属性)
		bool AddDefaultFav();
		bool CreateTablePlayLists();
		bool CreateTableSongLists(const char* sPlayTableName);
	protected:
		virtual bool OnCreateTables();
		virtual bool OnUpdateTables(int oldVer,int newVer);
	};
	typedef smart_pointer::SmartPtr<CUserDBase> spUserDBaseT;