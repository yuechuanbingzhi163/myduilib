#ifndef __UTILS_H__
#define __UTILS_H__

#pragma once
#include <string>

namespace DuiLib
{
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class STRINGorID
	{
	public:
		STRINGorID(LPCTSTR lpString) : m_lpstr(lpString)
		{
			m_hInst = NULL;
			m_ID = 0;
		}
		STRINGorID(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID))
		{
			m_hInst = NULL;
			m_ID = nID;
		}
		STRINGorID(HINSTANCE hInst, UINT nID) :m_lpstr(_T(""))
		{
			m_hInst = hInst;
			m_ID = nID;
		}
		operator LPCTSTR() const
		{
			if (!m_hInst)
			{
				if (m_ID > 0)
				{
					static TCHAR szOut[MAX_PATH] = { 0 };
					wsprintf(szOut, _T("%d"), m_ID);
					return szOut;
				}
				else
					return m_lpstr;
			}
			else
			{
				TCHAR szFileName[MAX_PATH] = { 0 };
				GetModuleFileName(m_hInst, szFileName, MAX_PATH);
				static TCHAR szOut[MAX_PATH] = { 0 };
				wsprintf(szOut, _T("%s:%d"), szFileName, m_ID);
				return szOut;
			}
		}
		LPCTSTR m_lpstr;
		HINSTANCE m_hInst;
		UINT m_ID;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CPoint : public tagPOINT ,public IDuiObject
	{
	public:
		CPoint();
		CPoint(const POINT& src);
		CPoint(int x, int y);
		CPoint(LPARAM lParam);
		virtual LPCTSTR GetClass() const;
		virtual bool IsClass(LPCTSTR pstrClass);
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CSize : public tagSIZE , public IDuiObject
	{
	public:
		CSize();
		CSize(const SIZE& src);
		CSize(const RECT rc);
		CSize(int cx, int cy);
		virtual LPCTSTR GetClass() const;
		virtual bool IsClass(LPCTSTR pstrClass);
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CDuiRect : public tagRECT, public IDuiObject
	{
	public:
		CDuiRect();
		CDuiRect(const RECT& src);
		CDuiRect(int iLeft, int iTop, int iRight, int iBottom);

		int GetWidth() const;
		int GetHeight() const;
		void Empty();
		bool IsNull() const;
		void Join(const RECT& rc);
		void ResetOffset();
		void Normalize();
		void Offset(int cx, int cy);
		void Inflate(int cx, int cy);
		void Deflate(int cx, int cy);
		void Union(CDuiRect& rc);
		virtual LPCTSTR GetClass() const;
		virtual bool IsClass(LPCTSTR pstrClass);
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CStdPtrArray : public IDuiObject
	{
	public:
		CStdPtrArray(int iPreallocSize = 0);
		CStdPtrArray(const CStdPtrArray& src);
		~CStdPtrArray();

		void Empty();
		void Resize(int iSize);
		bool IsEmpty() const;
		int Find(LPVOID iIndex) const;
		bool Add(LPVOID pData);
		bool SetAt(int iIndex, LPVOID pData);
		bool InsertAt(int iIndex, LPVOID pData);
		bool Remove(int iIndex);
		int GetSize() const;
		LPVOID* GetData();

		LPVOID GetAt(int iIndex) const;
		LPVOID operator[] (int nIndex) const;

		virtual bool    IsClass(LPCTSTR pstrClass);
		virtual LPCTSTR GetClass() const;
		static LPCTSTR GetClassName();
	protected:
		LPVOID* m_ppVoid;
		int m_nCount;
		int m_nAllocated;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CStdValArray : public IDuiObject
	{
	public:
		CStdValArray(int iElementSize, int iPreallocSize = 0);
		~CStdValArray();

		void Empty();
		bool IsEmpty() const;
		bool Add(LPCVOID pData);
		bool Remove(int iIndex);
		int GetSize() const;
		LPVOID GetData();

		LPVOID GetAt(int iIndex) const;
		LPVOID operator[] (int nIndex) const;

		virtual LPCTSTR GetClass() const;
	protected:
		LPBYTE m_pVoid;
		int m_iElementSize;
		int m_nCount;
		int m_nAllocated;
	};
	/////////////////////////////////////////////////////////////////////////////////////
	//
	template<typename T = LPVOID>
	class UILIB_API TStdPtrArray : public CStdPtrArray
	{
	public:
		TStdPtrArray(int iPreallocSize = 0) :CStdPtrArray(iPreallocSize){};
		TStdPtrArray(const TStdPtrArray& src) :CStdPtrArray(src){};
		int Find(T iIndex) const { return CStdPtrArray::Find(iIndex); };
		bool Add(T pData){ return CStdPtrArray::Add(pData); };
		bool SetAt(int iIndex, T pData){ return CStdPtrArray::SetAt(iIndex, pData); };
		bool InsertAt(int iIndex, T pData){ return CStdPtrArray::InsertAt(iIndex, pData); };
		bool Remove(int iIndex, bool bDeleteObj = false){
			if (bDeleteObj){
				T p = GetAt(iIndex);
				if (p)
					delete p;
			}
			return CStdPtrArray::Remove(iIndex);
		}
		T* GetData(){ return static_cast<T>(CStdPtrArray::GetData()); };
		T GetAt(int iIndex) const { return static_cast<T>(CStdPtrArray::GetAt(iIndex)); };
		T operator[] (int nIndex) const{ return static_cast<T>(CStdPtrArray::operator[](nIndex)); };
	};
	/////////////////////////////////////////////////////////////////////////////////////
	//
	template<typename T = LPVOID, typename T1 = T>
	class UILIB_API TStdValArray : public CStdValArray
	{
	public:
		TStdValArray(int iElementSize = sizeof(T), int iPreallocSize = 0) :CStdValArray(iElementSize, iPreallocSize){};
		bool Add(const T1 pData){ return CStdValArray::Add((LPCVOID)&pData); };
		bool InsertAt(int iIndex, const T pData){
			if (iIndex == m_nCount) return Add(pData);
			if (iIndex < 0 || iIndex > m_nCount) return false;
			if (++m_nCount >= m_nAllocated) {
				int nAllocated = m_nAllocated * 2;
				if (nAllocated == 0) nAllocated = 11;
				LPBYTE pVoid = static_cast<LPBYTE>(realloc(m_pVoid, nAllocated * m_iElementSize));
				if (pVoid != NULL) {
					m_nAllocated = nAllocated;
					m_pVoid = pVoid;
				}
				else {
					--m_nCount;
					return false;
				}
			}

			memmove(&m_pVoid + (iIndex + 1) * m_iElementSize, &m_pVoid + iIndex * m_iElementSize, m_iElementSize);
			::CopyMemory(m_pVoid + (iIndex * m_iElementSize), &pData, m_iElementSize);
			return true;
		}
		T GetData(){ return static_cast<T>(CStdValArray::GetData()); };
		T GetAt(int iIndex) const { return *static_cast<T1*>(CStdValArray::GetAt(iIndex)); };
		T operator[] (int nIndex) const{ return (T)CStdValArray:::operator[](nIndex); };
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CDuiString : public IDuiObject
	{
	public:
		enum { MAX_LOCAL_STRING_LEN = 63 };

		CDuiString();
		CDuiString(const TCHAR ch);
		CDuiString(const CDuiString& src);
		CDuiString(LPCTSTR lpsz, int nLen = -1);
		~CDuiString();

		virtual LPCTSTR GetClass() const;
		virtual bool    IsClass(LPCTSTR pstrClass);

		void Empty();
		int GetLength() const;
		bool IsEmpty() const;
		TCHAR GetAt(int nIndex) const;
		void Append(LPCTSTR pstr);
		void Assign(LPCTSTR pstr, int nLength = -1);
		LPCTSTR GetData() const;

		std::wstring GetWString();
		std::string GetAString();

#ifdef _UNICODE
		std::wstring GetString();
#else
		std::string GetString();
#endif

		void SetAt(int nIndex, TCHAR ch);
		virtual operator LPCTSTR() const;

		TCHAR operator[] (int nIndex) const;
		const CDuiString& operator=(const CDuiString& src);
		const CDuiString& operator=(const TCHAR ch);
		const CDuiString& operator=(LPCTSTR pstr);
#ifdef _UNICODE
		const CDuiString& CDuiString::operator=(LPCSTR lpStr);
		const CDuiString& CDuiString::operator+=(LPCSTR lpStr);
#else
		const CDuiString& CDuiString::operator=(LPCWSTR lpwStr);
		const CDuiString& CDuiString::operator+=(LPCWSTR lpwStr);
#endif
		CDuiString operator+(const CDuiString& src) const;
		CDuiString operator+(LPCTSTR pstr) const;
		const CDuiString& operator+=(const CDuiString& src);
		const CDuiString& operator+=(LPCTSTR pstr);
		const CDuiString& operator+=(const TCHAR ch);

		bool operator == (LPCTSTR str) const;
		bool operator != (LPCTSTR str) const;
		bool operator <= (LPCTSTR str) const;
		bool operator <  (LPCTSTR str) const;
		bool operator >= (LPCTSTR str) const;
		bool operator >  (LPCTSTR str) const;

		int Compare(LPCTSTR pstr) const;
		int CompareNoCase(LPCTSTR pstr) const;
		bool IsSame(LPCTSTR pstr) const;
		bool IsSameNoCase(LPCTSTR pstr) const;

		void MakeUpper();
		void MakeLower();

		CDuiString Left(int nLength) const;
		CDuiString Mid(int iPos, int nLength = -1) const;
		CDuiString Right(int nLength) const;

		int Find(TCHAR ch, int iPos = 0) const;
		int Find(LPCTSTR pstr, int iPos = 0) const;
		int ReverseFind(TCHAR ch) const;
		int Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo);

		int __cdecl Format(LPCTSTR pstrFormat, ...);
		int __cdecl SmallFormat(LPCTSTR pstrFormat, ...);

		//Add by dfn.li
		static CDuiString& FormatString(LPCTSTR pstrFormat, ...);
		static CDuiString& SmallFormatString(LPCTSTR pstrFormat,...);		

		void AssignChar(const char* pstr, int nLength = -1);
		static CDuiString& AssignString(const char* pstr, int nLength  = -1);

		std::string str() const;
	protected:
		LPTSTR m_pstr;
		TCHAR m_szBuffer[MAX_LOCAL_STRING_LEN + 1];
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	struct TITEM
	{
		CDuiString Key;
		LPVOID Data;
		struct TITEM* pPrev;
		struct TITEM* pNext;
	};

	class UILIB_API CStdStringPtrMap : public IDuiObject
	{
	public:
		CStdStringPtrMap(int nSize = 83);
		~CStdStringPtrMap();

		virtual LPCTSTR GetClass() const;
		virtual bool    IsClass(LPCTSTR pstrClass);

		void Resize(int nSize = 83);
		LPVOID Find(LPCTSTR key, bool optimize = true) const;
		bool Insert(LPCTSTR key, LPVOID pData);
		LPVOID Set(LPCTSTR key, LPVOID pData);
		bool Remove(LPCTSTR key);
		void RemoveAll();
		int GetSize() const;
		LPCTSTR GetAt(int iIndex) const;
		LPCTSTR operator[] (int nIndex) const;

	protected:
		TITEM** m_aT;
		int m_nBuckets;
		int m_nCount;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CWaitCursor
	{
	public:
		CWaitCursor();
		~CWaitCursor();

	protected:
		HCURSOR m_hOrigCursor;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CVariant : public VARIANT
	{
	public:
		CVariant() 
		{ 
			VariantInit(this); 
		}
		CVariant(int i)
		{
			VariantInit(this);
			this->vt = VT_I4;
			this->intVal = i;
		}
		CVariant(float f)
		{
			VariantInit(this);
			this->vt = VT_R4;
			this->fltVal = f;
		}
		CVariant(LPOLESTR s)
		{
			VariantInit(this);
			this->vt = VT_BSTR;
			this->bstrVal = s;
		}
		CVariant(IDispatch *disp)
		{
			VariantInit(this);
			this->vt = VT_DISPATCH;
			this->pdispVal = disp;
		}

		~CVariant() 
		{ 
			VariantClear(this); 
		}
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API CDuiImage : public CDuiString
	{
	public:
		CDuiImage();
		CDuiImage(const CDuiString& src);
		CDuiImage(const CDuiImage& src);
		CDuiImage(LPCTSTR lpsz, int nLen = -1);
		~CDuiImage();

		virtual LPCTSTR GetClass() const;

		void SetResType(int _iResType);
		void SetHole(bool _bHole);
		void SetTiledX(bool _bTiledX);
		void SetTiledY(bool _bTiledY);
		void SetFade(BYTE _bFade);
		void SetMask(DWORD _dwMask);
		void SetControlRect(RECT _rcControl);
		void SetSource(RECT _rcSource);
		void SetDest(RECT _rcDest);
		void SetCorner(RECT _rcCorner);
		void SetImage(LPCTSTR _strImage);
		void SetImage(LPCTSTR _strImage,RECT _rcControl);
		void SetRes(LPCTSTR _strRes);
		int GetResType();
		bool GetNeedDestRect();
		bool GetHole();
		bool GetTiledX();
		bool GetTiledY();
		BYTE GetFade();
		DWORD GetMask();
		RECT GetSource();
		RECT GetDest();
		RECT GetCorner();
		CDuiString GetRes();
		CDuiString GetImagePath();
		CDuiString GetImageSetting();
		const CDuiImage& operator=(const CDuiImage& src);
		const CDuiImage& operator=(LPCTSTR pstr);
		bool operator==(CDuiImage& src);
		bool operator==(CDuiString& src);
		virtual operator LPCTSTR() const;
	private:
		int m_iResType;
		bool m_bNeedDestRect;
		bool m_bHole;
		bool m_bTiledX;
		bool m_bTiledY;
		BYTE m_bFade;
		DWORD m_dwMask;
		RECT m_rcItem;
		RECT m_rcSource;
		RECT m_rcDest;
		RECT m_rcCorner;
		CDuiString m_sRes;
		CDuiString m_sImageFile;
	};

	/////////////////////////////////////////////
	//
	class UILIB_API CDuiImageEx
	{
	public:
		enum EM_RES_FROM
		{
			FILE = 0,
			ZIP,
			RESOURCE,
			NETWORK,
		};
		union ResInfo
		{
			LPCTSTR strFile;
			LPVOID pBuf;
		};
	public:
		CDuiImageEx();
		CDuiImageEx(const CDuiImageEx& src);
		CDuiImageEx(LPCTSTR lpsz, int nLen = -1);
		~CDuiImageEx();
	private:
		int m_iResType;
		bool m_bNeedDestRect;
		bool m_bHole;
		bool m_bTiledX;
		bool m_bTiledY;
		EM_RES_FROM m_iResFrom;
		ResInfo m_Res;
		BYTE m_bFade;
		DWORD m_dwMask;
		RECT m_rcItem;
		RECT m_rcSource;
		RECT m_rcDest;
		RECT m_rcCorner;
		CDuiString m_sRes;
		CDuiString m_sImageFile;
	};

}// namespace DuiLib

#endif // __UTILS_H__