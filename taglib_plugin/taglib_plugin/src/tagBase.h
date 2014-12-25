#pragma once
#include <string>
#include <algorithm>
#include "itag.h"


#define  TAGLIB_NO_CONFIG
//#define TAGLIB_STATIC

#include "taglib/fileref.h"

#include "taglib/toolkit/tfile.h"
#include "taglib/asf/asffile.h"
#include "taglib/ogg/vorbis/vorbisfile.h"
#include "taglib/mpeg/mpegfile.h"
#include "taglib/flac/flacfile.h"
//#include "taglib/ogg/oggfile.h"
#include "taglib/ogg/flac/oggflacfile.h"
#include "taglib/mpc/mpcfile.h"
#include "taglib/wavpack/wavpackfile.h"
#include "taglib/ogg/speex/speexfile.h"
#include "taglib/trueaudio/trueaudiofile.h"
#include "taglib/riff/aiff/aifffile.h"
#include "taglib/mp4/mp4file.h"
#include "taglib/riff/wav/wavfile.h"
#include "taglib/ape/apefile.h"

#include "taglib/tag.h"
#include "taglib/toolkit/tstring.h"
#include "taglib/mpeg/id3v2/id3v2framefactory.h"

#include "taglib/mpeg/id3v2/id3v2tag.h"
#include "taglib/mpeg/id3v2/frames/attachedpictureframe.h"

#pragma comment(lib,"taglib.lib")
//#pragma comment(lib,"zlib.lib")
#include "SmartPtr.h"
//typedef TagLib::File* spTagFileT;
typedef smart_pointer::SmartPtr<TagLib::File> spTagFileT;

class CTagBase : public ITag
{
public:
	typedef std::wstring WStrT;
	CTagBase();
	CTagBase(ITag::TagFileType tagType,spTagFileT tagFile);
	~CTagBase();

	virtual TagFileType filetype() const;
	virtual unsigned long beginpos() const;
	virtual unsigned long filesize() const;
	virtual const wchar_t* filename() const;
	virtual const wchar_t* title() const;
	virtual const wchar_t* artist() const;
	virtual const wchar_t* album() const;
	virtual const wchar_t* comment() const;
	virtual const wchar_t* genre() const;
	virtual unsigned long year() const;
	virtual unsigned long track() const;
	virtual unsigned long playSec() const;
	virtual unsigned long bitRate() const;
	virtual unsigned long sampleRate() const;
	virtual unsigned long channels() const;
	virtual unsigned long mineType() const;
	virtual void setTitle(const wchar_t* s);
	virtual void setArtist(const wchar_t* s);
	virtual void setAlbum(const wchar_t* s);
	virtual void setComment(const wchar_t* s);
	virtual void setGenre(const wchar_t* s);
	virtual void setYear(unsigned long i);
	virtual void setTrack(unsigned long i);
	virtual bool isEmpty() const;
	virtual bool save();
	virtual bool addImage(wchar_t* s);
	virtual bool exportImage(const wchar_t* s);
	virtual unsigned long subTagCnt() const;
	virtual ITag* nextSubTag();
protected:
	ITag::TagFileType _tagType;
	spTagFileT        _tagFile;

	WStrT             _sFileName;
	WStrT             _sTitle;
	WStrT             _sArtist;
	WStrT             _sAlbum;
	WStrT             _sComment;
	WStrT             _sGenre;
};

class ImageFile : public TagLib::File
{
public:
	ImageFile(const char *file) : TagLib::File(file)
	{

	}

	TagLib::String mimeType() const
	{
		TagLib::FileName fName = name();
		TagLib::String fileName = fName.operator const char *();

		if(fileName.substr(fileName.size() - 4, 4).upper() == ".PNG")
			return "image/png";
		else if(fileName.substr(fileName.size() - 4, 4).upper() == ".JPG" ||
			fileName.substr(fileName.size() - 5, 5).upper() == ".JPEG")
			return "image/jpeg";
		else
			return TagLib::String::null;
	}

	TagLib::ByteVector data()
	{
		return readBlock(length());
	}

	bool isValid() const
	{
		return isOpen() && !mimeType().isNull();
	}

private:
	virtual TagLib::Tag *tag() const { return 0; }
	virtual TagLib::AudioProperties *audioProperties() const { return 0; }
	virtual bool save() { return false; }
};
