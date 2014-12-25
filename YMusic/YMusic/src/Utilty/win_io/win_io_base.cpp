#include "stdafx.h"
#include "win_io_base.h"
//#include "All.h"
//#include "WinFileIO.h"
#include <windows.h>
//#include "CharacterHelper.h"
#include <cassert>

namespace em_utility
{
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

bool win_io_base::is_dir_exists(const TCHAR* p_dir)
{
	assert(p_dir != NULL);
	bool b_result = false;
	WIN32_FIND_DATA wfd;
	memset(&wfd, 0x0, sizeof(wfd));
	HANDLE hFind = FindFirstFile(p_dir, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		b_result = true;   
	}
	if (hFind != INVALID_HANDLE_VALUE)
		FindClose(hFind);
	return b_result;
}

int win_io_base::make_dir(const TCHAR* p_dir)
{
	if(is_dir_exists(p_dir))
		return 0;
// 	SECURITY_ATTRIBUTES sa;
// 	SECURITY_DESCRIPTOR sd;
// 
// 	InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
// 	SetSecurityDescriptorDacl(&sd,TRUE,NULL,FALSE);
// 	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
// 	sa.bInheritHandle = TRUE;
// 	sa.lpSecurityDescriptor = &sd;
// 
// 	assert(CreateDirectory(p_dir,&sa));

	return _wmkdir(p_dir);
}

bool win_io_base::is_file_exists(const TCHAR* p_name)
{
	assert(p_name);
	if (p_name != NULL && wcslen(p_name) > 0)
	{
		struct _stat32 stat_info;
		return _wstat32(p_name, &stat_info) == 0;
	}
	else
		return false;
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
	_tcscpy_s(ms_file_name, MAX_PATH, p_name);		//?????ļ???
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
	//??????whileΪ?˽???ʲô?IO?????
    while ((n_bytes_left > 0) && (*p_bytes_read > 0) && b_result)		//??????????ʣ?? && ?ϴζ?>0 && ?ϴζ?????
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
	BOOL b_result = ::SetEndOfFile(mh_file);			//???????ض??ļ?,????????,ֱ??Ӱ??Ӳ???ļ?
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
	//?????ļ?
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
	//ɾ???ļ?
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

}		//end namespace em_utility
