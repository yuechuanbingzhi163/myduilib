#include "stdafx.h"
#include "LrcMgr.h"
#include "../YApp.h"


struct LrcLoadParam
{
	spSongInfoT spSongInfo;
	CLrcMgr::spSearchFromWeb pSearch;
	CLrcMgr::spLrcReaderT spLrcReader;
};
const TCHAR CLrcMgr::REPLACE_CHARS[] = _T("/\\:*\"\?><|");

CLrcMgr::CLrcMgr()
{
	Init();
}

CLrcMgr::~CLrcMgr()
{
	
}

void CLrcMgr::Init()
{

	m_spLrcReader  = spLrcReaderT(new CLrcFileReader);
	m_spSerchLrc = spSearchFromWeb(new CSearchLrcFromWeb);

	m_nLineHeight   = 20;
	m_fTimeOffset   = 0.0;
	m_dwBKColor     = 0xFFFFFFFF;
	m_dwCurRowColor = 0xFFFC8103;
	m_dwNorRowColor = 0x00FFFBF0;//0xFF55444C;

	memset(&m_LogFont,0,sizeof(m_LogFont));
	m_LogFont.lfWidth       = 0;
	m_LogFont.lfUnderline   = FALSE;
	m_LogFont.lfStrikeOut   = FALSE;
	m_LogFont.lfOrientation = 0;
	m_LogFont.lfEscapement  = 0;
	m_LogFont.lfCharSet     = DEFAULT_CHARSET;
	m_LogFont.lfHeight      = -12;
	m_LogFont.lfWeight      = FW_NORMAL;
	m_LogFont.lfItalic      = FALSE;
	_tcscpy_s(m_LogFont.lfFaceName,_T("微软雅黑"));
}

CLrcMgr::spLrcReaderT CLrcMgr::GetLrcReader(spSongInfoT spSongInfo)
{
	ResetLrc();
	m_spLrcReader->Clear();
	if(spSongInfo && IsLrcExist(spSongInfo->GetLrcLocalPath()))
	{ //歌词文件存在
		m_spLrcReader->Load(spSongInfo->GetLrcLocalPath());
		theMainWin->LoadSongLrc();
		sLog(("CLrcMgr::GetLrcReader %s"),mci::unicode_2_ansi(spSongInfo->GetLrcLocalPath()).c_str());
		return m_spLrcReader;
	}

	CString sLrcFile = GetLrcDir(spSongInfo);
	if(sLrcFile.IsEmpty())
	{	
		theMainWin->LoadSongLrc();
		return m_spLrcReader;
	}
	if(IsLrcExist(sLrcFile))
	{
		m_spLrcReader->Load(sLrcFile);
		spSongInfo->SetLrcLocalPath(sLrcFile);
		thePlayListMgr->UpdateSong(spSongInfo,thePlayListMgr->GetCurList()->GetLocalId());
		thePlayListMgr->SetCurSong(spSongInfo);
		theMainWin->LoadSongLrc();
	}
	else
	{
		LrcLoadParam* param = new LrcLoadParam;
		param->pSearch = m_spSerchLrc;
		param->spLrcReader = m_spLrcReader;
		param->spSongInfo = spSongInfo;
		DWORD dwID = 0;
		HANDLE handle = CreateThread(NULL,0,DownloadLrcFromWeb,(LPVOID)param,0,&dwID);
		if(handle) CloseHandle(handle);
	}

	return m_spLrcReader;
}

CLrcMgr::spLrcReaderT CLrcMgr::GetLrcReader2()
{
	return m_spLrcReader;
}
DWORD __stdcall CLrcMgr::DownloadLrcFromWeb(LPVOID param)
{
	bool b_find = true;
	CDesktopLrcWindow* pDeskLrc(theMainWin->GetDesktopLrcWin2());
	if(pDeskLrc&&pDeskLrc->IsWindow())
		pDeskLrc->SetStopText(_T("正在查找歌词"));
	LrcLoadParam* pObj = (LrcLoadParam*)param;

	spLrcReaderT spReader = pObj->spLrcReader;
	spSearchFromWeb spSearch = pObj->pSearch;
	spSongInfoT spSong = pObj->spSongInfo;
	SongInfoVT LrcUrls = spSearch->SearchLrc(spSong);
	CString sFilePath = GetLrcDir(spSong);

	if (LrcUrls.size()>0)
	{
		int iIndex = 0;
		while(FALSE == spSearch->DownLoadFile(LrcUrls[iIndex].strLrcUrl,sFilePath)&&iIndex<LrcUrls.size())
		{
			iIndex++;
		}
		if (iIndex>=LrcUrls.size())
		{
			if(pDeskLrc&&pDeskLrc->IsWindow())
				pDeskLrc->SetStopText(_T("未搜索到匹配的歌词"));
			b_find = false;
		}
	}
	else
	{
		if(pDeskLrc&&pDeskLrc->IsWindow())
			pDeskLrc->SetStopText(_T("未搜索到匹配的歌词"));
		b_find = false;
	}

/*RETURN:	*/
	if(b_find)
	{		
		if(pDeskLrc&&pDeskLrc->IsWindow())
			pDeskLrc->SetStopText(_T("成功下载歌词"));
		::Sleep(500);
		theLrcMgr->GetLrcReader(spSong);

		::Sleep(500);
		if(pDeskLrc&&pDeskLrc->IsWindow())
			pDeskLrc->SetStopText(_T("逸听音乐"));
	}
	else
	{
		if(spSong&&pDeskLrc&&pDeskLrc->IsWindow())
			pDeskLrc->SetStopText(spSong->GetSongName());
	}
	delete pObj;

	return 0;
}
void CLrcMgr::DownloadLrcFromOther(spSongInfoT spSongInfo)
{
	assert(spSongInfo);
	if(!spSongInfo) return ;

	m_LrcList      = m_spSerchLrc->SearchLrc(spSongInfo);

	return ;
}

void CLrcMgr::DownloadLrcFromOther(CString sSongName,CString sArtistName)
{
	
	m_LrcList = m_spSerchLrc->SearchLrc(sSongName,sArtistName);

	return ;
}
void CLrcMgr::SetSelectLrcDownload(int nIndex, spSongInfoT pSong)
{
	assert(pSong);
	if (pSong)
	{
		CString sLrcFile = GetLrcDir(pSong);
		if(sLrcFile.IsEmpty())
			return ;
		if(IsLrcExist(sLrcFile))
		{
			int bRet = em_utility::mci::delete_file(sLrcFile.GetBuffer());
			sLrcFile.ReleaseBuffer();
			assert(bRet);
		}
		pSong->SetLrcUrl(m_LrcList[nIndex].strLrcUrl);
//		m_spSerchLrc->DownLoadFile(m_LrcList[nIndex].strLrcUrl,sLrcFile);
		GetLrcReader(pSong);
	}
}
CString CLrcMgr::GetLrcDir(spSongInfoT spSongInfo)
{
	CString sLrcDir = theCacheMgr->GetLrcPath();
	CString sLrcPath,sArtist,sSong;
	if(!spSongInfo) return sLrcPath;
	sArtist = spSongInfo->GetArtistName();
	sSong   = spSongInfo->GetSongName();

	if(sArtist.IsEmpty() || sSong.IsEmpty())
		return sLrcPath;

	CString sArtistName = FsReplace(spSongInfo->GetArtistName(),_T('_'));
	CString sSongName   = FsReplace(spSongInfo->GetSongName(),_T('_'));

	if(sArtistName.IsEmpty()|| sSongName.IsEmpty())
		return sLrcPath;

	sLrcPath.Format(_T("%s%s-%s.lrc"),sLrcDir,sArtistName,sSongName);
	return sLrcPath;
} 
CString CLrcMgr::FsReplace(const CString& sInput, const TCHAR cNew)
{
	CString sOutput = sInput;
	assert(!sOutput.IsEmpty());
	if(sOutput.IsEmpty())
		return sOutput;

	unsigned int nCount = sizeof(REPLACE_CHARS) / sizeof(REPLACE_CHARS[0]);
	for (unsigned int i = 0; i < nCount; ++i)
	{
		const TCHAR cOld = REPLACE_CHARS[i];
		sOutput.Replace(cOld, cNew);
	}
	int nLen = sOutput.GetLength();

	/*对非可视ASCII字符的处理*/
	static const unsigned short MIN_ASCII_VALID = 30;
	for (int i = 0; i < nLen; ++i)
	{
		TCHAR sValue = sOutput[i];
		unsigned short nValue = sValue;
		if (nValue < MIN_ASCII_VALID)
			sOutput.SetAt(i, cNew);
	}

	if (nLen > 0)					        /*对点号的特殊处理*/
	{
		if (sOutput[0] == _T('.'))		    /*起首点号*/
			sOutput.SetAt(0, cNew);
		if (sOutput[nLen - 1] == _T('.'))	/*结尾点号*/
			sOutput.SetAt(nLen - 1, cNew);
	}
	return sOutput;
}

bool CLrcMgr::IsLrcExist(CString& sLrcFileName)
{
	bool bFind = em_utility::mci::file_exist(sLrcFileName.GetBuffer());
	sLrcFileName.ReleaseBuffer();
	return bFind;
}
/************************************************************************/
/*    歌词设置                                                          */
/************************************************************************/
void CLrcMgr::SetFont(CString sFontName,int nFontHeight,bool bBold,bool bItalic)
{
	assert(sFontName.IsEmpty());
	if (nFontHeight > 0)
		m_LogFont.lfHeight = nFontHeight;
	if (bBold)
		m_LogFont.lfWeight = FW_BOLD;
	if(bItalic)
		m_LogFont.lfItalic = bItalic;
	if(!sFontName.IsEmpty())
		_tcscpy_s(m_LogFont.lfFaceName,sFontName);
}

void CLrcMgr::SetCurRowTextColor(DWORD dwColor)
{
	m_dwCurRowColor = dwColor;
}

void CLrcMgr::SetNorRowTextColor(DWORD dwColor)
{
	m_dwNorRowColor = dwColor;
}

void CLrcMgr::SetBkColor(DWORD dwColor)
{
	m_dwBKColor = dwColor;
}

void CLrcMgr::SetLineHeight(int nLineHeight)
{
	if (nLineHeight > 0)
		m_nLineHeight = nLineHeight;
}

void CLrcMgr::SetLrcForward(float fTimeOffset/* = 0.5*/)
{
	m_fTimeOffset += fTimeOffset;
}

void CLrcMgr::SetLrcBack(float fTimeOffset/* = 0.5*/)
{
	m_fTimeOffset -= fTimeOffset;
}

void CLrcMgr::ResetLrc()
{
	m_fTimeOffset = 0.0;
}

LOGFONT CLrcMgr::GetFont() const
{
	return m_LogFont;
}

DWORD CLrcMgr::GetCurRowTextColor() const
{
	return m_dwCurRowColor;
}

DWORD CLrcMgr::GetNorRowTextColor() const
{
	return m_dwNorRowColor;
}

DWORD CLrcMgr::GetBkColor() const
{
	return m_dwBKColor;
}

size_t CLrcMgr::GetLineHeight() const
{
	return m_nLineHeight;
}

float CLrcMgr::GetLrcTimeOffset() const
{
	return m_fTimeOffset;
}