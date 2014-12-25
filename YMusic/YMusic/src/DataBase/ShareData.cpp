#include "stdafx.h"
#include "ShareData.h"
/************************************************************************/
/*                                                                      */
/************************************************************************/
// #if defined (_DEBUG) 
// TCHAR const XIAMI_HOST[] = _T("http://www.xiami.com");
// TCHAR const SERVER_URL[] = _T("http://www.xiami.com/app/xiating");
// #else
// TCHAR const XIAMI_HOST[] = _T("http://www.xiami.com");
// TCHAR const SERVER_URL[] = _T("http://www.xiami.com/app/xiating");
// #endif // _DEBUG
#if defined (_DEBUG) 
TCHAR const XIAMI_HOST[] = _T("http://localhost");
TCHAR const SERVER_URL[] = _T("http://localhost/YMusic");
#else
TCHAR const XIAMI_HOST[] = _T("http://localhost");
TCHAR const SERVER_URL[] = _T("http://localhost/YMusic");
#endif // _DEBUG
/************************************************************************/
/*                                                                      */
/************************************************************************/

CSongInfo::CSongInfo()
{
	m_nLocalID = 0;
	m_bLocal = true;
	m_nSongID = 0;
	m_nResourceID = 0;
	m_nAlbumID = 0;
	m_nArtistID = 0;
	m_nAudioLength = 0;
	m_nPlayCount = 0;
	m_nFileType = 0;
	m_nStartPos = 0;
	m_nFileSize = 0;
	m_bAble = true;
	m_nPlayCount = 0;
	m_lAddTimeGMT = 0;
	m_lLastModifyTimeGMT = 0;
	m_lLastPlayTimeGMT = 0;
	m_nFlag = 0;
	m_nTag = 0;
	m_sFileExt = _T("");
}

CSongInfo::CSongInfo(const CSongInfo& s)
{
	*this = s;
}


CSongInfo::~CSongInfo()
{

}
bool CSongInfo::operator ==(const CSongInfo& other)
{
	return ((IsLocal()==other.IsLocal())
		&&(GetLocalId()==other.GetLocalId())
		&&(GetSongId()==other.GetSongId())
		&&(GetSongName()==other.GetSongName())
		&&(GetResourceId()==other.GetResourceId())
		&&(GetAlbumId()==other.GetAlbumId())
		&&(GetArtistId()==other.GetArtistId())
		&&(GetAudioLength()==other.GetAudioLength())
		&&(GetAddTime()==other.GetAddTime())
		&&(GetFileType()==other.GetFileType())
		&&(GetFileStartPos()==other.GetFileStartPos())
		&&(GetFileSize()==other.GetFileSize())
		&&(GetAlbumName()==other.GetAlbumName())
		&&(GetArtistName()==other.GetArtistName())
		&&(GetRealArtistName()==other.GetRealArtistName())
		&&(GetCoverLocalPath()==other.GetCoverLocalPath())
		&&(GetFileExt()==other.GetFileExt())
		&&(GetMD5()==other.GetMD5())
		&&(GetFlag()==other.GetFlag())
		&&(GetTag()==other.GetTag())
		);
}
bool CSongInfo::IsLocal()const
{
	return m_bLocal;
}
void CSongInfo::SetLocal(bool bLocal)
{
	m_bLocal = bLocal;
}
bool CSongInfo::IsEnable()const
{
	return m_bAble;
}
void CSongInfo::SetEnable(bool bEnable)
{
	m_bAble = bEnable;
}
UINT CSongInfo::GetLocalId()const
{
	return m_nLocalID;
}
void CSongInfo::SetLocalId(UINT nId)
{
	m_nLocalID = nId;
}
UINT CSongInfo::GetSongId()const
{
	return m_nSongID;
}
void CSongInfo::SetSongId(UINT nSongId)
{
	if(m_nSongID!=0)
		return;
	m_nSongID = nSongId;
}
UINT CSongInfo::GetResourceId()const
{
	return m_nResourceID;
}
void CSongInfo::SetResourceID(UINT nResourceId)
{
	m_nResourceID = nResourceId;
}
UINT CSongInfo::GetAlbumId()const
{
	return m_nAlbumID;
}
void CSongInfo::SetAlbumId(UINT nAlbumId)
{
	m_nAlbumID = nAlbumId;
}
UINT CSongInfo::GetArtistId()const
{
	return m_nArtistID;
}
void CSongInfo::SetArtistId(UINT nArtistId)
{
	m_nArtistID = nArtistId;
}
UINT CSongInfo::GetAudioLength()const
{
	return m_nAudioLength;
}
void CSongInfo::SetAudioLength(UINT nAudioLength)
{
	m_nAudioLength = nAudioLength;
}
UINT CSongInfo::GetPlayCount()const
{
	return m_nPlayCount;
}
void CSongInfo::SetPlayCount(UINT nPlayCount)
{
	m_nPlayCount = nPlayCount;
}
void CSongInfo::AddPlayCount()
{
	m_nPlayCount++;
}
int CSongInfo::GetFileType()const
{
	return m_nFileType;
}
void CSongInfo::SetFileType(int nFileType)
{
	m_nFileType = nFileType;
}
UINT CSongInfo::GetFileSize()const
{
	return m_nFileSize;
}
void CSongInfo::SetFileSize(UINT nFileSize)
{
	m_nFileSize = nFileSize;
}
UINT CSongInfo::GetFileStartPos()const
{
	return m_nStartPos;
}
void CSongInfo::SetFileStartPos(UINT nStartPos)
{
	m_nStartPos = nStartPos;
}
CString CSongInfo::GetSongName()const
{
	return m_sSongName;
}
void CSongInfo::SetSongName(const CString& sSongName)
{
	m_sSongName = sSongName;
}
CString CSongInfo::GetAlbumName()const
{
	return m_sAlbumName;
}
void CSongInfo::SetAblumName(const CString& sAblumName)
{
	m_sAlbumName=sAblumName;
}
CString CSongInfo::GetArtistName()const
{
	return m_sArtistName;
}
void CSongInfo::SetArtistName(const CString& sArtistName)
{
	m_sArtistName = sArtistName;
}
CString CSongInfo::GetRealArtistName()const
{
	return m_sRealArtistName;
}
void CSongInfo::SetRealArtistName(const CString& sRealArtistName)
{
	m_sRealArtistName = sRealArtistName;
}
CString CSongInfo::GetLocalPath()const
{
	return m_sLocalPath;
}
void CSongInfo::SetLocalPath(const CString& sPath)
{
	m_sLocalPath = sPath;
}
CString CSongInfo::GetCoverLocalPath()const
{
	return m_sCoverLocalPath;
}
void CSongInfo::SetCoverLocalPath(const CString& sPath)
{
	m_sCoverLocalPath = sPath;
}
CString CSongInfo::GetLrcLocalPath()const
{
	return m_sLrcLocalPath;
}
void CSongInfo::SetLrcLocalPath(const CString& sPath)
{
	m_sLrcLocalPath = sPath;
}
CString CSongInfo::GetListenFileUrl()const
{
	return m_sListenFileUrl;
}
void CSongInfo::SetListenFileUrl(const CString& sUrl)
{
	m_sListenFileUrl = sUrl;
}
void CSongInfo::SetFileExt(const CString& sExt)
{
	m_sFileExt = sExt;
}
CString CSongInfo::GetFileExt() const
{
	return m_sFileExt;
}
CString CSongInfo::GetMD5()const
{
	return m_sMD5;
}
void CSongInfo::SetMD5(const CString& sMD5)
{
	m_sMD5 = sMD5;
}
CString CSongInfo::GetCoverUrl()const
{
	return m_sCoverUrl;
}
void CSongInfo::SetCoverUrl(const CString& sUrl)
{
	m_sCoverUrl = sUrl;
}
CString CSongInfo::GetLrcUrl()const
{
	return m_sLrcUrl;
}
void CSongInfo::SetLrcUrl(const CString& sUrl)
{
	m_sLrcUrl = sUrl;
}

long CSongInfo::GetAddTime()const
{
	return m_lAddTimeGMT;
}
void CSongInfo::SetAddTime(long lTime)
{
	m_lAddTimeGMT = lTime;
}
long CSongInfo::GetLastModifyTime()const
{
	return m_lLastModifyTimeGMT;
}
void CSongInfo::SetLastModifyTime(long lTime)
{
	m_lLastModifyTimeGMT = lTime;
}
long CSongInfo::GetLastPlayTime()const
{
	return m_lLastPlayTimeGMT;
}
void  CSongInfo::SetLastPlayTime(long lTime)
{
	m_lLastPlayTimeGMT = lTime;
}
int CSongInfo::GetFlag()const
{
	return m_nFlag;
}
void CSongInfo::SetFlag(int nFlag)
{
	m_nFlag = nFlag;
}
int CSongInfo::GetTag() const
{
	return m_nTag;
}

void CSongInfo::SetTag(int ntag)
{
	m_nTag = ntag;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CPlayListInfo::CPlayListInfo()
{
	m_nLocalID = 0;
	m_nID = 0;
	m_nFlag = 0;
	m_eType= plt_user;
	m_bShow = true;
	m_nTag = 0;
}
CPlayListInfo::~CPlayListInfo()
{

}
CPlayListInfo::CPlayListInfo(const CString& sPlayListName,CPlayListInfo::PlayListType eType)
{
	m_nLocalID = 0;
	m_nID = 0;
	m_sName = sPlayListName;
	m_eType = eType;
}

UINT CPlayListInfo::GetLocalId()const
{
	return m_nLocalID;
}
void CPlayListInfo::SetLocalId(UINT nId)
{
	m_nLocalID = nId;
}
UINT CPlayListInfo::GetId()const
{
	return m_nID;
}
void CPlayListInfo::SetId(UINT nId)
{
	m_nID = nId;
}
CPlayListInfo::PlayListType CPlayListInfo::GetListType()const
{
	return m_eType;
}
void CPlayListInfo::SetListType(CPlayListInfo::PlayListType eType)
{
	m_eType = eType;
}
CString CPlayListInfo::GetName()const
{
	return m_sName;
}
void CPlayListInfo::SetName(const CString& sName)
{
	m_sName = sName;
}
CString CPlayListInfo::GetComment()const
{
	return m_sComment;
}
void CPlayListInfo::SetComment(const CString& sComment)
{
	m_sComment = sComment;
}
bool CPlayListInfo::IsShow()const
{
	return m_bShow;
}
void CPlayListInfo::SetShow(bool bShow)
{
	m_bShow = bShow;
}
int CPlayListInfo::GetFlag()const
{
	return m_nFlag;
}
void CPlayListInfo::SetFlag(int nFlag)
{
	m_nFlag = nFlag;
}
int CPlayListInfo::GetTag() const
{
	return m_nTag;
}

void CPlayListInfo::SetTag(int ntag)
{
	m_nTag = ntag;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CFavInfo::CFavInfo()
{
	// 	m_nId = 0;
	// 	m_eType = ft_local;//0表示未匹配的单曲;1表示收藏的为单曲;2专辑;3艺人;4精选集;
}
CFavInfo::CFavInfo(FavType type,UINT nId,LPCTSTR pszName /* = NULL */):m_eType(type),m_nId(nId),m_sName(pszName)
{
	// 	m_nId = 0;
	// 	m_eType = ft_local;//0表示未匹配的单曲;1表示收藏的为单曲;2专辑;3艺人;4精选集;
}
CFavInfo::~CFavInfo()
{

}

UINT CFavInfo::GetFavId()const
{
	return m_nId;
}

void CFavInfo::SetFavId(UINT nId)
{
	m_nId = nId;
}
CString CFavInfo::GetFavName()const
{
	return m_sName;
}
void CFavInfo::SetFavName(const CString& sName)
{
	m_sName = sName;
}
CFavInfo::FavType CFavInfo::GetType()const
{
	return m_eType;
}
void CFavInfo::SetType(CFavInfo::FavType eType)
{
	m_eType = eType;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CSynOperation::CSynOperation()
{
	nAutoID = 0;
	nObjectID = 0;
	eObjType = SOT_PlayList;
	eOperType = OT_Add;
	nFlag = 0 ;
}

CSynOperation::CSynOperation( unsigned int nId,ObjectType eObType,OperateType eOpType )
{
	nAutoID = 0;
	nObjectID = nId;
	eObjType = eObType;
	eOperType = eOpType;
	nFlag = 0 ;
}

CSynOperation::~CSynOperation()
{

}

unsigned int CSynOperation::GetAutoID() const
{
	return nAutoID;
}

void CSynOperation::SetAutoID( unsigned int nID )
{
	nAutoID = nID;
}

unsigned int CSynOperation::GetObjectID() const
{
	return nObjectID;
}

void CSynOperation::SetObjectID( unsigned int nID )
{
	nObjectID = nID;
}

CSynOperation::ObjectType CSynOperation::GetObjectType() const
{
	return eObjType;
}

void CSynOperation::SetObjectType( ObjectType eType )
{
	eObjType = eType;
}

CSynOperation::OperateType CSynOperation::GetOperateType() const
{
	return eOperType;
}

void CSynOperation::SetOperateType( OperateType eType )
{
	eOperType = eType;
}

int CSynOperation::GetFlag() const
{
	return nFlag;
}

void CSynOperation::SetFlag( int nflag )
{
	nFlag = nflag;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
unsigned int const MAX_USER_LIST = 30;
unsigned int const MAX_FAV_NOTICE = 5;
/************************************************************************/
/*                                                                      */
/************************************************************************/
namespace ConfigEX
{
	const TCHAR KEEPMOST_TOP[] = _T("_KeepMostTop");
	const TCHAR VOLUME_OFF[] = _T("_VolumeOff");
	const TCHAR VOLUME[] = _T("_Volume");
	const TCHAR LOCKDESKLRC[] = _T("_LockDeskLrc");
	const TCHAR NOWARN_REMOVELIST[] = _T("_NoWarn_RemoveList");
	const TCHAR NOWARN_REMOVESONG[] = _T("_NoWarn_RemoveSong");
};