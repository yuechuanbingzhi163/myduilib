#include "tagBase.h"
#include <tchar.h>
#include <cstdio>
#include <assert.h>

#include <atlstr.h>

void toWStr(CTagBase::WStrT& sOutStr, TagLib::String& sInStr)
{
	if (sInStr.isEmpty())
	{
		sOutStr.clear();
		return;
	}

	if (sInStr.isAscii() || sInStr.isLatin1())
	{
		sOutStr = CA2W(sInStr.toCString());
	}
	else
	{
		sOutStr = CA2W(sInStr.toCString(true), CP_UTF8);
	}

}
CTagBase::CTagBase()
{}
CTagBase::CTagBase(ITag::TagFileType tagType, spTagFileT tagFile)
	:_tagType(tagType),_tagFile(tagFile) ,_sFileName(tagFile->name())
{
	assert(tagFile);
	/*
	if(ITag::TAG_MPEG == tagType)
	{
	TagLib::MPEG::File* p = (TagLib::MPEG::File*)(tagFile.get());
	const TagLib::ID3v2::FrameListMap themap =  p->ID3v2Tag()->frameListMap();

	for (TagLib::ID3v2::FrameListMap::ConstIterator it = themap.begin();it != themap.end();++it)
	{
	const TagLib::ID3v2::FrameList thelist = it->second;
	printf("FrameID:%s,framesize:%d\n",
	it->first.data(),
	it->second.size());

	for (TagLib::ID3v2::FrameList::ConstIterator iter = thelist.begin();iter != thelist.end();++iter )
	{
	const TagLib::ID3v2::Frame* theFrame = (*iter);

	WStrT s;
	toWStr(s,theFrame->toString());

	printf("FrameID:%s,Framesize:%d,data:\n",
	theFrame->frameID(),
	theFrame->size());
	}


	}
	}
	*/
}
CTagBase::~CTagBase()
{

}

ITag::TagFileType CTagBase::filetype() const
{
	return _tagType;
}
unsigned long CTagBase::beginpos() const
{
	return 0;
}
unsigned long CTagBase::filesize() const
{
	if (_tagFile)
		return _tagFile->length();
	return 0;
}
const wchar_t* CTagBase::filename() const
{
	return _sFileName.c_str();
}
const wchar_t* CTagBase::title() const
{
	if (_sTitle.empty() && _tagFile && _tagFile->tag())
	{
		CTagBase* pThis = const_cast<CTagBase*>(this);
		//pThis->_sTitle = _tagFile->tag()->title().toWString();
		toWStr(pThis->_sTitle, _tagFile->tag()->title());
	}
	return _sTitle.c_str();
}
const wchar_t* CTagBase::artist() const
{
	if (_sArtist.empty() && _tagFile && _tagFile->tag())
	{
		CTagBase* pThis = const_cast<CTagBase*>(this);
		toWStr(pThis->_sArtist, _tagFile->tag()->artist());
	}
	return _sArtist.c_str();
}
const wchar_t* CTagBase::album() const
{
	if (_sAlbum.empty() && _tagFile)
	{
		CTagBase* pThis = const_cast<CTagBase*>(this);
		toWStr(pThis->_sAlbum, _tagFile->tag()->album());
	}
	return _sAlbum.c_str();
}
const wchar_t* CTagBase::comment() const
{
	if (_sComment.empty() && _tagFile && _tagFile->tag())
	{
		CTagBase* pThis = const_cast<CTagBase*>(this);
		toWStr(pThis->_sComment, _tagFile->tag()->comment());
	}
	return _sComment.c_str();
}
const wchar_t* CTagBase::genre() const
{
	if (_sGenre.empty() && _tagFile  && _tagFile->tag())
	{
		CTagBase* pThis = const_cast<CTagBase*>(this);
		toWStr(pThis->_sGenre, _tagFile->tag()->genre());
	}
	return _sGenre.c_str();
}
unsigned long CTagBase::year() const
{
	if (_tagFile &&  _tagFile->tag())
		return _tagFile->tag()->year();
	return 0;
}
unsigned long CTagBase::track() const
{
	if (_tagFile  && _tagFile->tag())
		return _tagFile->tag()->track();
	return 0;
}
unsigned long CTagBase::playSec() const
{
	if (_tagFile)
	{
		const TagLib::AudioProperties* audioProperties = _tagFile->audioProperties();
		if (audioProperties)
			return audioProperties->length();
	}
	return 0;
}

unsigned long CTagBase::bitRate() const
{
	if (_tagFile)
	{
		const TagLib::AudioProperties* audioProperties = _tagFile->audioProperties();
		if (audioProperties)
			return audioProperties->bitrate();
	}
	return 0;
}
unsigned long CTagBase::sampleRate() const
{
	if (_tagFile)
	{
		const TagLib::AudioProperties* audioProperties = _tagFile->audioProperties();
		if (audioProperties)
			return audioProperties->sampleRate();
	}
	return 0;
}
unsigned long CTagBase::channels() const
{
	if (_tagFile)
	{
		const TagLib::AudioProperties* audioProperties = _tagFile->audioProperties();
		if (audioProperties)
			return audioProperties->channels();
	}
	return 0;
}

void CTagBase::setTitle(const wchar_t* s)
{
	_sTitle = s;
	if (_tagFile  && _tagFile->tag())
	{
		TagLib::String theTitle(CW2A(s, CP_UTF8), TagLib::String::UTF8);
		_tagFile->tag()->setTitle(theTitle);
	}
}
void CTagBase::setArtist(const wchar_t* s)
{
	_sArtist = s;
	if (_tagFile  && _tagFile->tag())
	{
		TagLib::String theArtist(CW2A(s, CP_UTF8), TagLib::String::UTF8);
		_tagFile->tag()->setArtist(theArtist);
	}
}
void CTagBase::setAlbum(const wchar_t* s)
{
	_sAlbum = s;
	if (_tagFile  && _tagFile->tag())
	{
		TagLib::String theAlbum(CW2A(s, CP_UTF8), TagLib::String::UTF8);
		_tagFile->tag()->setAlbum(theAlbum);
	}
}
void CTagBase::setComment(const wchar_t* s)
{
	_sComment = s;
	if (_tagFile  && _tagFile->tag())
	{
		TagLib::String theComment(CW2A(s, CP_UTF8), TagLib::String::UTF8);
		_tagFile->tag()->setComment(theComment);
	}
}
void CTagBase::setGenre(const wchar_t* s)
{
	_sGenre = s;
	if (_tagFile && _tagFile->tag())
	{
		TagLib::String theGenre(CW2A(s, CP_UTF8), TagLib::String::UTF8);
		_tagFile->tag()->setGenre(theGenre);
	}
}
void CTagBase::setYear(unsigned long i)
{
	if (_tagFile  && _tagFile->tag())
	{
		_tagFile->tag()->setYear(i);
	}
}
void CTagBase::setTrack(unsigned long i)
{
	if (_tagFile  && _tagFile->tag())
	{
		_tagFile->tag()->setTrack(i);
	}
}
bool CTagBase::isEmpty() const
{
	return (!_tagFile)
		|| (!_tagFile->tag())
		|| _tagFile->tag()->isEmpty();
}
bool CTagBase::save()
{
	if (!_tagFile)
		return false;
	return _tagFile->save();
}
bool CTagBase::addImage(wchar_t* s)
{
	if (!_tagFile) return false;
	TagLib::MPEG::File *pAudioFile = (TagLib::MPEG::File *)_tagFile.get();
	CString strFileName = s;
	ImageFile imageFile((CW2A)strFileName.GetBuffer());
	if (!pAudioFile->isValid() || !imageFile.isValid())
		return false;
	TagLib::ID3v2::Tag *tag = pAudioFile->ID3v2Tag(true);
	TagLib::ID3v2::AttachedPictureFrame *frame = new TagLib::ID3v2::AttachedPictureFrame;
	frame->setMimeType(imageFile.mimeType());
	frame->setPicture(imageFile.data());
	tag->addFrame(frame);
	return pAudioFile->save();

}
bool CTagBase::exportImage(const wchar_t* s)
{
	if (!s)	return false;
	if (_tagFile)
	{
		//get picture
		TagLib::MPEG::File *f = (TagLib::MPEG::File *)_tagFile.get();
		if (!f->hasID3v2Tag() || f->ID3v2Tag()->isEmpty())
			return false;
		/*
		TagLib::ID3v2::FrameList::ConstIterator it = f->ID3v2Tag()->frameList().begin();
		for (; it != f->ID3v2Tag()->frameList().end(); it++)
		{
			if ((*it)->frameID().operator == (TagLib::ByteVector("APIC")))
			{
				HANDLE hFile = CreateFile(s, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if (INVALID_HANDLE_VALUE == hFile)
					return false;
				int nWriteBytes = 0;
				size_t size = (*it)->size();
				::WriteFile(hFile, (*it)->render().data(), (*it)->size(), (LPDWORD)&nWriteBytes, NULL);
				::CloseHandle(hFile);
				if (nWriteBytes != size)
					return false;
				return true;
			}
		}*/
		if (f->ID3v2Tag()->frameListMap().size() == 0)
			return false;
		if (f->ID3v2Tag()->frameListMap().find("APIC") == f->ID3v2Tag()->frameListMap().end())
			return false;

		TagLib::ID3v2::FrameList Flist = f->ID3v2Tag()->frameListMap()["APIC"];
		if (Flist.isEmpty())
			return false;
		TagLib::ID3v2::AttachedPictureFrame *p = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(Flist.front());
		size_t size = p->picture().size();
		CString strPicType = p->mimeType().toCString(true);
		int nPos = strPicType.Find('/');
		CString strTemp = strPicType.Right(strPicType.GetLength() - nPos - 1);
		//CString strPicPath   = s;

		//if (strTemp == _T("png"))
		//	strPicPath.Append(_T(".png"));
		//else
		//	strPicPath.Append(_T(".jpg"));

		HANDLE hFile = CreateFile(s, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
			return false;
		int nWriteBytes = 0;
		::WriteFile(hFile, p->picture().data(), size, (LPDWORD)&nWriteBytes, NULL);
		::CloseHandle(hFile);
		if (nWriteBytes != size)
			return false;

		return true;
	}
	return false;
}
unsigned long CTagBase::subTagCnt() const
{
	return 0;
}
ITag* CTagBase::nextSubTag()
{
	return NULL;
}
unsigned long CTagBase::mineType() const
{
	if (!_tagFile)
		return ITag::MineType::UNKONW;
	ImageFile* pFile = new ImageFile(_tagFile->name());
	if (!pFile)
		return ITag::MineType::UNKONW;

	if (pFile->isValid())
	{
		TagLib::String s = pFile->mimeType();

		delete pFile;

		if (s == TagLib::String::null)
		{
			return ITag::MineType::UNKONW;
		}
		else if (s == TagLib::String("image/jpeg"))
		{
			return ITag::MineType::JPEG;
		}
		else if (s == TagLib::String("image/png"))
		{
			return ITag::MineType::PNG;
		}		
	}
	delete pFile;
	return ITag::MineType::UNKONW;
}