#include <tchar.h>
#include "tagFactory.h"
#include "tagBase.h"
#include "cueTag.h"
#include <assert.h>

CTagFactory::CTagFactory()
{
//	TagLib::ID3v2::FrameFactory::instance()->setDefaultTextEncoding(TagLib::String::UTF16);

	_createFunMap.insert(std::make_pair(ITag::TAG_MPEG,&CTagFactory::MPEG_Create));
	_createFunMap.insert(std::make_pair(ITag::TAG_OGG,&CTagFactory::OGG_Create));
	_createFunMap.insert(std::make_pair(ITag::TAG_OGGFLAC,&CTagFactory::OGGFLAC_Create));
	_createFunMap.insert(std::make_pair(ITag::TAG_FLAC,&CTagFactory::FLAC_Create));
	_createFunMap.insert(std::make_pair(ITag::TAG_AIFF,&CTagFactory::AIFF_Create));
	_createFunMap.insert(std::make_pair(ITag::TAG_WAV,&CTagFactory::WAV_Create));
	_createFunMap.insert(std::make_pair(ITag::TAG_APE,&CTagFactory::APE_Create));
	_createFunMap.insert(std::make_pair(ITag::TAG_CUE,&CTagFactory::CUE_Create));
	_createFunMap.insert(std::make_pair(ITag::TAG_MPC,&CTagFactory::MPC_Create));
	//_createFunMap.insert(std::make_pair(ITag::TAG_WV,&CTagFactory::WV_Create));
	//_createFunMap.insert(std::make_pair(ITag::TAG_SPEEX,&CTagFactory::SPEEX_Create));
	//_createFunMap.insert(std::make_pair(ITag::TAG_TRUEAUDIO,&CTagFactory::TRUEAUDIO_Create));
	_createFunMap.insert(std::make_pair(ITag::TAG_MP4,&CTagFactory::MP4_Create));
	_createFunMap.insert(std::make_pair(ITag::TAG_ASF,&CTagFactory::ASF_Create));


	_extMap.insert(std::make_pair(_T("MP3"),ITag::TAG_MPEG));
	_extMap.insert(std::make_pair(_T("OGG"),ITag::TAG_OGG));
	_extMap.insert(std::make_pair(_T("OGA"),ITag::TAG_OGGFLAC));
	_extMap.insert(std::make_pair(_T("FLAC"),ITag::TAG_FLAC));
	_extMap.insert(std::make_pair(_T("AIF"),ITag::TAG_AIFF));
	_extMap.insert(std::make_pair(_T("AIFF"),ITag::TAG_AIFF));
	_extMap.insert(std::make_pair(_T("WAV"),ITag::TAG_WAV));
	_extMap.insert(std::make_pair(_T("APE"),ITag::TAG_APE));
	//_extMap.insert(std::make_pair(_T("CUE"),ITag::TAG_CUE));
	_extMap.insert(std::make_pair(_T("MPC"),ITag::TAG_MPC));
	//_extMap.insert(std::make_pair(_T("WV"),ITag::TAG_WV));
	//_extMap.insert(std::make_pair(_T("SPX"),ITag::TAG_SPEEX));
	//_extMap.insert(std::make_pair(_T("TTA"),ITag::TAG_TRUEAUDIO));
	_extMap.insert(std::make_pair(_T("M4A"),ITag::TAG_MP4));
	_extMap.insert(std::make_pair(_T("M4B"),ITag::TAG_MP4));
	_extMap.insert(std::make_pair(_T("M4P"),ITag::TAG_MP4));
	_extMap.insert(std::make_pair(_T("MP4"),ITag::TAG_MP4));
	_extMap.insert(std::make_pair(_T("3G2"),ITag::TAG_MP4));
	_extMap.insert(std::make_pair(_T("WMA"),ITag::TAG_ASF));
	_extMap.insert(std::make_pair(_T("ASF"),ITag::TAG_ASF));
	_extMap.insert(std::make_pair(_T("AAC"),ITag::TAG_APE));
}

CTagFactory::~CTagFactory()
{

}


ITag* CTagFactory::create(
	const wchar_t* pfile,
	bool readAutioInfo,
	ITag::ReadAudioStyle style,
	int* nerr)
{
	assert(pfile);
	if(!pfile)
		return NULL;

	std::wstring s(pfile);
	std::wstring::size_type pos = s.rfind(L".");
	if(std::wstring::npos == pos)
		return NULL;

	std::wstring ext(s.substr(pos+1));
	std::transform(ext.begin(),ext.end(),ext.begin(),toupper);
	if(ext.empty())
		return NULL;

	ExtMapT::iterator it = _extMap.find(ext);
	if(_extMap.end() != it)
	{
		return CTagFactory::create(pfile,it->second,readAutioInfo,style,nerr);
	}

	return defaultCreate(pfile,readAutioInfo,style,nerr);
}

ITag* CTagFactory::create(
	const wchar_t* pfile,
	ITag::TagFileType tagtype,
	bool readAutioInfo,
	ITag::ReadAudioStyle style,
	int* nerr)
{
	assert(pfile);
	if(!pfile)
		return NULL;


	CreateFunMapT::iterator it = _createFunMap.find(tagtype);
	if(_createFunMap.end() != it)
	{
		assert(it->second);
		ITag* pTag = it->second(pfile,readAutioInfo,(int)style,nerr);
		if(pTag)
		{
 			if(readAutioInfo && (0 >= pTag->playSec()))
 			{
 				xiami_close_tag(pTag);
 				return defaultCreate(pfile,readAutioInfo,style,nerr);
 			}
		
			return pTag;
		}
	}
	return defaultCreate(pfile,readAutioInfo,style,nerr);
}

ITag* CTagFactory::defaultCreate(
	const wchar_t* pfile, 
	bool readAutioInfo,
	ITag::ReadAudioStyle style,
	int* nerr)
{
	ITag* pTag = NULL;
	for (CreateFunMapT::reverse_iterator iter = _createFunMap.rbegin(),iend = _createFunMap.rend();iter != iend;++iter)
	{
		assert(iter->second);
		ITag* pTmpTag = iter->second(pfile,readAutioInfo,(int)style,nerr);
		if(pTmpTag)
		{
			if(!pTmpTag->isEmpty() && (pTmpTag->playSec() > 0))
			{
				if(pTag)
					xiami_close_tag(pTag);
				return pTmpTag;
			}

			if((pTmpTag->playSec() > 0))
			{
				if(pTag && (0 >= pTag->playSec()))
				{
					xiami_close_tag(pTag);
					pTag = pTmpTag;
					continue;
				}
			}

			if(!pTag)
				pTag = pTmpTag;
			else
				xiami_close_tag(pTmpTag);
		}
	}
	if(!pTag && nerr)
		*nerr = ITag::TAG_NOSUPPORT;
	return pTag;
}

void CTagFactory::close(ITag* tag)
{
	if(tag)
	{
		delete tag;
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
ITag* CTagFactory::MPEG_Create(
	const wchar_t* pfile,
	bool readAutioInfo, 
	int readStyle,
	int* nerr)
{
// 	pfile = L"C:\\Users\\go886\\Desktop\\testfile\\다른 사람 품에 안겨서 - 副本.mp3";
// 	TagLib::FileName name(pfile);
	spTagFileT  theFile(new TagLib::MPEG::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
		return NULL;

	return (new CTagBase(ITag::TAG_MPEG,theFile));
}

ITag* CTagFactory::OGG_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
	spTagFileT  theFile(new TagLib::Ogg::Vorbis::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
		return NULL;

	return (new CTagBase(ITag::TAG_OGG,theFile));
}

ITag* CTagFactory::OGGFLAC_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
	spTagFileT  theFile(new TagLib::Ogg::FLAC::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
	{
		return CTagFactory::OGG_Create(pfile,readAutioInfo,readStyle,nerr);
	}

	return (new CTagBase(ITag::TAG_OGGFLAC,theFile));
}

ITag* CTagFactory::FLAC_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
	spTagFileT  theFile(new TagLib::FLAC::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
		return NULL;

	return (new CTagBase(ITag::TAG_FLAC,theFile));
}

ITag* CTagFactory::AIFF_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
	spTagFileT  theFile(new TagLib::RIFF::AIFF::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
		return NULL;

	return (new CTagBase(ITag::TAG_AIFF,theFile));
}

ITag* CTagFactory::WAV_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
	spTagFileT  theFile(new TagLib::RIFF::WAV::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
	{
		theFile = spTagFileT(new TagLib::WavPack::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
		if(!theFile || !theFile->isValid())
		{
			return NULL;
		}
	}

	return (new CTagBase(ITag::TAG_WAV,theFile));
}

ITag* CTagFactory::APE_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
	spTagFileT  theFile(new TagLib::APE::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
		return NULL;

	return (new CTagBase(ITag::TAG_APE,theFile));
}


ITag* CTagFactory::MPC_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
	spTagFileT  theFile(new TagLib::MPC::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
		return NULL;

	return (new CTagBase(ITag::TAG_MPC,theFile));
}

ITag* CTagFactory::WV_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
	spTagFileT  theFile(new TagLib::WavPack::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
		return NULL;

	return (new CTagBase(ITag::TAG_WV,theFile));
}

ITag* CTagFactory::SPEEX_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
	spTagFileT  theFile(new TagLib::Ogg::Speex::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
		return NULL;

	return (new CTagBase(ITag::TAG_SPEEX,theFile));
}

ITag* CTagFactory::TRUEAUDIO_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
	spTagFileT  theFile(new TagLib::TrueAudio::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
		return NULL;

	return (new CTagBase(ITag::TAG_TRUEAUDIO,theFile));
}

ITag* CTagFactory::MP4_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
	spTagFileT  theFile(new TagLib::MP4::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
		return NULL;

	return (new CTagBase(ITag::TAG_MP4,theFile));
}

ITag* CTagFactory::ASF_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
	spTagFileT  theFile(new TagLib::ASF::File(pfile,readAutioInfo,(TagLib::AudioProperties::ReadStyle)readStyle));
	if(!theFile || !theFile->isValid())
		return NULL;

	return (new CTagBase(ITag::TAG_ASF,theFile));
}

ITag* CTagFactory::CUE_Create(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle,
	int* nerr)
{
// 	spTagFileT  theFile(new TagLib::ASF::File(pfile, readAutioInfo, (TagLib::AudioProperties::ReadStyle)readStyle));
// 	if (!theFile || !theFile->isValid())
// 		return NULL;
// 
// 	return (new CTagBase(ITag::TAG_CUE, theFile));
	CCueTag* pTag = new CCueTag();
	if (pTag->loadCurTag(pfile, readAutioInfo, readStyle))
		*nerr = ITag::TagErrCode::TAG_NOERROR;
	else
		*nerr = ITag::TagErrCode::TAG_NOSUPPORT;
	
	return pTag;
}