#include "stdafx.h"
#include "down_http_file.h"
#include "win_io/win_io_base.h"

#include "../../YMusic.h"
#include <wininet.h>
#include "../YApp.h"

using namespace std;

namespace em_utility
{

const TCHAR down_http_file::COMMON_HEADER[] = _T("Accept:   */*\r\n\r\n");

struct SINGLE_TASK_PARAM
{
	down_http_file * p_obj;
	down_http_file::down_task * p_task;
	SINGLE_TASK_PARAM()
	{
		p_obj = NULL;
		p_task = NULL;
	}
};

down_http_file::down_http_file() : m_task_signal(false), m_exit_signal(false)
{
	mh_thread = NULL;
	mb_exit = false;
}

down_http_file::~down_http_file()
{
	mb_exit = true;
	m_down_list.clear();
}

bool down_http_file::find_task(const down_task& task)
{
	bool b_find = false;
   std::deque<down_task>::iterator i;
   m_lock.lock();
   for (i = m_down_list.begin(); i != m_down_list.end(); ++i)
   {
		down_task* p_cur = &*i;
		if (p_cur->s_url==task.s_url && p_cur->task_type==task.task_type)		//已经在下载请求队列中
		{
			b_find = true;
			break;
		}
	}
	m_lock.unlock();
	return b_find;
}

void down_http_file::add_task(const down_task& task)
{	
	if(find_task(task))
	{    
		return ;
	}
	int n_path=task.s_local_file_down.ReverseFind(_T('\\'));
	if (n_path < 0)			//非有效目录
	{
		n_path=task.s_local_file_down.ReverseFind(_T('/'));
	}
	if (n_path < 0)
	{
		return;
	}
	CString cs_path = task.s_local_file_down.Left(n_path);		//task.s_local_file.Left(n_path+1);//修改于2009-6-11
	static const int DRIVER_ROOT_LENGTH = 2;			//sample "d:"
	if (cs_path.GetLength() == DRIVER_ROOT_LENGTH)
		cs_path += _T("\\");
	if (!mci::is_directory(cs_path))
	{
		em_utility::mci::confirm_dir(cs_path,_T('/'));
	}

	m_lock.lock();
	m_down_list.push_back(task);
	m_lock.unlock();

	m_task_signal.set_signal();
}

down_http_file::down_result_e down_http_file::down_file_sync(const CString& s_url, const CString& s_local_file)
{
	DWORD dwFlags = 0;
	::InternetGetConnectedState(&dwFlags, 0);
	if (dwFlags & INTERNET_CONNECTION_OFFLINE)		//take appropriate steps
		return dr_net_open_failed;;
	//TCHAR s_agent[64];
	//s_agent[64]=;
	//wsprintf(s_agent, _T("Agent%ld"), timeGetTime());
	HINTERNET h_open;
	/*if (!(dwFlags & INTERNET_CONNECTION_PROXY))
		h_open = ::InternetOpen(_T("xiAMi_down_file_image"), INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	else
		h_open = ::InternetOpen(_T("xiAMi_down_file_image"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);*/
	if(mb_exit)
	{
		return dr_exit;
	}
	if (dwFlags & INTERNET_CONNECTION_PROXY)
	{
		// 如果IE浏览器中设置了使用代理服务器, 则使用代理或从注册表中读取配置.禁用脚本
		h_open = ::InternetOpen(_T("xiAMi_down_file_image"), INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	}
	else
	{
		// 直连
		h_open = ::InternetOpen(_T("xiAMi_down_file_image"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}
    //////////////////////////////////////////////////////////////////////////////////////////////
	if (h_open != NULL)					//打开internet
	{
		if(mb_exit)
		{
			InternetCloseHandle(h_open);
			return dr_exit;
		}
		HINTERNET h_connect = ::InternetOpenUrl(h_open, (LPCTSTR) s_url,  COMMON_HEADER,
			static_cast<DWORD>(_tcslen(COMMON_HEADER)), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);
		if (h_connect == NULL)
		{  
			int nError = GetLastError();
			InternetCloseHandle(h_open);
			return dr_open_url_failed;
		}   
		TCHAR   retBuf[10] = {0};		
		DWORD   bufLen   =  sizeof(retBuf);   
		BOOL b_result = HttpQueryInfo(h_connect,   HTTP_QUERY_STATUS_CODE,   retBuf,   &bufLen   ,NULL);    		
      	int   dwRtn  = _wtol(retBuf);
		if (dwRtn != HTTP_STATUS_OK)
		{	
			::InternetCloseHandle(h_connect);
			InternetCloseHandle(h_open);
			return dr_net_open_failed;	
		}
		win_io_base local_file;
		int n_result = 0;
		n_result = local_file.create(s_local_file);
		if (n_result != 0)
		{
			local_file.close();
			::InternetCloseHandle(h_connect);
			InternetCloseHandle(h_open);
			return dr_open_file_failed;
		}
		unsigned int n_all_size = 0;
		DWORD n_read_size = 0;
		void *s_buf[DEF_BUF_SIZE];   
		unsigned int n_written_bytes = 0;
		do
		{
			if(mb_exit)
			{
				n_result = local_file.destroy();
				assert(n_result == 0);
				::InternetCloseHandle(h_connect);
				InternetCloseHandle(h_open);
				return dr_exit;
			}
			if (!InternetReadFile (h_connect, s_buf, DEF_BUF_SIZE,  &n_read_size))
			{
				n_result = local_file.destroy();
				assert(n_result == 0);
				::InternetCloseHandle(h_connect);
				InternetCloseHandle(h_open);
				return dr_read_file_failed;
			}
			if (n_read_size == 0)
				break;  
			else
			{
				local_file.write(s_buf, n_read_size, &n_written_bytes);
				assert(n_written_bytes == n_read_size);
			}
			n_all_size += n_read_size;
		}
		while(true);
		local_file.flush();
		if (n_all_size > 0)
		{
			assert(local_file.get_size() == n_all_size);
		}
		local_file.close();
		::InternetCloseHandle(h_connect);
	//	theApp->GetCacheMgr()->StatSize(n_all_size);
	}
	else
	{
		return dr_net_open_failed;
	}
	InternetCloseHandle(h_open);
	return dr_success;
}
down_http_file::down_result_e down_http_file::http_file_down(const CString& s_url, const CString& s_local_file)
{
	DWORD dwFlags = 0;
	::InternetGetConnectedState(&dwFlags, 0);
	if (dwFlags & INTERNET_CONNECTION_OFFLINE)		//take appropriate steps
		return dr_net_open_failed;;
	//TCHAR s_agent[64];
	//s_agent[64]=;
	//wsprintf(s_agent, _T("Agent%ld"), timeGetTime());
	HINTERNET h_open;
	/*if (!(dwFlags & INTERNET_CONNECTION_PROXY))
		h_open = ::InternetOpen(_T("xiAMi_down_file_image"), INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	else
		h_open = ::InternetOpen(_T("xiAMi_down_file_image"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);*/
	if (dwFlags & INTERNET_CONNECTION_PROXY)
	{
		// 如果IE浏览器中设置了使用代理服务器, 则使用代理或从注册表中读取配置.禁用脚本
		h_open = ::InternetOpen(_T("xiAMi_down_file_image"), INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	}
	else
	{
		// 直连
		h_open = ::InternetOpen(_T("xiAMi_down_file_image"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	}
    //////////////////////////////////////////////////////////////////////////////////////////////
	if (h_open != NULL)					//打开internet
	{
		HINTERNET h_connect = ::InternetOpenUrl(h_open, (LPCTSTR) s_url,  COMMON_HEADER,
			static_cast<DWORD>(_tcslen(COMMON_HEADER)), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);
		if (h_connect == NULL)
		{  
			int nError = GetLastError();
			InternetCloseHandle(h_open);
			return dr_open_url_failed;
		}   
		TCHAR   retBuf[10] = {0};		
		DWORD   bufLen   =  sizeof(retBuf);   
		BOOL b_result = HttpQueryInfo(h_connect,   HTTP_QUERY_STATUS_CODE,   retBuf,   &bufLen   ,NULL);    		
      	int   dwRtn  = _wtol(retBuf);
		if (dwRtn != HTTP_STATUS_OK)
		{	
			::InternetCloseHandle(h_connect);
			InternetCloseHandle(h_open);
			return dr_net_open_failed;	
		}
		win_io_base local_file;
		int n_result = 0;
		n_result = local_file.create(s_local_file);
		if (n_result != 0)
		{
			local_file.close();
			::InternetCloseHandle(h_connect);
			InternetCloseHandle(h_open);
			return dr_open_file_failed;
		}
		unsigned int n_all_size = 0;
		DWORD n_read_size = 0;
		void *s_buf[DEF_BUF_SIZE];   
		unsigned int n_written_bytes = 0;
		do
		{
			if (!InternetReadFile (h_connect, s_buf, DEF_BUF_SIZE,  &n_read_size))
			{
				n_result = local_file.destroy();
				assert(n_result == 0);
				::InternetCloseHandle(h_connect);
				InternetCloseHandle(h_open);
				return dr_read_file_failed;
			}
			if (n_read_size == 0)
				break;  
			else
			{
				local_file.write(s_buf, n_read_size, &n_written_bytes);
				assert(n_written_bytes == n_read_size);
			}
			n_all_size += n_read_size;
		}while(true);

		local_file.flush();
		if (n_all_size > 0)
		{
			assert(local_file.get_size() == n_all_size);
		}
		local_file.close();
		::InternetCloseHandle(h_connect);
	//	theApp->GetCacheMgr()->StatSize(n_all_size);
	}
	else
	{
		return dr_net_open_failed;
	}
	InternetCloseHandle(h_open);
	return dr_success;
}
//------------------------------------------------------------------
void down_http_file::_httpfileload_proc(down_task* obj)
{

}
//------------------------------------------------------------------
DWORD WINAPI down_http_file::httpfileload_proc(void* p_param)
{
	down_task* p_task = (down_task*)p_param;
	assert(p_task);
	if(!p_task) return 0;
// 	CurlData* pdata = new CurlData();
// 	pdata->SetURL(mci::unicode_2_utf8(p_task->s_url).c_str());
// 	pdata->SetFileName(mci::unicode_2_ansi(p_task->s_local_file_down).c_str());
// 	pdata->h_notify_wnd = p_task->h_notify_wnd;
// 	pdata->n_notify_msg = p_task->n_notify_msg;
// 	pdata->n_lparam = p_task->n_lparam;
// 	pdata->n_wparam = p_task->n_wparam;
// 	pdata->bshowprogress = true;
//  
//  int e_result = HttpManagerProto::Instance()->DoHttpGet(pdata);

	down_result_e e_result = http_file_down(p_task->s_url, p_task->s_local_file_down);
 
 	if(e_result == dr_success)
	{
		switch (p_task->task_type)
		{
		case TT_NORMAL:
			{
			}
			break;
		default:
			break;
		}
		if(!p_task->s_local_file_normal.IsEmpty())
		{
			BOOL bResult = ::MoveFile(p_task->s_local_file_down,p_task->s_local_file_normal);
			bResult ? sLog("::MoveFile[%s ---> %s]",
				mci::unicode_2_ansi(p_task->s_local_file_down).c_str(),
				mci::unicode_2_ansi(p_task->s_local_file_normal).c_str()) : sLogError("::MoveFile[%s--->%s]",
				mci::unicode_2_ansi(p_task->s_local_file_down).c_str(),
				mci::unicode_2_ansi(p_task->s_local_file_normal).c_str());
		}
		if (p_task->h_notify_wnd != NULL && p_task->n_notify_msg > 0)
		{
			::PostMessage(p_task->h_notify_wnd,p_task->n_notify_msg,WPARAM(p_task->n_wparam),WP_LOAD_FINISH);
		}
	}//end if (e_result == dr_success)
	
	delete p_task;
	p_task = NULL;
//	delete param;
	return 0;
}

void down_http_file::_thread_proc()
{
	assert(m_task_signal.get_handle() != NULL);
	assert(m_exit_signal.get_handle() != NULL);
	HANDLE h_events[] = { m_task_signal.get_handle(), m_exit_signal.get_handle() };
	bool b_result = false;
	
	while (true)
	{
		DWORD n_result = WaitForMultipleObjects(sizeof(h_events) / sizeof(h_events[0]), h_events, FALSE, INFINITE);
		switch (n_result)
		{
		case WAIT_OBJECT_0 + 0:
			{
				m_task_signal.reset_signal();
			}
			break;
// 		case WAIT_OBJECT_0 +1:	
// 			mb_exit = true;
// 			break;
		}
		if (mb_exit)
			break;
		
		while (!m_down_list.empty())   
		{
			if (mb_exit)
				break;
			m_lock.lock();
			down_task task = m_down_list.front();
			m_down_list.pop_front();
			m_lock.unlock();

// 			SINGLE_TASK_PARAM * param = new SINGLE_TASK_PARAM();
// 			assert(param);
// 			param->p_obj = this;
// 			param->p_task = &task;
			
			down_task * p_task = new down_task(task);
			DWORD dwThreadId = 0;
			HANDLE thread = CreateThread(NULL, 0, httpfileload_proc, p_task, 0, &dwThreadId);
			assert(thread);
			CloseHandle(thread);

//			Sleep(10); 
/*			
			down_result_e e_result = down_file_sync(task.s_url, task.s_local_file_down);
            
			if(e_result == dr_success)
			{
				switch (task.task_type)
				{
				case TT_NORMAL:
					{
					}
					break;
				default:
					break;
				}
				if(!task.s_local_file_normal.IsEmpty())
					::MoveFile(task.s_local_file_down,task.s_local_file_normal);
				if (task.h_notify_wnd != NULL && task.n_notify_msg > 0)
				{
					::PostMessage(task.h_notify_wnd,task.n_notify_msg,WPARAM(task.n_wparam),WPARAM(task.n_lparam));
				}
			}//end if (e_result == dr_success)
*/			
		}
	}
	m_task_signal.set_signal();
	return;
}

DWORD WINAPI down_http_file::thread_proc(void* p_param)
{
	down_http_file* p_obj = (down_http_file*)p_param;
	p_obj->_thread_proc();
	return 0;
}

bool down_http_file::start_thread()
{
	assert(mh_thread == NULL);
	mh_thread = CreateThread(NULL, 0, thread_proc, this, 0, NULL);
	return mh_thread != NULL;
}

void down_http_file::stop_thread()
{
	if (mh_thread != NULL)
	{
		mb_exit = true;
		m_task_signal.reset_signal();
		m_exit_signal.set_signal();
		//m_task_signal.wait(INFINITE);
		

		WaitForSingleObject(mh_thread, INFINITE);
		CloseHandle(mh_thread);
		mh_thread = NULL;
	}
	return;
}

}		//end namespace em_utility









