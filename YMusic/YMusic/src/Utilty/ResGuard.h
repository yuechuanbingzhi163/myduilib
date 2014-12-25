#ifndef __RES_GUARD_H__
#define __RES_GUARD_H__

///////////////////////////////////////////////////////////////////////////////
#pragma once

class CResGuard
{
public:
	CResGuard();

	~CResGuard();


	// IsGuarded is used for debugging
	BOOL IsGuarded() const;


public:
	class CGuard
	{
	public:
		CGuard(CResGuard &rg) : m_rg(rg)
		{
			m_rg.Guard();
		};
		~CGuard()
		{
			m_rg.Unguard();
		}

	private:
		CResGuard &m_rg;
	};

private:
	void Guard();

	void Unguard();


	// Guard/Unguard can only be accessed by the nested CGuard class.
	friend class CResGuard::CGuard;

private:
	CRITICAL_SECTION m_cs;
	long m_lGrdCnt;   // # of EnterCriticalSection calls
};

#endif