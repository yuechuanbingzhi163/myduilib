#pragma  once



#if defined(YPLAY_EXPORTS)
	#if defined(_MSC_VER)
		#define YPLAY_API __declspec(dllexport)
		#pragma comment(lib,"libzplay.lib")
	#else
		#define YPLAY_API 
	#endif
#else
	#if defined(_MSC_VER)
		#define YPLAY_API __declspec(dllimport)
		#pragma comment(lib,"YPlay.lib")
	#else
		#define YPLAY_API 
	#endif
#endif

#include "libzplay.h"
/*#pragma comment(lib,"libzplay.lib")*/
using namespace libZPlay;
//播放管理

//播放状态
enum EP_STATE
{
	EP_INIT,
	EP_STOP,
	EP_PLAY,
	EP_PAUSE,
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
struct ID3Info
{
	TID3InfoEx tid3_;
	long       time;
};

struct ID3InfoW
{
	TID3InfoExW  tid3_;
	long         time_;
};

typedef int (__stdcall * YCallbackFunc)(void* instance, void *user_data, TCallbackMessage message, unsigned int param1, unsigned int param2);
typedef YCallbackFunc  YFun;


class YPLAY_API YPlayCenter
{
public:
	YPlayCenter();
	virtual ~YPlayCenter();
public:
	virtual void Play();
	virtual void Next();
	virtual void Prev();
	virtual void Stop();
	virtual void Pause();
	virtual void Resume();
	virtual void SetPostion(unsigned long u_pos);
	virtual long GetPosition();
	virtual void GetStatus(TStreamStatus* ts);
	virtual void SetVolume(unsigned int u_volume);
	virtual int  GetVolume();
	virtual bool IsVolumeOff();
	virtual void SwitchVolume(bool b_off);
	virtual int  GetPlayIndex();
	virtual void SetPlayIndex(unsigned int u_index);
	virtual int  GetListIndex();
	virtual void SetListIndex(unsigned int u_index);
	virtual byte GetPlayState();
	virtual void SetPlayState(byte b_state);
	virtual byte GetPlayMode();
	virtual void SetPlayMode(byte b_mode);

public:
	virtual void LoadFileInfo(ID3InfoW* fileinfo);
	virtual TStreamFormat GetFileFormat(const wchar_t* spath);
	virtual long    GetTimeLength();
	virtual CString GetTimeLength2();
	virtual CString GetTimeUsed();
	virtual long    GetTimeUsed2();
	virtual void    SetCallBackFunc(YFun pFun);
	virtual bool DrawBMPToHDC(HDC hdc,RECT rt,LPCTSTR file_path);
	virtual bool DrawBMPToHWND(HWND hwnd,RECT rt,LPCTSTR file_path);
	virtual bool WriteBMPToFile(LPCTSTR song_file,LPCTSTR lpszFile);
protected:
	virtual bool PlayThis(const wchar_t* filepath);
protected:
	virtual void PlayForward(unsigned int u_pos,unsigned int pos_used);
	virtual void PlayBack(unsigned int u_pos,unsigned int pos_used);
	virtual bool OpenFile(const wchar_t* filepath,TStreamFormat nFormat = sfAutodetect);
	virtual bool LoadFileInfo2(const wchar_t* filepath,TID3InfoExW* fileinfo,TStreamFormat nFormat = sfAutodetect);
	YFun pCallBack;
protected:

private:

protected:
	ZPlay*  m_PlayCenter;
private:
	byte    m_bPlayState;
	byte    m_bPlayMode;
	bool    m_bVolumeOff;
	int     m_iVolumeSize;
};