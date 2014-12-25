#include "cueTag.h"
#include <cassert>
#include <fstream>

bool CCueTag::loadCurTag(
	const wchar_t* pfile,
	bool readAutioInfo,
	int readStyle)
{
	assert(pfile);

	std::wifstream fs(pfile);
	if(!fs.is_open())
		return NULL;

	const std::wstring artistKey(L"PERFORMER \"");
	const std::wstring titleKey(L"TITLE \"");
	const std::wstring apefileKey(L"FILE \"");
	const wchar_t      endKey = L'\"';

	bool isParseArtits    = false;
	bool isParseTitle     = false;
	bool isParseApeFile   = false;
	while (!fs.eof())
	{
		std::wstring inbuf;
		std::getline(fs,inbuf);
		if(inbuf.empty())
			continue;

		if(!isParseArtits)
		{
			std::wstring::size_type b = inbuf.find(artistKey) + artistKey.size(); 
			if(std::wstring::npos != b)
			{
				std::wstring::size_type e = inbuf.rfind(endKey) - sizeof(endKey)*sizeof(wchar_t);
				if(std::wstring::npos != e && e >= b)
				{
					_sArtist = inbuf.substr(b,e);

					wprintf(L"aritst:%s\n",_sArtist.c_str());
					isParseArtits = true;
					continue;
				}
			}
		}

		if(!isParseTitle)
		{
			std::wstring::size_type b = inbuf.find(titleKey) + titleKey.size(); 
			if(std::wstring::npos != b)
			{
				std::wstring::size_type e = inbuf.rfind(endKey);
				if(std::wstring::npos != e && e >= b)
				{
					_sTitle = inbuf.substr(b,e);
					isParseTitle = true;
					continue;
				}
			}
		}

		if(!isParseApeFile)
		{
			std::wstring::size_type b = inbuf.find(apefileKey) + apefileKey.size(); 
			if(std::wstring::npos != b)
			{
				std::wstring::size_type e = inbuf.rfind(endKey);
				if(std::wstring::npos != e && e >= b)
				{
					std::wstring apeFile;
					apeFile = inbuf.substr(b,e);
					isParseApeFile = true;

					std::wstring filename(pfile);
					std::replace(filename.begin(),filename.end(),L'\\',L'/');
					
					std::wstring::size_type pos = filename.rfind(L'/');
					apeFile = filename.substr(0,pos+1) + apeFile;
					continue;
				}
			}
		}
	}


	return false;
}
