#pragma once

#ifdef XIAMI_DLL_EXPORT
#define XIAMI_DLL_API __declspec(dllexport)
#else
#define XIAMI_DLL_API __declspec(dllimport)
#pragma comment(lib,"taglib_plugin.lib")
#endif

class ITag
{
public:
	enum TagFileType{
		TAG_MPEG =0,        //.MP3 
		TAG_OGG,            //.OGG
		TAG_OGGFLAC,        //.OGA
		TAG_FLAC,           //.FLAC
		TAG_AIFF,           //.AIF | .AIFF
		TAG_WAV,            //.WAV
		TAG_APE,            //.APE
		TAG_CUE,			//.CUE
		TAG_MPC,            //.MPC
		TAG_WV,             //.WV
		TAG_SPEEX,          //.SPX
		TAG_TRUEAUDIO,      //.TTA

		TAG_MP4,            //.M4A | .M4B | .M4P | .MP4 | .3G2 | .AAC
		TAG_ASF             //.WMA | .ASF
	};
	enum ReadAudioStyle {
		//! Read as little of the file as possible
		Fast = 0,
		//! Read more of the file and make better values guesses
		Average,
		//! Read as much of the file as needed to report accurate values
		Accurate
	};
	enum TagErrCode {
		TAG_NOERROR = 0,
		TAG_NOSUPPORT
	};
	enum MineType {
		UNKONW = 0,
		JPEG,
		PNG
	};
public:
	virtual ~ITag(){};

	virtual TagFileType filetype() const = 0;
	virtual unsigned long beginpos() const = 0;
	virtual unsigned long filesize() const = 0;
	virtual const wchar_t* filename() const = 0;
    virtual const wchar_t* title() const = 0;
    virtual const wchar_t* artist() const = 0;
    virtual const wchar_t* album() const = 0;
    virtual const wchar_t* comment() const = 0;
    virtual const wchar_t* genre() const = 0;
    virtual unsigned long year() const = 0;
    virtual unsigned long track() const = 0;
	virtual unsigned long playSec() const = 0;
	virtual unsigned long bitRate() const = 0;
	virtual unsigned long sampleRate() const = 0;
	virtual unsigned long channels() const = 0;
	virtual unsigned long mineType() const = 0;
    virtual void setTitle(const wchar_t* s) = 0;
    virtual void setArtist(const wchar_t* s) = 0;
    virtual void setAlbum(const wchar_t* s) = 0;
    virtual void setComment(const wchar_t* s) = 0;
    virtual void setGenre(const wchar_t* s) = 0;
	virtual void setYear(unsigned long i) = 0;
	virtual void setTrack(unsigned long i) = 0;
	virtual bool save() = 0;
	virtual bool addImage(wchar_t* s) = 0;
	virtual bool exportImage(const wchar_t* s) = 0;
	virtual bool isEmpty() const = 0;
	virtual unsigned long subTagCnt() const = 0;
	virtual ITag* nextSubTag()  = 0;

protected:
	ITag(){};
private:
	ITag(const ITag &){};
	ITag &operator=(const ITag &){return *this;};
};


#ifdef   __cplusplus
extern "C" {
#endif

	XIAMI_DLL_API ITag*  xiami_load_tag(const wchar_t* pfile,
		                                bool readAutioInfo,
										ITag::ReadAudioStyle style,
		                                int* nerr
										);
	XIAMI_DLL_API ITag*  xiami_load_tag_type(const wchar_t* pfile,
		                                    ITag::TagFileType tagtype,
											bool readAutioInfo,
											ITag::ReadAudioStyle style,
											int* nerr);
	XIAMI_DLL_API void   xiami_close_tag(ITag* ptag);

#ifdef   __cplusplus
}
#endif