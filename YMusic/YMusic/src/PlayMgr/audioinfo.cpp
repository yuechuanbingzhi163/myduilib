#include "StdAfx.h"
#include "audioinfo.h"
#include <fstream>

using namespace std;

void toWStr(std::wstring& sOutStr, std::string& sInStr)
{
	if (sInStr.empty())
	{
		sOutStr.clear();
		return;
	}

	sOutStr = CA2W(sInStr.c_str(), CP_ACP);
}
void toMStr(std::string& sOutStr, std::wstring& sInStr)
{
	if (sInStr.empty())
	{
		sOutStr.clear();
		return;
	}
	sOutStr = CW2A(sInStr.c_str(), CP_ACP);
}

AudioTagInfo::AudioTagInfo()
{
	_pTag = NULL;
}
AudioTagInfo::~AudioTagInfo()
{
	if (_pTag)
	{
		xiami_close_tag(_pTag);
	}
	_pTag = NULL;
}

bool AudioTagInfo::save_pic2file(const std::string& savename)
{
	if (_pTag)
	{
		std::wstring str;
		toWStr(str, std::string(savename.c_str()));
		return _pTag->exportImage(str.c_str());
	}
	return false;
}

void AudioTagInfo::init(const std::string& path)
{
	std::wstring str;
	toWStr(str, std::string(path.c_str()));
	int err = 0;
	_pTag = xiami_load_tag(str.c_str(), true, ITag::ReadAudioStyle::Average, &err);
	//assert(_pTag);
}
std::string AudioTagInfo::title() const
{
	if (_pTag)
	{
		string s_return;
		toMStr(s_return, wstring(_pTag->title()));
		return s_return;
	}
	return "";
}
std::string AudioTagInfo::artist() const
{
	if (_pTag)
	{
		string s_return;
		toMStr(s_return, wstring(_pTag->artist()));
		return s_return;
	}
	return "";
}
std::string AudioTagInfo::album() const
{
	if (_pTag)
	{
		string s_return;
		toMStr(s_return, wstring(_pTag->album()));
		return s_return;
	}
	return "";
}
std::string AudioTagInfo::comment() const
{
	if (_pTag)
	{
		string s_return;
		toMStr(s_return, wstring(_pTag->comment()));
		return s_return;
	}
	return "";
}
std::string AudioTagInfo::genre() const
{
	if (_pTag)
	{
		string s_return;
		toMStr(s_return, wstring(_pTag->genre()));
		return s_return;
	}
	return "";
}
uint AudioTagInfo::track() const
{
	if (_pTag)
	{
		return _pTag->track();
	}
	return 0;
}
uint AudioTagInfo::year() const
{
	if (_pTag)
	{
		return _pTag->year();
	}
	return 0;
}
uint AudioTagInfo::length() const
{
	if (_pTag)
	{
		return _pTag->filesize();
	}
	return 0;
}

uint AudioTagInfo::mineType() const
{
	if (_pTag)
	{
		return _pTag->mineType();
	}
	return ITag::MineType::UNKONW;
}

void AudioTagInfo::title(const std::string& s_title)
{
	if (_pTag)
	{
		std::wstring str;
		toWStr(str, std::string(s_title.c_str()));
		_pTag->setTitle(str.c_str());
	}
}
void AudioTagInfo::artist(const std::string& s_artist)
{
	if (_pTag)
	{
		std::wstring str;
		toWStr(str, std::string(s_artist.c_str()));
		_pTag->setArtist(str.c_str());
	}
}
void AudioTagInfo::album(const std::string& s_album)
{
	if (_pTag)
	{
		std::wstring str;
		toWStr(str, std::string(s_album.c_str()));
		_pTag->setAlbum(str.c_str());
	}
}
void AudioTagInfo::comment(const std::string& s_comment)
{
	if (_pTag)
	{
		std::wstring str;
		toWStr(str, std::string(s_comment.c_str()));
		_pTag->setComment(str.c_str());
	}
}
void AudioTagInfo::genre(const std::string& s_genre)
{
	if (_pTag)
	{
		std::wstring str;
		toWStr(str, std::string(s_genre.c_str()));
		_pTag->setGenre(str.c_str());
	}
}
void AudioTagInfo::year(uint u_year)
{
	if (_pTag)
	{
		_pTag->setYear(u_year);
	}
}
void AudioTagInfo::track(uint u_track)
{
	if (_pTag)
	{
		_pTag->setTrack(u_track);
	}
}
void AudioTagInfo::save()
{
	if (_pTag)
	{
		_pTag->save();
	}
}
