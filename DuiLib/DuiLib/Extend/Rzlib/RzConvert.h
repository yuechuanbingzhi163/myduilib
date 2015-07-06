#ifndef __RZCONVERT_H__
#define __RZCONVERT_H__
#include "RzType.h"
#include <string>

#ifdef WIN32
   #include <Windows.h>
#endif

//_RzCFunBegin
_RzStdBegin

class RZ_DLL_API CRzConvert
{
public:
	typedef std::string stringtype;
	typedef std::wstring wstringtype;

	CRzConvert(){};
	~CRzConvert(){};


	static stringtype UTF8ToGB2312(const char* ptext,int nlen)
	{
		stringtype sResult;

		char buf[4];
		memset(buf,0,4);
		char* rst = new char[nlen + (nlen >> 2) + 2];		
		memset(rst,0,nlen + (nlen >> 2) + 2);

		int i =0;
		int j = 0;

		while(i < nlen)
		{
			if(*(ptext + i) >= 0)
			{
				rst[j++] = ptext[i++];
			}
			else                 
			{
				wchar_t Wtemp;

				_UTF8ToUTF16ofChar(&Wtemp,ptext + i);
				_UTF16ToGB2312ofChar(buf,Wtemp);

				unsigned short int tmp = 0;
				tmp = rst[j] = buf[0];
				tmp = rst[j+1] = buf[1];
				tmp = rst[j+2] = buf[2];

				i += 3;    
				j += 2;   
			}
		}
		rst[j]='\0';
		sResult = rst; 
		delete []rst;

		return sResult;
	}

	static wstringtype UTF8ToUTF16(char* ptext,int len)  //?
	{
		stringtype stmp = UTF8ToGB2312(ptext,len);
		return GB2312ToUTF16(stmp.c_str(),stmp.length());
	}
	static stringtype GB2312ToUTF8(const char* ptext,int len)
	{
		stringtype sResult;
		char buf[4];
		memset(buf,0,4);

		int i = 0;
		while(i < len)
		{
			//如果是英文直接复制就可以
			if( ptext[i] >= 0)
			{
				char asciistr[2]={0};
				asciistr[0] = (ptext[i++]);
				sResult.append(asciistr);
			}
			else
			{
				wchar_t pbuffer;
				_Gb2312ToUTF16ofChar(&pbuffer,*(ptext + i));

				_UTF16ToUTF8ofChar(buf,&pbuffer);

				sResult.append(buf);

				i += 2;
			}
		}

		return sResult;
	}
	static wstringtype GB2312ToUTF16(const char* ptext,int len)
	{
		wstringtype sResult;

		stringtype curLocale = setlocale(LC_ALL,NULL);// curLocale = "C";
		setlocale(LC_ALL,"chs");

		size_t size = len + 1;
		wchar_t *p = new wchar_t[size];
		wmemset(p, 0, size);
		mbstowcs(p,ptext,size);
		sResult = p;
		delete []p;

		setlocale(LC_ALL, curLocale.c_str());

		return sResult;
	}
	static stringtype UTF16ToGB2312(const wchar_t* ptext,int len)
	{
 		stringtype sResult;
// 
// 		for (int i=0;i<len;++i)
// 		{
// 			char c;
// 			_UTF16ToGB2312ofChar(&c,ptext[i]);
// 			sResult.append(c);
// 		}
// 
// 		return sResult;

		stringtype curLocale = setlocale(LC_ALL,NULL);// curLocale = "C";
		setlocale(LC_ALL,"chs");

		size_t size = 2*len + 1;
		char* p = new char[size];
		memset(p,0,size);
		wcstombs(p,ptext,size);

		sResult = p;
		delete []p;

		setlocale(LC_ALL,curLocale.c_str());

		return sResult;
	}
	static stringtype UTF16ToUTF8(const wchar_t* ptext,int len)
	{
		stringtype sResult;

#ifdef WIN32
		int nBufSize = ::WideCharToMultiByte(CP_UTF8,0,ptext,-1,NULL,0,0,FALSE);
		char *szBuf = new char[nBufSize];
		::WideCharToMultiByte(CP_UTF8,0,ptext,-1,szBuf,nBufSize,0,FALSE);
		sResult = szBuf;
		delete []szBuf;
		szBuf = NULL;
#else
		for (int i=0;i<len;++i)
		{
			char c;
			_UTF16ToUTF8ofChar(&c,ptext[i]);
			sResult.append(c);
		}
#endif
		return sResult;
	} 



private:
	static void _UTF16ToGB2312ofChar(char* pOut,const wchar_t& uData)
	{
#ifdef WIN32
		::WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(uData),NULL,NULL);
#else
		char* pcurLocale = setlocale(LC_ALL,NULL);// curLocale = "C";
		setlocale(LC_ALL,"chs");
		wcstombs(pOut,&uData,sizeof(uData));
		setlocale(LC_ALL,pcurLocale);
#endif
	}

	static void _Gb2312ToUTF16ofChar(wchar_t* pOut,const char &gbBuffer)
	{
#ifdef WIN32
		::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,&gbBuffer,2,pOut,1);
#else
		char* pcurLocale = setlocale(LC_ALL,NULL);// curLocale = "C";
		setlocale(LC_ALL,"chs");
		mbstowcs(pOut,&gbBuffer,sizeof(gbBuffer));
		setlocale(LC_ALL,pcurLocale);
#endif
	}

	static void _UTF8ToUTF16ofChar(wchar_t* pOut,const char *pText)
	{
		char* uchar = (char *)pOut;

		uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
		uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
	}

	static void _UTF16ToUTF8ofChar(char* pOut,wchar_t* pText)
	{
		// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
		char* pchar = (char *)pText;

		pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
		pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
		pOut[2] = (0x80 | (pchar[0] & 0x3F));
	}

	static int _IsTextUTF8(char* str,ULONGLONG length)
	{
		  int i;   
		  DWORD nBytes=0;                       //UFT8可用1-6个字节编码,ASCII用一个字节   
		  UCHAR chr;   
		  BOOL bAllAscii=TRUE; //如果全部都是ASCII, 说明不是UTF-8  
		  for(i=0;i<length;i++)   
		  {
			  chr= *(str+i);
			  if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx  
				  bAllAscii= FALSE; 
			  if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
			  {   
				  if(chr>=0x80)
				  {   
					  if(chr>=0xFC&&chr<=0xFD) 
						  nBytes=6;  
					  else if(chr>=0xF8)
						  nBytes=5;   
					  else if(chr>=0xF0)   
						  nBytes=4;   
					  else if(chr>=0xE0)
						  nBytes=3;   
					  else if(chr>=0xC0) 
						  nBytes=2;
					  else 
					  {   
						  return FALSE; 
					  }   
					  nBytes--;   
				  }
			  }   
			  else //多字节符的非首字节,应为 10xxxxxx   
			  {   
				  if( (chr&0xC0) != 0x80 )  
				  {
					  return FALSE;
				  }   
				  nBytes--; 
			  }
		  } 
		  if( nBytes > 0 ) //违返规则
		  {  
			  return FALSE; 
		  }
		  if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8 
		  {  
			  return FALSE; 
		  } 
		  return TRUE;
	}  
};
_RzStdEnd
//_RzCFunEnd
#endif 