//
//  XiamiRuntime.m
//  xiamiShare
//
//  Created by go886 on 11-11-10.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#include "stdafx.h"
#include "YTingRuntime.h"
#include <memory.h>
#include <assert.h>
#include "CMemPool.h"


void* memcpyFromRing(void * dst,
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
void* memcpyToRing(const void *src,
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


int memfind(const void* src,size_t srcsize,const void* dst,size_t dstsize)
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
void* PoolMalloc(size_t nSize) {
	return __mempool.Alloc(nSize);
}
void  PoolFree(void* p) { 
	return __mempool.Free(p);
}

