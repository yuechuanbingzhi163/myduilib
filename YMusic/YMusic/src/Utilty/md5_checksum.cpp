/*****************************************************************************************

***		MD5Checksum.cpp: implementation of the MD5Checksum class.

***		Developed by Langfine Ltd. 
***		Released to the public domain 12/Nov/2001.
***		Please visit our website www.langfine.com

***		Any modifications must be clearly commented to distinguish them from Langfine's 
***		original source code. Please advise Langfine of useful modifications so that we 
***		can make them generally available. 

*****************************************************************************************/


/****************************************************************************************
This software is derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm. 
Incorporation of this statement is a condition of use; please see the RSA
Data Security Inc copyright notice below:-

Copyright (C) 1990-2, RSA Data Security, Inc. Created 1990. All
rights reserved.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.
License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.
License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.
RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
*****************************************************************************************/

/****************************************************************************************
This implementation of the RSA MD5 Algorithm was written by Langfine Ltd 
(www.langfine.com).

Langfine Ltd makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

In addition to the above, Langfine make no warrant or assurances regarding the 
accuracy of this implementation of the MD5 checksum algorithm nor any assurances regarding
its suitability for any purposes.

This implementation may be used freely provided that Langfine is credited
in a copyright or similar notices (eg, RSA MD5 Algorithm implemented by Langfine
Ltd.) and provided that the RSA Data Security notices are complied with.
*/
#include "stdafx.h"

#include "md5_checksum.h"
#include <cassert>
#include <windows.h>

using namespace std;

namespace em_utility
{

//Magic initialization constants
#define MD5_INIT_STATE_0 0x67452301
#define MD5_INIT_STATE_1 0xefcdab89
#define MD5_INIT_STATE_2 0x98badcfe
#define MD5_INIT_STATE_3 0x10325476

//Constants for Transform routine.
#define MD5_S11  7
#define MD5_S12 12
#define MD5_S13 17
#define MD5_S14 22
#define MD5_S21  5
#define MD5_S22  9
#define MD5_S23 14
#define MD5_S24 20
#define MD5_S31  4
#define MD5_S32 11
#define MD5_S33 16
#define MD5_S34 23
#define MD5_S41  6
#define MD5_S42 10
#define MD5_S43 15
#define MD5_S44 21

//Transformation Constants - Round 1
#define MD5_T01  0xd76aa478 //Transformation Constant 1 
#define MD5_T02  0xe8c7b756 //Transformation Constant 2
#define MD5_T03  0x242070db //Transformation Constant 3
#define MD5_T04  0xc1bdceee //Transformation Constant 4
#define MD5_T05  0xf57c0faf //Transformation Constant 5
#define MD5_T06  0x4787c62a //Transformation Constant 6
#define MD5_T07  0xa8304613 //Transformation Constant 7
#define MD5_T08  0xfd469501 //Transformation Constant 8
#define MD5_T09  0x698098d8 //Transformation Constant 9
#define MD5_T10  0x8b44f7af //Transformation Constant 10
#define MD5_T11  0xffff5bb1 //Transformation Constant 11
#define MD5_T12  0x895cd7be //Transformation Constant 12
#define MD5_T13  0x6b901122 //Transformation Constant 13
#define MD5_T14  0xfd987193 //Transformation Constant 14
#define MD5_T15  0xa679438e //Transformation Constant 15
#define MD5_T16  0x49b40821 //Transformation Constant 16

//Transformation Constants - Round 2
#define MD5_T17  0xf61e2562 //Transformation Constant 17
#define MD5_T18  0xc040b340 //Transformation Constant 18
#define MD5_T19  0x265e5a51 //Transformation Constant 19
#define MD5_T20  0xe9b6c7aa //Transformation Constant 20
#define MD5_T21  0xd62f105d //Transformation Constant 21
#define MD5_T22  0x02441453 //Transformation Constant 22
#define MD5_T23  0xd8a1e681 //Transformation Constant 23
#define MD5_T24  0xe7d3fbc8 //Transformation Constant 24
#define MD5_T25  0x21e1cde6 //Transformation Constant 25
#define MD5_T26  0xc33707d6 //Transformation Constant 26
#define MD5_T27  0xf4d50d87 //Transformation Constant 27
#define MD5_T28  0x455a14ed //Transformation Constant 28
#define MD5_T29  0xa9e3e905 //Transformation Constant 29
#define MD5_T30  0xfcefa3f8 //Transformation Constant 30
#define MD5_T31  0x676f02d9 //Transformation Constant 31
#define MD5_T32  0x8d2a4c8a //Transformation Constant 32

//Transformation Constants - Round 3
#define MD5_T33  0xfffa3942 //Transformation Constant 33
#define MD5_T34  0x8771f681 //Transformation Constant 34
#define MD5_T35  0x6d9d6122 //Transformation Constant 35
#define MD5_T36  0xfde5380c //Transformation Constant 36
#define MD5_T37  0xa4beea44 //Transformation Constant 37
#define MD5_T38  0x4bdecfa9 //Transformation Constant 38
#define MD5_T39  0xf6bb4b60 //Transformation Constant 39
#define MD5_T40  0xbebfbc70 //Transformation Constant 40
#define MD5_T41  0x289b7ec6 //Transformation Constant 41
#define MD5_T42  0xeaa127fa //Transformation Constant 42
#define MD5_T43  0xd4ef3085 //Transformation Constant 43
#define MD5_T44  0x04881d05 //Transformation Constant 44
#define MD5_T45  0xd9d4d039 //Transformation Constant 45
#define MD5_T46  0xe6db99e5 //Transformation Constant 46
#define MD5_T47  0x1fa27cf8 //Transformation Constant 47
#define MD5_T48  0xc4ac5665 //Transformation Constant 48

//Transformation Constants - Round 4
#define MD5_T49  0xf4292244 //Transformation Constant 49
#define MD5_T50  0x432aff97 //Transformation Constant 50
#define MD5_T51  0xab9423a7 //Transformation Constant 51
#define MD5_T52  0xfc93a039 //Transformation Constant 52
#define MD5_T53  0x655b59c3 //Transformation Constant 53
#define MD5_T54  0x8f0ccc92 //Transformation Constant 54
#define MD5_T55  0xffeff47d //Transformation Constant 55
#define MD5_T56  0x85845dd1 //Transformation Constant 56
#define MD5_T57  0x6fa87e4f //Transformation Constant 57
#define MD5_T58  0xfe2ce6e0 //Transformation Constant 58
#define MD5_T59  0xa3014314 //Transformation Constant 59
#define MD5_T60  0x4e0811a1 //Transformation Constant 60
#define MD5_T61  0xf7537e82 //Transformation Constant 61
#define MD5_T62  0xbd3af235 //Transformation Constant 62
#define MD5_T63  0x2ad7d2bb //Transformation Constant 63
#define MD5_T64  0xeb86d391 //Transformation Constant 64


//Null data (except for first BYTE) used to finalise the checksum calculation
static unsigned char PADDING[64] = 
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

md5_hash_value::md5_hash_value()
{
	n_value1 = n_value2 = n_value3 = n_value4 = 0;
}

md5_hash_value::md5_hash_value(const unsigned char* p_buf)
{
	assert(p_buf != NULL);
	const unsigned int* p_value = (const unsigned int*)p_buf;
	n_value1 = *p_value++;
	n_value2 = *p_value++;
	n_value3 = *p_value++;
	n_value4 = *p_value;
}

std::string md5_hash_value::int32_2_str(unsigned int n_value)
{
	std::string s_result;
	const unsigned char* p_buf = NULL;
	p_buf = (const unsigned char*)&n_value;
	for (unsigned int i = 0; i < 4; ++i)
	{
		std::string s_value;
		if (p_buf[i] == 0)
		{
			s_value = string("00");
		}
		else if (p_buf[i] <= 15)
		{
			char s_buf[8];
			memset(s_buf, 0x0, 8);
			sprintf(s_buf, "0%x", p_buf[i]);
			s_value = s_buf;
		}
		else 
		{
			char s_buf[8];
			memset(s_buf, 0x0, 8);
			sprintf(s_buf, "%x", p_buf[i]);
			s_value = s_buf;
		}
		assert(s_value.length() == 2);
		s_result += s_value;
	}
	return s_result;
}

std::string md5_hash_value::to_string() const
{
	std::string s_value;
	s_value += int32_2_str(n_value1);
	s_value += int32_2_str(n_value2);
	s_value += int32_2_str(n_value3);
	s_value += int32_2_str(n_value4);
	assert(s_value.length() == 32);
	return s_value;
}

/*****************************************************************************************
FUNCTION:		CMD5Checksum::GetMD5
DETAILS:		static, public
DESCRIPTION:	Gets the MD5 checksum for a specified file
RETURNS:		CString : the hexadecimal MD5 checksum for the specified file
ARGUMENTS:		CString& strFilePath : the full pathname of the specified file
NOTES:			Provides an interface to the CMD5Checksum class. 'strFilePath' name should 
hold the full pathname of the file, eg C:\My Documents\Arcticle.txt.
NB. If any problems occur with opening or reading this file, a CFileException
will be thrown; callers of this function should be ready to catch this 
exception.
*****************************************************************************************/
string md5_checksum::get_md5(const TCHAR* p_file_path, unsigned int n_begin, unsigned int n_len)
{
	string s_hash;
	HANDLE h_file = ::CreateFile(p_file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h_file == INVALID_HANDLE_VALUE)
		return "";
	int n_total_bytes = ::GetFileSize(h_file, NULL);
	if (n_total_bytes == 0 ||  n_begin >= n_total_bytes || (n_len > (n_total_bytes - n_begin)))
	{
		::CloseHandle(h_file);
		return "";
	}
	md5_checksum md5_check;
	static const unsigned int BUF_SIZE = 1024;
	unsigned char s_buf[BUF_SIZE];
	DWORD n_result = ::SetFilePointer(h_file, n_begin, NULL, FILE_BEGIN);
	if (n_result !=  INVALID_SET_FILE_POINTER)
	{
		unsigned int n_remains = n_len;
		unsigned int n_needs = 0;
		unsigned int n_reads = 0;
		BOOL b_result = FALSE;
		while (n_remains > 0)
		{
			n_needs = min(BUF_SIZE, n_remains); 
			b_result = ::ReadFile(h_file, s_buf, n_needs, (unsigned long*)&n_reads, NULL);
			assert(b_result && n_needs == n_reads);
			md5_check.Update(s_buf, n_reads);
			n_remains -= n_reads;
		}
		s_hash = md5_check.Final();
	}
	::CloseHandle(h_file);
	return s_hash;
}

string md5_checksum::get_md5(const TCHAR* p_file_path)
{
	HANDLE h_file = ::CreateFile(p_file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h_file == INVALID_HANDLE_VALUE)
		return "";
	int n_total_bytes = ::GetFileSize(h_file, NULL);
	if (n_total_bytes == 0)
	{
		::CloseHandle(h_file);
		return "";
	}
	md5_checksum md5_check;
	static const unsigned int BUF_SIZE = 1024;
	unsigned char s_buf[BUF_SIZE];
	BOOL b_result = FALSE;
	unsigned int n_need_bytes = 0;
	unsigned int n_read_bytes = 0;
	while (n_total_bytes > 0)
	{
		n_need_bytes = min(BUF_SIZE, n_total_bytes);
		b_result = ::ReadFile(h_file, s_buf, n_need_bytes, (unsigned long*)&n_read_bytes, NULL);
		assert(b_result && n_need_bytes == n_read_bytes);
		md5_check.Update(s_buf, n_read_bytes);
		n_total_bytes -= n_read_bytes;
	}
	::CloseHandle(h_file);
	return md5_check.Final();
}

bool md5_checksum::add_data(const unsigned char* p_buf, unsigned int n_length)
{
	assert(p_buf != NULL && n_length > 0);
	Update(p_buf, n_length);
	return true;
}

string md5_checksum::finish()
{
	return Final();
}

bool md5_checksum::same_md5(const unsigned char* p_src, const unsigned char* p_dest)
{
	bool b_result = false;
	if (p_src != NULL && p_dest != NULL)
	{
		int n_result = memcmp(p_src, p_dest, MD5_BUF_SIZE);
		b_result = n_result == 0;
	}
	return b_result;
}

void md5_checksum::clear()
{
	// zero members
	memset(ms_buffer, 0x0, 64 );
	mn_count[0] = mn_count[1] = 0;

	// Load magic state initialization constants
	mn_MD5[0] = MD5_INIT_STATE_0;
	mn_MD5[1] = MD5_INIT_STATE_1;
	mn_MD5[2] = MD5_INIT_STATE_2;
	mn_MD5[3] = MD5_INIT_STATE_3;
}
/*****************************************************************************************
FUNCTION:		CMD5Checksum::GetMD5
DETAILS:		static, public
DESCRIPTION:	Gets the MD5 checksum for data in a BYTE array
RETURNS:		CString : the hexadecimal MD5 checksum for the specified data
ARGUMENTS:		BYTE* pBuf  :	pointer to the BYTE array
UINT nLength :	number of BYTEs of data to be checksumed
NOTES:			Provides an interface to the CMD5Checksum class. Any data that can
be cast to a BYTE array of known length can be checksummed by this
function. Typically, CString and char arrays will be checksumed, 
although this function can be used to check the integrity of any BYTE array. 
A buffer of zero length can be checksummed; all buffers of zero length 
will return the same checksum. 
*****************************************************************************************/
string md5_checksum::get_md5(const unsigned char* p_buf, unsigned int n_length)
{
	md5_checksum md5_check;
	md5_check.Update(p_buf, n_length);
	return md5_check.Final();
}

md5_hash_value md5_checksum::get_hash_value(const unsigned char* p_buf, unsigned int n_len)
{
	if (p_buf != NULL && n_len > 0)
	{
		unsigned char s_md5_value[MD5_BUF_SIZE];
		md5_checksum md5;
		md5.Update(p_buf, n_len);
		md5.Final_core(s_md5_value, md5_checksum::MD5_BUF_SIZE);
		return md5_hash_value(s_md5_value);
	}
	else
		return md5_hash_value();
}

bool md5_checksum::get_md5(const unsigned char* p_buf, unsigned int n_len, unsigned char* p_md5)
{
	bool b_result = false;
	if (p_buf != NULL && n_len > 0 && p_md5 != NULL)
	{
		md5_checksum md5;
		md5.Update(p_buf, n_len);
		md5.Final_core(p_md5, md5_checksum::MD5_BUF_SIZE);
		b_result = true;
	}
	return b_result;
}

/*****************************************************************************************
FUNCTION:		CMD5Checksum::RotateLeft
DETAILS:		private
DESCRIPTION:	Rotates the bits in a 32 bit DWORD left by a specified amount
RETURNS:		The rotated DWORD 
ARGUMENTS:		DWORD x : the value to be rotated
int n   : the number of bits to rotate by
*****************************************************************************************/
unsigned int md5_checksum::RotateLeft(unsigned int x, int n)
{
	//check that DWORD is 4 bytes long - true in Visual C++ 6 and 32 bit Windows
	assert(sizeof(x) == 4 );
	//rotate and return x
	return (x << n) | (x >> (32 - n));
}

/*****************************************************************************************
FUNCTION:		CMD5Checksum::FF
DETAILS:		protected
DESCRIPTION:	Implementation of basic MD5 transformation algorithm
RETURNS:		none
ARGUMENTS:		DWORD &A, B, C, D : Current (partial) checksum
DWORD X           : Input data
DWORD S			  : MD5_SXX Transformation constant
DWORD T			  :	MD5_TXX Transformation constant
NOTES:			None
*****************************************************************************************/
void md5_checksum::FF(unsigned int& A, unsigned int B, unsigned int C, unsigned int D, unsigned int X, unsigned int S, unsigned int T)
{
	unsigned int F = (B & C) | (~B & D);
	A += F + X + T;
	A = RotateLeft(A, S);
	A += B;
}

/*****************************************************************************************
FUNCTION:		CMD5Checksum::GG
DETAILS:		protected
DESCRIPTION:	Implementation of basic MD5 transformation algorithm
RETURNS:		none
ARGUMENTS:		DWORD &A, B, C, D : Current (partial) checksum
DWORD X           : Input data
DWORD S			  : MD5_SXX Transformation constant
DWORD T			  :	MD5_TXX Transformation constant
NOTES:			None
*****************************************************************************************/
void md5_checksum::GG(unsigned int& A, unsigned int B, unsigned int C, unsigned int D, unsigned int X, unsigned int S, unsigned int T)
{
	unsigned int G = (B & D) | (C & ~D);
	A += G + X + T;
	A = RotateLeft(A, S);
	A += B;
}

/*****************************************************************************************
FUNCTION:		CMD5Checksum::HH
DETAILS:		protected
DESCRIPTION:	Implementation of basic MD5 transformation algorithm
RETURNS:		none
ARGUMENTS:		DWORD &A, B, C, D : Current (partial) checksum
DWORD X           : Input data
DWORD S			  : MD5_SXX Transformation constant
DWORD T			  :	MD5_TXX Transformation constant
NOTES:			None
*****************************************************************************************/
void md5_checksum::HH(unsigned int& A, unsigned int B, unsigned int C, unsigned int D, unsigned int X, unsigned int S, unsigned int T)
{
	unsigned int H = (B ^ C ^ D);
	A += H + X + T;
	A = RotateLeft(A, S);
	A += B;
}

/*****************************************************************************************
FUNCTION:		CMD5Checksum::II
DETAILS:		protected
DESCRIPTION:	Implementation of basic MD5 transformation algorithm
RETURNS:		none
ARGUMENTS:		DWORD &A, B, C, D : Current (partial) checksum
DWORD X           : Input data
DWORD S			  : MD5_SXX Transformation constant
DWORD T			  :	MD5_TXX Transformation constant
NOTES:			None
*****************************************************************************************/
void md5_checksum::II(unsigned int& A, unsigned int B, unsigned int C, unsigned int D, unsigned int X, unsigned int S, unsigned int T)
{
	unsigned int I = (C ^ (B | ~D));
	A += I + X + T;
	A = RotateLeft(A, S);
	A += B;
}


/*****************************************************************************************
FUNCTION:		CMD5Checksum::ByteToDWord
DETAILS:		private
DESCRIPTION:	Transfers the data in an 8 bit array to a 32 bit array
RETURNS:		void
ARGUMENTS:		DWORD* Output : the 32 bit (unsigned long) destination array 
BYTE* Input	  : the 8 bit (unsigned char) source array
UINT nLength  : the number of 8 bit data items in the source array
NOTES:			Four BYTES from the input array are transferred to each DWORD entry
of the output array. The first BYTE is transferred to the bits (0-7) 
of the output DWORD, the second BYTE to bits 8-15 etc. 
The algorithm assumes that the input array is a multiple of 4 bytes long
so that there is a perfect fit into the array of 32 bit words.
*****************************************************************************************/
void md5_checksum::uchar_2_uint(unsigned int* p_output, const unsigned char* p_input, unsigned int n_length)
{
	//entry invariants
	assert(n_length % 4 == 0 );

	//initialisations
	unsigned int i=0;	//index to Output array
	unsigned int j=0;	//index to Input array

	//transfer the data by shifting and copying
	for ( ; j < n_length; i++, j += 4)
	{
		p_output[i] = (unsigned int)p_input[j]			| 
			(unsigned int)p_input[j+1] << 8	| 
			(unsigned int)p_input[j+2] << 16 | 
			(unsigned int)p_input[j+3] << 24;
	}
}

/*****************************************************************************************
FUNCTION:		CMD5Checksum::Transform
DETAILS:		protected
DESCRIPTION:	MD5 basic transformation algorithm;  transforms 'm_lMD5'
RETURNS:		void
ARGUMENTS:		BYTE Block[64]
NOTES:			An MD5 checksum is calculated by four rounds of 'Transformation'.
The MD5 checksum currently held in m_lMD5 is merged by the 
transformation process with data passed in 'Block'.  
*****************************************************************************************/
void md5_checksum::Transform(const unsigned char Block[64])
{
	//initialise local data with current checksum
	unsigned int a = mn_MD5[0];
	unsigned int b = mn_MD5[1];
	unsigned int c = mn_MD5[2];
	unsigned int d = mn_MD5[3];

	//copy BYTES from input 'Block' to an array of ULONGS 'X'
	unsigned int X[16];
	uchar_2_uint(X, Block, 64);

	//Perform Round 1 of the transformation
	FF (a, b, c, d, X[ 0], MD5_S11, MD5_T01); 
	FF (d, a, b, c, X[ 1], MD5_S12, MD5_T02); 
	FF (c, d, a, b, X[ 2], MD5_S13, MD5_T03); 
	FF (b, c, d, a, X[ 3], MD5_S14, MD5_T04); 
	FF (a, b, c, d, X[ 4], MD5_S11, MD5_T05); 
	FF (d, a, b, c, X[ 5], MD5_S12, MD5_T06); 
	FF (c, d, a, b, X[ 6], MD5_S13, MD5_T07); 
	FF (b, c, d, a, X[ 7], MD5_S14, MD5_T08); 
	FF (a, b, c, d, X[ 8], MD5_S11, MD5_T09); 
	FF (d, a, b, c, X[ 9], MD5_S12, MD5_T10); 
	FF (c, d, a, b, X[10], MD5_S13, MD5_T11); 
	FF (b, c, d, a, X[11], MD5_S14, MD5_T12); 
	FF (a, b, c, d, X[12], MD5_S11, MD5_T13); 
	FF (d, a, b, c, X[13], MD5_S12, MD5_T14); 
	FF (c, d, a, b, X[14], MD5_S13, MD5_T15); 
	FF (b, c, d, a, X[15], MD5_S14, MD5_T16); 

	//Perform Round 2 of the transformation
	GG (a, b, c, d, X[ 1], MD5_S21, MD5_T17); 
	GG (d, a, b, c, X[ 6], MD5_S22, MD5_T18); 
	GG (c, d, a, b, X[11], MD5_S23, MD5_T19); 
	GG (b, c, d, a, X[ 0], MD5_S24, MD5_T20); 
	GG (a, b, c, d, X[ 5], MD5_S21, MD5_T21); 
	GG (d, a, b, c, X[10], MD5_S22, MD5_T22); 
	GG (c, d, a, b, X[15], MD5_S23, MD5_T23); 
	GG (b, c, d, a, X[ 4], MD5_S24, MD5_T24); 
	GG (a, b, c, d, X[ 9], MD5_S21, MD5_T25); 
	GG (d, a, b, c, X[14], MD5_S22, MD5_T26); 
	GG (c, d, a, b, X[ 3], MD5_S23, MD5_T27); 
	GG (b, c, d, a, X[ 8], MD5_S24, MD5_T28); 
	GG (a, b, c, d, X[13], MD5_S21, MD5_T29); 
	GG (d, a, b, c, X[ 2], MD5_S22, MD5_T30); 
	GG (c, d, a, b, X[ 7], MD5_S23, MD5_T31); 
	GG (b, c, d, a, X[12], MD5_S24, MD5_T32); 

	//Perform Round 3 of the transformation
	HH (a, b, c, d, X[ 5], MD5_S31, MD5_T33); 
	HH (d, a, b, c, X[ 8], MD5_S32, MD5_T34); 
	HH (c, d, a, b, X[11], MD5_S33, MD5_T35); 
	HH (b, c, d, a, X[14], MD5_S34, MD5_T36); 
	HH (a, b, c, d, X[ 1], MD5_S31, MD5_T37); 
	HH (d, a, b, c, X[ 4], MD5_S32, MD5_T38); 
	HH (c, d, a, b, X[ 7], MD5_S33, MD5_T39); 
	HH (b, c, d, a, X[10], MD5_S34, MD5_T40); 
	HH (a, b, c, d, X[13], MD5_S31, MD5_T41); 
	HH (d, a, b, c, X[ 0], MD5_S32, MD5_T42); 
	HH (c, d, a, b, X[ 3], MD5_S33, MD5_T43); 
	HH (b, c, d, a, X[ 6], MD5_S34, MD5_T44); 
	HH (a, b, c, d, X[ 9], MD5_S31, MD5_T45); 
	HH (d, a, b, c, X[12], MD5_S32, MD5_T46); 
	HH (c, d, a, b, X[15], MD5_S33, MD5_T47); 
	HH (b, c, d, a, X[ 2], MD5_S34, MD5_T48); 

	//Perform Round 4 of the transformation
	II (a, b, c, d, X[ 0], MD5_S41, MD5_T49); 
	II (d, a, b, c, X[ 7], MD5_S42, MD5_T50); 
	II (c, d, a, b, X[14], MD5_S43, MD5_T51); 
	II (b, c, d, a, X[ 5], MD5_S44, MD5_T52); 
	II (a, b, c, d, X[12], MD5_S41, MD5_T53); 
	II (d, a, b, c, X[ 3], MD5_S42, MD5_T54); 
	II (c, d, a, b, X[10], MD5_S43, MD5_T55); 
	II (b, c, d, a, X[ 1], MD5_S44, MD5_T56); 
	II (a, b, c, d, X[ 8], MD5_S41, MD5_T57); 
	II (d, a, b, c, X[15], MD5_S42, MD5_T58); 
	II (c, d, a, b, X[ 6], MD5_S43, MD5_T59); 
	II (b, c, d, a, X[13], MD5_S44, MD5_T60); 
	II (a, b, c, d, X[ 4], MD5_S41, MD5_T61); 
	II (d, a, b, c, X[11], MD5_S42, MD5_T62); 
	II (c, d, a, b, X[ 2], MD5_S43, MD5_T63); 
	II (b, c, d, a, X[ 9], MD5_S44, MD5_T64); 

	//add the transformed values to the current checksum
	mn_MD5[0] += a;
	mn_MD5[1] += b;
	mn_MD5[2] += c;
	mn_MD5[3] += d;
}


/*****************************************************************************************
CONSTRUCTOR:	CMD5Checksum
DESCRIPTION:	Initialises member data
ARGUMENTS:		None
NOTES:			None
*****************************************************************************************/
md5_checksum::md5_checksum()
{
	// zero members
	memset(ms_buffer, 0x0, 64 );
	mn_count[0] = mn_count[1] = 0;

	// Load magic state initialization constants
	mn_MD5[0] = MD5_INIT_STATE_0;
	mn_MD5[1] = MD5_INIT_STATE_1;
	mn_MD5[2] = MD5_INIT_STATE_2;
	mn_MD5[3] = MD5_INIT_STATE_3;
}

/*****************************************************************************************
FUNCTION:		CMD5Checksum::DWordToByte
DETAILS:		private
DESCRIPTION:	Transfers the data in an 32 bit array to a 8 bit array
RETURNS:		void
ARGUMENTS:		BYTE* Output  : the 8 bit destination array 
DWORD* Input  : the 32 bit source array
UINT nLength  : the number of 8 bit data items in the source array
NOTES:			One DWORD from the input array is transferred into four BYTES 
in the output array. The first (0-7) bits of the first DWORD are 
transferred to the first output BYTE, bits bits 8-15 are transferred from
the second BYTE etc. 

The algorithm assumes that the output array is a multiple of 4 bytes long
so that there is a perfect fit of 8 bit BYTES into the 32 bit DWORDs.
*****************************************************************************************/
void md5_checksum::uint_2_uchar(unsigned char* p_output, unsigned int* p_input, unsigned int n_length)
{
	//entry invariants
	assert(n_length % 4 == 0);

	//transfer the data by shifting and copying
	unsigned int i = 0;
	unsigned int j = 0;
	for ( ; j < n_length; i++, j += 4) 
	{
		p_output[j] =   (unsigned char)(p_input[i] & 0xff);
		p_output[j+1] = (unsigned char)((p_input[i] >> 8) & 0xff);
		p_output[j+2] = (unsigned char)((p_input[i] >> 16) & 0xff);
		p_output[j+3] = (unsigned char)((p_input[i] >> 24) & 0xff);
	}
}


/*****************************************************************************************
FUNCTION:		CMD5Checksum::Final
DETAILS:		protected
DESCRIPTION:	Implementation of main MD5 checksum algorithm; ends the checksum calculation.
RETURNS:		CString : the final hexadecimal MD5 checksum result 
ARGUMENTS:		None
NOTES:			Performs the final MD5 checksum calculation ('Update' does most of the work,
this function just finishes the calculation.) 
*****************************************************************************************/
void md5_checksum::Final_core(unsigned char* p_buf, unsigned int n_len)
{
	if (p_buf == NULL || n_len < MD5_BUF_SIZE)
		return;

	//Save number of bits
	unsigned char Bits[8];
	uint_2_uchar(Bits, mn_count, 8);

	//Pad out to 56 mod 64.
	unsigned int nIndex = (unsigned int)((mn_count[0] >> 3) & 0x3f);
	unsigned int nPadLen = (nIndex < 56) ? (56 - nIndex) : (120 - nIndex);
	Update( PADDING, nPadLen );

	//Append length (before padding)
	Update( Bits, 8 );

	uint_2_uchar(p_buf, mn_MD5, MD5_BUF_SIZE);
	return;
}

string md5_checksum::Final()
{
	unsigned char lpszMD5[MD5_BUF_SIZE];
	Final_core(lpszMD5, MD5_BUF_SIZE);
	//Convert the hexadecimal checksum to a CString
	string strMD5;
	for ( int i=0; i < MD5_BUF_SIZE; i++) 
	{
		string Str;
		if (lpszMD5[i] == 0) {
			Str = string("00");
		}
		else if (lpszMD5[i] <= 15) 	{
			char s_buf[8];
			memset(s_buf, 0x0, 8);
			sprintf(s_buf, "0%x", lpszMD5[i]);
			Str = s_buf;
		}
		else {
			char s_buf[8];
			memset(s_buf, 0x0, 8);
			sprintf(s_buf, "%x", lpszMD5[i]);
			Str = s_buf;
		}

		assert(Str.length() == 2 );
		strMD5 += Str;
	}
	assert(strMD5.length() == 32 );
	return strMD5;
}

/*****************************************************************************************
FUNCTION:		CMD5Checksum::Update
DETAILS:		protected
DESCRIPTION:	Implementation of main MD5 checksum algorithm
RETURNS:		void
ARGUMENTS:		BYTE* Input    : input block
UINT nInputLen : length of input block
NOTES:			Computes the partial MD5 checksum for 'nInputLen' bytes of data in 'Input'
*****************************************************************************************/
void md5_checksum::Update(const unsigned char* Input, unsigned int nInputLen)
{
	//Compute number of bytes mod 64
	unsigned int nIndex = (unsigned int)((mn_count[0] >> 3) & 0x3F);

	//Update number of bits
	if ((mn_count[0] += nInputLen << 3 )  <  ( nInputLen << 3) )
	{
		mn_count[1]++;
	}
	mn_count[1] += (nInputLen >> 29);

	//Transform as many times as possible.
	unsigned int i=0;		
	unsigned int nPartLen = 64 - nIndex;
	if (nInputLen >= nPartLen) 	
	{
		memcpy( &ms_buffer[nIndex], Input, nPartLen );
		Transform(ms_buffer );
		for (i = nPartLen; i + 63 < nInputLen; i += 64) 
		{
			Transform( &Input[i] );
		}
		nIndex = 0;
	} 
	else 
	{
		i = 0;
	}

	// Buffer remaining input
	memcpy( &ms_buffer[nIndex], &Input[i], nInputLen-i);
}

}		//end namespace em_utility
