#include "StdAfx.h"
#include "UIGifAnim.h"
#include <atlstr.h>

DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;
typedef struct
{ 
	int index;                 // index of this file within the zip
	char name[MAX_PATH];       // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;
typedef struct
{ 
	int index;                 // index of this file within the zip
	TCHAR name[MAX_PATH];      // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRYW;
#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern HZIP OpenZipU(void *z,unsigned int len,DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);
#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif
extern ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern ZRESULT FindZipItemA(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern ZRESULT FindZipItemW(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);

extern "C"
{
	extern unsigned char *stbi_load_from_memory(unsigned char const *buffer, int len, int *x, int *y, \
		int *comp, int req_comp);
	extern void     stbi_image_free(void *retval_from_stbi_load);

};
namespace DuiLib
{

	REGIST_DUICLASS(CGifAnimUI);

	CGifAnimUI::CGifAnimUI(void)
	{
		m_pGifImage			=	NULL;
		m_pPropertyItem		=	NULL;
		m_nFrameCount		=	0;	
		m_nFramePosition	=	0;	
		GdiplusStartup( &m_gdiplusToken,&m_gdiplusStartupInput, NULL); // 加载GDI接口
	}


	CGifAnimUI::CGifAnimUI(CPaintManagerUI& paint_manager) 
	{
		m_pGifImage			=	NULL;
		m_pPropertyItem		=	NULL;
		m_nFrameCount		=	0;	
		m_nFramePosition	=	0;	
		GdiplusStartup( &m_gdiplusToken,&m_gdiplusStartupInput, NULL); // 加载GDI接口
		SetManager(&paint_manager, NULL);
	}

	CGifAnimUI::~CGifAnimUI(void)
	{
		DeleteGif();
		m_pManager->KillTimer( this, EVENT_TIEM_ID );
		GdiplusShutdown( m_gdiplusToken );	//  卸载GDI接口
	}
	LPCTSTR CGifAnimUI::GetClassName()
	{
		return _T("GifAnimUI");
	}
	//************************************
	// Method:    GetClass
	// FullName:  CGifAnimUI::GetClass
	// Access:    public 
	// Returns:   LPCTSTR
	// Qualifier: const
	// Note:	  
	//************************************
	LPCTSTR CGifAnimUI::GetClass() const
	{
		try
		{
			return _T("GifAnimUI");
		}
		catch (...)
		{
			throw "CGifAnimUI::GetClass";
		}
	}

	//************************************
	// Method:    GetInterface
	// FullName:  CGifAnimUI::GetInterface
	// Access:    public 
	// Returns:   LPVOID
	// Qualifier:
	// Parameter: LPCTSTR pstrName
	// Note:	  
	//************************************
	LPVOID CGifAnimUI::GetInterface( LPCTSTR pstrName )
	{
		try
		{
			if( _tcscmp(pstrName, _T("GifAnim")) == 0 ) return static_cast<CGifAnimUI*>(this);
			return CLabelUI::GetInterface(pstrName);
		}
		catch (...)
		{
			throw "CGifAnimUI::GetInterface";
		}
	}

	//************************************
	// Method:    DoPaint
	// FullName:  CGifAnimUI::DoPaint
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: HDC hDC
	// Parameter: const RECT & rcPaint
	// Note:	  
	//************************************
	void CGifAnimUI::DoPaint( HDC hDC, const RECT& rcPaint )
	{
		try
		{
			if( !::IntersectRect( &m_rcPaint, &rcPaint, &m_rcItem ) ) return;

			if ( NULL == m_pGifImage )
			{		
				InitGifImage();
			}
			DrawFrame( hDC );

			__super::DoPaint(hDC,rcPaint);
		}
		catch (...)
		{
			throw "CGifAnimUI::DoPaint";
		}
	}

	//************************************
	// Method:    DoEvent
	// FullName:  CGifAnimUI::DoEvent
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: TEventUI & event
	// Note:	  
	//************************************
	void CGifAnimUI::DoEvent( TEventUI& event )
	{
		try
		{
			if( event.Type == UIEVENT_TIMER )
				OnTimer( (UINT_PTR)event.wParam );
			else if( event.Type == UIEVENT_SETCURSOR ) {
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HELP)));
				return;
			}
			else 
				CLabelUI::DoEvent(event);
		}
		catch (...)
		{
			throw "CGifAnimUI::DoEvent";
		}
	}
	CDuiString CGifAnimUI::GetGifImage() const
	{
		return m_strImage;
	}
	void CGifAnimUI::SetGifImage(LPCTSTR pstrText)
	{
		if(pstrText == m_strImage) return ;
		m_strImage = pstrText;
//		InitGifImage();
		Invalidate();
	}
	void CGifAnimUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("gifimage")) == 0 ) SetGifImage(pstrValue);
		else __super::SetAttribute(pstrName,pstrValue);
	}

	//************************************
	// Method:    InitGifImage
	// FullName:  CGifAnimUI::InitGifImage
	// Access:    private 
	// Returns:   void
	// Qualifier:
	// Note:	  
	//************************************
	void CGifAnimUI::InitGifImage()
	{
		try
		{
			CDuiString sImageName	=	GetGifImage();
			CDuiString sZipPith = CPaintManagerUI::GetResourceZip();
			CDuiString sImagePath	=	CPaintManagerUI::GetResourcePath();
			bool bZip = sZipPith.IsEmpty() ? false : true;

			bZip ? sImagePath += sZipPith : sImagePath += sImageName;
			if ( sImagePath.IsEmpty() ) return;
			if(!bZip)
				m_pGifImage = Image::FromFile( CStringW((LPCTSTR)sImagePath) );
			else
			{
				CDuiString sFile = sImagePath;
				HZIP hz = NULL;
				if( CPaintManagerUI::IsCachedResourceZip() ) hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
				else hz = (HZIP)OpenZip((void*)sFile.GetData(), 0, 2);
				if( hz == NULL ) return;
				ZIPENTRY ze; 
				int i; 
				if( FindZipItem(hz, sImageName.GetData(), true, &i, &ze) != 0 ) return;
				LPBYTE pData = NULL;
				DWORD dwSize = 0;
				dwSize = ze.unc_size;
				if( dwSize == 0 ) return;
				pData = new BYTE[ dwSize ];
				int res = UnzipItem(hz, i, pData, dwSize, 3);
				if( res != 0x00000000 && res != 0x00000600) {
					delete[] pData;
					pData = NULL;
					if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
				}
				if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);

				if (!pData)
				{
					//::MessageBox(0, _T("读取图片数据失败！"), _T("抓BUG"), MB_OK);
					return ;
				}
				
				HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, dwSize);
				if (hMemBmp == NULL) return ;
				IStream* pStream = NULL;
				CreateStreamOnHGlobal(hMemBmp,FALSE,&pStream);
				if(!pStream) GlobalFree(hMemBmp);
				if(!pStream)return ;
				BYTE* pbyBmp = (BYTE *)GlobalLock(hMemBmp);
				memcpy(pbyBmp,pData,dwSize);
				m_pGifImage = Image::FromStream(pStream);
	//			pStream->Release();
				GlobalUnlock(hMemBmp);
				GlobalFree(hMemBmp);
				delete []pData;
				pData = NULL;
			} 
			if ( NULL == m_pGifImage ) return;
			UINT nCount	= 0;
			nCount	=	m_pGifImage->GetFrameDimensionsCount();
			if(nCount == 0) return ;
			GUID* pDimensionIDs	=	new GUID[ nCount ];
			m_pGifImage->GetFrameDimensionsList( pDimensionIDs, nCount );
			m_nFrameCount	=	m_pGifImage->GetFrameCount( &pDimensionIDs[0] );
			int nSize		=	m_pGifImage->GetPropertyItemSize( PropertyTagFrameDelay );
			m_pPropertyItem	=	(PropertyItem*) malloc( nSize );
			m_pGifImage->GetPropertyItem( PropertyTagFrameDelay, nSize, m_pPropertyItem );
			delete  []pDimensionIDs;
			pDimensionIDs = NULL;

			long lPause = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 10;
			if ( lPause == 0 ) lPause = 100;
			m_pManager->SetTimer( this, EVENT_TIEM_ID, lPause );
		}
		catch (...)
		{
			throw "CGifAnimUI::InitGifImage";
		}
	}

	//************************************
	// Method:    DeleteGif
	// FullName:  CGifAnimUI::DeleteGif
	// Access:    private 
	// Returns:   void
	// Qualifier:
	// Note:	  
	//************************************
	void CGifAnimUI::DeleteGif()
	{
		try
		{
			if ( m_pGifImage != NULL )
			{
				delete m_pGifImage;
				m_pGifImage = NULL;
			}

			if ( m_pPropertyItem != NULL )
			{
				free( m_pPropertyItem );
				m_pPropertyItem = NULL;
			}
			m_nFrameCount		=	0;	
			m_nFramePosition	=	0;	
		}
		catch (...)
		{
			throw "CGifAnimUI::DeleteGif";
		}
	}

	//************************************
	// Method:    OnTimer
	// FullName:  CGifAnimUI::OnTimer
	// Access:    private 
	// Returns:   void
	// Qualifier:
	// Parameter: UINT_PTR idEvent
	// Note:	  
	//************************************
	void CGifAnimUI::OnTimer( UINT_PTR idEvent )
	{
		try
		{
			if ( idEvent != EVENT_TIEM_ID )
				return;
			m_pManager->KillTimer( this, EVENT_TIEM_ID );
			m_nFramePosition++;
			if (m_nFramePosition == m_nFrameCount)
				m_nFramePosition = 0;
			this->Invalidate();
			long lPause = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 10;
			if ( lPause == 0 ) lPause = 100;
			m_pManager->SetTimer( this, EVENT_TIEM_ID, lPause );
		}
		catch (...)
		{
			throw "CGifAnimUI::OnTimer";
		}
	}

	//************************************
	// Method:    DrawFrame
	// FullName:  CGifAnimUI::DrawFrame
	// Access:    private 
	// Returns:   void
	// Qualifier:
	// Parameter: HDC hDC
	// Note:	  
	//************************************
	void CGifAnimUI::DrawFrame( HDC hDC )
	{
		try
		{
			if ( NULL == hDC || NULL == m_pGifImage ) return;
			GUID pageGuid = Gdiplus::FrameDimensionTime;
			Graphics graphics( hDC );
			int nWidth = m_rcItem.right - m_rcItem.left;
			int nHeight = m_rcItem.bottom - m_rcItem.top;
			graphics.DrawImage( m_pGifImage, m_rcItem.left, m_rcItem.top,nWidth,nHeight);
			
			m_pGifImage->SelectActiveFrame( &pageGuid, m_nFramePosition );
		}
		catch (...)
		{
			throw "CGifAnimUI::DrawFrame";
		}
	}
}
