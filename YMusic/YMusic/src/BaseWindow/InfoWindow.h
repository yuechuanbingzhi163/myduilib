#pragma once
#include "../../YMusic.h"

class CInfoWindow 
	: public YWindow
{
public:
	static const size_t MaxWidth = 800;
	static const size_t MaxHeight = 600;

	CInfoWindow(){};
	~CInfoWindow(){};
public:
	static void ShowInfoWindow(HWND hparent,POINT pt,LPCTSTR pszText);
	static void ShowInfoWindow(HWND hparent,RECT rt,LPCTSTR pszText);
	static void ShowInfoWindow(HWND hparent,LPCTSTR pszText);
	void ShowInfo(HWND hparent,
		POINT pt,
		LPCTSTR pszText);
protected:
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	void AdjustWindow(HWND hparent,POINT pt);
	void SetText(LPCTSTR pszText);
private:
	UINT_PTR    _nTimerId;
};
