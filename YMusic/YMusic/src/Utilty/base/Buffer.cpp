#include "stdafx.h"
#include <algorithm>
#include "Buffer.h"
#include <cassert>
#include "YTingRuntime.h"

const uint8 buffer_flag_isuser        = 1<<1;
const uint8 buffer_flag_iserror       = 1<<2;

CBuffer::CBuffer(const uint8 * pBuffer,
                 size_t len)
:_flag(buffer_flag_isuser),
_rpos(0),
_wpos(0),
_size(len),
_cnt(len),
_pdata((uint8 *)pBuffer)
{
	assert(pBuffer);
    assert(len >0);
}
CBuffer::CBuffer(size_t len /* = DEFAULT_SIZE */)
:_flag(0),
_rpos(0),
_wpos(0),
_size(len),
_cnt(0),
_pdata(NULL)
{
	if (0 >= _size) _size = DEFAULT_SIZE;
    
	_pdata    = getBuf(_size);
    memset(_pdata, 0, _size);
}
CBuffer::~CBuffer(void)
{
	if (!isUserBuffer() && _pdata)
	{
		releaseBuf(_pdata);
		_pdata = NULL;
	}
}

bool CBuffer::isUserBuffer() {
    return (_flag&buffer_flag_isuser) > 0;
}
uint8 * CBuffer::getBuf(size_t size)
{
    assert(size > 0);
    return (uint8 *)PoolMalloc(size);
}
void    CBuffer::releaseBuf(uint8 * p) 
{
    assert(NULL != p);
    PoolFree(p);
}

size_t  CBuffer::size() const
{
    return _size;
}
size_t CBuffer::cnt() const
{
    return _cnt;
}
bool   CBuffer::full() const 
{
    return _size == _cnt;
}

bool   CBuffer::empty() const
{
    return 0 == _cnt;
}
size_t  CBuffer::rpos() const
{
    return _rpos;
}
size_t   CBuffer::wpos() const
{
    return _wpos;
}
size_t  CBuffer::rpos(size_t pos,bool advance /*=true*/)
{
	assert(!(_rpos == _wpos && pos == _rpos));
    _rpos = (pos%_size);
    if(_rpos == _wpos) {
        _cnt = advance ? 0 : size();
    }else {
        _cnt = (_rpos > _wpos) ? (size() - (_rpos - _wpos)) : (_wpos - _rpos);
    }
    return _rpos;
}
size_t  CBuffer::wpos(size_t pos,bool advance/*=true*/)
{
	assert(!(_rpos == _wpos && pos == _wpos));
    _wpos = (pos%_size);
    if(_wpos == _rpos) {
        _cnt = advance ? size() : 0;
    }else {
        _cnt = (_rpos > _wpos) ? (size() -(_rpos - _wpos)) : (_wpos - _rpos);
    }
    return _wpos;
}
void   CBuffer::clear() 
{
    _rpos = 0;
    _wpos = 0;
    _cnt  = 0;
}
void   CBuffer::resize(size_t size /*= 0*/) {
    if(size <= cnt()) size = 2*_size;
    if(0 == size) size = DEFAULT_SIZE;
   // if(size == _size) return;
    
    printf("oldsize:%ld,newsize:%ld\n",_size,size);
    uint8 * pnew = getBuf(size);
	assert(pnew);
    //memset(pnew, 0, size);
    
    if(_cnt > 0) memcpyFromRing(pnew, _cnt, _pdata, _rpos, _size);
    
    if (!isUserBuffer() && _pdata)
        releaseBuf(_pdata);
    
    _pdata   = pnew;
    _rpos    = 0;
    _wpos    = _cnt;
    _size    = size;
    
    _flag &= ~buffer_flag_isuser;
}

size_t  CBuffer::nextreadblocksize() {
    if(empty()) return 0;
    return (_wpos > _rpos) ? _cnt : (_size - _rpos);
}
size_t  CBuffer::nextwriteblocksize() {
    if(full()) return 0;
    return (_wpos >= _rpos) ? (_size - _wpos) : (_rpos - _wpos);
}
int CBuffer::find(const uint8* dest,size_t n) const
{
	assert(dest);
	assert(n);
	if(!dest || 0>= n) return -1;
	if(cnt() < n) return -1;

	
	size_t pos = rpos();
	char* pdst = (char*)dest;
	size_t k = n;
	size_t j = _cnt;
	while(j-- >= k)
	{
		if(*((*this)[pos++]) != *pdst++)
		{
			pdst = (char*)dest;
			k    = n;
		}else
		{
			--k;
			if(0 == k)
				return (pos - n)%_size;
		}
	}
	return -1;
}

/*
size_t   CBuffer::nextread(uint8** r) {
    assert(r);
    if(empty() || NULL == r) return 0;
    
    *r = &_pdata[_rpos];
    return (_wpos > _rpos) ? cnt() : (_size - _rpos);
    
}
size_t   CBuffer::nextwrite(uint8** w) {
    assert(w);
    if(full() || NULL == w) return 0;
    
    *w = &_pdata[_wpos];
    return (_wpos >= _rpos) ? (_size - _wpos) : (_rpos - _wpos);
}

size_t CBuffer::ReadBlockdata(uint8** dest,const size_t cnt) {
    assert(dest);
    assert(cnt > 0);
    if (0 >= cnt || NULL == dest) return 0;
    if (this->cnt() > cnt) return 0; 
    
    if(_wpos >= _rpos) 
    {
        *dest = &_pdata[_rpos];
        _cnt -= cnt;
        return cnt;
    }
    if((_rpos + cnt) <= this->size()) 
    {
        *dest = &_pdata[_rpos];
        _cnt -= cnt;
        return cnt;
    }
    return 0;
}
*/

void CBuffer::swap(CBuffer& buffer)
{
	std::swap(this->_pdata,buffer._pdata);
	std::swap(this->_cnt,buffer._cnt);
	std::swap(this->_size,buffer._size);
	std::swap(this->_rpos,buffer._rpos);
	std::swap(this->_wpos,buffer._wpos);
	std::swap(this->_flag,buffer._flag);
}
size_t CBuffer::Read(uint8 *dest,const size_t cnt)
{
	assert(dest);
    assert(cnt >0);
    if(HasError()) return 0;
	if (empty() || NULL == dest || _cnt < cnt) {
        SetError();
        return 0;
    }
    
    memcpyFromRing(dest, cnt, _pdata, _rpos, _size);
    _rpos = (_rpos + cnt)%_size;
    
    
    _cnt -= cnt;
    assert(_cnt >= 0);
    assert(_cnt <= _size);
    return cnt;
}
size_t   CBuffer::Write(const uint8 *src,const size_t cnt) 
{
	assert(src);
    assert(cnt >0);
	if(NULL == src || 0 >= cnt) return 0;
   
  if (cnt > (_size - _cnt)) {
	  const size_t a = 2*(cnt + _cnt);
	  const size_t b = 2*_size;
      const size_t newsize = (a > b ? a : b);
      this->resize(newsize);
    }
 
    memcpyToRing(src, cnt, _pdata, _wpos, _size);
    _wpos = (_wpos + cnt)%_size;
    
    _cnt += cnt;
    assert(_cnt >= 0);
    assert(_cnt <= _size);
    return cnt;
}
void    CBuffer::SetError(bool error/*=true*/) {
    error ? (_flag |= buffer_flag_iserror) : (_flag &= ~buffer_flag_iserror);
}
bool    CBuffer::HasError() {
    return (_flag&buffer_flag_iserror) > 0;
}

bool CBuffer::operator==(CBuffer& src) 
{
    if(this == &src) 
        return true;
	if(cnt() != src.cnt())
		return false;
    
    for (size_t i = _rpos; i <_cnt+_rpos; ++i) {
        if(*(*this)[i] != *src[i])
            return false;
    }
    
    return true;
}
uint8* CBuffer::operator[](size_t pos) const {
    return &_pdata[pos%_size];
}

/************************************************************************/

template<typename T>
inline size_t CBuffer::Read(T &dst)
{
	size_t len = Read((uint8 *)&dst,sizeof(T));
	EndianConvert(dst);
	return len;
}
template<typename T>
inline T CBuffer::Read()
{
	T v;
	Read<T>(v);
	return v;
}
template<typename T>
inline void CBuffer::Write(const T &src)
{
    EndianConvert(src);
	Write((uint8 *)&src,sizeof(T));
}

//////////////////////////////////////////////////////////////////////////
CBuffer& CBuffer::operator<<(int8 v)
{
	Write<int8>(v);
	return *this;
}
CBuffer& CBuffer::operator<<(int16 v)
{
	Write<int16>(v);
	return *this;
}
CBuffer& CBuffer::operator<<(int32 v)
{
	Write<int32>(v);
	return *this;
}
CBuffer& CBuffer::operator<<(int64 v)
{
	Write<int64>(v);
	return *this;
}
CBuffer& CBuffer::operator<<(uint8 v)
{
	Write<uint8>(v);
	return *this;
}
CBuffer& CBuffer::operator<<(uint16 v)
{
	Write<uint16>(v);
	return *this;
}
CBuffer& CBuffer::operator<<(uint32 v)
{
	Write<uint32>(v);
	return *this;
}
CBuffer& CBuffer::operator<<(uint64 v)
{
	Write<uint64>(v);
	return *this;
}
CBuffer& CBuffer::operator<<(bool v)
{
	Write<bool>(v);
	return *this;
}
CBuffer& CBuffer::operator<<(float v)
{
	Write<float>(v);
	return *this;
}
CBuffer& CBuffer::operator<<(double v)
{
	Write<double>(v);
	return *this;
}
CBuffer& CBuffer::operator<<(const char *str)
{
	Write((uint8 const *)str,str ? strlen(str) : 0);
	return *this;
}

CBuffer& CBuffer::operator<<(CBuffer &v)
{
    if(v.empty()) return *this;
    
    size_t pos = v.rpos();
    while (!v.empty()) {
        uint8 * src = v[v.rpos()];
        size_t  cnt = v.nextreadblocksize();
        this->Write(src, cnt);
        v.rpos(v.rpos() + cnt,true);
    }
    v.rpos(pos,false);
	return *this;
}
//////////////////////////////////////////////////////////////////////////
CBuffer& CBuffer::operator>>(int8 &v)
{
	Read<int8>(v);
	return *this;
}
CBuffer& CBuffer::operator>>(int16 &v)
{
	Read<int16>(v);
	return *this;
}
CBuffer& CBuffer::operator>>(int32 &v)
{
	Read<int32>(v);
	return *this;
}
CBuffer& CBuffer::operator>>(int64 &v)
{
	Read<int64>(v);
	return *this;
}
CBuffer& CBuffer::operator>>(uint8 &v)
{
	Read<uint8>(v);
	return *this;
}
CBuffer& CBuffer::operator>>(uint16 &v)
{
	Read<uint16>(v);
	return *this;
}
CBuffer& CBuffer::operator>>(uint32 &v)
{
	Read<uint32>(v);
	return *this;
}
CBuffer& CBuffer::operator>>(uint64 &v)
{
	Read<uint64>(v);
	return *this;
}
CBuffer& CBuffer::operator>>(bool &v)
{
	Read<bool>(v);
	return *this;
}
CBuffer& CBuffer::operator>>(float &v)
{
	Read<float>(v);
	return *this;
}
CBuffer& CBuffer::operator>>(double &v)
{
	Read<double>(v);
	return *this;
}

CBuffer& CBuffer::operator>>(CBuffer &v)
{
	v<<*this;
	return *this;
}

void CBuffer::posShow() const
{
    printf("size:%ld,cnt:%ld,rpos:%ld,wpos:%ld\n",
           _size,
           _cnt,
           _rpos,
           _wpos);
}


void CBuffer::textShow() const
{
    posShow();
    size_t pos = _rpos;
    
    for (size_t i=0; i<_cnt; ++i) {
		char c = *(char*)((*this)[pos++]);
		switch (c)
		{
		case '\r':
			printf("/r\t");
			break;
		case '\n':
			printf("/n\t");
			break;
		case '\t':
			printf("/t\t");
			break;
		case '\0':
			printf("/0\t");
			break;
		default:
			printf("%c\t",c);
		}
        pos %= _size;
    }
	printf("\n");

}
void CBuffer::hexShow() const
{
	posShow();
	size_t pos = _rpos;

	for (size_t i=0; i<_cnt; ++i) {
		printf("%X\t",_pdata[pos++]);
		pos %= _size;
	}
	printf("\n");
}