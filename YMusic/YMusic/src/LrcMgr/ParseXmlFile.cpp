#include "StdAfx.h"
#include "ParseXmlFile.h"

#include "../Utilty/tinyxml/tinyxml.h"

CParseXmlFile::CParseXmlFile(void)
{

}

CParseXmlFile::~CParseXmlFile(void)
{

}

BOOL CParseXmlFile::Parse(fileinfo_container& filesInfo,CString strXmlFileName)
{
	if (strXmlFileName.IsEmpty())
		return FALSE;
	
	if (mci::file_exist(strXmlFileName))
	{
		em_utility::TiXmlDocument doc;
		std::string ss = mci::unicode_2_ansi(strXmlFileName);
		if (doc.LoadFile(ss))
		{
			em_utility::TiXmlElement* root = doc.FirstChildElement("result");
			if (NULL!=root)
			{
				em_utility::TiXmlElement* pComponet = root->FirstChildElement("lrc");
				while (NULL!=pComponet)
				{
					std::string s_id = pComponet->Attribute((const char*)"id");
					std::string s_artist = pComponet->Attribute((const char*)"artist");
					std::string s_title = pComponet->Attribute((const char*)"title");
					CString sId = mci::utf8_2_CString(s_id.c_str());
					CString sArtist = mci::utf8_2_CString(s_artist.c_str());
					CString sTitle = mci::utf8_2_CString(s_title.c_str());
					stLrcInfo sLrc = {mci::ToLong(sId),sArtist,sTitle};
					filesInfo.push_back(sLrc);

					pComponet = pComponet->NextSiblingElement("lrc");
				}
			}
		}
	}
	return filesInfo.size() > 0;
}

BOOL CParseXmlFile::ParseBaiduLrcList(fileinfo_container& filesInfo,CString strXmlFileName)
{
	if (strXmlFileName.IsEmpty())
		return FALSE;

	if (mci::file_exist(strXmlFileName))
	{
		em_utility::TiXmlDocument doc;
		std::string ss = mci::unicode_2_ansi(strXmlFileName);
		if (doc.LoadFile(ss))
		{
			em_utility::TiXmlElement* root = doc.FirstChildElement("result");
			if (root != NULL)
			{
				em_utility::TiXmlElement* pComponet = root->FirstChildElement("url");
				if (pComponet != NULL)
				{
					em_utility::TiXmlElement *pLrcID = pComponet->FirstChildElement("lrcid");
					if (pLrcID != NULL)
					{
						std::string lrc_id = pLrcID->GetText();

						stLrcInfo lrc_data;
						lrc_data.nLrcID = atoi(lrc_id.c_str());
						lrc_data.strArtist = _T("");
						lrc_data.strTitle = _T("");
						filesInfo.push_back(lrc_data);
						pComponet = pComponet->NextSiblingElement("lrc");
					}						
				}
			}
		}
	}
	return filesInfo.size() > 0;
}


