#pragma  once
#include <queue>
#include "lrcFileReader.h"

#include "SearchLrcFromWeb.h"
#include "../Utilty/mt_helper.h"

class CLrcMgr
{

public:
	typedef smart_pointer::SmartPtr<CLrcFileReader>  spLrcReaderT;
	typedef smart_pointer::SmartPtr<CSearchLrcFromWeb> spSearchFromWeb;
public:
	CLrcMgr(void);
	virtual ~CLrcMgr(void);

public:
	spLrcReaderT GetLrcReader(spSongInfoT spSongInfo);
	spLrcReaderT GetLrcReader2();

	void SetSelectLrcDownload(int nIndex,spSongInfoT pSong);
	void DownloadLrcFromOther(spSongInfoT spSongInfo);
	void DownloadLrcFromOther(CString sSongName,CString sArtistName);
	SongInfoVT GetSearchResultList(){ return m_LrcList ;}
private:
	bool Load(LPCTSTR lpszLrcFile);
	static DWORD __stdcall DownloadLrcFromWeb(LPVOID param);

	static CString GetLrcDir(spSongInfoT spSongInfo);
	static CString FsReplace(const CString& sInput, const TCHAR cNew);
	bool IsLrcExist(CString& sLrcFileName);
public:
	void SetFont(CString sFontName,int nFontHeight,bool bBold,bool bItalic);
	void SetCurRowTextColor(DWORD dwColor);
	void SetNorRowTextColor(DWORD dwColor);
	void SetBkColor(DWORD dwColor);
	void SetLineHeight(int nLineHeight);
	void SetLrcForward(float fTimeOffset = 0.5);
	void SetLrcBack(float fTimeOffset = 0.5);
	void ResetLrc();

	LOGFONT GetFont() const;
	DWORD   GetCurRowTextColor() const;
	DWORD   GetNorRowTextColor() const;
	DWORD   GetBkColor() const;
	size_t  GetLineHeight() const;
	float   GetLrcTimeOffset() const;

	
private:
	void Init();


private:
	em_utility::critical_session  m_lock;
	spLrcReaderT            m_spLrcReader;
	static const TCHAR      REPLACE_CHARS[];
	SongInfoVT              m_LrcList;
    spSearchFromWeb         m_spSerchLrc;

	//∏Ë¥ …Ë÷√
	LOGFONT                 m_LogFont;
	size_t                  m_nLineHeight;
	float                   m_fTimeOffset;//µ•Œª£∫√Î
	DWORD                   m_dwBKColor;
	DWORD                   m_dwCurRowColor;
	DWORD                   m_dwNorRowColor;
};