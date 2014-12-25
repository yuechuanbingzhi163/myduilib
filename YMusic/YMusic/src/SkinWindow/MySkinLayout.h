#ifndef __MySkinLayout_h
#define __MySkinLayout_h

#pragma once
#define MyRowNum  5
class CMySkinLayoutUI : public CListContainerElementUI
{
public:
	CMySkinLayoutUI();
	virtual ~CMySkinLayoutUI();

	static LPCTSTR GetClassName();
	LPCTSTR GetClass() const;
	int GetImageCount() { return _ImageCount; }
	void AddImage(CString szImage);
protected:
	void Init();
	CAlbumButtonUI* _pImage[MyRowNum];
// 	CAlbumButtonUI* _pImage1;
// 	CAlbumButtonUI* _pImage2;
// 	CAlbumButtonUI* _pImage3;

	int _ImageCount;
};

#endif//__MySkinLayout_h