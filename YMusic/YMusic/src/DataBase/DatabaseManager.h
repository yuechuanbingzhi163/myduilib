#pragma once
#include "Database.h"
#include <vector>

#include "../../YMusic.h"

class CDatabaseManager
	{
		static const TCHAR * USER_DB; 
		static const TCHAR * DEFAULT_DB; 
		static const TCHAR * GLOBEL_DB; 
	public:
		CDatabaseManager();
		virtual ~CDatabaseManager();
	public:
		void SetDatabasePath(CString &sDatabasePath);
		CString &GetDatabasePath();

		bool SetCurrentUser(spLogonUserInfoT spUserInfo);

		spGlobalDBaseT GetGlobalDBase();
		spUserDBaseT GetUserDBase();
		spUserDBaseT GetDefaultUserDBase();
		spUserDBaseT GetUserDB();
	private:

		spLogonUserInfoT m_spUserInfo; 
		CString m_sDatabasePath;

		spGlobalDBaseT      m_spGlobalDBase;
		spUserDBaseT   m_spUserDBase;
		spUserDBaseT   m_spDefaultUserDBase;

	};
	
