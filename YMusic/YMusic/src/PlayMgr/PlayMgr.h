#pragma once
#include "libzplay.h"
using namespace libZPlay;

//播放管理

//播放状态
enum EP_STATE
{
	EP_STOP = 0,
	EP_PLAY,
	EP_PAUSE
};
//播放模式
enum EM_MODE
{
	EM_LIST_ORDER = 0,
	EM_LIST_LOOP,
	EM_SINGLE,
	EM_SINGLE_LOOP,
	EM_RAMDON
};

struct ID3InfoW
{
	TID3InfoExW  tid3_;
	long         time_;
};

class CPlayMgr
{
public:
	CPlayMgr(void);
	virtual ~CPlayMgr(void);

public:
	virtual bool Play(const TCHAR* filepath);
	virtual void Play();
	virtual void Next();
	virtual void Prev();
	virtual int  NextPlayIndex();
	virtual int  PrevPlayIndex();

	virtual void Stop();
	virtual void Pause();
	virtual void Resume();
	virtual void PlayTo(unsigned int u_pos);
	virtual long GetPosition();
	virtual void SetVolume(unsigned int u_volume);
	virtual int  GetVolume();
	virtual void GetStatus(TStreamStatus* ts);

	virtual bool DrawBMPToHDC(HDC hdc,RECT rt);
	virtual bool DrawBMPToHWND(HWND hwnd,RECT rt);
	virtual bool WriteBMPToFile(LPCTSTR lpszFile);

	virtual int  GetPlayIndex();
	virtual void SetPlayIndex(unsigned int u_index);
	virtual byte GetPlayState();
	virtual void SetPlayState(byte b_state);
	virtual byte GetPlayMode();
	virtual void SetPlayMode(byte b_mode);
	virtual void PushPlayQueue(const TCHAR* filepath);
	virtual void PushPlayQueue(MusicInfo music);
	virtual void PushPlayQueue(std::vector<MusicInfo> musics);

	virtual CString PopPlayQueue(unsigned int u_index);
	virtual void    EmptyPlayQueue();

	virtual void LoadFileInfo(ID3InfoW* fileinfo);
	virtual CString GetNowPlaying() ;
	virtual CString GetLrcPathName();
	virtual long    GetTimeLength();
	virtual CString GetTimeLength2();
	virtual CString GetTimeUsed();
	virtual long    GetTimeUsed2();

	virtual void   GetCurrentMusicInfo(MusicInfoVT& spSong);
protected:
	virtual void PlayForward(unsigned int u_pos,unsigned int pos_used);
	virtual void PlayBack(unsigned int u_pos,unsigned int pos_used);
	virtual bool OpenFile(const TCHAR* filepath,TStreamFormat nFormat = sfAutodetect);
	virtual void LoadFileInfo(const wchar_t* filepath,TID3InfoExW* fileinfo,TStreamFormat nFormat = sfAutodetect);

protected:
	static int __stdcall  CallbackFunc(void* instance, void *user_data, TCallbackMessage message, unsigned int param1, unsigned int param2);
private:
	ZPlay*  m_PlayCenter;
	int     m_iPlayIndex;
	byte    m_bPlayState;
	byte    m_bPlayMode;
	std::vector<CString> m_playqueue;
};
extern CPlayMgr thePlayMgr;

