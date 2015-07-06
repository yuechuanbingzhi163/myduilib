#ifndef win_io_base_h
#define win_io_base_h

#pragma once
#ifdef	WIN32
	#include <Windows.h>
	#include <cassert>
#endif


#include "io_interface.h"
#include <wtypes.h>

namespace em_utility
{

	class RZ_DLL_API win_io_base : public io_base
	{
	public:
		win_io_base();
		virtual ~win_io_base();
		virtual int open(const TCHAR* p_name, bool b_readonly);
		virtual int close();
		virtual int read(void* p_buffer, unsigned int n_bytes_2_read, unsigned int* p_bytes_read);
		virtual int write(const void* p_buffer, unsigned int n_bytes_2_write, unsigned int* p_bytes_written);
		virtual int seek(int n_distance, unsigned int n_move_mode);
		virtual int set_EOF();
		virtual int create(const TCHAR* p_name);
		virtual int destroy();
		virtual int get_position();
		virtual int get_size();
		virtual int get_name(TCHAR* p_buffer);
		virtual int flush();
		bool is_open();
	private:
		HANDLE	mh_file;
		TCHAR		ms_file_name[MAX_PATH];
		bool			mb_read_only;
	};
	////
	win_io_base::win_io_base()
	{
		mh_file = INVALID_HANDLE_VALUE;
		memset(ms_file_name, 0x0, MAX_PATH * sizeof(TCHAR));
		mb_read_only = false;
	}

	win_io_base::~win_io_base()
	{
		close();
	}

	int win_io_base::open(const TCHAR* p_name, bool b_readonly)
	{
		assert(p_name != NULL);
		assert(_tcslen(p_name) < MAX_PATH);
		close();
		mb_read_only = b_readonly;
		if (mb_read_only)
			mh_file = ::CreateFile(p_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		else
			mh_file = ::CreateFile(p_name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (mh_file == INVALID_HANDLE_VALUE) 
			return io_base::NORMAL_FAILED;
		_tcscpy_s(ms_file_name, MAX_PATH, p_name);		//保存文件名
		return io_base::NORMAL_SUCCESS;
	}

	int win_io_base::close()
	{
		if (mh_file != INVALID_HANDLE_VALUE)
		{
			BOOL bret = ::CloseHandle(mh_file);
			assert(bret);
			mh_file = INVALID_HANDLE_VALUE;
		}
		return io_base::NORMAL_SUCCESS;
	}

	int win_io_base::read(void* p_buffer, unsigned int n_bytes_2_read, unsigned int* p_bytes_read)
	{
		unsigned int n_total_bytes_read = 0;
		int n_bytes_left = n_bytes_2_read;
		BOOL b_result = TRUE;
		unsigned char* p_buf = (unsigned char*)p_buffer;

		*p_bytes_read = 1;
		//这里的while为了解决什么?IO堵塞?
		while ((n_bytes_left > 0) && (*p_bytes_read > 0) && b_result)		//缓冲区仍有剩余 && 上次读>0 && 上次读正常
		{
			b_result = ::ReadFile(mh_file, &p_buf[n_bytes_2_read - n_bytes_left], n_bytes_left, (unsigned long*)p_bytes_read, NULL);
			if (b_result == TRUE)
			{
				n_bytes_left -= *p_bytes_read;
				n_total_bytes_read += *p_bytes_read;
			}
		}
		*p_bytes_read = n_total_bytes_read;
		return b_result == FALSE ? io_base::NORMAL_FAILED : io_base::NORMAL_SUCCESS;
	}

	int win_io_base::write(const void* p_buffer, unsigned int n_bytes_2_write, unsigned int* p_bytes_written)
	{
		BOOL b_result = ::WriteFile(mh_file, p_buffer, n_bytes_2_write, (unsigned long*)p_bytes_written, NULL);
		if ((b_result == FALSE) || (*p_bytes_written != n_bytes_2_write))
			return io_base::NORMAL_FAILED;
		else
			return io_base::NORMAL_SUCCESS;
	}

	int win_io_base::seek(int n_distance, unsigned int n_move_mode)
	{
		::SetFilePointer(mh_file, n_distance, NULL, n_move_mode);
		return io_base::NORMAL_SUCCESS;
	}

	int win_io_base::set_EOF()
	{
		BOOL b_result = ::SetEndOfFile(mh_file);			//增长或截断文件,物理操作,直接影响硬盘文件
		return b_result == FALSE ? io_base::NORMAL_FAILED : io_base::NORMAL_SUCCESS;
	}

	int win_io_base::get_position()
	{
		return static_cast<int>(::SetFilePointer(mh_file, 0, NULL, FILE_CURRENT));
	}

	int win_io_base::get_size()
	{
		return static_cast<int>(::GetFileSize(mh_file, NULL));
	}

	int win_io_base::get_name(TCHAR* p_buffer)
	{
		_tcscpy_s(p_buffer, MAX_PATH, ms_file_name);
		return io_base::NORMAL_SUCCESS;
	}

	int win_io_base::create(const TCHAR* p_name)
	{
		close();
		//创建文件
		mh_file = ::CreateFile(p_name, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (mh_file == INVALID_HANDLE_VALUE) 
			return io_base::NORMAL_FAILED;
		mb_read_only = false;
		_tcscpy_s(ms_file_name, p_name);
		return io_base::NORMAL_SUCCESS;
	}

	int win_io_base::destroy()
	{
		close();
		//删除文件
		return ::DeleteFile(ms_file_name) ? io_base::NORMAL_SUCCESS : io_base::NORMAL_FAILED;
	}

	int win_io_base::flush()
	{
		return ::FlushFileBuffers(mh_file) ? io_base::NORMAL_SUCCESS : io_base::NORMAL_FAILED;
	}

	bool win_io_base::is_open()
	{
		return mh_file != INVALID_HANDLE_VALUE;
	}
}//end namespace em_utility

#endif//win_io_base_h