#ifndef __THEMEMGR_H
#define __THEMEMGR_H

typedef
struct tagTSkinInfo
{
	CDuiString m_themeName;
	CDuiString m_appVersion;
	CDuiString m_skinFolder;
	CDuiString m_skinZipName;
	CDuiString m_skinFace;
	std::vector<CDuiString> m_skinList;;
	int        m_skinId;
}TSkinInfo;
//目前仅实现文件加载
class IUserTheme
{
public:
	virtual bool LoadSkin(STRINGorID xml) = 0;
	virtual bool UseSkin(STRINGorID skin) = 0;
};

class CSkinMgr : public IUserTheme
{
public:
	CSkinMgr();
public:
	bool LoadSkin(STRINGorID xml);
	bool UseSkin(STRINGorID skin);
public:
	static CSkinMgr* getInstance();
private:
	CMarkup m_xml;
	TSkinInfo m_skin;
	static CSkinMgr* m_hInst;

};


#endif//__THEMEMGR_H