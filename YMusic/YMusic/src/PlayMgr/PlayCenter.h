#pragma once

#include "YPlay.h"
#include "../../YMusic.h"
#include <map>

class CPlayCenter :
	public YPlayCenter
{
public:
	CPlayCenter(void);
	virtual ~CPlayCenter(void);
public:
	typedef std::vector<CSongInfo>            songsArrayT;
	typedef songsArrayT::iterator             songsArrayIterT;
	typedef std::vector<CPlayListInfo>        listsArrayT;
	typedef listsArrayT::iterator             listsArrayIterT;
	typedef std::vector<CString>              pathArrayT;
	typedef pathArrayT::iterator              pathArrayIterT;
	typedef std::vector<int>                  songIdArrayT;
	typedef songIdArrayT::iterator            songIdArrayIterT;

	typedef std::map<int,CPlayListInfo>       playListMapT;
	typedef std::map<int,songsArrayT>         playListSongMapT;
private:
	unsigned int  m_iCurPlayingSongIndex;
	unsigned int  m_iCurPlayingListIndex;
	songIdArrayT  m_songIdArray;
	playListMapT  m_spPlayList;
	playListSongMapT  m_spSongListMap;
public:
	virtual void Play();
	virtual void Next();
	virtual void Prev();
public:
	virtual void AddPlayList(CPlayListInfo spList);
	virtual void RenamePlayList(unsigned int u_listIndex,CString strNewListName);
	virtual void RemovePlayList(unsigned int u_listIndex);
	virtual void RemoveAllList();
	virtual void AddSongToList(spSongInfoT songT,unsigned int u_listIndex);
	virtual void AddSongToList(CSongInfo spSong,unsigned int u_listIndex);
//	virtual void AddSongToList(pathArrayT pathArray,unsigned int u_listIndex);
 	virtual void AddSongToList(songsArrayT songArray,unsigned int u_listIndex);
// 	virtual void AddSongToList(const wchar_t* filepath,unsigned int u_listIndex);
	virtual void RemoveSongFromList(unsigned int u_index,unsigned int u_listIndex);
	virtual void RemoveAllFromList(unsigned int u_listIndex);
	CString GetPathBySongLocalId(songsArrayT songArray,unsigned int u_index);
public:
	void GetAllPlayList() ;
	void PlayListChanged() ;
	playListMapT LoadAllPlayList() ;
	playListMapT LoadAllPlayListByType(CPlayListInfo::PlayListType listType);
	songsArrayT LoadAllSongList(unsigned int u_listIndex);
public:
	virtual void LoadFileInfo(ID3InfoW* fileinfo);

private:
	virtual int MakeNextPlayIndex(unsigned int u_listIndex);
	virtual int MakePrevPlayIndex(unsigned int u_listIndex);
public:
	void         CheckSong(spSongInfoT& spSong);
	spSongInfoT  ParaseFromPath(CString sPath);
	spSongInfoT  GetCurrentPlayingSong();
	void		 UpdateCurrentPlayingSong(spSongInfoT spSong);
	virtual int  GetPlayIndex();
	virtual void SetPlayIndex(unsigned int u_index);
	virtual int  GetListIndex();
	virtual void SetListIndex(unsigned int u_index);
public:
	static int __stdcall CallBackFunc(void* instance, void *user_data, TCallbackMessage message, unsigned int param1, unsigned int param2);
};

