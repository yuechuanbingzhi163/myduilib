#include "stdafx.h"

#include "FileMgr.h"

#include <Commdlg.h>  //
#include <commctrl.h>
#include <Shobjidl.h>

CFileMgr::CFileMgr(void)
{
	m_FileExt.insert(_T(".mp3"));
	m_FileExt.insert(_T(".ape"));
	m_FileExt.insert(_T(".m4a"));
	m_FileExt.insert(_T(".aac"));
	m_FileExt.insert(_T(".ogg"));
	m_FileExt.insert(_T(".flac"));
	m_FileExt.insert(_T(".wav"));
	m_FileExt.insert(_T(".cue"));
	m_FileExt.insert(_T(".wma"));
}


CFileMgr::~CFileMgr(void)
{
}
/////

CString CFileMgr::GetModuleFilePath()
{
	TCHAR s_buf[MAX_PATH];
	s_buf[0] = 0x0;
	DWORD n_result = ::GetModuleFileName(NULL, s_buf, sizeof(TCHAR) * MAX_PATH);
	TCHAR	s_drive[MAX_PATH];
	s_drive[0] = 0x0;
	TCHAR s_dir[MAX_PATH];
	s_dir[0] = 0x0;
	/*errno_t n_err_no =*/ 
	TCHAR s_filename[MAX_PATH];
	s_filename[0] = 0x0;
	TCHAR s_ext[MAX_PATH];
	s_ext[0] = 0x0;
	//	_tsplitpath(s_buf, s_drive, s_dir, NULL, NULL);
	_tsplitpath_s(s_buf, s_drive,MAX_PATH, s_dir, MAX_PATH,s_filename,MAX_PATH,s_ext,MAX_PATH);
	//assert(n_err_no == 0);
	_tcscpy_s(s_buf, s_drive);
	_tcscat_s(s_buf, s_dir);
	return CString(s_buf);
}

bool CFileMgr::file_exist(const wchar_t* s_file)
{
	if (s_file != NULL && wcslen(s_file) > 0)
	{
		struct _stat32 stat_info;
		return _wstat32(s_file, &stat_info) == 0;
	}
	else
		return false;
}

CString CFileMgr::rip_file_name(CString& s_path_file)
{
	CString s_file_name;
	int n_pos = s_path_file.ReverseFind(_T('\\'));
	if (n_pos > 0)
	{
		s_file_name = s_path_file.Right(s_path_file.GetLength() - n_pos - 1);
	}
	return s_file_name;
}

int CFileMgr::OpenFile(HWND hWnd,CFileMgr::filesArrayT& importfiles)
{
	OPENFILENAME openfilename;
	ZeroMemory(&openfilename, sizeof(OPENFILENAME)); 
	TCHAR szfiletitle[MAX_PATH] = _T("\0");
	CString s_title=_T("请选择要添加的音频文件");
	LPTSTR szFile = new TCHAR[MAX_PATH * 101 + 1]; //最多可以选中100项
	szFile[0] = 0x0;
	const TCHAR lpstr[] = _T("音频文件(*.mp3;*.ape;*.m4a;*.aac;*.ogg;*.flac;*.wav;*wma;*.cue)\0*.mp3;*.ape;*.m4a;*.aac;*.ogg;*.flac;*.wav;*wma;*.cue\0\0");
	openfilename.lStructSize = sizeof(OPENFILENAME) ;
	openfilename.hwndOwner = hWnd ;
	openfilename.hInstance = NULL ;
	openfilename.lpstrFilter = lpstr;//lpstr;
	openfilename.lpstrCustomFilter = (LPTSTR)NULL ;
	openfilename.nMaxCustFilter = 0L ;
	openfilename.nFilterIndex = 1L ;
	openfilename.lpstrFile = szFile ;
	openfilename.nMaxFile = MAX_PATH * 101 + 1 ;
	openfilename.lpstrFileTitle = szfiletitle ;
	openfilename.nMaxFileTitle = sizeof(szfiletitle) ;
	openfilename.lpstrInitialDir = NULL ;
	openfilename.lpstrTitle = s_title;
	openfilename.nFileOffset = 0;
	openfilename.nFileExtension = 0;
	openfilename.lpstrDefExt = _T("*.mp3") ;
	openfilename.lCustData = 0 ;

	openfilename.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_READONLY | OFN_ALLOWMULTISELECT | OFN_EXPLORER /*|OFN_LONGNAMES*/;

	if (GetOpenFileName(&openfilename))
	{
		LPTSTR p = szFile;
		while(*p++);
		if (*p == NULL) // 只选了一个文件
		{
			CString s_file = szFile;

			importfiles.push_back(s_file);
		}
		else    ///////////////////////////////////////////////////////// 多选，循环取出文件
		{
			CString folder = szFile;
			while(*p)
			{
				CString fileName = p;
				CString file = folder + _T("\\") + fileName;
				importfiles.push_back(file);

				while(*p++);
			}
		}
	}
	delete[] szFile;
	return 0;
}
int CFileMgr::OpenFolder(HWND hWnd,CFileMgr::filesArrayT &improtfiles)
{
	TCHAR Buffer[MAX_PATH]={0};
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;//此参数如为NULL，则不能显示对话框
	bi.lpszTitle = _T("选择媒体文件夹");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
	if(pIDList)
	{//获取路径名称
		SHGetPathFromIDList(pIDList,Buffer);
		
		FindImportFiles(Buffer,improtfiles);

	}
	return 0;
}
int CFileMgr::FindImportFiles(CString strPath,CFileMgr::filesArrayT &importfiles)
{
	try
	{
		WIN32_FIND_DATA find_data;
		HANDLE h_find = INVALID_HANDLE_VALUE;
		CString dir = strPath;
		if(strPath[strPath.GetLength() - 1] == _T('\\'))
		{
			dir = strPath.Left(strPath.GetLength() - 1);
		}
		CString s_search_dir = dir;
		s_search_dir += _T("\\*.*");
		h_find = ::FindFirstFile(s_search_dir, &find_data);
		s_search_dir = dir + _T('\\');
		s_search_dir += find_data.cFileName;
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //目录
		{
			if (!(_tcscmp(find_data.cFileName, _T(".")) ==0 || _tcscmp(find_data.cFileName, _T("..")) == 0))	//非上层和自身目录
			{
				FindImportFiles(s_search_dir,importfiles);
			}
		}
		else
		{
			int nIndex=s_search_dir.ReverseFind(_T('.'));
			if (nIndex>0)
			{
				CString s_temp=s_search_dir.Right(s_search_dir.GetLength()-nIndex).MakeLower();
				if (m_FileExt.find(s_temp)!=m_FileExt.end())
				{
					CString FilePath = s_search_dir;

					importfiles.push_back(FilePath);
				}
			}
		}
		while (::FindNextFile(h_find, &find_data))
		{
			s_search_dir = dir + _T('\\');
			s_search_dir += find_data.cFileName;
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //目录
			{
				if (!(_tcscmp(find_data.cFileName, _T(".")) ==0 || _tcscmp(find_data.cFileName, _T("..")) == 0))	//非上层和自身目录
				{
					FindImportFiles(s_search_dir,importfiles);
				}
			}
			else
			{
				int nIndex=s_search_dir.ReverseFind(_T('.'));
				if (nIndex>0)
				{
					CString s_temp=s_search_dir.Right(s_search_dir.GetLength()-nIndex).MakeLower();
					if (m_FileExt.find(s_temp)!=m_FileExt.end())
					{
						CString FilePath = s_search_dir;
					
						importfiles.push_back(FilePath);
					}
				}
			}
		}
		::FindClose(h_find);
	}
	catch(...)
	{
		throw _T("CFileMgr::FindImportFiles");
	}
	return 0;
}
