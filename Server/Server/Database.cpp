#include "stdafx.h"
#include "DataBase.h"
#include <time.h>
#include "Msg.h"
#pragma comment(lib,"sqlite3cpp.lib")
const int LAST_ID = 0;


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	const char CDatabase::DicTableName[] = "dictionary";
	const std::string CDatabase::DBVersionKey = "DBVersion";
	const std::string CDatabase::LastBackupKey = "_LastBackup";
	//打开数据库文件
	bool CDatabase::OpenDB(std::string sFileName)
	{
		//先关闭连接
		CloseDB();
		_pDB = new CppSQLite3DB();
		try 
		{
			_pDB->open(sFileName.c_str());
		}
		catch(CppSQLite3Exception e)
		{
			return false;
		}
		catch(...)
		{
			return false;
		}
		//检测是否成功打开
		if(!IsOpen())
			return false;

		try
		{
			//词典是否存在
			if(!IsExistTable(DicTableName))
			{//不存在词典，则新建词典
				static const char sSql[] = "CREATE TABLE %Q (key text primary key,value text,comment text)";
				if(SQLERROR == this->ExecSql(sSql,DicTableName))
					goto _OnError;
			}
			if(!OnCreateTables()) 
				goto _OnError;
			//取得数据库文件版本
			const int OldDBVersion = GetDBVersion();
			if(OldDBVersion > DBVersion)
			{
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
		try
		{
			if(IsOpen())
			{
				_pDB->close();
				delete _pDB;
				_pDB = NULL;
			}
			return true;
		}
		catch(...)
		{
			return false;
		}		
	}

	bool CDatabase::BackupDB(std::string sFileName)
	{
		std::string newFileName(sFileName);
		newFileName += ".bak";
		if(::CopyFile(CString(sFileName.c_str()),CString(newFileName.c_str()),FALSE))
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

	bool CDatabase::GetDictValue(std::string sKey, std::string &sValue)
	{
		static const char sSql[] = "select value from %Q where key = %Q;";

		std::string sKeyUTF8 = sKey;

		SQLResult rs = this->QuerySql(sSql,DicTableName,sKeyUTF8.c_str());

		if(rs.eof()) return false;

		sValue = std::string(rs.getStringField(0));
		return true;
	}

	bool CDatabase::GetDictValue(std::string sKey,int& nValue)
	{
		std::string sValue;
		if(!GetDictValue(sKey,sValue) || sValue.empty())
			return false;
		nValue = atoi(sValue.c_str());
		return true;
	}
	bool CDatabase::GetDictValue(std::string sKey,DWORD& dwValue)
	{
		std::string sValue;
		if(!GetDictValue(sKey,sValue) || sValue.empty())
			return false;
		dwValue =  atol(sValue.c_str());
		return true;
	}
	bool CDatabase::GetDictValue(std::string sKey,bool& nValue)
	{
		int nV = 0;
		if(!GetDictValue(sKey,nV))
			return false;
		nValue = (bool)nV;
		return true;
	}

	bool CDatabase::UpdateDictValue(std::string sKey, std::string sValue)
	{
		static const char sSql[] = "update %Q set value = %Q where key =%Q;";

		std::string sKeyUTF8   = sKey;
		std::string sValueUTF8 = sValue;

		return SQLERROR != this->ExecSql(sSql,DicTableName,sValueUTF8.c_str(),sKeyUTF8.c_str());
	}

	bool CDatabase::UpdateDictValue(std::string sKey,int nValue)
	{
// 		CString sValue;
// 		sValue.Format(_T("%d"),nValue);
		char buf[100]={0};
		_itoa_s(nValue,buf,100,10);
		std::string sValue(buf);
		return UpdateDictValue(sKey,sValue);
	}
	bool CDatabase::UpdateDictValue(std::string sKey,DWORD dwValue)
	{
		return UpdateDictValue(sKey,(int)dwValue);
	}
	bool CDatabase::UpdateDictValue(std::string sKey,bool nValue)
	{
		return UpdateDictValue(sKey,(int)nValue);
	}

	bool CDatabase::InsertDict(std::string sKey, std::string sValue)
	{
		static const char sSql[] = "insert into %Q(key,value) values (%Q,%Q);";

		std::string sKeyUTF8 = sKey;
		std::string sValueUTF8 = sValue;

		return SQLERROR != this->ExecSql(sSql,DicTableName,sKeyUTF8.c_str(),sValueUTF8.c_str());
	}

	bool CDatabase::InsertDict(std::string sKey,int nValue)
	{
		char buf[100]={0};
//		itoa(nValue,buf,10);
//		_itoa(nValue,buf,10);
		_itoa_s(nValue,buf,100,10);
		std::string sValue = std::string(buf);
		return InsertDict(sKey,sValue);
	}
	bool CDatabase::InsertDict(std::string sKey,DWORD dwValue)
	{
		return InsertDict(sKey,(int)dwValue);
	}
	bool CDatabase::InsertDict(std::string sKey,bool nValue)
	{
		return InsertDict(sKey,(int)nValue);
	}

	bool CDatabase::DeleteDict(std::string sKey)
	{
		static const char sSql[] = "delete from  %Q where key = %Q;";

		std::string sKeyUTF8 = sKey;

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
			assert(false);
			return SQLERROR;
		}
		catch (...)
		{
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
		}
		catch (CppSQLite3Exception e)
		{
			throw e;
			assert(false);
			return SQLERROR;
		}
		catch (...)
		{
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
		}
		catch (CppSQLite3Exception e)
		{
			assert(false);
			return SQLResult();
		}
		catch (...)
		{
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
		return SQLERROR != ExecSql("begin transaction;");
	}
	bool CDatabase::Commit()
	{
		return SQLERROR != ExecSql("commit transaction;");
	}

	bool CDatabase::RollBack()
	{
		return SQLERROR != ExecSql("rollback transaction");
	}
	bool CDatabase::EmptyTable(const char* tablename)
	{
		return SQLERROR!=ExecSql("delete from %Q;",tablename);
	}
// 	bool CDatabase::CreateTable()
// 	{
// 		return false;
// 	}
	bool CDatabase::OnCreateTables()
	{
		return false;
	}
	bool CDatabase::OnUpdateTables(int oldVer,int newVer)
	{
		return false;
	}
	bool CDatabase::AlterTableName(const char* sOld,const char* sNew)
	{
		ASSERT(sOld);
		ASSERT(sNew);
		ASSERT(IsExistTable(sOld));
		static const char sSql[] = "ALTER TABLE %Q RENAME TO %Q;";

		return this->ExecSql(sSql,sOld,sNew);
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	const char CUserMgr::UserCacheTableName[] = "userlist";
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	bool CUserMgr::GetAllUsersToList(std::vector<User> &UserList)
	{
		if(!this->IsExistTable(UserCacheTableName))
			return false;
		static const char sSql[] = "select user_name,user_passwd  from %Q;";

		SQLResult rs = this->QuerySql(sSql,UserCacheTableName);

		while(!rs.eof())
		{
			User UserInfo;
//			memcpy(UserInfo._name,rs.getStringField(0),strlen(rs.getStringField(0)));
//			memcpy(UserInfo._passwd,rs.getStringField(1),strlen(rs.getStringField(1)));
			UserInfo.m_loadName = mci::ansi_2_CString(rs.getStringField(0));
			UserInfo.m_password = mci::ansi_2_CString(rs.getStringField(1));
			UserList.push_back(UserInfo);

			rs.nextRow();
		}
		return true;
	}
	bool CUserMgr::GetOtherUsersToList(std::vector<User> &UserList,User UserInfo)
	{
		if(!this->IsExistTable(UserCacheTableName))
			return false;
		static const char sSql[] = "select user_name,user_passwd  from %Q where user_name != %Q;";
		std::string sname = mci::unicode_2_ansi(UserInfo.m_loadName);
		SQLResult rs = this->QuerySql(sSql,UserCacheTableName,sname.c_str());

		while(!rs.eof())
		{
			User UserInfo;
			//memcpy(UserInfo._name,rs.getStringField(0),strlen(rs.getStringField(0)));
			//memcpy(UserInfo._passwd,rs.getStringField(1),strlen(rs.getStringField(1)));
			UserInfo.m_loadName = mci::ansi_2_CString(rs.getStringField(0));
			UserInfo.m_password = mci::ansi_2_CString(rs.getStringField(1));

			UserList.push_back(UserInfo);

			rs.nextRow();
		}
		return true;
	}

	bool CUserMgr::GetUserInfo(const char* userName,User &UserInfo)
	{
		if(!this->IsExistTable(UserCacheTableName))
			return false;
		static const char sSql[] = "select user_name,user_passwd  from %Q where user_name = %Q;";

		SQLResult rs = this->QuerySql(sSql,UserCacheTableName,userName);	
		if (!rs.eof())
		{
			UserInfo.m_loadName = mci::ansi_2_CString(rs.getStringField(0));
			UserInfo.m_password = mci::ansi_2_CString(rs.getStringField(1));


			return true;
		}
		return false;
	}
	bool CUserMgr::InsertUser(User UserInfo)
	{
		if(!this->IsExistTable(UserCacheTableName))
			return false;
		static const char sSql[] = "insert into %Q(user_name,user_passwd) values (%Q,%Q);";
		std::string sname = mci::unicode_2_ansi(UserInfo.m_loadName);
		std::string spasswd = mci::unicode_2_ansi(UserInfo.m_password);
		return SQLERROR != this->ExecSql(
			sSql,
			UserCacheTableName,
			sname.c_str(),
			spasswd.c_str()
			);
	}
// 	bool CUserMgr::UpdateUser(USERINFO UserInfo,const int iKey)
// 	{
// 		if(!this->IsExistTable(UserCacheTableName))
// 			return false;
// 		static const char sSql[] = "update %Q set user_name = %Q,user_passwd = %Q,user_state = %u where user_id = %u;";
// 
// 		 if(SQLERROR == this->ExecSql(
// 			sSql,
// 			UserCacheTableName,
// 			UserInfo._name.c_str(),
// 			UserInfo._passwd.c_str(),
// 			UserInfo._state,
// 			atoi(UserInfo._id.c_str())
// 			))
// 			return false;
// 
// 		return true;
// 	}
	bool CUserMgr::UpdateUser(User UserInfo,const char* userName)
	{
		if(!this->IsExistTable(UserCacheTableName))
			return false;
		static const char sSql[] = "update %Q set user_name = %Q,user_passwd = %Q where user_name = %Q;";
		std::string sname = mci::unicode_2_ansi(UserInfo.m_loadName);
		std::string spasswd = mci::unicode_2_ansi(UserInfo.m_password);
		if(SQLERROR == this->ExecSql(
			sSql,
			UserCacheTableName,
			sname.c_str(),
			spasswd.c_str(),
			sname.c_str()
			))
			return false;

		return true;
	}
	bool CUserMgr::RemoveUser(const int iKey)
	{
		if(!this->IsExistTable(UserCacheTableName))
			return false;
		static const char sSql[] = "delete from %Q where user_id = %u;";
		return SQLERROR != this->ExecSql(
			sSql,
			UserCacheTableName,
			iKey
			);
	}
	bool CUserMgr::RemoveUser(const char* userName)
	{
		if(!this->IsExistTable(UserCacheTableName))
			return false;
		static const char sSql[] = "delete from %Q where user_name = %Q;";

		return SQLERROR != this->ExecSql(
			sSql,
			UserCacheTableName,
			userName
			);
	}
	bool CUserMgr::IsUserExist(User UserInfo)
	{
		if(!this->IsExistTable(UserCacheTableName))
			return false;
		static const char sSql[] = "select count(*) from %Q where user_name = %Q and user_passwd = %Q;";
		std::string sname = mci::unicode_2_ansi(UserInfo.m_loadName);
		std::string spasswd = mci::unicode_2_ansi(UserInfo.m_password);
		return this->ExecScalar(sSql,UserCacheTableName,sname.c_str(),spasswd.c_str()) == 1;
	}
	
	bool CUserMgr::IsNameUsed(const char* userName)
	{
		if(!this->IsExistTable(UserCacheTableName))
			return false;
		static const char sSql[] = "select count(*) from %Q where user_name = %Q;";
		return this->ExecScalar(sSql,UserCacheTableName,userName) == 1;
	}
	bool CUserMgr::CreateUserTable()
	{
		static const char sSql[] = "create table %Q (user_id Integer primary key autoincrement,user_name text unique,user_passwd text );";
		try
		{
			if(!this->IsExistTable(UserCacheTableName))
				this->ExecSql(sSql,UserCacheTableName);
			return true;
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
		return true;
	}
	
	bool CUserMgr::OnCreateTables()
	{// 
		return CreateUserTable();
	}
	bool CUserMgr::OnUpdateTables(int oldVer,int newVer)
	{
		return true;
	}

	int CUserMgr::GetUserCount()
	{
		if(!this->IsExistTable(UserCacheTableName))
			return false;
		return ExecScalar("select count(*) from %Q;",UserCacheTableName);
	}
	
	int CUserMgr::GetMaxUserID()
	{
		if(!this->IsExistTable(UserCacheTableName))
			return -1;
		const char sSql[] = "select max(user_id) from %Q;";
		try
		{
		//	if (GetUserCount()>0)
			{
				SQLResult rs = this->QuerySql(sSql,UserCacheTableName);
				if (!rs.eof())
				{
					return rs.getIntField(0);
				}
			}
			
			return 0;
		}
		catch(CppSQLite3Exception e)
		{
			throw e;
			return SQLERROR;
		}
		catch(...)
		{
			assert(false);
			return SQLERROR;
		}
	}
	 CUserMgr* theDB = NULL;
	
