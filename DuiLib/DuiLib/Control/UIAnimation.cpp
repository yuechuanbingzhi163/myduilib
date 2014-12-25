#include "StdAfx.h"
#include "UIAnimation.h"

namespace DuiLib {

	CUIAnimation::CUIAnimation(CControlUI* pOwner)
	{
		ASSERT(pOwner != NULL);
		m_pControl = pOwner;
	}

	BOOL CUIAnimation::StartAnimation(int nElapse, int nTotalFrame, int nAnimationID /*= 0*/, BOOL bLoop/* = FALSE*/)
	{
		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		if( NULL != pData 
			|| nElapse <= 0
			|| nTotalFrame <= 0
			|| NULL == m_pControl )
		{
			ASSERT(FALSE);
			return FALSE;
		}

		CAnimationData* pAnimation = new CAnimationData(nElapse, nTotalFrame, nAnimationID, bLoop);
		if( NULL == pAnimation ) return FALSE;

		if(m_pControl->GetManager()->SetTimer( m_pControl, nAnimationID, nElapse ))
		{
			m_arAnimations.Add(pAnimation);
			return TRUE;
		}
		return FALSE;
	}

	VOID CUIAnimation::StopAnimation(int nAnimationID /*= 0*/)
	{
		if(m_pControl == NULL) return;

		if(nAnimationID  != 0)
		{
			CAnimationData* pData = GetAnimationDataByID(nAnimationID);
			if( NULL != pData )
			{
				m_pControl->GetManager()->KillTimer( m_pControl, nAnimationID );
				RemoveAnimationData(pData);
				return;
			}
		}
		else
		{
			int nCount = m_arAnimations.GetSize();
			for(int i=0; i<nCount; ++i)
			{
				CAnimationData* pData = static_cast<CAnimationData*>(m_arAnimations[i]);
				m_pControl->GetManager()->KillTimer( m_pControl, pData->m_nAnimationID );
			}
			m_arAnimations.Empty();
		}
	}

	BOOL CUIAnimation::IsAnimationRunning(int nAnimationID) const
	{
		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		return NULL != pData;
	}

	int CUIAnimation::GetCurrentFrame(int nAnimationID/* = 0*/) const
	{
		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		if( NULL == pData )
		{
			ASSERT(FALSE);
			return -1;
		}
		return pData->m_nCurFrame;
	}

	BOOL CUIAnimation::SetCurrentFrame(int nFrame, int nAnimationID/* = 0*/)
	{
		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		if( NULL == pData)
		{
			ASSERT(FALSE);
			return FALSE;
		}

		if(nFrame >= 0 && nFrame <= pData->m_nTotalFrame)
		{
			pData->m_nCurFrame = nFrame;
			return TRUE;
		}
		else
		{
			ASSERT(FALSE);
		}
		return FALSE;
	}

	VOID CUIAnimation::OnAnimationElapse(int nAnimationID)
	{
		if(m_pControl == NULL) return;

		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		if( NULL == pData ) return;

		int nCurFrame = pData->m_nCurFrame;
		if(nCurFrame == 0)
		{
			OnAnimationStart(nAnimationID, pData->m_bFirstLoop);
			pData->m_bFirstLoop = FALSE;
		}

		OnAnimationStep(pData->m_nTotalFrame, nCurFrame, nAnimationID);

		if(nCurFrame >= pData->m_nTotalFrame)
		{
			OnAnimationStop(nAnimationID);
			if(pData->m_bLoop)
			{
				pData->m_nCurFrame = 0;
			}
			else
			{
				m_pControl->GetManager()->KillTimer( m_pControl, nAnimationID );
				RemoveAnimationData(pData);
				pData = NULL;
			}
		}

		if( NULL != pData )
		{
			++(pData->m_nCurFrame);
		}
	}

	CAnimationData* CUIAnimation::GetAnimationDataByID(int nAnimationID) const
	{
		CAnimationData* pRet = NULL;
		int nCount = m_arAnimations.GetSize();
		for(int i=0; i<nCount; ++i)
		{
			CAnimationData* pData = static_cast<CAnimationData*>(m_arAnimations[i]);
			if(pData->m_nAnimationID == nAnimationID)
			{
				pRet = pData;
				break;
			}
		}

		return pRet;
	}
	void CUIAnimation::RemoveAnimationData(CAnimationData* pData)
	{
		int size = m_arAnimations.GetSize();
		while (size-- > 0)
		{
			if(m_arAnimations[size] == pData)
				m_arAnimations.Remove(size);
		}
	}

} // namespace DuiLib