#pragma once
#include <queue>
#include <vector>
#include <atlstr.h>
#include "mt_helper.h"

using namespace std;
namespace em_utility
{
class down_http_file
{
public:
	enum task_type_e
	{
		TT_NORMAL, //一般的文件下载
	};

	struct down_task
	{
		CString s_url;
		CString s_local_file_down;			//下载时的本地文件名
		CString s_local_file_normal;		//下载后更名的本地文件名（如果值为空，则下载后不需要更名）
		HWND h_notify_wnd;
		UINT n_notify_msg;
		WPARAM n_wparam;
		LPARAM n_lparam;

		task_type_e task_type;
		down_task()
		{
			task_type = TT_NORMAL;
			h_notify_wnd = NULL;
			n_notify_msg = 0;
			n_wparam = 0;
			n_lparam = 0;
		}
	};
	enum down_result_e
	{
		dr_success = 0,
		dr_open_url_failed = 1,
		dr_open_file_failed,
		dr_read_file_failed,
		dr_net_open_failed,
		dr_exit
	};
	static const TCHAR COMMON_HEADER[];
	static const unsigned int DEF_BUF_SIZE = 16384;
public:
	down_http_file();
	~down_http_file();
	bool start_thread();
	void stop_thread();
	void add_task(const down_task& task);
	down_result_e down_file_sync(const CString& s_url, const CString& s_local_file);
    bool find_task(const down_task& task);
	static down_result_e http_file_down(const CString& s_url, const CString& s_local_file);
private:
	static DWORD WINAPI thread_proc(void* p_param);
	void _thread_proc();
	static DWORD WINAPI httpfileload_proc(void* p_param);
	void _httpfileload_proc(down_task* obj);

	typedef std::deque<down_task> task_container;
	task_container m_down_list;
	HANDLE mh_thread;
	signal_event m_exit_signal;
	signal_event m_task_signal;
	em_utility::critical_session m_lock;
	bool mb_exit;
};

}
