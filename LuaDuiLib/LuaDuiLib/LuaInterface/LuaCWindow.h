#ifndef __CLuaWindow__H
#define __CLuaWindow__H

namespace DuiLib
{
	class LUAUILIB_API CLuaWindow : public CWin, public INotifyUI, public IMessageFilterUI,
		public IDialogBuilderCallback, public IListCallbackUI
	{
	public:
		CLuaWindow(LPCTSTR szName, UINT nStyle = CS_DBLCLKS);
		LPCTSTR GetWindowClassName() const;
		UINT GetClassStyle() const;
		void OnFinalMessage(HWND hWnd);

		CPaintManagerUI* PaintMgr() const;

		void TouchMsgTable(int nLuaObjRef,int nLuaMsgRef);
		void UnTouchMsgTable();
		lua_State* L() const;
	public:
		bool onUIEvent(void* param);
		bool onUIDestroy(void* param);
		void Notify(TNotifyUI &msg);
		LRESULT MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam, bool &handled);
		CControlUI *CreateControl(LPCTSTR pstrClass);
		LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);
    protected:
		void PushThis();
		void PushLuaObjTable();
		LRESULT ProcessWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnCreate(UINT msg, WPARAM wparam, LPARAM lparam, BOOL &handled);
		LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam, BOOL &handled);
    protected:
		bool LuaFuncExists(const char *func);
		bool CallLuaFuncBegin(const char* func);   
		void CallLuaFuncEnd(int nArgs, int nRets = 0);
	private:
		const CDuiString szWindowName;
		const UINT nClassStyle;

		int mLuaObjRef;
		int mLuaMsgRef;
	};
}


#endif//__CLuaWindow__H