#pragma once
#include "tagBase.h"

class CCueTag : public CTagBase
{
public:
	CCueTag(){}
	~CCueTag(){}

	bool loadCurTag(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle);
/*

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
	*/
};