
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include "RzIni.h"
CRzIni::CRzIni()
{
}
CRzIni::CRzIni(const TCHAR* pFile)
{
	assert(pFile);
	OpenIni(pFile);
}
CRzIni::~CRzIni()
{
}
void CRzIni::SetIniFile(const TCHAR* pFile)
{
	assert(pFile);
	OpenIni(pFile);
}
void CRzIni::OpenIni(const TCHAR* pFile)
{
    Rzifstream ifs(pFile);
	if(!ifs.is_open()) return;
	vector<Rzstring> filebuf;
	vector<Rzstring>::const_iterator itor;
	while(!ifs.eof())
	{
		Rzstring buf;	
		getline(ifs,buf);
		filebuf.push_back(buf);
	}
	Rzstring curSection = _T("");
	for(itor=filebuf.begin(); itor!=filebuf.end(); ++itor)
	{
        curSection = DoSwitchALine(*itor,curSection);
	}	
}
Rzstring CRzIni::DoSwitchALine(const Rzstring &aLineStr,Rzstring &curSection)
{
	if(aLineStr.empty()) return curSection;  //空行 
	if(_T(';') == aLineStr.at(0)) return curSection; //注释行
	 
	Rzstring::size_type n;	
	if(_T('[') == aLineStr.at(0))//section项 
	{
	   n = aLineStr.find(_T(']'),1);
	   curSection = aLineStr.substr(1,n-1);
	   AddSection(curSection);
	   return curSection;		
	}
	                              //key & vaule     
	Rzstring strKey;
	Rzstring strVaule;
	n = aLineStr.find(_T('='),0);
	if(Rzstring::npos == n) return curSection; //没有=号 
	
    strKey = aLineStr.substr(0,n);
	if(Rzstring::npos == n+1)   //=号后无内容 
	   strVaule = _T("");  
	else   
       strVaule = aLineStr.substr(n+1);
	AddKey(curSection,strKey,strVaule);
	return curSection;		
}
void CRzIni::AddSection(Rzstring &SectionStr)
{
	m_SectionMap[SectionStr];
}
void CRzIni::AddKey(Rzstring &SectionStr,Rzstring &KeyStr,Rzstring &vauleStr)
{
	m_SectionMap[SectionStr][KeyStr] = vauleStr;
}
void CRzIni::PrintIni()
{
	SectionMapItor itor;
	KeyMapItor n;
	for(itor=m_SectionMap.begin(); itor!=m_SectionMap.end(); ++itor)
	{
		//Rzcout<<_T('[') << itor->first << _T("]\n");
		for(n=itor->second.begin(); n!=itor->second.end(); ++n)
		{
			//Rzcout<<n->first <<_T("=") <<n->second <<endl;
		}		
	}
}
const Rzstring&  CRzIni::GetStr(const Rzstring &SectionStr,const Rzstring &KeyStr,const Rzstring &DefaultStr/*=_T("")*/)
{
	SectionMapItor itor;
	itor = m_SectionMap.find(SectionStr);
	if(m_SectionMap.end() == itor) return DefaultStr;

	KeyMapItor n;
	n = itor->second.find(KeyStr);
	if(itor->second.end() == n) return DefaultStr;

	return m_SectionMap[SectionStr][KeyStr];

}
