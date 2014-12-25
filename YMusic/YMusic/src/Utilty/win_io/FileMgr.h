#pragma once


#include <set>

//文件管理
class CFileMgr : public Singleton<CFileMgr>
{

	CFileMgr(void);
public:
	virtual ~CFileMgr(void);
	typedef std::vector<CString>        filesArrayT;
	typedef filesArrayT::iterator       filesArrayIterT;
	typedef std::set<CString>           fileExtArrayT;
	typedef fileExtArrayT::iterator     fileExtArrayIterT;
public:
	CString GetModuleFilePath();
	bool file_exist(const wchar_t* s_file);
	CString rip_file_name(CString& s_path_file);
	int OpenFile(HWND hWnd,filesArrayT& importfiles);
	int OpenFolder(HWND hWnd,filesArrayT& improtfiles);
	int FindImportFiles(CString strPath,filesArrayT &importfiles);
protected:
	fileExtArrayT m_FileExt;
private:
	DECLARE_SINGLETON_CLASS(CFileMgr);
};

