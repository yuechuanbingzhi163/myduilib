#include <stdafx.h>

#include "Database.h"
#include <time.h>
#ifdef _DEBUG
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

#include "../YApp.h"

const int LAST_ID = 0;


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	const char CDatabase::DicTableName[] = "dictionary";
	const CString CDatabase::DBVersionKey = _T("DBVersion");
	const CString CDatabase::LastBackupKey = _T("_LastBackup");
	bool CDatabase::OpenDB(CString sFileName)
	{
		CloseDB();
		std::string sFileNameUTF8 = em_utility::mci::unicode_2_utf8(sFileName);
		_pDB = new CppSQLite3DB();
		try 
		{
			_pDB->open(sFileNameUTF8.c_str());
		}
		catch(CppSQLite3Exception e)
		{
			return false;
		}
		catch(...)
		{
			return false;
		}

		if(!IsOpen())
			return false;

		try
		{
			if(!IsExistTable(DicTableName))
			{
				static const char sSql[] = "CREATE TABLE %Q (key text primary key,value text,comment text)";
				if(SQLERROR == this->ExecSql(sSql,DicTableName))
					goto _OnError;
			}
			if(!OnCreateTables())
				goto _OnError;

			const int OldDBVersion = GetDBVersion();
			sLog("OldDBVersion:%d",OldDBVersion);
			if(OldDBVersion > DBVersion)
			{
// 				theNotifyCenter->sendNotifyOnMainThread(NT_VersionExpired,
// 													   OldDBVersion,
// 													   DBVersion);
				goto _OnError;
			}

			if(OldDBVersion < DBVersion)
			{
				if(!BackupDB(sFileName))
					goto _OnError;

				if(!OnUpdateTables(OldDBVersion,DBVersion))
					goto _OnError;

				DeleteDict(DBVersionKey);
				if(!InsertDict(DBVersionKey,DBVersion))
					goto _OnError;
			}
			else
			{
				int nLastBackupTime = 0;
				if(GetDictValue(LastBackupKey,nLastBackupTime))
				{
					int nOffTime = (int)time(NULL) - nLastBackupTime;
					if(nOffTime > BackupDBRate)
							BackupDB(sFileName);
				}
			}
		}catch(CppSQLite3Exception e)
		{
			assert(false);
			sLogError("openDB error(%s)",e.errorMessage());
			goto _OnError;
		}
		catch(...)
		{
			assert(false);
			goto _OnError;
		}
		return true;

_OnError:
		{
			this->CloseDB();
			return false;
		}
	}		

	bool CDatabase::CloseDB()
	{ 
		if(IsOpen())
		{
			_pDB->close();
			delete _pDB;
			_pDB = NULL;
		}
		return true;
	}

	bool CDatabase::BackupDB(CString sFileName)
	{
		CString newFileName(sFileName);
		newFileName += _T(".bak");
		if(::CopyFile(sFileName,newFileName,FALSE))
		{
			DeleteDict(LastBackupKey);
			InsertDict(LastBackupKey,(int)time(NULL));
			return true;
		}
		
		return false;
	}
	int CDatabase::GetDBVersion()
	{
		int nVer = -1;
		GetDictValue(DBVersionKey,nVer);
		return nVer;
	}

	bool CDatabase::GetDictValue(CString sKey, CString &sValue)
	{
		static const char sSql[] = "select value from %Q where key = %Q;";

		std::string sKeyUTF8 = em_utility::mci::unicode_2_utf8(sKey);

		SQLResult rs = this->QuerySql(sSql,DicTableName,sKeyUTF8.c_str());

		if(rs.eof()) return false;

		sValue = em_utility::mci::utf8_2_CString(rs.getStringField(0));
		return true;
	}
	bool CDatabase::GetDictValue(CString sKey,int& nValue)
	{
		CString sValue;
		if(!GetDictValue(sKey,sValue) || sValue.IsEmpty())
			return false;
		std::string utf8Str = em_utility::mci::unicode_2_utf8(sValue.GetBuffer());
		nValue =  atoi(utf8Str.c_str());
		return true;
	}
	bool CDatabase::GetDictValue(CString sKey,DWORD& dwValue)
	{
		CString sValue;
		if(!GetDictValue(sKey,sValue) || sValue.IsEmpty())
			return false;
		std::string utf8Str = em_utility::mci::unicode_2_utf8(sValue.GetBuffer());
		dwValue =  atol(utf8Str.c_str());
		return true;
	}
	bool CDatabase::GetDictValue(CString sKey,bool& nValue)
	{
		int nV = 0;
		if(!GetDictValue(sKey,nV))
			return false;
		nValue = (0 != nV);
		return true;
	}

	bool CDatabase::UpdateDictValue(CString sKey, CString sValue)
	{
		static const char sSql[] = "update %Q set value = %Q where key =%Q;";

		std::string sKeyUTF8   = em_utility::mci::unicode_2_utf8(sKey);
		std::string sValueUTF8 = em_utility::mci::unicode_2_utf8(sValue);

		return SQLERROR != this->ExecSql(sSql,DicTableName,sValueUTF8.c_str(),sKeyUTF8.c_str());
	}
	bool CDatabase::UpdateDictValue(CString sKey,int nValue)
	{
		CString sValue;
		sValue.Format(_T("%d"),nValue);
		return UpdateDictValue(sKey,sValue);
	}
	bool CDatabase::UpdateDictValue(CString sKey,DWORD dwValue)
	{
		return UpdateDictValue(sKey,(int)dwValue);
	}
	bool CDatabase::UpdateDictValue(CString sKey,bool nValue)
	{
		return UpdateDictValue(sKey,(int)nValue);
	}

	bool CDatabase::InsertDict(CString sKey, CString sValue)
	{
		static const char sSql[] = "insert into %Q(key,value) values (%Q,%Q);";

		std::string sKeyUTF8 = em_utility::mci::unicode_2_utf8(sKey);
		std::string sValueUTF8 = em_utility::mci::unicode_2_utf8(sValue);

		return SQLERROR != this->ExecSql(sSql,DicTableName,sKeyUTF8.c_str(),sValueUTF8.c_str());
	}
	bool CDatabase::InsertDict(CString sKey,int nValue)
	{
		CString sValue;
		sValue.Format(_T("%d"),nValue);
		return InsertDict(sKey,sValue);
	}
	bool CDatabase::InsertDict(CString sKey,DWORD dwValue)
	{
		return InsertDict(sKey,(int)dwValue);
	}
	bool CDatabase::InsertDict(CString sKey,bool nValue)
	{
		return InsertDict(sKey,(int)nValue);
	}

	bool CDatabase::DeleteDict(CString sKey)
	{
		static const char sSql[] = "delete from  %Q where key = %Q;";
	
		std::string sKeyUTF8 = em_utility::mci::unicode_2_utf8(sKey);

		return SQLERROR != this->ExecSql(sSql,DicTableName,sKeyUTF8.c_str());
	}

	int CDatabase::ExecScalar(const char* fmt,...)
	{
		assert(IsOpen());
		assert(fmt);
		if(!IsOpen() || !fmt) return SQLERROR;

		int nResult = SQLERROR;
		SQLBuffer buffer;
		try
		{
			va_list va;
			va_start(va,fmt);
			buffer.format(fmt,va);
			va_end(va);
			nResult = _pDB->execScalar(buffer);
		}
		catch (CppSQLite3Exception e)
		{
			sLogError("ExecScalar<%s>,error<%s>",buffer,e.errorMessage());
			assert(false);
			return SQLERROR;
		}
		catch (...)
		{
			sLogError("ExecScalar<%s>,error<unknow>",buffer);
			assert(false);
			return SQLERROR;
		}

		return nResult;
	}
	int CDatabase::ExecSql(const char* fmt,...)
	{
		assert(IsOpen());
		assert(fmt);
		if(!IsOpen() || !fmt) return SQLERROR;

		int nResult = SQLERROR;
		SQLBuffer buffer;
		try
		{
			va_list va;
			va_start(va,fmt);
			buffer.format(fmt,va);
			va_end(va);
			nResult = _pDB->execDML(buffer);
//			sLog("ExecSql<%s>",buffer);
			CString sbuffer(mci::utf8_2_CString(buffer));
			sLog("ExecSql<%s>",mci::unicode_2_ansi(sbuffer).c_str());
		}
		catch (CppSQLite3Exception e)
		{
			sLogError("ExecSql<%s>,error<%s>",buffer,e.errorMessage());
			assert(false);
			return SQLERROR;
		}
		catch (...)
		{
			sLogError("ExecSql<%s>,error<unknow>",buffer);
			assert(false);
			return SQLERROR;
		}

		return nResult;
	}
	CDatabase::SQLResult CDatabase::QuerySql(const char* fmt,...)
	{
		assert(IsOpen());
		assert(fmt);
		if(!IsOpen() || !fmt) return SQLResult();

		SQLResult rs;
		SQLBuffer buffer;
		try
		{
			va_list va;
			va_start(va,fmt);
			buffer.format(fmt,va);
			va_end(va);
			rs = _pDB->execQuery(buffer);
//			sLog("QuerySql<%s>",buffer);
			CString sbuffer(mci::utf8_2_CString(buffer));
			sLog("QuerySql<%s>",mci::unicode_2_ansi(sbuffer).c_str());
		}
		catch (CppSQLite3Exception e)
		{
			sLogError("QuerySql<%s>,error<%s>",buffer,e.errorMessage());
			assert(false);
			return SQLResult();
		}
		catch (...)
		{
			sLogError("QuerySql<%s>,error<unknow>",buffer);
			assert(false);
			return SQLResult();
		}

		return rs;
	}
	bool CDatabase::IsExistTable(const char* tablename)
	{
		assert(IsOpen());
		if(!IsOpen()) return false;
		return _pDB->tableExists(tablename);
	}
	bool CDatabase::DeleteTable(const char* tablename)
	{
		return SQLERROR != ExecSql("DROP TABLE %q;",tablename);
	}
	
	bool CDatabase::BeginTransaction()
	{
		sLog("Begin Transaction");
		return SQLERROR != ExecSql("begin transaction;");
	}
	bool CDatabase::Commit()
	{
		sLog("Commit Transaction");
		return SQLERROR != ExecSql("commit transaction;");
	}
	bool CDatabase::RollBack()
	{
		sLog("RollBack Transaction");
		return SQLERROR != ExecSql("rollback transaction");
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	const char CGlobalDBase::UserInfoTableName[] = "user_lists";
	const CString CGlobalDBase::LastLogonUserKey = _T("_lastLogonUser");

	bool CGlobalDBase::CreateUserTable()
	{
		sLog("[Begin]创建用户列表<table:%s>----数据库",UserInfoTableName);
		static const char sSql[] = "create table %Q (id Integer primary key autoincrement ,user_id text unique,email text unique,password text,nick_name text,keep_logged int);";
		try
		{
			if(!this->IsExistTable(UserInfoTableName))
				this->ExecSql(sSql,UserInfoTableName);
		}
		catch(CppSQLite3Exception e)
		{
			sLogError("[End]创建用户列表<table:%s>----数据库",UserInfoTableName);
			assert(false);
			return SQLERROR;
		}
		catch(...)
		{
			sLogError("[End]创建用户列表<table:%s>----数据库",UserInfoTableName);
			assert(false);
			return SQLERROR;
		}
		sLog("[End]创建用户列表<table:%s>----数据库",UserInfoTableName);
		return true;
	}

	bool CGlobalDBase::GetAllUserInfo(std::vector<spLogonUserInfoT> &UserInfoList)
	{
		sLog("[Begin]获取用户列表<table:%s>---数据库",UserInfoTableName);
		static const char sSql[] = "select user_id,email,password,nick_name,keep_logged from %Q;";
		SQLResult rs = this->QuerySql(sSql,UserInfoTableName);

		while(!rs.eof())
		{
			spLogonUserInfoT spInfo(new LogonUserInfoT);
			if(!spInfo)
				return false;
			spInfo->sUserID      = em_utility::mci::utf8_2_CString(rs.getStringField(0));
			spInfo->sLogonEmail  = em_utility::mci::utf8_2_CString(rs.getStringField(1));
			spInfo->sPasswordMD5 = em_utility::mci::utf8_2_CString(rs.getStringField(2));
			spInfo->sNickName    = em_utility::mci::utf8_2_CString(rs.getStringField(3));
			spInfo->bKeepLogged  = rs.getIntField(4) ? true : false;

			UserInfoList.push_back(spInfo);
			rs.nextRow();
		}
		sLog("[End]获取用户列表<table:%s>----数据库",UserInfoTableName);

		return true;
	}

	spLogonUserInfoT CGlobalDBase::GetUserInfo( const char* sUserID)
	{
		sLog("[Begin]获取用户信息<table:%s><user_id:%s>----数据库",UserInfoTableName,sUserID);
		static  const char sSql[] = "select user_id,email,password,nick_name,keep_logged from %Q where user_id = %Q;";
		SQLResult rs = this->QuerySql(sSql,UserInfoTableName,sUserID);

		spLogonUserInfoT info;
		if(!rs.eof())
		{
			info = spLogonUserInfoT(new LogonUserInfoT);

			info->sUserID        = em_utility::mci::utf8_2_CString(rs.getStringField(0));
			info->sLogonEmail    = em_utility::mci::utf8_2_CString(rs.getStringField(1));
			info->sPasswordMD5   = em_utility::mci::utf8_2_CString(rs.getStringField(2));
			info->sNickName      = em_utility::mci::utf8_2_CString(rs.getStringField(3));
			info->bKeepLogged    = rs.getIntField(4) ? true : false;
		}
		sLog("[End]获取用户信息<table:%s><user_id:%s>----数据库",UserInfoTableName,sUserID);

		return info;
	}

	bool CGlobalDBase::AddUserInfo(spLogonUserInfoT spUserInfo)
	{
		assert(spUserInfo);
		if(!spUserInfo) return false;
		sLog("[Begin]添加用户信息<table:%s><user_id:%s>----数据库",UserInfoTableName,em_utility::mci::unicode_2_ansi(spUserInfo->sUserID).c_str());

		static const char sSql[] = "insert into %Q(user_id ,email,password,nick_name,keep_logged) values (%Q,%Q,%Q,%Q,%u)";

		std::string sLogId    = em_utility::mci::unicode_2_utf8(spUserInfo->sUserID);
		std::string sEmail    = em_utility::mci::unicode_2_utf8(spUserInfo->sLogonEmail);
		std::string sPassword = em_utility::mci::unicode_2_utf8(spUserInfo->sPasswordMD5);
		std::string sNickName = em_utility::mci::unicode_2_utf8(spUserInfo->sNickName);

		int nErrCode = this->ExecSql(
			sSql,
			UserInfoTableName,
			sLogId.c_str(),
			sEmail.c_str(),
			sPassword.c_str(),
			sNickName.c_str(),
			(spUserInfo->bKeepLogged?1:0)
			);
		sLog("[End]添加用户信息<table:%s><user_id:%s>----数据库",UserInfoTableName,em_utility::mci::unicode_2_ansi(spUserInfo->sUserID).c_str());

		return SQLERROR != nErrCode;
	}

	bool CGlobalDBase::UpdateUserInfo(spLogonUserInfoT spUserInfo)
	{
		assert(spUserInfo);
		if(!spUserInfo) return false;
		sLog("[Begin]更新用户信息<table:%s><user_id:%s>----数据库",UserInfoTableName,em_utility::mci::unicode_2_ansi(spUserInfo->sUserID).c_str());

		static const char sSql[] = "update %Q set email = %Q, password = %Q,nick_name = %Q,keep_logged = %u where user_id = %Q";
		
		std::string sLogId    = em_utility::mci::unicode_2_utf8(spUserInfo->sUserID);
		std::string sEmail    = em_utility::mci::unicode_2_utf8(spUserInfo->sLogonEmail);
		std::string sPassword = em_utility::mci::unicode_2_utf8(spUserInfo->sPasswordMD5);
		std::string sNickName = em_utility::mci::unicode_2_utf8(spUserInfo->sNickName);

		int nErrCode = this->ExecSql(
			sSql,
			UserInfoTableName,
			sEmail.c_str(),
			sPassword.c_str(),
			sNickName.c_str(),
			(spUserInfo->bKeepLogged?1:0),
			sLogId.c_str()
			);

		sLog("[End]更新用户信息<table:%s><user_id:%s>----数据库",UserInfoTableName,em_utility::mci::unicode_2_ansi(spUserInfo->sUserID).c_str());

		return SQLERROR != nErrCode;
	}

	bool CGlobalDBase::RemoveUserInfo(const char* sUserID)
	{
		sLog("[Begin]移除用户信息<table:%s><user_id:%s>----数据库",UserInfoTableName,sUserID);

		static const char sSql[] = "delete from %Q where user_id = %Q";

		int nErrCode = this->ExecSql(
			sSql,
			UserInfoTableName,
			sUserID
			);

		sLog("[End]移除用户信息<table:%s><user_id:%s>----数据库",UserInfoTableName,sUserID);

		return SQLERROR != nErrCode;
	}
	bool CGlobalDBase::RemoveUserInfo(unsigned int nUserID)
	{
		static const char sSql[] = "delete from %Q where user_id = %u";

		return SQLERROR != this->ExecSql(
			sSql,
			UserInfoTableName,
			nUserID
			);
	}

	spLogonUserInfoT CGlobalDBase::GetLastUserInfo()
	{
		sLog("[Begin]获取最后一个登录用户信息<table:%s>----数据库",UserInfoTableName);
		CString sValue;
		int nUserID = 0;
		std::string sUserID;
		if(GetDictValue(LastLogonUserKey,sValue) && !sValue.IsEmpty())
		{
			std::string utf8Str = em_utility::mci::unicode_2_utf8(sValue.GetBuffer());
			nUserID = atoi(utf8Str.c_str());
			sUserID = utf8Str;
			sValue.ReleaseBuffer();
		}

		if(""==sUserID)
		{
			sLogError("[End]获取最后一个登录用户信息<table:%s>----数据库",UserInfoTableName);
			return spLogonUserInfoT();
		}
		sLog("[End]获取最后一个登录用户信息<table:%s>----数据库",UserInfoTableName);
		return GetUserInfo(sUserID.c_str());
	}

	bool CGlobalDBase::UpdateLastUserInfo(spLogonUserInfoT spUserInfo)
	{
		assert(spUserInfo);
		if(!spUserInfo) return false;
		sLog("[Begin]更新最后一个登录用户信息<table:%s><user_id:%s>----数据库",UserInfoTableName,em_utility::mci::unicode_2_ansi(spUserInfo->sUserID).c_str());
// 		CString sUserID;
// 		sUserID.Format(_T("%u"),spUserInfo->nUserID);
		DeleteDict(LastLogonUserKey);
//		return InsertDict(LastLogonUserKey,sUserID);
		sLog("[End]更新最后一个登录用户信息<table:%s><user_id:%s>----数据库",UserInfoTableName,em_utility::mci::unicode_2_ansi(spUserInfo->sUserID).c_str());
		return InsertDict(LastLogonUserKey,spUserInfo->sUserID);
	}

	bool CGlobalDBase::OnCreateTables()
	{
		return CreateUserTable();
	}
	bool CGlobalDBase::OnUpdateTables(int oldVer,int newVer)
	{
		return true;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	const char CUserDBase::PlayListTableName[] = "play_lists";
	const char CUserDBase::SongListTableName[] = "song_lists";
	const char CUserDBase::FavoritesTableName[] = "favorites";
	const char CUserDBase::PlayRecordTableName[] = "play_record";
	const char CUserDBase::DetailPlayRecordTableName[] = "play_record_detail";
	const char CUserDBase::LocalMusicPacketTableName[] = "local_music_packet";
	const char CUserDBase::OperationTableName[] = "operation";
	/*创建播放列表*/
	bool CUserDBase::CreateTablePlayLists()
	{
		sLog("[Begin]创建播放列表<table:%s>----数据库",PlayListTableName);
		static const char sSql[] = "create table %Q (local_id Integer primary key autoincrement,"
			"web_id int,list_name text unique,type int,is_show int,gmt_create int,gmt_last_modify int,flag int);";
		try
		{
			if(!this->IsExistTable(PlayListTableName))
				this->ExecSql(sSql,PlayListTableName);
		}
		catch(CppSQLite3Exception e)
		{
			assert(false);
			return SQLERROR;
		}
		catch(...)
		{
			assert(false);
			return SQLERROR;
		}
		sLog("[End]创建播放列表<table:%s>----数据库",PlayListTableName);

		AddDefaultPlayList();
		AddDefaultFav();

		return true;
	}
	/*根据列表创建歌曲数据库*/
	bool CUserDBase::CreateTableSongLists(const char* sPlayTableName)
	{
		assert(sPlayTableName);
		sLog("[Begin]创建歌曲列表<table:%s>----数据库",sPlayTableName);
		//song_local_id,song_id,song_name,artist_id,artist_name,album_id,"
		//album_name,cover_url,cover_path,listen_url,audio_length,"
		//is_disable,lrc_url,audio_type,audio_size,audio_start_pos,"
		//md5,local_path,is_local,gmt_last_play,gmt_last_modify,gmt_add_time,play_count
		static const char sSql[] = "create table %Q (song_local_id Integer primary key autoincrement,song_id int,song_name text,artist_id int,artist_name text,"
			"album_id int,album_name text,cover_url text,cover_path text,listen_url text,audio_length int,is_disable int,lrc_url text,audio_type int,"
			"audio_size int,audio_start_pos int,md5 text,local_path text,is_local int,gmt_last_play int,gmt_last_modify int,gmt_add_time int,play_count int default 0);";
		try
		{
			if(!this->IsExistTable(sPlayTableName))
				this->ExecSql(sSql,sPlayTableName);
		}
		catch(CppSQLite3Exception e)
		{
			sLogError("[End]创建歌曲列表<table:%s>----数据库",sPlayTableName);
			assert(false);
			return SQLERROR;
		}
		catch(...)
		{
			sLogError("[End]创建歌曲列表<table:%s>----数据库",sPlayTableName);
			assert(false);
			return SQLERROR;
		}
		sLog("[End]创建歌曲列表<table:%s>----数据库",sPlayTableName);
		return true;
	}
	bool CUserDBase::GetPlayList(spPlayListInfoT& PlayListInfo,unsigned int nPlayListID)
	{
		sLog("[Begin]获取指定<local_id:%d>播放列表<table:%s>----数据库",nPlayListID,PlayListTableName);
		static const char sSql[] = "select local_id,web_id,list_name,type,is_show,flag from %Q where local_id = %u;";

		SQLResult rs = this->QuerySql(sSql,PlayListTableName,nPlayListID);
		if(!rs.eof())
		{
			if(!PlayListInfo)
				PlayListInfo = spPlayListInfoT(new CPlayListInfo);
			if(!PlayListInfo)
				return false;
			PlayListInfo->SetLocalId(rs.getIntField(0));
			PlayListInfo->SetId(rs.getIntField(1));
			PlayListInfo->SetName(em_utility::mci::utf8_2_CString(rs.getStringField(2)));
			PlayListInfo->SetListType((CPlayListInfo::PlayListType)(rs.getIntField(3)));
			PlayListInfo->SetShow(rs.getIntField(4)==1 ? true :false);
			PlayListInfo->SetFlag(rs.getIntField(5));

		}
		sLog("[End]获取指定<local_id:%d>播放列表<table:%s>----数据库",nPlayListID,PlayListTableName);
		return true;
	}
	/*获取用户播放列表*/
	bool CUserDBase::GetAllPlayLists(std::vector<spPlayListInfoT> &PlayList)
	{
		sLog("[Begin]获取播放列表<table:%s>----数据库",PlayListTableName);
		static const char sSql[] = "select local_id,web_id,list_name,type,is_show,flag from %Q;";

		SQLResult rs = this->QuerySql(sSql,PlayListTableName);
		while(!rs.eof())
		{
			spPlayListInfoT spInfo(new CPlayListInfo);
			if(!spInfo)
				return false;
			spInfo->SetLocalId(rs.getIntField(0));
			spInfo->SetId(rs.getIntField(1));
			spInfo->SetName(em_utility::mci::utf8_2_CString(rs.getStringField(2)));
			spInfo->SetListType((CPlayListInfo::PlayListType)(rs.getIntField(3)));
			spInfo->SetShow(rs.getIntField(4)==1 ? true :false);
			spInfo->SetFlag(rs.getIntField(5));

			PlayList.push_back(spInfo);
			rs.nextRow();
		}
		sLog("[End]获取播放列表<table:%s>----数据库",PlayListTableName);
		return true;
	}
	
	bool CUserDBase::GetAllPlayListsByType(std::vector<spPlayListInfoT> &PlayList,unsigned int nType)
	{
		sLog("[Begin]获取<type:%d>播放列表<table:%s>----数据库",nType,PlayListTableName);
		static const char sSql[] = "select local_id,web_id,list_name,type,is_show,flag from %Q where type=%d;";

		SQLResult rs = this->QuerySql(sSql,PlayListTableName,nType);
		while(!rs.eof())
		{
			spPlayListInfoT spInfo(new CPlayListInfo);
			if(!spInfo)
				return false;
			spInfo->SetLocalId(rs.getIntField(0));
			spInfo->SetId(rs.getIntField(1));
			spInfo->SetName(em_utility::mci::utf8_2_CString(rs.getStringField(2)));
			spInfo->SetListType((CPlayListInfo::PlayListType)(rs.getIntField(3)));
			spInfo->SetShow(rs.getIntField(4)==1 ? true :false);
			spInfo->SetFlag(rs.getIntField(5));

			PlayList.push_back(spInfo);
			rs.nextRow();
		}
		sLog("[End]获取<type:%d>播放列表<table:%s>----数据库",nType,PlayListTableName);
		return true;
	}
	
	/*新建播放列表*/
	bool CUserDBase::AddPlayList(spPlayListInfoT spListInfo)
	{
		assert(spListInfo);
		if(!spListInfo) return false;

		sLog("[Begin]新建播放列表<table:%s>----数据库",PlayListTableName);

		static const char sSql[] = "insert into %Q(web_id,list_name,type,is_show,gmt_create,gmt_last_modify,flag) values (%u,%Q,%u,%u,%u,%u,%u);";

		std::string sNameUTF8    = em_utility::mci::unicode_2_utf8(spListInfo->GetName());
		int gmt = (int)time(NULL);
		bool bResult = (this->ExecSql(
			sSql,
			PlayListTableName,
			spListInfo->GetId(),
			sNameUTF8.c_str(),
			(int)(spListInfo->GetListType()),
			spListInfo->IsShow() ? 1 : 0,
			gmt,
			gmt,
			spListInfo->GetFlag()
			)!=SQLERROR);

		if (bResult)
		{
			SQLResult rs = this->QuerySql("select last_insert_rowid() from %Q",PlayListTableName);
			if(rs.eof())
				return false;
			spListInfo->SetLocalId(rs.getIntField(0));

			char szBuf[MAX_PATH] = {0};
			sprintf_s(szBuf,MAX_PATH,"%s_%d",SongListTableName,spListInfo->GetLocalId());
			CreateTableSongLists(szBuf);
		}
		sLog("[End]新建播放列表<table:%s>----数据库",PlayListTableName);
		return bResult;
	}
	/*更新播放列表*/
	bool CUserDBase::UpdatePlayList(spPlayListInfoT spListInfo)
	{
		assert(spListInfo);
		if(!spListInfo) return false;

		sLog("[Begin]更新指定<local_id:%d>播放列表<table:%s>----数据库",spListInfo->GetLocalId(),PlayListTableName);

		static const char sSql[] = "update %Q set web_id = %u ,list_name = %Q ,is_show = %u,gmt_last_modify = %u where local_id = %u;";

		std::string sNameUTF8    = em_utility::mci::unicode_2_utf8(spListInfo->GetName());

		int nErrCode = this->ExecSql(
			sSql,
			PlayListTableName,
			spListInfo->GetId(),
			sNameUTF8.c_str(),
			spListInfo->IsShow() ? 1 : 0,
			(int)(time(NULL)),
			spListInfo->GetLocalId()
			);
		sLog("[End]更新指定<local_id:%d>播放列表<table:%s>----数据库",spListInfo->GetLocalId(),PlayListTableName);

		return SQLERROR != nErrCode;
	}
	/*移除播放列表*/
	bool CUserDBase::RemovePlayList(unsigned int nPlayListID)
	{
		sLog("[Begin]移除指定<local_id:%d>播放列表<table:%s>----数据库",nPlayListID,PlayListTableName);
		static const char sSql[] = "delete from %Q where local_id = %u;";

//		RemoveAllPlayListSongs(nPlayListID);
		// --先删除对于Id的歌曲列表
		DeletePlayList( nPlayListID );

		int nErrCode = this->ExecSql(sSql,PlayListTableName,nPlayListID);

		sLog("[End]移除指定<local_id:%d>播放列表<table:%s>----数据库",nPlayListID,PlayListTableName);

		return SQLERROR != nErrCode;
	}
	bool CUserDBase::RenamePlayList(unsigned int nPlayListID,CString newName)
	{
		sLog("[Begin]更新指定<local_id:%d>播放列表<table:%s>----数据库",nPlayListID,PlayListTableName);

		static const char sSql[] = "update %Q set list_name = %Q gmt_last_modify = %u where local_id = %u;";

		std::string sNameUTF8    = em_utility::mci::unicode_2_utf8(newName);

		int nErrCode = this->ExecSql(
			sSql,
			PlayListTableName,
			sNameUTF8.c_str(),
			(int)(time(NULL)),
			nPlayListID
			);
		sLog("[End]更新指定<local_id:%d>播放列表<table:%s>----数据库",nPlayListID,PlayListTableName);

		return SQLERROR != nErrCode;
	}
	bool CUserDBase::IsPlayListExists(CString sPlayListName)
	{
		sLog("[Begin]检查指定<list_name:%s>播放列表<table:%s>----数据库",mci::unicode_2_ansi(sPlayListName.GetBuffer()).c_str(),PlayListTableName);
		sPlayListName.ReleaseBuffer();

		static const char sSql[] = "select local_id,web_id,list_name from %Q where  list_name = %Q;";
		SQLResult rs = this->QuerySql(sSql,PlayListTableName,mci::unicode_2_utf8(sPlayListName).c_str());
		bool bFind = false;
		if(!rs.eof())
			bFind =  true;
		else 
			bFind =  false;

		sLog("[End]检查指定<list_name:%s>播放列表<table:%s>----数据库",mci::unicode_2_ansi(sPlayListName.GetBuffer()).c_str(),PlayListTableName);
		sPlayListName.ReleaseBuffer();
		return bFind;
	}
	bool CUserDBase::GetPlayListByName(CString sPlayListName,spPlayListInfoT& PlayListInfo)
	{
		sLog("[Begin]获取指定<list_name:%s>播放列表<table:%s>----数据库",mci::unicode_2_ansi(sPlayListName.GetBuffer()).c_str(),PlayListTableName);
		sPlayListName.ReleaseBuffer();
		static const char sSql[] = "select local_id,web_id,list_name,type,is_show,flag from %Q where list_name = %Q;";

		SQLResult rs = this->QuerySql(sSql,PlayListTableName,mci::unicode_2_utf8(sPlayListName).c_str());
		if(!rs.eof())
		{
			if(!PlayListInfo)
				PlayListInfo = spPlayListInfoT(new CPlayListInfo);
			if(!PlayListInfo)
				return false;
			PlayListInfo->SetLocalId(rs.getIntField(0));
			PlayListInfo->SetId(rs.getIntField(1));
			PlayListInfo->SetName(em_utility::mci::utf8_2_CString(rs.getStringField(2)));
			PlayListInfo->SetListType((CPlayListInfo::PlayListType)(rs.getIntField(3)));
			PlayListInfo->SetShow(rs.getIntField(4)==1 ? true :false);
			PlayListInfo->SetFlag(rs.getIntField(5));
		}
		sLog("[End]检查指定<list_name:%s>播放列表<table:%s>----数据库",mci::unicode_2_ansi(sPlayListName.GetBuffer()).c_str(),PlayListTableName);
		sPlayListName.ReleaseBuffer();
		return true;
	}
	bool CUserDBase::AddDefaultPlayList()
	{
		sLog("[Begin]新建试听列表----数据库");
		if(IsPlayListExists(_T("试听列表")))
			return true;
		spPlayListInfoT spListInfo = spPlayListInfoT(new CPlayListInfo);
		assert(spListInfo);
		if(!spListInfo) return false;
		spListInfo->SetListType(CPlayListInfo::plt_sys);
		spListInfo->SetShow(true);
		spListInfo->SetComment(_T("试听列表"));
		spListInfo->SetName(_T("试听列表"));

		bool bRet = AddPlayList(spListInfo);
		if(!bRet)
			return	assert(false),sLogError("[End]新建试听列表----数据库"), false;

		sLog("[End]新建试听列表----数据库");
		return bRet;
	}
	/*得到播放列表ID对应的歌曲列表*/
	bool CUserDBase::GetPlayListSongs(std::vector<spSongInfoT> &SongList,unsigned int nPlayListID /* = 0 */ )
	{
		char szBuf[MAX_PATH] = {0};
		sprintf_s(szBuf,MAX_PATH,"%s_%d",SongListTableName,nPlayListID);
		sLog("[Begin]获取列表内容<table:%s>----数据库",szBuf);

		static const char sSql[] = "select song_local_id,song_id,song_name,artist_id,artist_name,album_id,album_name,"
			"cover_url,cover_path,listen_url,audio_length,is_disable,lrc_url,audio_type,audio_size,audio_start_pos,"
			"md5,local_path,is_local,gmt_last_play,gmt_last_modify,gmt_add_time,play_count "
			" from %Q;";
		
		SQLResult rs = this->QuerySql(sSql,szBuf);
		while(!rs.eof())
		{
			spSongInfoT spInfo(new CSongInfo);
			if(!spInfo)
				return false;

			spInfo->SetLocalId(rs.getIntField(0));
			spInfo->SetSongId(rs.getIntField(1));
			spInfo->SetSongName(em_utility::mci::utf8_2_CString(rs.getStringField(2)));
			spInfo->SetArtistId(rs.getIntField(3));
			spInfo->SetArtistName(em_utility::mci::utf8_2_CString(rs.getStringField(4)));
			spInfo->SetAlbumId(rs.getIntField(5));
			spInfo->SetAblumName(em_utility::mci::utf8_2_CString(rs.getStringField(6)));
			spInfo->SetCoverUrl(em_utility::mci::utf8_2_CString(rs.getStringField(7)));
			spInfo->SetCoverLocalPath(em_utility::mci::utf8_2_CString(rs.getStringField(8)));
			spInfo->SetListenFileUrl(em_utility::mci::utf8_2_CString(rs.getStringField(9)));
			spInfo->SetAudioLength(rs.getIntField(10));
			spInfo->SetEnable((rs.getIntField(11)==1));
			spInfo->SetLrcUrl(em_utility::mci::utf8_2_CString(rs.getStringField(12)));
			spInfo->SetFileType(rs.getIntField(13));
			spInfo->SetFileSize(rs.getIntField(14));
			spInfo->SetFileStartPos(rs.getIntField(15));
			spInfo->SetMD5(em_utility::mci::utf8_2_CString(rs.getStringField(16)));
			spInfo->SetLocalPath(em_utility::mci::utf8_2_CString(rs.getStringField(17)));
			spInfo->SetLocal((rs.getIntField(18)==1));
			spInfo->SetLastPlayTime((long)rs.getIntField(19));
			spInfo->SetLastModifyTime((long)rs.getIntField(20));
			spInfo->SetAddTime((long)rs.getIntField(21));
			spInfo->SetPlayCount(rs.getIntField(22));

			SongList.push_back(spInfo);
			rs.nextRow();
		}
		sLog("[End]获取列表内容<table:%s>----数据库",szBuf);
		return true;
	}
	/*添加歌曲到指定ID的播放列表*/
	bool CUserDBase::AddSongToPlayList(CString songPath,unsigned int nPlayListID,spSongInfoT& spSongInfo)
	{
		spSongInfo = thePlayListMgr->PraseFromPath(songPath);
		if (!spSongInfo)
			return false;

		assert(spSongInfo);
		
		char szBuf[MAX_PATH] = {0};
		sprintf_s(szBuf,MAX_PATH,"%s_%d",SongListTableName,nPlayListID);
		sLog("[Begin]新增歌曲<table:%s>----数据库",szBuf);

		static const char sSql[] = "insert into %Q(song_id,song_name,artist_id,artist_name,album_id,"
			"album_name,cover_url,cover_path,listen_url,audio_length,is_disable,lrc_url,audio_type,audio_size,"
			"audio_start_pos,md5,local_path,is_local,gmt_add_time,play_count)"
			"values(%u,%Q,%u,%Q,%u,%Q,%Q,%Q,%Q,%u,%u,%Q,%u,%u,%u,%Q,%Q,%u,%u,%u);";

		int bResult = (this->ExecSql(sSql,
			szBuf,
			spSongInfo->GetSongId(),
			mci::unicode_2_utf8(spSongInfo->GetSongName()).c_str(),
			spSongInfo->GetArtistId(),
			mci::unicode_2_utf8(spSongInfo->GetArtistName()).c_str(),
			spSongInfo->GetAlbumId(),
			mci::unicode_2_utf8(spSongInfo->GetAlbumName()).c_str(),
			mci::unicode_2_utf8(spSongInfo->GetCoverUrl()).c_str(),
			mci::unicode_2_utf8(spSongInfo->GetCoverLocalPath()).c_str(),
			mci::unicode_2_utf8(spSongInfo->GetListenFileUrl()).c_str(),
			spSongInfo->GetAudioLength(),
			(spSongInfo->IsEnable() ? 1 :0),
			mci::unicode_2_utf8(spSongInfo->GetLrcUrl()).c_str(),
			spSongInfo->GetFileType(),
			spSongInfo->GetFileSize(),
			spSongInfo->GetFileStartPos(),
			mci::unicode_2_utf8(spSongInfo->GetMD5()).c_str(),
			mci::unicode_2_utf8(spSongInfo->GetLocalPath()).c_str(),
			(spSongInfo->IsLocal() ? 1 : 0),
			(int)time(NULL),
			spSongInfo->GetPlayCount()));
		if (bResult)
		{
			SQLResult rs = this->QuerySql("select last_insert_rowid() from %Q",szBuf);
			if(rs.eof())
				return false;
			spSongInfo->SetLocalId(rs.getIntField(0));
		}
		sLog("[End]新增歌曲<table:%s>----数据库",szBuf);

		return true;
	}
	bool CUserDBase::AddSongsToPlayList(spSongInfoT spSongInfo, unsigned int nPlayListID/* = 0*/)
	{
		assert(spSongInfo);
		
		char szBuf[MAX_PATH] = {0};
		sprintf_s(szBuf,MAX_PATH,"%s_%d",SongListTableName,nPlayListID);
		sLog("[Begin]新增歌曲<table:%s>----数据库",szBuf);

		static const char sSql[] = "insert into %Q(song_id,song_name,artist_id,artist_name,album_id,"
			"album_name,cover_url,cover_path,listen_url,audio_length,is_disable,lrc_url,audio_type,audio_size,"
			"audio_start_pos,md5,local_path,is_local,gmt_add_time,play_count)"
			"values(%u,%Q,%u,%Q,%u,%Q,%Q,%Q,%Q,%u,%u,%Q,%u,%u,%u,%Q,%Q,%u,%u,%u);";

		int bResult = (this->ExecSql(sSql,
			szBuf,
			spSongInfo->GetSongId(),
			mci::unicode_2_utf8(spSongInfo->GetSongName()).c_str(),
			spSongInfo->GetArtistId(),
			mci::unicode_2_utf8(spSongInfo->GetArtistName()).c_str(),
			spSongInfo->GetAlbumId(),
			mci::unicode_2_utf8(spSongInfo->GetAlbumName()).c_str(),
			mci::unicode_2_utf8(spSongInfo->GetCoverUrl()).c_str(),
			mci::unicode_2_utf8(spSongInfo->GetCoverLocalPath()).c_str(),
			mci::unicode_2_utf8(spSongInfo->GetListenFileUrl()).c_str(),
			spSongInfo->GetAudioLength(),
			(spSongInfo->IsEnable() ? 1 :0),
			mci::unicode_2_utf8(spSongInfo->GetLrcUrl()).c_str(),
			spSongInfo->GetFileType(),
			spSongInfo->GetFileSize(),
			spSongInfo->GetFileStartPos(),
			mci::unicode_2_utf8(spSongInfo->GetMD5()).c_str(),
			mci::unicode_2_utf8(spSongInfo->GetLocalPath()).c_str(),
			(spSongInfo->IsLocal() ? 1 : 0),
			(int)time(NULL),
			spSongInfo->GetPlayCount()));
		if (bResult)
		{
			SQLResult rs = this->QuerySql("select last_insert_rowid() from %Q",szBuf);
			if(rs.eof())
				return false;
			spSongInfo->SetLocalId(rs.getIntField(0));
		}
		sLog("[End]新增歌曲<table:%s>----数据库",szBuf);

		return true;
	}
	bool CUserDBase::AddSongsToPlayList(std::vector<spSongInfoT> &SongList, unsigned int nPlayListID /*= 0*/)
	{
		if(!this->BeginTransaction())
			return assert(false),false;
		for (auto it=SongList.begin();it!=SongList.end();++it)
		{
			this->AddSongsToPlayList(*it,nPlayListID);
		}
		if(!this->Commit())
			return assert(false),false;
		return true;
	}
	/*更新指定播放列表ID的歌曲*/
	bool CUserDBase::UpdatePlayListSongInfo(spSongInfoT spSongInfo, unsigned int nPlayListID/* = 0*/)
	{   
		assert(spSongInfo);
		
		char szBuf[MAX_PATH] = {0};
		sprintf_s(szBuf,MAX_PATH,"%s_%d",SongListTableName,nPlayListID);
		sLog("[Begin]更新歌曲<table:%s>----数据库",szBuf);

		static const char sSql[] ="update %Q set song_id = %u, song_name = %Q,"
			"artist_id = %u,artist_name = %Q,album_id = %u, album_name = %Q,"
			"cover_url = %Q,cover_path = %Q,listen_url = %Q,audio_length = %u,"
			"is_disable = %u,lrc_url = %Q,audio_type = %u,audio_size = %u,"
			"audio_start_pos = %u,md5 = %Q,local_path = %Q,is_local = %u,"
			"gmt_last_play = %u,gmt_last_modify = %u,play_count = %u where song_local_id = %u;";
		int nErrCode = this->ExecSql(
			sSql,
			szBuf,
			spSongInfo->GetSongId(),
			mci::unicode_2_utf8(spSongInfo->GetSongName()).c_str(),			
			spSongInfo->GetArtistId(),
			mci::unicode_2_utf8(spSongInfo->GetArtistName()).c_str(),
			spSongInfo->GetAlbumId(),
			mci::unicode_2_utf8(spSongInfo->GetAlbumName()).c_str(),
			mci::unicode_2_utf8(spSongInfo->GetCoverUrl()).c_str(),
			mci::unicode_2_utf8(spSongInfo->GetCoverLocalPath()).c_str(),
			mci::unicode_2_utf8(spSongInfo->GetListenFileUrl()).c_str(),
			spSongInfo->GetAudioLength(),
			spSongInfo->IsEnable() ? 0 : 1,
			mci::unicode_2_utf8(spSongInfo->GetLrcUrl()).c_str(),
			spSongInfo->GetFileType(),
			spSongInfo->GetFileSize(),
			spSongInfo->GetFileStartPos(),	
			mci::unicode_2_utf8(spSongInfo->GetMD5()).c_str(),
			mci::unicode_2_utf8(spSongInfo->GetLocalPath()).c_str(),
			spSongInfo->IsLocal() ? 1 : 0,
			spSongInfo->GetLastPlayTime(),
			(int)time(NULL),			
			spSongInfo->GetPlayCount(),
			spSongInfo->GetLocalId()
			);
		sLog("[End]更新歌曲<table:%s>----数据库",szBuf);
		return SQLERROR != nErrCode;
	}
	bool CUserDBase::UpdatePlayListSongInfo(std::vector<spSongInfoT> SongArray,unsigned int nPlayListID /*= 0*/)
	{
		if(!this->BeginTransaction())
			return assert(false),false;
		for (auto it=SongArray.begin();it!=SongArray.end();++it)
		{
			this->UpdatePlayListSongInfo(*it,nPlayListID);
		}
		if(!this->Commit())
			return assert(false),false;
		return true;
	}
	/*更新歌曲路径*/
	bool CUserDBase::UpdatePlayListSongPath(spSongInfoT spSongInfo, unsigned int nPlayListID/* = 0*/)
	{
		assert(spSongInfo);
		if(!spSongInfo) return false;
		static const char sSql[] = "update %Q set local_path = %Q where song_local_id = %u;";
		char szBuf[MAX_PATH] = {0};
		sprintf_s(szBuf,MAX_PATH,"%s_%d",SongListTableName,nPlayListID);
		return SQLERROR != this->ExecSql(sSql,
			szBuf,
			mci::unicode_2_utf8(spSongInfo->GetLocalPath()).c_str(),
			spSongInfo->GetLocalId()
		) ;
	}
	/*移除指定播放列表ID的歌曲*/
	bool CUserDBase::RemovePlayListSongs(spSongInfoT spSongInfo, unsigned int nPlayListID/* = 0*/)
	{
		assert(spSongInfo);
		if(!spSongInfo) return false;
		static const char sSql[] = "delete from %Q where song_local_id = %u;";
		char szBuf[MAX_PATH] = {0};
		sprintf_s(szBuf,MAX_PATH,"%s_%d",SongListTableName,nPlayListID);

		sLog("[Begin]移除歌曲<table:%s>----数据库",szBuf);

		bool bRet = (this->ExecSql(
			sSql,
			szBuf,
			spSongInfo->GetLocalId()
			) != SQLERROR);

		sLog("[End]移除歌曲<table:%s>----数据库",szBuf);

		return bRet;
	}
	bool CUserDBase::RemovePlayListSongs(std::vector<spSongInfoT> &SongList, unsigned int nPlayListID/* = 0*/)
	{
		if(!this->BeginTransaction())
			return assert(false),false;

		for (std::vector<spSongInfoT>::iterator iter=SongList.begin();iter!=SongList.end();iter++)
		{
			this->RemovePlayListSongs(*iter,nPlayListID);
		}

		return this->Commit();
	}
	bool CUserDBase::RemoveAllPlayListSongs( unsigned int nPlayListID/* = 0*/)
	{
		char szBuf[MAX_PATH] = {0};
		sprintf_s(szBuf,MAX_PATH,"%s_%d",SongListTableName,nPlayListID);
		static const char sSql[] = "delete from %Q;";
	
		return SQLERROR != this->ExecSql(sSql,szBuf);
	}
	bool CUserDBase::DeletePlayList(unsigned int nPlayListID /* = 0 */)
	{
		char szBuf[MAX_PATH] = {0};
		sprintf_s(szBuf,MAX_PATH,"%s_%d",SongListTableName,nPlayListID);
		
		sLog("[Begin]删除播放列表<table:%s>----数据库",szBuf);

		static const char sSql[] = "drop table %Q;";

		int nErrCode = this->ExecSql(sSql,szBuf);

		sLog("[End]删除播放列表<table:%s>----数据库",szBuf);

		return SQLERROR != nErrCode;
	}
	bool CUserDBase::CreateFavTable()
	{
		sLog("[Begin]创建收藏列表<table:%s>----数据库",FavoritesTableName);
		static const char sSql[] = "create table %Q (favorites_id Integer primary key autoincrement,"
			"favorites_name text ,type int,gmt_create int,playlist_id int,song_id int);";
		try
		{
			if(!this->IsExistTable(FavoritesTableName))
				this->ExecSql(sSql,FavoritesTableName);
		}
		catch(CppSQLite3Exception e)
		{
			sLogError("[End]创建收藏列表<table:%s>----数据库",FavoritesTableName);
			assert(false);
			return SQLERROR;
		}
		catch(...)
		{
			sLogError("[End]创建收藏列表<table:%s>----数据库",FavoritesTableName);
			assert(false);
			return SQLERROR;
		}
		sLog("[End]创建收藏列表<table:%s>----数据库",FavoritesTableName);

		return true;
	}
	bool CUserDBase::AddDefaultFav()
	{
		sLog("[Begin]新建我的收藏列表<table:%s>----数据库",FavoritesTableName);
		if(IsPlayListExists(_T("我的收藏")))
			return true;
		spPlayListInfoT spListInfo = spPlayListInfoT(new CPlayListInfo);
		assert(spListInfo);
		if(!spListInfo) return false;
		spListInfo->SetListType(CPlayListInfo::plt_sys);
		spListInfo->SetShow(true);
		spListInfo->SetComment(_T("我的收藏"));
		spListInfo->SetName(_T("我的收藏"));

		bool bRet = AddPlayList(spListInfo);
		if(!bRet)
			return	assert(false),sLogError("[End]新建我的收藏列表<table:%s>----数据库",FavoritesTableName), false;

		sLog("[End]新建我的收藏列表<table:%s>----数据库",FavoritesTableName);
		return bRet;
	}
	bool CUserDBase::AddFavorites(spFavInfoT spFavInfo)
	{
		sLog("[Begin]添加收藏歌曲<table:%s>----数据库",FavoritesTableName);
		static const char sSql[] = "replace into  %Q (favorites_id,favorites_name,type,gmt_create) values (%u,%Q,%u,%u);";

		std::string sNameUTF8 = em_utility::mci::unicode_2_utf8(spFavInfo->GetFavName());

		int nErrCode = this->ExecSql(sSql,FavoritesTableName,
			spFavInfo->GetFavId(),
			sNameUTF8.c_str(),
			(int)(spFavInfo->GetType()),
			(int) time(NULL));

		sLog("[End]新建我的收藏列表<table:%s>----数据库",FavoritesTableName);

		return SQLERROR != nErrCode;
	}
	bool CUserDBase::AddFavorites(std::vector<spFavInfoT> &FavList)
	{
		if(!this->BeginTransaction())
			return assert(false),false;

		for (auto iter=FavList.begin();iter!=FavList.end();iter++)
		{
			this->AddFavorites(*iter);
		}

		return this->Commit();
	}
	bool CUserDBase::RemoveFavorites(unsigned int nId,unsigned int nType)
	{
		static const char sSql[] = "delete from %Q where favorites_id = %u and type = %u;";

		return SQLERROR != this->ExecSql(sSql,FavoritesTableName,nId,nType);
	}
	bool CUserDBase::RemoveFavorites(std::vector<spFavInfoT> &FavList)
	{
		if(!this->BeginTransaction())
			return assert(false),false;

		for (auto iter=FavList.begin();iter!=FavList.end();iter++)
		{
			this->RemoveFavorites((*iter)->GetFavId(),(*iter)->GetType());
		}

		return this->Commit();
	}
	bool CUserDBase::RemoveAllFavorites()
	{
		static const char sSql[] = "delete from %Q ;";
		return SQLERROR != this->ExecSql(sSql,FavoritesTableName);
	}
	bool CUserDBase::GetAllFavorites(std::vector<spFavInfoT> &FavList)
	{
		static const char sSql[] = "select favorites_id,favorites_name,type from %Q ;";

		SQLResult rs = this->QuerySql(sSql,FavoritesTableName);
		while(!rs.eof())
		{
			spFavInfoT spInfo(new CFavInfo);
			if(spInfo)
			{
				spInfo->SetFavId(rs.getIntField(0));
				spInfo->SetFavName(em_utility::mci::utf8_2_CString(rs.getStringField(1)));
				spInfo->SetType((CFavInfo::FavType)(rs.getIntField(2)));
				FavList.push_back(spInfo);
			}			
			rs.nextRow();
		}

		return true;
	}

	bool CUserDBase::AddPlayDetailRecord(unsigned int nSongId,unsigned int nSecond,bool bSkip)
	{
		static const char sSql[] = "insert into %Q (song_id,play_second,gmt_play_time,type) values (%u,%u,%u,%u);";

		bool bResult = (SQLERROR != this->ExecSql(sSql,DetailPlayRecordTableName,
			nSongId,nSecond,(int)time(NULL),
			bSkip ? 1 : 0));

		if (bResult)
		{
			//对record表的修改;
		}

		return bResult;
	}

	bool CUserDBase::GetRecentFavRecord(std::vector<spFavInfoT> &FavList)
	{
		static const char sSql[] = "select favorites_id,favorites_name,type from %Q order by gmt_create desc limit 0,50;";

		SQLResult rs = this->QuerySql(sSql,FavoritesTableName);
		while (!rs.eof())
		{
			spFavInfoT spInfo(new CFavInfo);
			if(spInfo)
			{
				spInfo->SetFavId(rs.getIntField(0));
				spInfo->SetFavName(em_utility::mci::utf8_2_CString(rs.getStringField(1)));
				spInfo->SetType((CFavInfo::FavType)(rs.getIntField(2)));
				FavList.push_back(spInfo);
			}			
			rs.nextRow();
		}
		return true;
	}

	bool CUserDBase::GetPlayRecord(std::vector<std::pair<int,int> >& DataArray,bool bSkipRecord)
	{
		static const char sSql[] = "select song_id,count(song_id) as play_count from %Q where type = %u group by song_id ;";
		
		SQLResult rs = this->QuerySql(sSql,DetailPlayRecordTableName,bSkipRecord ? 1 :0);
		while(!rs.eof())
		{
			DataArray.push_back(std::pair<int,int>(rs.getIntField(0),rs.getIntField(1)));
			rs.nextRow();
		}

		return true;

	}

	bool CUserDBase::GetLocalMusicPacket(unsigned int nPlayListLocalId,std::vector<std::pair<UINT,UINT> >& PacketArray,UINT nStartPos,UINT nLimit,bool bSong)
	{
		static const char sSqlSong[] = "select packet_id,type from %Q where play_list_local_id = %u and type=4 order by gmt_create desc limit %u ,%u;";
		static const char sSqlNotSong[] = "select packet_id,type from %Q where play_list_local_id = %u and type in (1,2,3) order by gmt_create desc limit %u ,%u;";

		SQLResult rs = this->QuerySql(bSong ? sSqlSong : sSqlNotSong,LocalMusicPacketTableName,nPlayListLocalId,nStartPos,nLimit);

		while(!rs.eof())
		{
			PacketArray.push_back(std::pair<int,int>(rs.getIntField(0),rs.getIntField(1)));
			rs.nextRow();
		}
		return true;
	}

	bool CUserDBase::AddMusicPacket(unsigned int nPlayListLocalId,const std::pair<UINT,UINT>& Packet)
	{
		static const char sSql[] = "replace into %Q (play_list_local_id,packet_id,type,gmt_create) values (%u,%u,%u,%u);";

		bool bResult = (SQLERROR != this->ExecSql(sSql,LocalMusicPacketTableName,
			nPlayListLocalId,Packet.first,Packet.second,(int)time(NULL)));

		return bResult;

	}

	bool CUserDBase::AddMusicPacket(unsigned int nPlayListLocalId,const std::vector<std::pair<UINT,UINT> >& PacketArray)
	{
		if(!this->BeginTransaction())
			return assert(false),false;

		for (auto iter=PacketArray.begin();iter!=PacketArray.end();iter++)
		{
			this->AddMusicPacket(nPlayListLocalId,*iter);
		}

		return this->Commit();
	}

	bool CUserDBase::RemoveMusicPacket(unsigned int nPlayListLocalId,const std::pair<UINT,UINT>& Packet)
	{
		static const char sSql[] = "delete from %Q where play_list_local_id = %u and packet_id = %u and type = %u;";

		return SQLERROR != this->ExecSql(sSql,LocalMusicPacketTableName,nPlayListLocalId,Packet.first,Packet.second);
	}

	UINT CUserDBase::GetMusicPacketCount(unsigned int nPlayListLocalId,bool bSong)
	{
		UINT nCount = 0;
		static const char sSqlSong[] = "select count(*) from %Q where play_list_local_id = %u and type = 4;";
		static const char sSqlPacket[] = "select count(*) from %Q where play_list_local_id = %u and type in (1,2,3);";
		SQLResult rs = this->QuerySql(bSong ? sSqlSong : sSqlPacket,LocalMusicPacketTableName,nPlayListLocalId);
		if (!rs.eof())
		{
			nCount = rs.getIntField(0);
		}
		return nCount;
	}
	//Operation表操作;
	bool CUserDBase::CreateOperationTable()
	{
		sLog("[Begin]创建操作列表(%s)----数据库",OperationTableName);
		static const char sSql[] = "create table %Q (auto_id Integer primary key autoincrement,"
			"object_id int,object_type int,operate_type int,gmt_create int,flag int);";
		try
		{
			if(!this->IsExistTable(OperationTableName))
				this->ExecSql(sSql,OperationTableName);
		}
		catch(CppSQLite3Exception e)
		{
			assert(false);
			return SQLERROR;
		}
		catch(...)
		{
			assert(false);
			return SQLERROR;
		}
		sLog("[End]创建操作列表(%s)----数据库",OperationTableName);

		return true;
	}
	bool CUserDBase::GetAllOperation(std::vector<spSynOperationT>& OperationArray)
	{
		OperationArray.clear();
		static const char sSql[] = "select * from %Q";

		SQLResult rs = this->QuerySql(sSql,OperationTableName);

		while(!rs.eof())
		{
			spSynOperationT spSynOperation(new CSynOperation);
			spSynOperation->SetAutoID(rs.getIntField(0));
			spSynOperation->SetObjectID(rs.getIntField(1));
			spSynOperation->SetObjectType((CSynOperation::ObjectType)rs.getIntField(2));
			spSynOperation->SetOperateType((CSynOperation::OperateType)rs.getIntField(3));
			spSynOperation->SetFlag(rs.getIntField(4));

			OperationArray.push_back(spSynOperation);
			rs.nextRow();
		}
		return true;

	}

	bool CUserDBase::AddOperation(spSynOperationT spSynOperation)
	{
		static const char sSql[] = "insert into %Q (object_id,object_type,operate_type,gmt_create,flag) values (%u,%u,%u,%u,%u);";

		bool bResult = (SQLERROR != this->ExecSql(sSql,OperationTableName,spSynOperation->GetObjectID(),
			spSynOperation->GetObjectType(),spSynOperation->GetOperateType(),(int)time(NULL),spSynOperation->GetFlag()));

		if (bResult)
		{
			SQLResult rs = this->QuerySql("select last_insert_rowid() from %Q",OperationTableName);
			if(rs.eof())
				return false;
			spSynOperation->SetAutoID(rs.getIntField(0));
		}

		return bResult;
	}

	bool CUserDBase::DelOperation(spSynOperationT spSynOperation)
	{
		static const char sSql[] = "delete from %Q where auto_id = %u;";
		return SQLERROR != this->ExecSql(sSql,OperationTableName,spSynOperation->GetAutoID());
	}

	bool CUserDBase::OnCreateTables()
	{
		bool bRet1 = CreateTablePlayLists();//创建播放列表
		assert(bRet1);
		bool bRet2 = CreateFavTable();//创建收藏列表
		assert(bRet2);
		bool bRet3 = CreateOperationTable();//创建操作列表
		assert(bRet3);
		return bRet1&&bRet2&&bRet3;
	}
	bool CUserDBase::OnUpdateTables(int oldVer,int newVer)
	{
		return true;
	}
