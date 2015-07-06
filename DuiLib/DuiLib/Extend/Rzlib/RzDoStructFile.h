/********************************************************************
    FileName :  RzDoStructFile.h   
    Version  :  0.10
    Date     :	2010-2-11 9:59:54
    Author   :  小家伙
    Comment  :  目前暂不支持wchar_t字符内型

*********************************************************************/
#ifndef __RZDOSTRUCTFILE_H__
#define __RZDOSTRUCTFILE_H__
#include <fstream>
#include <vector>
#include <algorithm>
#include "RzDReadAWriteLock.h"

using namespace std;

template <class T,class U = vector<T> >
class RZ_DLL_API CRzDoStructFile
{ 
public:
	typedef typename U::value_type value_type;
	typedef typename U::size_type size_type;
	typedef typename U  container_type;
	typedef typename U::const_iterator const_itor;
private:
	CRzDReadAWriteLock m_Lock; 
	U m_contian;	
	const unsigned int m_nSize;
	typedef bool (__stdcall *EnumVaulePro)(const value_type&,void*);
public:
    CRzDoStructFile();
    ~CRzDoStructFile();
public:
    bool ReadFileToMem(LPCTSTR lpFileName);
	bool WriteFileFormMem(LPCTSTR lpFileName);	
	void InsertVaule(const value_type& x);
	void Clear();
	void EnumVaule(const EnumVaulePro lpEnumVaulePro,void* pParam = NULL);
};

template <class T,class U>
CRzDoStructFile<T,U>::CRzDoStructFile():m_nSize(sizeof(value_type))
{	
}
template <class T, class U>
CRzDoStructFile<T,U>::~CRzDoStructFile()
{
}
template <class T, class U>
bool CRzDoStructFile<T,U>::ReadFileToMem(LPCTSTR lpFileName)
{
	assert(lpFileName);
	if(!lpFileName) return false;  
	bool bVar = false;    
	
	m_Lock.EnterWrite();  
	ifstream fs(lpFileName,ios::in|ios::binary);
	if(!fs.is_open()) goto _End_Fun;
	
	{
	  bVar = true;
	  m_contian.clear();
	  value_type vule;
      while(fs.read((char*)&vule,m_nSize))
      {
    	m_contian.push_back(vule);
	  }
	  fs.close();
	  goto _End_Fun;
	}
	
_End_Fun:
    m_Lock.LeaveWrite();                	
    return bVar;	
}
template <class T,class U>
bool CRzDoStructFile<T,U>::WriteFileFormMem(LPCTSTR lpFileName)
{
	assert(lpFileName);
	if(!lpFileName) return false;
	bool bVar = false;
	                  
    m_Lock.EnterRead();  
	ofstream fs(lpFileName,ios::out|ios::binary);
	if(!fs.is_open()) goto _End_Fun;
	
	{
	  bVar = true;	
	  const_itor i;
	  for(i=m_contian.begin(); i!=m_contian.end(); ++i)
	  {
		value_type vale = *i;	
	    fs.write((char*)&vale,m_nSize);
	  }
	  fs.close();
	  goto _End_Fun;
	}

_End_Fun:
    m_Lock.LeaveRead();	 
	return bVar;
}
template <class T,class U>
void CRzDoStructFile<T,U>::EnumVaule(const EnumVaulePro lpEnumVaulePro, void *pParam = NULL)
{
	m_Lock.EnterRead();
	const_itor i;
	for (i=m_contian.begin();i!=m_contian.end();++i)
	{
		if (  lpEnumVaulePro(*i,pParam) )	break;	
	}
    m_Lock.LeaveRead();
}
template <class T,class U>
void CRzDoStructFile<T,U>::InsertVaule(const value_type& x)
{
	m_Lock.EnterWrite();
	m_contian.push_back(x);
	m_Lock.LeaveWrite();
}
template <class T,class U>
void CRzDoStructFile<T,U>::Clear()
{
	m_Lock.EnterWrite();
	m_contian.clear();
	m_Lock.LeaveWrite();
}
#endif
