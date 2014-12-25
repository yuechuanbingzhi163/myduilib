#include "StdAfx.h"
#include "UISeqAnimation.h"
namespace DuiLib {

	CSeqAnimation::CSeqAnimation(CControlUI* pOwner):CUIAnimation(pOwner),m_nTotalFrameCnt(0),m_nCurFrame(0)
	{

	}

	CSeqAnimation::~CSeqAnimation()
	{
		StopAnimation();
	}

	void CSeqAnimation::SetSeqImages(LPCTSTR lpszImage)
	{
		ASSERT(lpszImage);
		m_sImagesName = lpszImage;
	}
	CDuiString CSeqAnimation::GetSeqImages() const
	{
		return m_sImagesName;
	}
	void CSeqAnimation::SetTotalFrameCnt(unsigned int n)
	{
		ASSERT(n);
		m_nTotalFrameCnt = n;
	}
	unsigned int CSeqAnimation::GetTotalFrameCnt() const
	{
		return m_nTotalFrameCnt;
	}

	void CSeqAnimation::DoAnimationEvent(TEventUI& event)
	{
		switch (event.Type)
		{
		case UIEVENT_MOUSEENTER: EnterSeqIn(); break;
		case UIEVENT_MOUSELEAVE: EnterSeqOut(); break;
		case UIEVENT_TIMER: OnAnimationElapse(event.wParam); break;
		default: break;
		}
	}
	BOOL CSeqAnimation::IsAnimationing() const
	{
		return (IsAnimationRunning(SEQ_IN_ID) || IsAnimationRunning(SEQ_OUT_ID)); 
	}
	void CSeqAnimation::OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID)
	{
		if(SEQ_IN_ID == nAnimationID)
		{
			m_nCurFrame = nCurFrame;
			UpdateAnimation();
		}
		else if(SEQ_OUT_ID == nAnimationID)
		{
			m_nCurFrame =(nTotalFrame - nCurFrame);
			UpdateAnimation();
		}
	}

	void CSeqAnimation::PaintAnimationStatusImage(HDC hDC,LPCTSTR lpszNormalImage,LPCTSTR lpszHotImage,UINT nState)
	{
		ASSERT(hDC);
		if(!m_pControl)
			return;

		if( IsAnimationing() && !m_sImagesName.IsEmpty())
		{
			unsigned int n = ((m_nCurFrame > m_nTotalFrameCnt) ? m_nTotalFrameCnt : m_nCurFrame);
			if(m_nCurFrame > 1)
			{
				m_pControl->DrawImage(hDC,m_sImagesName.GetData(),NULL,false,255,m_nCurFrame-1);
				m_pControl->DrawImage(hDC, m_sImagesName.GetData(), NULL, false,255,m_nCurFrame);
			}
			else
			{
				m_pControl->DrawImage(hDC,m_sImagesName.GetData(),NULL,false,255,m_nCurFrame);
			}
		}
		else
		{
			if(0 != (nState&UISTATE_HOT))
				m_pControl->DrawImage(hDC,lpszHotImage);
			else
				m_pControl->DrawImage(hDC,lpszNormalImage);
		}
	}

	BOOL CSeqAnimation::EnterSeqIn()
	{
		if(!IsAnimationRunning(SEQ_IN_ID) && (m_nTotalFrameCnt > 0) && !m_sImagesName.IsEmpty())
		{
			StopAnimation(SEQ_OUT_ID);
			StartAnimation( SEQ_ELAPSE, m_nTotalFrameCnt, SEQ_IN_ID);
		//	UpdateAnimation();
			return TRUE;
		}
		return FALSE;
	}
	BOOL CSeqAnimation::EnterSeqOut()
	{
		if(!IsAnimationRunning(SEQ_OUT_ID) && (m_nTotalFrameCnt > 0) && !m_sImagesName.IsEmpty())
		{
			StopAnimation(SEQ_IN_ID);
			StartAnimation( SEQ_ELAPSE, m_nTotalFrameCnt, SEQ_OUT_ID);
			//UpdateAnimation();
			return TRUE;
		}
		return FALSE;
	}
	void CSeqAnimation::UpdateAnimation()
	{
		ASSERT(m_pControl);
		if(m_pControl)
			m_pControl->Invalidate();
	}
}