#ifndef __RZINI_H__
#define __RZINI_H__
#include <rzlib\RzType.h>
#include <map> 
#include <string>
#include <fstream>
#include <vector>

_RzStdBegin

class CRzIni
{
public:
	typedef std::string                                          stringtype;
	typedef std::ifstream                                        ifstreamtype;

	typedef std::map<stringtype,stringtype,std::less<stringtype> >    KeyMap;
	typedef KeyMap::iterator                                     KeyMapItor;

	typedef std::map<stringtype,KeyMap,std::less<stringtype> >        SectionMap;
	typedef SectionMap::iterator                                 SectionMapItor;

private:
   SectionMap  _SectionMap;
public:
	CRzIni(){};
	~CRzIni(){};   
public:
	KeyMap& operator[] (const char* psection)
	{
		return (*this)[stringtype(psection)];
	}
	KeyMap& operator[](const stringtype& section)
	{
		return _SectionMap[section];
	}
	const stringtype& operator()(const char* psection,const char* pkey)
	{
		return (*this)[psection][pkey];		
	}
	bool OpenIni(const char* pFile)
	{
		assert(pFile);
		if(NULL == pFile)
			return false;

		ifstreamtype ifs(pFile);
		if(!ifs.is_open())
			return false;

		_SectionMap.clear();

		std::vector<stringtype> filebuf;
		std::vector<stringtype>::const_iterator itor;
		while(!ifs.eof())
		{
			stringtype buf;	
			getline(ifs,buf);
			filebuf.push_back(buf);
		}
		stringtype curSection = "";
		for(itor=filebuf.begin(); itor!=filebuf.end(); ++itor)
		{
			curSection = DoSwitchALine(*itor,curSection);
		}	
		return true;
	}
   const stringtype&  GetStr(const stringtype &SectionStr,const stringtype &KeyStr,const stringtype &DefaultStr="") 
   {
	   SectionMapItor itor;
	   itor = _SectionMap.find(SectionStr);
	   if(_SectionMap.end() == itor) 
		   return DefaultStr;

	   KeyMapItor n;
	   n = itor->second.find(KeyStr);
	   if(itor->second.end() == n)
		   return DefaultStr;

	   return _SectionMap[SectionStr][KeyStr];
   }
   const stringtype& GetStr(const char *pSection,const char *pKey,const char *pDefaultStr = "")
   {
	   return GetStr(stringtype(pSection),stringtype(pKey),stringtype(pDefaultStr));
   }

//    int  GetInt(const char* pSection,const char* pKey,int nDefault = 0)
//    {
// 
//    }
// 
//    bool GetBool(const char* pSection,const char* pKey,int bDefault = false)
//    {
// 
//    }
// 
//    bool WriteStr()
//    {
// 
//    }
// 
//    bool WriteInt()
//    {
// 
//    }
// 
//    bool WriteBool()
//    {
// 
//    }

   void PrintIni()
   {
	   for (SectionMapItor itor = _SectionMap.begin();itor!=_SectionMap.end();++itor)
	   {
		   std::cout
			   <<"["
			   <<itor->first
			   <<"]"
			   <<std::endl;

		   KeyMap * pKeyMap = &(itor->second);
		   for (KeyMapItor keyitor = pKeyMap->begin();keyitor!=pKeyMap->end();++keyitor)
		   {
			   std::cout
				   <<keyitor->first
				   <<"="
				   <<keyitor->second
				   <<std::endl;
		   }
	   }
   }

 private:  
   stringtype DoSwitchALine(const stringtype &aLineStr,stringtype &curSection)
   {
	   if(aLineStr.empty()) return curSection;  //空行 
	   if(';' == aLineStr.at(0)) return curSection; //注释行

	   stringtype::size_type n;	
	   if('[' == aLineStr.at(0))//section项 
	   {
		   n = aLineStr.find(']',1);
		   curSection = aLineStr.substr(1,n-1);
		   AddSection(curSection);
		   return curSection;		
	   }

	   //key & vaule     
	   stringtype strKey;
	   stringtype strVaule;
	   n = aLineStr.find('=',0);
	   if(stringtype::npos == n) return curSection; //没有=号 

	   strKey = aLineStr.substr(0,n);
	   if(stringtype::npos == n+1)   //=号后无内容 
		   strVaule = "";  
	   else   
		   strVaule = aLineStr.substr(n+1);
	   AddKey(curSection,strKey,strVaule);
	   return curSection;		
   }
   void AddSection(stringtype &SectionStr)
   {
	   _SectionMap[SectionStr];
   }
   void AddKey(stringtype &SectionStr,stringtype &KeyStr,stringtype &vauleStr)
   {
	   _SectionMap[SectionStr][KeyStr] = vauleStr;
   }
};
_RzStdEnd
#endif
