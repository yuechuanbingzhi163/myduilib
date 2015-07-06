#ifndef _IDuiObject_H
#define _IDuiObject_H

namespace DuiLib
{
	class UILIB_API IDuiObject
	{
	public:
		virtual LPCTSTR GetClass() const = 0;
		virtual bool    IsClass(LPCTSTR pstrClass) = 0;
		/*{
			return ((GetClass()) != NULL) && (_tcscmp(pstrClass, GetClass()) == 0);
		}*/
	};
}

#endif//_IDuiObject_H