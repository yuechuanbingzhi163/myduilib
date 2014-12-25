#include "stdafx.h"
#include "MySkinLayout.h"
#include "../YApp.h"

CMySkinLayoutUI::CMySkinLayoutUI()
{
	this->SetFixedHeight(100);
	CDialogBuilder builder;
	builder.Create(_T("MySkinLayout.xml"),0,NULL,NULL,this);
	_ImageCount = 0;
	Init();
}
CMySkinLayoutUI::~CMySkinLayoutUI()
{

}

LPCTSTR CMySkinLayoutUI::GetClassName()
{
	return _T("MySkinItem");
}
LPCTSTR CMySkinLayoutUI::GetClass() const
{
	return CMySkinLayoutUI::GetClassName();
}

void CMySkinLayoutUI::Init()
{
	for (int i=0;i<MyRowNum;++i)
	{
		_pImage[i] = NULL;
		CDuiString sImage;
		sImage.Format(_T("image_%d"),i+1);
		_pImage[i] = static_cast<CAlbumButtonUI*>(FindSubControl(sImage));
		ASSERT(_pImage[i]);
		CDuiString sBkHotImage;
		sBkHotImage = _T("file='YTing/hotmask.png'  corner='5,5,5,5'");
		_pImage[i]->SetHotBkImage(sBkHotImage);
	}
// 	_pImage1 = _pImage2 = _pImage3 = NULL;
// 	_pImage1 = static_cast<CAlbumButtonUI*>(FindSubControl(_T("image_1")));
// 	_pImage2 = static_cast<CAlbumButtonUI*>(FindSubControl(_T("image_2")));
// 	_pImage3 = static_cast<CAlbumButtonUI*>(FindSubControl(_T("image_3")));
// 	ASSERT(_pImage3&&_pImage2&&_pImage1);
// 	CDuiString sBkHotImage;
// 	sBkHotImage = _T("file='YTing/hotmask.png'  corner='5,5,5,5'");
// 	_pImage1->SetHotBkImage(sBkHotImage);
// 	_pImage2->SetHotBkImage(sBkHotImage);
// 	_pImage3->SetHotBkImage(sBkHotImage);
}
void CMySkinLayoutUI::AddImage(CString szImage)
{
	ASSERT(szImage);
	CDuiString sImage;
	sImage.Format(_T("file='%s' dest='8,8,132,92' corner='5,5,5,5'"),szImage.GetString());
	CDuiString sUserData;
	sUserData.Format(_T("file='%s' corner='5,5,5,5'"),szImage.GetString());
	if (_ImageCount<MyRowNum-1)
	{
		_pImage[_ImageCount]->SetNormalImage(sImage);
		_pImage[_ImageCount]->SetUserData(sUserData);
		_pImage[_ImageCount]->SetVisible(true);
		_ImageCount ++;
		_pImage[_ImageCount]->SetVisible(true);
	}
	else if(_ImageCount==MyRowNum-1)
	{
		_pImage[_ImageCount]->SetNormalImage(sImage);
		_pImage[_ImageCount]->SetUserData(sUserData);
		_pImage[_ImageCount]->SetVisible(true);
		_ImageCount ++;
	}
	else
	{

	}
}
