#include "stdafx.h"
#include "SearchLrcFromWeb.h"
#include "ParseXmlFile.h"


CSearchLrcFromWeb::CSearchLrcFromWeb(void)
{
	m_strTTSearchPath      = _T("http://ttlrcct2.qianqian.com/dll/lyricsvr.dll?sh?Artist=%s&Title=%s&Flags=0");
	m_strTTDownloadPath    = _T("http://ttlrcct2.qianqian.com/dll/lyricsvr.dll?dl?Id=%d&Code=%s");
	m_strBaiduSearchPath   = _T("http://box.zhangmen.baidu.com/x?op=12&count=1&title=%s$$%s$$$$");
    m_strBaiduDownloadPath = _T("http://box.zhangmen.baidu.com/bdlrc/%d/%d.lrc");
}


CSearchLrcFromWeb::~CSearchLrcFromWeb(void)
{

}

SongInfoVT CSearchLrcFromWeb::SearchLrc(spSongInfoT pSong)
{
	m_SongInfoV.clear();
	m_SongInfoV = SearchLrcFromTT(pSong);
	if (m_SongInfoV.empty())
		m_SongInfoV = SearchLrcFromBaidu(pSong);
	return m_SongInfoV;
}

SongInfoVT CSearchLrcFromWeb::SearchLrc(CString sSongName,CString sArtistName)
{
	m_SongInfoV.clear();
	if (sSongName.IsEmpty() || sArtistName.IsEmpty())
		return m_SongInfoV;
	m_SongInfoV = SearchLrcFromTT(sSongName,sArtistName);
	if (m_SongInfoV.empty())
		m_SongInfoV = SearchLrcFromBaidu(sSongName,sArtistName);
	return m_SongInfoV;
}

SongInfoVT CSearchLrcFromWeb::SearchLrcFromTT(spSongInfoT pSong)
{
	
	CString strArtist = pSong->GetArtistName();
	CString strTitle  = pSong->GetSongName();

	strArtist.MakeLower();
	strArtist.Replace(_T(" "), _T(""));
	strArtist.Replace(_T("'"), _T(""));
	strTitle.MakeLower();
	strTitle.Replace(_T(" "), _T(""));
	strTitle.Replace(_T("'"), _T(""));
	if (strTitle == _T(""))
		return m_SongInfoV;

	CString str_artist;
	CString str_title;

	if (strArtist != _T(""))
		str_artist = EncodeHelper::_binary_2_hex((unsigned char*)strArtist.GetBuffer(),strArtist.GetLength()*sizeof(TCHAR));
	if (strTitle != _T(""))
		str_title  = EncodeHelper::_binary_2_hex((unsigned char*)strTitle.GetBuffer(),strTitle.GetLength()*sizeof(TCHAR));

	str_artist.MakeUpper();
	str_title.MakeUpper();
	CString str_search_path;
	str_search_path.Format(m_strTTSearchPath, str_artist, str_title);

	CString strModulePath;
	strModulePath = mci::GetModulePath();
	if (strModulePath.Right(1) != '\\')
		strModulePath + _T("\\");


	CString strLocalFile = strModulePath;
	
	strLocalFile += _T("tracks.xml");

	bool bRet = DownloadLrcXml(str_search_path,strLocalFile);
//	assert(bRet);
	if(!bRet) return m_SongInfoV;

	fileinfo_container xmlTracks;
	CParseXmlFile::Parse(xmlTracks, strLocalFile);
	if (xmlTracks.size() <= 0)
		return m_SongInfoV;

	CString strCode;
	CString str_download_path;

	SONGINFO lrc_info;
	for (size_t i = 0; i < xmlTracks.size(); i++)
	{
		strCode = EncodeHelper::CreateQianQianCode(xmlTracks[i].strArtist, xmlTracks[i].strTitle, xmlTracks[i].nLrcID);
		str_download_path.Format(m_strTTDownloadPath, xmlTracks[i].nLrcID, strCode);

		lrc_info.strLrcUrl = str_download_path;
		lrc_info.strTitle  = xmlTracks[i].strTitle;
		lrc_info.strArtist = xmlTracks[i].strArtist;
		m_SongInfoV.push_back(lrc_info);
	}
	return m_SongInfoV;
}

SongInfoVT CSearchLrcFromWeb::SearchLrcFromTT(CString sSongName,CString sArtistName)
{
	CString strArtist = sArtistName;
	CString strTitle  = sSongName;

	strArtist.MakeLower();
	strArtist.Replace(_T(" "), _T(""));
	strArtist.Replace(_T("'"), _T(""));
	strTitle.MakeLower();
	strTitle.Replace(_T(" "), _T(""));
	strTitle.Replace(_T("'"), _T(""));
	if (strTitle == _T(""))
		return m_SongInfoV;

	CString str_artist;
	CString str_title;

	if (strArtist != _T(""))
		str_artist = EncodeHelper::_binary_2_hex((unsigned char*)strArtist.GetBuffer(),strArtist.GetLength()*sizeof(TCHAR));
	if (strTitle != _T(""))
		str_title  = EncodeHelper::_binary_2_hex((unsigned char*)strTitle.GetBuffer(),strTitle.GetLength()*sizeof(TCHAR));

	str_artist.MakeUpper();
	str_title.MakeUpper();
	CString str_search_path;
	str_search_path.Format(m_strTTSearchPath, str_artist, str_title);


	CString strModulePath;
	strModulePath = mci::GetModulePath();
	if (strModulePath.Right(1) != '\\')
		strModulePath + _T("\\");


	CString strLocalFile = strModulePath;
	strLocalFile += _T("tracks.xml");
	
	bool bRet = DownloadLrcXml(str_search_path,strLocalFile);
//	assert(bRet);
	if(!bRet) return m_SongInfoV;

	fileinfo_container xmlTracks;
	CParseXmlFile::Parse(xmlTracks, strLocalFile);
	if (xmlTracks.size() <= 0)
		return m_SongInfoV;

	CString strCode;
	CString str_download_path;

	SONGINFO lrc_info;
	for (size_t i = 0; i < xmlTracks.size(); i++)
	{
		strCode = EncodeHelper::CreateQianQianCode(xmlTracks[i].strArtist, xmlTracks[i].strTitle, xmlTracks[i].nLrcID);
		str_download_path.Format(m_strTTDownloadPath, xmlTracks[i].nLrcID, strCode);

		lrc_info.strLrcUrl = str_download_path;
		lrc_info.strTitle  = xmlTracks[i].strTitle;
		lrc_info.strArtist = xmlTracks[i].strArtist;
		m_SongInfoV.push_back(lrc_info);
	}
	return m_SongInfoV;
}

SongInfoVT CSearchLrcFromWeb::SearchLrcFromBaidu(CString sSongName,CString sArtistName)
{
	CString strArtist = sArtistName;
	CString strTitle  = sSongName;

	strArtist.MakeLower();
	strArtist.Replace(_T(" "), _T(""));
	strArtist.Replace(_T("'"), _T(""));
	strTitle.MakeLower();
	strTitle.Replace(_T(" "), _T(""));
	strTitle.Replace(_T("'"), _T(""));
	if (strTitle == _T(""))
		return m_SongInfoV;

	CString str_artist(strArtist);
	CString str_title(strTitle);

	str_artist.MakeUpper();
	str_title.MakeUpper();
	CString str_search_path;
	str_search_path.Format(m_strBaiduSearchPath, str_title, str_artist);

	CString strModulePath;
	strModulePath = mci::GetModulePath();
	if (strModulePath.Right(1) != '\\')
		strModulePath + _T("\\");

	CString strLocalFile = strModulePath;
	strLocalFile += _T("tracks.xml");
	
	bool bRet = DownloadLrcXml(str_search_path,strLocalFile);
//	assert(bRet);
	if(!bRet) return m_SongInfoV;

	fileinfo_container xmlTracks;
	CParseXmlFile::ParseBaiduLrcList(xmlTracks, strLocalFile);
	if (xmlTracks.size() <= 0)
		return m_SongInfoV;

	CString str_download_path;
	SONGINFO lrc_info;

	int n1 = xmlTracks[0].nLrcID;
	int n2 = int(n1 / 100.0);
	str_download_path.Format(m_strBaiduDownloadPath, n2, n1);

	lrc_info.strLrcUrl = str_download_path;
	lrc_info.strTitle  = strTitle;
	lrc_info.strArtist = strArtist;
	m_SongInfoV.push_back(lrc_info);

	return m_SongInfoV;
}

SongInfoVT CSearchLrcFromWeb::SearchLrcFromBaidu(spSongInfoT pSong)
{
	CString strArtist = pSong->GetArtistName();
	CString strTitle  = pSong->GetSongName();

	strArtist.MakeLower();
	strArtist.Replace(_T(" "), _T(""));
	strArtist.Replace(_T("'"), _T(""));
	strTitle.MakeLower();
	strTitle.Replace(_T(" "), _T(""));
	strTitle.Replace(_T("'"), _T(""));
	if (strTitle == _T(""))
		return m_SongInfoV;

	CString str_artist(strArtist);
	CString str_title(strTitle);

	str_artist.MakeUpper();
	str_title.MakeUpper();
	CString str_search_path;
	str_search_path.Format(m_strBaiduSearchPath, str_title, str_artist);

	CString strModulePath;
	strModulePath = mci::GetModulePath();
	if (strModulePath.Right(1) != '\\')
		strModulePath + _T("\\");


	CString strLocalFile = strModulePath;
	strLocalFile += _T("tracks.xml");

	bool bRet = DownloadLrcXml(str_search_path,strLocalFile);
//	assert(bRet);
	if(!bRet) return m_SongInfoV;

	fileinfo_container xmlTracks;
	CParseXmlFile::ParseBaiduLrcList(xmlTracks, strLocalFile);
	if (xmlTracks.size() <= 0)
		return m_SongInfoV;

	CString str_download_path;
	SONGINFO lrc_info;

	int n1 = xmlTracks[0].nLrcID;
	int n2 = int(n1 / 100.0);
	str_download_path.Format(m_strBaiduDownloadPath, n2, n1);

	lrc_info.strLrcUrl = str_download_path;
	lrc_info.strTitle  = strTitle;
	lrc_info.strArtist = strArtist;
	m_SongInfoV.push_back(lrc_info);

	return m_SongInfoV;
}

bool CSearchLrcFromWeb::DownloadLrcXml(CString strUrl,CString strLocalFilePath)
{
#define  BUFFER_LEN 4096 // 4K

	try{
		HINTERNET hInternetSession = NULL, hOpenUrl = NULL ;
		hInternetSession = InternetOpen(
			_T("Microsoft@Internet Explorer"),
			INTERNET_OPEN_TYPE_PRECONFIG,
			NULL,
			NULL,
			0);
		assert(hInternetSession);
		if(!hInternetSession) return false;

		hOpenUrl=::InternetOpenUrl(hInternetSession,strUrl, NULL, 0, INTERNET_FLAG_RAW_DATA, 0);
//		assert(hOpenUrl);
		if(!hOpenUrl)
		{
			InternetCloseHandle(hInternetSession);
			return false;
		}
		FILE *stream = NULL;
		//	stream = _tfopen(strLocalFilePath.GetBuffer(),_T("wb"));
		_wfopen_s(&stream,strLocalFilePath,_T("wb"));
		assert(stream);
		if (!stream)
		{
			InternetCloseHandle(hOpenUrl);
			InternetCloseHandle(hInternetSession);
			return false;
		}
		ULONG nNumber=1;
		while (nNumber>0)
		{
			BYTE Buffer[BUFFER_LEN];
			InternetReadFile(hOpenUrl,(LPVOID)Buffer,BUFFER_LEN,&nNumber);
			fwrite(Buffer,sizeof(char),nNumber,stream);
		}
		fclose(stream);
		InternetCloseHandle(hOpenUrl);
		InternetCloseHandle(hInternetSession);
		return true;
	}
	catch(...)
	{
		throw L"DownloadLrcXml";
		return false;
	}
}
//http://ttlrcct2.qianqian.com/dll/lyricsvr.dll?dl?Id=448265&Code=-974517898

// UINT DownloadThread(void *pArg)
// {
// 	return 0;
// }
// bool CSearchLrcFromWeb::DownLoad(const char* url)
// {
// 	
//	return 0;
//}