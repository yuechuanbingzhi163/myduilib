#include "StdAfx.h"
#include "Delegate.h"


YDelegateBase::YDelegateBase(void* pObject, void* pFn) 
{
    m_pObject = pObject;
    m_pFn = pFn; 
}

YDelegateBase::YDelegateBase(const YDelegateBase& rhs) 
{
    m_pObject = rhs.m_pObject;
    m_pFn = rhs.m_pFn; 
}

YDelegateBase::~YDelegateBase()
{

}

bool YDelegateBase::Equals(const YDelegateBase& rhs) const 
{
    return m_pObject == rhs.m_pObject && m_pFn == rhs.m_pFn; 
}

bool YDelegateBase::operator() (void* param) 
{
    return Invoke(param); 
}

void* YDelegateBase::GetFn() 
{
    return m_pFn; 
}

void* YDelegateBase::GetObject() 
{
    return m_pObject; 
}






