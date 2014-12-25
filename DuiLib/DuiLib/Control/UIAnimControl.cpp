#include "StdAfx.h"
#include "UIAnimControl.h"

namespace DuiLib {

	REGIST_DUICLASS(CAnimControlUI);

	CAnimControlUI::CAnimControlUI():CUIAnimation(this),m_uElapse(10),m_uCurFrame(0)
	{

	}

	CAnimControlUI::~CAnimControlUI()
	{
		Stop();
	}

	LPCTSTR CAnimControlUI::GetClassName()
	{
		return _T("AnimControlUI");
	}

	LPCTSTR CAnimControlUI::GetClass() const
	{
		return CAnimControlUI::GetClassName();
	}
	LPVOID CAnimControlUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("AnimControl")) == 0 ) return static_cast<CAnimControlUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	void CAnimControlUI::SetImages(LPCTSTR pstrImagesName)
	{
		ASSERT(pstrImagesName);
		m_sImagesName = pstrImagesName;
	}
	CDuiString CAnimControlUI::GetImages() const
	{
		return m_sImagesName;
	}

	void CAnimControlUI::SetElapse(unsigned int uEllapse)
	{
		if(uEllapse > 0)
			m_uElapse = uEllapse;
	}
	unsigned int CAnimControlUI::GetElapse() const
	{
		return m_uElapse;
	}

	void CAnimControlUI::SetTotalFrameCnt(unsigned int uTotalFrameCnt)
	{
		ASSERT(uTotalFrameCnt);
		m_uTotalFrameCnt = uTotalFrameCnt;
	}
	unsigned int CAnimControlUI::GetTotalFrameCnt() const
	{
		return m_uTotalFrameCnt;
	}

	void CAnimControlUI::DoEvent(TEventUI& event)
	{
		if(UIEVENT_TIMER == event.Type && event.wParam == CAnimControlUI::ANIMATION_ID)
			OnAnimationElapse(event.wParam);
		else
			CLabelUI::DoEvent(event);
	}
	void CAnimControlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("images")) == 0 ) SetImages(pstrValue);
		else if( _tcscmp(pstrName, _T("framecnt")) == 0 ) SetTotalFrameCnt(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("elapse")) == 0 ) SetElapse(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("auto")) == 0 ) 
		{
			if(_tcscmp(pstrValue,_T("true")) == 0) Start();
		}
		else CLabelUI::SetAttribute(pstrName,pstrValue);
	}

	void CAnimControlUI::PaintStatusImage(HDC hDC)
	{
		if(!m_sImagesName.IsEmpty())
		{
			unsigned int n = ((m_uCurFrame > m_uTotalFrameCnt) ? m_uTotalFrameCnt : m_uCurFrame);
			if(m_uCurFrame > 1)
			{
				m_pControl->DrawImage(hDC,m_sImagesName.GetData(),NULL,false,255,m_uCurFrame-1);
				m_pControl->DrawImage(hDC, m_sImagesName.GetData(), NULL, false,255,m_uCurFrame);
			}
			else
			{
				m_pControl->DrawImage(hDC,m_sImagesName.GetData(),NULL,false,255,m_uCurFrame);
			}
		}
	}
	void CAnimControlUI::Start()
	{
		if(!IsRunning())
		{
			StartAnimation(m_uElapse,m_uTotalFrameCnt,ANIMATION_ID,TRUE);
		}
	}
	void CAnimControlUI::Stop()
	{
		if(IsRunning())
		{
			StopAnimation(ANIMATION_ID);
		}
	}
	BOOL CAnimControlUI::IsRunning() const
	{
		return IsAnimationRunning(ANIMATION_ID);
	}
	void CAnimControlUI::OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID)
	{
		m_uCurFrame = nCurFrame;
		Invalidate();
	}
}