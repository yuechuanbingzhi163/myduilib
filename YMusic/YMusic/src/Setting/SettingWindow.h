#pragma once



class CSettingWindow : 
	public YWindow ,
	public INotifyUI ,
	public IDialogBuilderCallback ,
	public Singleton<CSettingWindow>
{

public:
	CSettingWindow();
	~CSettingWindow();
	
protected:
	virtual void Notify(TNotifyUI& msg); 

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//void OnPaint(CDCHandle dc);
	//void OnShowWindow(BOOL bShow, UINT nStatus);
	/*virtual*/ CControlUI* CreateControl(LPCTSTR pstrClass);
public:
	enum emTabPages
	{
		AboutPage,
		NormalPage,
		LrcPanelPage,
		LrcDeskPage,
		CachePage,
		HotKeyPage,
		UpdataPage,

		PageCount
	};

	enum emInsertModes
	{
		BeginPosMode,
		CurPosMode,
		EndPosMode
	};
	void showPage(emTabPages emPage);
private:
	struct
	{
		CFadeOptionUI *ConfigAbout_Option;//软件关于
		CFadeOptionUI *ConfigNormal_Option;//常规设置
		CFadeOptionUI *ConfigLrcPanel_Option;//歌词显示
		CFadeOptionUI *ConfigLrcDesk_Option;//桌面歌词
		CFadeOptionUI *ConfigCache_Option;//缓存设置
		CFadeOptionUI *ConfigHotKey_Option;//热键设置
		CFadeOptionUI *ConfigUpdate_Option;//软件更新
		//常规设置页面
		CCheckBoxUI *AutoLogin_CheckBox;//自动登录账户
		CCheckBoxUI *AutoPlay_CheckBox;//自动播放
		CCheckBoxUI *AutoRun_CheckBox;//开机启动
		CCheckBoxUI *AutoOpenLRC_CheckBox;//自动显示桌面歌词
		COptionUI *MinTrayIcon_Option;
		COptionUI *Exit_Option;

		CCheckBoxUI *PlayRoaming_CheckBox;//漫游列表生成后立即播放
		CCheckBoxUI *RelateFile_CheckBox;//文件关联
		//歌词显示
		CButtonUI *ColorPicker_Btn1;
		CButtonUI *ColorPicker_Btn2;
		CButtonUI *ColorPicker_Btn3;
		CButtonUI *ColorPicker_Btn4;
		CButtonUI *ColorPicker_Btn5;
		CLabelUI *Color_Label1;
		CLabelUI *Color_Label2;
		CLabelUI *Color_Label3;
		CLabelUI *Color_Label4;
		CLabelUI *Color_Label5;
		CComboBoxUI *LrcPanelFontName;
		CComboBoxUI *LrcPanelFontStyle;
		CComboBoxUI *LrcPanelFontSize;
		CComboBoxUI *LrcDeskFontName;
		CComboBoxUI *LrcDeskFontStyle;
		CComboBoxUI *LrcDeskFontSize;
		CCheckBoxUI *LrcDeskKaraok;
		//桌面歌词
		//缓存设置页面
		CEditUI *CacheDir_Edit;
		CEditUI *MaxCache_Edit;
		CLabelUI *CachedLabel;
		CButtonUI *ChangeDir_Btn;
		CButtonUI *OpenDir_Btn;
		CButtonUI *ClearCache_Btn;
		//软件更新
		CCheckBoxUI *AutoUpdate_CheckBox;

		//热键设置页面
		CAnimationTabLayoutUI *PanelItemTab_TabLayout;
		CButtonUI *CheckUpdate_Btn;
		CHotKeyUI *HK_Mode;
		CHotKeyUI *HK_Lrc;
		CHotKeyUI *HK_PlayPause;
		CHotKeyUI *HK_PreSong;
		CHotKeyUI *HK_NextSong;
		CHotKeyUI *HK_VolUp;
		CHotKeyUI *HK_VolDown;

		CButtonUI *Close_Btn;
		CButtonUI *Ok_Btn;
		CButtonUI *Cancle_Btn;
		CButtonUI *Apply_Btn;
	}ui;
	bool m_bModify;

	void init();
	void ok();
	void cancle();
	void apply();
	void setModify(bool isModify);
	void initUiStruct();
	void colorBox(CControlUI* pUI);
	DWORD dwFromText(LPCTSTR lpszValue);
	int   intFromText(LPCTSTR lpszValue,CComboBoxUI* pCombo);
};

