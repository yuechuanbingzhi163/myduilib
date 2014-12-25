#pragma once
#include "io_interface.h"
#include <wtypes.h>

namespace em_utility
{

class win_io_base : public io_base
{
public:
	win_io_base();
    virtual ~win_io_base();
	virtual bool is_dir_exists(const TCHAR* p_dir) ;  //判断目录是否存在
	virtual int make_dir(const TCHAR* p_dir); //创建一个目录
	virtual bool is_file_exists(const TCHAR* p_name); //文件是否存在

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

}			//end namespace em_utility

