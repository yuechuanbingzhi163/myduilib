// YPlay.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

//#define YPLAY_EXPORTS

#ifdef _WIN32
#include <windows.h>
#include <cassert>
#endif
#include "YPlay.h"



YPlayCenter::YPlayCenter(void)
	: m_PlayCenter(NULL)
	, m_bPlayMode(EM_MODE::EM_LIST_ORDER)
	, m_bPlayState(EP_STATE::EP_STOP)
	, pCallBack(NULL)
{
	m_PlayCenter = CreateZPlay();
	assert(m_PlayCenter);

	int ver = m_PlayCenter->GetVersion();
	if(ver < 190)
	{
		m_PlayCenter->Release();
		assert(FALSE&&"need library version 2.00 or above ");
	}
}
YPlayCenter::~YPlayCenter()
{
	if(m_PlayCenter)
		m_PlayCenter->Release();
}

void YPlayCenter::SetCallBackFunc(YFun pFun)
{
	if (pFun&&m_PlayCenter)
	{
		TCallbackMessage tMsg =  (TCallbackMessage) (MsgStopAsync|
			MsgPlayAsync |
			MsgEnterLoopAsync |
			MsgExitLoopAsync |
			MsgEnterVolumeSlideAsync |
			MsgExitVolumeSlideAsync |
			MsgStreamBufferDoneAsync |
			MsgStreamNeedMoreDataAsync |
			MsgNextSongAsync |
			MsgStop |
			MsgPlay |
			MsgEnterLoop |
			MsgExitLoop |
			MsgEnterVolumeSlide |
			MsgExitVolumeSlide |
			MsgStreamBufferDone |
			MsgStreamNeedMoreData |
			MsgNextSong |
			MsgWaveBuffer );
		pCallBack = pFun;
		m_PlayCenter->SetCallbackFunc(pFun,tMsg,0);
	}
}
void YPlayCenter::Play()
{
	
}
void YPlayCenter::Next()
{

}
void YPlayCenter::Prev()
{

}
void YPlayCenter::Stop()
{
	if(m_PlayCenter)m_PlayCenter->Stop();
	m_bPlayState = EP_STOP;
}
void YPlayCenter::Pause()
{
	if(m_PlayCenter)m_PlayCenter->Pause();
	m_bPlayState = EP_PAUSE;
}
void YPlayCenter::Resume()
{
	if(m_PlayCenter)m_PlayCenter->Resume();
	m_bPlayState = EP_PLAY;
}
void YPlayCenter::SetPostion(unsigned long u_pos)
{
	if(NULL==m_PlayCenter) return ;
	long pos_used = GetPosition();
	if(u_pos > pos_used)
		PlayForward(u_pos,pos_used);
	else
		PlayBack(u_pos,pos_used);
}
long YPlayCenter::GetPosition()
{
	if(NULL==m_PlayCenter) return 0;
	TStreamTime TSTPos;
	m_PlayCenter->GetPosition(&TSTPos);
	long used = TSTPos.hms.hour*60*60 + TSTPos.hms.minute*60+TSTPos.hms.second;
	return used;
}
void YPlayCenter::GetStatus(TStreamStatus* ts)
{
	m_PlayCenter->GetStatus(ts);
}
void YPlayCenter::SetVolume(unsigned int u_volume)
{
	if(m_PlayCenter)
		 m_PlayCenter->SetPlayerVolume(u_volume,u_volume);
	m_iVolumeSize = u_volume;
}
int  YPlayCenter::GetVolume()
{
	return m_iVolumeSize;
}
bool YPlayCenter::IsVolumeOff()
{
	return m_bVolumeOff;
}
void YPlayCenter::SwitchVolume(bool b_off)
{
	if(m_PlayCenter)
		(m_bVolumeOff=b_off) ? m_PlayCenter->SetPlayerVolume(0,0) : m_PlayCenter->SetPlayerVolume(m_iVolumeSize,m_iVolumeSize);
}
int  YPlayCenter::GetPlayIndex()
{
	return 0;
}
void YPlayCenter::SetPlayIndex(unsigned int u_index)
{
	
}
int YPlayCenter::GetListIndex()
{
	return 0;
}
void YPlayCenter::SetListIndex(unsigned int u_index)
{
	
}
byte YPlayCenter::GetPlayState()
{
	return m_bPlayState;
}
void YPlayCenter::SetPlayState(byte b_state)
{
	m_bPlayState = b_state;
}
byte YPlayCenter::GetPlayMode()
{
	return m_bPlayMode;
}
void YPlayCenter::SetPlayMode(byte b_mode)
{
	m_bPlayMode = b_mode;
}


long YPlayCenter::GetTimeLength()
{
	if(!m_PlayCenter) return 0;
	TStreamInfo pInfo;
	m_PlayCenter->GetStreamInfo(&pInfo);
	return pInfo.Length.hms.minute*60+pInfo.Length.hms.second;
}
CString YPlayCenter::GetTimeLength2()
{
	if(!m_PlayCenter) return _T("00:00");
	TStreamInfo pInfo;
	m_PlayCenter->GetStreamInfo(&pInfo);
	CString sTime;
	sTime.Format(_T("%02u:%02u"),pInfo.Length.hms.minute,pInfo.Length.hms.second);
	return sTime;
}
CString YPlayCenter::GetTimeUsed()
{
	if(!m_PlayCenter) return _T("00:00");
	TStreamTime TSTPos;
	m_PlayCenter->GetPosition(&TSTPos);
	CString sTime;
	sTime.Format(_T("%02u:%02u"),TSTPos.hms.minute,TSTPos.hms.second);
	return sTime;
}
long    YPlayCenter::GetTimeUsed2()
{
	if(!m_PlayCenter)	return 0;
	TStreamTime TSTPos;
	m_PlayCenter->GetPosition(&TSTPos);
	return TSTPos.hms.hour*60*60+TSTPos.hms.minute*60+TSTPos.hms.second;
}
bool YPlayCenter::PlayThis(const wchar_t* filepath)
{
	TStreamStatus ts;
	m_PlayCenter->GetStatus(&ts);
	if(ts.fPlay||ts.fPause)
		m_PlayCenter->Stop();
	if(OpenFile(filepath))
	{
		m_PlayCenter->Play();
		m_bPlayState = EP_PLAY;
		return true;
	}
	return false;
}
void YPlayCenter::PlayForward(unsigned int u_pos,unsigned int pos_used)
{
	if(m_PlayCenter==NULL) return;
	TStreamTime pTime;
	pTime.sec = u_pos - pos_used;
	m_PlayCenter->Seek(tfSecond, &pTime, smFromCurrentForward);
}
void YPlayCenter::PlayBack(unsigned int u_pos,unsigned int pos_used)
{
	if(m_PlayCenter==NULL) return;
	TStreamTime pTime;
	pTime.sec = pos_used - u_pos;
	m_PlayCenter->Seek(tfSecond, &pTime, smFromCurrentBackward);
}
bool YPlayCenter::OpenFile(const wchar_t* filepath,TStreamFormat nFormat/* = sfAutodetect*/)
{
	if(m_PlayCenter==NULL) return false;
	int iVal = m_PlayCenter->OpenFileW(filepath,nFormat);
//	int iVal = m_PlayCenter->OpenFile(em_utility::mci::unicode_2_ansi(filepath).c_str(),nFormat);
	return iVal > 0;
}

bool YPlayCenter::LoadFileInfo2(const wchar_t* filepath,TID3InfoExW* fileinfo,TStreamFormat nFormat/* = sfAutodetect*/)
{
	if(m_PlayCenter==NULL) return 0;
	return m_PlayCenter->LoadFileID3ExW(filepath,nFormat,fileinfo,1);
}
void YPlayCenter::LoadFileInfo(ID3InfoW* fileinfo)
{

}
