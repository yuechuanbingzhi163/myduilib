
/***********************************************************
 *	FileName£ºDatabase.h
 *  Author  £ºDFn Li
 *  Timer   : 2013/3/1 18:10
 *  E-Mail  : libyyu@qq.com
 *  Comment : the Database operation object encapsulation
 *  Version : 1.0.0.1
 ***********************************************************/
#ifndef __DATABASE_H
#define __DATABASE_H

#pragma once
#include "CppSQLite3.h"


	class CDatabase
	{
	public:
		static const int  SQLERROR = -1;
		static const int  DBVersion = 0;
		static const int  BackupDBRate = 60*60*24;
		static const char DicTableName[]; 
		static const std::string DBVersionKey;
		static const std::string LastBackupKey;
		typedef CppSQLite3Query     SQLResult;
		typedef CppSQLite3Buffer    SQLBuffer;
	public:
		CDatabase():_pDB(NULL){}
		virtual ~CDatabase(){CloseDB();}

		bool OpenDB(std::string sFileName);
		bool CloseDB();
		bool BackupDB(std::string sFileName);
		bool IsOpen() const {return !!_pDB;}
		int GetDBVersion();

		bool GetDictValue(std::string sKey, std::string &sValue); //if sKey is exsited return true,if not exsit return false
		bool GetDictValue(std::string sKey,int& nValue);
		bool GetDictValue(std::string sKey,DWORD& dwValue);
		bool GetDictValue(std::string sKey,bool& nValue);
		bool UpdateDictValue(std::string sKey, std::string sValue); //if Key is exsited update it and  return true,if not exsit return false
		bool UpdateDictValue(std::string sKey,int nValue);
		bool UpdateDictValue(std::string sKey,DWORD dwValue);
		bool UpdateDictValue(std::string sKey,bool nValue);
		bool InsertDict(std::string sKey, std::string sValue);//if Key is exsited return false ,if not exsit insert new one and return true
		bool InsertDict(std::string sKey,int nValue);  
		bool InsertDict(std::string sKey,DWORD dwValue);
		bool InsertDict(std::string sKey, bool nValue);
		bool DeleteDict(std::string sKey);

		bool AlterTableName(const char* sOld,const char* sNew);
	protected:
		int ExecScalar(const char* fmt,...);
		int ExecSql(const char* fmt,...);
		SQLResult QuerySql(const char* fmt,...);
	public:
		bool IsExistTable(const char* tablename);
		bool DeleteTable(const char* tablename);
		bool EmptyTable(const char* tablename);
		bool BeginTransaction();
		bool Commit();
		bool RollBack();
	protected:
		virtual bool OnCreateTables();
		virtual bool OnUpdateTables(int oldVer,int newVer);
	private:
		CppSQLite3DB * _pDB;
	};
/*
#define  BUF_SIZE  100
	typedef struct _tagUser
	{
		char _name[BUF_SIZE];
		char _passwd[BUF_SIZE];
		_tagUser()
		{
			memset(_name,0,BUF_SIZE);
			memset(_passwd,0,BUF_SIZE);
		}
		_tagUser(const char* name,const char* passwd)
		{
			memset(_name,0,BUF_SIZE);
			memset(_passwd,0,BUF_SIZE);
			memcpy(_name,name,strlen(name));
			memcpy(_passwd,passwd,strlen(passwd));
		}
		_tagUser & operator = (const _tagUser other)
		{
			memset(_name,0,BUF_SIZE);
			memset(_passwd,0,BUF_SIZE);
			memcpy(_name,other._name,strlen(other._name));
			memcpy(_passwd,other._passwd,strlen(other._passwd));
		}
	}USERINFO,*LPUSERINFO;
*/
	struct User;
	class  CUserMgr : public CDatabase
	{
	public:
		CUserMgr():CDatabase(){}
		static const char UserCacheTableName[];
	public:
		bool GetAllUsersToList(std::vector<User> &UserList);
		bool GetOtherUsersToList(std::vector<User> &UserList,User UserInfo);
		bool GetUserInfo(const char* userName,User &UserInfo);
		bool InsertUser(User UserInfo);
//		bool UpdateUser(User UserInfo,const int iKey);
		bool UpdateUser(User UserInfo,const char* userName);
		bool RemoveUser(const int iKey);
		bool RemoveUser(const char* userName);
		bool IsUserExist(User UserInfo);
		bool IsNameUsed(const char* userName);

		bool CreateUserTable();

		int  GetUserCount();
		int  GetMaxUserID();
	protected:
		virtual bool OnCreateTables();
		virtual bool OnUpdateTables(int oldVer,int newVer);
	};

	extern CUserMgr* theDB;

#endif//__DATABASE_H