#ifndef __SysSkinLayout_h
#define __SysSkinLayout_h

#pragma once
#define SysRowNum  5

class CSysSkinLayoutUI : public CListContainerElementUI
{
public:
	CSysSkinLayoutUI();
	virtual ~CSysSkinLayoutUI();

	static LPCTSTR GetClassName();
	LPCTSTR GetClass() const;
	int GetImageCount() { return _ImageCount; }
	void AddImage(CString szImage);
protected:
	void Init();
	CAlbumButtonUI* _pImage[SysRowNum];
// 	CAlbumButtonUI* _pImage1;
// 	CAlbumButtonUI* _pImage2;
// 	CAlbumButtonUI* _pImage3;

	int _ImageCount;
};






#endif//__SysSkinLayout_h