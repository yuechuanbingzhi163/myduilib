#include "stdafx.h"
#include "PlayListMgr.h"
#include "../YApp.h"
#include "audioinfo.h"

PlayListMgr::PlayListMgr()
	: m_playMode(EM_LIST_ORDER)
{}
PlayListMgr::~PlayListMgr()
{}

PlayListMgr::playListSongMapT PlayListMgr::GetAllPlayList() const
{
	return m_spSongListMap;
}

PlayListMgr::songsArrayT PlayListMgr::GetSongListByListID(unsigned int u_listId) 
{
	playListSongMapIterT itEnd = m_spSongListMap.end();
	playListSongMapIterT itFind = m_spSongListMap.find(u_listId);
	ASSERT(itEnd != itFind);

	return itFind->second;
}
spSongInfoT PlayListMgr::GetSongInfo(unsigned int u_songId,unsigned int u_listId)
{
	playListSongMapIterT itEnd = m_spSongListMap.end();
	playListSongMapIterT itFind = m_spSongListMap.find(u_listId);
	ASSERT(itEnd != itFind);
	spSongInfoT spSong;
	const songsArrayT& songList = itFind->second;
	for (auto iter=songList.begin();iter!=songList.end();++iter)
	{
		if(iter->GetLocalId()==u_listId)
		{
			CSongInfo song = *iter;
			spSong = spSongInfoT(&song);
			return spSong;
		}
	}

	return spSong;
}
void PlayListMgr::UpdateList(spPlayListInfoT spList)
{
	ASSERT(spList);
	
}
void PlayListMgr::UpdateSong(spSongInfoT spSong,unsigned int u_listId)
{
	ASSERT(spSong);
	playListSongMapIterT itEnd = m_spSongListMap.end();
	for (auto it=m_spSongListMap.begin();it!=itEnd;++it)
	{
		if (it->first == u_listId)
		{
			songsArrayT& songList = it->second;
			int index = 0;
			for (auto it2=songList.begin();it2!=songList.end();++it2)
			{
				if (it2->GetLocalId()==spSong->GetLocalId())
				{
					CSongInfo& songInfo = *it2;
					songInfo.SetAblumName(spSong->GetAlbumName());
					songInfo.SetAddTime(spSong->GetAddTime());
					songInfo.SetAlbumId(spSong->GetAlbumId());
					songInfo.SetArtistId(spSong->GetArtistId());
					songInfo.SetArtistName(spSong->GetArtistName());
					songInfo.SetAudioLength(spSong->GetAudioLength());
					songInfo.SetCoverLocalPath(spSong->GetCoverLocalPath());
					songInfo.SetCoverUrl(spSong->GetCoverUrl());
					songInfo.SetEnable(spSong->IsEnable());
					songInfo.SetFileExt(spSong->GetFileExt());
					songInfo.SetFileSize(spSong->GetFileSize());
					songInfo.SetFileStartPos(spSong->GetFileStartPos());
					songInfo.SetFileType(spSong->GetFileType());
					songInfo.SetFlag(spSong->GetFlag());
					songInfo.SetLastModifyTime(spSong->GetLastModifyTime());
					songInfo.SetLastPlayTime(spSong->GetLastPlayTime());
					songInfo.SetListenFileUrl(spSong->GetListenFileUrl());
					songInfo.SetLocal(spSong->IsLocal());
					songInfo.SetLocalId(spSong->GetLocalId());
					songInfo.SetLocalPath(spSong->GetLocalPath());
					songInfo.SetLrcLocalPath(spSong->GetLrcLocalPath());
					songInfo.SetLrcUrl(spSong->GetLrcUrl());
					songInfo.SetMD5(spSong->GetMD5());
					songInfo.SetPlayCount(spSong->GetPlayCount());
					songInfo.SetRealArtistName(spSong->GetRealArtistName());
					songInfo.SetResourceID(spSong->GetResourceId());
					songInfo.SetSongId(spSong->GetSongId());
					songInfo.SetSongName(spSong->GetSongName());
					songInfo.SetTag(spSong->GetTag());

					break;
				}
			}
		}
	}
}
void PlayListMgr::Reset()
{
	m_spSongListMap.clear();
	m_spCurSong = spSongInfoT(NULL);
	m_spCurList = spPlayListInfoT(NULL);
}
void PlayListMgr::LoadAllPlayList()
{
	Reset();

	if (theRuntimeState->GetLogUserInfo())
	{
		theDbMgr->SetCurrentUser(theRuntimeState->GetLogUserInfo());
	}
	//获取播放列表
	std::vector<spPlayListInfoT> PlayList;
	theDbMgr->GetUserDB()->GetAllPlayLists(PlayList);

	for (auto it=PlayList.begin();it!=PlayList.end();++it)
	{
// 		CPlayListInfo playList;
// 		playList.SetComment((*it)->GetComment());
// 		playList.SetFlag((*it)->GetFlag());
// 		playList.SetId((*it)->GetId());
// 		playList.SetListType((*it)->GetListType());
// 		playList.SetLocalId((*it)->GetLocalId());
// 		playList.SetName((*it)->GetName());
// 		playList.SetShow((*it)->IsShow());
// 		playList.SetTag((*it)->GetTag());
		AddList(*it);
		//根据列表ID获取歌曲列表
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
void PlayListMgr::AddList(spPlayListInfoT spList)
{
	songsArrayT songList;
	m_spSongListMap.insert(std::pair<int,songsArrayT>(spList->GetLocalId(),songList));
}
void PlayListMgr::AddList(CPlayListInfo playList)
{
	songsArrayT songList;
	m_spSongListMap.insert(std::pair<int,songsArrayT>(playList.GetLocalId(),songList));
}
void PlayListMgr::AddSongToList(CSongInfo spSong,unsigned int u_listId)
{
	auto it = m_spSongListMap.find(u_listId);
	if (it!=m_spSongListMap.end())
	{
		songsArrayT& songList = it->second;
		songList.push_back(spSong);
	}
	else
	{
		songsArrayT songList;
		songList.push_back(spSong);
		m_spSongListMap.insert(std::pair<int,songsArrayT>(u_listId,songList));
	}
}
void PlayListMgr::AddSongToList(spSongInfoT spSong,unsigned int u_listId)
{
	ASSERT(spSong);
	CSongInfo songInfo;
	songInfo.SetAblumName(spSong->GetAlbumName());
	songInfo.SetAddTime(spSong->GetAddTime());
	songInfo.SetAlbumId(spSong->GetAlbumId());
	songInfo.SetArtistId(spSong->GetArtistId());
	songInfo.SetArtistName(spSong->GetArtistName());
	songInfo.SetAudioLength(spSong->GetAudioLength());
	songInfo.SetCoverLocalPath(spSong->GetCoverLocalPath());
	songInfo.SetCoverUrl(spSong->GetCoverUrl());
	songInfo.SetEnable(spSong->IsEnable());
	songInfo.SetFileExt(spSong->GetFileExt());
	songInfo.SetFileSize(spSong->GetFileSize());
	songInfo.SetFileStartPos(spSong->GetFileStartPos());
	songInfo.SetFileType(spSong->GetFileType());
	songInfo.SetFlag(spSong->GetFlag());
	songInfo.SetLastModifyTime(spSong->GetLastModifyTime());
	songInfo.SetLastPlayTime(spSong->GetLastPlayTime());
	songInfo.SetListenFileUrl(spSong->GetListenFileUrl());
	songInfo.SetLocal(spSong->IsLocal());
	songInfo.SetLocalId(spSong->GetLocalId());
	songInfo.SetLocalPath(spSong->GetLocalPath());
	songInfo.SetLrcLocalPath(spSong->GetLrcLocalPath());
	songInfo.SetLrcUrl(spSong->GetLrcUrl());
	songInfo.SetMD5(spSong->GetMD5());
	songInfo.SetPlayCount(spSong->GetPlayCount());
	songInfo.SetRealArtistName(spSong->GetRealArtistName());
	songInfo.SetResourceID(spSong->GetResourceId());
	songInfo.SetSongId(spSong->GetSongId());
	songInfo.SetSongName(spSong->GetSongName());
	songInfo.SetTag(spSong->GetTag());

	AddSongToList(songInfo,u_listId);
}

void PlayListMgr::RemoveList(spPlayListInfoT spList)
{
	ASSERT(spList);
	playListSongMapIterT it = m_spSongListMap.find(spList->GetLocalId());
	if (it!=m_spSongListMap.end())
	{
		m_spSongListMap.erase(it);
	}
}
void PlayListMgr::RemoveSongFromList(spSongInfoT spSong,unsigned int u_listId)
{
	ASSERT(spSong);
	playListSongMapIterT it = m_spSongListMap.find(u_listId);
	if (it!=m_spSongListMap.end())
	{
		songsArrayT& songArr = it->second;
		songsArrayIterT itSong = songArr.begin();
		for (;itSong!=songArr.end();++itSong)
		{
			if (itSong->GetLocalId()==spSong->GetLocalId())
			{
				songArr.erase(itSong);
				break;
			}
		}
	}
}

void PlayListMgr::RemoveAllSongFromList(unsigned int u_listId)
{
	playListSongMapIterT it = m_spSongListMap.find(u_listId);
	if (it!=m_spSongListMap.end())
	{
		it->second.clear();
	}
}

int PlayListMgr::GetCurIndexByCurSong(const songsArrayT& songList)
{
	for (size_t ii=0;ii<songList.size();++ii)
	{
		if (songList[ii].GetLocalId()==m_spCurSong->GetLocalId())
		{
			return ii;
		}
	}
	return 0;
}

void PlayListMgr::MakeRandId(unsigned int uRandNum)
{
	if (uRandNum <= 0)
	{
		return;
	}

	unsigned uSizeOld = m_spRandomId.size();
	unsigned uSizeNew = uSizeOld + uRandNum;
	m_spRandomId.resize(uSizeNew);
	srand(unsigned(time(NULL)));

	for(unsigned i = uSizeOld; i < uSizeNew; i++)
	{
		m_spRandomId[i] = i;
	}

	for(unsigned i = uSizeOld; i < uSizeNew; i++)
	{
		std::swap(m_spRandomId[i], m_spRandomId[rand() % uSizeNew]);
	}
}

spSongInfoT PlayListMgr::GetNextSongToPlay()
{
	spSongInfoT spSong;
	if(!m_spCurList)
	{
		SetCurSong(spSong);
		return spSong;
	}

	auto it = m_spSongListMap.find(m_spCurList->GetLocalId());
	if (it != m_spSongListMap.end())
	{
		const songsArrayT& songList = it->second;
		const size_t nSize = songList.size();
		if (nSize<=0)
		{//列表没有歌曲
			SetCurSong(spSong);
			return spSong;
		}
		switch (m_playMode)
		{
		case EM_LIST_ORDER://顺序播放
			{
				if (!m_spCurSong)
				{
					const CSongInfo& songInfo = songList[0];
					spSong = spSongInfoT(new CSongInfo(songInfo));
					SetCurSong(spSong);
					return spSong;
				}
				else
				{
					int nIndex = GetCurIndexByCurSong(songList);
					nIndex ++;
					if (nIndex<nSize)
					{
						const CSongInfo& songInfo = songList[nIndex];
						spSong = spSongInfoT(new CSongInfo(songInfo));
						SetCurSong(spSong);
						return spSong;
					}
					else
					{
						SetCurSong(spSong);
						return spSong;
					}					
				}
			}
		case EM_LIST_LOOP://列表循环
			{
				if (!m_spCurSong)
				{
					const CSongInfo& songInfo = songList[0];
					spSong = spSongInfoT(new CSongInfo(songInfo));
					SetCurSong(spSong);
					return spSong;
				}
				else
				{
					int nIndex = GetCurIndexByCurSong(songList);
					nIndex ++;
					if (nIndex<nSize)
					{
						const CSongInfo& songInfo = songList[nIndex];
						spSong = spSongInfoT(new CSongInfo(songInfo));					
					}
					else
					{
						const CSongInfo& songInfo = songList[0];
						spSong = spSongInfoT(new CSongInfo(songInfo));
					}
					SetCurSong(spSong);
					return spSong;
				}
			}
		case EM_SINGLE_LOOP://单曲循环
			{
				int nIndex = 0;
				if (!m_spCurSong)
				{
					nIndex = 0;
				}
				else
				{
					nIndex = GetCurIndexByCurSong(songList);					
				}
				const CSongInfo& songInfo = songList[nIndex];
				spSong = spSongInfoT(new CSongInfo(songInfo));
				SetCurSong(spSong);
				return spSong;
			}
		case EM_RAMDON://随机播放
			{
				if (! m_spRandomId.size())
				{
					MakeRandId(songList.size());
				}

				int nIndex = m_spRandomId.front();
				m_spRandomId.pop_front();

				const CSongInfo& songInfo = songList[nIndex];
				spSong = spSongInfoT(new CSongInfo(songInfo));
				SetCurSong(spSong);
				return spSong;
			}
		}	
	}
	SetCurSong(spSong);
	return spSong;
}

spSongInfoT PlayListMgr::GetPrevSongToPlay()
{
	spSongInfoT spSong;
	if(!m_spCurList)
	{
		SetCurSong(spSong);
		return spSong;
	}

	auto it = m_spSongListMap.find(m_spCurList->GetLocalId());
	if (it != m_spSongListMap.end())
	{
		const songsArrayT& songList = it->second;
		const size_t nSize = songList.size();
		if (nSize<=0)
		{//列表没有歌曲
			SetCurSong(spSong);
			return spSong;
		}
		switch (m_playMode)
		{
		case EM_LIST_ORDER://顺序播放
			{
				if (!m_spCurSong)
				{
					const CSongInfo& songInfo = songList[0];
					spSong = spSongInfoT(new CSongInfo(songInfo));
					SetCurSong(spSong);
					return spSong;
				}
				else
				{
					int nIndex = GetCurIndexByCurSong(songList);
					nIndex --;
					if (nIndex>=0)
					{
						const CSongInfo& songInfo = songList[nIndex];
						spSong = spSongInfoT(new CSongInfo(songInfo));
						SetCurSong(spSong);
						return spSong;
					}
					else
					{
						SetCurSong(spSong);
						return spSong;
					}					
				}
			}
		case EM_LIST_LOOP://列表循环
			{
				if (!m_spCurSong)
				{
					const CSongInfo& songInfo = songList[0];
					spSong = spSongInfoT(new CSongInfo(songInfo));
					SetCurSong(spSong);
					return spSong;
				}
				else
				{
					int nIndex = GetCurIndexByCurSong(songList);
					nIndex --;
					if (nIndex>=0)
					{
						const CSongInfo& songInfo = songList[nIndex];
						spSong = spSongInfoT(new CSongInfo(songInfo));					
					}
					else
					{
						const CSongInfo& songInfo = songList[nSize-1];
						spSong = spSongInfoT(new CSongInfo(songInfo));
					}
					SetCurSong(spSong);
					return spSong;
				}
			}
		case EM_SINGLE_LOOP://单曲循环
			{
				int nIndex = 0;
				if (!m_spCurSong)
				{
					nIndex = 0;
				}
				else
				{
					nIndex = GetCurIndexByCurSong(songList);					
				}
				const CSongInfo& songInfo = songList[nIndex];
				spSong = spSongInfoT(new CSongInfo(songInfo));
				SetCurSong(spSong);
				return spSong;
			}
		case EM_RAMDON://随机播放
			{
				if (! m_spRandomId.size())
				{
					MakeRandId(songList.size());
				}

				int nIndex = m_spRandomId.front();
				m_spRandomId.pop_front();

				const CSongInfo& songInfo = songList[nIndex];
				spSong = spSongInfoT(new CSongInfo(songInfo));
				SetCurSong(spSong);
				return spSong;
			}
		}	
	}
	SetCurSong(spSong);
	return spSong;
}
spSongInfoT PlayListMgr::PraseFromPath(LPCTSTR path)
{
	ASSERT(path);
	spSongInfoT spSong;
	CSongInfo song;
	AudioTagInfo * ptagInfo = new AudioTagInfo;
	ptagInfo->init(mci::unicode_2_ansi(path));

	song.SetSongName(mci::ansi_2_CString(ptagInfo->title().c_str()));
	if (song.GetSongName().IsEmpty())
	{
		song.SetSongName(mci::rip_file_name(path,false));
	}
	song.SetAblumName(mci::ansi_2_CString(ptagInfo->album().c_str()));
	song.SetArtistName(mci::ansi_2_CString(ptagInfo->artist().c_str()));
	song.SetLocalPath(path);
	song.SetLocal(true);
	song.SetAddTime(mci::now_time());
	song.SetFileSize(mci::get_file_size(path));
	song.SetFileStartPos(0);
	song.SetFileExt(mci::rip_file_ext(path,true));
	song.SetRealArtistName(mci::ansi_2_CString(ptagInfo->artist().c_str()));		
	song.SetEnable(true);

	spSong = spSongInfoT(new CSongInfo(song));

	delete ptagInfo;

	return spSong;
}

void PlayListMgr::CheckSong(spSongInfoT& spSong)
{
	assert(spSong);
	if(!spSong) return;
	spSong->SetAblumName(spSong->GetAlbumName().IsEmpty()?_T("未知"):spSong->GetAlbumName());
	spSong->SetSongName(spSong->GetSongName().IsEmpty()?mci::rip_file_name(spSong->GetLocalPath(),false):spSong->GetSongName());
	spSong->SetArtistName(spSong->GetArtistName().IsEmpty()?_T("未知"):spSong->GetArtistName());
	spSong->SetRealArtistName(spSong->GetRealArtistName().IsEmpty()?_T("未知"):spSong->GetRealArtistName());

}