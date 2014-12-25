#ifndef __UIMEDIA_H
#define __UIMEDIA_H

#pragma once
#include "../../YMusic.h"

namespace DuiLib
{
// 将HWND显示到CControlUI上面
class CMediaUI: public CControlUI
{
public:
    CMediaUI();

    static LPCTSTR GetClassName();
    
	LPCTSTR GetClass() const;

    virtual void SetVisible(bool bVisible = true);
   

    virtual void SetInternVisible(bool bVisible = true);
  

    virtual void SetPos(RECT rc);
 

    BOOL Attach(HWND hWndNew);
 

    HWND Detach();
 

    HWND GetHWND();


protected:
    HWND m_hWnd;
};
}

#endif//__UIMEDIA_H