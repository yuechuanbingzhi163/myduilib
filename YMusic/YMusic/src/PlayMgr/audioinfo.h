#pragma once
//#include <fstream>
#include "../../YMusic.h"
#include "src/itag.h"


class AudioTagInfo
{
public:
	AudioTagInfo();
	virtual ~AudioTagInfo();
	virtual bool save_pic2file(const std::string& savename);
public:
	virtual void init(const std::string& path);
	virtual std::string title() const;
	virtual std::string artist() const;
	virtual std::string album() const;
	virtual std::string comment() const;
	virtual std::string genre() const;
	virtual uint year() const;
	virtual uint track() const;
	virtual uint length() const;
	virtual uint mineType() const;

	virtual void title(const std::string& s_title);
	virtual void artist(const std::string& s_artist);
	virtual void album(const std::string& s_album);
	virtual void comment(const std::string& s_comment);
	virtual void genre(const std::string& s_genre);
	virtual void year(uint u_year);
	virtual void track(uint u_track);
	virtual void save();
private:
	ITag* _pTag;
};



