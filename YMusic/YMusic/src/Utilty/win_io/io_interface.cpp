#include "stdafx.h"
#include "io_interface.h"

namespace em_utility
{
pos_info::pos_info(unsigned int n_begin, unsigned int n_len)
{
	mn_begin = n_begin;
	mn_len = n_len;
}

pos_info::~pos_info()
{
}

unsigned int pos_info::get_begin() const
{
	return mn_begin;
}

unsigned int pos_info::get_len() const
{
	return mn_len;
}

unsigned int pos_info::get_end() const
{
	return get_begin() + get_len();
}

void pos_info::set_info(unsigned int n_begin, unsigned int n_len)
{
	mn_begin = n_begin;
	mn_len = n_len;
}

bool pos_info::valid() const
{
	return mn_len > 0;
}

unsigned int pos_info::size() const
{
	return get_len();
}

bool pos_info::is_sub_pos(const pos_info& pos) const
{
	bool b_result = false;
	if (pos.get_begin() >= get_begin() && pos.size() <= size() && pos.get_end() <= get_end())
		b_result = true;
	return b_result;
}

bool pos_info::is_equal_pos(const pos_info& pos) const
{
	return pos.get_begin() == get_begin() && pos.size() == size();
}

}	//end namespace em_utility
