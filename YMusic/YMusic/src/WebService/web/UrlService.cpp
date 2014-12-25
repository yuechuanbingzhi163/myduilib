#include "stdafx.h"
#include "UrlService.h"



namespace url {
CRequest::CRequest(emRequestT requestType,const URLSTR& paths,const URLSTR& params)
	:_requestType(requestType),
	_sPaths(paths),
	_sParams(params),
	_hwnd(NULL),
	_tCreate(clock()),
	_bIsFromCatch(false),
	_nStrategyType(CST_NOCATCH),
	_isTimeOut(false),
	_dwTag(0)
{

}
CRequest::CRequest(emRequestT requestType,const URLSTR& paths,const URLSTR& params,ResponderT responder)
	:_requestType(requestType),
	_sPaths(paths),
	_sParams(params),
	_hwnd(NULL),
	_responder(responder),
	_tCreate(clock()),
	_bIsFromCatch(false),
	_nStrategyType(CST_NOCATCH),
	_isTimeOut(false),
	_dwTag(0)
{

}
CRequest::~CRequest()
{

}
int CRequest::GetCreateTime()
{
	return _tCreate;
}
emRequestT CRequest::GetRequestType() const
{
	return _requestType;
}
URLSTR CRequest::GetPaths() const
{
	return _sPaths;
}
URLSTR CRequest::GetParams() const
{
	return _sParams;
}

HWND CRequest::GetHwnd() const
{
	return _hwnd;
}
void CRequest::SetHwnd(HWND h)
{
	_hwnd = h;
}
DWORD CRequest::GetTag() const
{
	return _dwTag;
}
void CRequest::SetTag(DWORD dwTag)
{
	_dwTag = dwTag;
}

CatchStrategyType CRequest::GetCatchStrategyType() const
{
	return _nStrategyType;
}
void CRequest::SetCatchStrategyType(CatchStrategyType nType)
{
	_nStrategyType = nType;
}

bool CRequest::IsCatched() const
{
	return _bIsFromCatch;
}
void CRequest::SetCatched(bool bCatch)
{
	_bIsFromCatch = bCatch;
}

bool CRequest::IsTimeOut() const
{
	return _isTimeOut;
}
void CRequest::SetTimeOut(bool b)
{
	_isTimeOut = b;
}

bool CRequest::HasReponder() const
{
	return !!_responder;
}
void CRequest::SetReponder(ResponderT responder)
{
	_responder = responder;
}
void CRequest::Response(void* pdata)
{
	assert(pdata);
	assert(HasReponder());
	if(_responder)
		_responder(this,pdata);
}

static CRequest* MakeRequest(emRequestT reType,const char* szPaths,const char* szParams)
{
	URLSTR sPaths,sParams;
	if(szPaths) sPaths += szPaths;
	if(szParams) sParams += szParams;
	return (new CRequest(reType,sPaths,URL::UrlEncode(sParams)));
}

static CRequest* MakeRequestWithTag(emRequestT reType,const char* szPaths,const char* szParams,DWORD dwTag)
{
	CRequest* rq = MakeRequest(reType,szPaths,szParams);
	rq->SetTag(dwTag);
	return rq;
}
static CRequest* MakeRequestWithParamsTag(emRequestT reType,const char* szParams,DWORD dwTag)
{
	return MakeRequestWithTag(reType,"/api",szParams,dwTag);
}
static CRequest* MakeRequestWithParams(emRequestT reType,const char* szParams)
{
	return MakeRequestWithParamsTag(reType,szParams,0); 
}

/************************************************************************/
/*                                                                      */
/************************************************************************/


const std::string Requester::_sAPI_URL ="/api/";
const std::string Requester::_sAPI_KEY ="bf2061ba3590d1c725bb25a0f6c100eb";
const std::string Requester::_sAPI_SECRET ="d2b94a19c762c0b69d04cefe5b66f211";
Requester::Requester()
{

}
Requester::~Requester()
{

}

std::string Requester::GenSigUrl(const std::string& sMethod, const std::vector<std::pair<std::string,std::string> >& v)
{
	std::string sSigUrl;
	std::string sValue;
	std::string sMd5;

	assert(!sMethod.empty());
	assert(v.size());

	if(sMethod.empty())
		return sSigUrl;

	std::map<std::string,std::string> m;

	// 拼接url
	//sSigUrl = _sAPI_URL + "?";
	for (auto iter = v.begin(); iter != v.end(); ++iter)
	{
		sSigUrl.append(URL::UrlEncode(iter->first));
		sSigUrl.append("=");
		sSigUrl.append(URL::UrlEncode(iter->second));
		sSigUrl.append("&");

		m.insert(std::make_pair(iter->first,iter->second));
	}

	sSigUrl.append("api_key=");
	sSigUrl.append(_sAPI_KEY);
	sSigUrl.append("&method=");
	sSigUrl.append(sMethod);
	sSigUrl.append("&call_id=");

	time_t ltime;
	time(&ltime);
	char buf[50];
	_itoa((int)ltime,buf,10);
	std::string sCallId = std::string(buf);
	
	
	sSigUrl.append(sCallId);
	sSigUrl.append("&api_sig=");

	//计算api_sig

	m.insert(std::make_pair("api_key",_sAPI_KEY));
	m.insert(std::make_pair("method",sMethod));
	m.insert(std::make_pair("call_id",sCallId));

	for(auto iter = m.begin(); iter != m.end(); ++iter)
	{
		sValue.append(iter->first);
		sValue.append(iter->second);
	}
	sValue.append(_sAPI_SECRET);
	sMd5 = em_utility::md5_checksum::get_md5((const unsigned char*)sValue.c_str(),(unsigned int)sValue.length());
	assert(!sMd5.empty());
	sSigUrl.append(sMd5);
	return sSigUrl;
}

std::string Requester::GenSigUrlEX( const std::string& sMethod, const std::vector<std::pair<std::string,std::string> >& v)
{
	std::string sSigUrl;
	std::string sValue;
	std::string sMd5;

	assert(!sMethod.empty());
	assert(v.size());

	if(sMethod.empty())
		return sSigUrl;

	// 拼接url
	//sSigUrl = _sAPI_URL + "?";

	std::string sS,sAl,sAr;

	for (auto iter = v.begin(); iter != v.end(); ++iter)
	{
		std::string first  = iter->first;
		std::string second = iter->second;
		sSigUrl.append(URL::UrlEncodeEx(first));
		sSigUrl.append("=");
		sSigUrl.append(URL::UrlEncodeEx(second));
		sSigUrl.append("&");

		std::string::size_type n = first.find_first_of('[');
		if(n != std::string::npos)
		{
			std::string sType = first.substr(0,n);
			if (sType == "s")
			{
				sS.append(second);
				sS.append(",");
			} 
			else if(sType == "al")
			{
				sAl.append(second);
				sAl.append(",");
			}
			else if(sType == "ar")
			{
				sAr.append(second);
				sAr.append(",");
			}
		}
	}

	sS  = sS.substr(0, sS.length()-1);
	sAl = sAl.substr(0, sAl.length()-1);
	sAr = sAr.substr(0, sAr.length()-1);

	sSigUrl.append("api_key=");
	sSigUrl.append(_sAPI_KEY);
	sSigUrl.append("&method=");
	sSigUrl.append(sMethod);
	sSigUrl.append("&call_id=");

	time_t ltime;
	time(&ltime);
	char buf[50];
	_itoa((int)ltime,buf,10);
	std::string sCallId = std::string(buf);
	sSigUrl.append(sCallId);
	sSigUrl.append("&api_sig=");

	//计算api_sig
	std::map<std::string,std::string> m;

	m.insert(std::make_pair("s",sS));
	m.insert(std::make_pair("al",sAl));
	m.insert(std::make_pair("ar",sAr));
	m.insert(std::make_pair("api_key",_sAPI_KEY));
	m.insert(std::make_pair("method",sMethod));
	m.insert(std::make_pair("call_id",sCallId));
	m.insert(std::make_pair("type","1"));

	for(auto iter = m.begin(); iter != m.end(); ++iter)
	{
		sValue.append(iter->first);
		sValue.append(iter->second);
	}
	sValue.append(_sAPI_SECRET);
	sMd5 = em_utility::md5_checksum::get_md5((const unsigned char*)sValue.c_str(),(unsigned int)sValue.length());
	assert(!sMd5.empty());
	
	sSigUrl.append(sMd5);
	return sSigUrl;
}

std::string Requester::GenSigUrl(const CString& sMethod, const std::vector<std::pair<CString,CString> >& v)
{
	std::string strMethod = em_utility::mci::unicode_2_utf8(sMethod);

	std::vector<std::pair<std::string,std::string> > vParam;

	for (auto iter = v.begin();iter!=v.end();++iter)
	{
		std::string strKey,strValue;
		strKey =  em_utility::mci::unicode_2_utf8(iter->first);
		strValue =  em_utility::mci::unicode_2_utf8(iter->second);
		vParam.push_back(std::pair<std::string,std::string>(strKey,strValue));
	}

	return GenSigUrl(strMethod,vParam);

}

std::string Requester::GenSigUrlEX(const CString& sMethod, const std::vector<std::pair<CString,CString> >& v)
{
	std::string strMethod = em_utility::mci::unicode_2_utf8(sMethod);

	std::vector<std::pair<std::string,std::string> > vParam1;

	for (auto iter = v.begin();iter!=v.end();++iter)
	{
		std::string strKey,strValue;
		strKey =  em_utility::mci::unicode_2_utf8(iter->first);
		strValue =  em_utility::mci::unicode_2_utf8(iter->second);
		vParam1.push_back(std::pair<std::string,std::string>(strKey,strValue));
	}

	return GenSigUrlEX(strMethod,vParam1);

}

CRequest* Requester::MakeLogin(const UTF8STR& sUserName,const UTF8STR& sPasswordMd5)
{
	std::string sFunc("Members.getByEmail");
	std::vector<std::pair<std::string,std::string> > v;

	v.push_back(std::pair<std::string,std::string>("email",sUserName));
	v.push_back(std::pair<std::string,std::string>("pwd",sPasswordMd5));
	v.push_back(std::pair<std::string,std::string>("type","1"));


	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());
	return MakeRequestWithParams(WSRT_LOGIN,strUrl);
}
CRequest* Requester::MakeLogin(LPCTSTR pszUserName,LPCTSTR pszPasswordMd5)
{
	return MakeLogin(mci::unicode_2_utf8(pszUserName),mci::unicode_2_utf8(pszPasswordMd5));
}
CRequest* Requester::MakeLogin(const UTF8STR& sToken)
{
	std::string sFunc("Members.auth");
	std::vector<std::pair<std::string,std::string> > v;

	v.push_back(std::pair<std::string,std::string>("t",sToken));
	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());

	return MakeRequestWithParams(WSRT_WEBLOGIN,strUrl);
}
CRequest* Requester::MakeLogin(LPCTSTR pszToken)
{
	return MakeLogin(mci::unicode_2_utf8(pszToken));
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

//CRequest* Requester::MakeShare(uint nSendUserId,const UTF8STR& sCommenMsg,uint nObjId,emShareObjT objType)
//{
//	char * pStrObjType = NULL;
//	switch(objType)
//	{
//	case SOT_SONG: pStrObjType = "song"; break;
//	case SOT_ALBUM: pStrObjType = "album"; break;
//	case SOT_ARTIST: pStrObjType = "artist"; break;
//	case SOT_AMBUS: pStrObjType = "ambus"; break;
//	default: assert(false);break;
//	}
//	CStringA strUrl;
//	strUrl.Format("/app/xiating/share-post/content/%s/type/%s/objid/%d/uid/%d",sCommenMsg.c_str(),pStrObjType,nObjId,nSendUserId);
//
//	return MakeRequest(WSRT_SHARE,strUrl);
//}
CRequest* Requester::MakeShare(uint nSendUserId,LPCTSTR lpszCommentMsg,uint nObjId,emShareObjT objType)
{
	CString sFunc = _T("Share.post");
	std::vector<std::pair<CString,CString> > v;
	CString sUserId,sObjId,sType;
	sUserId.Format(_T("%d"),nSendUserId);
	sObjId.Format(_T("%d"),nObjId);
	sType.Format(_T("%d"),objType);

	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	v.push_back(std::pair<CString,CString>(_T("object_id"),sObjId));
	v.push_back(std::pair<CString,CString>(_T("type"),sType));
	v.push_back(std::pair<CString,CString>(_T("shareTo"),_T("all")));
	v.push_back(std::pair<CString,CString>(_T("message"),lpszCommentMsg));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());
	return MakeRequestWithParamsTag(WSRT_SHARE,strUrl,nSendUserId);
	//return MakeShare(nSendUserId,string::UnicodeToUTF8(lpszCommentMsg),nObjId,objType);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CRequest* Requester::MakePlayLog(uint nUserId,uint nSongId)
{
	CString sFunc = _T("Playlog.add");
	std::vector<std::pair<CString,CString> > v;
	CString sUserId,sSongId;
	sUserId.Format(_T("%d"),nUserId);
	sSongId.Format(_T("%d"),nSongId);
	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	v.push_back(std::pair<CString,CString>(_T("id"),sSongId));
	v.push_back(std::pair<CString,CString>(_T("type"),_T("20")));//虾听编号为20;
	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());

	return MakeRequestWithParams(WSRT_PLAYLOG,strUrl);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CRequest* Requester::MakeGetList(uint nUserId)
{
	CString sFunc = _T("Xt_Playlists.get");
	std::vector<std::pair<CString,CString> > v;
	CString sUserId;
	sUserId.Format(_T("%d"),nUserId);
	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());

	return MakeRequestWithParamsTag(WSRT_GET_LIST,strUrl,nUserId);
}

CRequest* Requester::MakeNewList(uint nUserId,uint nListId,const CString& sName,const CString& sSongs,bool bShow)
{
	CString sFunc = _T("Xt_Playlists.add");
	std::vector<std::pair<CString,CString> > v;

	CString sUserId,sShow;
	sUserId.Format(_T("%d"),nUserId);
	sShow = bShow ? _T("1") : _T("0") ;

	assert(!sName.IsEmpty());
	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	v.push_back(std::pair<CString,CString>(_T("name"),sName));

	if (!sSongs.IsEmpty())
	{
		v.push_back(std::pair<CString,CString>(_T("ids"),sSongs));
	}
	v.push_back(std::pair<CString,CString>(_T("is_show"),sShow));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());

	return MakeRequestWithParamsTag(WSRT_NEW_LIST,strUrl,nListId);
}
CRequest* Requester::MakeNewList(uint nUserId,uint nListId,const CString& sName,const std::vector<spSongInfoT>& songs,bool bShow)
{
	CString sSongArray;
	if (songs.size()>0)
	{	
		for (auto it = songs.begin();it!=songs.end();++it)
		{
			spSongInfoT spSong = *it;
			if (spSong->GetSongId()>0)
			{
				CString sSong;
				sSong.Format(_T("(%d,%d)"),CFavInfo::ft_song,spSong->GetSongId());
				sSongArray += sSong;
			}			
		}
	}
	return MakeNewList(nUserId,nListId,sName,sSongArray,bShow);
}

CRequest* Requester::MakeDelList(uint nUserId,uint nListId,bool bAll)
{
	CString sFunc = _T("Xt_Playlists.del");
	std::vector<std::pair<CString,CString> > v;

	CString sUserId,sListWebId,sAll;
	sUserId.Format(_T("%d"),nUserId);
	sListWebId.Format(_T("%d"),nListId);
	sAll = bAll ? _T("1"):_T("0");

	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	v.push_back(std::pair<CString,CString>(_T("id"),sListWebId));
	v.push_back(std::pair<CString,CString>(_T("type"),sAll));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());

	return MakeRequestWithParams(WSRT_DEL_LIST,strUrl);
}
CRequest* Requester::MakeReNameList(uint nUserId,uint nListId,const CString& sName)
{
	CString sFunc = _T("Xt_Playlists.updateName");
	std::vector<std::pair<CString,CString> > v;

	CString sUserId,sListWebId;
	sUserId.Format(_T("%d"),nUserId);
	sListWebId.Format(_T("%d"),nListId);
	assert(!sName.IsEmpty());

	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	v.push_back(std::pair<CString,CString>(_T("id"),sListWebId));
	v.push_back(std::pair<CString,CString>(_T("name"),sName));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());

	return MakeRequestWithParams(WSRT_RENAME_LIST,strUrl);
}
CRequest* Requester::MakeChangeListStatus(uint nUserId,uint nListId,bool bShow)
{
	CString sFunc = _T("Xt_Playlists.updateStatus");
	std::vector<std::pair<CString,CString> > v;

	CString sUserId,sListWebId,sShow;
	sUserId.Format(_T("%d"),nUserId);
	sListWebId.Format(_T("%d"),nListId);
	sShow = bShow ? _T("1"):_T("0");

	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	v.push_back(std::pair<CString,CString>(_T("id"),sListWebId));
	v.push_back(std::pair<CString,CString>(_T("is_show"),sShow));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());

	return MakeRequestWithParams(WSRT_CHANGE_STATUS,strUrl);
}
CRequest* Requester::MakeGetSongsFromList(uint nUserId,uint nListId,uint nPage,uint nType)
{
	CString sFunc = _T("Xt_Playlists.details");
	std::vector<std::pair<CString,CString> > v;

	CString sUserId,sListWebId,sPage,sType;
	sUserId.Format(_T("%d"),nUserId);
	sListWebId.Format(_T("%d"),nListId);
	//sLimit.Format(_T("%d"),nType==CFavInfo::ft_song ? )
	sPage.Format(_T("%d"),nPage);
	sType.Format(_T("%d"),nType);

	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	v.push_back(std::pair<CString,CString>(_T("id"),sListWebId));
	v.push_back(std::pair<CString,CString>(_T("limit"),_T("100")));
	v.push_back(std::pair<CString,CString>(_T("page"),sPage));
	v.push_back(std::pair<CString,CString>(_T("type"),sType));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());

	return MakeRequestWithParams(WSRT_LIST_GET_SONGS,strUrl);
}
CRequest* Requester::MakeAddSongToList(uint nUserId,uint nListId,const CString& sSongs)
{
	CString sFunc = _T("Xt_Playlists.addDetails");
	std::vector<std::pair<CString,CString> > v;

	CString sUserId,sListWebId;
	sUserId.Format(_T("%d"),nUserId);
	sListWebId.Format(_T("%d"),nListId);
	//assert(!sName.IsEmpty());
	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	v.push_back(std::pair<CString,CString>(_T("id"),sListWebId));

	v.push_back(std::pair<CString,CString>(_T("ids"),sSongs));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());

	return MakeRequestWithParamsTag(WSRT_LIST_ADD_SONGS,strUrl,nListId);

}
CRequest* Requester::MakeDelSongFromList(uint nUserId,uint nListId,const CString& sSongs)
{
	CString sFunc = _T("Xt_Playlists.delDetails");
	std::vector<std::pair<CString,CString> > v;

	CString sUserId,sListWebId;
	sUserId.Format(_T("%d"),nUserId);
	sListWebId.Format(_T("%d"),nListId);
	//assert(!sName.IsEmpty());
	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	v.push_back(std::pair<CString,CString>(_T("id"),sListWebId));
	v.push_back(std::pair<CString,CString>(_T("ids"),sSongs));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());

	return MakeRequestWithParamsTag(WSRT_LIST_DEL_SONGS,strUrl,nListId);
}
CRequest* Requester::MakeClearList(uint nUserId,uint nListId)
{
	CStringA strUrl;

	return MakeRequestWithParams(WSRT_LIST_CLEAR,strUrl);
}
CRequest* Requester::MakeGetRoamingSongs(uint nSongId,uint nListId)
{
	CString sFunc = _T("Songs.roaming");
	std::vector<std::pair<CString,CString> > v;
	CString sSongId;
	sSongId.Format(_T("%d"),nSongId);

	v.push_back(std::pair<CString,CString>(_T("id"),sSongId));


	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());
	return MakeRequestWithParamsTag(WSRT_GET_ROAMING,strUrl,nListId);
}

CRequest* Requester::MakeTagList(bool bLogin,const CString& sData,uint nCount)
{
	assert(nCount > 0);

	CString sFunc = _T("Xt_Playlists.sysLists");
	std::vector<std::pair<CString,CString> > v;
	CString sType,sCount;
	sType = (bLogin ? _T("1") : _T("2"));
	sCount.Format(_T("%d"),nCount);

	v.push_back(std::pair<CString,CString>(_T("type"),sType));
	v.push_back(std::pair<CString,CString>(_T("count"),sCount));
	v.push_back(std::pair<CString,CString>(_T("id"),sData));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());
	return MakeRequestWithParams(WSRT_GET_TAG_LIST,strUrl);
}

CRequest* Requester::MakeTagList(bool bLogin,const std::vector<std::pair<int,int> >& DataArray,uint nCount)
{
	CString sData;
	for (auto it=DataArray.begin();it!=DataArray.end();++it)
	{
		CString sTemp;
		sTemp.Format(_T("(%d,0,%d)"),it->first,it->second);

		sData = sData + sTemp;
	}

	return MakeTagList(bLogin,sData,nCount);
}

CRequest* Requester::MakeGetTagListSongs(uint nType,uint nListWebId,uint nUserId)
{
	CString sFunc = _T("Radios.songs");
	std::vector<std::pair<CString,CString> > v;

	CString sWebId,sType,sLimit = _T("50");
	sWebId.Format(_T("%d"),nListWebId);
	sType.Format(_T("%d"),nType);
	v.push_back(std::pair<CString,CString>(_T("type"),sType));
	v.push_back(std::pair<CString,CString>(_T("oid"),sWebId));
	v.push_back(std::pair<CString,CString>(_T("limit"),sLimit));

	if (nUserId > 0)
	{
		CString sUserId;
		sUserId.Format(_T("%d"),nUserId);
		v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	}

	v.push_back(std::pair<CString,CString>(_T("api"),_T("xiating")));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());
	return MakeRequestWithParams(WSRT_GET_TAG_SONGS,strUrl);
}
CRequest* Requester::MakeMatchSong(std::vector<spSongInfoT> songs)
{
	assert(!songs.empty());
	typedef std::pair<CString,CString> strKeyValueT;
	const CString sFunc = _T("Songs.matchSong");
	std::vector<strKeyValueT > v;

	for (std::size_t i = 0;i < songs.size();++i)
	{
		CString sIndex;
		sIndex.Format(_T("[%d]"),i);
		CString sKey(_T("s"));
		sKey += sIndex;
		v.push_back(strKeyValueT(sKey,songs[i]->GetSongName()));
		if(!songs[i]->GetAlbumName().IsEmpty() || songs[i]->GetAlbumName()!=UnKnown)
		{
			sKey = _T("al");
			sKey += sIndex;
			v.push_back(strKeyValueT(sKey,songs[i]->GetAlbumName()));
		}
		if(!songs[i]->GetArtistName().IsEmpty() || songs[i]->GetArtistName()!=UnKnown)
		{
			sKey = _T("ar");
			sKey += sIndex;
			v.push_back(strKeyValueT(sKey,songs[i]->GetArtistName()));
		}
	}

	v.push_back(std::pair<CString,CString>(_T("type"),_T("1")));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrlEX(sFunc,v).c_str());
	return MakeRequestWithParams(WSRT_MATCH_SONG,strUrl);
}
//收藏数据;
CRequest* Requester::MakeGetFavData(uint nUserId,uint nPageIndex,uint nType)
{
	//目前只取单曲数据;
	//assert(nType == CFavInfo::ft_song);
	CString sFunc = _T("Library.getLibrary");
	std::vector<std::pair<CString,CString> > v;
	CString sUserId,sType,sPage,sPageSize=_T("800");
	sUserId.Format(_T("%d"),nUserId);
	sPage.Format(_T("%d"),nPageIndex);
	sType.Format(_T("%d"),nType);

	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	v.push_back(std::pair<CString,CString>(_T("type"),sType));
	v.push_back(std::pair<CString,CString>(_T("limit"),sPageSize));
	v.push_back(std::pair<CString,CString>(_T("page"),sPage));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());
	return MakeRequestWithParamsTag(WSRT_FAVORITE,strUrl,nType);
}

CRequest* Requester::MakeAddFav(uint nUserId,uint nId,CFavInfo::FavType eType)
{
	CStringA strUrl;
	assert(eType > 0);
	CString sFunc,sUserId,sId,sGrade(_T("0"));
	std::vector<std::pair<CString,CString> > v;
	switch(eType)
	{
	case CFavInfo::ft_song:
		sFunc = _T("Library.addSong");
		break;
	case CFavInfo::ft_album:
		sFunc = _T("Library.addAlbum");
		break;
	case CFavInfo::ft_artist:
		sFunc = _T("Library.addArtist");
		break;
	case CFavInfo::ft_collect:
		sFunc = _T("Library.addCollect");
		break;	
	}
	sUserId.Format(_T("%d"),nUserId);
	sId.Format(_T("%d"),nId);

	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	v.push_back(std::pair<CString,CString>(_T("id"),sId));

	if (eType == CFavInfo::ft_song)
	{
		v.push_back(std::pair<CString,CString>(_T("grade"),sGrade));
	}

	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());

	//strUrl.Format("/app/xiating/fav/id/%d/type/%d",nId,nType);
	return MakeRequestWithParams(WSRT_ADD_FAVORITE,strUrl);
}

CRequest* Requester::MakeDelFav(uint nUserId,uint nId,CFavInfo::FavType eType)
{
	CStringA strUrl;
	assert(eType > 0);
	CString sFunc,sUserId,sId;
	std::vector<std::pair<CString,CString> > v;
	switch(eType)
	{
	case CFavInfo::ft_song:
		sFunc = _T("Library.removeSong");
		break;
	case CFavInfo::ft_album:
		sFunc = _T("Library.removeAlbum");
		break;
	case CFavInfo::ft_artist:
		sFunc = _T("Library.removeArtist");
		break;
	case CFavInfo::ft_collect:
		sFunc = _T("Library.removeCollect");
		break;	
	}
	sUserId.Format(_T("%d"),nUserId);
	sId.Format(_T("%d"),nId);

	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));
	v.push_back(std::pair<CString,CString>(_T("id"),sId));

	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());
	return MakeRequestWithParams(WSRT_DEL_FAVORITE,strUrl);
}

CRequest* Requester::MakeDetailInfo(const CString& sIdArray,CFavInfo::FavType eType)
{
	assert(!sIdArray.IsEmpty());	
	CString sType,sFunc = _T("Xt_Playlists.playlist");
	std::vector<std::pair<CString,CString> > v;
	sType.Format(_T("%d"),eType);

	v.push_back(std::pair<CString,CString>(_T("id"),sIdArray));
	v.push_back(std::pair<CString,CString>(_T("type"),sType));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());
	return MakeRequestWithParams(WSRT_DETAIL_INFO,strUrl);
}

CRequest* Requester::MakeDetailInfo(const std::vector<uint>& IdArray,CFavInfo::FavType eType)
{
	assert(!IdArray.empty());
	CString sIdArray;
	for (auto it=IdArray.begin();it!=IdArray.end();++it)
	{
		CString sId;
		sId.Format(_T("%d,"),*it);
		sIdArray = sIdArray + sId;
	}

	sIdArray = sIdArray.Left(sIdArray.GetLength()-1);

	return MakeDetailInfo(sIdArray,eType);
}
CRequest* Requester::MakeMusicPacketDetailInfo(uint nUserId,uint nId,uint nType,uint nSonglimit,uint nSongPage,uint nLimit,uint nPage)
{
	typedef std::pair<CString,CString> strKeyValueT;
	std::vector<strKeyValueT> v;
	CString sTmp;
	sTmp.Format(_T("%u"),nUserId),    v.push_back(strKeyValueT(_T("user_id"),sTmp));
	sTmp.Format(_T("%u"),nId),        v.push_back(strKeyValueT(_T("id"),sTmp));
	sTmp.Format(_T("%u"),nType),      v.push_back(strKeyValueT(_T("type"),sTmp));
	sTmp.Format(_T("%u"),nSonglimit), v.push_back(strKeyValueT(_T("limit1"),sTmp));
	sTmp.Format(_T("%u"),nSongPage),  v.push_back(strKeyValueT(_T("page1"),sTmp));
	sTmp.Format(_T("%u"),nLimit),     v.push_back(strKeyValueT(_T("limit2"),sTmp));
	sTmp.Format(_T("%u"),nPage),      v.push_back(strKeyValueT(_T("page2"),sTmp));

	CStringA sparam;
	sparam.Format("%s",GenSigUrl(_T("Xt_Playlists.getListDetail"),v).c_str());
	return MakeRequestWithParams(WSRT_NULL,sparam);
}
//乐库推荐数据;
CRequest* Requester::MakeGetUnloginRecommendSongs(const CString& sPlayRecordData,uint nRetCount)
{
	CString sFunc = _T("Recommend.xtUnloginSongs");
	std::vector<std::pair<CString,CString> > v;

	CString sRetCount;
	sRetCount.Format(_T("%d"),nRetCount);
	v.push_back(std::pair<CString,CString>(_T("count"),sRetCount));
	v.push_back(std::pair<CString,CString>(_T("ids"),sPlayRecordData));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());
	return MakeRequestWithParams(WSRT_LIB_RECOMMEND_SONGS,strUrl);
}
CRequest* Requester::MakeGetLoginRecommendSongs(uint nUserId,uint nRetCount)
{
	CString sUserId,sFunc = _T("Recommend.xtLoginSongs");
	std::vector<std::pair<CString,CString> > v;
	sUserId.Format(_T("%d"),nUserId);
	CString sRetCount;
	sRetCount.Format(_T("%d"),nRetCount);
	v.push_back(std::pair<CString,CString>(_T("count"),sRetCount));
	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());
	return MakeRequestWithParams(WSRT_LIB_RECOMMEND_SONGS,strUrl);
}

CRequest* Requester::MakeGetUnloginRecommendAlbums(const CString& sPlayRecordData,uint nRetCount)
{
	CString sUserId,sFunc = _T("Recommend.getUnloginAlbums");
	std::vector<std::pair<CString,CString> > v;
	CString sRetCount;
	sRetCount.Format(_T("%d"),nRetCount);
	v.push_back(std::pair<CString,CString>(_T("count"),sRetCount));
	v.push_back(std::pair<CString,CString>(_T("ids"),sPlayRecordData));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());
	return MakeRequestWithParams(WSRT_LIB_RECOMMEND_ALBUMS,strUrl);
}
CRequest* Requester::MakeGetLoginRecommendAlbums(uint nUserId,uint nRetCount)
{
	CString sUserId,sFunc = _T("Recommend.getSoftAlbums");
	std::vector<std::pair<CString,CString> > v;
	sUserId.Format(_T("%d"),nUserId);
	CString sRetCount;
	sRetCount.Format(_T("%d"),nRetCount);
	v.push_back(std::pair<CString,CString>(_T("count"),sRetCount));
	v.push_back(std::pair<CString,CString>(_T("user_id"),sUserId));

	CStringA strUrl;
	strUrl.Format("%s", GenSigUrl(sFunc,v).c_str());
	return MakeRequestWithParams(WSRT_LIB_RECOMMEND_ALBUMS,strUrl);
}

}

