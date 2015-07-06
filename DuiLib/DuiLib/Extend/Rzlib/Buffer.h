#ifndef buffer_h
#define buffer_h

#pragma once
#include <map>
#include <set>
#include <list>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <vector>
#include <bitset>
#include <cassert>
#include "CMemPool.h"
#include "RzType.h"

namespace em_utility
{

	namespace ByteConverter
	{
		template<size_t T>
		inline void convert(char *val)
		{
			std::swap(*val, *(val + T - 1));
			convert<T - 2>(val + 1);
		}

		inline bool  IsLittle_Endian() 
		{
			unsigned short t = 0x1122;
			return 0x22 == *((unsigned char *)(&t));
		}
		template<> inline void convert<0>(char *) {}
		template<> inline void convert<1>(char *) {}            // ignore central byte

		template<typename T> inline void apply(T *val)
		{
			convert<sizeof(T)>((char *)(val));
		}
	};
	template<typename T> inline void EndianConvert(T& val)
	{ 
#ifdef Big_Endian
		if (ByteConverter::IsLittle_Endian()) {
			ByteConverter::apply<T>(&val);
		}
#else
		if (!ByteConverter::IsLittle_Endian()) {
			ByteConverter::apply<T>(&val);
		}
#endif
	}


	template<typename T> 
	void EndianConvert(T*);         // will generate link error

	inline void EndianConvert(uint8& t) { }
	inline void EndianConvert(int8& t) { }

	class RZ_DLL_API CBuffer
	{
	public:
		const static size_t DEFAULT_SIZE = 1024*10;

		explicit CBuffer(const uint8 * pBuffer,
			size_t len);

		CBuffer(size_t len = DEFAULT_SIZE);
		virtual ~CBuffer(void);
	public:
		size_t              size() const;
		size_t              cnt() const;
		bool                empty() const;
		bool                full() const;
		void                clear();
		void                resize(size_t size = 0);

		size_t              rpos() const;
		size_t              wpos() const;

		size_t              rpos(size_t pos,bool advance=true);
		size_t              wpos(size_t pos,bool advance=true);

		void                SetError(bool error=true);
		bool                HasError();

		size_t              nextreadblocksize();
		size_t              nextwriteblocksize();

		int                 find(const uint8* dest,size_t n) const;
		//size_t              rfind(const uint8* dest,size_t n);

		void                swap(CBuffer& buffer);


		const uint8*        data() const;
		const uint8*        c_str() const;
		size_t              Read(uint8 *dest,const size_t cnt);
		size_t              Write(const uint8 *src,const size_t cnt);

		template<typename T>
		size_t              Read(T &dst);
		template<typename T>
		T                   Read();

		template<typename T>
		void                Write(const T &src);


		uint8*   operator[](size_t pos) const;
		bool     operator==(CBuffer& src);

		void                posShow() const;
		void                textShow() const;
		void                hexShow() const;

		//
		template<typename T>
		CBuffer& operator<<(T v); // will generate link error
		CBuffer& operator<<(int8 v);
		CBuffer& operator<<(int16 v);
		CBuffer& operator<<(int32 v);
		CBuffer& operator<<(int64 v);
		CBuffer& operator<<(uint8 v);
		CBuffer& operator<<(uint16 v);
		CBuffer& operator<<(uint32 v);
		CBuffer& operator<<(uint64 v);
		CBuffer& operator<<(bool v);
		CBuffer& operator<<(float v);
		CBuffer& operator<<(double v);
		CBuffer& operator<<(const char *str);

		CBuffer& operator<<(CBuffer &v);

		//
		template<typename T>
		CBuffer& operator>>(T &t); // will generate link error
		CBuffer& operator>>(int8 &v);
		CBuffer& operator>>(int16 &v);
		CBuffer& operator>>(int32 &v);
		CBuffer& operator>>(int64 &v);
		CBuffer& operator>>(uint8 &v);
		CBuffer& operator>>(uint16 &v);
		CBuffer& operator>>(uint32 &v);
		CBuffer& operator>>(uint64 &v);
		CBuffer& operator>>(bool &v);
		CBuffer& operator>>(float &v);
		CBuffer& operator>>(double &v);
		CBuffer& operator>>(const char *dst);
		CBuffer& operator>>(CBuffer &v);

		//


	protected:
		CBuffer(const CBuffer& src){}                        //复制构造函数（也称为拷贝构造函数）
		// ...类型转换构造函数
		CBuffer &operator =(const CBuffer &src){return  *this;}            // 等号运算符重载

		bool         isUserBuffer();
		uint8 *      getBuf(size_t size);
		void         releaseBuf(uint8 * p);
	protected:
		uint8 *             _pdata;

		size_t              _cnt;
		size_t              _size;
		size_t              _rpos;
		size_t              _wpos;

		uint8               _flag;
	};


	/*********************************************************/
	inline CBuffer& operator<<(CBuffer& buffer,const std::string &v)
	{
		buffer.Write((const uint8 *)v.c_str(), v.size() + 1);
		return buffer;
	}
	inline CBuffer& operator>>(CBuffer& buffer,std::string &v)
	{
		v.clear();
		while(!buffer.empty())
		{
			uint8 c = '\0';
			buffer>>c;
			if ('\0' == c) break;

			v += c;
		}

		return buffer;
	}

	template<class T>
	inline CBuffer& operator<<(CBuffer& buffer,std::set<T>& TSet)
	{
		buffer<<(uint32)TSet.size();
		typename std::set<T>::const_iterator itor;
		for (itor = TSet.begin();itor != TSet.end();++itor)
			buffer<<*itor;

		return buffer;
	}
	template<class T>
	inline CBuffer& operator>>(CBuffer& buffer,std::set<T>& TSet)
	{
		uint32 size = 0;
		buffer>>size;
		TSet.clear();

		while(size--)
		{
			T t;
			buffer>>t;
			TSet.insert(t);
		}

		return buffer;
	}

	template<class T>
	inline CBuffer& operator<<(CBuffer& buffer,std::vector<T>& TVector)
	{
		buffer<<(uint32)TVector.size();

		typename std::vector<T>::const_iterator itor;
		for(itor = TVector.begin();itor!=TVector.end();++itor)
			buffer<< *itor;

		return buffer;
	}
	template<class T>
	inline CBuffer& operator>>(CBuffer& buffer,std::vector<T>& TVector)
	{
		uint32 size = 0;
		buffer>>size;
		TVector.clear();

		while(size--)
		{
			T t;
			buffer>>t;
			TVector.push_back(t);
		}
		return buffer;
	}


	template<class T>
	inline CBuffer& operator<<(CBuffer& buffer,std::list<T>& TList)
	{
		buffer<<(uint32)TList.size();

		typename std::list<T>::const_iterator itor;
		for(itor = TList.begin();itor != TList.end();++itor)
			buffer<< *itor;

		return buffer;
	}

	template<class T>
	inline CBuffer& operator>>(CBuffer& buffer,std::list<T>& TList)
	{
		uint32 size = 0;
		buffer>>size;
		TList.clear();

		while(size--)
		{
			T t;
			buffer>>t;
			TList.push_back(t);
		}

		return buffer;
	}

	template<class K,class V>
	inline CBuffer& operator<<(CBuffer& buffer,std::map<K,V>& TMap)
	{
		buffer<<(uint32)TMap.size();

		typename std::map<K,V>::const_iterator itor;
		for(itor = TMap.begin();itor!=TMap.end();++itor)
			buffer
			<<itor->first
			<<itor->second;

		return buffer;
	}

	template<class K,class V>
	inline CBuffer& operator>>(CBuffer& buffer,std::map<K,V>& TMap)
	{
		uint32 size = 0;
		buffer>>size;
		TMap.clear();

		while(size--)
		{
			K k;
			V v;
			buffer>>k>>v;
			TMap.insert(make_pair(k,v));
		}

		return buffer;
	}
	//////////////////////////////////////////////
	inline void* memcpyFromRing(void * dst,
		const size_t cnt,
		const void * src,
		size_t b,
		size_t size) {

			assert(cnt <= size);
			assert(dst);
			assert(src);
			assert(cnt > 0);
			assert(size > 0);
			assert(b < size);

			char * pdst = (char *)dst;
			char * psrc = (char *)src;

			if((b + cnt) <= size) {
				memcpy(pdst, &psrc[b], cnt);
				return dst;
			}

			size_t l = size - b;
			memcpy(pdst, &psrc[b], l);
			memcpy((pdst + l), src, cnt -l);
			return dst;
	}
	inline void* memcpyToRing(const void *src,
		const size_t cnt,
		void * dst,
		size_t b,
		size_t size) {
			assert(cnt <= size);
			assert(dst);
			assert(src);
			assert(cnt > 0);
			assert(size > 0);
			assert(b < size);

			char * pdst = (char *)dst;
			char * psrc = (char *)src;

			if ((b + cnt) <= size) {
				memcpy(&pdst[b], psrc, cnt);
				return dst;
			}

			size_t l = size - b;
			memcpy(&pdst[b], psrc, l);
			memcpy(pdst,(psrc + l), cnt - l);
			return dst;
	}


	inline int memfind(const void* src,size_t srcsize,const void* dst,size_t dstsize)
	{
		assert(src);
		assert(dst);
		assert(srcsize);
		assert(dstsize);
		if(!src || !dst) return -1;
		if(!srcsize || !dstsize) return -1;

		char* psrc = (char*)src;
		char* pdst = (char*)dst;
		size_t k = dstsize;
		size_t n = srcsize;
		while(n-- >= k)
		{
			if(*psrc++ != *pdst++)
			{
				pdst = (char*)dst;
				k    = dstsize;
			}else
			{
				--k;
				if(0 == k)
					return (srcsize - n - dstsize);
			}
		}
		return -1;
	}
	static CMemPool<16,16384> __mempool;
	inline void* PoolMalloc(size_t nSize) {
		return __mempool.Alloc(nSize);
	}
	inline void  PoolFree(void* p) { 
		return __mempool.Free(p);
	}

	static const uint8 buffer_flag_isuser        = 1<<1;
	static const uint8 buffer_flag_iserror       = 1<<2;

	CBuffer::CBuffer(const uint8 * pBuffer,size_t len)
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
	const uint8* CBuffer::data() const
	{
		return _pdata;
	}
	const uint8* CBuffer::c_str() const
	{
		return _pdata;
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

	CBuffer& CBuffer::operator>>(const char *dst)
	{
		uint8* p = (uint8*)dst;
		Read(p,_cnt);
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

}
#endif//buffer_h