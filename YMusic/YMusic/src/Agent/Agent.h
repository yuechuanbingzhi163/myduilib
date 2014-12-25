#pragma once

#ifdef XIAMI_DLL_EXPORT
#define XIAMI_DLL_API __declspec(dllexport)
#else
#define XIAMI_DLL_API __declspec(dllimport)
#pragma comment(lib,"Agent.lib")
#endif



#ifdef   __cplusplus
extern "C" {
#endif

	XIAMI_DLL_API bool  AsynSendPacket(unsigned int nIndex,const void* buf,size_t cnt); 
	XIAMI_DLL_API bool  InitAgentClient(int nport);

#ifdef   __cplusplus
}
#endif