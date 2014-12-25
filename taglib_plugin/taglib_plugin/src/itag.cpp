#include "itag.h"
#include "tagFactory.h"


/************************************************************************/
/*                                                                      */
/************************************************************************/
static CTagFactory theTagFoctory;


#ifdef   __cplusplus
extern "C" {
#endif

	XIAMI_DLL_API ITag*  xiami_load_tag(
		const wchar_t* pfile,
		bool readAutioInfo,
		ITag::ReadAudioStyle style,
		int* nerr
		)
	{
		return theTagFoctory.create(pfile,readAutioInfo,style,nerr);
	}
	XIAMI_DLL_API ITag* xiami_load_tag_type(
		const wchar_t* pfile,
		ITag::TagFileType tagtype, 
		bool readAutioInfo,
		ITag::ReadAudioStyle style, 
		int* nerr)
	{
		return theTagFoctory.create(pfile,tagtype,readAutioInfo,style,nerr);
	}
	XIAMI_DLL_API void   xiami_close_tag(ITag* ptag)
	{
		return theTagFoctory.close(ptag);
	}

#ifdef   __cplusplus
}
#endif