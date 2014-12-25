#pragma once

namespace DuiLib{
class UILIB_API CAlbumButtonUI : 
	public CFadeButtonUI
{
public:
	CAlbumButtonUI();

	static LPCTSTR GetClassName();
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void DoEvent(TEventUI& event);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void PaintBkImage(HDC hDC);

	void SetHotBkImage(LPCTSTR pStrImage);
	CDuiString GetHotBkImage() const;

	void PaintStatusImage(HDC hDC);

	void SetAlbumImage(LPCTSTR pStrImage);
private:
	CDuiString m_sHotBkImage;
	CDuiString m_sOldNormalImage;
};

}