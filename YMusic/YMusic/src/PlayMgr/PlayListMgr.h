#ifndef __PLAYLISTMGR_H
#define __PLAYLISTMGR_H

#pragma once

#include "../../YMusic.h"
#include <map>

//播放模式
enum EM_MODE
{
	EM_LIST_ORDER = 0,
	EM_LIST_LOOP,
//	EM_SINGLE,
	EM_SINGLE_LOOP,
	EM_RAMDON,
};
class PlayListMgr : public Singleton<PlayListMgr>
{
public:
	typedef std::vector<CSongInfo>            songsArrayT;
	typedef songsArrayT::iterator             songsArrayIterT;

	typedef std::map<int,songsArrayT >					 playListSongMapT;
	typedef std::map<int,songsArrayT >::iterator         playListSongMapIterT;

	typedef std::deque<int>					 randomIdArrayT;
	typedef std::deque<int>::iterator        randomIdArrayIterT;
public:
	DECLARE_SINGLETON_CLASS(PlayListMgr)
	PlayListMgr();
	virtual ~PlayListMgr();
public:
	void	SetPlayMode(int mode){ m_playMode = mode; }
	int     GetPlayMode() { return m_playMode; }
	playListSongMapT GetAllPlayList() const;
	songsArrayT      GetSongListByListID(unsigned int u_listId) ;
	spSongInfoT      GetSongInfo(unsigned int u_songId,unsigned int u_listId);
	
	void LoadAllPlayList();
	void AddList(spPlayListInfoT spList);
	void AddList(CPlayListInfo playList);
	void AddSongToList(CSongInfo spSong,unsigned int u_listId);
	void AddSongToList(spSongInfoT spSong,unsigned int u_listId);

	void RemoveList(spPlayListInfoT spList);
	void RemoveSongFromList(spSongInfoT spSong,unsigned int u_listId);
	void RemoveAllSongFromList(unsigned int u_listId);

	spPlayListInfoT  GetCurList() { return m_spCurList; }
	spSongInfoT      GetCurSong() { return m_spCurSong; }
	void SetCurList(spPlayListInfoT spList) { m_spCurList = spList ; }
	void SetCurSong(spSongInfoT spSong) { m_spCurSong = spSong ;}
	void UpdateList(spPlayListInfoT spList);
	void UpdateSong(spSongInfoT spSong,unsigned int u_listId);

	void  MakeRandId(unsigned int uRandNum);
	void  ClearRand(){ m_spRandomId.clear(); }
	spSongInfoT      GetNextSongToPlay();
	spSongInfoT      GetPrevSongToPlay();

	spSongInfoT      PraseFromPath(LPCTSTR path);
	void			 CheckSong(spSongInfoT& spSong);
protected:
	int     GetCurIndexByCurSong(const songsArrayT& songList);
	void    Reset();
private:
	playListSongMapT  m_spSongListMap;
	randomIdArrayT    m_spRandomId;
	spPlayListInfoT   m_spCurList;//记录当前播放的列表
	spSongInfoT       m_spCurSong;//记录当前播放的歌曲
	int				  m_playMode;
};



#endif//__PLAYLISTMGR_H