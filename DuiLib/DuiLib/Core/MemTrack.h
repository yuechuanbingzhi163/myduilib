#ifndef _MemTrack_h
#define _MemTrack_h

#pragma once

#include <map>

namespace DuiLib
{
	struct UILIB_API MemInfo
	{
		char sfile[256];
		int  line;
		char sfunction[256];
		size_t bytes;
		size_t del;
		size_t leak;
		MemInfo(const char* file_ = "<UnKnow Source>",int line_ = 0,const char* function_ = "<UnKnow Function>",size_t bytes_ = 0)
		{
			memcpy(sfile,file_,strlen(file_));
			sfile[strlen(file_)] = '\0';
			line = line_;
			memcpy(sfunction,function_,strlen(function_));
			sfunction[strlen(function_)] = '\0';
			bytes = bytes_;
			del = 0;
			leak = 0;
		}
	};

	class UILIB_API MemLooker
	{
	public:   		
		~MemLooker();
		static MemLooker* Instance();


		void Alloc(size_t sz);
	
		void Delloc(size_t sz);

		void AddMemInfo(long lkey,const MemInfo& info);
		void UpdateMemInfo(long lkey,size_t del_);
	
		void MemoryTrack();
	private:
		static unsigned long m_AllocBytes;
		static unsigned long m_FreeBytes;
		static std::map<long,MemInfo> m_AllocMap;
		static const char MEM_FILE[];
	};

}









#endif//_MemTrack_h