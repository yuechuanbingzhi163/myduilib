/********************************************************************
    FileName :  RzMemPool.h   
    Version  :  0.10
    Date     :	2010-3-9 17:11:48
    Author   :  小家伙
    Comment  :

*********************************************************************/
#ifndef  __RZMEMPOOL_H__
#define  __RZMEMPOOL_H__
#include <assert.h>
#include "RzDReadAWriteLock.h"
#include <iostream>
_RzUsing(std)
/*****************************************************************************************************
  本类为内存池类，以链表形式管理，实则栈式，从链表头处插入与移除，
  同时类本身也为一个链表的节点;m_pNext记录下一个链表
  类中任何大小内存都可以成功申请，但只存放小于MaxSaveSize的内存块，其它直接删除

******************************************************************************************************/
_RzCFunBegin
_RzNameSpaceBegin(RzStd)
typedef struct _Node_
{
	unsigned int nLen;
	struct _Node_* next;
}Node;
const unsigned int NodeSize = sizeof(Node);
const unsigned int MaxSaveBlock =1024*1024*1;  //定义大于1M内存时内存池不管理

class RZ_DLL_API CRzMemPool
{
public:
	CRzMemPool(const unsigned int& nDataSize,const unsigned int& nMaxSize=0):m_nBlockSize(nDataSize/*+NodeSize*/),
		m_nMaxSize(nMaxSize),m_pFirst(NULL),m_pNext(NULL),m_nInCount(0),m_nOutConut(0){};  
	~CRzMemPool()
	{
		m_Lock.EnterWrite();   
		{
			if (m_pFirst) DeleteNext(m_pFirst);
			m_pFirst = NULL;
			if (m_pNext) delete m_pNext;//注意 实际是递归调用
			m_pNext = NULL;
		}
		m_Lock.LeaveWrite();
		//cout<<"内存池，块大小："<<m_nBlockSize<<"存个数："<<m_nInCount<<"抛出个数："<<m_nOutConut<<endl;
	};
public:
	void* GetBuf(const unsigned int &nSize)                    //按指定大小从内存池中获取内存（nSize）
	{
		void* pResutl = NULL;
		Node* pNewNode =NULL;

		if ( (nSize+NodeSize) <= m_nBlockSize)  //内存块大小 > （用户请求内存大小 + 链表指针大小)
		{		                               //表示本对象的大小可以满足申请需求，申请从本链表中获取
			m_Lock.EnterWrite();
			{
				if (m_pFirst)                  //链表非空，从链表获取节点
				{                             //注意不考虑空节点情况，因为空表时，说明表中没有内存，则应该检查是否可以满足申请条件，如不满足直接反回NULL					  				
					pNewNode = m_pFirst;
					m_pFirst = m_pFirst->next;
					pNewNode->next = NULL;				
					--m_nInCount;
					++m_nOutConut;
					pResutl = (char*)pNewNode + NodeSize;  //把用户使用内存后移，因为内存中前面NodeSize大小的内存存放了Node中的记录
				}
				else if (0 == m_nMaxSize || m_nOutConut < m_nMaxSize)
				{
					pNewNode = (Node*)malloc(m_nBlockSize);
					if (pNewNode)
					{					
						pNewNode->nLen = m_nBlockSize;       //结点中记录本内存块大小，方便释放时回归到本链表中
						pNewNode->next = NULL;				 
						pResutl = (char*)pNewNode + NodeSize;//把用户使用内存后移，因为内存中前面NodeSize大小的内存存放了Node中的记录
						++m_nOutConut;	
					}
				}
			}
			m_Lock.LeaveWrite();
		}
		else   //非本链表管理，请求下个连表处理
		{
			m_Lock.EnterWrite();
			if (!m_pNext)
				m_pNext = new CRzMemPool(2*(m_nBlockSize/*-NodeSize*/),m_nMaxSize); //注意为倍数申请
			if(m_pNext)
				pResutl = m_pNext->GetBuf(nSize);
			m_Lock.LeaveWrite();
		}	
		return pResutl;
	};
	bool FreeBuf(void* pBuf,bool bDelete)                      //释放内存，bDelete 指示是否真正删除内存	
	{
		if (!pBuf) return false;
		bool bResult = false;
		Node* pNode = GetNode(pBuf);
		if(!pNode) return false;

		if (m_nBlockSize == pNode->nLen)   //本链表管理内存
		{
			if (m_nBlockSize >= MaxSaveBlock || bDelete) //满足直接删除条件，则直接删除
			{
				free(pNode);
				--m_nOutConut;
			}
			else                                        //存入链表    
			{
				m_Lock.EnterWrite();//加锁，写访问             
				{
					pNode->next =  m_pFirst;
					m_pFirst = pNode;
				}
				m_Lock.LeaveWrite();
				--m_nOutConut;
				++m_nInCount;
			}
			bResult = true;   //注意这里直接返回True ,因为操作肯定成功
		}
		else //非本链表管理
		{
			m_Lock.EnterRead();   //加读锁，对m_pNext读访问
			{
				if (m_pNext)			
					bResult = m_pNext->FreeBuf(pBuf,bDelete);			
			}
			m_Lock.LeaveRead();
		}

		return bResult;
	}
	bool IsIn(const void* pBuf,const unsigned int &nNewSize)
	{
		assert(pBuf);
		if (0 >= nNewSize) return false;
		if ( (CRzMemPool::GetNode(pBuf)->nLen - NodeSize) >= nNewSize )
			return true;
		else 
			return false;
	}
	static unsigned int GetBufLen(const void* pBuf)
	{
		return GetNode(pBuf)->nLen;
	};
	void PrintInfo()
	{
		//暂不实现
	};
private:
	void DeleteNext(Node* pNext)                      //递归删除链表中的节点
	{
		//本函数类私有，类中其它函数调用（加锁），所以此处不用加锁
		if( !pNext) return;
		Node* pNode = pNext;
		Node* pTmpNode = NULL;
		while(1)
		{
			pTmpNode = pNode->next;
			free(pNode);
			--m_nInCount;
			if (!pTmpNode) break;
			pNode = pTmpNode;
		}
	}
	static Node* GetNode(const void* pBuf)
	{                               //静态函数不用加锁
		assert(pBuf);
		Node* pTmpNode = (Node*)((char*)pBuf-NodeSize);
		return pTmpNode;
	};
private:
	const unsigned int  m_nBlockSize;                  //内存块大小  以16K为初使大小，以倍数递增(常量防止修改)
	const unsigned int  m_nMaxSize;                    //链表管理的最内存块最大个数
	DWORD               m_nOutConut;                   //抛出的内存块个数
	DWORD               m_nInCount;                    //内存池中存储的内存块个数
	Node*         m_pFirst;                            //此连表的头节点
	CRzDReadAWriteLock m_Lock;                         //多读单定锁保护变量
	CRzMemPool*   m_pNext;                             //本链表的下一个链表指针
};

class RZ_DLL_API CRzMemPoolMgr
{
public:
	CRzMemPoolMgr(unsigned int nMinSize = 8,const unsigned int nMaxSize = 0)
	{
		m_pHead = new CRzMemPool((nMinSize<16 ? nMinSize =16:nMinSize),nMaxSize) ; //以最小内存块4bytes 初使管理连表头；
	};
	~CRzMemPoolMgr()
	{
		if (m_pHead)
		{
			delete m_pHead;
			m_pHead = NULL;
		}
	};
public:
	void* GetBuf(unsigned int &nSize)
	{
		if (0 >= nSize) return NULL;

		void* pResult = NULL;
		if (m_pHead)
		{
			pResult = m_pHead->GetBuf(nSize);
		}
		return pResult;
	};                                             
	void* ReGetBuf(void* pOldBuf,unsigned int& nNewSize,bool bCopyOldData = true)
	{
		assert(pOldBuf);

		void* pResutl = NULL;
		if (0 >= nNewSize)
		{
			return pResutl;
		}	
		if (m_pHead->IsIn(pOldBuf,nNewSize))
		{
			pResutl = pOldBuf;
		}
		else
		{
			pResutl = m_pHead->GetBuf(nNewSize);
			if (pResutl && pOldBuf)
			{
				if(bCopyOldData)
					memcpy(pResutl,pOldBuf,CRzMemPool::GetBufLen(pOldBuf));
			}
		}
		return pResutl;
	}; 
	bool  Free(void* pBuf,bool bDelete = false)
	{
		if (!pBuf) return false;
		if(m_pHead)
			m_pHead->FreeBuf(pBuf,bDelete);
		return true;
	};
private:
	CRzMemPool* m_pHead;      //管理链表头结点
};
_RzNameSpaceEnd
_RzCFunEnd
#endif
