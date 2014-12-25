#include "StdAfx.h"
#include "MemTrack.h"

namespace DuiLib
{
	unsigned long MemLooker::m_AllocBytes = 0;
	unsigned long MemLooker::m_FreeBytes = 0;
	std::map<long,MemInfo> MemLooker::m_AllocMap;
	const char MemLooker::MEM_FILE[] = "memorytrack.txt";

	MemLooker* MemLooker::Instance()
	{
		static MemLooker looker;
		return &looker;
	}
	MemLooker::~MemLooker()
	{
		MemLooker::Instance()->MemoryTrack();
	}

	void MemLooker::Alloc(size_t sz)
	{
		MemLooker::Instance()->m_AllocBytes += sz;
	}
	void MemLooker::Delloc(size_t sz)
	{
		MemLooker::Instance()->m_FreeBytes += sz;
	}

	void MemLooker::AddMemInfo(long lkey,const MemInfo& info)
	{
		MemLooker::Instance()->m_AllocMap.insert(std::make_pair(lkey,info));
	}
	void MemLooker::UpdateMemInfo(long lkey,size_t del_)
	{
		if (MemLooker::Instance()->m_AllocMap.find(lkey)!=MemLooker::Instance()->m_AllocMap.end())
		{
			MemLooker::Instance()->m_AllocMap[lkey].del = del_;
			MemLooker::Instance()->m_AllocMap[lkey].leak = MemLooker::Instance()->m_AllocMap[lkey].bytes - del_;
		}	
	}

	void MemLooker::MemoryTrack()
	{
		FILE * fp = fopen(MemLooker::MEM_FILE,"w+");

		if(!fp)
			return ;
		fprintf(fp,"\n");
		fprintf(fp,"-----------------------\n");
		fprintf(fp,"Memory Usage Statistics\n");
		fprintf(fp,"-----------------------\n");
		fprintf(fp,"\n");
		fprintf(fp,"AllocPos %d,AllocByte %d\n", m_AllocMap.size(),m_AllocBytes);
		for (std::map<long,MemInfo>::iterator it=m_AllocMap.begin();it!=m_AllocMap.end();++it)
		{
			fprintf(fp,"File %s,Func %s,Line %d,Alloc %d,Delete %d ,Leaker %d\n", 
				it->second.sfile,it->second.sfunction,it->second.line,it->second.bytes,it->second.del,it->second.leak);
		}
		fprintf(fp,"AllocPos %d,DellocByte %d\n", m_AllocMap.size(),m_FreeBytes);
		fprintf(fp,"MemoryTrack Leaker %d Byte\n",m_AllocBytes - m_FreeBytes);
		fflush(fp);
		fclose(fp);
	}
}