#pragma once
#include <map>
#include <set>
#include <list>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <vector>
#include <bitset>

#include "YTingRuntime.h"



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
inline void EndianConvert( int8& t) { }

class CBuffer
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
