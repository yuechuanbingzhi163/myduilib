#pragma once

/**
 * 管理各种文件路径
 * 数据库文件
 * 缓存路径
 * 日志文件路径
 */

#include "../../YMusic.h"
class CFilePathMgr
{
public:
	CFilePathMgr();
	~CFilePathMgr();


	//void SetDatabasePath(CString sPath);
	//void SetOnlineResourcePath(CString sPath);
	//void SetImageCachePath(CString sPath);
	//void SetCachePath(CString sPath);
	//void SetExpressionPath(CString sPath);
	CString LinkAvatarPath(unsigned int nUserID);
	CString LinkListenPath(unsigned int nResID,CString sFileName);
	CString LinkAlbumCoverPath(unsigned int nSongID);

	CString GetDatabasePath();
	CString GetTemplateDataBasePath();
	CString GetOnlineResourcePath();
	CString GetImageCachePath();
	CString GetDefaultCachePath();
	CString GetCurrentCachePath();
	//CString GetExpressionPath();
	CString GetLrcFilePath();
	CString GetAppPath();
	CString GetDataPath();
	CString GetWebServiceCatchPath();
	CString GetLogPath();
private:
	CString m_sTemplateDatabasePath;
	CString m_sDatabasePath;
	CString m_sOnlineResourcePath;
	CString m_sDefaultCachePath;
	CString m_sImageCachePath;
	CString m_sExpressionPath;
	CString m_sLrcFilePath;
	CString m_sWebServiceCatchPath;
	CString m_sLogPath;
};
