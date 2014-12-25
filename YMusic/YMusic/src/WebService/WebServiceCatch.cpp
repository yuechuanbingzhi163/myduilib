#include "stdafx.h"
#include <time.h>
#include "WebServiceCatch.h"
#include "../YApp.h"

bool CWebServiceCatch::LoadCache(const std::string& url,CBuffer& buffer)
{
	std::string sFilePath = GetFilePath(url);
	bool bRet = em_utility::mci::file_exist(sFilePath.c_str());
	if (!bRet) return false;
	FILE *fp = fopen(sFilePath.c_str(),"rb");
	if (!fp) return false;
	int version;
	time_t nTime;
//	int nType;

	fread(&version,1,sizeof(int),fp);
	fread(&nTime,1,sizeof(time_t),fp);
	time_t nCurTime =  time(NULL);

	if(nCurTime >= nTime || cacheVer != version)
	{
		fclose(fp);
		return false;
	}

	while(!feof(fp))
	{
		if (buffer.full())
			buffer.resize();
		int n =fread(buffer[buffer.wpos()],1,buffer.nextwriteblocksize(),fp);
		buffer.wpos(buffer.wpos() + n);
	}
	fclose(fp);

	return !buffer.empty();
}

void CWebServiceCatch::SaveCache(const std::string& url,CBuffer& buffer,int nTimeLen)
{
	int nTotalWrite = 0;
	std::string sFilePath = GetFilePath(url);
	assert(!sFilePath.empty());
	assert(!buffer.empty());
	if(sFilePath.empty() || buffer.empty()) return;

	FILE *fp = fopen(sFilePath.c_str(),"wb");
	if(!fp) return;

	time_t nCurTime = time(NULL);
	nCurTime += (time_t)nTimeLen;
	fwrite(&cacheVer,sizeof(int),1,fp);
	fwrite(&nCurTime,sizeof(time_t),1,fp);

	const size_t oldPos = buffer.rpos();
	while(!buffer.empty())
	{
		int  n =fwrite(buffer[buffer.rpos()],1,buffer.nextreadblocksize(),fp);
		buffer.rpos(buffer.rpos() + n);
		nTotalWrite += n;
	}
	buffer.rpos(oldPos,false);
	fclose(fp);
	nTotalWrite += sizeof(int) + sizeof(time_t);
//	theCacheMgr->StatSize(nTotalWrite);
}

std::string CWebServiceCatch::GetFilePath(const std::string& url)
{
	std::string md5_FileName = em_utility::md5_checksum::get_md5((const unsigned char*)url.c_str(),(unsigned int)url.length());

	static std::string ansiFilePathStr;
	if(ansiFilePathStr.empty())
	{
		CString sFilePath = theFilePathMgr->GetWebServiceCatchPath();
		/*em_utility::mci::GetModulePath();
		sFilePath.Replace('\\','/');
		if(sFilePath.GetAt(sFilePath.GetLength()-1) != _T('/'))
			sFilePath += _T("/");
		sFilePath += _T("WebServiceCatch/");
		em_utility::mci::confirm_dir(sFilePath,_T('/'));*/

		ansiFilePathStr = em_utility::mci::unicode_2_ansi(sFilePath);
	}

	assert(!ansiFilePathStr.empty());

	return (ansiFilePathStr + md5_FileName);
}