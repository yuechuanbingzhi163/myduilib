
/***********************************************************
 *	FileName£ºUILoginList.hpp
 *  Author  £ºDFn Li
 *  Timer   : 2013/3/2 14:24
 *  E-Mail  : libyyu@qq.com
 *  Comment : 
 *  Version : 1.0.0.1
 ***********************************************************/

#ifndef UILOGNILIST_HPP
#define UILOGNILIST_HPP

#pragma once
struct UserInfo;

namespace DuiLib{
	class CMyButtonUI : public CButtonUI
	{
	public:
		CMyButtonUI(){};
		static LPCTSTR GetClassName()
		{
			return _T("MyButton");
		}
		LPCTSTR GetClass() const
		{
			return CMyButtonUI::GetClassName();
		}
		void DoEvent(TEventUI& event)
		{
			if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
				if( m_pParent != NULL ) m_pParent->DoEvent(event);
				else CLabelUI::DoEvent(event);
				return;
			}
			else if( event.Type == UIEVENT_SETCURSOR ) {
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
				return;
			}
			else 
				CButtonUI::DoEvent(event);
		}
	};
	class CDlgBuilder : public IDialogBuilderCallback
	{
	public:
		CControlUI* CreateControl(LPCTSTR pstrClass)
		{
			if(_tcscmp(pstrClass,_T("MyButton"))==0)
				return new CMyButtonUI;
			return NULL;
		}
	};
class CLoginListItemUI : public CListContainerElementUI
{
public:
	CLoginListItemUI();
	~CLoginListItemUI();
public:
	static LPCTSTR GetClassName();
	LPCTSTR GetClass() const;
public:
	void SetUser(UserInfo user);
	UserInfo USER() const;
	void SetPage(int iIndex);
	int GetPage();
/*protected:*/
// 	void Update();
	void DoEvent(TEventUI& event);
};

class CLoginListUI : public CListUI
{
public:
	enum {SCROLL_TIMERID = 10};

	CLoginListUI(CPaintManagerUI& paint_manager);

	~CLoginListUI();

	bool Add(CControlUI* pControl);

	bool AddAt(CControlUI* pControl, int iIndex);

	bool Remove(CControlUI* pControl);

	bool RemoveAt(int iIndex);

	void RemoveAll();

	void DoEvent(TEventUI& event);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	bool AddNode(UserInfo user);
	UserInfo& GetUser(const int iIndex);
private:
	std::vector<UserInfo> userList_;
	LONG	delay_deltaY_;
	DWORD	delay_number_;
	DWORD	delay_left_;


    CDialogBuilder m_dlgBuilder;

public:
	CPaintManagerUI& paint_manager_;
};
};


#endif // UILOGNILIST_HPP