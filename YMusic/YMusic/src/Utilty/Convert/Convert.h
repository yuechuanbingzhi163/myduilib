
/***********************************************************
*	FileName：Convert.h
*  Author  ：DFn Li
*  Timer   : 2013/3/2 14:24
*  E-Mail  : libyyu@qq.com
*  Comment : deal with common type conversion
*  Version : 1.0.0.1
***********************************************************/
#ifndef __CONVERT_H
#define __CONVERT_H


#pragma once

// #include <atlbase.h>
// #include <atlstr.h>
// #include <atlconv.h>

#include "ATLComTime.h"
#include <comutil.h>   
#include <comdef.h>  
#include <wchar.h> 
#include <cassert>
#include <sys/stat.h>
#include <locale.h>
#include <windows.h>
#include <direct.h>
#include <shellapi.h>
#include <vector>
using namespace std;

namespace em_utility{
#define  MAX_TIME_BUF 128
	class mci{
	public:
		static BOOL UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase);
		static BOOL UrlDecode(const char* szSrc, char* pBuf, int cbBufLen);

		static CString utf8_2_CString(const char* p_input);

		static std::string unicode_2_utf8(const wchar_t* p_input);

		static std::string unicode_2_ansi(const wchar_t* p_input);

		static std::string utf8_2_ansi(const char* p_input);

		static std::string ansi_2_utf8(const char* p_input);

		static char* CString_2_charArr(LPCTSTR lpValue);

		static	CString ansi_2_CString(const char* p_input);

		static	CString Htmlspecialchars(LPCTSTR pBuf);

		static	BOOL IsValidEmailChar(TCHAR ch);

		static	BOOL IsValidEmail(CString strEmail);

		static	CString GetModulePath();

		static	void  GetModulePath(CString  &Path);

		static	std::string get_module_path();

		static	int file_last_modified(const char* p_file_name);

		static	int file_last_modified(const wchar_t * p_file_name);

		static	bool file_exist(const char * p_file_name);

		static	bool file_exist(const wchar_t* p_file_name);

		static	bool is_dir_exist(const char* p_dir_name);

		static	bool is_dir_exist(const wchar_t* p_dir_name);

		static	bool delete_file(const char* p_file_name);

		static	bool delete_file(const wchar_t* p_file_name);

		static	bool delete_file_undo(const wchar_t* p_file_name, bool b_file_only);

		static	bool delete_dir(const wchar_t* p_dir_name);

		static  bool make_dir(const wchar_t* p_dir_name,LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL );

		static  bool make_dir(const char* p_dir_name,LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL );

		static	bool is_directory(const char* p_file_name);

		static	bool is_directory(const wchar_t* p_file_name);

		static	CString rip_file_name(const CString& s_path_file, bool b_suffix);

		static CString rip_file_name(const TCHAR* s_filename);

		static	CString rip_file_ext(const CString& s_path_file, bool b_lower);

		static	std::vector<std::wstring> get_dir_stack(const std::wstring& s_file_name, bool b_reverse);

		static	bool is_file(const char* p_file_name);

		static	bool move_file(const TCHAR* p_src_file, const TCHAR* p_dest_file);

		static	CString rip_path(const CString& s_file_name);

		/*文件是否只读状态*/
		static	bool is_read_only(const char* p_file_name);

		static	bool is_read_only(const wchar_t* p_file_name);

		/*去掉的文件的只读状态*/
		static	bool remove_read_only(const char* p_file_name);	

		static	bool remove_read_only(const wchar_t* p_file_name);

		static	bool copy_file(const TCHAR* p_source_file, const TCHAR* p_dest_file, bool b_force);

		static	bool create_file(const TCHAR* p_file_name, bool b_force, unsigned int n_len);

		static	unsigned int get_file_size(const char* p_file_name);

		static	unsigned int get_file_size(const wchar_t* p_file_name);

		static	bool confirm_dir(const char* p_dir_name, const char c_splitter);

		static	bool confirm_dir_2(const char* p_file_name, const char c_splitter);

		static	bool confirm_dir(const wchar_t* p_dir_name, const wchar_t c_splitter);

		static	bool confirm_dir_2(const wchar_t* p_file_name, const wchar_t c_splitter);

		static	void format_time_old(int n_gmt_time, const char* p_format, char* p_output);

		static	void format_time_old(int n_gmt_time, const wchar_t* p_format, wchar_t* p_output);

		static	void format_time_new(int n_gmt_time, const char* p_format, char* p_output);

		static	void format_time(int n_gmt_time, const char* p_format, char* p_output);

		static	void format_time(int n_gmt_time, const wchar_t* p_format, wchar_t* p_output);

		static	void init_io();

		static	int local_time(int n_gmt_time=0);

		static	int now_time(int n_local_time=0);

		static	void init_time_zone();

		static	int time_diff(int n_from, int n_to);

		static void format_currenttime(char* ptime,int len);

		static CString format_currenttime();

		static	unsigned int check_char(const unsigned char* p_input, unsigned int n_len, const unsigned char c_find);

		static	unsigned int check_char(const  wchar_t* p_input, unsigned int n_len, const  wchar_t c_find);

		static	__int64 get_disk_space(const TCHAR* p_disk_name);

		static	CString get_max_space_disk();

		static	CString get_have_dir_disk(const TCHAR* p_dir);

		static	std::string binary_2_hex(const unsigned char* p_data, unsigned int n_len);

		static	CString _binary_2_hex(const unsigned char* p_data, unsigned int n_len);

		static	void confirm_splash(CString& s_input, bool b_tail=true);

		static	void confirm_n_splash(CString& s_input, bool b_tail=true);

		static	CString link_dir_splash(const CString& s_base, const CString& s_sub);

		static	CString fs_replace(const CString& s_input, const TCHAR c_new);

		static	void shift_html_item(std::wstring& s_data);

		static	void erase_html_item(std::wstring& s_data);

		//////////////////////////////////////////////////////////
		static	CString ToString(char* cValue);

		static	CString ToString(const char* cValue);

		static	CString ToString(BYTE btValue)  ;

		static	CString ToString(int iValue)  ;

		static	CString ToString(unsigned int iValue)  ;

		static	CString ToString(long lValue)  ;

		static	CString ToString(unsigned long lValue)  ;

		static	CString ToString(__int64 i64Value)  ;

		static	CString ToString(unsigned __int64 i64Value)  ;

		static	CString ToString(float fltValue)  ;

		static	CString ToString(double dblValue)  ;

		// 时间输出格式：2010-05-06 22:07:08   
		static	CString ToString(const COleDateTime& time,bool bdatetime/*=true*/)  ;

		/*
		static	CString ToString(const _variant_t& var)  ;
		*/
		static  CString CheckFilePathValid(LPCTSTR lpszValue);


		/*******************************************/
		static std::string ToCharArr(LPCTSTR lpszValue);

		static	BYTE ToByte(LPCTSTR lpszValue)  ;

		static	int ToInt(LPCTSTR lpszValue)  ;

		static	unsigned int ToUInt(LPCTSTR lpszValue) ; 

		static	long ToLong(LPCTSTR lpszValue)  ;

		static	unsigned long ToULong(LPCTSTR lpszValue)  ;

		static	__int64 ToInt64(LPCTSTR lpszValue)  ;

		static	unsigned __int64 ToUInt64(LPCTSTR lpszValue)  ;

		static	float ToFloat(LPCTSTR lpszValue)  ;

		static	double ToDouble(LPCTSTR lpszValue)  ;

		// 时间格式例子：2010-05-06 22:07:08   
		//               2010 05 06 22 07 08   
		//               2010:05:06 22:07:08   
		//               2010-05-06-22-07-08   
		// 只要是"- :"分割的时间格式都符合   
		static	COleDateTime ToTime(LPCTSTR lpszValue)  ;


		static	_variant_t ToVariant(const COleDateTime& time)  ;

		struct url_encoder
		{
		public:
			static unsigned char conv_2_hex(const unsigned char x)
			{
				return x > 9 ? x + 55: x + 48;
			}
			static std::string url_encoding(const std::string& s_input);
			typedef unsigned char BYTE;

			inline static BYTE toHex(const BYTE &x)
			{
				return x > 9 ? x -10 + 'A': x + '0';
			}

			inline static BYTE fromHex(const BYTE &x)
			{
				return isdigit(x) ? x-'0' : x-'A'+10;
			}

			inline static std::string URLEncode(const std::string &sIn)
			{
				std::string sOut;
				for( size_t ix = 0; ix < sIn.size(); ix++ )
				{      
					BYTE buf[4];
					memset( buf, 0, 4 );
					if( isalnum( (BYTE)sIn[ix] ) )
					{      
						buf[0] = sIn[ix];
					}
					else if ( isspace( (BYTE)sIn[ix] ) ) //貌似把空格编码成%20或者+都可以
					{
					    buf[0] = '+';
					}
					else
					{
						buf[0] = '%';
						buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
						buf[2] = toHex( (BYTE)sIn[ix] % 16);
					}
					sOut += (char *)buf;
				}
				return sOut;
			};

			inline static std::string URLDecode(const std::string &sIn)
			{
				std::string sOut;
				for( size_t ix = 0; ix < sIn.size(); ix++ )
				{
					BYTE ch = 0;
					if(sIn[ix]=='%')
					{
						ch = (fromHex(sIn[ix+1])<<4);
						ch |= fromHex(sIn[ix+2]);
						ix += 2;
					}
					else if(sIn[ix] == '+')
					{
						ch = ' ';
					}
					else
					{
						ch = sIn[ix];
					}
					sOut += (char)ch;
				}
				return sOut;
			}
		};
	};
};

#endif//__CONVERT_H