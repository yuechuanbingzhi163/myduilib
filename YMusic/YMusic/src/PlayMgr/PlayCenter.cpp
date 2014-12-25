#include "stdafx.h"
#include "PlayCenter.h"
#include <iterator>
#include <cassert>
#include <ctime>
#include <cmath>
#include "../YApp.h"
#include "audioinfo.h"
#include "../Utilty/lock/RzLock.h"

CPlayCenter::CPlayCenter(void)
	: YPlayCenter()
{
	m_iCurPlayingSongIndex = 0;
	m_iCurPlayingListIndex = 0;

	SetCallBackFunc(&CPlayCenter::CallBackFunc);
}


CPlayCenter::~CPlayCenter(void)
{
}
void CPlayCenter::Play()
{
	if(m_spPlayList.size()<=0) return ;
	auto it = m_spPlayList.begin();
	if(m_iCurPlayingListIndex<=0) m_iCurPlayingListIndex = it->second.GetLocalId();

	it = m_spPlayList.find(m_iCurPlayingListIndex);
	if(it==m_spPlayList.end()) return ;

	auto itSong = m_spSongListMap.find(it->second.GetLocalId());
	if(itSong==m_spSongListMap.end()) return ;

	if(itSong->second.size()<=0) return ;
	if(m_iCurPlayingSongIndex<=0) {
		auto itSongIndex = itSong->second.begin();
		m_iCurPlayingSongIndex = itSongIndex->GetLocalId();
	}
		
	CString path = GetPathBySongLocalId(itSong->second,m_iCurPlayingSongIndex);
	if (!path.IsEmpty())
	{
		this->PlayThis(path);

		spSongInfoT song = GetCurrentPlayingSong();
		song->SetPlayCount(song->GetPlayCount()+1);
		theDbMgr->GetUserDB()->UpdatePlayListSongInfo(song,GetListIndex());
	}
}
void CPlayCenter::Next()
{
	if(m_spPlayList.size()<=0) return ;
	if(m_iCurPlayingListIndex<=0) m_iCurPlayingListIndex = m_spPlayList.begin()->second.GetLocalId();
	auto it = m_spPlayList.find(m_iCurPlayingListIndex);
	if(it==m_spPlayList.end()) return ;
	auto itSong = m_spSongListMap.find(it->second.GetLocalId());
	if(itSong==m_spSongListMap.end()) return ;
	if(itSong->second.size()<=0) return ;

	this->MakeNextPlayIndex(m_iCurPlayingListIndex);
	bool bFind = false;
	do 
	{
		for (auto itFind=itSong->second.begin();itFind!=itSong->second.end();++itFind)
		{
			if (itFind->GetLocalId()==m_iCurPlayingSongIndex)
			{
				bFind = true;
				break;
			}
		}
		if(!bFind)MakeNextPlayIndex(m_iCurPlayingListIndex);
	} while (!bFind);
	this->Play();
}
void CPlayCenter::Prev()
{
	if(m_spPlayList.size()<=0) return ;
	if(m_iCurPlayingListIndex<=0) m_iCurPlayingListIndex = m_spPlayList.begin()->second.GetLocalId();
	auto it = m_spPlayList.find(m_iCurPlayingListIndex);
	if(it==m_spPlayList.end()) return ;
	auto itSong = m_spSongListMap.find(it->second.GetLocalId());
	if(itSong==m_spSongListMap.end()) return ;
	if(itSong->second.size()<=0) return ;

	this->MakePrevPlayIndex(m_iCurPlayingListIndex);
	bool bFind = false;
	do 
	{
		for (auto itFind=itSong->second.begin();itFind!=itSong->second.end();++itFind)
		{
			if (itFind->GetLocalId()==m_iCurPlayingSongIndex)
			{
				bFind = true;
				break;
			}
		}
		if(!bFind)MakePrevPlayIndex(m_iCurPlayingListIndex);
	} while (!bFind);
	
	this->Play();
}
void CPlayCenter::AddPlayList(CPlayListInfo spList)
{
	m_spPlayList.insert(std::pair<int,CPlayListInfo>(spList.GetLocalId(),spList));
	songsArrayT songArray;
	m_spSongListMap.insert(std::pair<int,songsArrayT>(spList.GetLocalId(),songArray));
}
void CPlayCenter::RenamePlayList(unsigned int u_listIndex,CString strNewListName)
{
	auto it = m_spPlayList.find(u_listIndex);
	if(it==m_spPlayList.end()) return ;
	spPlayListInfoT spList;
	theDbMgr->GetUserDB()->GetPlayList(spList,u_listIndex);
	if(spList)
	{
		spList->SetComment(strNewListName);
		spList->SetName(strNewListName);

		theDbMgr->GetUserDB()->UpdatePlayList(spList);
	}
}
void CPlayCenter::RemovePlayList(unsigned int u_listIndex)
{
	auto it = m_spPlayList.find(u_listIndex);
	if(it!=m_spPlayList.end())
	{
		if(u_listIndex==m_iCurPlayingListIndex)
		{
			this->Stop();
			m_iCurPlayingListIndex = -1;
			m_iCurPlayingSongIndex = -1;
		}
		RemoveAllFromList(u_listIndex);
		m_spPlayList.erase(it);
	}
}
void CPlayCenter::RemoveAllList()
{
	for (auto it=m_spPlayList.begin();it!=m_spPlayList.end();++it)
	{
		RemoveAllFromList(it->second.GetLocalId());
	}
	m_spPlayList.clear();
}
void CPlayCenter::AddSongToList(spSongInfoT songT,unsigned int u_listIndex)
{
	ASSERT(songT);
	CSongInfo song;
	song = *songT;
	return AddSongToList(song,u_listIndex);
}
void CPlayCenter::AddSongToList(CSongInfo spSong,unsigned int u_listIndex)
{
	auto it = m_spSongListMap.find(u_listIndex);
	if (it!=m_spSongListMap.end())
	{
		it->second.push_back(spSong);
		m_songIdArray.push_back(spSong.GetLocalId());
	}
}
void CPlayCenter::AddSongToList(CPlayCenter::songsArrayT songArray,unsigned int u_listIndex)
{
	auto it = m_spSongListMap.find(u_listIndex);
	if (it!=m_spSongListMap.end())
	{
		for (songsArrayIterT it2=songArray.begin();it2!=songArray.end();++it2)
		{
			it->second.push_back(*it2);
			m_songIdArray.push_back(it2->GetLocalId());
		}
	}
}
// void CPlayCenter::AddSongToList(CPlayCenter::pathArrayT pathArray,unsigned int u_listIndex)
// {
// 	for (pathArrayIterT it=pathArray.begin();it!=pathArray.end();++it)
// 	{
// 		AddSongToList(*it,u_listIndex);
// 	}
// }
// void CPlayCenter::AddSongToList(const wchar_t* filepath,unsigned int u_listIndex)
// {
// 	CSongInfo spSong;
// 	TID3InfoExW tid3;
// 	ZeroMemory(&tid3,sizeof(tid3));
// 	spSong.SetLocalPath(filepath);
// 	spSong.SetLocal(true);
// 	spSong.SetAddTime(mci::now_time());
// 	spSong.SetFileSize(mci::get_file_size(filepath));
// 	spSong.SetFileStartPos(0);
// 	if(LoadFileInfo2(filepath,&tid3))
// 	{	
// 		spSong.SetArtistName((tid3.Artist));
// 		spSong.SetSongName(tid3.Title);
// 		spSong.SetAblumName(tid3.Album);
// 		spSong.SetFileExt(mci::rip_file_ext(filepath,true));
// 		spSong.SetRealArtistName(tid3.AlbumArtist);	
// 		spSong.SetEnable(true);
// 	}
// 	AddSongToList(spSong,u_listIndex);
// }

void CPlayCenter::RemoveSongFromList(unsigned int u_index,unsigned int u_listIndex)
{
	auto it = m_spSongListMap.find(u_listIndex);
	if (it!=m_spSongListMap.end())
	{
		songsArrayT& spSong = it->second;
		int i=0;
		for (songsArrayIterT iter=spSong.begin();iter!=spSong.end();++iter)
		{
			if (iter->GetLocalId()==u_index)
			{
				m_songIdArray.erase(m_songIdArray.begin()+i);
				spSong.erase(iter);
				break;
			}
			++i;
		}
	}
}
void CPlayCenter::RemoveAllFromList(unsigned int u_listIndex)
{
	auto it = m_spSongListMap.find(u_listIndex);
	if (it!=m_spSongListMap.end())
		m_spSongListMap.erase(it),m_songIdArray.clear();
}

void  CPlayCenter::GetAllPlayList() 
{
	RemoveAllList();

	if (theRuntimeState->GetLogUserInfo())
	{
		theDbMgr->SetCurrentUser(theRuntimeState->GetLogUserInfo());
	}
	{
		std::vector<spPlayListInfoT> PlayList;
		theDbMgr->GetUserDB()->GetAllPlayLists(PlayList);
		for (auto it=PlayList.begin();it!=PlayList.end();++it)
		{
			CPlayListInfo playList;
			playList.SetComment((*it)->GetComment());
			playList.SetFlag((*it)->GetFlag());
			playList.SetId((*it)->GetId());
			playList.SetListType((*it)->GetListType());
			playList.SetLocalId((*it)->GetLocalId());
			playList.SetName((*it)->GetName());
			playList.SetShow((*it)->IsShow());
			playList.SetTag((*it)->GetTag());

			AddPlayList(playList);
			std::vector<spSongInfoT> SongList;
			theDbMgr->GetUserDB()->GetPlayListSongs(SongList,(*it)->GetLocalId());
			for (auto it2=SongList.begin();it2!=SongList.end();++it2)
			{
				CSongInfo songInfo;
				songInfo.SetAblumName((*it2)->GetAlbumName());
				songInfo.SetAddTime((*it2)->GetAddTime());
				songInfo.SetAlbumId((*it2)->GetAlbumId());
				songInfo.SetArtistId((*it2)->GetArtistId());
				songInfo.SetArtistName((*it2)->GetArtistName());
				songInfo.SetAudioLength((*it2)->GetAudioLength());
				songInfo.SetCoverLocalPath((*it2)->GetCoverLocalPath());
				songInfo.SetCoverUrl((*it2)->GetCoverUrl());
				songInfo.SetEnable((*it2)->IsEnable());
				songInfo.SetFileExt((*it2)->GetFileExt());
				songInfo.SetFileSize((*it2)->GetFileSize());
				songInfo.SetFileStartPos((*it2)->GetFileStartPos());
				songInfo.SetFileType((*it2)->GetFileType());
				songInfo.SetFlag((*it2)->GetFlag());
				songInfo.SetLastModifyTime((*it2)->GetLastModifyTime());
				songInfo.SetLastPlayTime((*it2)->GetLastPlayTime());
				songInfo.SetListenFileUrl((*it2)->GetListenFileUrl());
				songInfo.SetLocal((*it2)->IsLocal());
				songInfo.SetLocalId((*it2)->GetLocalId());
				songInfo.SetLocalPath((*it2)->GetLocalPath());
				songInfo.SetLrcLocalPath((*it2)->GetLrcLocalPath());
				songInfo.SetLrcUrl((*it2)->GetLrcUrl());
				songInfo.SetMD5((*it2)->GetMD5());
				songInfo.SetPlayCount((*it2)->GetPlayCount());
				songInfo.SetRealArtistName((*it2)->GetRealArtistName());
				songInfo.SetResourceID((*it2)->GetResourceId());
				songInfo.SetSongId((*it2)->GetSongId());
				songInfo.SetSongName((*it2)->GetSongName());
				songInfo.SetTag((*it2)->GetTag());
				AddSongToList(songInfo,(*it)->GetLocalId());
			}
		}
		
	}
}
void CPlayCenter::PlayListChanged()
{

}
CPlayCenter::playListMapT CPlayCenter::LoadAllPlayList()
{
	return m_spPlayList;
}
CPlayCenter::playListMapT CPlayCenter::LoadAllPlayListByType(CPlayListInfo::PlayListType listType)
{
	playListMapT listMap;
	playListMapT::iterator iter = m_spPlayList.begin();
	for (;iter!=m_spPlayList.end();++iter)
	{
		if (iter->second.GetListType()==listType)
		{
			listMap.insert(std::pair<int,CPlayListInfo>(iter->first,iter->second));
		}
	}
	return listMap;
}
CPlayCenter::songsArrayT CPlayCenter::LoadAllSongList(unsigned int u_listIndex)
{
	std::map<int,songsArrayT>::iterator iter = m_spSongListMap.find(u_listIndex);
	if (iter!=m_spSongListMap.end())
	{
		return iter->second;
	}
	return songsArrayT();
}
void CPlayCenter::LoadFileInfo(ID3InfoW* fileinfo)
{
// 	if(NULL == m_PlayCenter) return;
// 	TID3InfoExW id3;
// 	ZeroMemory(&id3,sizeof(id3));
// 	LoadFileInfo2(m_spCurPlayingSong.GetLocalPath(),&id3);
// 	TStreamInfo pInfo;
// 	m_PlayCenter->GetStreamInfo(&pInfo);
// 
// 	fileinfo->time_ = pInfo.Length.hms.hour*60*60 + pInfo.Length.hms.minute*60 + pInfo.Length.hms.second;
// 	fileinfo->tid3_ = id3;
}

int CPlayCenter::MakeNextPlayIndex(unsigned int u_listIndex)
{
	size_t size_count = 0;
	auto it = m_spSongListMap.find(u_listIndex);
	if(it!=m_spSongListMap.end())
		size_count = it->second.size();
	if(size_count==0)
	{
		m_iCurPlayingSongIndex = -1;
		return -1;
	}
	songsArrayIterT iterFirst = it->second.begin();
	songsArrayIterT iterLast = it->second.begin()+(size_count-1);
	unsigned int iFirst = iterFirst->GetLocalId();
	unsigned int iLast = iterLast->GetLocalId();
	switch (GetPlayMode())
	{
	case EM_MODE::EM_LIST_ORDER://顺序播放
		{
			m_iCurPlayingSongIndex ++;
			if(m_iCurPlayingSongIndex>iLast)
				m_iCurPlayingSongIndex = iLast;
			return -1;
		}
		break;
	case EM_MODE::EM_LIST_LOOP://列表循环
		{
			m_iCurPlayingSongIndex ++;
			if(m_iCurPlayingSongIndex > iLast)
				m_iCurPlayingSongIndex = iFirst;
			return 0;
		}
		break;
	case EM_MODE::EM_SINGLE:
		{
			return -1;
		}
		break;
	case EM_MODE::EM_SINGLE_LOOP:
		return 0;
		break;
	case EM_MODE::EM_RAMDON:
		{
			srand((unsigned int)time(NULL));
			if(iLast-iFirst==0)
				m_iCurPlayingSongIndex = iFirst;
			else 
				m_iCurPlayingSongIndex = iFirst + rand()%(iLast-iFirst+1);
			return 0;
		}
		break;
	default:
		assert(FALSE);
		return -1;
	}
}

int CPlayCenter::MakePrevPlayIndex(unsigned int u_listIndex)
{
	size_t size_count = 0;
	auto it = m_spSongListMap.find(u_listIndex);
	if(it!=m_spSongListMap.end())
		size_count = it->second.size();
	if(size_count==0)
	{
		m_iCurPlayingSongIndex = -1;
		return -1;
	}
	songsArrayIterT iterFirst = it->second.begin();
	songsArrayIterT iterLast = it->second.begin()+(size_count-1);
	unsigned int iFirst = iterFirst->GetLocalId();
	unsigned int iLast = iterLast->GetLocalId();
	switch (GetPlayMode())
	{
	case EM_MODE::EM_LIST_ORDER:
		{
			m_iCurPlayingSongIndex --;
			if(m_iCurPlayingSongIndex<iFirst)
				m_iCurPlayingSongIndex = iFirst;
			return -1;
		}
		break;
	case EM_MODE::EM_LIST_LOOP:
		{
			m_iCurPlayingSongIndex --;
			if(m_iCurPlayingSongIndex<iFirst)
				m_iCurPlayingSongIndex = iFirst;
			return 0;
		}
		break;
	case EM_MODE::EM_SINGLE:
		{
			return -1;
		}
		break;
	case EM_MODE::EM_SINGLE_LOOP:
		return 0;
		break;
	case EM_MODE::EM_RAMDON:
		{

			srand((unsigned int)time(NULL));
			if(iLast-iFirst==0)
				m_iCurPlayingSongIndex = iFirst;
			else 
				m_iCurPlayingSongIndex = iFirst + rand()%(iLast-iFirst+1);
			return 0;
		}
		break;
	default:
		assert(FALSE);
		return -1;
	}
}
spSongInfoT CPlayCenter::ParaseFromPath(CString sPath)
{
	CSongInfo spSong;
	TStreamFormat tf = GetFileFormat(sPath);
	if (tf==TStreamFormat::sfUnknown)
	{
//		byte bb[128]={0};
//		YTing::CMp3FileInfo::Mp3Info mp3 = YTing::CMp3FileInfo::getMp3Info(sPath,bb);
	}
	
	TID3InfoExW tid3;
	ZeroMemory(&tid3,sizeof(tid3));
	spSong.SetLocalPath(sPath);
	spSong.SetLocal(true);
	spSong.SetAddTime(mci::now_time());
	spSong.SetFileSize(mci::get_file_size(sPath));
	spSong.SetFileStartPos(0);
	if(LoadFileInfo2(sPath,&tid3))
	{	
		spSong.SetArtistName((tid3.Artist));
		spSong.SetSongName(tid3.Title);
		spSong.SetAblumName(tid3.Album);
		spSong.SetFileExt(mci::rip_file_ext(sPath,true));
		spSong.SetRealArtistName(tid3.AlbumArtist);		
		spSong.SetEnable(true);		
	}
	spSongInfoT spSongInfo = spSongInfoT(new CSongInfo(spSong));
	CheckSong(spSongInfo);
	return spSongInfo;
}
void CPlayCenter::CheckSong(spSongInfoT& spSong)
{
	assert(spSong);
	if(!spSong) return;
	spSong->SetAblumName(spSong->GetAlbumName().IsEmpty()?_T("未知"):spSong->GetAlbumName());
	spSong->SetSongName(spSong->GetSongName().IsEmpty()?mci::rip_file_name(spSong->GetLocalPath(),false):spSong->GetSongName());
	spSong->SetArtistName(spSong->GetArtistName().IsEmpty()?_T("未知"):spSong->GetArtistName());
	spSong->SetRealArtistName(spSong->GetRealArtistName().IsEmpty()?_T("未知"):spSong->GetRealArtistName());

}
CString CPlayCenter::GetPathBySongLocalId(songsArrayT songArray,unsigned int u_index)
{
	for (songsArrayIterT iter=songArray.begin();iter!=songArray.end();++iter)
	{
		if(iter->GetLocalId()==u_index)
		{
			CString path = iter->GetLocalPath();

			return path;
		}
	}

	return _T("");
}
spSongInfoT CPlayCenter::GetCurrentPlayingSong()
{
	spSongInfoT spSongInfo;
	if(m_spPlayList.size()==0||m_spSongListMap.size()==0) return spSongInfo;
	if(m_spPlayList.find(m_iCurPlayingListIndex)==m_spPlayList.end()) return spSongInfo;
	auto iter = m_spSongListMap.find(m_iCurPlayingListIndex);
	if (iter!=m_spSongListMap.end())
	{
		songsArrayT songs = iter->second;
		if(songs.size()>0)
		{
			for (songsArrayIterT iter=songs.begin();iter!=songs.end();++iter)
			{
				if(iter->GetLocalId()==m_iCurPlayingSongIndex)
				{
					spSongInfo = spSongInfoT(new CSongInfo(*iter));
					break;
				}
			}
		}
	}
	return spSongInfo;
}
void CPlayCenter::UpdateCurrentPlayingSong(spSongInfoT spSong)
{
	ASSERT(spSong);
	if(m_spPlayList.size()==0||m_spSongListMap.size()==0) return ;
	
	if(m_spPlayList.find(m_iCurPlayingListIndex)==m_spPlayList.end()) return ;
	auto iter = m_spSongListMap.find(m_iCurPlayingListIndex);
	if (iter != m_spSongListMap.end())
	{
		songsArrayT& songs = iter->second;
		if(songs.size()>0)
		{
			int iIndex = 0;
			for (songsArrayIterT iter=songs.begin();iter!=songs.end();++iter)
			{
				if(iter->GetLocalId()==m_iCurPlayingSongIndex)
				{
					songs[iIndex].SetAblumName(spSong->GetAlbumName());
					songs[iIndex].SetAlbumId(spSong->GetAlbumId());
					songs[iIndex].SetAddTime(spSong->GetAddTime());
					songs[iIndex].SetArtistId(spSong->GetArtistId());
					songs[iIndex].SetArtistName(spSong->GetArtistName());
					songs[iIndex].SetAudioLength(spSong->GetAudioLength());
					songs[iIndex].SetCoverLocalPath(spSong->GetCoverLocalPath());
					songs[iIndex].SetCoverUrl(spSong->GetCoverUrl());
					songs[iIndex].SetEnable(spSong->IsEnable());
					songs[iIndex].SetFileExt(spSong->GetFileExt());
					songs[iIndex].SetFileSize(spSong->GetFileSize());
					songs[iIndex].SetFileStartPos(spSong->GetFileStartPos());
					songs[iIndex].SetFileType(spSong->GetFileType());
					songs[iIndex].SetFlag(spSong->GetFlag());
					songs[iIndex].SetLastModifyTime(spSong->GetLastModifyTime());
					songs[iIndex].SetLastPlayTime(spSong->GetLastPlayTime());
					songs[iIndex].SetListenFileUrl(spSong->GetListenFileUrl());
					songs[iIndex].SetLocal(spSong->IsLocal());
					songs[iIndex].SetLocalId(spSong->GetLocalId());
					songs[iIndex].SetLocalPath(spSong->GetLocalPath());
					songs[iIndex].SetLrcLocalPath(spSong->GetLrcLocalPath());
					songs[iIndex].SetLrcUrl(spSong->GetLrcUrl());
					songs[iIndex].SetMD5(spSong->GetMD5());
					songs[iIndex].SetPlayCount(spSong->GetPlayCount());
					songs[iIndex].SetRealArtistName(spSong->GetRealArtistName());
					songs[iIndex].SetResourceID(spSong->GetResourceId());
					songs[iIndex].SetSongId(spSong->GetSongId());
					songs[iIndex].SetSongName(spSong->GetSongName());
					songs[iIndex].SetTag(spSong->GetTag());
					break;
				}
				iIndex ++;
			}
		}
	}
}
int CPlayCenter::GetPlayIndex()
{
	return m_iCurPlayingSongIndex;
}
void CPlayCenter::SetPlayIndex(unsigned int u_index)
{
	m_iCurPlayingSongIndex = u_index;
}
int CPlayCenter::GetListIndex()
{
	return m_iCurPlayingListIndex;
}
void CPlayCenter::SetListIndex(unsigned int u_index)
{
	m_iCurPlayingListIndex = u_index;
}

int __stdcall CPlayCenter::CallBackFunc(void* instance, void *user_data, TCallbackMessage message, unsigned int param1, unsigned int param2)
{
	return 0;
}