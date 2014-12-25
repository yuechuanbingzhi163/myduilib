#ifndef UIRadio_h__
#define UIRadio_h__

#pragma once

namespace DuiLib
{
	class UILIB_API CRadioUI : public COptionUI
	{
	public:
		CRadioUI(void);
		~CRadioUI(void);

		LPCTSTR GetClass() const;
		static LPCTSTR GetClassName();
		LPVOID GetInterface(LPCTSTR pstrName);
	};
}



#endif // UIRadioBox_h__
