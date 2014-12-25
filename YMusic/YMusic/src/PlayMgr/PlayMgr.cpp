#include "stdafx.h"

#include "PlayMgr.h"

#pragma comment(lib,"libzplay.lib")

CPlayMgr thePlayMgr;

CPlayMgr::CPlayMgr(void)
	: m_PlayCenter(NULL)
	, m_iPlayIndex(0)
	, m_bPlayMode(EM_MODE::EM_LIST_ORDER)
	, m_bPlayState(EP_STATE::EP_STOP)
{
	m_PlayCenter = CreateZPlay();
	ASSERT(m_PlayCenter);
	//sLog(_T("CreateZPlay()"));	
	// get library version
	int ver = m_PlayCenter->GetVersion();
	if(ver < 190)
	{
		//sLogError(_T("Error: Need library version 2.00 and above !"));
		m_PlayCenter->Release();
		ASSERT(FALSE&&"need library version 2.00 or above ");
	}
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
	m_PlayCenter->SetCallbackFunc(CallbackFunc,tMsg, 0);
}


CPlayMgr::~CPlayMgr(void)
{
	if(m_PlayCenter)
		m_PlayCenter->Release();
}

////
bool CPlayMgr::OpenFile(const TCHAR* filepath,TStreamFormat nFormat /* = sfAutodetect */)
{
	if(m_PlayCenter==NULL) return false;
	int iVal = m_PlayCenter->OpenFileW(filepath,nFormat);
	
	return iVal > 0;
}

void CPlayMgr::PlayForward(unsigned int u_pos,unsigned int pos_used)
{
	if(m_PlayCenter==NULL) return;
	TStreamTime pTime;
	pTime.sec = u_pos - pos_used;
	m_PlayCenter->Seek(tfSecond, &pTime, smFromCurrentForward);
}

void CPlayMgr::PlayBack(unsigned int u_pos,unsigned int pos_used)
{
	if(m_PlayCenter==NULL) return;
	TStreamTime pTime;
	pTime.sec = pos_used - u_pos;
	m_PlayCenter->Seek(tfSecond, &pTime, smFromCurrentBackward);
}

int CPlayMgr::NextPlayIndex()
{
	switch (m_bPlayMode)
	{
	case EM_MODE::EM_LIST_ORDER:
		{
			m_iPlayIndex ++;
			if(m_iPlayIndex>=m_playqueue.size())
				m_iPlayIndex = m_playqueue.size()-1;
			return -1;
		}
		break;
	case EM_MODE::EM_LIST_LOOP:
		{
			m_iPlayIndex ++;
			if(m_iPlayIndex>=m_playqueue.size())
				m_iPlayIndex = 0;
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
			int iTotal = m_playqueue.size();
			srand(time(NULL));
			m_iPlayIndex = rand()%iTotal;
			return 0;
		}
		break;
	default:
		ASSERT(FALSE);
		return -1;
	}
}

int CPlayMgr::PrevPlayIndex()
{
	switch (m_bPlayMode)
	{
	case EM_MODE::EM_LIST_ORDER:
		{
			m_iPlayIndex --;
			if(m_iPlayIndex<0)
				m_iPlayIndex = 0;
			return -1;
		}
		break;
	case EM_MODE::EM_LIST_LOOP:
		{
			m_iPlayIndex --;
			if(m_iPlayIndex<0)
				m_iPlayIndex = 0;
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
			int iTotal = m_playqueue.size();
			srand(time(NULL));
			m_iPlayIndex = rand()%iTotal;
			return 0;
		}
		break;
	default:
		ASSERT(FALSE);
		return -1;
	}
}
///////////////
bool CPlayMgr::Play(const TCHAR* filepath)
{
	if(OpenFile(filepath))
	{
		m_PlayCenter->Play();
		return true;
	}
	return false;
}

void CPlayMgr::Play()
{
	if(m_iPlayIndex<0||m_iPlayIndex>=m_playqueue.size()) return;
	this->Play(m_playqueue[m_iPlayIndex]);
	m_bPlayState = EP_STATE::EP_PLAY;


}

void CPlayMgr::Stop()
{
	if(m_PlayCenter)
		m_PlayCenter->Stop();
	m_bPlayState = EP_STATE::EP_STOP;
	
}

void CPlayMgr::Pause()
{
	if(m_PlayCenter)
		m_PlayCenter->Pause();
	m_bPlayState = EP_STATE::EP_PAUSE;

}

void CPlayMgr::Resume()
{
	if(m_PlayCenter)
		m_PlayCenter->Resume();
	m_bPlayState = EP_STATE::EP_PLAY;
}

void CPlayMgr::Next()
{
	this->NextPlayIndex();
	this->Play();
}

void CPlayMgr::Prev()
{
	this->PrevPlayIndex();
	this->Play();
}

void CPlayMgr::PlayTo(unsigned int u_pos)
{
	if(NULL==m_PlayCenter) return ;
	long pos_used = GetPosition();
	if(u_pos > pos_used)
		PlayForward(u_pos,pos_used);
	else
		PlayBack(u_pos,pos_used);
}

long CPlayMgr::GetPosition()
{
	if(NULL==m_PlayCenter) return 0;
	TStreamTime TSTPos;
	m_PlayCenter->GetPosition(&TSTPos);
	long used = TSTPos.hms.hour*60*60 + TSTPos.hms.minute*60+TSTPos.hms.second;
	return used;
}

void CPlayMgr::SetVolume(unsigned int u_volume)
{
	if(NULL==m_PlayCenter) return ;
	m_PlayCenter->SetMasterVolume(u_volume,u_volume);
}

int CPlayMgr::GetVolume()
{
	if(NULL==m_PlayCenter) return 0;
	unsigned int ivolume = 0;
	m_PlayCenter->GetMasterVolume(&ivolume,&ivolume);
	return ivolume;
}

void CPlayMgr::GetStatus(TStreamStatus* ts)
{
	m_PlayCenter->GetStatus(ts);
}

bool CPlayMgr::DrawBMPToHDC(HDC hdc,RECT rt)
{
	if(m_iPlayIndex<0||m_iPlayIndex>=m_playqueue.size()) return false;
	ZPlay* player = CreateZPlay();
		
	ASSERT(player);
	if(player==NULL) return false;
	TID3InfoExW *id3info = new TID3InfoExW;
	CString file_path = m_playqueue[m_iPlayIndex];
	if(player->OpenFileW(file_path,sfAutodetect)==0)
	{
		player->Release();
		return false;
	}
	bool b_ret = false;
	if(player->LoadID3ExW(id3info,1))
	{
		
		if(id3info->Picture.CanDrawPicture)
		{			
		//	HDC myDC = CreateCompatibleDC(hdc);
			player->DrawBitmapToHDC(hdc, rt.left,rt.top,0,0, id3info->Picture.hBitmap);
			b_ret = true;
		//	DeleteDC(myDC);
		}
		else
		{
			// can't decode attached image to hBitmap, use PictureData and PictureDataSize
			// to decode this format by your own image decoder
			b_ret = false;
		}
	}
	delete id3info;
	player->Release();
	return b_ret;
}

bool CPlayMgr::DrawBMPToHWND(HWND hwnd,RECT rt)
{
	if(m_iPlayIndex<0||m_iPlayIndex>=m_playqueue.size()) return false;
	ZPlay* player = CreateZPlay();

	ASSERT(player);
	if(player==NULL) return false;
	TID3InfoExW *id3info = new TID3InfoExW;
	CString file_path = m_playqueue[m_iPlayIndex];
	if(player->OpenFileW(file_path,sfAutodetect)==0)
	{
		player->Release();
		return false;
	}
	bool b_ret = false;
	if(player->LoadID3ExW(id3info,1))
	{

		if(id3info->Picture.CanDrawPicture)
		{			
//			player->DrawBitmapToHWND(hwnd, rt.left,rt.top,0,0, id3info->Picture.hBitmap);
			CRect rect;
			::GetClientRect(hwnd,rect);

			HDC hdc = ::GetDC(hwnd);

			CDC memDC;
			memDC.CreateCompatibleDC(CDC::FromHandle(hdc));
			CBitmap bmp;
			bmp.Attach(id3info->Picture.hBitmap);
			memDC.SelectObject(&bmp);

//			::BitBlt(hdc,0,0,rect.Width(),rect.Height(),memDC.m_hDC,0,0,SRCCOPY);
			::StretchBlt(hdc,0,0,rect.Width(),rect.Height(),memDC.m_hDC,0,0,id3info->Picture.Width,id3info->Picture.Height,SRCCOPY);
			bmp.DeleteObject();
			memDC.DeleteDC();
			b_ret = true;
		}
		else
		{
			// can't decode attached image to hBitmap, use PictureData and PictureDataSize
			// to decode this format by your own image decoder
			b_ret = false;
		}
	}
	delete id3info;
	player->Release();
	return b_ret;
}

bool CPlayMgr::WriteBMPToFile(LPCTSTR lpszFile)
{
	ASSERT(lpszFile);
	if(m_iPlayIndex<0||m_iPlayIndex>=m_playqueue.size()) return false;
	ZPlay* player = CreateZPlay();

	ASSERT(player);
	if(player==NULL) return false;
	TID3InfoExW *id3info = new TID3InfoExW;
	CString file_path = m_playqueue[m_iPlayIndex];
	if(player->OpenFileW(file_path,sfAutodetect)==0)
	{
		player->Release();
		return false;
	}
	bool b_ret = false;
	if(player->LoadID3ExW(id3info,1))
	{

		if(id3info->Picture.CanDrawPicture)
		{			
			BITMAP bInfo;
			//::GetBitmapBits((HBITMAP)id3info->Picture.hBitmap,sizeof(BITMAP),&bInfo);
			if(GetObject(id3info->Picture.hBitmap, sizeof(BITMAP), &bInfo) == 0)
			{				
				return false;
			}

			int szPanel = 0;
			if (bInfo.bmBitsPixel<16)
				szPanel = pow(2,bInfo.bmBitsPixel)*sizeof(RGBQUAD);
			BITMAPINFO* pBitInfo = (BITMAPINFO*) LocalAlloc(LPTR,sizeof(BITMAPINFO)+szPanel);
			pBitInfo->bmiHeader.biBitCount = bInfo.bmBitsPixel;

			pBitInfo->bmiHeader.biClrImportant = 0;
			pBitInfo->bmiHeader.biCompression = 0;
			pBitInfo->bmiHeader.biHeight = bInfo.bmHeight;
			pBitInfo->bmiHeader.biPlanes = bInfo.bmPlanes;
			pBitInfo->bmiHeader.biSize = sizeof(BITMAPINFO);
			pBitInfo->bmiHeader.biSizeImage = bInfo.bmWidthBytes* bInfo.bmHeight;
			pBitInfo->bmiHeader.biWidth = bInfo.bmWidth;
			pBitInfo->bmiHeader.biXPelsPerMeter = 0;
			pBitInfo->bmiHeader.biYPelsPerMeter = 0;

			HDC hdc = ::GetDC((HWND) NULL);
			::SelectObject(hdc,id3info->Picture.hBitmap);
			char* pData = new char[bInfo.bmWidthBytes*bInfo.bmHeight];
			GetDIBits(hdc,(HBITMAP)id3info->Picture.hBitmap,0,bInfo.bmHeight,pData,pBitInfo,DIB_RGB_COLORS);

			BITMAPFILEHEADER FileHeader;
			FileHeader.bfType = 0x4d42; //BM
			FileHeader.bfReserved1 = 0;
			FileHeader.bfReserved2 = 0;
			FileHeader.bfSize = sizeof(BITMAPFILEHEADER);
			FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFO)+szPanel;

			CFile file;
			file.Open(lpszFile,CFile::modeCreate|CFile::modeWrite);
			file.Write(&FileHeader,sizeof(BITMAPFILEHEADER));
			file.Write(pBitInfo,sizeof(BITMAPINFO));
			file.Write(pData,bInfo.bmWidthBytes*bInfo.bmHeight);
			file.Close();

			LocalFree(pBitInfo);
			delete pData;

			b_ret = true;
		}
		else
		{
			// can't decode attached image to hBitmap, use PictureData and PictureDataSize
			// to decode this format by your own image decoder
			b_ret = false;
		}
	}
	delete id3info;
	player->Release();
	return b_ret;
}

int CPlayMgr::GetPlayIndex()
{
	return m_iPlayIndex;
}

void CPlayMgr::SetPlayIndex(unsigned int u_index)
{
	m_iPlayIndex = u_index;
}

void CPlayMgr::SetPlayState(byte b_state)
{
	m_bPlayState = b_state;
}

byte CPlayMgr::GetPlayState()
{
	return m_bPlayState;
}

byte CPlayMgr::GetPlayMode()
{
	return m_bPlayMode;
}

void CPlayMgr::SetPlayMode(byte b_mode)
{
	m_bPlayMode = b_mode;
}

void CPlayMgr::PushPlayQueue(const TCHAR* filepath)
{
	m_playqueue.push_back(filepath);
}

void CPlayMgr::PushPlayQueue(MusicInfo music)
{
	m_playqueue.push_back(music.FilePath);
}

void CPlayMgr::PushPlayQueue(std::vector<MusicInfo> musics)
{
	m_playqueue.clear();
	for (std::vector<MusicInfo>::iterator it=musics.begin();it!=musics.end();++it)
	{
		m_playqueue.push_back(it->FilePath);
	}
}

CString CPlayMgr::PopPlayQueue(unsigned int u_index)
{
	ASSERT(u_index>=0&&u_index<m_playqueue.size());
	CString s_filepath = m_playqueue[u_index];
	m_playqueue.erase(m_playqueue.begin()+u_index);

	return s_filepath;
}

void CPlayMgr::EmptyPlayQueue()
{
	m_playqueue.clear();
}

void CPlayMgr::LoadFileInfo(const wchar_t* filepath,TID3InfoExW* fileinfo,TStreamFormat nFormat /* = sfAutodetect */)
{
	if(m_PlayCenter==NULL) return ;
	if (!m_PlayCenter->LoadFileID3ExW(filepath,nFormat,fileinfo,1))
	{

	}
}

void CPlayMgr::LoadFileInfo(ID3InfoW* fileinfo)
{
	if(m_iPlayIndex<0||m_iPlayIndex>=m_playqueue.size()) return;
	TID3InfoExW id3;
	LoadFileInfo(m_playqueue[m_iPlayIndex],&id3);
	TStreamInfo pInfo;
	m_PlayCenter->GetStreamInfo(&pInfo);

	fileinfo->time_ = pInfo.Length.hms.hour*60*60 + pInfo.Length.hms.minute*60 + pInfo.Length.hms.second;
	fileinfo->tid3_ = id3;
}

CString CPlayMgr::GetNowPlaying()
{
	if(m_iPlayIndex<0||m_iPlayIndex>=m_playqueue.size()) return _T("");
	return theFileMgr.rip_file_name(m_playqueue[m_iPlayIndex]);
}

CString CPlayMgr::GetLrcPathName()
{
	if(m_iPlayIndex<0||m_iPlayIndex>=m_playqueue.size()) return _T("");
	CString s_path;
	CString s_name = GetNowPlaying();
	s_path = theFileMgr.GetModuleFilePath();
	if(s_path.Right(1)!=_T('\\')) s_path += _T('\\');
	s_path += _T("lrc\\");
	s_path += s_name;
	int i_pos = s_path.ReverseFind(_T('.'));
	if(i_pos<0) return _T("");
	s_path = s_path.Left(i_pos);
	s_path += _T(".lrc");
	if(theFileMgr.file_exist(s_path))
		return s_path;
	return _T("");
}

long CPlayMgr::GetTimeLength()
{
	TStreamInfo pInfo;
	m_PlayCenter->GetStreamInfo(&pInfo);
	return pInfo.Length.hms.minute*60+pInfo.Length.hms.second;
}

CString CPlayMgr::GetTimeLength2()
{
	TStreamInfo pInfo;
	m_PlayCenter->GetStreamInfo(&pInfo);
	CString sTime;
	sTime.Format(_T("%02u:%02u"),pInfo.Length.hms.minute,pInfo.Length.hms.second);
	return sTime;
}

CString CPlayMgr::GetTimeUsed()
{
	TStreamTime TSTPos;
	m_PlayCenter->GetPosition(&TSTPos);
	CString sTime;
	sTime.Format(_T("%02u:%02u"),TSTPos.hms.minute,TSTPos.hms.second);
	return sTime;
}

long CPlayMgr::GetTimeUsed2()
{
	TStreamTime TSTPos;
	m_PlayCenter->GetPosition(&TSTPos);
	return TSTPos.hms.hour*60*60+TSTPos.hms.minute*60+TSTPos.hms.second;
}

void CPlayMgr::GetCurrentMusicInfo(MusicInfoVT& spSong)
{

	if(m_iPlayIndex<0||m_iPlayIndex>=m_playqueue.size()) return ;
	ZPlay* player = CreateZPlay();
	if(!player)		
		ASSERT(false);
	{
		TID3InfoExW *id3info = new TID3InfoExW;
		CString file_path = m_playqueue[m_iPlayIndex];
		if(player->OpenFileW(file_path,sfAutodetect)==0)
		{
			player->Release();
			return ;
		}
		int ret;
		if((ret=player->LoadID3ExW(id3info,1)))
		{
			spSong.sSongName = CString(id3info->Title);
			spSong.sArtistName = CString(id3info->Artist);
		}
		delete id3info;
	}
	player->Release();

	return ;
}

int __stdcall CPlayMgr::CallbackFunc(void* instance, void *user_data, TCallbackMessage message, unsigned int param1, unsigned int param2)
{
	ZPlay *theInst = (ZPlay*) instance;
	ASSERT(theInst);
	switch (message)
	{
	case libZPlay::MsgStopAsync:
		break;
	case libZPlay::MsgPlayAsync:
		break;
	case libZPlay::MsgEnterLoopAsync:
		break;
	case libZPlay::MsgExitLoopAsync:
		break;
	case libZPlay::MsgEnterVolumeSlideAsync:
		break;
	case libZPlay::MsgExitVolumeSlideAsync:
		break;
	case libZPlay::MsgStreamBufferDoneAsync:
		break;
	case libZPlay::MsgStreamNeedMoreDataAsync:
		break;
	case libZPlay::MsgNextSongAsync:
		break;
	case libZPlay::MsgStop:
		break;
	case libZPlay::MsgPlay:
		break;
	case libZPlay::MsgEnterLoop:
		break;
	case libZPlay::MsgExitLoop:
		break;
	case libZPlay::MsgEnterVolumeSlide:
		break;
	case libZPlay::MsgExitVolumeSlide:
		break;
	case libZPlay::MsgStreamBufferDone:
		break;
	case libZPlay::MsgStreamNeedMoreData:
		break;
	case libZPlay::MsgNextSong:
		break;
	case libZPlay::MsgWaveBuffer:
		break;
	default:
		break;
	}
	return 0;
}