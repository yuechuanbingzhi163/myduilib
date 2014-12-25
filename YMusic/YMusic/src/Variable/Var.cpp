#include "stdafx.h"

#include "Var.h"

namespace varMainWindow
{

	const LPCTSTR kExpandMusicLib = _T("expandbtn");
	const LPCTSTR kHideMusicLib = _T("shrinkbtn");
	const LPCTSTR kChangeSkin = _T("skinbtn");
	const LPCTSTR kMainMenu = _T("menubtn");
	const LPCTSTR kMiniMode = _T("minibtn");
	const LPCTSTR kShowMin = _T("minbtn");
	const LPCTSTR kRestoreSize = _T("restorebtn");
	const LPCTSTR kShowMax = _T("maxbtn");
	const LPCTSTR kQuitApp = _T("closebtn");
	const LPCTSTR kLogin = _T("logbtn");
	const LPCTSTR kUserName = _T("username");
	const LPCTSTR kUser_LogSwitch = _T("logtablayout");
	const LPCTSTR kPlayList = _T("play_list");
	const LPCTSTR kSongList = _T("song_list");
	const LPCTSTR kAlbumLogo = _T("albumlogo");
	const LPCTSTR kTabLayout = _T("switch");
	const LPCTSTR kPrev = _T("prev");
	const LPCTSTR kPlay = _T("play");
	const LPCTSTR kPause = _T("pause");
	const LPCTSTR kNext = _T("next");
	const LPCTSTR kVolume = _T("volume");
	const LPCTSTR kVolumetab = _T("volumetable");
	const LPCTSTR kPlayModetab = _T("playModeTabLayout");
	const LPCTSTR kLrc = _T("lrc");
	const LPCTSTR kVolumeslider = _T("volume_slider");
	const LPCTSTR kAddPlayList = _T("new_list");
	const LPCTSTR kTimeUsed = _T("time_used");
	const LPCTSTR kTimeTotal = _T("time_total");
	const LPCTSTR kPlayProgress = _T("playpro");
	const LPCTSTR kSongName = _T("songname");
	const LPCTSTR kAuthorName = _T("author");
	const LPCTSTR kCollect = _T("collect");
	const LPCTSTR kShare = _T("share");
	const LPCTSTR kCount = _T("show_count");
	const LPCTSTR kInfoLayout = _T("info");
	const LPCTSTR kInfoLabel = _T("info_label");
	const LPCTSTR kModeLayout = _T("playModeTabLayout");
	const LPCTSTR kListOrder = _T("listMode");
	const LPCTSTR kListLoop = _T("listLoopMode");
	const LPCTSTR kSingle = _T("singleMode");
	const LPCTSTR kSingleLoop = _T("singleLoopMode");
	const LPCTSTR kRandom = _T("randomMode");
	const LPCTSTR kSaveAsList = _T("store_new_list");
	const LPCTSTR kGoBack = _T("GoBack");
	const LPCTSTR kGoNext = _T("GoNext");
	const LPCTSTR kGoDesk = _T("GoDesk");
	const LPCTSTR kSearchBtn = _T("SearchLrc");
	const LPCTSTR kLrcViewTab = _T("lrcview_layout");
	const LPCTSTR kSimilarSongs = _T("similarsong");
	const LPCTSTR kLrcView1 = _T("LrcView");
};

namespace varLoginWindow
{
	const LPCTSTR kCloseBtn = _T("closebtn");
	const LPCTSTR kUsername = _T("username");
	const LPCTSTR kRegister = _T("register");
	const LPCTSTR kPassword = _T("password");
	const LPCTSTR kGetpassword = _T("getpassword");
	const LPCTSTR kInforesulttab = _T("info_result");
	const LPCTSTR kRempassword = _T("rempassword");
	const LPCTSTR kAutologin = _T("autologin");
	const LPCTSTR kResultLabel = _T("result");
	const LPCTSTR kLoginbtn = _T("loginbtn");
	const LPCTSTR kLoginlab = _T("loginlab");
};
namespace play_list_item
{
	const LPCTSTR kListItem = _T("listitem");
	const LPCTSTR kListLabel = _T("list-name");
	const LPCTSTR kListEdit = _T("rename");
};
namespace list_song_item
{
	const LPCTSTR kListItem = _T("listitem");
	const LPCTSTR kSongNorItem = _T("songitemnormallayout");
	const LPCTSTR kSongExpItem = _T("songitemexplayout");
	const LPCTSTR kNorMenu = _T("normenu");
};
namespace varPlayListMenu
{
	const LPCTSTR kPlay = _T("play");
	const LPCTSTR kRename = _T("rename");
	const LPCTSTR kRemove = _T("remove");
	const LPCTSTR kNewList = _T("newlist");

};
namespace varSongListMenu
{
	const LPCTSTR kAddTo = _T("addto");
	const LPCTSTR kMoveTo = _T("moveto");
	const LPCTSTR kPlay = _T("play");
	const LPCTSTR kShare = _T("share");
	const LPCTSTR kCollect = _T("collect");
	const LPCTSTR kCollectCheckBox = _T("collect_checkbox");
	const LPCTSTR kModify = _T("modify");
	const LPCTSTR kDumplicate = _T("remove_duplicate");
	const LPCTSTR kRemove = _T("remove");
	const LPCTSTR kClear = _T("clear");
	const LPCTSTR kProperty = _T("properly");
	const LPCTSTR kOpenDir = _T("opendir");
	const LPCTSTR kAddFile = _T("addfile");
	const LPCTSTR kAddFolder = _T("addfolder");
	const LPCTSTR kToNewList = _T("to_newlist");
	const LPCTSTR kAdd = _T("add");
};
namespace varSongItemMenu
{
	const LPCTSTR kPlay = _T("play");
	const LPCTSTR kCollect = _T("collect");
	const LPCTSTR kShare = _T("share");
	const LPCTSTR kRemove = _T("remove");
	const LPCTSTR kDownLoad = _T("download");
};
namespace varPlayModeMenu
{
	const LPCTSTR kListOrder = _T("listPlay");
	const LPCTSTR kListLoop = _T("listLoop");
	const LPCTSTR kSingleLoop = _T("singleLoop");
	const LPCTSTR kSingle = _T("singlePlay");
	const LPCTSTR kRandom = _T("randomPlay");
	const LPCTSTR kCheckBox = _T("checkbox");
	const LPCTSTR kUserData = _T("play_mode");
};

namespace varLrcViewMenu
{
	const LPCTSTR kSearchLrc = _T("search");
	const LPCTSTR kLrcModify = _T("modify");
	const LPCTSTR kLrcAdvance = _T("advance");
	const LPCTSTR kLrcBack = _T("back");
	const LPCTSTR kLrcReset = _T("reset");
	const LPCTSTR kLrcLocal = _T("linklocalrc");
	const LPCTSTR kLrcEdit = _T("editlrc");
	const LPCTSTR kLrcSetting = _T("lrcsetting");
	const LPCTSTR kLrcFolder = _T("lrcdir");
	const LPCTSTR kLrcDesk = _T("desklrc");
};
namespace varMainMenu
{
	const LPCTSTR kExit = _T("exit");
	const LPCTSTR kLogin = _T("login");
	const LPCTSTR kLogout = _T("logout");
	const LPCTSTR kSetting = _T("setoption");
	const LPCTSTR kKeepTop = _T("keeptop");
	const LPCTSTR kDeskLrc = _T("showlrc");
	const LPCTSTR kLockLrc = _T("locklrc");
	const LPCTSTR kSkinWin = _T("changeskin");
}
const LPCTSTR UnKnown = _T("未知");
const UINT    nMaxSongNumber = 250;