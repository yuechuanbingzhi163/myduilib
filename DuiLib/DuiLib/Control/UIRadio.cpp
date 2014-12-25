#include "StdAfx.h"
#include "UIRadio.h"

namespace DuiLib
{

	REGIST_DUICLASS(CRadioUI);

	CRadioUI::CRadioUI(void)
	{
	}


	CRadioUI::~CRadioUI(void)
	{
	}

	//************************************
	// Method:    GetClass
	// FullName:  CRadioUI::GetClass
	// Access:    public 
	// Returns:   LPCTSTR
	// Qualifier: const
	// Note:	  
	//************************************
	LPCTSTR CRadioUI::GetClass() const
	{
		return _T("RadioUI");
	}

	//************************************
	// Method:    GetInterface
	// FullName:  CRadioUI::GetInterface
	// Access:    public 
	// Returns:   LPVOID
	// Qualifier:
	// Parameter: LPCTSTR pstrName
	// Note:	  
	//************************************
	LPVOID CRadioUI::GetInterface( LPCTSTR pstrName )
	{
		try
		{
			if( _tcscmp(pstrName, _T("Radio")) == 0 ) return static_cast<CRadioUI*>(this);
			return COptionUI::GetInterface(pstrName);
		}
		catch (...)
		{
			throw "CRadioUI::GetInterface";
		}
	}

	LPCTSTR CRadioUI::GetClassName()
	{
		return _T("RadioUI");
	}

}

