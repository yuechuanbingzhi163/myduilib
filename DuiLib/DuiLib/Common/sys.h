#pragma once

namespace DuiLib
{
	class UILIB_API CSys
	{
	public:
		static DWORD GetDllVersion(LPCTSTR lpszDllName);
		static BOOL  IsWindow7();
		static BOOL  IsVista();
		static BOOL  IsAdmin();

		static WORD HKF2SDK(WORD mod);
		static WORD SDK2HKF(WORD mod);
		static BOOL OpenUrl(LPCTSTR lpUrl);
		static BOOL ReStartApp(LPCTSTR lpCmd,BOOL isAdmin = FALSE);
	};
}