#include "stdafx.h"
#include "CacheMgr.h"



CCacheMgr::CCacheMgr(void)
{
	m_nTotalCacheSize = 0;
}

CCacheMgr::~CCacheMgr(void)
{
}

void CCacheMgr::Init(CString sCachePath)
{
	sCachePath.Replace('\\','/');
	if(sCachePath.GetAt(sCachePath.GetLength()-1) != _T('/'))
	{
		sCachePath += _T("/");
	}
	m_sCachePath = sCachePath;
	m_sLrcPath = m_sCachePath + _T("lrc/");
	em_utility::mci::confirm_dir(m_sLrcPath,_T('/'));
	m_sSongPath = m_sCachePath + _T("song/");
	em_utility::mci::confirm_dir(m_sSongPath,_T('/'));
	m_sImagePath = m_sCachePath + _T("image/");
	em_utility::mci::confirm_dir(m_sImagePath,_T('/'));
	m_sWebPath = m_sCachePath + _T("web/");
	em_utility::mci::confirm_dir(m_sWebPath,_T('/'));
	m_nTotalCacheSize = GetFolderSize(m_sLrcPath) + GetFolderSize(m_sSongPath) + GetFolderSize(m_sImagePath) + GetFolderSize(m_sWebPath);
	StatFolderSize(0);
}

void CCacheMgr::ClearAllCatch()
{	
// 	CleanFolder(m_sLrcPath);
// 	CleanFolder(m_sSongPath);
// 	CleanFolder(m_sImagePath);
// 	CleanFolder(m_sWebPath);
	m_nTotalCacheSize = 0;
}

void CCacheMgr::CleanFolder(CString sPath)
{
	WIN32_FIND_DATA FIND_DATA;
	int nTotalSize = 0;
	HANDLE h_find = INVALID_HANDLE_VALUE;

	CString sFilePath = sPath + _T("/*.*");
	h_find = ::FindFirstFile(sFilePath,&FIND_DATA);
	if (h_find == INVALID_HANDLE_VALUE) 
		return ;

	do 
	{
		CString s_file_name = sPath;
		s_file_name += FIND_DATA.cFileName;
		em_utility::mci::delete_file(s_file_name);
	} while (::FindNextFile(h_find, &FIND_DATA));
	FindClose(h_find);
}

size_t CCacheMgr::GetFolderSize(CString sPath)
{
	WIN32_FIND_DATA FIND_DATA;
	size_t nTotalSize = 0;
	HANDLE h_find = INVALID_HANDLE_VALUE;

	CString sFilePath = sPath + _T("*.*");
	h_find = ::FindFirstFile(sFilePath,&FIND_DATA);
	if (h_find == INVALID_HANDLE_VALUE) 
		return 0;

	do 
	{
		CString s_file_name = sPath;
		s_file_name += FIND_DATA.cFileName;
		nTotalSize += em_utility::mci::get_file_size(s_file_name);
	} while (::FindNextFile(h_find, &FIND_DATA));

	FindClose(h_find);
	return nTotalSize;
}

size_t CCacheMgr::GetAllCatchSize()
{
	return m_nTotalCacheSize;
}

void CCacheMgr::StatFolderSize(size_t nNewFileSize)
{

		
}


CString CCacheMgr::GetLrcPath()
{
	return m_sLrcPath;
}

CString CCacheMgr::GetSongPath()
{
	return m_sSongPath;
}

CString CCacheMgr::GetImagePath()
{
	return m_sImagePath;
}

CString CCacheMgr::GetWebPath()
{	
	return m_sWebPath;
}
