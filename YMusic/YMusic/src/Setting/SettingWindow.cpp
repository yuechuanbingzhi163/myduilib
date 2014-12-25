#include "stdafx.h"
#include "SettingWindow.h"
#include "../YApp.h"
#include <commdlg.h>

//#include "PlayList.h"
#include "LrcPreview.h"


CSettingWindow::CSettingWindow()
:m_bModify(false)
{
}

CSettingWindow::~CSettingWindow()
{

}
CControlUI* CSettingWindow::CreateControl(LPCTSTR pstrClass)
{
	if(_tcscmp(pstrClass,_T("LrcPreView")) == 0)
		return new CLrcPreViewUI;
	return NULL;
}
LRESULT CSettingWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	this->ModifyStyle(WS_CAPTION,WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	this->RegisterSkin(_T("setting-window.xml"),0,this);
	this->GetPaintMgr()->AddNotifier(this);
	CenterWindow();
	init();
	return 0;
}
LRESULT CSettingWindow::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if((TCHAR)wParam != VK_ESCAPE)
	{
		bHandled = FALSE;
		return 0;
	}
	
	cancle();
	return 0;
}

void CSettingWindow::Notify(TNotifyUI& msg)
{
	YTrace_(_T("class:%s,id:%s,msg:%s"),msg.pSender->GetClass(),msg.pSender->GetName().GetData(),msg.sType.GetData());
	CControlUI* pSender = msg.pSender;
	if(0 == msg.sType.Compare(kSelectChanged))
	{
		if(pSender == ui.ConfigAbout_Option)
			ui.PanelItemTab_TabLayout->SelectItem(AboutPage);
		else if (pSender == ui.ConfigNormal_Option)
			ui.PanelItemTab_TabLayout->SelectItem(NormalPage);
		else if(pSender == ui.ConfigLrcPanel_Option)
			ui.PanelItemTab_TabLayout->SelectItem(LrcPanelPage);
		else if(pSender == ui.ConfigLrcDesk_Option)
			ui.PanelItemTab_TabLayout->SelectItem(LrcDeskPage);
		else if (pSender == ui.ConfigCache_Option)
			ui.PanelItemTab_TabLayout->SelectItem(CachePage);
		else if (pSender == ui.ConfigHotKey_Option)
			ui.PanelItemTab_TabLayout->SelectItem(HotKeyPage);
		else if (pSender == ui.ConfigUpdate_Option)
			ui.PanelItemTab_TabLayout->SelectItem(UpdataPage);
		else if ((pSender == ui.MinTrayIcon_Option)
			|| (pSender == ui.Exit_Option)
			)
		{
			setModify(true);
		}		
	}
	else if (0 == msg.sType.Compare(kItemSelect))
	{
		CLrcPreViewUI* pLrcPreView = static_cast<CLrcPreViewUI*>(GetPaintMgr()->FindControl(_T("lrcPreView")));
		ASSERT(pLrcPreView);
		CConfigMgr::spLrcConfigT pLrcCfg = theConfigMgr->GetLrcConfig();
		if(msg.pSender->IsName(_T("font_name2")))
		{
			pLrcPreView->m_iFontSize = pLrcCfg->GetLrcDeskFontSize();
			pLrcPreView->m_iFontStyle = pLrcCfg->GetLrcDeskFontStyle();
			pLrcPreView->m_sFontName = ui.LrcDeskFontName->GetText();
			pLrcPreView->m_dwFontColor = pLrcCfg->GetLrcDeskUnPlayedColor();
		}
		else if (msg.pSender->IsName(_T("font_style2")))
		{
			pLrcPreView->m_iFontSize = pLrcCfg->GetLrcDeskFontSize();
			pLrcPreView->m_iFontStyle = ui.LrcDeskFontStyle->GetCurSel();
			pLrcPreView->m_sFontName = pLrcCfg->GetLrcDeskFontName();
			pLrcPreView->m_dwFontColor = pLrcCfg->GetLrcDeskUnPlayedColor();
		}
		else if (msg.pSender->IsName(_T("font_size2")))
		{
			pLrcPreView->m_iFontSize = mci::ToUInt(ui.LrcDeskFontSize->GetText());
			pLrcPreView->m_iFontStyle = pLrcCfg->GetLrcDeskFontStyle();
			pLrcPreView->m_sFontName = ui.LrcDeskFontName->GetText();
			pLrcPreView->m_dwFontColor = pLrcCfg->GetLrcDeskUnPlayedColor();
		}
		
		pLrcPreView->Invalidate();
//		setModify(true);
	}
	else if(0 == msg.sType.Compare(kClick))
	{
		if ((pSender == ui.Close_Btn)
			|| (pSender == ui.Cancle_Btn)
			)
			cancle();
		else if (pSender == ui.Ok_Btn)
			ok();
		else if (pSender == ui.Apply_Btn)
			apply();
		else if ((pSender == ui.AutoLogin_CheckBox)
			|| (pSender == ui.AutoPlay_CheckBox)
			|| (pSender == ui.AutoRun_CheckBox)
			|| (pSender == ui.AutoOpenLRC_CheckBox)
			|| (pSender == ui.PlayRoaming_CheckBox)
			|| (pSender == ui.RelateFile_CheckBox)
			|| (pSender == ui.AutoUpdate_CheckBox)
			|| (pSender == ui.LrcDeskKaraok)
			)
		{
			setModify(true);
		}
		else if (pSender == ui.CheckUpdate_Btn)
		{
			//TODO:
		}
		else if (pSender == ui.ChangeDir_Btn)
		{
			CString sSelectedDir;
		/*	CFolderDialog fldDlg (GetHWND(), _T("选择目录"),
				BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE );

			if ( IDOK == fldDlg.DoModal())
			{
				sSelectedDir = fldDlg.m_szFolderPath;
				if (sSelectedDir.GetLength()!=0)
				{
					ui.CacheDir_Edit->SetText(sSelectedDir);
					setModify(true);
				}				
			}*/
		}
		else if (pSender == ui.OpenDir_Btn)
		{
			LPCTSTR path = ui.CacheDir_Edit->GetText().GetData();
			ShellExecute(GetHWND(),_T("open"),path,NULL,NULL,SW_SHOWNORMAL);
		}
		else if (pSender == ui.ClearCache_Btn)
		{
			theCacheMgr->ClearAllCatch();
			CString str;
			str.Format(_T("已缓存%dM"), theCacheMgr->GetAllCatchSize()/1024/1024);
			ui.CachedLabel->SetText(str);
		}
		else if ((pSender == ui.ColorPicker_Btn1)
			||(pSender == ui.ColorPicker_Btn2)
			||(pSender == ui.ColorPicker_Btn3)
			||(pSender == ui.ColorPicker_Btn4)
			||(pSender == ui.ColorPicker_Btn5)
			)
		{
			colorBox(pSender);
		}
	}
	else if(0 == msg.sType.Compare(kTextChanged))
	{
		if ((pSender == ui.CacheDir_Edit)
			|| (pSender == ui.MaxCache_Edit)
			|| pSender->IsName(_T("HK_Mode"))
			|| pSender->IsName(_T("HK_Lrc"))
			|| pSender->IsName(_T("HK_PlayPause"))
			|| pSender->IsName(_T("HK_PreSong"))
			|| pSender->IsName(_T("HK_NextSong"))
			|| pSender->IsName(_T("HK_VolUp"))
			|| pSender->IsName(_T("HK_VolDown"))
			)
		{
			setModify(true);
		}
	}
	else if(0 == msg.sType.Compare(kWindowInit))
	{
		CString str;
		str.Format(_T("已缓存%dM"), theCacheMgr->GetAllCatchSize()/1024/1024);
		ui.CachedLabel->SetText(str);
	}
}

void CSettingWindow::ok()
{
	if (m_bModify)
	{
		apply();
	}
	Close();
}

void CSettingWindow::cancle()
{
	if (m_bModify)
	{
		//TODO:
	}
	Close();
}
DWORD CSettingWindow::dwFromText(LPCTSTR lpszValue)
{
	ASSERT(lpszValue);
	DWORD dwValue = 0;
	CString szValue(lpszValue);
	if(szValue==_T("常规"))
		dwValue = 0;
	else if (szValue==_T("粗体"))
		dwValue = 1;
	else if(szValue==_T("斜体"))
		dwValue = 2;
	else if(szValue==_T("粗体|斜体"))
		dwValue = 3;

	return dwValue;
}
int CSettingWindow::intFromText(LPCTSTR lpszValue,CComboBoxUI* pCombo)
{
	ASSERT(lpszValue&&pCombo);
	int iIndex = -1;
	for (int i=0;i<pCombo->GetCount();++i)
	{
		if(pCombo->GetItemAt(i)->GetText().Compare(lpszValue)==0)
		{
			pCombo->GetItemAt(i)->SetVisible(true);
			pCombo->GetItemAt(i)->SetInternVisible(true);
			iIndex = i;
			break;
		}
	}
	return iIndex;

}
void CSettingWindow::apply()
{
	CConfigMgr::spNormalConfigT normalConfig = theConfigMgr->GetNormalConfig();
	CConfigMgr::spCacheConfigT cacheConfig = theConfigMgr->GetCacheConfig();
	CConfigMgr::spHotKeyConfigT hotKeyConfig = theConfigMgr->GetHotKeyConfig();
	CConfigMgr::spUpdateConfigT updateCongfig = theConfigMgr->GetUpdateConfig();
	CConfigMgr::spLrcConfigT  lrcConfig = theConfigMgr->GetLrcConfig();
	bool enable;
	CString text;

	//常规设置
	enable = ui.AutoLogin_CheckBox->GetCheck();
	normalConfig->EnableAutoLogin(enable);
	enable = ui.AutoPlay_CheckBox->GetCheck();
	normalConfig->EnableAutoPlay(enable);
	enable = ui.AutoRun_CheckBox->GetCheck();
	normalConfig->EnableAutoStart(enable);
	enable = ui.AutoOpenLRC_CheckBox->GetCheck();
	normalConfig->EnableAutoOpenLrc(enable);
	enable = ui.MinTrayIcon_Option->IsSelected();
	normalConfig->SetCloseMode(enable);

	enable = ui.PlayRoaming_CheckBox->GetCheck();
	normalConfig->EnablePlayRoaming(enable);
	enable = ui.RelateFile_CheckBox->GetCheck();
	normalConfig->EnableAllRelate(enable);
	enable = ui.LrcDeskKaraok->GetCheck();
	normalConfig->SetKaraokMode(enable);

	normalConfig->Save();
	//歌词设置
	CString lrcFontName;
	lrcFontName = ui.LrcPanelFontName->GetText();
	lrcConfig->SetLrcPanelFontName(lrcFontName);
	lrcFontName = ui.LrcDeskFontName->GetText();
	lrcConfig->SetLrcDeskFontName(lrcFontName);
	DWORD dwValue = ui.LrcPanelFontStyle->GetCurSel();
	lrcConfig->SetLrcPanelFontStyle((CLrcConfig::LrcFontStyle)dwValue);
	dwValue = ui.LrcDeskFontStyle->GetCurSel();
	lrcConfig->SetLrcDeskFontStyle((CLrcConfig::LrcFontStyle)dwValue);
	dwValue = mci::ToUInt(ui.LrcPanelFontSize->GetText().GetData());
	lrcConfig->SetLrcPanelFontSize(dwValue);
	dwValue = mci::ToUInt(ui.LrcDeskFontSize->GetText().GetData());
	lrcConfig->SetLrcDeskFontSize(dwValue);
	dwValue = ui.Color_Label1->GetBkColor();
	lrcConfig->SetLrcPanelPlayedColor(dwValue);
	dwValue = ui.Color_Label2->GetBkColor();
	lrcConfig->SetLrcPanelUnPlayedColor(dwValue);
	dwValue = ui.Color_Label3->GetBkColor();
	lrcConfig->SetLrcPanelPlayingColor(dwValue);
	dwValue = ui.Color_Label4->GetBkColor();
	lrcConfig->SetLrcDeskPlayedColor(dwValue);
	dwValue = ui.Color_Label5->GetBkColor();
	lrcConfig->SetLrcDeskUnPlayedColor(dwValue);

	lrcConfig->Save();
	//缓存设置
	text = ui.CacheDir_Edit->GetText().GetData();
	cacheConfig->SetCacheDir(text);
	text = ui.MaxCache_Edit->GetText().GetData();
	cacheConfig->SetMaxCacheSpace(text);

	cacheConfig->Save();
	text = cacheConfig->GetMaxCacheSpace();
	ui.MaxCache_Edit->SetText(text);

	//热键设置
	DWORD hotKey = 0;
	hotKey = ui.HK_Mode->GetHotKey();
	hotKeyConfig->SetChangeModeHotKey(hotKey);
	hotKey = ui.HK_Lrc->GetHotKey();
	hotKeyConfig->SetLrcWindowHotKey(hotKey);
	hotKey = ui.HK_PlayPause->GetHotKey();
	hotKeyConfig->SetPlayControlHotKey(hotKey);
	hotKey = ui.HK_PreSong->GetHotKey();
	hotKeyConfig->SetPlayPreHotKey(hotKey);
	hotKey = ui.HK_NextSong->GetHotKey();
	hotKeyConfig->SetPlayNextHotKey(hotKey);
	hotKey = ui.HK_VolUp->GetHotKey();
	hotKeyConfig->SetUpVolHotKey(hotKey);
	hotKey = ui.HK_VolDown->GetHotKey();
	hotKeyConfig->SetDownVolHotKey(hotKey);

	hotKeyConfig->Save();

	//软件更新
	enable = ui.AutoUpdate_CheckBox->GetCheck();
	updateCongfig->SetAutoUpdate(enable);

	updateCongfig->Save();

	setModify(false);
}

void CSettingWindow::setModify(bool isModify)
{
	m_bModify = isModify;
	ui.Apply_Btn->SetEnabled(isModify);
}

void CSettingWindow::init()
{
	initUiStruct();

	CConfigMgr::spNormalConfigT normalConfig = theConfigMgr->GetNormalConfig();
	CConfigMgr::spCacheConfigT cacheConfig = theConfigMgr->GetCacheConfig();
	CConfigMgr::spHotKeyConfigT hotKeyConfig = theConfigMgr->GetHotKeyConfig();
	CConfigMgr::spUpdateConfigT updateCongfig = theConfigMgr->GetUpdateConfig();
	CConfigMgr::spSkinConfigT pSkinCfg = theConfigMgr->GetSkinConfig();
	CConfigMgr::spLrcConfigT pLrcCfg = theConfigMgr->GetLrcConfig();

	CString text;
	bool isCheck = false;
	
	//常规设置
	isCheck = normalConfig->IsEnableAutoLogin();//1 自动登录
	ui.AutoLogin_CheckBox->SetCheck(isCheck);

	isCheck = normalConfig->IsEnableAutoPlay();//2 自动播放
	ui.AutoPlay_CheckBox->SetCheck(isCheck);

	isCheck = normalConfig->IsEnableAutoStart();//3 开机启动	
	ui.AutoRun_CheckBox->SetCheck(isCheck);

	isCheck = normalConfig->IsEnableAutoOpenLrc();//4 自动打开歌词	
	ui.AutoOpenLRC_CheckBox->SetCheck(isCheck);


	isCheck = normalConfig->IsPlayRoaming();//5 自动播放漫游列表
	ui.PlayRoaming_CheckBox->SetCheck(isCheck);

	isCheck = normalConfig->IsAllRelate();//6 文件关联
	ui.RelateFile_CheckBox->SetCheck(isCheck);

	isCheck = normalConfig->IsCloseToTray();//7 关闭按钮行为
	isCheck? ui.MinTrayIcon_Option->Selected(isCheck) : ui.Exit_Option->Selected(!isCheck);
	//歌词设置
	isCheck = normalConfig->IsKaraokMode();//卡拉ok
	ui.LrcDeskKaraok->SetCheck(isCheck);
	CString sName = pLrcCfg->GetLrcPanelFontName();
	int iIndex = intFromText(sName,ui.LrcPanelFontName);
	if(iIndex>-1) ui.LrcPanelFontName->SelectItem(iIndex),ui.LrcPanelFontName->SetText(sName);
	sName = pLrcCfg->GetLrcDeskFontName();
	iIndex = intFromText(sName,ui.LrcDeskFontName);
	if(iIndex>-1) ui.LrcDeskFontName->SelectItem(iIndex),ui.LrcDeskFontName->SetText(sName);
	iIndex = pLrcCfg->GetLrcPanelFontStyle();
	ui.LrcPanelFontStyle->GetItemAt(iIndex)->SetVisible(true),ui.LrcPanelFontStyle->GetItemAt(iIndex)->SetInternVisible(true),ui.LrcPanelFontStyle->SelectItem(iIndex);
	iIndex = pLrcCfg->GetLrcDeskFontStyle();
	ui.LrcDeskFontStyle->GetItemAt(iIndex)->SetVisible(true),ui.LrcDeskFontStyle->GetItemAt(iIndex)->SetInternVisible(true),ui.LrcDeskFontStyle->SelectItem(iIndex);
	sName = mci::ToString((long)pLrcCfg->GetLrcPanelFontSize());
	iIndex = intFromText(sName,ui.LrcPanelFontSize);
	if(iIndex>-1) ui.LrcPanelFontSize->SelectItem(iIndex);
	sName = mci::ToString((long)pLrcCfg->GetLrcDeskFontSize());
	iIndex = intFromText(sName,ui.LrcDeskFontSize);
	if(iIndex>-1) ui.LrcDeskFontSize->SelectItem(iIndex);
	DWORD dwColor = pLrcCfg->GetLrcPanelPlayedColor();
	ui.Color_Label1->SetBkColor(dwColor);
	dwColor = pLrcCfg->GetLrcPanelUnPlayedColor();
	ui.Color_Label2->SetBkColor(dwColor);
	dwColor = pLrcCfg->GetLrcPanelPlayingColor();
	ui.Color_Label3->SetBkColor(dwColor);
	dwColor = pLrcCfg->GetLrcDeskPlayedColor();
	ui.Color_Label4->SetBkColor(dwColor);
	dwColor = pLrcCfg->GetLrcDeskUnPlayedColor();
	ui.Color_Label5->SetBkColor(dwColor);
	//缓存设置
	text = cacheConfig->GetCacheDir();
	ui.CacheDir_Edit->SetText(text);

	text = cacheConfig->GetMaxCacheSpace();
	ui.MaxCache_Edit->SetText(text);

	//热键设置
	DWORD hotKey = 0;
	hotKey = hotKeyConfig->GetChangeModeHotKey();
	ui.HK_Mode->SetHotKey(hotKey);
	hotKey = hotKeyConfig->GetLrcWindowHotKey();
	ui.HK_Lrc->SetHotKey(hotKey);
	hotKey = hotKeyConfig->GetPlayControlHotKey();
	ui.HK_PlayPause->SetHotKey(hotKey);
	hotKey = hotKeyConfig->GetPlayPreHotKey();
	ui.HK_PreSong->SetHotKey(hotKey);
	hotKey = hotKeyConfig->GetPlayNextHotKey();
	ui.HK_NextSong->SetHotKey(hotKey);
	hotKey = hotKeyConfig->GetUpVolHotKey();
	ui.HK_VolUp->SetHotKey(hotKey);
	hotKey = hotKeyConfig->GetDownVolHotKey();
	ui.HK_VolDown->SetHotKey(hotKey);

	//软件更新
	isCheck = updateCongfig->IsAutoUpdate();
	ui.AutoUpdate_CheckBox->SetCheck(isCheck);
	

	if(pSkinCfg->IsEnableUnityBg())
	{
		COptionUI* options[] =
		{
			ui.ConfigNormal_Option,
			ui.ConfigCache_Option,
			ui.ConfigHotKey_Option,
			ui.ConfigUpdate_Option
		};

		for (int i=0;i<sizeof(options)/sizeof(options[0]);++i)
		{
			options[i]->SetHotImage(pSkinCfg->GetItemHoverBg());
			options[i]->SetSelectedImage(pSkinCfg->GetItemBg());
		}
	}
}

void CSettingWindow::initUiStruct()
{	
	memset(&ui, 0, sizeof(ui));
	ui.ConfigAbout_Option = static_cast<CFadeOptionUI*>(GetPaintMgr()->FindControl(_T("Config_About")));
	ui.ConfigNormal_Option = static_cast<CFadeOptionUI*>(GetPaintMgr()->FindControl(_T("Config_Normal")));
	ui.ConfigLrcPanel_Option = static_cast<CFadeOptionUI*>(GetPaintMgr()->FindControl(_T("Config_LrcPanel")));
	ui.ConfigLrcDesk_Option = static_cast<CFadeOptionUI*>(GetPaintMgr()->FindControl(_T("Config_LrcDesk")));
	ui.ConfigCache_Option = static_cast<CFadeOptionUI*>(GetPaintMgr()->FindControl(_T("Config_Cache")));
    ui.ConfigHotKey_Option = static_cast<CFadeOptionUI*>(GetPaintMgr()->FindControl(_T("Config_HotKey")));
	ui.ConfigUpdate_Option = static_cast<CFadeOptionUI*>(GetPaintMgr()->FindControl(_T("Config_Update")));
	ui.AutoLogin_CheckBox = static_cast<CCheckBoxUI*>(GetPaintMgr()->FindControl(_T("Startup_AutoLogin")));
	ui.AutoPlay_CheckBox = static_cast<CCheckBoxUI*>(GetPaintMgr()->FindControl(_T("Startup_AutoPlay")));
	ui.AutoRun_CheckBox = static_cast<CCheckBoxUI*>(GetPaintMgr()->FindControl(_T("Startup_AutoRun")));
	ui.AutoOpenLRC_CheckBox = static_cast<CCheckBoxUI*>(GetPaintMgr()->FindControl(_T("Startup_AutoOpenLRC")));
	ui.MinTrayIcon_Option = static_cast<COptionUI*>(GetPaintMgr()->FindControl(_T("ToTray")));
	ui.Exit_Option = static_cast<COptionUI*>(GetPaintMgr()->FindControl(_T("Exit")));

	ui.PlayRoaming_CheckBox = static_cast<CCheckBoxUI*>(GetPaintMgr()->FindControl(_T("PlayRoaming")));
	ui.RelateFile_CheckBox = static_cast<CCheckBoxUI*>(GetPaintMgr()->FindControl(_T("RelateFile")));
	ui.CacheDir_Edit = static_cast<CEditUI*>(GetPaintMgr()->FindControl(_T("CacheDir")));
	ui.MaxCache_Edit = static_cast<CEditUI*>(GetPaintMgr()->FindControl(_T("MaxCache")));
	ui.CachedLabel = static_cast<CLabelUI*>(GetPaintMgr()->FindControl(_T("CachedLabel")));
	ui.ChangeDir_Btn = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("ChangeDir")));
	ui.OpenDir_Btn = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("OpenDir")));
	ui.ClearCache_Btn = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("ClearCache")));
	ui.AutoUpdate_CheckBox = static_cast<CCheckBoxUI*>(GetPaintMgr()->FindControl(_T("AutoUpdate")));
	ui.Close_Btn = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("close")));
	ui.Ok_Btn = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("OkBtn")));
	ui.Cancle_Btn = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("CancelBtn")));
	ui.Apply_Btn = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("ApplyBtn")));
	ui.PanelItemTab_TabLayout = static_cast<CAnimationTabLayoutUI*>(GetPaintMgr()->FindControl(_T("PanelItemTab")));
	ui.CheckUpdate_Btn = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("CheckUpdate")));
	ui.HK_Mode = static_cast<CHotKeyUI*>(GetPaintMgr()->FindControl(_T("HK_Mode")));
	ui.HK_Lrc = static_cast<CHotKeyUI*>(GetPaintMgr()->FindControl(_T("HK_Lrc")));
	ui.HK_PlayPause = static_cast<CHotKeyUI*>(GetPaintMgr()->FindControl(_T("HK_PlayPause")));
	ui.HK_PreSong = static_cast<CHotKeyUI*>(GetPaintMgr()->FindControl(_T("HK_PreSong")));
	ui.HK_NextSong = static_cast<CHotKeyUI*>(GetPaintMgr()->FindControl(_T("HK_NextSong")));
	ui.HK_VolUp = static_cast<CHotKeyUI*>(GetPaintMgr()->FindControl(_T("HK_VolUp")));
	ui.HK_VolDown = static_cast<CHotKeyUI*>(GetPaintMgr()->FindControl(_T("HK_VolDown")));
	ui.ColorPicker_Btn1 = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("color_picker1")));
	ui.ColorPicker_Btn2 = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("color_picker2")));
	ui.ColorPicker_Btn3 = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("color_picker3")));
	ui.ColorPicker_Btn4 = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("color_picker4")));
	ui.ColorPicker_Btn5 = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("color_picker5")));
	ui.Color_Label1 = static_cast<CLabelUI*>(GetPaintMgr()->FindControl(_T("font_color1")));
	ui.Color_Label2 = static_cast<CLabelUI*>(GetPaintMgr()->FindControl(_T("font_color2")));
	ui.Color_Label3 = static_cast<CLabelUI*>(GetPaintMgr()->FindControl(_T("font_color3")));
	ui.Color_Label4 = static_cast<CLabelUI*>(GetPaintMgr()->FindControl(_T("font_color4")));
	ui.Color_Label5 = static_cast<CLabelUI*>(GetPaintMgr()->FindControl(_T("font_color5")));
	ui.LrcPanelFontName = static_cast<CComboBoxUI*>(GetPaintMgr()->FindControl(_T("font_name1")));
	ui.LrcPanelFontStyle = static_cast<CComboBoxUI*>(GetPaintMgr()->FindControl(_T("font_style1")));
	ui.LrcPanelFontSize = static_cast<CComboBoxUI*>(GetPaintMgr()->FindControl(_T("font_size1")));
	ui.LrcDeskFontName = static_cast<CComboBoxUI*>(GetPaintMgr()->FindControl(_T("font_name2")));
	ui.LrcDeskFontStyle = static_cast<CComboBoxUI*>(GetPaintMgr()->FindControl(_T("font_style2")));
	ui.LrcDeskFontSize = static_cast<CComboBoxUI*>(GetPaintMgr()->FindControl(_T("font_size2")));
	ui.LrcDeskKaraok = static_cast<CCheckBoxUI*>(GetPaintMgr()->FindControl(_T("karaok")));
#ifdef DEBUG
	unsigned int* header = (unsigned int*)(&ui);
	for (int i = 0; i < sizeof(ui)/sizeof(int); ++i)
	{
		assert(header[i] != 0);
	}
#endif

}
void CSettingWindow::colorBox(CControlUI* pUI)
{
	static CHOOSECOLOR cc;
	static COLORREF crCustCoLors[16];
	cc.lStructSize=sizeof(CHOOSECOLOR);
	cc.hwndOwner= this->m_hWnd;
	cc.hInstance=NULL;
	cc.rgbResult = static_cast<CHorizontalLayoutUI*>(pUI->GetParent())->GetItemAt(0)->GetBkColor();
	cc.lpCustColors=crCustCoLors;
	cc.Flags=CC_RGBINIT|CC_FULLOPEN | CC_ANYCOLOR; 
	cc.lCustData=0;
	cc.lpfnHook=NULL;
	cc.lpTemplateName=NULL;
	if(TRUE == ChooseColor(&cc))
	{ 
		TCHAR szBuf[MAX_PATH] = {0};
#if defined(UNDER_CE)
		_stprintf(szBuf, _T("FF%02X%02X%02X"), GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult));
#else
		_stprintf_s(szBuf, MAX_PATH - 1, _T("FF%02X%02X%02X"), GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult));
#endif
		LPTSTR pstr = NULL;
		DWORD dwFontColor = _tcstoul(szBuf, &pstr, 16);
		static_cast<CHorizontalLayoutUI*>(pUI->GetParent())->GetItemAt(0)->SetBkColor(dwFontColor);
		CLrcPreViewUI* pLrcPreView = static_cast<CLrcPreViewUI*>(GetPaintMgr()->FindControl(_T("lrcPreView")));
		ASSERT(pLrcPreView);
		CConfigMgr::spLrcConfigT pLrcCfg = theConfigMgr->GetLrcConfig();
		pLrcPreView->m_iFontSize = pLrcCfg->GetLrcDeskFontSize();
		pLrcPreView->m_iFontStyle = pLrcCfg->GetLrcDeskFontStyle();
		pLrcPreView->m_sFontName = pLrcCfg->GetLrcDeskFontName();
		pLrcPreView->m_dwFontColor = dwFontColor;
		pLrcPreView->Invalidate();
		setModify(true);
	}

}
//void CSettingWindow::OnPaint( CDCHandle dc )

void CSettingWindow::showPage(emTabPages emPage)
{
	assert(emPage>=0&&emPage<PageCount);
	ui.PanelItemTab_TabLayout->SelectItem(emPage);
}