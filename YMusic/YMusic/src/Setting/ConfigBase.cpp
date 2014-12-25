#include "stdafx.h"
#include "ConfigBase.h"
#include "../YApp.h"
//#include "../Utilty/json/include/value.h"
// #include "../WebService/sock.h"
//#include "../Agent/Agent.h"
// #include "../MainWindow/MainWindow.h"



	CConfigBase::CConfigBase()
	{	
		m_bModify = false;
		m_spUserDataBase = theDbMgr->GetUserDB();
		assert(m_spUserDataBase);
	}

	CConfigBase::~CConfigBase()
	{
	}

	spUserDBaseT CConfigBase::GetDB()
	{
		return m_spUserDataBase;
	}

	void CConfigBase::UpdateDB()
	{
		m_spUserDataBase = theDbMgr->GetUserDB();
	}

	bool CConfigBase::RegSet(HKEY hKey,CString cs_subkey,CString cs_value_name,CString cs_value)
	{
		HKEY hKEY;
		long ret0=::RegOpenKeyEx(hKey,cs_subkey, 0, KEY_ALL_ACCESS, &hKEY);
		if (ret0!=ERROR_SUCCESS)
		{
			ret0=RegCreateKey(hKey,cs_subkey,&hKEY);
			if (ret0!=ERROR_SUCCESS)
			{
				return false;
			}
		}

		long ret1=::RegSetValueEx(hKEY,cs_value_name,0,REG_SZ,(CONST BYTE*)(LPCTSTR)cs_value,cs_value.GetLength()*2);

		long ret2=::RegCloseKey(hKEY); 

		if (ret1!=ERROR_SUCCESS)
			return false;

		return true;
	}
	bool CConfigBase::RegSetEx(HKEY hKey,CString cs_subkey,CString cs_value_name,CString cs_value)
	{
		HKEY hKEY;
		bool b_OK = false;

		long n_result = ::RegOpenKeyEx(hKey,cs_subkey, 0, KEY_ALL_ACCESS, &hKEY);
		if (n_result != ERROR_SUCCESS)
		{
			n_result = RegCreateKey(hKey,cs_subkey,&hKEY);
			if (n_result != ERROR_SUCCESS)
			{
				return false;
			}
		}

		TCHAR lpa[MAX_PATH];

		DWORD dwAidSize = MAX_PATH;
		DWORD type = REG_SZ;

		n_result = ::RegQueryValueEx(hKEY,cs_value_name,NULL,&type,(LPBYTE)lpa,&dwAidSize);

		if (n_result == ERROR_SUCCESS)
		{
			if(cs_value  == lpa)
			{
				b_OK = true;
			}
		}

		if(!b_OK)
		{
			n_result = ::RegSetValueEx(hKEY,cs_value_name,0,REG_SZ,(CONST BYTE*)(LPCTSTR)cs_value,cs_value.GetLength()*2);
			if (n_result == ERROR_SUCCESS)
			{
				b_OK =true;
			}

		}
		::RegCloseKey(hKEY); 



		return b_OK;
	}
	bool CConfigBase::RegDelete(CString cs_subkey,CString cs_value_name)
	{
		HKEY hKEY;
		long ret0=::RegOpenKeyEx(HKEY_LOCAL_MACHINE,cs_subkey, 0, KEY_ALL_ACCESS, &hKEY);
		long ret1=::RegDeleteValue(hKEY,cs_value_name);
		long ret2=::RegCloseKey(hKEY); 

		if (ret1==ERROR_SUCCESS)
			return true;
		else
			return false;
	}
	CString CConfigBase::RegQuery(CString cs_subkey,CString cs_value_name)
	{
		HKEY hKEY;
		long ret0 = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,cs_subkey, 0, KEY_ALL_ACCESS, &hKEY);

		TCHAR lpa[_MAX_DIR];
		CString cs_value;

		DWORD dwAidSize=MAX_PATH;
		DWORD type=REG_SZ;
		long ret1=::RegQueryValueEx(hKEY,cs_value_name,NULL,&type,(LPBYTE)lpa,&dwAidSize);
		if (ret1==ERROR_SUCCESS)
		{
			cs_value=lpa;
		}
		long ret2=::RegCloseKey(hKEY);
		return cs_value;
	}

	CString CConfigBase::GetAppName()
	{
		
		TCHAR strAppName[MAX_PATH + 1];
		GetModuleFileName(NULL, strAppName, MAX_PATH);
		CString sAppName(strAppName);
		return sAppName;
	}

	/************************************************************************/
	/*                     常规设置                                         */
	/************************************************************************/
	const TCHAR CNormalConfig::CloseModeKey[]= _T("_CloseMode");
	const TCHAR CNormalConfig::PlayModeKey[]=_T("_PlayMode");
	const TCHAR CNormalConfig::VolumeKey[]=_T("_Volume");
	const TCHAR CNormalConfig::VolumeOffKey[] = _T("_VolumeOff");
	const TCHAR CNormalConfig::KaraokModeKey[] = _T("_KaraokMode");
	const TCHAR CNormalConfig::AutoLoginKey[] = _T("_AutoLoginKey");
	const TCHAR CNormalConfig::AutoStartKey[] = _T("_AutoStartKey");
	const TCHAR CNormalConfig::AutoPlayKey[] = _T("_AutoPlayKey");
	const TCHAR CNormalConfig::AutoLrcKey[] = _T("_AutoOpenLrcKey");
	const TCHAR CNormalConfig::InsertModeKey[] = _T("_InsertModeKey");
	const TCHAR CNormalConfig::PlayRoamingKey[] = _T("_PlayRoaming");
	const TCHAR CNormalConfig::ShowInfoKey[] = _T("_AutoShowInfoKey");
	const TCHAR CNormalConfig::RelateAACKey[] = _T("_AACKey");
	const TCHAR CNormalConfig::RelateAPEKey[] = _T("_APEKey");
	const TCHAR CNormalConfig::RelateFLACKey[] = _T("_FLACKey");
	const TCHAR CNormalConfig::RelateMP3Key[] = _T("_MP3Key");
	const TCHAR CNormalConfig::RelateOGGKey[] = _T("_OGGKey");
	const TCHAR CNormalConfig::RelateWAVKey[] = _T("_WAVKey");
	const TCHAR CNormalConfig::EnableUrlProtoKey[] = _T("_EnableUrlProtoKey");
	CNormalConfig::CNormalConfig()
	{
		m_bToTray = true;//关闭到托盘
		m_bVolOff = false;
		m_nVol =100;
		m_nPlayMode = 0;
		m_bKaraok = true;
		m_bAutoLogin = true;
		m_bAutoStart = true;
		m_bAutoPlay = false;
		m_bAutoLrc = false;
		m_nInsertMode = 2;
#ifdef MinVersion
		m_bShowInfo = false;
#else
		m_bShowInfo = true;
#endif		
		m_bRelateAAC = false;
		m_bRelateAPE = false;
		m_bRelateFLAC = false;
		m_bRelateMP3 = false;
		m_bRelateOGG = false;
		m_bRelateWAV = false;	
		m_bEnableUrlProto = false;
		m_bPlayRoaming = true;
		m_sAppName = GetAppName();
	}
	CNormalConfig::~CNormalConfig()
	{
		//析构函数没有被掉用。
		Save();		
	}

	bool CNormalConfig::Load()
	{
		UpdateDB();
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;

		theDB->GetDictValue(CloseModeKey,m_bToTray);
		theDB->GetDictValue(PlayModeKey,m_nPlayMode);//播放模式
		theDB->GetDictValue(VolumeOffKey,m_bVolOff);
		theDB->GetDictValue(VolumeKey,m_nVol);//音量
		theDB->GetDictValue(AutoLoginKey,m_bAutoLogin);//自动登录
		theDB->GetDictValue(AutoStartKey,m_bAutoStart);//开机启动
		theDB->GetDictValue(AutoPlayKey,m_bAutoPlay);//自动播放
		theDB->GetDictValue(AutoLrcKey,m_bAutoLrc);//自动下载歌词
		theDB->GetDictValue(KaraokModeKey,m_bKaraok);
		int nInsertMode = 0;
		if(theDB->GetDictValue(InsertModeKey,nInsertMode))
		{
			m_nInsertMode=nInsertMode;
		}
		theDB->GetDictValue(ShowInfoKey,m_bShowInfo);
		theDB->GetDictValue(RelateAACKey,m_bRelateAAC);
		theDB->GetDictValue(RelateAPEKey,m_bRelateAPE);
		theDB->GetDictValue(RelateFLACKey,m_bRelateFLAC);
		theDB->GetDictValue(RelateMP3Key,m_bRelateMP3);
		theDB->GetDictValue(RelateOGGKey,m_bRelateOGG);
		theDB->GetDictValue(RelateWAVKey,m_bRelateWAV);
		theDB->GetDictValue(PlayRoamingKey,m_bPlayRoaming);		

		return true;
	}
	bool CNormalConfig::Save()
	{
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;
		if(!IsModify()) return true;

		typedef struct _tagKeyValue_
		{
			const TCHAR*   key;
			bool           bvalue;
		}KeyValue;

		KeyValue keyValues[] = 
		{
			{VolumeKey,m_nVol},
			{VolumeOffKey,m_bVolOff},
			{PlayModeKey,m_nPlayMode},
			{KaraokModeKey,m_bKaraok},
			{AutoLoginKey,m_bAutoLogin},
			{AutoStartKey,m_bAutoStart},
			{AutoPlayKey,m_bAutoPlay},
			{AutoLrcKey,m_bAutoLrc},
			//{InsertModeKey,m_nInsertMode},
			{ShowInfoKey,m_bShowInfo},
			{RelateAACKey,m_bRelateAAC},
			{RelateAPEKey,m_bRelateAPE},
			{RelateFLACKey,m_bRelateFLAC},
			{RelateMP3Key,m_bRelateMP3},
			{RelateOGGKey,m_bRelateOGG},
			{RelateWAVKey,m_bRelateWAV},
			{PlayRoamingKey,m_bPlayRoaming},
			{CloseModeKey,m_bToTray}
		};

		theDB->BeginTransaction();
		for (size_t i = 0;i<sizeof(keyValues)/sizeof(keyValues[0]);++i)
		{
			theDB->DeleteDict(keyValues[i].key);
			theDB->InsertDict(keyValues[i].key,(int)keyValues[i].bvalue);
		}
		theDB->DeleteDict(InsertModeKey);
		theDB->InsertDict(InsertModeKey,(int)m_nInsertMode);
		theDB->Commit();
		Modify(false);

		return true;
	}
	void CNormalConfig::SendRelationPacket(CString strExtension,bool bRelate/* = true*/)
	{
		if (strExtension.IsEmpty())
			return;
		CString RunKey;
		RunKey.Format(_T("YTing%s"),strExtension);

// 		Json::Value  JsonRelate;
// 		JsonRelate["extension"] = mci::unicode_2_utf8(strExtension);
// 		JsonRelate["runkey"] = mci::unicode_2_utf8(RunKey);
// 		if (bRelate)
// 		{
// 			CString appName;
// 			appName.Format(_T("%s %s %s"),GetAppName().GetBuffer(),_T("-play"),_T("%1"));
// 
// 			JsonRelate["appname"]         = mci::unicode_2_utf8(appName);
// 			JsonRelate["defaultIcon"]     = mci::unicode_2_utf8(m_sAppName);
// 
// 			Json::Value rightMenuValue;
// 
// 			CString command;
// 			command.Format(_T("%s %s %s"),GetAppName(),_T("-add"),_T("%1"));
// 			Json::Value item;
// 			item["key"]        = "YTing.2.Add";
// 			item["value"]      = mci::unicode_2_utf8(_T("加入 虾听 播放列表(&E)"));
// 			item["command"]    = mci::unicode_2_utf8(command);
// 			rightMenuValue.append(item);
// 			JsonRelate["rightmenu"]     = rightMenuValue;
//		}

//		std::string sBuf =   JsonRelate.toStyledString();

//		this->SendAgentPacket((bRelate ? PI_RegExt : PI_UnRegExt),sBuf);
	}

	void CNormalConfig::SendAgentPacket(int index,std::string& packet)
	{
//		AsynSendPacket(index,packet.c_str(),packet.size()+1);
	}
	
	void CNormalConfig::SendRunPacket(bool bAutoRun)
	{
// 		Json::Value  JsonRun;
// 		CString appName;
// 		appName.Format(_T("%s -autorun"),GetAppName());
// 		JsonRun["keyname"] = mci::unicode_2_utf8(_T("YTing"));
// 		JsonRun["appname"] = mci::unicode_2_utf8(appName);
// 		std::string sBuf =   JsonRun.toStyledString();
// 
// 		this->SendAgentPacket((bAutoRun ? PI_Run : PI_UnRun),sBuf);
	}

	void CNormalConfig::SendEnableUrlProtoPacket(bool bEnable/* = true*/)
	{
// 		Json::Value v;
// 		CString appName;
// 		appName.Format(_T("%s %s"),GetAppName(),_T("%1"));
// 		v["keyname"] = misc::unicode_2_utf8(_T("xiaTing"));
// 		v["appname"] = misc::unicode_2_utf8(appName);
// 		std::string sBuf =   v.toStyledString();
// 
// 		SendAgentPacket((bEnable ? PI_RegUrlProto : PI_UnRegUrlProto),sBuf);
	}

	bool CNormalConfig::IsCloseToTray()
	{
		return m_bToTray;
	}
	void CNormalConfig::SetCloseMode(bool bToTray /* = true */)
	{
		if(m_bToTray != bToTray)
		{
			m_bToTray = bToTray;
			Modify(true);
		}	
	}

	int CNormalConfig::GetPlayMode()
	{
		return m_nPlayMode;
	}

	void CNormalConfig::SetPlayMode(int nMode)
	{
		m_nPlayMode = nMode;

		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return;

		theDB->DeleteDict(PlayModeKey);
		theDB->InsertDict(PlayModeKey,nMode);
	}

	int CNormalConfig::GetVolume()
	{
		return m_nVol;
	}

	void CNormalConfig::SetVolume(int nVol)
	{
		m_nVol = nVol;

		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return;

		theDB->DeleteDict(VolumeKey);
		theDB->InsertDict(VolumeKey,nVol);

	}
	bool CNormalConfig::IsVolumeOff()
	{
		return m_bVolOff;
	}
	void CNormalConfig::SetVolumeOff(bool bOff /* = true */)
	{
		m_bVolOff = bOff;

		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return;

		theDB->DeleteDict(VolumeOffKey);
		theDB->InsertDict(VolumeOffKey,m_bVolOff);
	}
	bool CNormalConfig::IsKaraokMode()
	{
		return m_bKaraok;
	}
	void CNormalConfig::SetKaraokMode(bool bKaraok /* = true */)
	{
		if(m_bKaraok != bKaraok)
		{
			m_bKaraok = bKaraok;
			Modify(true);
		}
	}
	bool CNormalConfig::IsEnableAutoLogin()const
	{
		return m_bAutoLogin;
	}
	void CNormalConfig::EnableAutoLogin(bool bEnable/* = false*/)
	{
		if (m_bAutoLogin != bEnable)
		{
			m_bAutoLogin = bEnable;
			Modify(true);
		}		
	}
	bool CNormalConfig::IsEnableAutoStart()
	{
		/*CString str = RegQuery(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), _T("xiaTing"));
		if (str.IsEmpty())
		m_bAutoStart =false;
		else
		m_bAutoStart =true;*/
		return m_bAutoStart;
	}
	void CNormalConfig::EnableAutoStart(bool bEnable/* = true*/)
	{
		if (m_bAutoStart != bEnable)
		{
			m_bAutoStart = bEnable;
			/*if (m_bAutoStart)
			{
			TCHAR strAppName[MAX_PATH + 1];
			GetModuleFileName(NULL, strAppName, MAX_PATH);
			CString szAppName = strAppName;
			RegSet(HKEY_LOCAL_MACHINE,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), _T("XiaTing"), szAppName);
			}
			else
			{
			RegDelete(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), _T("XiaTing"));
			}*/
			SendRunPacket(bEnable);
			Modify(true);
		}		
	}
	bool CNormalConfig::IsEnableAutoPlay()const
	{
		return m_bAutoPlay;
	}
	void CNormalConfig::EnableAutoPlay(bool bEnable/* = false*/)
	{
		if(m_bAutoPlay != bEnable)
		{
			m_bAutoPlay = bEnable;
			Modify(true);
		}		
	}
	bool CNormalConfig::IsEnableAutoOpenLrc()const
	{
		return m_bAutoLrc;
	}
	void CNormalConfig::EnableAutoOpenLrc(bool bEnable/* = false*/)
	{
		if (m_bAutoLrc != bEnable)
		{
			m_bAutoLrc = bEnable;
			Modify(true);
		}
		
	}
	unsigned int CNormalConfig::GetInsertMode()const
	{
		return m_nInsertMode;
	}
	void CNormalConfig::SetInsertMode(unsigned int nMode)
	{
		if (m_nInsertMode != nMode)
		{
			m_nInsertMode = nMode;
			Modify(true);
		}
		
	}
	bool CNormalConfig::IsEnableAutoShowSongInfo()const
	{
		return m_bShowInfo;
	}
	void CNormalConfig::EnableAutoShowSongInfo(bool bEnable/* = true*/)
	{
		if (m_bShowInfo != bEnable)
		{
			m_bShowInfo = bEnable;
			Modify(true);
		}
		
	}
	bool CNormalConfig::IsEnableRelateAAC()const
	{
		return m_bRelateAAC;
	}
	void CNormalConfig::EnableRelateAAC(bool bEnable/* = true*/)
	{
		if (m_bRelateAAC != bEnable)
		{
			m_bRelateAAC = bEnable;
			Modify(true);
			
			SendRelationPacket(_T(".aac"),bEnable);
			SendRelationPacket(_T(".m4a"),bEnable);
		}
	}
	bool CNormalConfig::IsEnableRelateAPE()const
	{
		return m_bRelateAPE;
	}
	void CNormalConfig::EnableRelateAPE(bool bEnable/* = true*/)
	{
		if (m_bRelateAPE != bEnable)
		{
			m_bRelateAPE = bEnable;
			Modify(true);
			SendRelationPacket(_T(".ape"),bEnable);
			SendRelationPacket(_T(".cue"),bEnable);
		}
		
	}
	bool CNormalConfig::IsEnableRelateFLAC()const
	{
		return m_bRelateFLAC;
	}
	void CNormalConfig::EnableRelateFLAC(bool bEnable/* = true*/)
	{
		if (m_bRelateFLAC != bEnable)
		{
			m_bRelateFLAC = bEnable;
			Modify(true);

			SendRelationPacket(_T(".flac"),bEnable);
		}		
	}
	bool CNormalConfig::IsEnableRelateMP3()const
	{
		return m_bRelateMP3;
	}
	void CNormalConfig::EnableRelateMP3(bool bEnable/* = true*/)
	{
		if (m_bRelateMP3 != bEnable)
		{
			m_bRelateMP3 = bEnable;
			Modify(true);

			SendRelationPacket(_T(".mp3"),bEnable);
		}
	}
	bool CNormalConfig::IsEnableRelateOGG()const
	{
		return m_bRelateOGG;
	}
	void CNormalConfig::EnableRelateOGG(bool bEnable/* = true*/)
	{
		if (m_bRelateOGG != bEnable)
		{
			m_bRelateOGG = bEnable;
			Modify(true);

			SendRelationPacket(_T(".ogg"),bEnable);
		}
		
	}
	bool CNormalConfig::IsEnableRelateWAV()const
	{
		return m_bRelateWAV;
	}
	void CNormalConfig::EnableRelateWAV(bool bEnable/* = true*/)
	{
		if (m_bRelateWAV != bEnable)
		{
			m_bRelateWAV = bEnable;
			Modify(true);
			SendRelationPacket(_T(".wav"),bEnable);
		}	
	}
	
	bool CNormalConfig::IsAllRelate() const
	{
		return (IsEnableRelateAAC() &&
			IsEnableRelateAPE() &&
			IsEnableRelateFLAC() &&
			IsEnableRelateMP3() &&
			IsEnableRelateOGG() &&
			IsEnableRelateWAV());
	}

	void CNormalConfig::EnableAllRelate(bool bEnable)
	{
		EnableRelateAAC(bEnable);
		EnableRelateAPE(bEnable);
		EnableRelateFLAC(bEnable);
		EnableRelateMP3(bEnable);
		EnableRelateOGG(bEnable);
		EnableRelateWAV(bEnable);
	}

	bool CNormalConfig::IsEnableUrlProto() const
	{
		return m_bEnableUrlProto;
	}
	void CNormalConfig::EnableUrlProto(bool bEnable/* = true*/)
	{
		if(bEnable != m_bEnableUrlProto	)
		{
			m_bEnableUrlProto = bEnable;
			SendEnableUrlProtoPacket(bEnable);
		}
	}

	bool CNormalConfig::IsPlayRoaming() const
	{
		return m_bPlayRoaming;
	}

	void CNormalConfig::EnablePlayRoaming( bool enable )
	{
		if (m_bPlayRoaming != enable)
		{
			m_bPlayRoaming = enable;
			Modify(true);
		}
	}


	/************************************************************************/
	/*                     提醒设置                                         */
	/************************************************************************/
	const TCHAR CAlertConfig::PrivateMsgKey[] = _T("_PrivateMsgKey");
	const TCHAR CAlertConfig::CommentMsgKey[] = _T("_CommentMsgKey");
	const TCHAR CAlertConfig::PraiseMsgKey[]  = _T("_PraiseMsgKey");
	CAlertConfig::CAlertConfig()
	{		
#ifdef MinVersion
		_enablePrivateMsg = false;
		_enableCommentMsg = false;
		_enablePraiseMsg  = false;
#else
		_enablePrivateMsg = true;
		_enableCommentMsg = true;
		_enablePraiseMsg  = true;
#endif
		
	}
	CAlertConfig::~CAlertConfig()
	{

	}
	bool CAlertConfig::Load()
	{
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;

		theDB->GetDictValue(PrivateMsgKey,_enablePrivateMsg);
		theDB->GetDictValue(CommentMsgKey,_enableCommentMsg);
		theDB->GetDictValue(PraiseMsgKey,_enablePraiseMsg);
		return true;
	}

	bool CAlertConfig::Save()
	{
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;
		if(!IsModify()) return true;

		typedef struct _tagKeyValue_
		{
			const TCHAR*   key;
			bool           bvalue;
		}KeyValue;

		KeyValue keyValues[] = 
		{
			{PrivateMsgKey,_enablePrivateMsg},
			{CommentMsgKey,_enableCommentMsg},
			{PraiseMsgKey,_enablePraiseMsg}
		};

		theDB->BeginTransaction();
		for (size_t i = 0;i<sizeof(keyValues)/sizeof(keyValues[0]);++i)
		{
			theDB->DeleteDict(keyValues[i].key);
			theDB->InsertDict(keyValues[i].key,(int)keyValues[i].bvalue);
		}
		theDB->Commit();
		Modify(false);

		return true;
	}

	bool CAlertConfig::IsEnablePrivateMsg()const
	{
		return _enablePrivateMsg;
	}
	void CAlertConfig::EnablePrivateMsg(bool bEnable/* = true*/)
	{
		if(_enablePrivateMsg != bEnable)
		{
			_enablePrivateMsg = bEnable;
			Modify();
		}
	}

	bool CAlertConfig::IsEnableCommentMsg()const
	{
		return _enableCommentMsg;
	}
	void CAlertConfig::EnableCommentMsg(bool bEnable /*= true*/)
	{
		if(_enableCommentMsg != bEnable)
		{
			_enableCommentMsg = bEnable;
			Modify();
		}
	}

	bool CAlertConfig::IsEnablePraiseMsg()const
	{
		return _enablePraiseMsg;
	}
	void CAlertConfig::EnablePraiseMsg(bool bEnable /*= true*/)
	{
		if(_enablePraiseMsg != bEnable)
		{
			_enablePraiseMsg = bEnable;
			Modify();
		}
	}

	/************************************************************************/
	/*                     热键设置                                         */
	/************************************************************************/
	const TCHAR CHotKeyConfig::ChangeModeHotKey[] = _T("_ChangeModeKey");
	const TCHAR CHotKeyConfig::LrcWindowHotKey[] = _T("_OpenLrcKey");
	const TCHAR CHotKeyConfig::PlayControl[] = _T("_PlayControlKey");
	const TCHAR CHotKeyConfig::PlayPreSong[] = _T("_PlayPreSongKey");
	const TCHAR CHotKeyConfig::PlayNextSong[] = _T("_PlayNextSongKey");
	const TCHAR CHotKeyConfig::UpVolKey[] = _T("_UpVolKey");
	const TCHAR CHotKeyConfig::DownVolKey[] = _T("_DownVolKey");
	const TCHAR CHotKeyConfig::MusicLibWindowHotKey[] = _T("_OpenMusicLibKey");

	CHotKeyConfig::CHotKeyConfig()
	{
		_dwChangeModeHotKey = 1617;
		_dwLrcWindowHotKey = 1604;
		_dwPlayControlHotKey = 1652;
		_dwPreSongHotKey = 3621;
		_dwNextSongHotKey = 3623;
		_dwUpVolHotKey = 3622;
		_dwDownVolHotKey = 3624;
		_dwMusiclibWindowHotKey = 3628;
	}
	CHotKeyConfig::~CHotKeyConfig()
	{
		
	}
	bool CHotKeyConfig::Load()
	{
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;

		theDB->GetDictValue(ChangeModeHotKey,_dwChangeModeHotKey);
		theDB->GetDictValue(LrcWindowHotKey,_dwLrcWindowHotKey);
		theDB->GetDictValue(PlayControl,_dwPlayControlHotKey);
		theDB->GetDictValue(PlayPreSong,_dwPreSongHotKey);
		theDB->GetDictValue(PlayNextSong,_dwNextSongHotKey);
		theDB->GetDictValue(UpVolKey,_dwUpVolHotKey);
		theDB->GetDictValue(DownVolKey,_dwDownVolHotKey);
		theDB->GetDictValue(MusicLibWindowHotKey,_dwMusiclibWindowHotKey);
		return true;
	}
	bool CHotKeyConfig::Save()
	{
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;
		if(!IsModify()) return true;

		typedef struct _tagKeyValue_
		{
			const TCHAR*   key;
			DWORD          value;
		}KeyValue;

		KeyValue keyValues[] = 
		{
			{ChangeModeHotKey,_dwChangeModeHotKey},
			{LrcWindowHotKey,_dwLrcWindowHotKey},
			{PlayControl,_dwPlayControlHotKey},
			{PlayPreSong,_dwPreSongHotKey},
			{PlayNextSong,_dwNextSongHotKey},
			{UpVolKey,_dwUpVolHotKey},
			{DownVolKey,_dwDownVolHotKey},
			{MusicLibWindowHotKey,_dwMusiclibWindowHotKey}
		};

		theDB->BeginTransaction();
		for (size_t i = 0;i<sizeof(keyValues)/sizeof(keyValues[0]);++i)
		{
			theDB->DeleteDict(keyValues[i].key);
			theDB->InsertDict(keyValues[i].key,(int)keyValues[i].value);
		}
		theDB->Commit();
		Modify(false);

		return true;
	}

	void CHotKeyConfig::RegAllHotKey()
	{
		if(!RegHotKey(_dwChangeModeHotKey))
			_dwChangeModeHotKey = 0;
		if(!RegHotKey(_dwLrcWindowHotKey))
			_dwLrcWindowHotKey = 0;
		if(!RegHotKey(_dwPlayControlHotKey))
			_dwPlayControlHotKey = 0;
		if(!RegHotKey(_dwPreSongHotKey))
			_dwPreSongHotKey = 0;
		if(!RegHotKey(_dwNextSongHotKey))
			_dwNextSongHotKey = 0;
		if(!RegHotKey(_dwUpVolHotKey))
			_dwUpVolHotKey = 0;
		if(!RegHotKey(_dwDownVolHotKey))
			_dwDownVolHotKey = 0;
		if(!RegHotKey(_dwMusiclibWindowHotKey))
			_dwMusiclibWindowHotKey = 0;
	}

	bool CHotKeyConfig::RegHotKey(DWORD dwkey)
	{
		WORD wVirtualKeyCode = LOBYTE(LOWORD(dwkey));
		WORD wModifiers = HIBYTE(LOWORD(dwkey));
		UINT fsModifiers = CSys::HKF2SDK(wModifiers);	
		BOOL bRet = ::RegisterHotKey(theMainWin->GetHWND(),dwkey,fsModifiers,wVirtualKeyCode);
		//assert(bRet);
		return !!bRet;
	}

	DWORD CHotKeyConfig::GetChangeModeHotKey() const
	{
		return _dwChangeModeHotKey;
	}
	void CHotKeyConfig::SetChangeModeHotKey(DWORD dwkey)
	{
		if (_dwChangeModeHotKey!=dwkey)
		{
			//取消原先的注册热键;
			::UnregisterHotKey(theMainWin->GetHWND(),_dwChangeModeHotKey);
			
			_dwChangeModeHotKey = dwkey;
			RegHotKey(dwkey);
			Modify(true);		
		}
	}
	DWORD CHotKeyConfig::GetLrcWindowHotKey() const
	{
		return _dwLrcWindowHotKey;
	}
	void CHotKeyConfig::SetLrcWindowHotKey(DWORD dwkey)
	{
		if (_dwLrcWindowHotKey!=dwkey)
		{
			::UnregisterHotKey(theMainWin->GetHWND(),_dwLrcWindowHotKey);

			_dwLrcWindowHotKey = dwkey;
			RegHotKey(dwkey);
			Modify(true);	
		}
	}
	DWORD CHotKeyConfig::GetPlayControlHotKey() const
	{
		return _dwPlayControlHotKey;
	}
	void CHotKeyConfig::SetPlayControlHotKey(DWORD dwkey)
	{
		if (_dwPlayControlHotKey!=dwkey)
		{
			::UnregisterHotKey(theMainWin->GetHWND(),_dwPlayControlHotKey);

			_dwPlayControlHotKey = dwkey;
			RegHotKey(dwkey);
			Modify(true);		
		}
	}
	DWORD CHotKeyConfig::GetPlayPreHotKey() const
	{
		return _dwPreSongHotKey;
	}
	void CHotKeyConfig::SetPlayPreHotKey(DWORD dwkey)
	{
		if (_dwPreSongHotKey!=dwkey)
		{
			::UnregisterHotKey(theMainWin->GetHWND(),_dwPreSongHotKey);

			_dwPreSongHotKey = dwkey;
			RegHotKey(dwkey);
			Modify(true);
		}
	}
	DWORD CHotKeyConfig::GetPlayNextHotKey() const
	{
		return _dwNextSongHotKey;
	}
	void CHotKeyConfig::SetPlayNextHotKey(DWORD dwkey)
	{
		if (_dwNextSongHotKey!=dwkey)
		{
			::UnregisterHotKey(theMainWin->GetHWND(),_dwNextSongHotKey);

			_dwNextSongHotKey = dwkey;
			RegHotKey(dwkey);
			Modify(true);		
		}
	}
	DWORD CHotKeyConfig::GetUpVolHotKey() const
	{
		return _dwUpVolHotKey;
	}
	void CHotKeyConfig::SetUpVolHotKey(DWORD dwkey)
	{
		if (_dwUpVolHotKey!=dwkey)
		{
			::UnregisterHotKey(theMainWin->GetHWND(),_dwUpVolHotKey);

			_dwUpVolHotKey = dwkey;
			RegHotKey(dwkey);
			Modify(true);			
		}
	}
	DWORD CHotKeyConfig::GetDownVolHotKey() const
	{
		return _dwDownVolHotKey;
	}
	void CHotKeyConfig::SetDownVolHotKey(DWORD dwkey)
	{
		if (_dwDownVolHotKey!=dwkey)
		{
			::UnregisterHotKey(theMainWin->GetHWND(),_dwDownVolHotKey);

			_dwDownVolHotKey = dwkey;
			RegHotKey(dwkey);
			Modify(true);			
		}
	}

	DWORD CHotKeyConfig::GetMusiclibHotKey() const
	{
		return _dwMusiclibWindowHotKey;
	}
	void CHotKeyConfig::SetMusiclibHotKey(DWORD dwkey)
	{
		if (_dwMusiclibWindowHotKey!=dwkey)
		{
			::UnregisterHotKey(theMainWin->GetHWND(),_dwMusiclibWindowHotKey);

			_dwMusiclibWindowHotKey = dwkey;
			RegHotKey(dwkey);
			Modify(true);			
		}
	}


	/************************************************************************/
	/*                缓存设置                                              */
	/************************************************************************/
	const TCHAR CCacheConfig::CacheDirKey[] = _T("_CacheDirKey");
	const TCHAR CCacheConfig::MaxCacheKey[] = _T("_MaxCacheKey");
	static const int MaxCacheSize = 1024;
	static const int MinCacheSize = 512;
	CCacheConfig::CCacheConfig()
	{
		m_sCacheDir = theFilePathMgr->GetDefaultCachePath();
		m_sMaxCacheSpace = _T("512");
	}

	CCacheConfig::~CCacheConfig()
	{
// 		if(IsModify())
// 			Save();
	}

	bool CCacheConfig::Load()
	{
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;

		theDB->GetDictValue(CacheDirKey,m_sCacheDir);
		CString sMax;
		if(theDB->GetDictValue(MaxCacheKey,sMax))
		{
			m_sMaxCacheSpace=sMax;
		}

		return true;
	}

	bool CCacheConfig::Save()
	{

		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;
		if(!IsModify()) return true;

		typedef struct _tagKeyValue_
		{
			const TCHAR*   key;
			CString   svalue;
		}KeyValue;

		KeyValue keyValues[] = 
		{
			{CacheDirKey,m_sCacheDir},
			{MaxCacheKey,m_sMaxCacheSpace}
		};

		theDB->BeginTransaction();
		for (size_t i = 0;i<sizeof(keyValues)/sizeof(keyValues[0]);++i)
		{
			theDB->DeleteDict(keyValues[i].key);
			theDB->InsertDict(keyValues[i].key,keyValues[i].svalue);
		}
		theDB->Commit();
		Modify(false);

		return true;
	}

	CString CCacheConfig::GetCacheDir()
	{
		return m_sCacheDir;
	}

	void CCacheConfig::SetCacheDir(CString sPath)
	{
		sPath.Replace(_T('\\'),_T('/'));
		if (sPath.GetAt(sPath.GetLength()-1) != _T('/'))
		{
			sPath += _T("/");
		}
		if (m_sCacheDir!=sPath)
		{
			theCacheMgr->ClearAllCatch();
			m_sCacheDir = sPath;
			theCacheMgr->Init(sPath);
			Modify(true);
		}	
	}

	CString CCacheConfig::GetMaxCacheSpace()
	{
		return m_sMaxCacheSpace;
	}

	void CCacheConfig::SetMaxCacheSpace(CString& sMax)
	{
		CString size = sMax;
		if (_ttoi(sMax) < MinCacheSize)
			size.Format(_T("%d"), MinCacheSize);
		else if (_ttoi(sMax) > MaxCacheSize)
			size.Format(_T("%d"), MaxCacheSize);
		//end if

		if (m_sMaxCacheSpace!=size)
		{
			m_sMaxCacheSpace = size;
			Modify(true);
		}		
	}

	/************************************************************************/
	/*                                   皮肤设置                           */
	/************************************************************************/
	const TCHAR CSkinConfig::SkinCorKey[] = _T("_SkinCor");
	const TCHAR CSkinConfig::SkinBgKey[] = _T("_SkinBgImage");
	const TCHAR CSkinConfig::SkinUnityBgKey[] = _T("_SkinEnableUnityBg");
	const TCHAR CSkinConfig::SkinStyleKey[]  = _T("_SkinStyle");
	const TCHAR CSkinConfig::SkinItemBgKey[] = _T("_SkinItemBg");
	const TCHAR CSkinConfig::SkinItemHoverBgKey[] = _T("_SkinItemHoverBg");
	const TCHAR CSkinConfig::SkinItemPlayBgKey[]  = _T("_SkinItemPlayBg");
	CSkinConfig::CSkinConfig():m_dwCor(0xFFFFFFFF),m_bEnableUnityBg(false)
	{
	}
	CSkinConfig::~CSkinConfig()
	{

	}

	bool CSkinConfig::Load()
	{
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;

		theDB->GetDictValue(SkinCorKey,m_dwCor);
		theDB->GetDictValue(SkinBgKey,m_sBg);
		theDB->GetDictValue(SkinUnityBgKey,m_bEnableUnityBg);
		theDB->GetDictValue(SkinStyleKey,m_sStyle);
		theDB->GetDictValue(SkinItemBgKey,m_sItemBg);
		theDB->GetDictValue(SkinItemHoverBgKey,m_sItemHoverBg);
		theDB->GetDictValue(SkinItemPlayBgKey,m_sItemPlayBg);

// 		CPaintManagerUI::EnableUnityBg(m_bEnableUnityBg);
// 		CPaintManagerUI::SetUnityBgColor(m_dwCor);
// 		CPaintManagerUI::SetUnityBgImage(m_sBg);
		return true;
	}
	bool CSkinConfig::Save()
	{
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;
		if(!IsModify()) return true;

		theDB->BeginTransaction();
		theDB->DeleteDict(SkinCorKey);
		theDB->DeleteDict(SkinBgKey);
		theDB->DeleteDict(SkinUnityBgKey);
		theDB->DeleteDict(SkinStyleKey);
		theDB->DeleteDict(SkinItemBgKey);
		theDB->DeleteDict(SkinItemHoverBgKey);
		theDB->DeleteDict(SkinItemPlayBgKey);

		theDB->InsertDict(SkinCorKey,m_dwCor);
		theDB->InsertDict(SkinBgKey,m_sBg);
		theDB->InsertDict(SkinUnityBgKey,m_bEnableUnityBg);
		theDB->InsertDict(SkinStyleKey,m_sStyle);
		theDB->InsertDict(SkinItemBgKey,m_sItemBg);
		theDB->InsertDict(SkinItemHoverBgKey,m_sItemHoverBg);
		theDB->InsertDict(SkinItemPlayBgKey,m_sItemPlayBg);

		theDB->Commit();
		Modify(false);

		return true;
	}
	DWORD CSkinConfig::GetColor() const
	{
		return m_dwCor;
	}
	void CSkinConfig::SetColor(DWORD dwCor)
	{
		if(dwCor != m_dwCor)
		{
			m_dwCor = dwCor;
			m_sBg.Empty();
			Modify();
			//theNotifyCenter->sendNotifyOnMainThread(NT_SkinChanged);
//			CPaintManagerUI::SetUnityBgColor(dwCor);
		}
	}

	CString CSkinConfig::GetBg() const
	{
		return m_sBg;
	}
	void CSkinConfig::SetBg(LPCTSTR lpszName)
	{
		if(0 != m_sBg.Compare(lpszName))
		{
			m_sBg = lpszName;
			Modify();
			//theNotifyCenter->sendNotifyOnMainThread(NT_SkinChanged);
//			CPaintManagerUI::SetUnityBgImage(lpszName);
		}
	}
	CString CSkinConfig::GetStyleName() const
	{
		return m_sStyle;
	}
	void CSkinConfig::SetStyleName(LPCTSTR lpszName)
	{
		if(0 != m_sStyle.Compare(lpszName))
		{
			m_sStyle = lpszName;
			Modify();
		}
	}
	bool CSkinConfig::IsEnableUnityBg() const
	{
		return m_bEnableUnityBg;
	}
	void CSkinConfig::EnableUnityBg(bool b/* = true*/)
	{
		if(m_bEnableUnityBg != b)
		{
			m_bEnableUnityBg = b;
//			CPaintManagerUI::EnableUnityBg(b);
		}
	}

	CString CSkinConfig::GetItemBg() const
	{
		return m_sItemBg;
	}
	void CSkinConfig::SetItemBg(LPCTSTR lpszName)
	{
		if(0 != m_sItemBg.Compare(lpszName))
		{
			m_sItemBg = lpszName;
			Modify();
		}
	}

	CString CSkinConfig::GetItemHoverBg() const
	{
		return m_sItemHoverBg;
	}
	void CSkinConfig::SetItemHoverBg(LPCTSTR lpszName)
	{
		if(0 != m_sItemHoverBg.Compare(lpszName))
		{
			m_sItemHoverBg = lpszName;
			Modify();
		}
	}

	CString CSkinConfig::GetItemPlayBg() const
	{
		return m_sItemPlayBg;
	}
	void CSkinConfig::SetItemPlayBg(LPCTSTR lpszName)
	{
		if(0 != m_sItemPlayBg.Compare(lpszName))
		{
			m_sItemPlayBg = lpszName;
			Modify();
		}
	}
	/************************************************************************/
	/*                                   软件更新设置                       */
	/************************************************************************/
	const TCHAR CUpdateConfig::AutoUpdateKey[] = _T("_AutoUpdate");

	CUpdateConfig::CUpdateConfig()
		:m_autoUpdate(false)
	{

	}
	CUpdateConfig::~CUpdateConfig()
	{

	}

	bool CUpdateConfig::Load()
	{
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;

		theDB->GetDictValue(AutoUpdateKey,m_autoUpdate);

		return true;
	}
	bool CUpdateConfig::Save()
	{
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;
		if(!IsModify()) return true;

		theDB->BeginTransaction();
		theDB->DeleteDict(AutoUpdateKey);
		theDB->InsertDict(AutoUpdateKey,m_autoUpdate);
		theDB->Commit();
		Modify(false);

		return true;
	}

	bool CUpdateConfig::IsAutoUpdate() const
	{
		return m_autoUpdate;
	}

	void CUpdateConfig::SetAutoUpdate( bool enable )
	{
		if (m_autoUpdate != enable)
		{
			m_autoUpdate = enable;
			Modify(true);
		}	
	}
	/************************************************************************/
	/*                                   歌词设置                           */
	/************************************************************************/
	const TCHAR CLrcConfig::LrcPanelFontNameKey[] = _T("_LrcPanelFontName");
	const TCHAR CLrcConfig::LrcPanelFontStyleKey[] = _T("_LrcPanelFontStyle");
	const TCHAR CLrcConfig::LrcPanelFontSizeKey[] = _T("_LrcPanelFontSize");

	const TCHAR CLrcConfig::LrcPanelPlayedCorKey[] = _T("_LrcPanelPlayedCor");
	const TCHAR CLrcConfig::LrcPanelUnPlayedCorKey[] = _T("_LrcPanelUnPlayedCor");
	const TCHAR CLrcConfig::LrcPanelPlayingCorKey[] = _T("_LrcPanelPlayingCor");

	const TCHAR CLrcConfig::LrcDeskFontNameKey[] = _T("_LrcDeskFontName");
	const TCHAR CLrcConfig::LrcDeskFontStyleKey[] = _T("_LrcDeskFontStyle");
	const TCHAR CLrcConfig::LrcDeskFontSizeKey[] = _T("_LrcDeskFontSize");
	const TCHAR CLrcConfig::LrcDeskPlayedCorKey[] = _T("_LrcDeskPlayedCor");
	const TCHAR CLrcConfig::LrcDeskUnPlayedCorKey[] = _T("_LrcDeskUnPlayedCor");
	CLrcConfig::CLrcConfig()
		: m_dLrcPanelFontSize(10)
		, m_dLrcPanelFontStyle(NormalFont)
		, m_dLrcDeskFontSize(50)
		, m_dLrcDeskFontStyle(NormalFont)
	{

	}
	CLrcConfig::~CLrcConfig()
	{
		Save();
	}
	bool CLrcConfig::Load()
	{
		UpdateDB();
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;

		theDB->GetDictValue(LrcPanelFontNameKey,m_sLrcPanelFontName);//
		DWORD fontStyle = NormalFont;
		theDB->GetDictValue(LrcPanelFontStyleKey,fontStyle);
		m_dLrcPanelFontStyle = (LrcFontStyle)fontStyle;
		theDB->GetDictValue(LrcPanelFontSizeKey,m_dLrcPanelFontSize);//
		theDB->GetDictValue(LrcPanelPlayedCorKey,m_dLrcPanelPlayedCor);//
		theDB->GetDictValue(LrcPanelUnPlayedCorKey,m_dLrcPanelUnPlayedCor);//
		theDB->GetDictValue(LrcPanelPlayingCorKey,m_dLrcPanelPlayingCor);//
		theDB->GetDictValue(LrcDeskFontNameKey,m_sLrcDeskFontName);//
		fontStyle = NormalFont;
		theDB->GetDictValue(LrcDeskFontStyleKey,fontStyle);
		m_dLrcDeskFontStyle = (LrcFontStyle)fontStyle;

		theDB->GetDictValue(LrcDeskFontSizeKey,m_dLrcDeskFontSize);
		theDB->GetDictValue(LrcDeskPlayedCorKey,m_dLrcDeskPlayedCor);
		theDB->GetDictValue(LrcDeskUnPlayedCorKey,m_dLrcDeskUnPlayedCor);	

		return true;
	}
	bool CLrcConfig::Save()
	{
		spUserDBaseT theDB = GetDB();
		assert(theDB);
		if(!theDB) return false;
		if(!IsModify()) return true;

		typedef struct _tagKeyValue
		{
			const TCHAR*   key;
			CString   svalue;
		}KeyValue;

		KeyValue keyValues[] = 
		{
			{LrcPanelFontNameKey,m_sLrcPanelFontName},
			{LrcDeskFontNameKey,m_sLrcDeskFontName},
		};
		typedef struct _tagKeyValue_
		{
			const TCHAR*  key;
			DWORD         dvalue;
		}KeyValue_;
		KeyValue_ keyValues_[] = 
		{
			{LrcPanelFontStyleKey,(DWORD)m_dLrcPanelFontStyle},
			{LrcPanelFontSizeKey,m_dLrcPanelFontSize},
			{LrcPanelPlayedCorKey,m_dLrcPanelPlayedCor},
			{LrcPanelPlayingCorKey,m_dLrcPanelPlayingCor},
			{LrcPanelUnPlayedCorKey,m_dLrcPanelUnPlayedCor},
			{LrcDeskFontStyleKey,(DWORD)m_dLrcDeskFontStyle},
			{LrcDeskFontSizeKey,m_dLrcDeskFontSize},
			{LrcDeskPlayedCorKey,m_dLrcDeskPlayedCor},
			{LrcDeskUnPlayedCorKey,m_dLrcDeskUnPlayedCor}
		};
		theDB->BeginTransaction();
		for (size_t i = 0;i<sizeof(keyValues)/sizeof(keyValues[0]);++i)
		{
			theDB->DeleteDict(keyValues[i].key);
			theDB->InsertDict(keyValues[i].key,keyValues[i].svalue);
		}
		
		for (size_t i = 0;i<sizeof(keyValues_)/sizeof(keyValues_[0]);++i)
		{
			theDB->DeleteDict(keyValues_[i].key);
			theDB->InsertDict(keyValues_[i].key,keyValues_[i].dvalue);
		}
		theDB->Commit();
		Modify(false);

		return true;
	}
	CString CLrcConfig::GetLrcPanelFontName() const
	{
		return m_sLrcPanelFontName;
	}
	void CLrcConfig::SetLrcPanelFontName(LPCTSTR lpszName)
	{
		if(m_sLrcPanelFontName.Compare(lpszName)!=0)
		{
			m_sLrcPanelFontName = lpszName;
			Modify(true);
		}
	}
	CLrcConfig::LrcFontStyle CLrcConfig::GetLrcPanelFontStyle() const
	{
		return m_dLrcPanelFontStyle;
	}
	void CLrcConfig::SetLrcPanelFontStyle(LrcFontStyle fontStyle)
	{
		if(m_dLrcPanelFontStyle!=fontStyle)
		{
			m_dLrcPanelFontStyle = fontStyle;
			Modify(true);
		}
	}
	DWORD CLrcConfig::GetLrcPanelFontSize() const
	{
		return m_dLrcPanelFontSize;
	}
	void CLrcConfig::SetLrcPanelFontSize(DWORD fontSize)
	{
		if(m_dLrcPanelFontSize!=fontSize)
		{
			m_dLrcPanelFontSize = fontSize;
			Modify(true);
		}
	}
	DWORD CLrcConfig::GetLrcPanelPlayedColor() const
	{
		return m_dLrcPanelPlayedCor;
	}
	void CLrcConfig::SetLrcPanelPlayedColor(DWORD dwCor)
	{
		if(m_dLrcPanelPlayedCor!=dwCor)
		{
			m_dLrcPanelPlayedCor = dwCor;
			Modify(true);
		}
	}
	DWORD CLrcConfig::GetLrcPanelUnPlayedColor() const
	{
		return m_dLrcPanelUnPlayedCor;
	}
	void CLrcConfig::SetLrcPanelUnPlayedColor(DWORD dwCor)
	{
		if(m_dLrcPanelUnPlayedCor!=dwCor)
		{
			m_dLrcPanelUnPlayedCor = dwCor;
			Modify(true);
		}
	}
	DWORD CLrcConfig::GetLrcPanelPlayingColor() const
	{
		return m_dLrcPanelPlayingCor;
	}
	void CLrcConfig::SetLrcPanelPlayingColor(DWORD dwCor)
	{
		if(m_dLrcPanelPlayingCor!=dwCor)
		{
			m_dLrcPanelPlayingCor = dwCor;
			Modify(true);
		}
	}
	CString CLrcConfig::GetLrcDeskFontName() const
	{
		return m_sLrcDeskFontName;
	}
	void CLrcConfig::SetLrcDeskFontName(LPCTSTR lpszName)
	{
		if(m_sLrcDeskFontName.Compare(lpszName)!=0)
		{
			m_sLrcDeskFontName = lpszName;
			Modify(true);
		}
	}

	CLrcConfig::LrcFontStyle CLrcConfig::GetLrcDeskFontStyle()  const
	{
		return m_dLrcDeskFontStyle;
	}
	void CLrcConfig::SetLrcDeskFontStyle(LrcFontStyle fontStyle)
	{
		if(m_dLrcDeskFontStyle!=fontStyle)
		{
			m_dLrcDeskFontStyle = fontStyle;
			Modify(true);
		}
	}

	DWORD CLrcConfig::GetLrcDeskFontSize() const
	{
		return m_dLrcDeskFontSize;
	}
	void CLrcConfig::SetLrcDeskFontSize(DWORD fontSize)
	{
		if(m_dLrcDeskFontSize!=fontSize)
		{
			m_dLrcDeskFontSize = fontSize;
			Modify(true);
		}
	}

	DWORD CLrcConfig::GetLrcDeskPlayedColor() const
	{
		return m_dLrcDeskPlayedCor;
	}
	void CLrcConfig::SetLrcDeskPlayedColor(DWORD dwCor)
	{
		if(m_dLrcDeskPlayedCor!=dwCor)
		{
			m_dLrcDeskPlayedCor = dwCor;
			Modify(true);
		}

	}
	DWORD CLrcConfig::GetLrcDeskUnPlayedColor() const
	{
		return m_dLrcDeskUnPlayedCor;
	}
	void CLrcConfig::SetLrcDeskUnPlayedColor(DWORD dwCor)
	{
		if(m_dLrcDeskUnPlayedCor!=dwCor)
		{
			m_dLrcDeskUnPlayedCor = dwCor;
			Modify(true);
		}
	}