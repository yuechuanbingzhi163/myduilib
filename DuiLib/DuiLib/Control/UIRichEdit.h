#ifndef __UIRICHEDIT_H__
#define __UIRICHEDIT_H__

#pragma once
#include <Imm.h>
#pragma comment(lib,"imm32.lib")
#include <richole.h>
#include <objidl.h>

#define IDM_VERBMIN                     40200
#define IDM_VERBMAX                     40300
#define ID_EDIT_CONVERT                 40013
#define ID_EDIT_CUT                     40006
#define ID_EDIT_COPY                    40007
#define ID_EDIT_PASTE                   40008

namespace DuiLib {

	class CTxtWinHost;
	class CRichEditOleCallback;
	class COleInPlaceFrame;
	class CImageDataObjectCallback;
	class CImageDataObject;
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CRichEditUI : public CContainerUI, public IMessageFilterUI
	{
	public:
		CRichEditUI();
		~CRichEditUI();

		static LPCTSTR GetClassName();
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool IsWantTab();
		void SetWantTab(bool bWantTab = true);
		bool IsWantReturn();
		void SetWantReturn(bool bWantReturn = true);
		bool IsWantCtrlReturn();
		void SetWantCtrlReturn(bool bWantCtrlReturn = true);
		bool IsRich();
		void SetRich(bool bRich = true);
		bool IsReadOnly();
		void SetReadOnly(bool bReadOnly = true);
		bool GetWordWrap();
		void SetWordWrap(bool bWordWrap = true);
		int GetFont();
		void SetFont(int index);
		void SetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
		LONG GetWinStyle();
		void SetWinStyle(LONG lStyle);
		DWORD GetTextColor();
		void SetTextColor(DWORD dwTextColor);
		int GetLimitText();
		void SetLimitText(int iChars);
		long GetTextLength(DWORD dwFlags = GTL_DEFAULT) const;
		CDuiString GetText() const;
		void SetText(LPCTSTR pstrText);
		bool GetModify() const;
		void SetModify(bool bModified = true) const;
		void GetSel(CHARRANGE &cr) const;
		void GetSel(long& nStartChar, long& nEndChar) const;
		int SetSel(CHARRANGE &cr);
		int SetSel(long nStartChar, long nEndChar);
		void ReplaceSel(LPCTSTR lpszNewText, bool bCanUndo);
		void ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo = false);
		CDuiString GetSelText() const;
		int SetSelAll();
		int SetSelNone();
		WORD GetSelectionType() const;
		bool GetZoom(int& nNum, int& nDen) const;
		bool SetZoom(int nNum, int nDen);
		bool SetZoomOff();
		bool GetAutoURLDetect() const;
		bool SetAutoURLDetect(bool bAutoDetect = true);
		DWORD GetEventMask() const;
		DWORD SetEventMask(DWORD dwEventMask);
		CDuiString GetTextRange(long nStartChar, long nEndChar) const;
		void HideSelection(bool bHide = true, bool bChangeStyle = false);
		void ScrollCaret();
		int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo = false);
		int AppendText(LPCTSTR lpstrText, bool bCanUndo = false);
		DWORD GetDefaultCharFormat(CHARFORMAT2 &cf) const;
		bool SetDefaultCharFormat(CHARFORMAT2 &cf);
		DWORD GetSelectionCharFormat(CHARFORMAT2 &cf) const;
		bool SetSelectionCharFormat(CHARFORMAT2 &cf);
		bool SetWordCharFormat(CHARFORMAT2 &cf);
		DWORD GetParaFormat(PARAFORMAT2 &pf) const;
		bool SetParaFormat(PARAFORMAT2 &pf);
		bool Redo();
		bool Undo();
		void Clear();
		void Copy();
		void Cut();
		void Paste();
		int GetLineCount() const;
		CDuiString GetLine(int nIndex, int nMaxLength) const;
		int LineIndex(int nLine = -1) const;
		int LineLength(int nLine = -1) const;
		bool LineScroll(int nLines, int nChars = 0);
		CPoint GetCharPos(long lChar) const;
		long LineFromChar(long nIndex) const;
		CPoint PosFromChar(UINT nChar) const;
		int CharFromPos(CPoint pt) const;
		void EmptyUndoBuffer();
		UINT SetUndoLimit(UINT nLimit);
		long StreamIn(int nFormat, EDITSTREAM &es);
		long StreamOut(int nFormat, EDITSTREAM &es); 
		bool SetOLECallback( IRichEditOleCallback *pCallback );
		IRichEditOleCallback *GetOLECallback();
		LPRICHEDITOLE GetRichEditOle();

		bool SetImageObject( CImageDataObjectCallback *pImageObject);
		CImageDataObjectCallback *GetImageObject();

		CTxtWinHost* GetTxWinHost()  ;

		void InsertBitmap(HBITMAP hBitmap);
		void InsertBitmap(LPCTSTR lpValue);


		void DoInit();
		// 注意：TxSendMessage和SendMessage是有区别的，TxSendMessage没有multibyte和unicode自动转换的功能，
		// 而richedit2.0内部是以unicode实现的，在multibyte程序中，必须自己处理unicode到multibyte的转换
		virtual HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const; 
		IDropTarget* GetTxDropTarget();
		virtual bool OnTxViewChanged();
		virtual void OnTxNotify(DWORD iNotify, void *pv);

		void SetScrollPos(SIZE szPos);
		void LineUp();
		void LineDown();
		void PageUp();
		void PageDown();
		void HomeUp();
		void EndDown();
		void LineLeft();
		void LineRight();
		void PageLeft();
		void PageRight();
		void HomeLeft();
		void EndRight();

		SIZE EstimateSize(SIZE szAvailable);
		void SetPos(RECT rc);
		void DoEvent(TEventUI& event);
		void DoPaint(HDC hDC, const RECT& rcPaint);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	protected: 
		CTxtWinHost* m_pTwh;
		bool m_bVScrollBarFixing;
		bool m_bWantTab;
		bool m_bWantReturn;
		bool m_bWantCtrlReturn;
		bool m_bRich;
		bool m_bReadOnly;
		bool m_bWordWrap;
		DWORD m_dwTextColor;
		int m_iFont;
		int m_iLimitText;
		LONG m_lTwhStyle;
		LPRICHEDITOLE m_pRichEditOle;
		IRichEditOleCallback *m_pCallback;
		CImageDataObjectCallback*	     m_pImageObject;
		bool m_bInited;
	};


	class UILIB_API CImageDataObjectCallback : public IDataObject
	{
	public:
		CImageDataObjectCallback(CRichEditUI* pRichEdit) : m_pRichEdit(pRichEdit){}
		virtual ~CImageDataObjectCallback(){}
		virtual void InsertBitmap( HBITMAP hBitmap) = 0 ;
		virtual void SetBitmap(HBITMAP hBitmap) = 0;
		virtual void InsertBitmap(LPCTSTR lpValue) = 0;
	protected:
		CRichEditUI* m_pRichEdit;
	};

	class UILIB_API CImageDataObject : public CImageDataObjectCallback
	{
	public:
		virtual void InsertBitmap(LPCTSTR lpValue);
		virtual void InsertBitmap(HBITMAP hBitmap);
	public:
		CImageDataObject(CRichEditUI* pRichEdit)
			: m_ulRefCnt(0)
			, m_bRelease(FALSE)
			, CImageDataObjectCallback(pRichEdit)
			, m_pOleClientSite(NULL)
			, m_pOleObject(NULL)
			, m_pStorage(NULL)
			, m_lpDataObject(NULL)
		{}
		virtual ~CImageDataObject()
		{
			if (m_bRelease)
				::ReleaseStgMedium(&m_stgmed);
			if(m_pOleObject) m_pOleObject->Release();
			if(m_pOleClientSite) m_pOleClientSite->Release();
			if(m_pStorage) m_pStorage->Release();
			if(m_lpDataObject) m_lpDataObject->Release();
		}
		// Methods of the IUnknown interface
		STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
		{
			if (iid == IID_IUnknown || iid == IID_IDataObject)
			{
				*ppvObject = this;
				AddRef();
				return S_OK;
			}
			else
				return E_NOINTERFACE;
		}
		STDMETHOD_(ULONG, AddRef)(void)
		{
			m_ulRefCnt++;
			return m_ulRefCnt;
		}
		STDMETHOD_(ULONG, Release)(void)
		{
			if (--m_ulRefCnt == 0)
			{
				delete this;
			}

			return m_ulRefCnt;
		}
		// Methods of the IDataObject Interface
		//
		STDMETHOD(GetData)(FORMATETC *pformatetcIn, STGMEDIUM *pmedium) {
			HANDLE hDst = NULL;
			hDst = ::OleDuplicateData(m_stgmed.hBitmap, CF_BITMAP, NULL);
			if (hDst == NULL)
			{
				return E_HANDLE;
			}

			pmedium->tymed = TYMED_GDI;
			pmedium->hBitmap = (HBITMAP)hDst;
			pmedium->pUnkForRelease = NULL;

			return S_OK;
		}
		STDMETHOD(GetDataHere)(FORMATETC* pformatetc, STGMEDIUM*  pmedium ) {
			return E_NOTIMPL;
		}
		STDMETHOD(QueryGetData)(FORMATETC*  pformatetc ) {
			return E_NOTIMPL;
		}
		STDMETHOD(GetCanonicalFormatEtc)(FORMATETC*  pformatectIn ,FORMATETC* pformatetcOut ) 	{
			return E_NOTIMPL;
		}
		STDMETHOD(SetData)(FORMATETC* pformatetc , STGMEDIUM*  pmedium , BOOL  fRelease ) {
			m_fromat = *pformatetc;
			m_stgmed = *pmedium;

			return S_OK;
		}
		STDMETHOD(EnumFormatEtc)(DWORD  dwDirection , IEnumFORMATETC**  ppenumFormatEtc ) {
			return E_NOTIMPL;
		}
		STDMETHOD(DAdvise)(FORMATETC *pformatetc, DWORD advf, IAdviseSink *pAdvSink,
			DWORD *pdwConnection) {
				return E_NOTIMPL;
		}
		STDMETHOD(DUnadvise)(DWORD dwConnection) {
			return E_NOTIMPL;
		}
		STDMETHOD(EnumDAdvise)(IEnumSTATDATA **ppenumAdvise) {
			return E_NOTIMPL;
		}

		// Some Other helper functions
		//
		void SetBitmap(HBITMAP hBitmap);
		IOleObject *GetOleObject(IOleClientSite *pOleClientSite, IStorage *pStorage);
	private:
	private:
		ULONG	m_ulRefCnt;
		BOOL	m_bRelease;

		IOleObject * m_pOleObject;
		IOleClientSite * m_pOleClientSite;
		IStorage * m_pStorage ;
		LPDATAOBJECT m_lpDataObject;
		// The data being bassed to the richedit
		//
		STGMEDIUM m_stgmed;
		FORMATETC m_fromat;
	}; 
} // namespace DuiLib

#endif // __UIRICHEDIT_H__
