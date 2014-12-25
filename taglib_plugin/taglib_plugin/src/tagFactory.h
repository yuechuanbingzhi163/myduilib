#pragma once

#include <map>
#include <string>
#include "itag.h"

class CTagFactory
{
public:
	CTagFactory();
	virtual ~CTagFactory();
public:
	ITag* create(
		const wchar_t* pfile,
		bool readAutioInfo,
		ITag::ReadAudioStyle style,
		int* nerr);

	ITag* create(
		const wchar_t* pfile,
		ITag::TagFileType tagtype,
		bool readAutioInfo,
		ITag::ReadAudioStyle style,
		int* nerr);

	void close(ITag* tag);
protected:
	ITag* defaultCreate(
		const wchar_t* pfile,
		bool readAutioInfo,
		ITag::ReadAudioStyle style,
		int* nerr);
protected:
	static ITag* MPEG_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* OGG_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* OGGFLAC_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* FLAC_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* AIFF_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* WAV_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* APE_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* MPC_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* WV_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* SPEEX_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* TRUEAUDIO_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* MP4_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* ASF_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);

	static ITag* CUE_Create(
		const wchar_t* pfile,
		bool readAutioInfo,
		int readStyle,
		int* nerr);
protected:
	typedef ITag* (*TagCreateFun)(const wchar_t*,bool,int,int*);
	typedef std::map<std::wstring,ITag::TagFileType> ExtMapT;
	typedef std::map<ITag::TagFileType,TagCreateFun> CreateFunMapT;

	ExtMapT       _extMap;
	CreateFunMapT _createFunMap;
private:
};