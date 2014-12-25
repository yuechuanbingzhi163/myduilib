#include "stdafx.h"
#include "FilePathMgr.h"
#include <Shlobj.h>
#include "../YApp.h"

CFilePathMgr::CFilePathMgr()
{
	m_sTemplateDatabasePath = GetAppPath();
	em_utility::mci::confirm_dir(m_sTemplateDatabasePath,_T('/'));
	m_sDatabasePath = GetDataPath()+ _T("db/");
	em_utility::mci::confirm_dir(m_sDatabasePath,_T('/'));	
	//m_sExpressionPath = GetAppPath()+ _T("gif/");
	//em_utility::misc::confirm_dir(m_sExpressionPath,_T('/'));
	m_sDefaultCachePath = GetDataPath()+_T("cache/");
	em_utility::mci::confirm_dir(m_sDefaultCachePath,_T('/'));
	m_sLogPath = GetDataPath() + _T("log/");
	em_utility::mci::confirm_dir(m_sLogPath,_T('/'));
}

CFilePathMgr::~CFilePathMgr()
{

}

CString CFilePathMgr::GetAppPath()
{
	CString sAppPath =em_utility::mci::GetModulePath();
	sAppPath.Replace(_T('\\'),_T('/'));
	if(sAppPath.GetAt(sAppPath.GetLength()-1) != _T('/'))
	{
		sAppPath += _T("/");
	}
	return sAppPath;
}

CString CFilePathMgr::GetDataPath()
{
	TCHAR Path[MAX_PATH] = {0};
	SHGetFolderPath(NULL, CSIDL_APPDATA,NULL,SHGFP_TYPE_CURRENT,Path);
	CString sDataPath(Path);
	sDataPath = sDataPath +_T("\\YTing");
	sDataPath.Replace(_T('\\'),_T('/'));
	if(sDataPath.GetAt(sDataPath.GetLength()-1) != _T('/'))
	{
		sDataPath += _T("/");
	}
	return sDataPath;
}


CString CFilePathMgr::LinkAvatarPath(unsigned int nUserID)
{
	CString sImgPath;
	sImgPath.Format(_T("avater_%u.jpg"),nUserID);
	sImgPath = GetImageCachePath() + sImgPath;
	return sImgPath;
}

CString CFilePathMgr::LinkAlbumCoverPath(unsigned int nSongID)
{
	CString sImgPath;
	sImgPath.Format(_T("AlbumCover_%u.jpg"),nSongID);
	sImgPath = GetImageCachePath() + sImgPath;
	return sImgPath;
}

CString CFilePathMgr::LinkListenPath(unsigned int nResID,CString sFileName)
{
	CString sFilePath;
	sFilePath.Format(_T("%u"),nResID);
	sFilePath = GetOnlineResourcePath() + sFilePath;
	return sFilePath;
}

/*void CFilePathMgr::SetDatabasePath(CString sPath)
{
m_sDatabasePath = sPath;
}*/

/*void CFilePathMgr::SetOnlineResourcePath(CString sPath)
{
m_sOnlineResourcePath = sPath;
}*/

/*void CFilePathMgr::SetImageCachePath(CString sPath)
{
m_sImageCachePath = sPath;
}*/

/*void CFilePathMgr::SetExpressionPath(CString sPath)
{
m_sExpressionPath = sPath;
}*/

CString CFilePathMgr::GetDatabasePath()
{
	return m_sDatabasePath;
}

CString CFilePathMgr::GetTemplateDataBasePath()
{
	return m_sTemplateDatabasePath;
}

CString CFilePathMgr::GetOnlineResourcePath()
{
	return _T("");
//	return  theCacheMgr->GetSongPath();
}

CString CFilePathMgr::GetImageCachePath()
{

	return theCacheMgr->GetImagePath();
}

CString CFilePathMgr::GetDefaultCachePath()
{
	return m_sDefaultCachePath;
}

//CString CFilePathMgr::GetExpressionPath()
//{
//	return m_sExpressionPath;
//}

CString CFilePathMgr::GetLrcFilePath()
{

	return theCacheMgr->GetLrcPath();
}

CString CFilePathMgr::GetWebServiceCatchPath()
{
	
	return theCacheMgr->GetWebPath();
}

CString CFilePathMgr::GetLogPath()
{
	return m_sLogPath;
}

